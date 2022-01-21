# Version 0 Code Overview
We are having 3 algorithms, Greedy algo using First Come First Serve  (FCFS), Greedy Algo using sorting (SORT), and a checker dp-brute force algo(Checker.cpp). Right now we have to compare FCFS and SORT algo

## Important Edits
I have only printed the time required by robot to for testing purpose, if you want to check the path, uncomment that part out.

## How to check

### 1) Build all the file using file following compile command
  ```
g++ <name_of_file>.cpp -o <name_of_file>
 ```
Do not change the naming of any file as the all the script.bat file is made using these namings. Most probably you do not have to take care of this thing as most ide already have build command in this way. 
Do this for anything label as .cpp (there are too many files so not writing the whole info)
To Do ==> Probably add a maker process to automate this part

### 2) Run script.bat
```
./script.bat
```
### 3) Expected Format output
If you have an output in following format then give yourself a pat on back, you know how to use this unintentionally complicated way of checking 2 cpp files.
```
fcfs_approach 15816  15750  15870  15774  15784  15786
sort_approach 15186  15188  15240  15188  15196  15194
Better_Approach_FCFS 0  0  0  0  0  0
Better_Approach_SORT 1  1  1  1  1  1
```

## Edit Instruction
I have made a comparison.cpp file, which if done a brief overview is easy to observe that I used used file stream instead of cin to read different files simultaneously. So to add custom check type, just change the comparison.cpp file.


## ToDo
Adding a maker(for automate building also) might help
