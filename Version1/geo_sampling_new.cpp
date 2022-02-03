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
#include<time.h>
#define FASTIO	ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)
const int inf = 1e9;

int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;
int max_capacity_robot;

int distanceThreshold=5;    
double overlapThreshold=0.7;

// class to store details of a single cell
struct Cell{
    int x,y; 
    bool operator<(const Cell &o)  const {
        return (x < o.x) || (x == o.x && y < o.y);
    }
    friend ostream& operator << (ostream &os, const Cell &m) {return os << "{" << m.x << "," << m.y << "}";}
};

int distance(Cell a,Cell b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// class to store all the details related to a single order
struct Order{
    vector<Cell>cells;  // to store coordinates of each item in current order
    vector<Cell> optimalpath;
    int time;

    Order(){   
        time = 0;
    }

    int getOrderSize(){
        return cells.size() - 1;
    }
};

//This part calculate TSP, DO NOT TOUCH
vector<vector<pair<int,pair<int,int>>>> dp;
pair<int,pair<int,int>> main_info;
vector<Cell> dp_cells;
int recur_relation(int mask,int last_element){
    if(mask == 0) return inf;
    if(dp[mask][last_element].first != -1) return dp[mask][last_element].first;
    dp[mask][last_element].first = inf;
    for(int i = 0; i < dp_cells.size(); i++){
        if(((mask>>i)&1) == 0 || last_element == i) continue;
        pair<int,pair<int,int>> temp = {recur_relation(mask^(1<<last_element),i) + distance(dp_cells[i],dp_cells[last_element]) + docking_time,{(mask^(1<<last_element)),i}};

        dp[mask][last_element] = min(dp[mask][last_element],temp);
    }
    return dp[mask][last_element].first;
}

void start_recurrence(Order &o){
    dp_cells.clear();
    dp.clear();
    o.optimalpath.clear();
    o.time=0;
    main_info = {inf,{-1,-1}};

    for(auto &x:o.cells) dp_cells.push_back(x);
    int tot_number_of_element = dp_cells.size();                          
    pair<int,pair<int,int>> filler_value = {-1,{-1,-1}};  //used to intialize the dp states
    dp.resize(1<<tot_number_of_element,vector<pair<int,pair<int,int>>>(tot_number_of_element,filler_value));
    dp[1][0] = {0,{-1,-1}};   //current mask = 0000001, current last cell is  0, this is basically saying that when element is at position 0 , t = 0 intially
    for(int i = 1; i < tot_number_of_element; i++){
        pair<int,pair<int,int>> p = {recur_relation((1<<tot_number_of_element) - 1,i) + distance(dp_cells[0],dp_cells[i]) + docking_time,{(1<<tot_number_of_element) - 1,i}};
        main_info = min(main_info,p);
    }
    o.time = main_info.first;
    int main_mask = main_info.second.first;
    int last_element = main_info.second.second;
    while(last_element > -1){
        o.optimalpath.push_back(dp_cells[last_element]);
        int new_last_element = dp[main_mask][last_element].second.second;
        int new_main_mask = dp[main_mask][last_element].second.first;
        last_element = new_last_element;
        main_mask = new_main_mask;
    }
    o.optimalpath.pop_back();
    reverse(o.optimalpath.begin(),o.optimalpath.end());
    return;
}
//End of DO NOT TOUCH PART

vector<Order>allOrders;  // vector containing details of all orders
vector<vector<vector<int>>>L;

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity >> max_capacity_robot;
    cin>>num_of_robots>>num_of_orders;
    allOrders.clear();
    vector<int>tempv;
    L.assign(ROWS+1,vector<vector<int>>(COLS+1,tempv));
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
            L[tmp.x][tmp.y].push_back(i);
        }
        allOrders.push_back(currOrder);
        start_recurrence(allOrders[i]);
    }    
}

bool compOrdersDescTime(Order &order1,Order &order2)
{
    return order1.time>order2.time;
}

Order mergeTwoOrders(Order &order1,Order &order2)  //Adds all cells from order2 to order1 
{
    for(int i=1;i<order2.cells.size();i++)
        order1.cells.push_back(order2.cells[i]);
    return order1;
}

int dsu_find(int o1,vector<int>&orderParent)
{
    if(orderParent[o1]==o1)
        return o1;
    return orderParent[o1]=dsu_find(orderParent[o1],orderParent);
}
void dsu_merge(int o1,int o2,vector<int>&orderParent,vector<Order>&orderList)
{
    int rooto1=dsu_find(o1,orderParent);
    int rooto2=dsu_find(o2,orderParent);
    if(rooto1==rooto2)
        return;
    if(orderList[rooto1].getOrderSize()<orderList[rooto2].getOrderSize())
        swap(rooto1,rooto2);
    orderParent[rooto2]=rooto1;
    orderList[rooto1]=mergeTwoOrders(orderList[rooto1],orderList[rooto2]);
}

