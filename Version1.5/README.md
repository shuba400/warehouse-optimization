
# Version 1.5

## Overview
So now we know how to merge some orders using combination of DSU and geometry heuristics.  We also merge them greedily to maximize potential merging.

So this version basically focused on how to merge the orders geometrically.


## Logic
We merged 2 orders when their overalping is greater than a certain percent. One of the challenge over there was that we are not considering the factor of cells which is not in the overlap area, the chances of 2 order getting overlapped even when non overlap order are at extreme of the grids is decently high.

So instead of keeping track of cutoff percentage, what we have done is given Order A (a large merged order) and Order B (A new order which have potencial to get merged)

We are keeping track of d(A,B) ==> for each Cell c in B, find the closest cell X in Order A to cell c and add the answer dist(c,X)  to cnter. Do this of every cell in B.

Now you will be having bunch of orders with d(A,x<sub>1</sub>) , d(A,x<sub>2</sub>) , d(A,x<sub>3</sub>) .... , we will merge the order  xi which have the smallest value of d(A,x<sub>i</sub>)

Which orders to check? We will be using the set of orders that are within threshold distance of the cell

## Psuedo Codes

### Small modifications from Version 0
In version 0, time complexity for computing catering time for given a set of coordinates of size n was O(n!), we have used dp based TSP approach and brought it down to O(n2<sup>n</sup>).

### 1) Merging Orders (Based on sorting)
```c
Custom_Sort(All_Orders) 
New_Order_Set = {}
curr_Order = {}
robot_capacity
for(Order: Orders)
	if(curr_Order.size + Order.size > robot_capacity)
		new_Order_set.push(curr_Order)
		curr_Order = {}

	curr_Order.push(Order)
New_Order_Set.push(curr_Order)
return New_Order
```

### 2) Geometric Sampling
Now that we have merged order a new Heuristic that we can apply before merging orders by greedy method is employing geometry.

A high level design of how algorithm will work will look like this 

```c
Apply Geometric_Sampling(Orders AllOrder):
	MergedOrders = {}
	Parents = {} 
	for Order O in AllOrder:
		if(parent(O) != O) continue
		while(1):
			curr_parent = parent(O)  //found using dsu
			Neighbours = Find_Neighbours(curr_parent)
			if(Merge_with_Neigbours(MergedOrders,Neighbours,curr_parent)) continue;
			break
	return MergedOrders
```

So now we have to work on 2 functions, Find_Neighbours and Merge_with_Neigbours.

#### Finding Neighbours

Before proceeding with Neigbours, lets define a data structure L(i,j) which stores the Id of all the orders which have an item at location (i,j)

Now for finding Neigbours, what we have done is for each location, I know the Order ID, that this cell will be having.

```c
L[X][Y] = {} //keeps track of order order for L(i,j)
Find_Neighbours(Order O):
	threshold_distance = d
	set processed // keeps track of i,j pair which have already been processed
	freq<int,int> freq //keeps track of number of time each Order Id came
	for each Cells(c) of Order O:
		for (i,j) such that distance(i,j,c) <= d && not processed(i,j):
			processed(i,j) = 1
			for Id I in L(i,j)
				freq[I]++
	return freq
```

Genetic Parts

```c
// cater all order in current batches
time_taken_to_cater_all_orders = -1
initialize <priority_queue> queue // min-heap on the basic of R.time (earliest time when robot is free)
for each robot R in robots:
    initialize R.time = 0        
    push(R)

for order O in Batches
    earliest_free_robot(R) <-- queue.top()
    pop from queue
    R.time  <-- R.time + O.time 
    R.order <-- R.orders + O
    push(R)
    time_taken_to_cater_all_orders  <-- max of time_taken_to_cater_all_orders , R.time
```

#### Merging these Neigbours with current Order
```c
Merging_Neighbours(MergedOrders,Neighbours,O):
	best_distance = inf
	best_order = -1
	for each N of Neibours:
		if(capacity(O) + capacity(N) > robot_capacity) continue
		curr_distance = find_distance(N,O)
		if(curr_distance < best_distance)
			curr_distance = best_distance
			best_order = N
	if(best_order == -1) return false
	dsu_merge(N,O)
	return true
```


#### Finding Best distance between Order

```c
Find_Distance(Order Current, Order Parent):
	cnter = 0
	for each Cell C of Current:
		best_distance = inf
		for each Cell X of Parent
			best_distance = min(best_distance,manhatten_distance(C,X)
		cnter += best_distance
	return cnter
```


### Assignments of robots to modified Orders (after applying Merging Algo on intitial input Orders)
```c
initialize next_free_time=0 for each robot
push all robots in a priority queue sorted(ascending) by robot’s next_free_time

sort(Batches) //sorts order in decreasing order according to time

for batch B in Batches
	earliest_free_robot <-- queue.top()
	pop from queue	
	Assign batch B to robot R
	R.next_free_time <-- R.next_free_time + B.catering_time 
	push R to queue
```

So for testing purpose we can change the custom sort function to get get the answer for all the cases.


### Time Complexity Analysis 

### For geometric merging part
On an average each order will be called 3 times (As robot capacity if 10 and average order size if 3, so merge will happen 3 times)

Now for each merging we have
 --> Finding Neibours => O(d<sup>2</sup>) where d is threshold distance
 --> Merging with distance => O(cnt(N) * Robot_Capcitity*AvgOrderSize)

Overall time = O(N*(d<sup>2</sup> +  average_number_Neibours* Robot_Capcitity*AvgOrderSize))

### For greedy merging part
N=> Number of Order , R=> Num of Robots
Overall Complexity ⇒  O(N logR) 
   
<!-- ## Results -->
<!-- T(Sorting on basis of catering time) <= T(Sorting on basis of sorting time) -->
