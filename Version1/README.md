

# Version 1

## Overview
So we have made an algorithm to find the time taken by robots to complete orders given in amazon warehouse when  a single robot can process only one robot at a time.
This version adds a new feature of robots having a capacity C which is more than  any order size  , thus opening a possibility of merging bunch of orders then executing these orders. 

## Past Works
From searching about how merging problem are dealt with, we came to know about the Classic Tabu Search, which runs in O(N<sup>2</sup>) , the working is done in following manner
Lets there are N order O, and we know time to complete each order that T(O<sub>i</sub>)

We combine 2 order such that T(O<sub>a</sub>) + T(O<sub>b</sub>) - T(O<sub>b</sub>,O<sub>a</sub>) is maximum possible

## Our derived logic
Lets talk about how much will be the complexity of the above code
So we have to combine some set of order with size N ==> there will be N<sup>2</sup> combination. We have to repeat this for all N orders ( (each combination changes order size from N to N - 2) ) and finding the total catered time for all orders will 2<sup>Capacity_of_robot_C</sup>

So with tabu search Time Complexity will be N<sup>3</sup>C2<sup>C</sup>  which even for small C <= 10 is too much process time

So what we have done is sorted the orders in some manner  (First with respect to time and second with respect to capacity and then with respect to catering time) and than  greedily combined as many order as possible
This reduced overall time complexity to O(2<sup>C</sup>N log(N) )


## Psuedo Codes

### Small modifications from Version 0
In version 0, catering time for given set of coordinates of size n was n!, we have used dp based TSP approach and brought it down to O(n2<sup>n</sup>)

### Merging Orders
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
### Assignments of Robots for new Merged Orders


```c
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

So for testing purpose we can change the custom sort function to get get the answer for all the cases.

### Geometric Sampling
Now that we have merged order a new Heuristic that we can apply before merging orders by greedy method is employing geometry.

A high level design of how algorithm will work will look like this 

```c
Apply Geometric_Sampling(Orders AllOrder):
	MergedOrders = {}
	Parent = {} //this will keep track of parent of each order (main index where this order is being merged)
	for Order O in AllOrder:
		if(parent[O] == -1){
			parent[O] = MergedOrder.size()
			MergedOrder.push(O)
		}
		Neighbours = Find_Neighbours(O)
		Merge_with_Neigbours(MergedOrders,Neighbours,O)
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

### Merging these Neigbours with current Order
```c
Merging_Neighbours(MergedOrders,Neighbours,O):
	parent = parent(O) //this is the index in the Merged Order where our current Order O is
	sort(Neighbours) //sorts on the basis of overlapping area
	for(Each N of Neighbours):
		if(AllOrder[N].capacity + MergedOrder[Parent].capacity <= MaxRobotCapacity):
			Merge(Parent,N) //merged current neribouring order and merge order
			Remove_From_L(N) //if we are merging this order , than we should remove this order id from all the L(i,j)
	return
			
```
## Results
T(Sorting on basis of catering time) <= T(Sorting on basis of sorting time)
