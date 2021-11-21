// Copyright 2021 Olynin Alexandr
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include "../../../modules/task_2/olynin_a_all_reduce/all_reduce.h"

template <typename T>
int CalculateRecvBufferValue(void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm) {
    std::vector<T> buf_root;
    std::vector<T> tmp_buf(count);

    for (int i = 0; i < count; i++) {
        buf_root.push_back(static_cast<T*>(sendbuf)[i]);
    }

    int RecvCount;
    MPI_Comm_size(Comm, &RecvCount);

    while (--RecvCount) {
        MPI_Recv(tmp_buf.data(), count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
        if (Op == MPI_SUM) {
            for (int i = 0; i < buf_root.size(); i++) {
                buf_root[i] += tmp_buf[i];
            }
        } else if (Op == MPI_MAX) {
            for (int i = 0; i < buf_root.size(); i++) {
                if (buf_root[i] < tmp_buf[i]) {
                    buf_root[i] = tmp_buf[i];
                }
            }
        } else if (Op == MPI_MIN) {
            for (int i = 0; i < buf_root.size(); i++) {
                if (buf_root[i] > tmp_buf[i]) {
                    buf_root[i] = tmp_buf[i];
                }
            }
        } else if (Op == MPI_PROD) {
            for (int i = 0; i < buf_root.size(); i++) {
                buf_root[i] *= tmp_buf[i];
            }
        } else {
            return MPI_ERR_UNSUPPORTED_OPERATION;
        }
    }

    tmp_buf.clear();
    for (int i = 0; i < count; i++) {
        (static_cast<T*>(recvbuf))[i] = buf_root[i];
    }

    buf_root.clear();
    return MPI_SUCCESS;
}

int My_MPI_Allreduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm) {
    int ProcNum, ProcRank;
    MPI_Comm_size(Comm, &ProcNum);
    MPI_Comm_rank(Comm, &ProcRank);

    int root;
    if (ProcRank == 0) {
        std::mt19937 gen(time(0));
        root = gen() % ProcNum;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, Comm);

    int status = MPI_SUCCESS;
    if (ProcRank == root) {
        if (type == MPI_INT) {
            status = CalculateRecvBufferValue<int>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else if (type == MPI_FLOAT) {
            status = CalculateRecvBufferValue<float>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else if (type == MPI_DOUBLE) {
            status = CalculateRecvBufferValue<double>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else {
            status = MPI_ERR_TYPE;
        }
    } else {
        MPI_Send(sendbuf, count, type, root, 1, Comm);
        MPI_Recv(recvbuf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
        MPI_Recv(&status, 1, MPI_INT, MPI_ANY_SOURCE, 2, Comm, MPI_STATUS_IGNORE);
    }

    int shift = ((ProcNum - root) + ProcRank) % ProcNum;
    int begin = ceil(logf(shift + 1) / logf(2));
    // printf("ProcRank = %d ", ProcRank);
    for (int i = begin; powf(2, i) + shift < ProcNum; i++) {
        int send_rank = (static_cast<int>(powf(2, i)) + ProcRank) % ProcNum;
        // printf("send_rank = %d ", send_rank);
        MPI_Send(recvbuf, count, type, send_rank, 1, Comm);
        MPI_Send(&status, 1, MPI_INT, send_rank, 2, Comm);
    }

    return status;
}

template <typename T>
std::vector<T> GetRandomVector(int n, int max) {
    std::vector<T> rand_vec(n);
    std::mt19937 gen(time(0));
    for (int i = 0; i < n; i++) {
        rand_vec[i] = gen() % max;
    }

    return rand_vec;
}
template
std::vector<int> GetRandomVector(int n, int max);
template
std::vector<float> GetRandomVector(int n, int max);
template
std::vector<double> GetRandomVector(int n, int max);
