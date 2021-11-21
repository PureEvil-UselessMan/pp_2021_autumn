// Copyright 2021 Olynin Alexandr
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./all_reduce.h"

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_sum_test_for_int_vector) {

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());

    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_max_test_for_int_vector) {

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());

    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_min_test_for_int_vector) {

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());

    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_prod_test_for_int_vector) {

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());

    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_sum_test_for_float_vector) {

    std::vector<float> send_vec = GetRandomVector<float>(1000, 100);
    std::vector<float> actual_vec(send_vec.size());
    std::vector<float> expected_vec(send_vec.size());
    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_Computing_Tests, Mpi_max_test_for_double_vector) {

    std::vector<double> send_vec = GetRandomVector<double>(1000, 100);
    std::vector<double> actual_vec(send_vec.size());
    std::vector<double> expected_vec(send_vec.size());

    My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(send_vec.data(), expected_vec.data(), send_vec.size(), MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    ASSERT_TRUE(expected_vec == actual_vec);
}

TEST(My_MPI_All_Reduse_No_Error_Tests, Mpi_correct_test_for_int_vector) {

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());
    int actual_status = My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    int expected_status = MPI_SUCCESS;
    ASSERT_EQ(actual_status, expected_status);
}

TEST(My_MPI_All_Reduse_Error_Tests, Mpi_incorrect_type_test_for_float_vector) {

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> send_vec = GetRandomVector<int>(1000, 100);
    std::vector<int> actual_vec(send_vec.size());
    std::vector<int> expected_vec(send_vec.size());
    int actual_status = My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_CHARACTER, MPI_SUM, MPI_COMM_WORLD);

    if (ProcRank == 0) {
        int expected_status = MPI_ERR_TYPE;
        ASSERT_EQ(actual_status, expected_status);
    }
}

TEST(My_MPI_All_Reduse_Error_Tests, Mpi_incorrect_unsupported_operation_test_for_double_vector) {

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<double> send_vec = GetRandomVector<double>(1000, 100);
    std::vector<double> actual_vec(send_vec.size());
    std::vector<double> expected_vec(send_vec.size());
    int actual_status = My_MPI_Allreduce(send_vec.data(), actual_vec.data(), send_vec.size(), MPI_DOUBLE, MPI_LAND, MPI_COMM_WORLD);

    if (ProcRank == 0) {
        int expected_status = MPI_ERR_UNSUPPORTED_OPERATION;
        ASSERT_EQ(actual_status, expected_status);
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
