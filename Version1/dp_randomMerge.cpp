/*
    Input Format
    ROWS COLUMNS DOCKING_TIME VELCOCITY
    NUM_OF_ROBOTS NUM_OF_ORDERS
    (for each of next NUM_OF_ORDERS)
    SIZE_OF_NEW_ORDER
        (for SIZE_OF_NEW_ORDER)
        COORD_X COORD_Y
*/

/*
    Docking time ==> time required pick a single time from rack/cell
    t = dist/v + x*d    
    v*t = dist + x*d*v ==> sanity check to reduce precision issue (temprory solution)
*/
#include<bits/stdc++.h>
using namespace std;

const int inf = 1e9;

const int max_docking_time=100;
const int max_number_robots = 3;
const int max_number_order = 10;
const int max_cell_in_order = 5;
const int max_velocity = 5;

int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;
int max_capacity_robot;


// class to store details of a single cell
class Cell{
    public:
        int x,y;

        
};



int distance(Cell a,Cell b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}


// class to store all the details related to a single order
class Order{
private:
        vector<vector<pair<int,pair<int,int>>>> dp;
        /*
            dp[mask][l] ==> {time for current pair of mask and last value,{previous_mask,previous_last_value}}
        */ 
        pair<int,pair<int,int>> main_info;

        int recur_relation(int mask,int last_element){
        if(mask == 0) return inf;
        if(dp[mask][last_element].first != -1) return dp[mask][last_element].first;
        dp[mask][last_element].first = inf;
        for(int i = 0; i < cells.size(); i++){
            if(((mask>>i)&1) == 0 || last_element == i) continue;
            pair<int,pair<int,int>> temp = {recur_relation(mask^(1<<last_element),i) + distance(cells[i],cells[last_element]) + docking_time,{(mask^(1<<last_element)),i}};

            dp[mask][last_element] = min(dp[mask][last_element],temp);
        }
        return dp[mask][last_element].first;
    }

    void start_recurrence(){  //used travelling salesman problem to get the minimum time needed to complete the order with given docking time.
        time = 0;
        int tot_number_of_element = cells.size();                          
        pair<int,pair<int,int>> filler_value = {-1,{-1,-1}};  //used to intialize the dp states
        dp.resize(1<<tot_number_of_element,vector<pair<int,pair<int,int>>>(tot_number_of_element,filler_value));
        dp[1][0] = {0,{-1,-1}};   //current mask = 0000001, current last cell is  0, this is basically saying that when element is at position 0 , t = 0 intially
        for(int i = 1; i < tot_number_of_element; i++){
            pair<int,pair<int,int>> p = {recur_relation((1<<tot_number_of_element) - 1,i) + cells[i].x + docking_time,{(1<<tot_number_of_element) - 1,i}};
            main_info = min(main_info,p);
        }
        return;
    }

public:

    vector<Cell>cells;  // to store coordinates of each item in current order
    int time;
    vector<Cell> optimalpath;

    void orderSize(int szOrder){
        cells = vector<Cell>(szOrder);
    }

    Order(){  
        main_info = {inf,{-1,-1}};
    }

    int getTime(){
        if(dp.empty()) start_recurrence();
        time = main_info.first;
        return time;
    }

    vector<Cell> getPath(){
        if(!optimalpath.empty()) return optimalpath;
        if(dp.empty()) start_recurrence();
        int main_mask = main_info.second.first;
        int last_element = main_info.second.second;
        while(last_element > -1){
            optimalpath.push_back(cells[last_element]);
            int new_last_element = dp[main_mask][last_element].second.second;
            int new_main_mask = dp[main_mask][last_element].second.first;
            last_element = new_last_element;
            main_mask = new_main_mask;
        }
        optimalpath.pop_back();
        reverse(optimalpath.begin(),optimalpath.end());
        return optimalpath;
    }
    
    int getOrderSize(){
        return cells.size();
    }
};

vector<Order>allOrders;  // vector containing details of all orders

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity >> max_capacity_robot;
    cin>>num_of_robots>>num_of_orders;
    allOrders.clear();
    for(int i = 0 ; i < num_of_orders;++i){

        int currOrderSize;
        cin>>currOrderSize;
        Order currOrder = Order();   //this will be the main human cell, for now I am assuming this is 0,0
        Cell tmp;
        tmp.x = 0;
        tmp.y = 0;
        currOrder.cells.push_back(tmp);
        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            cin >> tmp.x >> tmp.y;
            currOrder.cells.push_back(tmp);
        }
        allOrders.push_back(currOrder);
        allOrders[i].getTime();
        allOrders[i].getPath();
    }
}

