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

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
    return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
    return uniform_int_distribution<T>()(rng);
}




const int max_docking_time=0;
const int max_number_robots = 3;
const int max_number_order = 1;
const int max_cell_in_order = 5;
const int max_velocity = 5;




int ROWS=3,COLS=3;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;

int main(){
    freopen("input.txt", "w", stdout);

    docking_time = rand(0,max_docking_time);
    num_of_orders = rand(1,max_number_order);
    num_of_robots = rand(1,max_number_robots);
    velocity = rand(1,max_velocity);

    cout << ROWS << " "  << COLS << " " <<  docking_time << " " << velocity << endl;
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
