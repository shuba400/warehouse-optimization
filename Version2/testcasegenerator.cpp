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
                    d*v = 12 // taking d as 9s and v as 80.4672metre/minute
*/

/*
    Warehouse size : 810,000 sq ft => 900ft x 900ft => 270m x 270m ~~ 250m x 250m 
    robot velocity : 80.4672 metre/minute     1.3m/s 
    no. of robots  : 800
    no. of orders  : 1.6*10^5 
    docking time   : no data 
                     taking it as 9s = 0.15minutes => d*v = 12.07, round off to 12 
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
const int max_number_robots = 5;      // 800 robots can be deployed in one floor
// right now taking order count as 1.6*10^4
//  total order tha "whole" amazon receives 1.6*10^6
// assuming at least 100 warehouses
// will try to find data 
const int max_number_order = 50;   // max number of orders a warehouse shifts | 
                                    // amazon gets 1.6 million orders, but a warehouse will cater smaller no. of orders
const int max_cell_in_order = 5;
const int max_velocity = 2;           // 1.3m/s
const int capacity_of_robot = 10;    //robot can pick 7 items at total


int ROWS = 250, COLS = 250;     // 250m x 250m 
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;

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

    cout << ROWS << " "  << COLS << " " <<  docking_time << " " << velocity << " " << capacity_of_robot << endl;
    cout << num_of_robots << " " << num_of_orders << endl;
    for(int i = 0; i < num_of_orders; i++){
        set<pair<int,int>> set_of_order;
        vector<pair<int,int>> curr_order;
        int tot_cells = rand(1,max_cell_in_order);
        while(curr_order.size() < tot_cells){
            int x = rand(1,ROWS);
            int y = rand(1,COLS);
            pair<int,int> curr_cell = {x,y};
            if(set_of_order.find(curr_cell) != set_of_order.end()) continue;
            set_of_order.insert(curr_cell);
            curr_order.push_back(curr_cell);
        }
        cout << curr_order.size() << endl;
        for(auto &x:curr_order) cout << x.first << " " << x.second << endl;
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