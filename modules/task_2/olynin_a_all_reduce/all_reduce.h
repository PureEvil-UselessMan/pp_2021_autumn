// Copyright 2021 Olynin Alexandr

#ifndef MODULES_TASK_2_OLYNIN_A_ALL_REDUSE_ALL_REDUSE_H_
#define MODULES_TASK_2_OLYNIN_A_ALL_REDUSE_ALL_REDUSE_H_

template <typename T>
int CalculateRecvBufferValue(void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

int My_MPI_Allreduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

template <typename T>
std::vector<T> GetRandomVector(int n, int max);

#endif  // MODULES_TASK_2_OLYNIN_A_ALL_REDUSE_ALL_REDUSE_H_
