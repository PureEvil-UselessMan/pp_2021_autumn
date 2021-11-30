// Copyright 2021 Olynin Alexandr
#include <mpi.h>
#include <random>
#include <ctime>
#include "../../../modules/task_3/olynin_a_radix_sort_odd_even_merge/radix_sort_odd_even_merge.h"

std::vector<int> RadixSort(std::vector<int> main_data, int n) {
    std::vector<int> sorted_data[10];
    int koef = 0;
    int max_power = GetMaxPower(main_data);
    while (koef < max_power) {
        for (int i = 0; i < n; i++) {
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

int GetMaxPower(std::vector<int> data) {
    int max_power = 0;
    int size = data.size();
    while (size > 0) {
        max_power++;
        for (size_t i = 0; i < data.size(); i++) {
            int div = data[i] / powf(10, max_power);
            if (div == 0)
                size--;
        }
    }

    return max_power;
}

std::vector<int> Merge(std::vector<int> first, std::vector<int> second) {
    int nf = first.size();
    int ns = second.size();
    std::vector<int> result(nf + ns);
    if (nf == 0 && ns == 0)
        return result;

    if (nf < ns)
        return Merge(second, first);

    first.push_back(abs(first[nf - 1] + 1));
    if (ns > 0)
        first[nf] += abs(second[ns - 1]);

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

std::vector<int> OddEvenMerge(std::vector<int> first, std::vector<int> second) {
    std::vector<int> first_odd_tmp = first;
    std::vector<int> second_odd_tmp = second;
    std::vector<int> first_even_tmp = first;
    std::vector<int> second_even_tmp = second;

    EvenSplitter(&first_even_tmp, &second_even_tmp);
    OddSplitter(&first_odd_tmp, &second_odd_tmp);

    first = Merge(first_even_tmp, second_even_tmp);
    second = Merge(first_odd_tmp, second_odd_tmp);

    return GetResult(first, second);
}

void EvenSplitter(std::vector<int>* first, std::vector<int>* second) {
    std::vector<int> even_tmp_buf_1;
    std::vector<int> even_tmp_buf_2;
    for (size_t i = 0; i < first->size(); i += 2)
        even_tmp_buf_1.push_back((*first)[i]);

    for (size_t i = 0; i < second->size(); i += 2)
        even_tmp_buf_2.push_back((*second)[i]);

    *first = even_tmp_buf_1;
    *second = even_tmp_buf_2;
}

void OddSplitter(std::vector<int>* first, std::vector<int>* second) {
    std::vector<int> odd_tmp_buf_1;
    std::vector<int> odd_tmp_buf_2;
    for (size_t i = 1; i < first->size(); i += 2)
        odd_tmp_buf_1.push_back((*first)[i]);

    for (size_t i = 1; i < second->size(); i += 2)
        odd_tmp_buf_2.push_back((*second)[i]);

    *first = odd_tmp_buf_1;
    *second = odd_tmp_buf_2;
}

std::vector<int> GetResult(std::vector<int> first, std::vector<int> second) {
    std::vector<int> res;
    for (size_t i = 0; i < first.size() || i < second.size(); i++) {
        if (i < first.size())
            res.push_back(first[i]);

        if (i < second.size())
            res.push_back(second[i]);
    }

    for (size_t i = 1; i < first.size() + second.size() - 1; i += 2) {
        if (res[i] > res[i + 1])
            std::swap(res[i], res[i + 1]);
    }

    return res;
}

std::vector<int> ParallelRadixSortWithOddEvenMerge(std::vector<int> data_root) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> send_counts(ProcNum);
    std::vector<int> offset(ProcNum);

    if (ProcRank == 0) {
        int sum = 0;
        size_t remains = data_root.size() % ProcNum;
        for (int i = 0; i < ProcNum; i++) {
            send_counts[i] = data_root.size() / ProcNum;
            if (remains > 0) {
                send_counts[i]++;
                remains--;
            }
            offset[i] = sum;
            sum += send_counts[i];
        }
    }

    MPI_Bcast(send_counts.data(), ProcNum, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(offset.data(), ProcNum, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> data_local(send_counts[ProcRank]);
    MPI_Scatterv(data_root.data(), send_counts.data(), offset.data(),
                 MPI_INT, data_local.data(), send_counts[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    data_local = RadixSort(data_local, data_local.size());

    int shift_rank = ProcRank;
    int shift_num = ProcNum;
    bool flag = true;
    int count = 1;
    int odd = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    while (floor(shift_num / 2)) {
        if (shift_num % 2 == 1 && flag) {
            if (ProcRank == ProcNum - 2 * count - odd)
                data_local = EvenCountProcess(data_local, ProcNum - count - odd);

            if (ProcRank == ProcNum - count - odd) {
                OddCountProcess(data_local, ProcNum - 2 * count - odd);
                flag = false;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (shift_rank % 2 == 0 && flag)
            data_local = EvenCountProcess(data_local, ProcRank + count);

        if (shift_rank % 2 == 1 && flag) {
            OddCountProcess(data_local, ProcRank - count);
            flag = false;
        }
        shift_rank = shift_rank / 2;
        if (shift_num % 2 == 1)
            odd += count;

        shift_num = floor(shift_num / 2);
        count *= 2;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (flag == false) {
        data_local.clear();
        return data_local;
    }

    return data_local;
}

std::vector<int> EvenCountProcess(std::vector<int> data_local, int partner) {
    std::vector<int> first = data_local;
    int second_size;
    MPI_Recv(&second_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int> second(second_size);
    MPI_Recv(second.data(), second_size, MPI_INT, partner, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int first_size = first.size();
    MPI_Send(&first_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
    MPI_Send(first.data(), first.size(), MPI_INT, partner, 1, MPI_COMM_WORLD);

    EvenSplitter(&first, &second);

    first = Merge(first, second);

    MPI_Recv(&second_size, 1, MPI_INT, partner, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    second.resize(second_size);
    MPI_Recv(second.data(), second_size, MPI_INT, partner, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    data_local.resize(first.size() + second.size());

    return GetResult(first, second);
}

void OddCountProcess(std::vector<int> second, int partner) {
    int second_size = second.size();

    MPI_Send(&second_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
    MPI_Send(second.data(), second.size(), MPI_INT, partner, 1, MPI_COMM_WORLD);

    int first_size;
    MPI_Recv(&first_size, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int> first(first_size);
    MPI_Recv(first.data(), first_size, MPI_INT, partner, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    OddSplitter(&first, &second);

    second = Merge(first, second);
    second_size = second.size();
    MPI_Send(&second_size, 1, MPI_INT, partner, 2, MPI_COMM_WORLD);
    MPI_Send(second.data(), second.size(), MPI_INT, partner, 3, MPI_COMM_WORLD);
}

std::vector<int> GetRandomData(int amount, int dist) {
    std::vector<int> rand_data(amount);
    std::mt19937 gen(time(0));
    for (int i = 0; i < amount; i++)
        rand_data[i] = gen() % dist;

    return rand_data;
}
