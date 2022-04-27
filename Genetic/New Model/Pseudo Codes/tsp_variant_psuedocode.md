# TSP Variants

### General Working of each code
* Input parametre
    * item_index - This is the parametre given to all TSP functionts which contains the collection of Item indexes for which we want an optimal path for.
* Output Parametre
    * Distance(int) - Gives the shortest distance accoding to the algorithm
    * vector of (Indices,Cells) - Gives the path to get the above mentioned distance, where each Index,cell pair represent the index of item on which we need to go to and which cell of the repective item should we go to

### Nearest Neigbour

```python
Function nearest_neighbour_TSP(item_index) 
{
    n ← items.size()
    time ← 0
    path[] ← {} // path[i] = {item id , cell for the item id}
    isItemVisited[n] ← {false}  //to  keep track of which cell we have taken in the path
    currentCell ← {0,0}
    while path.size() < n
        nextItem ← 0
        nextCell ← {}
        nextDistance ← infinity
        for i = 0 to n:  // get the next cell which have lest distance from currentCell
            if isItemVisited[i] == true  
                skip this cell as this is visited
            bestCell ← {}
            bestDistance ← inf
            for each cell of allItems[items[i]].cells)
                if distance(currentCell,cell)< bestDistance
                    bestDistance ← distance(currentCell,cell)
                    bestCell ← cell
            if bestDistance < nextDistance
                //take this cell and keep track of best distance so far
                nextDistance ← bestDistance
                nextCell ← bestCell
                nextItem ← i
            Endfor
        mark current cell and continue 
        isItemVisited[nextItem] ← true
        path.push({items[nextItem],nextCell})
        currentCell ← nextCell
        time ←  time + nextDistance
    }
    time ←  time + distance(currentCell,{0,0})  //tuake the  distance from last cell
    time ←  time + n*docking_time   //add required docking time
    return time,path
}
```


### Ordinary TSP

Over here we, given a sequence of Items, we need to find which cells of each item do we have to traverse to get a cell sequence
For this we use following recurrence
Let dp[i][j] tell us what is the minimum possible we have traversed to reach cell j of item i
Now for each cell k of item (i + 1): 
dp[i + 1][k] = min { dp[i][j] + distance from cell[j] to cell[k] } for all cells j contained item i

```python
Function ordinary_TSP(items_index)
    //first we conver indexes to actual items (as we required the detail of cells to work with)
    items ← {}
    items.push(0,0)  //inserting starting cell (0,0) as first item of the sequence
    for each index of item_index
        //push all cells of given item index
        item.push(allCell(allItems[index]))
    Endfor
    
    
    items.push(0,0)  //push the last item , again (0,0) as we need to reach  origin after collecting all items
    n ← items.size()
    dp ← [n + 1][max_cell_item]  --> dp[i][j]  tells minimum distance  traverlled to reach  cell  j of item i 
    dp[0][0] ←  0 //since we start at 0,0 , distance here is 0
    for i = 1 to n:
        for j = 0 to items[i].cells.size():
            for k = 0 to items[i - 1].cells.size():
                //applying main reccurence here
                dp[i][j] ← min(dp[i][j],dp[i - 1][k] + distance(items[i].cells[j],items[i - 1].cells[k]) + ]);
            Endfor
        Endfor
    Endfor
    
   return dp[n - 1][0] //return min distance to reach last cell , that is (0,0)
}
```


## Optimal TSP

Optimal TSP working is similar to standard Np - hard , exponential time TSP that is common, only difference is that in case of standard TSP , we had dp[mask][last_element] as dp state, here we are having the dp[mask][last_item][last_cell_of_last_item] , basically we are adding a new dimension to keep track of which cell of the last item we had iterated in order to calculate the distance from this last_cell to new cell of new item. We need to keep track of last items for each dp state as well so that we can do backtrack and get an optimal path 

```python
/* 
*/
Function recur(mask,curr_element,curr_index,dp,items,tot_elements){
    if mask == 0
        return infinity (this means we reached a point where we can't reach origin)
    if dp[mask][curr_element][curr_index][0] != -1
        //current state calculate, return the already calculate answer
        return dp[mask][curr_element][curr_index][0]
        
    best_distance ← {inf,0,0}  //intialize best_distance to infinity
    
    for new_item = 0 to tot_elements  //iterate over all items
        if item is already visited 
            continue
        for cells j of new_item
            best_distance ← min(best_distance,{recur(mask^(1<<curr_element),item,j,dp,items,tot_elements) + distance between (curr_index cell of current element) to cell j of new_item) 
        Endfor
    Endfor
    dp[mask][curr_element][curr_index] ← best
    return dp[mask][curr_element][curr_index][0]
}

Function optimal_TSP(itemIndex){
    //convert the given itemIndexs to actual items
    items ← {}  //array of items to be taken from indexes given in itemIndex
    items.push({0,0}) //push starting item with cell {0,0} , this is the starting node
    for each index of itemIndex
        items.push(allItems[index])
    Endfor
    
    tot_elements ← size(items)
    //prerequitics of TSP
    tot_masks ← (2**tot_elements) 
    best ← {inf,0,0}  //keeps track of {minimum distance,starting index,starting cell of given index}
    dp ← [tot_mask][tot_elements][6] 
    dp[1][0][0] ← {0,-1,-1}
    for i = 0 to tot_elements
        for j = 0 to items[i].cells.size()
            best ← min(best,{recur(tot_masks - 1,i,j,dp,items,tot_elements) + distance  between cell (0,0) and cell j of item i)
        Endfor
    Endfor
    
    //now we start backtrack to get the path
    main_mask ← tot_masks - 1
    last_element ← best[1]
    last_cell ← best[2]
    ans ← {} //keeps track of path of item and given cell of item
    
    while last_element > 0
        push the current pair of last element and last cell and go the previos pair of (element,cell) stored in this dp state while updating the mask we are working on
        
        ans.push(last_element,items[last_element].cells[last_cell]) 
        new_last_element ← dp[main_mask][last_element][last_cell][1]
        new_last_cell ← dp[main_mask][last_element][last_cell][2]
        main_mask ^= (2**last_element)
        last_element ← new_last_element
        last_cell ← new_last_cell
        
    for each item of ans 
            item = get_index(item)   //get the indexes of given item and store that
    Endfor
    
    return {best.minimum_distance,ans}
}
```
