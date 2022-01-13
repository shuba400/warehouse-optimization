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

    public:
    vector<Cell>locations;  // to store coordinates of each item in current order
    int time;
    vector<Cell> optimalpath;
    pair<int,pair<int,int>> main_info;

    void orderSize(int szOrder){
        locations = vector<Cell>(szOrder);
    }

    Order(int x,int y){      //This will store the location of starting cell  of the order ==> Mainly the human counter
        NewLocation(x,y);
        main_info = {inf,{-1,-1}};
    }

    void NewLocation(int x,int y){  //takes new order for the cell and calculates the time
        Cell c;
        c.x = x;
        c.y = y;
        locations.push_back(c);
    }

    int recur_relation(int mask,int last_element){
        if(mask == 0) return inf;
        if(dp[mask][last_element].first != -1) return dp[mask][last_element].first;
        dp[mask][last_element].first = inf;
        for(int i = 0; i < locations.size(); i++){
            if(((mask>>i)&1) == 0 || last_element == i) continue;
            pair<int,pair<int,int>> temp = {recur_relation(mask^(1<<last_element),i) + distance(locations[i],locations[last_element]) + docking_time,{(mask^(1<<last_element)),i}};

            dp[mask][last_element] = min(dp[mask][last_element],temp);
        }
        return dp[mask][last_element].first;
    }

    void start_recurrence(){  //used travelling salesman problem to get the minimum time needed to complete the order with given docking time.
        time = 0;
        int tot_number_of_element = locations.size();                          
        pair<int,pair<int,int>> filler_value = {-1,{-1,-1}};  //used to intialize the dp states
        dp.resize(1<<tot_number_of_element,vector<pair<int,pair<int,int>>>(tot_number_of_element,filler_value));
        dp[1][0] = {0,{-1,-1}};   //current mask = 0000001, current last cell is  0, this is basically saying that when element is at position 0 , t = 0 intially
        for(int i = 1; i < tot_number_of_element; i++){
            pair<int,pair<int,int>> p = {recur_relation((1<<tot_number_of_element) - 1,i) + distance(locations[i],locations[0]),{(1<<tot_number_of_element) - 1,i}};
            main_info = min(main_info,p);
        }
        return;
    }

    int getTime(){
        if(dp.empty()) start_recurrence();
        return main_info.first;
    }

    vector<Cell> getPath(){
        if(!optimalpath.empty()) return optimalpath;
        if(dp.empty()) start_recurrence();
        int main_mask = main_info.second.first;
        int last_element = main_info.second.second;
        optimalpath.push_back(locations[0]);
        while(last_element > -1){
            optimalpath.push_back(locations[last_element]);
            int new_last_element = dp[main_mask][last_element].second.second;
            int new_main_mask = dp[main_mask][last_element].second.first;
            last_element = new_last_element;
            main_mask = new_main_mask;
        }
        return optimalpath;
    }
    
    int getOrderSize(){
        return locations.size();
    }
};

vector<Order>allOrders;  // vector containing details of all orders

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity;
    cin>>num_of_robots>>num_of_orders;
    for(int i = 0 ; i < num_of_orders;++i){

        int currOrderSize;
        cin>>currOrderSize;
        Order currOrder = Order(0,0);   //this will be the main human cell, for now I am assuming this is 0,0
        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            int x,y;
            cin >> x >> y;
            currOrder.NewLocation(x,y);
        }
        allOrders.push_back(currOrder);
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


int main(){
    freopen("input.txt","r",stdin);
    take_input();
    cout << allOrders[0].getTime() << endl;
    vector<Cell> path = allOrders[0].getPath();
    for(auto &c:path){
        cout << c.x << ' ' << c.y << endl;
    }
}