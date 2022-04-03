/*
Input Format

ROWS COLUMNS DOCKING_TIME VELCOCITY ROBOT_CAPACITY
NUM_OF_ROBOTS NUM_OF_ORDERS 
TOTAL_NUM_ITEMS

(for each of next TOTAL_NUM_ITEMS)
    NUM_CELLS_FOR_ITEM
    (for each of next NUM_CELLS_FOR_ITEM)
        COORD_X COORD_Y

(for each of next NUM_OF_ORDERS)
    SIZE_OF_NEW_ORDER
    (for each of next SIZE_OF_NEW_ORDER)
        ITEM_NUMBER
*/

/*
Docking time ==> time required pick a single item from rack/cell
    t = dist/v + x*d    
    v*t = dist + x*d*v ==> sanity check to reduce precision issue (temprory solution)
                    d*v = 12 // taking d as 9s and v as 80.4672metre/minute
*/

/*
(Total Items in warehouse) * (Avg cells at which an item is stored) = (Total cells in warehouse) * (Capacity of each cell)
Assuming (Capacity of each cell) = 30
    ==> (Total Items in warehouse) * ((1+5)/2) = (250*250) * (30)
    ==> (Total Items in warehouse) = 6*10^5
*/

/*
    Warehouse size : 250*250        //810,000 sq ft => 900ft x 900ft => 270m x 270m ~~ 250m x 250m 
    robot velocity : 80.4672 metre/minute
    no. of robots  : 800            // 800 robots can be deployed in one floor
    no. of orders  : 1.6*10^5       // max number of orders a warehouse receives in a day
                                    // amazon gets 1.6 million orders per day, but a warehouse will cater smaller no. of orders
    robot capacity : 10             // robot can pick 15 items at total
    docking time   : no data 
                     taking it as 9s = 0.15minutes => d*v = 12.07, round off to 12 
    Total items    : 6*10^5         // Total number of distinct items in warehouse
*/

#include<bits/stdc++.h>
using namespace std;
#define FASTIO	ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
    return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
    return uniform_int_distribution<T>()(rng);
}

const int max_docking_time = 12;        
const int max_number_robots = 5;      
const int max_number_order = 300;   
const int max_order_size = 5;
const double max_velocity = 80.4672;  // metre/min
const int capacity_of_robot = 10;  
const int max_number_of_total_items = 60;
const int max_cells_in_item = 5;  


int ROWS = 250, COLS = 250;     // 250m x 250m 
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
double velocity;
int number_of_total_items;

void out_for_test(){

    // docking_time = rand(0,max_docking_time);
    // num_of_orders = rand(1,max_number_order);
    // num_of_robots = rand(1,max_number_robots);
    // velocity = rand(1,max_velocity);

    docking_time = 0 ; 
    num_of_orders = max_number_order;
    num_of_robots = max_number_robots;
    velocity = max_velocity;
    docking_time = max_docking_time;
    number_of_total_items=max_number_of_total_items;

    cout << ROWS << " "  << COLS << " " <<  docking_time << " " << velocity << " " << capacity_of_robot << endl;
    cout << num_of_robots << " " << num_of_orders << endl;
    cout << number_of_total_items << " " << endl;

    for(int i=0;i<number_of_total_items;i++)
    {
        set<pair<int,int>>set_of_cells;
        int num_cells_in_item=rand(1,max_cells_in_item);
        while(set_of_cells.size()<num_cells_in_item)
        {
            int x=rand(1,ROWS);
            int y=rand(1,COLS);
            pair<int,int> curr_cell={x,y};
            if(set_of_cells.find(curr_cell)!=set_of_cells.end())
                continue;
            set_of_cells.insert({x,y});
        }
        cout<<num_cells_in_item<<endl;
        for(auto &curr_cell:set_of_cells)
            cout<<curr_cell.first<<" "<<curr_cell.second<<endl;
        cout<<endl;
    }

    for(int i = 0; i < num_of_orders; i++){
        set<int>set_of_items;
        int order_size=rand(1,max_order_size);
        while(set_of_items.size()<order_size){
            int item_number=rand(0,number_of_total_items-1);
            if(set_of_items.find(item_number)!=set_of_items.end())
                continue;
            set_of_items.insert(item_number);
        }
        cout << order_size << endl;
        for(auto &item_number:set_of_items) 
            cout << item_number<< endl;
        cout << endl;
    }

}


int main(){
    FASTIO;
    freopen("input.txt", "w", stdout);
    int test_case = 1;
    cout << test_case << endl;
    while(test_case--){
        out_for_test();
    }
}
