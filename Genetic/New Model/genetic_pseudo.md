
### Crossover 
This function applies the crossover operation on given parents and generate two new children.

```python
function crossover (parent1, parent2){

    n1 <- number of batches in parent1
    n2 <- number of batches in parent2 

    l1,r1 <- randomly select a cross section from parent1
    l2,r2 <- randomly select a cross section from parent2

    child1, child2 <- two new children will be generated

    For child1  : 
    Insert the cross section of parent2 (l2..r2) before cross section of parent1 (l1..r1)

    for i = 0 ... l1-1 do 
        push_back ith batch of parent1 in child1 
        push_back ith itemSequence of parent1 in child1 
    
    for i = l2 ... r2  do 
        push_back ith batch of parent2 in child1 
        push_back ith itemSequence of parent2 in child1 
    
    for i = l1 ... n1-1 do 
        push_back ith batch of parent1 in child1 
        push_back ith itemSequence of parent1 in child1 

    For child2  : 
    Insert the cross section of parent1 (l1..r1) before cross section of parent2 (l2..r2)

    for i = 0 ... l2-1 do 
        push_back ith batch of parent2 in child2 
        push_back ith itemSequence of parent2 in child2 
    
    for i = l1 ... r1  do 
        push_back ith batch of parent1 in child2 
        push_back ith itemSequence of parent1 in child2 
    
    for i = l2 ... n2-1 do 
        push_back ith batch of parent2 in child2 
        push_back ith itemSequence of parent2 in child2 

    child1, child2 may be invalid because repititons of some orders.
    Get a valid child by calling get_valid_child function on child1 and child2
    
    child1 <- get_valid_child(child1)
    child2 <- get_valid_child(child2)

    return {child1,child2}
}
```

### Get Valid Child 
In crossover after appending cross section from one parent to another, child becomes invalid because of repetition
Make them valid, such that each order is included in only one batch 

```python
function get_valid_child(genotype& child){
    
    Some orders are repeated in given child. 

    for curr_batch in child.gene do
        if curr_batch contain at least one repeated order do 
            delete this batch 
    
    Some orders are deleted and are not included in any batch 

    for deleted_order in all_deleted_orders do 
        reinsert missing orders into new batches according to FIRST-FIT Rule
}
```

### Mutate Item Sequence  
This function mutates all the itemSequences corresponding to each batch in given member.

```python
function mutate_item_sequence(genotype& member) { 
    for all sequence in member.itemSequence :
        sequence <- two_opt(sequence) 
}
```

### Mutate Batching 
This function mutates the batching scheme of given member 

```python
function mutate_batching(genotype& member) {
    
    b1 , b2 <- select two random batches
    delete b1 and b2 from the batches of given member.

    Sort the deleted orders according to their size in increasing way.

    for curr_deleted_order in all_deleted_orders do 
        Assign to new batch applying BEST-FIT rule : 
        An order is assigned to the batch with minimum capacity greater than required capacity.
            if no such batch found
                make a new batch for this order 

}
```
