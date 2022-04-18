int ordinary_TSP(vector<int>item_numbers)
{
    vector<Item>items;
    Item tmp;
    tmp.cells.push_back({0,0});
    items.push_back(tmp);
    for(auto &x:item_numbers){
        tmp.cells.clear();
        for(auto &c:allItems[x].cells){
            tmp.cells.push_back(c);
        } 
        items.push_back(tmp);
    }
    int n = items.size();
    vector<vector<int>> dp(n + 1,vector<int>(max_cells_in_item+1,inf));
    dp[0][0] = 0;
    for(int i= 1; i < n; i++){
        for(int j = 0; j < items[i].cells.size(); j++){
            for(int k = 0; k < items[i - 1].cells.size(); k++){
                dp[i][j] = min(dp[i][j],distance(items[i].cells[j],items[i - 1].cells[k]) + dp[i - 1][k]);
            }
        }
    }
    return *min_element(dp[n - 1].begin(),dp[n - 1].end());
}

int cater_curr_order(Order curr_order){
    // minimum time required to cater this order by a single robot;
    if(curr_order.getOrderSize()==0){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"Zero Size"<<endl;
      return -1234;
    }
    if(curr_order.getOrderSize()>max_capacity_robot){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"sizeExceeded"<<endl;
      return -1234;
    }
    curr_order.time=ordinary_TSP(curr_order.items);
    return curr_order.time;
}

// pass orders list and get the time AQ FCFS 
pair<int,vector<vector<int>>> caterAllOrders(vector<Order>mergedOrder){   

    // vector<Order> mergedOrder=merge_orders_asc_size();
    // Priority queue will store the earliest free time of all robots
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>robotFreeTimes;
    // initially all the robots are free at t = 0
    for(int i = 0 ; i < num_of_robots ; ++i){
        robotFreeTimes.push({0,i});
    }
    int totalTime = 0;     // total time taken to cater all order
    vector<vector<int>>robotTasks(num_of_robots);

    // cater all the order one by one
    // allot the earliest free robot to current order
    for(int i = 0 ; i < mergedOrder.size() ; ++i){
        // pick the earliest free robot 
        int earliestFreeRobot=robotFreeTimes.top().second;
        int freeTime=robotFreeTimes.top().first;
        robotFreeTimes.pop();
        robotTasks[earliestFreeRobot].push_back(0);

        // assign ith task to that robot
        // int cateringTime = mergedOrder[i].time;
        int cateringTime = cater_curr_order(mergedOrder[i]);
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    // cout<<"Total Time"<<totalTime<<endl;
    return {totalTime,robotTasks};
}

// vector<Order> will be passed to caterAllOrders
vector<Order>getOrderVector(vector<vector<int>>batchVector){
  vector<Order>orderVector;
  for(auto & currBatch : batchVector){
    // currBatch is list of index of orders 
    Order currOrder;
    for(auto & ind : currBatch){
      // for all orders in curr Batch push all indices of items into currOrder
      for(auto & currItem : allOrders[ind].items){
        currOrder.items.push_back(currItem);
      }
    }
    orderVector.push_back(currOrder);
  }
  return orderVector;
}


