@echo off
@REM g++ testcasegenerator.cpp -o testcasegenerator
@REM testcasegenerator

g++ geo_sampling_new.cpp -o geo_sampling_new
geo_sampling_new

g++ dp_Sorted.cpp -o dp_Sorted
dp_Sorted

g++ dp_merge_greedy_size.cpp -o dp_merge_greedy_size
dp_merge_greedy_size

g++ dp_Merge_reverse_sort_time.cpp -o dp_Merge_reverse_sort_time
dp_Merge_reverse_sort_time

