// Copyright 2021 Olynin Alexandr
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./radix_sort_odd_even_merge.h"
#include <iostream>

TEST(Accessory_Functions_Tests, RadixSort_test) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::vector<int> data;
        int amount = 100;
        data = GetRandomData(amount, 1000);
        std::vector<int> expected_data = data;
        std::vector<int> actual_data = data;
        actual_data = RadixSort(actual_data);
        sort(expected_data.begin(), expected_data.end());
        ASSERT_TRUE(expected_data == actual_data);
    }
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
}

TEST(Parallel_Radix_Sort_Tests, Parallel_radix_sort_small_test) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double begin;
    double end;
    std::vector<int> data;
    const int amount = 1000;
    if (ProcRank == 0) {
        data = GetRandomData(amount, 1000);
        begin = MPI_Wtime();
    }

    std::vector<int> actual_data = ParallelRadixSortWithOddEvenMerge(data);

    if (ProcRank == 0) {
        end = MPI_Wtime();
        double parallel_time = end - begin;
        std::cout <<  "parallel_time = " << std::fixed << parallel_time << std::endl;
        std::vector<int> expected_data = data;
        begin = MPI_Wtime();
        expected_data = RadixSort(expected_data);
        end = MPI_Wtime();
        double sequential_time = end - begin;
        std::cout << "sequential_time = " << std::fixed << sequential_time << std::endl;
        std::cout << "performance improvement times = " << std::fixed
                  <<  sequential_time / parallel_time << std::endl;
        ASSERT_TRUE(expected_data == actual_data);
    }
}

TEST(Parallel_Radix_Sort_Tests, Parallel_radix_sort_medium_test) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double begin;
    double end;
    std::vector<int> data;
    const int amount = 10000;
    if (ProcRank == 0) {
        data = GetRandomData(amount, 10000);
        begin = MPI_Wtime();
    }

    std::vector<int> actual_data = ParallelRadixSortWithOddEvenMerge(data);

    if (ProcRank == 0) {
        end = MPI_Wtime();
        double parallel_time = end - begin;
        std::cout <<  "parallel_time = " << std::fixed << parallel_time << std::endl;
        std::vector<int> expected_data = data;
        begin = MPI_Wtime();
        expected_data = RadixSort(expected_data);
        end = MPI_Wtime();
        double sequential_time = end - begin;
        std::cout << "sequential_time = " << std::fixed << sequential_time << std::endl;
        std::cout << "performance improvement times = " << std::fixed
                  <<  sequential_time / parallel_time << std::endl;
        ASSERT_TRUE(expected_data == actual_data);
    }
}

TEST(Parallel_Radix_Sort_Tests, Parallel_radix_sort_large_test) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double begin;
    double end;
    std::vector<int> data;
    const int amount = 100000;
    if (ProcRank == 0) {
        data = GetRandomData(amount, 100000);
        begin = MPI_Wtime();
    }

    std::vector<int> actual_data = ParallelRadixSortWithOddEvenMerge(data);

    if (ProcRank == 0) {
        end = MPI_Wtime();
        double parallel_time = end - begin;
        std::cout <<  "parallel_time = " << std::fixed << parallel_time << std::endl;
        std::vector<int> expected_data = data;
        begin = MPI_Wtime();
        expected_data = RadixSort(expected_data);
        end = MPI_Wtime();
        double sequential_time = end - begin;
        std::cout << "sequential_time = " << std::fixed << sequential_time << std::endl;
        std::cout << "performance improvement times = " << std::fixed
                  <<  sequential_time / parallel_time << std::endl;
        ASSERT_TRUE(expected_data == actual_data);
    }
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