bool compOrders(int orderIndex1,int orderIndex2)
{
    return allOrders[orderIndex1].time>=allOrders[orderIndex2].time;
}

// Returns time taken to cater all orders by all robots
pair<int,vector<vector<int>>> caterAllOrders(){   

    vector<int>sortedOrders;  // Stores indices of orders sorted according to individual order total catering time.
    for(int i=0;i<allOrders.size();i++){
        sortedOrders.push_back(i);
    }
    sort(sortedOrders.begin(),sortedOrders.end(),compOrders);
    vector<Order> mergedOrder;
    Order currSetOrder;
    int currsum = 0; 
    Cell c;
    for(auto &ind:sortedOrders){
        if(currsum + allOrders[ind].cells.size() - 1 > max_capacity_robot){
            mergedOrder.push_back(currSetOrder);
            currsum = 0;
            currSetOrder = Order();
            c.x = 0;
            c.y = 0;
            currSetOrder.cells.push_back(c);
        }
        currsum += allOrders[ind].cells.size() - 1;
        for(int i = 1; i < allOrders[ind].cells.size(); i++){
            currSetOrder.cells.push_back(allOrders[ind].cells[i]);
        }
    }
    mergedOrder.push_back(currSetOrder);
    for(int i = 0; i < mergedOrder.size(); i++){
        mergedOrder[i].getPath();
        mergedOrder[i].getTime();
    }
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
        int cateringTime = mergedOrder[i].time;
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    return {totalTime,robotTasks};
}


void printTestCaseDetails(){
    for(int i = 0 ; i < num_of_orders ; ++i){
        int currOrderSize = allOrders[i].getOrderSize();
        cout<<currOrderSize<<endl;
        for(int j = 0 ; j < currOrderSize; ++j){
            cout<<allOrders[i].cells[j].x<<" "<<allOrders[i].cells[j].y<<endl;
        }
    }
}


int cal_for_given_test(){
    take_input();
    pair<int,vector<vector<int>>> cateringData = caterAllOrders();
    int totalTimeTaken=cateringData.first;
    vector<vector<int>>robotTasks=cateringData.second; // For each robot, it stores which orders will be catered by that robot
    cout<<"Time taken to complete all orders : \n"<<totalTimeTaken<<"\n";

    // cout<<"Each Order' s optimal cell visiting sequence:\n";
    // for(int i=0;i<num_of_orders;i++)
    // {
    //     cout<<"Order "<<i<<": ";
    //     for(int j=0;j<allOrders[i].optimalpath.size();j++)
    //         cout<<"("<<allOrders[i].optimalpath[j].x<<","<<allOrders[i].optimalpath[j].y<<") ";
    //         cout << "---->" << allOrders[i].time << endl;
    //     cout<<"\n";
    // }
    // cout<<"\n";

    // cout<<"Assignments of Orders to Robots:\n";
    // for(int i=0;i<num_of_robots;i++)
    // {
    //     cout<<"Robot "<<i+1<<": ";
    //     for(int j=0;j<robotTasks[i].size();j++)
    //         cout<<robotTasks[i][j]+1<<" ";
    //     cout<<"\n";
    // }
    //     cout<<"Each Robot's Path:\n";
    // for(int i=0;i<num_of_robots;i++)
    // {
    //     cout<<"Robot "<<i+1<<": ";
    //     for(int j=0;j<robotTasks[i].size();j++)
    //     {
    //         cout<<"(0,0) ";
    //         for(int k=0;k<allOrders[robotTasks[i][j]].optimalpath.size();k++)
    //             cout<<"("<<allOrders[robotTasks[i][j]].optimalpath[k].x<<","<<allOrders[robotTasks[i][j]].optimalpath[k].y<<") ";
    //     }           
    //     cout<<"(0,0)"; 
    //     cout<<"\n";
    // }
    return totalTimeTaken;
}

int main(){
    freopen("input.txt","r",stdin);
    int test = 1;
    cin >> test;
    while(test--){
        cal_for_given_test();
    }
}