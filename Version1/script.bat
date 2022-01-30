@echo off
g++ testcasegenerator.cpp -o testcasegenerator
testcasegenerator

g++ dp_Merge_sort_time.cpp -o dp_Merge_sort_time
g++ dp_Sorted.cpp -o dp_Sorted
g++ comparison.cpp -o comparison

dp_Merge_sort_time > output_dp_Merge_sort_time.txt
dp_Sorted  > output_dp_Sorted.txt
comparison