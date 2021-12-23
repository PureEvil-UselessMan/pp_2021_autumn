// Copyright 2021 Olynin Alexandr
#include <mpi.h>
#include <random>
#include <ctime>
#include "../../../modules/task_3/olynin_a_radix_sort_odd_even_merge/radix_sort_odd_even_merge.h"

std::vector<int> RadixSort(std::vector<int> main_data) {
    std::vector<int> sorted_data[10];

    int max = main_data[0];
    for (size_t i = 1; i < main_data.size(); i++) {
        if (main_data[i] > max) {
            max = main_data[i];
        }
    }

    int max_power = 0;
    while (max / powf(10, max_power) != 0) {
        max_power++;
    }

    int koef = 0;
    while (koef < max_power) {
        for (size_t i = 0; i < main_data.size(); i++) {
            int digit = main_data[i] / pow(10, koef);
            digit = digit % 10;
            sorted_data[digit].push_back(main_data[i]);
        }

        main_data.clear();
        for (int i = 0; i < 10; i++) {
            for (size_t j = 0; j < sorted_data[i].size(); j++) {
                main_data.push_back(sorted_data[i][j]);
            }
            sorted_data[i].clear();
        }
        koef++;
    }

    return main_data;
}

std::vector<int> Merge(std::vector<int> first, std::vector<int> second) {
    int nf = first.size();
    int ns = second.size();
    std::vector<int> result(nf + ns);
    if (nf == 0 && ns == 0) {
        return result;
    }

    if (nf < ns) {
        return Merge(second, first);
    }

    first.push_back(abs(first[nf - 1] + 1));
    if (ns > 0) {
        first[nf] += abs(second[ns - 1]);
    }

    second.push_back(first[nf]);
    int count = 0;
    int i = 0;
    int j = 0;
    while (count < nf + ns) {
        if (first[i] < second[j]) {
            result[count] = first[i++];
            count++;
        } else {
            result[count] = second[j++];
            count++;
        }
    }

    return result;
}

std::vector<int> OddEvenMerge(std::vector<int> first, std::vector<int> second, int start, int end, int size) {
    int nf = end;
    int ns = second.size();
    std::vector<int> result(size);
    int i = start;
    int j = 0;
    int count = 0;
    while (i < nf && j < ns) {
        if (first[i] < second[j]) {
            result[count++] = first[i++];
        } else {
            result[count++] = second[j++];
        }
    }
    while (i < nf) {
        result[count++] = first[i++];
    }
    while (j < ns) {
        result[count++] = second[j++];
    }

    return result;
}

std::vector<int> ParallelRadixSortWithOddEvenMerge(std::vector<int> data_root) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int send_count;
    int remain;
    if (ProcRank == 0) {
        send_count = data_root.size() / ProcNum;
        remain = data_root.size() % ProcNum;
    }

    MPI_Bcast(&send_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> data_local(send_count);
    MPI_Scatter(data_root.data(), send_count, MPI_INT,
                data_local.data(), send_count, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0 && remain != 0) {
        data_local.insert(data_local.end(), data_root.end() - remain, data_root.end());
    }
    int merge_count = ceil(logf(ProcNum) / logf(2));

    data_local = RadixSort(data_local);

    std::vector<int> data_copy = data_local;
    size_t size = data_local.size();
    if (ProcNum > 1) {
        for (size_t i = 0; i < size; i++) {
            data_local[i / 2 + (i % 2) * (size / 2 + size % 2)] = data_copy[i];
        }
    }

    int counter = 1;
    for (int i = 1; i <= merge_count; i++) {
        if (ProcRank % (2*counter) == 0 && ProcRank + counter < ProcNum) {
            data_local = EvenProcess(data_local, ProcRank + counter);
            if (i != merge_count) {
                data_copy = data_local;
                size = data_local.size();
                for (size_t i = 0; i < size; i++) {
                    data_local[i / 2 + (i % 2) * (size / 2 + size % 2)] = data_copy[i];
                }
            }
        }
        if ((ProcRank - counter) % (2*counter) == 0) {
            OddProcess(data_local, ProcRank - counter);
        }
        counter *= 2;
    }

    return data_local;
}

std::vector<int> EvenProcess(std::vector<int> first, int partner) {
    int first_size = first.size() / 2;
    int second_size;

    MPI_Sendrecv(&first_size, 1, MPI_INT, partner, 0,
                 &second_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::vector<int> second(second_size / 2 + second_size % 2);
    MPI_Sendrecv(&first[first_size + first.size() % 2], first_size, MPI_INT, partner, 1,
                 second.data(), second_size / 2 + second_size % 2, MPI_INT, partner, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::vector<int> even = OddEvenMerge(first, second, 0, first_size + first.size() % 2,
                       first_size + first.size() % 2 + second_size / 2 + second_size % 2);

    std::vector<int> odd(first_size + second_size / 2);
    MPI_Recv(odd.data(), first_size + second_size / 2, MPI_INT, partner,
             0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    first = Merge(even, odd);

    return first;
}

void OddProcess(std::vector<int> second, int partner) {
    int second_size = second.size();
    int first_size;
    MPI_Sendrecv(&second_size, 1, MPI_INT, partner, 0,
                 &first_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int> first(first_size);
    MPI_Sendrecv(second.data(), second_size / 2 + second_size % 2, MPI_INT, partner, 1,
                 first.data(), first_size, MPI_INT, partner, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::vector<int> odd = OddEvenMerge(second, first, second_size / 2 + second_size % 2,
                                        second_size, second_size / 2 + first_size);

    MPI_Send(odd.data(), odd.size(), MPI_INT, partner, 0, MPI_COMM_WORLD);
}

std::vector<int> GetRandomData(int amount, int dist) {
    std::vector<int> rand_data(amount);
    std::mt19937 gen(time(0));
    for (int i = 0; i < amount; i++) {
        rand_data[i] = gen() % dist;
    }

    return rand_data;
}
