@echo off
g++ dp_randomMerge.cpp -o dp_randomMerge
g++ dp_Sorted.cpp -o dp_Sorted
g++ comparison.cpp -o comparison
g++ testcasegenerator.cpp -o testcasegenerator

testcasegenerator
dp_randomMerge > output_dp_randomMerge.txt
dp_Sorted  > output_dp_Sorted.txt
comparison