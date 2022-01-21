@echo off


for /l %%x in (1, 1, 1000) do (
    testcasegenerator
    bruteForce_fcfsGreedy_v0 > greedyV0.txt
    bruteForce_sortedGreedy_v0  > checkerV0.txt
    fc greedyV0.txt checkerV0.txt > checker_log.txt || exit /b
    echo %%x
)
echo all tests passed