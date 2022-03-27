// #pragma GCC optimize("O2")
#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
using namespace std;
#include<time.h>
#define FASTIO	ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)
const int inf = 1e9;

int ROWS,COLS;
int docking_time;
int num_of_robots;
int num_of_orders;
double velocity;
int max_capacity_robot;
int number_of_total_items;

// Structure to store details of a single cell
struct Cell{
    int x,y; 
    bool operator<(const Cell &o)  const {
        return (x < o.x) || (x == o.x && y < o.y);
    }
    friend ostream& operator << (ostream &os, const Cell &m) {return os << "{" << m.x << "," << m.y << "}";}
};

// Structure to store details of a single item
struct Item{
    int item_number;
    vector<Cell>cells;  // cells where the item is availavle
    void print_cells(){
        cout << "{";
        for(auto &x:cells) cout << x;
        cout << "}";
        cout << endl;
    }
};

// Structure to store all the details related to a single order
struct Order{
    vector<int>items;  // Item Numbers of all items in the order
    vector<pair<int,Cell>>optimalpath;  // Contains the sequence of steps robot needs to follow to cater the order
                                        // Each element (item_number,cell) : Go to this cell and pick up this item
    vector<int>subOrderIndexes;
    int time;
    Order(){   
        time = 0;
    }
    int getOrderSize(){
        return items.size();
    }
};

int distance(Cell a,Cell b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}



vector<Order>allOrders;
vector<Item>allItems;


int shortest_path_time(vector<int> arr){
    vector<Item> a;
    Item tmp;
    tmp.cells.push_back({0,0});
    a.push_back(tmp);
    for(auto &x:arr){
        tmp.cells.clear();
        for(auto &c:allItems[x].cells){
            tmp.cells.push_back(c);
        } 
        a.push_back(tmp);
    }
    int n = a.size();
    int max_cell = 5;
    vector<vector<int>> dp(n + 1,vector<int>(max_cell,inf));
    dp[0][0] = 0;
    for(int i= 1; i < n; i++){
        for(int j = 0; j < a[i].cells.size(); j++){
            for(int k = 0; k < a[i - 1].cells.size(); k++){
                dp[i][j] = min(dp[i][j],distance(a[i].cells[j],a[i - 1].cells[k]) + dp[i - 1][k]);
            }
        }
    }
    return *min_element(dp[n - 1].begin(),dp[n - 1].end());

}

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity>>max_capacity_robot;
    cin>>num_of_robots>>num_of_orders;
    cin>>number_of_total_items;

    allOrders.clear();
    allItems.clear();    

    for(int i=0;i<number_of_total_items;i++)
    {
        int num_cells_in_item;
        cin>>num_cells_in_item;
        Item currItem=Item();
        currItem.item_number=i;
        for(int j=0;j<num_cells_in_item;j++)
        {
            Cell currCell;
            cin>>currCell.x>>currCell.y;
            currItem.cells.push_back(currCell);
        }
        allItems.push_back(currItem);
    }

    for(int i = 0 ; i < num_of_orders;++i)
    {
        int currOrderSize;
        cin>>currOrderSize;
        Order currOrder = Order();
        currOrder.subOrderIndexes.push_back(i);
        for(int j = 0 ; j < currOrderSize ; ++j)
        {
            int item_number;
            cin>>item_number;
            currOrder.items.push_back(item_number);
        }
        allOrders.push_back(currOrder);
        for(auto &x:currOrder.items) cout << x << " ";
        cout << endl;
        cout << shortest_path_time(currOrder.items) << endl;
    }    
}

void cal_for_given_test(){
    take_input();
    
    // cout<<"----greedy_tsp:----"<<endl;
    // clock_t tStart=clock();

    // pair<int,vector<vector<int>>> cateringData = caterAllOrders();
    // int totalTimeTaken=cateringData.first;
    // vector<vector<int>>robotTasks=cateringData.second; // For each robot, it stores which orders will be catered by that robot

    // double exec_time=(double)(clock()-tStart)/CLOCKS_PER_SEC;
    
    // cout<<"Code Execution Time (in min) : \n"<<exec_time/60<<endl;
    // cout<<"Total Catering Time (in hrs) : \n"<<((totalTimeTaken*1.0)/velocity)/60<<"\n"<<endl;

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
