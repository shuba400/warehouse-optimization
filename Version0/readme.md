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

## Logic Explanation

So for both greedy strategies we require a utility function to calculate time taken to complete an order by 1 robot, (starting from source, visit coordinates in some order and take items at that coordinate, come back to source). 
We have to done it using brute force approach as total number of orders are less than 7, so we have to process 7! orders which is not that time taking.
```
for each order:
	intialize order.time = INF
	for permutation in all_permutation_of_order:
		order.time = min(order.time,time_to_complete_present_permutation)
```
### FCFS Approach

Now for First Come First Serve Approach as the name implies, as the new orders are coming, we assign a robot which is going to be free at the earliest possible time to this order
To keep track of earlies free time, we can use data structure like min heap
```
initialize <priority_queue> queue //min-heap the basis of R.time
for each robot R in robots:
	initialize R.time = 0
	push(R)

for order O in Orders
	earliest_free_robot(R) <-- queue.top()
	pop from queue
	R.time 	<-- R.time + O.time 
	R.order <-- R.orders + O
	push(R)
```
Now once this is done we have the orders that each robot will process and time taken to complete its order, (this we have to coordinated each robot will visit)

### SORT Approach
Since each order takes non uniform time, one of the intuitive approach can be to sort order on the basis of decreasing time and then do the greedy strategy we followed in FCFS approach
```
initialize <priority_queue> queue  
for each robot R in robots:
	initialize R.time = 0
	push(R)

sort(Orders) //sorts order in decreasing order according to time

for order O in Orders
	earliest_free_robot(R) <-- queue.top()
	pop from queue
	R.time 	<-- R.time + O.time 
	R.order <-- R.orders + O
	push(R)
```
In all the random test cases that we generated
Time(FCFS) >= Time(SORT)

