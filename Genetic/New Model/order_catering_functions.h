// pass orders list and get the time AQ decreasing ordre of catering Time
pair<int,vector<vector<int>>> caterAllOrders(vector<vector<int>>itemSequences){   

    vector<int>cateringTimes;
    for(int i = 0; i < itemSequences.size(); i++)
        cateringTimes.push_back(ordinary_TSP(itemSequences[i]).first);
    sort(cateringTimes.begin(),cateringTimes.end(),greater<int>());

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>robotFreeTimes;
    // initially all the robots are free at t = 0
    for(int i = 0 ; i < num_of_robots ; ++i){
        robotFreeTimes.push({0,i});
    }
    int totalTime = 0;     // total time taken to cater all order
    vector<vector<int>>robotTasks(num_of_robots);

    // cater all the order one by one
    // allot the earliest free robot to current order
    for(int i = 0 ; i < cateringTimes.size() ; ++i){
        // pick the earliest free robot 
        int earliestFreeRobot=robotFreeTimes.top().second;
        int freeTime=robotFreeTimes.top().first;
        robotFreeTimes.pop();

        // assign ith task to that robot
        int cateringTime = cateringTimes[i];
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    return {totalTime,robotTasks};
}