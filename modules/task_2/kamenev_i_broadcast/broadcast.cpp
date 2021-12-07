// Copyright 2021 Kamenev Ilya

#include <mpi.h>
#include <random>
#include <vector>
#include "../../../modules/task_2/kamenev_i_broadcast/broadcast.h"

int randint(int min, int max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  return dist(rng);
}

template <typename T>
T* getRandomArray(int n, int max) {
  T* rand_arr = new T[n];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < n; i++) {
    rand_arr[i] = static_cast<T>(gen() % max);
  }
  return rand_arr;
}

template int* getRandomArray(int n, int max);

template double* getRandomArray(int n, int max);

template float* getRandomArray(int n, int max);

int Bcast(void* buffer, void* outbuf, int count, MPI_Datatype datatype,
          int root, MPI_Op op, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != root) {
        MPI_Send(buffer, count * size, datatype, i, 0, comm);
      }
    }
  }
  if (datatype == MPI_INT) {
    int* recvbuf = nullptr;
    if (rank != root) {
      recvbuf = new int[count * size];
      MPI_Recv(recvbuf, count * size, datatype, root, 0, comm,
               MPI_STATUSES_IGNORE);
    } else {
      recvbuf = reinterpret_cast<int*>(buffer);
    }
    int partial_out = 0;
    if (op == MPI_SUM) {
      for (int i = 0; i < count; i++) {
        partial_out += recvbuf[rank * count + i];
      }
    } else if (op == MPI_MAX) {
      partial_out = INT32_MIN;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] > partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_MIN) {
      partial_out = INT32_MAX;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] < partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_PROD) {
      partial_out = 1;
      for (int i = 0; i < count; i++) {
        partial_out *= recvbuf[rank * count + i];
      }
    }
    if (rank != root) {
      MPI_Send(&partial_out, 1, datatype, root, 1, comm);
    }
    if (rank == root) {
      int global_out = partial_out;
      for (int i = 0; i < size; i++) {
        if (i != root) {
          int partial_recv = 0;
          MPI_Recv(&partial_recv, count * size, datatype, root, 1, comm,
                   MPI_STATUSES_IGNORE);
          if (op == MPI_SUM) {
            global_out += partial_recv;
          } else if (op == MPI_MAX) {
            if (partial_recv > global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_MIN) {
            if (partial_recv < global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_PROD) {
            global_out *= partial_recv;
          }
        }
      }
      *reinterpret_cast<int*>(outbuf) = global_out;
    }
  } else if (datatype == MPI_DOUBLE) {
    double* recvbuf;
    if (rank != root) {
      recvbuf = new double[count * size];
      MPI_Recv(recvbuf, count * size, datatype, root, 0, comm,
               MPI_STATUSES_IGNORE);
    } else {
      recvbuf = reinterpret_cast<double*>(buffer);
    }
    double partial_out = 0;
    if (op == MPI_SUM) {
      for (int i = 0; i < count; i++) {
        partial_out += recvbuf[rank * count + i];
      }
    } else if (op == MPI_MAX) {
      partial_out = DBL_MIN;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] > partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_MIN) {
      partial_out = DBL_MAX;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] < partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_PROD) {
      partial_out = 1;
      for (int i = 0; i < count; i++) {
        partial_out *= recvbuf[rank * count + i];
      }
    }
    if (rank != root) {
      MPI_Send(&partial_out, 1, datatype, root, 1, comm);
    }
    if (rank == root) {
      double global_out = partial_out;
      for (int i = 0; i < size; i++) {
        if (i != root) {
          double partial_recv = 0;
          MPI_Recv(&partial_recv, count * size, datatype, root, 1, comm,
                   MPI_STATUSES_IGNORE);
          if (op == MPI_SUM) {
            global_out += partial_recv;
          } else if (op == MPI_MAX) {
            if (partial_recv > global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_MIN) {
            if (partial_recv < global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_PROD) {
            global_out *= partial_recv;
          }
        }
      }
      *reinterpret_cast<double*>(outbuf) = global_out;
    }
  } else if (datatype == MPI_FLOAT) {
    float* recvbuf;
    if (rank != root) {
      recvbuf = new float[count * size];
      MPI_Recv(recvbuf, count * size, datatype, root, 0, comm,
               MPI_STATUSES_IGNORE);
    } else {
      recvbuf = reinterpret_cast<float*>(buffer);
    }
    float partial_out = 0;
    if (op == MPI_SUM) {
      for (int i = 0; i < count; i++) {
        partial_out += recvbuf[rank * count + i];
      }
    } else if (op == MPI_MAX) {
      partial_out = FLT_MIN;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] > partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_MIN) {
      partial_out = FLT_MAX;
      for (int i = 0; i < count; i++) {
        if (recvbuf[rank * count + i] < partial_out) {
          partial_out = recvbuf[rank * count + i];
        }
      }
    } else if (op == MPI_PROD) {
      partial_out = 1;
      for (int i = 0; i < count; i++) {
        partial_out *= recvbuf[rank * count + i];
      }
    }
    if (rank != root) {
      MPI_Send(&partial_out, 1, datatype, root, 1, comm);
    }
    if (rank == root) {
      float global_out = partial_out;
      for (int i = 0; i < size; i++) {
        if (i != root) {
          float partial_recv;
          MPI_Recv(&partial_recv, count * size, datatype, root, 1, comm,
                   MPI_STATUSES_IGNORE);
          if (op == MPI_SUM) {
            global_out += partial_recv;
          } else if (op == MPI_MAX) {
            if (partial_recv > global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_MIN) {
            if (partial_recv < global_out) {
              global_out = partial_recv;
            }
          } else if (op == MPI_PROD) {
            global_out *= partial_recv;
          }
        }
      }
      *reinterpret_cast<float*>(outbuf) = global_out;
    }
  }
  return 0;
}
