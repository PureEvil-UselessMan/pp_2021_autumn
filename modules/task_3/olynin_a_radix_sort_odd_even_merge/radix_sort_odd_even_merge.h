// Copyright 2021 Olynin Alexandr
#include <vector>

#ifndef MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_
#define MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_

std::vector<int> RadixSort(std::vector<int> arr, int n);
int GetMaxPower(std::vector<int> data);
std::vector<int> Merge(std::vector<int> first, std::vector<int> second);
std::vector<int> OddEvenMerge(std::vector<int> first, std::vector<int> second);

void EvenSplitter(std::vector<int> &first, std::vector<int> &second);
void OddSplitter(std::vector<int> &first, std::vector<int> &second);
std::vector<int> GetResult(std::vector<int> first, std::vector<int> second);

std::vector<int> ParallelRadixSortWithOddEvenMerge(std::vector<int> root_arr);
std::vector<int> EvenCountProcess(std::vector<int> local_arr, int partner);
void OddCountProcess(std::vector<int> local_arr, int partner);

std::vector<int> GetRandomData(int amount, int dist);

#endif  // MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_
