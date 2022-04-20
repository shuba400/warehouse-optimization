void fill_item_sequence(struct genotype& member){
  member.itemSequence.clear();
  member.itemSequence = vector<vector<int>>(member.gene.size());
  for(int i = 0 ; i < member.gene.size() ; ++i){
    for(auto & currOrderIndex : member.gene[i]){
      for(auto & currItem : allOrders[currOrderIndex].items){
        member.itemSequence[i].push_back(currItem);
      }
    }
  }
}

genotype get_greedy_merged_member(){
    genotype member;
    vector<Order>mergedOrders=allOrders;
    mergedOrders=greedyMergeOrdersSize(mergedOrders);
    for(auto &m:mergedOrders) member.gene.push_back(m.subOrderIndexes);
    fill_item_sequence(member);
    return member;
}

genotype get_greedy_merged_nearest_negihbour_tsp_member(){
    genotype member;
    member.gene.clear();
    vector<Order>mergedOrders=allOrders;
    mergedOrders=greedyMergeOrdersSize(mergedOrders);
    for(auto &m:mergedOrders) member.gene.push_back(m.subOrderIndexes);
    fill_item_sequence(member);
    for(int i=0;i<member.gene.size();i++)
    {
      pair<int,vector<pair<int,Cell>>>temp=nearest_neighbour_TSP(member.itemSequence[i]);
      vector<int>currItemSequence;
      for(auto &itemNumber:temp.second)
        currItemSequence.push_back(itemNumber.first);
      member.itemSequence[i]=currItemSequence;
    }
    return member;
}

genotype get_random_member(){
  genotype member;
  set<int>includedOrders;
  vector<vector<int>>batches;
  vector<int>currBatch;
  int currBatchItems = 0;
  while(includedOrders.size()<num_of_orders){
    int orderIndex = rand(0,num_of_orders-1);
    while(includedOrders.find(orderIndex)!=includedOrders.end()){
      orderIndex++;
      if(orderIndex==num_of_orders)orderIndex = 0;
    }
    if(allOrders[orderIndex].items.size() + currBatchItems <= max_capacity_robot){
      currBatchItems+=allOrders[orderIndex].items.size();
      currBatch.push_back(orderIndex);
    }
    else{
      batches.push_back(currBatch);
      currBatch.clear();
      currBatchItems = allOrders[orderIndex].items.size();
      currBatch.push_back(orderIndex);
    }
    includedOrders.insert(orderIndex);
  }
  if(currBatchItems>0)batches.push_back(currBatch);
  member.gene=batches;
  fill_item_sequence(member);
  return member;
}

