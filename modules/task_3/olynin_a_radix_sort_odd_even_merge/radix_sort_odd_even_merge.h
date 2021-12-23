// Copyright 2021 Olynin Alexandr
#include <vector>

#ifndef MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_
#define MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_

std::vector<int> RadixSort(std::vector<int> main_data);

std::vector<int> Merge(std::vector<int> first, std::vector<int> second);
std::vector<int> OddEvenMerge(std::vector<int> first, std::vector<int> second, int start, int end, int size);


std::vector<int> ParallelRadixSortWithOddEvenMerge(std::vector<int> data_root);
std::vector<int> EvenProcess(std::vector<int> first, int partner);
void OddProcess(std::vector<int> second, int partner);

std::vector<int> GetRandomData(int amount, int dist);

#endif  // MODULES_TASK_3_OLYNIN_A_RADIX_SORT_ODD_EVEN_MERGE_RADIX_SORT_ODD_EVEN_MERGE_H_
