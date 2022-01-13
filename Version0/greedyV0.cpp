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


int main(){
    freopen("input.txt","r",stdin);
    take_input();
    printTestCaseDetails();
}