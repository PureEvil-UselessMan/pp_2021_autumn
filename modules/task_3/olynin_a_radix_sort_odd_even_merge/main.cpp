// Copyright 2021 Olynin Alexandr
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./radix_sort_odd_even_merge.h"

TEST(Accessory_Functions_Tests, RadixSort_test) {

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::vector<int> data;
        int amount = 100;
        data = GetRandomData(amount, 1000);
        std::vector<int> actual_data = RadixSort(data, data.size());
        sort(data.begin(), data.end());
        ASSERT_TRUE(data == actual_data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Accessory_Functions_Tests, Merge_test) {

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::vector<int> data_first = {2, 8, 9, 20, 42, 129};
        std::vector<int> data_second = {0, 10, 42, 43, 88, 90, 120};
        std::vector<int> expected_data = {0, 2, 8, 9, 10, 20, 42, 42, 43, 88, 90, 120, 129};
        std::vector<int> actual_data = Merge(data_first, data_second);
        ASSERT_TRUE(expected_data == actual_data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Accessory_Functions_Tests, Sequential_odd_even_merge_test) {

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::vector<int> data_first = {2, 8, 9, 20, 42, 129};
        std::vector<int> data_second = {0, 10, 42, 43, 88, 90, 120};
        std::vector<int> expected_data = {0, 2, 8, 9, 10, 20, 42, 42, 43, 88, 90, 120, 129};
        std::vector<int> actual_data = OddEvenMerge(data_first, data_second);
        ASSERT_TRUE(expected_data == actual_data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Radix_Sort_Tests, Parallel_radix_sort_small_test) {

    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> data;
    if (ProcRank == 0) {
        int amount = 1000;
        if (ProcNum <= amount)
            data = GetRandomData(amount, 100);
        else
            data = GetRandomData(ProcNum, 100);
    }

    std::vector<int> actual_data = ParallelRadixSortWithOddEvenMerge(data);

    if (ProcRank == 0) {
        std::vector<int> expected_data = RadixSort(data, data.size());
        ASSERT_TRUE(expected_data == actual_data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Radix_Sort_Tests, Parallel_radix_sort_large_test) {

    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> data;
    if (ProcRank == 0) {
        int amount = 100;
        if (ProcNum <= amount)
            data = GetRandomData(amount, 1000);
        else
            data = GetRandomData(ProcNum, 1000);
    }

    std::vector<int> actual_data = ParallelRadixSortWithOddEvenMerge(data);

    if (ProcRank == 0) {
        std::vector<int> expected_data = RadixSort(data, data.size());
        ASSERT_TRUE(expected_data == actual_data);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
