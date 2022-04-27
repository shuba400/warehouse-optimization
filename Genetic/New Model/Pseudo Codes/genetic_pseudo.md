# Genetic

### Data Structures used
```python
Structure Cell
    Integer: x
    Integer: y
    # Distance between two cells = Manhattan Distance

Structure Item
    Array<Cell>: cells

Structure Order
    Array<Item>: items
   
Structure Batch
    Array<Order>: orders
    Array<Item>: items      #Stores the summation of all the items in all the orders in this batch
    Integer: catering_time
    #Batch size = Number of items in the batch
    
Structure Genotype
    Float: fitness
    Array<Batch>: batches
    Array<Array<Item>>: item_sequences 
```

### Main Function
```python
#Global Variables
Integer: POPSIZE
Integer: NUMGEN
Float: PROBABILITY_MUTATION_BATCHING
Float: PROBABILITY_MUTATION_ITEM_SEQUENCE
Float: TOP_PERCENT
Array<Genotype>: population

Array<Item>: all_items      # Stores all the unique items in the warehouse
Array<Order>: all_orders        # Stores the orders that we are supposed to cater
# all_items and all_orders are taken as input 

Function main()
    initialize()
    For NUMGEN times
        parent_pairs ← select_parent_pairs(POPSIZE/2)
        children ← Apply crossover on each pair in parent_pairs to get 2 children from each pair
        children ← on each child apply mutate_batching with probability PROBABILITY_MUTATION_BATCHING
        children ← on each child apply mutate_item_sequence with probability PROBABILITY_MUTATION_ITEM_SEQUENCE
        random_members ← generate 0.2*POPSIZE members by generate_member(random_merging,random_item_sequence)
        new_population ← population ∪ children ∪ random_members
        population ← top POPSIZE members according to fitness in new_population
    final_member ← member with highest fitness in population
    final_member ← Apply optimal TSP on each item sequence in final_member.item_sequences
```
### Initial Population
```python
Function initialize()
    population ← {}
    
    # Merging Options: random_merging , greedy_merging_by_size , cw1_merging , cw2_merging
    # Item Sequence Options: random_item_sequence , nearest_neighbour_tsp , optimal_tsp
    
    # To generate a member, select one of the merging options and apply it on all_orders to get the batches.
    # Then select one of the item sequence options and apply it on each batch to get the item_sequences.
    population ← generate_member(merging_option,item_sequence_option)
    
    while population.size() < POPSIZE
        population ← generate_member(random_merging,random_item_sequence)
    
    Sort population in descending order of fitness
```

### Fitness Function
```python
Function get_fitness(member)
    fitness ← 1 / cater_batch(member.item_sequences)
    return fitness

Function cater_batch(item_sequences)
```

### Parents Selection
```python
Function roulette_wheel_selection(current_population,current_probabilities)
    random_probability ← random Float value from 0 to 1
    selected_index ← use binary search to find leftmost index in current_probabilities 
                     which has value greater than or equal to random_probability
    Return current_population[selected_index]

Function select_parent_pairs(num_parents)
    population_top ← top TOP_PERCENT of the population according to fitness
    sum_top ← sum of fitness of all members in population_top
    probabilities_top ← define a probability value for each member in population_top as member.fitness/sum_top
    probabilities_top ← cumulative_sum of probabilities_top from left to right
    
    population_bottom , sum_bottom  , probabilities_bottom ← define similarly for other members
    
    parents ← {}
    For num_parents times
        parent1 ← roulette_wheel_selection(population_top,probabilities_top)
        parent2 ← roulette_wheel_selection(population_bottom,probabilities_bottom)
        parents ← Add {parent1,parent2}
    Return parents    
```

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

In crossover after appending cross section from one parent to another, child becomes invalid because of repetition.

Make them valid, such that each order is included in only one batch.
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

### Mutation

This function mutates the batching scheme of given member.
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

This function mutates all the itemSequences corresponding to each batch in given member.
```python
function mutate_item_sequence(genotype& member) { 
    for all sequence in member.itemSequence :
        sequence <- two_opt(sequence) 
}
```


