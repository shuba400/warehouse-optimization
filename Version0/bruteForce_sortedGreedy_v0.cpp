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

const int max_docking_time=12;
const int max_number_robots = 3;
const int max_number_order = 10;
const int max_cell_in_order = 5;
const int max_velocity = 5;

int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;


// Class to store details of a single cell
class Cell{
    public:
        int x;
        int y;
};

// Class to store all the details related to a single order
class Order{
    public:
        vector<Cell>cells;  // Stores coordinates of each item in current order
        int cateringTime;  // Stores minimum time taken to visit the cells of this order

        void setOrderSize(int szOrder){
            cells = vector<Cell>(szOrder);
        }        
        int getOrderSize(){
            return cells.size();
        }
        void setCateringData(int cateringTime){
            this->cateringTime=cateringTime;
        }
};
vector<Order>allOrders;  // Stores details of all orders

void take_input(){
    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity;
    cin>>num_of_robots>>num_of_orders;
    
    allOrders = vector<Order>(num_of_orders);

    for(int i = 0 ; i < num_of_orders;++i){

        int currOrderSize;
        cin>>currOrderSize;
        allOrders[i].setOrderSize(currOrderSize);

        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            cin>>allOrders[i].cells[j].x>>allOrders[i].cells[j].y;
        }
    }
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

int manhattan_distance(Cell a, Cell b){
    return abs(a.x-b.x) + abs(a.y - b.y);
}

int min_all_permutation(vector<Cell>&cells,int l){
    int r = cells.size()-1;
    if(l==r){
        Cell starting_point = {0,0};
        int curr_total_time = manhattan_distance(starting_point,cells[0])+docking_time;
        for(int i = 1 ; i <=r ; ++i){
            curr_total_time+=manhattan_distance(cells[i],cells[i-1])+docking_time;
        }
        curr_total_time+=manhattan_distance(starting_point,cells[r])+docking_time;
        return curr_total_time;
    }
    int min_time = INT_MAX;
    for(int i = l; i <= r ; ++i){
        swap(cells[l],cells[i]);
        min_time = min(min_time,min_all_permutation(cells,l+1));
        swap(cells[l],cells[i]);
    }
    return min_time;
}

// Returns minimum time required to cater a order by a single robot 
// And also sorts the cells of this order into the optimal visiting sequence
int caterOrder(int orderIndex){    
    int min_time = min_all_permutation(allOrders[orderIndex].cells,0);
    return min_time;
}

// Comparator function used for sorting orders according to their catering time
bool compOrders(int orderIndex1,int orderIndex2)
{
    return allOrders[orderIndex1].cateringTime>=allOrders[orderIndex2].cateringTime;
}

// Returns time taken to cater all orders by all robots
pair<int,vector<vector<int>>> caterAllOrders(){   

    vector<int>sortedOrders;  // Stores indices of orders sorted according to individual order total catering time.
    for(int i=0;i<allOrders.size();i++)
    {
        allOrders[i].setCateringData(caterOrder(i));
        sortedOrders.push_back(i);
    }
    sort(sortedOrders.begin(),sortedOrders.end(),compOrders);

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
    for(int i = 0 ; i < num_of_orders ; ++i){
        // pick the earliest free robot 
        int earliestFreeRobot=robotFreeTimes.top().second;
        int freeTime=robotFreeTimes.top().first;
        robotFreeTimes.pop();
        robotTasks[earliestFreeRobot].push_back(sortedOrders[i]);

        // assign ith task to that robot
        int cateringTime = allOrders[sortedOrders[i]].cateringTime;
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    return {totalTime,robotTasks};
}


void cal_for_given_test(){
    take_input();
    // printTestCaseDetails();
    pair<int,vector<vector<int>>> cateringData = caterAllOrders();
    int totalTimeTaken=cateringData.first;
    vector<vector<int>>robotTasks=cateringData.second; // For each robot, it stores which orders will be catered by that robot
    cout<<"Time taken to complete all orders : \n"<<totalTimeTaken<<"\n";

    // cout<<"Each Order's optimal cell visiting sequence:\n";
    // for(int i=0;i<num_of_orders;i++)
    // {
    //     cout<<"Order "<<i+1<<": ";
    //     for(int j=0;j<allOrders[i].cells.size();j++)
    //         cout<<"("<<allOrders[i].cells[j].x<<","<<allOrders[i].cells[j].y<<") ";
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
    // cout<<"\n";

    // cout<<"Each Robot's Path:\n";
    // for(int i=0;i<num_of_robots;i++)
    // {
    //     cout<<"Robot "<<i+1<<": ";
    //     for(int j=0;j<robotTasks[i].size();j++)
    //     {
    //         cout<<"(0,0) ";
    //         for(int k=0;k<allOrders[robotTasks[i][j]].cells.size();k++)
    //             cout<<"("<<allOrders[robotTasks[i][j]].cells[k].x<<","<<allOrders[robotTasks[i][j]].cells[k].y<<") ";
    //     }           
    //     cout<<"(0,0)"; 
    //     cout<<"\n";
    // }

}

int main(){
    freopen("input.txt","r",stdin);
    int test = 1;
    cin >> test;
    while(test--){
        cal_for_given_test();
    }
}
