
# Version 1

## Overview
So we have made an algorithm to find the time taken by robots to complete orders given in amazon warehouse when  a single robot can process only one robot at a time.
This version adds a new feature of robots having a capacity C which is more than  any order size  , thus opening a possibility of merging bunch of orders then executing these orders. 

## Past Works
From searching about how merging problem are dealt with, we came to know about the Classic Tabu Search, which runs in O(N^2^) , the working is done in following manner
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
```
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

So for testing purpose we can change the custom sort function to get get the answer for all the cases.


## Results
T(Sorting on basis of catering time) <= T(Sorting on basis of sorting time)
