# TSP Variants


### Nearest Neigbour

```python
Function nearest_neighbour_TSP(items)
{
    n ← items.size()
    time ← 0
    path ← {} // path[i] = {item id , cell for the item id}
    isItemVisited ← {false}
    currentCell ← {0,0}
    while path.size() < n
        nextItem ← 0
        nextCell ← {}
        nextDistance ← infinity
        for i = 0 to n:
            if isItemVisited[i] == true
                continue
            bestCell ← {}
            bestDistance ← inf
            for each cell of allItems[items[i]].cells)
                if distance(currentCell,cell)< bestDistance
                    bestDistance ← distance(currentCell,cell)
                    bestCell ← cell
            if bestDistance < nextDistance
                nextDistance ← bestDistance
                nextCell ← bestCell
                nextItem ← i
            Endfor
            
        isItemVisited[nextItem] ← true
        path.push({items[nextItem],nextCell})
        currentCell ← nextCell
        time += nextDistance
    }
    time += distance(currentCell,{0,0})
    time += n*docking_time
    return time,path
}
```


### Ordinary TSP
```python
Function ordinary_TSP(items_index){ //takes item index (index in that was being used in main item vector)
    items ← {}
    items.push(0,0)
    for each index of item_index
        item.push(allCell(allItems[index]))
    Endfor
    items.push(0,0)
    n ← items.size();
    dp ← [n + 1][6] 
    dp[0][0] = 0;
    for i = 1 to n:
        for j = 0 to items[i].cells.size():
            for k = 0 to items[i - 1].cells.size():
                dp[i][j] ← min(dp[i][j],distance(items[i].cells[j],items[i - 1].cells[k]) + dp[i - 1][k]);
            Endfor
        Endfor
    Endfor
   return min_element(dp[n - 1])
}
```


## Optimal TSP
```python
/*
mask ==> refers to the set of elements we are working on in form of binary representation
itemIndex ==> refers to the indices of all the indexes of item contained in given order 


*/
Function recur(mask,curr_element,curr_index,dp,items,tot_elements){
    if mask == 0
        return infinity
    if dp[mask][curr_element][curr_index][0] != -1
        return dp[mask][curr_element][curr_index][0]
        
    arr ← {inf,0,0}  //keeps track of best distance
    
    for i = 0 to tot_elements
        if ith_bit_mask == 1 OR curr_element == i 
        	continue
        for j = 0 to a[i].cells.size()
            arr = min(arr,{recur(mask^(1<<curr_element),i,j,dp,a,tot_elements) + distance(a[curr_element].cells[curr_index],a[i].cells[j]),i,j})
        Endfor
    Endfor
    dp[mask][curr_element][curr_index] ← arr
    return dp[mask][curr_element][curr_index][0]
}

Function optimal_TSP(itemIndex){
    items ← {}  //array of items to be taken from indexes given in itemIndex
    items.push({0,0}) //push starting item with cell {0,0} , this is the starting node
    for each index of itemIndex
        items.push(allItems[index])
    Endfor
    tot_elements ← size(items)
    tot_masks ← (2**tot_elements)
    best ← {inf,0,0}  //keeps track of {minimum distance,starting index,starting cell of given index}
    dp ← [tot_mask][tot_elements][6]
    dp[1][0][0] ← {0,-1,-1}
    
    for i = 0 to tot_elements
        for j = 0 to items[i].cells.size()
            best = min(best,{recur(tot_masks - 1,i,j,dp,items,tot_elements) + distance(items[i].cells[j],{0,0}),i,j})
        Endfor
    Endfor
    
    main_mask ← tot_masks - 1
    last_element ← tmp[1]
    last_cell ← tmp[2]
    ans ← {} //keeps track of path of item and given cell of item
    while last_element > 0
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
