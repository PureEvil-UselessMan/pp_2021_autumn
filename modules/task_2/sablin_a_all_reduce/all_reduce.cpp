  // Copyright 2021 Sablin Alexander

#include <mpi.h>
#include <random>
#include "../../../modules/task_2/sablin_a_all_reduce/all_reduce.h"

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype type,
    MPI_Op Op, MPI_Comm Comm) {
    int FuncNum, FuncRank;
    int root;
    MPI_Comm_size(MPI_COMM_WORLD, &FuncNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &FuncRank);
    if (FuncRank == 0) {
        //  do
        //  {
        //    std::cout << "Enter a root: " << std::endl;
        //    std::cin >> root;
        //  } while ((root >= FuncNum) || (root < 0));

        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % FuncNum;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (FuncRank == root) {
        int type_num;
        if (type == MPI_INT) {
            type_num = sizeof(int);
        } else if (type == MPI_DOUBLE) {
            type_num = sizeof(double);
        } else {
            type_num = sizeof(float);
        }
        void* buf = new char[type_num * count];
        void* tmp = new char[type_num * count];

        int counter = 0;
        if (type == MPI_INT) {
            for (int iter = 0; iter < FuncNum; iter++) {
                if (iter != root) {
                    MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
                } else {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<int*>(buf))[i] = (reinterpret_cast<int*>(sendbuf))[i];
                    }
                }
                if (counter == 0) {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<int*>(tmp))[i] = (reinterpret_cast<int*>(buf))[i];
                    }
                    counter++;
                } else {
                    if (Op == MPI_SUM) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<int*>(tmp))[i] += (reinterpret_cast<int*>(buf))[i];
                        }
                    } else if (Op == MPI_MAX) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<int*>(buf))[i] > (reinterpret_cast<int*>(tmp))[i]) {
                                (reinterpret_cast<int*>(tmp))[i] = (reinterpret_cast<int*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_MIN) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<int*>(buf))[i] < (reinterpret_cast<int*>(tmp))[i]) {
                                (reinterpret_cast<int*>(tmp))[i] = (reinterpret_cast<int*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_PROD) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<int*>(tmp))[i] *= (reinterpret_cast<int*>(buf))[i];
                        }
                    }
                }
            }
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<int*>(recvbuf))[i] = (reinterpret_cast<int*>(tmp))[i];
            }
            delete[] reinterpret_cast<int*>(buf);
            delete[] reinterpret_cast<int*>(tmp);
        } else if (type == MPI_DOUBLE) {
            for (int iter = 0; iter < FuncNum; iter++) {
                if (iter != root) {
                    MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
                } else {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<double*>(buf))[i] = (reinterpret_cast<double*>(sendbuf))[i];
                    }
                }
                if (counter == 0) {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<double*>(tmp))[i] = (reinterpret_cast<double*>(buf))[i];
                    }
                    counter++;
                } else {
                    if (Op == MPI_SUM) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<double*>(tmp))[i] += (reinterpret_cast<double*>(buf))[i];
                        }
                    } else if (Op == MPI_MAX) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<double*>(buf))[i] > (reinterpret_cast<double*>(tmp)[i])) {
                                (reinterpret_cast<double*>(tmp))[i] = (reinterpret_cast<double*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_MIN) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<double*>(buf))[i] < (reinterpret_cast<double*>(tmp))[i]) {
                                (reinterpret_cast<double*>(tmp))[i] = (reinterpret_cast<double*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_PROD) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<double*>(tmp))[i] *= (reinterpret_cast<double*>(buf))[i];
                        }
                    }
                }
            }
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<double*>(recvbuf))[i] = (reinterpret_cast<double*>(tmp))[i];
            }
            delete[] reinterpret_cast<double*>(buf);
            delete[] reinterpret_cast<double*>(tmp);
        } else {
            for (int iter = 0; iter < FuncNum; iter++) {
                if (iter != root) {
                    MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
                } else {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<float*>(buf))[i] = (reinterpret_cast<float*>(sendbuf))[i];
                    }
                }
                if (counter == 0) {
                    for (int i = 0; i < count; i++) {
                        (reinterpret_cast<float*>(tmp))[i] = (reinterpret_cast<float*>(buf))[i];
                    }
                    counter++;
                } else {
                    if (Op == MPI_SUM) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<float*>(tmp))[i] += (reinterpret_cast<float*>(buf))[i];
                        }
                    } else if (Op == MPI_MAX) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<float*>(buf))[i] > (reinterpret_cast<float*>(tmp))[i]) {
                                (reinterpret_cast<float*>(tmp))[i] = (reinterpret_cast<float*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_MIN) {
                        for (int i = 0; i < count; i++) {
                            if ((reinterpret_cast<float*>(buf))[i] < (reinterpret_cast<float*>(tmp)[i])) {
                                (reinterpret_cast<float*>(tmp))[i] = (reinterpret_cast<float*>(buf))[i];
                            }
                        }
                    } else if (Op == MPI_PROD) {
                        for (int i = 0; i < count; i++) {
                            (reinterpret_cast<float*>(tmp))[i] *= (reinterpret_cast<float*>(buf))[i];
                        }
                    }
                }
            }
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<float*>(recvbuf))[i] = (reinterpret_cast<float*>(tmp))[i];
            }
            delete[] reinterpret_cast<float*>(buf);
            delete[] reinterpret_cast<float*>(tmp);
        }
        // delete[] buf;
        // delete[] tmp;
    } else {
        MPI_Send(sendbuf, count, type, root, 1, Comm);
        MPI_Recv(recvbuf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
    }
    int dif = FuncRank - root;
    if (dif < 0) {
        dif = FuncNum + dif;
    }
    int ui = ceil(logf(dif + 1) / logf(2));
    for (int i = ui; pow(2, i) + dif < FuncNum; i++) {
        int num = pow(2, i) + dif + root;
        if (num >= FuncNum) {
            num -= FuncNum;
        }
        MPI_Send(recvbuf, count, type, num, 1, Comm);
    }
    return 0;
}

template<typename T>
T* getRandomVector(T* rand_vec, int n, int dia, bool flag) {
    rand_vec = new T[n];
    if (flag) {
        std::random_device dev;
        std::mt19937 gen(dev());
        for (int i = 0; i < n; i++) {
            rand_vec[i] = static_cast<T>(gen() % dia);
        }
    }
    return rand_vec;
}

template
int* getRandomVector<int>(int* rand_vec, int n, int dia, bool flag);

template
double* getRandomVector<double>(double* rand_vec, int n, int dia, bool flag);

template
float* getRandomVector<float>(float* rand_vec, int n, int dia, bool flag);
