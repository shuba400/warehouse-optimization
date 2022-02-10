# Version 1

## Overview
So we have made an algorithm to find the time taken by robots to complete the orders given in a warehouse when a single robot can process only one order at a time.
This version adds a new feature of robots having a capacity (C items) which is more than any order size (number of items in an order). This opens the possibility of merging a bunch of orders and then executing this merged order in the best way possible.. 

## Past Works
While searching about how merging problems are dealt with, we came to know about the Classic Tabu Search, which runs in O(N<sup>2</sup>) , the working is done in following manner.
Lets say there are N orders O, and we know the time to complete each order T(O<sub>i</sub>).

Here, we combine 2 orders such that T(O<sub>a</sub>) + T(O<sub>b</sub>) - T(O<sub>b</sub>,O<sub>a</sub>) is maximum possible.

## Our derived logic for Past Works
Lets talk about how much will be the complexity of the above code.
So we have to combine a set of orders with size N ==> there will be O(N<sup>2</sup>) combinations. We have to repeat this for all N orders ( (each combination changes number of orders from N to N - 1) ) and finding the catering time for any order will be O(C2<sup>Capacity_of_robot_C</sup>).

So with tabu search Time Complexity will be N<sup>3</sup>C2<sup>C</sup>  which even for small C <= 10 is too much computation time.

So we came up with two new approaches:
* Sort the orders in some manner  (First with respect to time and second with respect to order size and then with respect to catering time) and than  greedily combined as many orders as possible. This reduced overall time complexity to O(NC2<sup>C</sup>).
* To find the best possible merge partners for an order O, consider the geometrically neighbouring locations of all item locations in order O. Now all these geometric neighbour cells will have some items (beloning to different orders, lets call these as neighbouring orders). So we choose an order from these neighbouring orders which has maximum percentage of its items included in the neighbourhood of order O. This chosen order will be merged with order O. This process of finding a partner is repeated for each order.

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
#### Merging these Neigbours with current Order
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

### 3) Geometric Sampling New
```c
Note: Order IS Array of Cell
      Order with greater number of items (cells) is made as parent in DSU_MERGING

FUNCTION geoMergeOrders(Array_of_Order: orderList ,Integer: distanceThreshold=d ,Integer: overlapPercent=ov,HashMap_from_Cell_to_Array_of_Order: L)

	INITIALIZE DsuTrees FOR ALL Order IN orderList
	
	FOR EACH iterOrder IN orderList
		IF iterOrder IS NOT Root IN DsuTrees
			CONTINUE
			
		WHILE TRUE
			curOrder = Root of iterOrder IN DsuTrees
			Array of Cell: neighbourCells={}
			
			FOR EACH curCell IN curOrder
				FOR ALL candidateCell SUCH THAT candidateCell IS VALID Cell IN Grid AND ManhattanDistance(candidateCell,curCell)<=d
					IF candidateCell IS NOT IN neighbourCells
						PUSH candidateCell TO neighbourCells
				
			HashMap_from_Order_to_Integer: numOrderItemsOverlapped				
			FOR EACH neighbourCell IN neighbourCells
				FOR EACH neighbourOrder in L[neighbourCell]
					rootNeighbourOrder = Root of neighbourOrder IN DsuTrees
					numOrderItemsOverlapped[rootNeighbourOrder]++
				
			Order: bestPartner=NULL
			FOR ALL candidateOrder in numOrderItemsOverlapped
				IF SIZEOF(candidateOrder) + SIZEOF(curOrder) > robotCapacity
					CONTINUE
				overlapPercent = 100 * numOrderItemsOverlapped[candidateOrder]/SIZEOF(candidateOrder)
				IF overlapPercent > overlapThreshold
					CONSIDER candidateOrder FOR bestPartner
				
			IF bestPartner = NULL
				BREAL LOOP
			MERGE curOrder and bestPartner
			UPDATE DsuTrees FOR MERGER
			
	RETURN ALL Order in orderList WHICH ARE Root IN DsuTrees	
```

### Assignments of robots to modified Orders (after applying Merging Algo on intitial input Orders)
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

<!-- ## Results -->
<!-- T(Sorting on basis of catering time) <= T(Sorting on basis of sorting time) -->
