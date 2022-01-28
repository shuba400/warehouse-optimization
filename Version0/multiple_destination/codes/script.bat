@echo off
@REM g++ testcasegenerator.cpp -o testcasegenerator
@REM testcasegenerator
g++ bruteForce_fcfsGreedy_v0.cpp -o bruteForce_fcfsGreedy_v0
g++ bruteForce_sortedGreedy_v0.cpp -o bruteForce_sortedGreedy_v0
g++ dp_sortedGreedy_v0.cpp -o dp_sortedGreedy_v0
g++ checkerV0.cpp -o checkerV0
g++ comparison.cpp -o comparison
bruteForce_fcfsGreedy_v0 > output_bruteForce_fcfsGreedy_v0.txt
bruteForce_sortedGreedy_v0  > output_bruteForce_sortedGreedy_v0.txt
comparison