vector<Order> geoMergeOrders(vector<Order>orderList)
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);
    vector<vector<int>>isNeighbour(ROWS+1,vector<int>(COLS+1,0));
    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)!=i)
            continue;
        while(1)
        {
            int curOrderIndex=dsu_find(i,orderParent);
            vector<pair<int,int>>neighbours;
            for(int j=1;j<orderList[curOrderIndex].cells.size();j++)
            {
                Cell cell=orderList[curOrderIndex].cells[j];
                for(int dx=-distanceThreshold;dx<=distanceThreshold;dx++)
                {
                    for(int dy=-distanceThreshold;dy<=distanceThreshold;dy++)
                    {
                        int nx=cell.x+dx;
                        int ny=cell.y+dy;
                        if(nx<1||nx>ROWS||ny<1||ny>COLS)
                            continue;                    
                        if(isNeighbour[nx][ny]!=0)
                            continue;
                        isNeighbour[nx][ny]=1;
                        neighbours.push_back({nx,ny});
                    }
                }
            }
            unordered_map<int,int>freq;
            for(auto &neighbour:neighbours)
            {
                for(auto &neighbourOrderIndex:L[neighbour.first][neighbour.second])
                    freq[dsu_find(neighbourOrderIndex,orderParent)]++;
                isNeighbour[neighbour.first][neighbour.second]=0;
            }
            if(freq.find(dsu_find(curOrderIndex,orderParent))!=freq.end())
                freq.erase(dsu_find(curOrderIndex,orderParent));
            int bestPartnerOrderIndex=-1;
            double bestOverlapRatio=0.0;
            for(auto &it:freq)
            {
                int neighbourOrderIndex=it.first;
                int neighbourOrderSize=orderList[neighbourOrderIndex].getOrderSize();
                if(neighbourOrderSize+orderList[curOrderIndex].getOrderSize()>max_capacity_robot)
                    continue;
                double overlapRatio=(it.second*1.0)/neighbourOrderSize;
                if(overlapRatio<overlapThreshold)
                    continue;
                if(overlapRatio>bestOverlapRatio)
                {
                    bestOverlapRatio=overlapRatio;
                    bestPartnerOrderIndex=neighbourOrderIndex;
                }
            }
            if(bestPartnerOrderIndex==-1)
                break;
            dsu_merge(curOrderIndex,bestPartnerOrderIndex,orderParent,orderList);
        }        
    }
    vector<Order>mergedOrders;
    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)==i)
            mergedOrders.push_back(orderList[i]);
    }
    return mergedOrders;
}

vector<Order> greedyMergeOrdersDescTime(vector<Order>orderList)  // merges orders by sorting them in descending order according to catering time
{
    int n=orderList.size();    
    for(int i=0;i<n;i++)
        start_recurrence(orderList[i]);
    sort(orderList.begin(),orderList.end(),compOrdersDescTime);

    vector<Order>mergedOrders;
    Order currSetOrder;
    int currsum=0; 
    Cell c;
    for(int i=0;i<n;i++)
    {
        if(currsum+orderList[i].getOrderSize()>max_capacity_robot)
        {
            mergedOrders.push_back(currSetOrder);
            currsum=0;
            currSetOrder=Order();
            c.x=0;
            c.y=0;
            currSetOrder.cells.push_back(c);
        }
        currsum+=orderList[i].getOrderSize();
        currSetOrder=mergeTwoOrders(currSetOrder,orderList[i]);
    }
    mergedOrders.push_back(currSetOrder);
    return mergedOrders;
}

// Returns time taken to cater all orders by all robots
pair<int,vector<vector<int>>> caterAllOrders()
{   
    vector<Order>mergedOrders=allOrders;
    cout<<"Number of Orders (Initially) : \n"<<mergedOrders.size()<<"\n";
    mergedOrders=geoMergeOrders(mergedOrders);
    cout<<"Number of Orders (After Geo_Merging) : \n"<<mergedOrders.size()<<"\n";
    mergedOrders=greedyMergeOrdersDescTime(mergedOrders);
    cout<<"Number of Orders (After Greedy_Merging_DESC_Time) : \n"<<mergedOrders.size()<<"\n";

    for(int i = 0; i < mergedOrders.size(); i++)
        start_recurrence(mergedOrders[i]);
    sort(mergedOrders.begin(),mergedOrders.end(),compOrdersDescTime);

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
    for(int i = 0 ; i < mergedOrders.size() ; ++i){
        // pick the earliest free robot 
        int earliestFreeRobot=robotFreeTimes.top().second;
        int freeTime=robotFreeTimes.top().first;
        robotFreeTimes.pop();
        robotTasks[earliestFreeRobot].push_back(0);

        // assign ith task to that robot
        int cateringTime = mergedOrders[i].time;
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    return {totalTime,robotTasks};
}


void printTestCaseDetails(){
}


void cal_for_given_test(){
    take_input();
    
    cout<<"----geo_sampling_new:----\n";
    clock_t tStart=clock();

    pair<int,vector<vector<int>>> cateringData = caterAllOrders();
    int totalTimeTaken=cateringData.first;
    vector<vector<int>>robotTasks=cateringData.second; // For each robot, it stores which orders will be catered by that robot

    double exec_time=(double)(clock()-tStart)/CLOCKS_PER_SEC;
    
    double velocityd = 80.4672; // metre per minute
    cout<<"Code Execution Time (in min) : \n"<<exec_time/60<<"\n";
    cout<<"Total Catering Time (in hrs) : \n"<<((totalTimeTaken*1.0)/velocityd)/60<<"\n\n";

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
    return;
}

int main(){
    FASTIO;
    freopen("input.txt","r",stdin);
    int test = 1;
    cin >> test;
    while(test--){
        cal_for_given_test();
    }
}
