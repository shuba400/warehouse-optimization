@echo off
g++ testcasegenerator.cpp -o testcasegenerator
testcasegenerator

g++ dp_Merge_sort_time.cpp -o dp_Merge_sort_time
g++ dp_Merge_reverse_sort_time.cpp -o dp_Merge_reverse_sort_time
g++ dp_Merge_sort_size.cpp -o dp_Merge_sort_size
g++ dp_Merge_reverse_sort_size.cpp -o dp_Merge_reverse_sort_size
g++ dp_Sorted.cpp -o dp_Sorted

dp_Merge_sort_time
dp_Merge_reverse_sort_time
dp_Merge_sort_size
dp_Merge_reverse_sort_size
dp_Sorted