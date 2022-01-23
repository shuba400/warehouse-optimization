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


// class to store details of a single cell
class Cell{
    public:
        int x;
        int y;
};


// class to store all the details related to a single order
class Order{
    public:
        vector<Cell>locations;  // to store coordinates of each item in current order

    void orderSize(int szOrder){
        locations = vector<Cell>(szOrder);
    }
    
    int getOrderSize(){
        return locations.size();
    }
};

// vector containing details of all orders
vector<Order>allOrders;

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity;
    cin>>num_of_robots>>num_of_orders;
    
    allOrders = vector<Order>(num_of_orders);

    for(int i = 0 ; i < num_of_orders;++i){

        int currOrderSize;
        cin>>currOrderSize;
        allOrders[i].orderSize(currOrderSize);

        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            cin>>allOrders[i].locations[j].x>>allOrders[i].locations[j].y;
        }
    }
}
void printTestCaseDetails(){
    for(int i = 0 ; i < num_of_orders ; ++i){
        int currOrderSize = allOrders[i].getOrderSize();
        cout<<currOrderSize<<endl;
        for(int j = 0 ; j < currOrderSize; ++j){
            cout<<allOrders[i].locations[j].x<<" "<<allOrders[i].locations[j].y<<endl;
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

int cater_curr_order(Order curr_order){
    // minimum time required to cater this order by a single robot;
    int min_time = min_all_permutation(curr_order.locations,0);
    return min_time;
}

int cater_all_orders(){
    // how much time it will take to cater all the order by robots.

    // priority queue will store the earliest free time of all robots
    priority_queue<int,vector<int>,greater<int>>earliest_free_robot;

    // initially all the robots are free at t = 0
    for(int i = 0 ; i < num_of_robots ; ++i){
        earliest_free_robot.push(0);
    }

    int total_time = 0;     // total time taken to cater all order

    // cater all the order one by one
    // allot the earliest free robot to current order
    for(int i = 0 ; i < num_of_orders ; ++i){
        // pick the earliest free robot 
        int earliest_start = earliest_free_robot.top();
        earliest_free_robot.pop();

        // assign ith task to that robot
        int cater_duration = cater_curr_order(allOrders[i]);
        int finish_time = earliest_start + cater_duration;
        total_time = max(total_time,finish_time);

        // current robot will be free after finishing the ith task
        earliest_free_robot.push(finish_time);
    }
    return total_time;
}

void cal_for_given_test(){
    take_input();
    // printTestCaseDetails();
    int total_time_taken = cater_all_orders();
    cout<<"Time taken to complete all orders : \n"<<total_time_taken<<"\n";
}

int main(){
    freopen("input.txt","r",stdin);
    int test = 1;
    cin >> test;
    while(test--){
        cal_for_given_test();
    }
}
