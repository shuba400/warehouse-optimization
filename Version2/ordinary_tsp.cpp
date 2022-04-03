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
    }    
}

bool compOrdersDescSize(Order &order1,Order &order2)
{
    return order1.getOrderSize()>order2.getOrderSize();
}
bool compOrdersDescTime(Order &order1,Order &order2)
{
    return order1.time>order2.time;
}
Order mergeTwoOrders(Order order1,Order order2)  //Adds all items from order2 to order1 
{
    for(int i=0;i<order2.items.size();i++)
        order1.items.push_back(order2.items[i]);
    for(int i=0;i<order2.subOrderIndexes.size();i++)
        order1.subOrderIndexes.push_back(order2.subOrderIndexes[i]);
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
// Iterates over all orders and greedily merges the largest possible order into current order
vector<Order> greedyMergeOrdersSize(vector<Order>orderList)  
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);
    sort(orderList.begin(),orderList.end(),compOrdersDescSize);    
    
    set<pair<int,int>>sizeSet;   //stores {orderSize,orderIndex}
    for(int i=0;i<n;i++)
        sizeSet.insert({orderList[i].getOrderSize(),i});

    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)!=i)
            continue;
        while(1)
        {
            int curOrderIndex=dsu_find(i,orderParent);
            int curOrderSize=orderList[curOrderIndex].getOrderSize();
            sizeSet.erase({curOrderSize,curOrderIndex});

            pair<int,int>tempPair={max_capacity_robot-curOrderSize,inf};
            auto it=sizeSet.upper_bound(tempPair);
            if(sizeSet.empty()||it==sizeSet.begin())
            {
                sizeSet.insert({curOrderSize,curOrderIndex});
                break;
            }
            it--;
            int partnerOrderIndex=(*it).second;
            int partnerOrderSize=orderList[partnerOrderIndex].getOrderSize();

            dsu_merge(curOrderIndex,partnerOrderIndex,orderParent,orderList);
            int newOrderIndex=dsu_find(curOrderIndex,orderParent);
            int newOrderSize=orderList[newOrderIndex].getOrderSize();

            sizeSet.erase({partnerOrderSize,partnerOrderIndex});
            sizeSet.insert({newOrderSize,newOrderIndex});
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

pair<int,vector<pair<int,Cell>>> ordinary_TSP(vector<int> arr){ //takes item index (index in that was being used in main item vector)
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
    vector<vector<int>> dp(n + 1,vector<int>(20+1,inf));
    dp[0][0] = 0;
    for(int i= 1; i < n; i++){
        for(int j = 0; j < a[i].cells.size(); j++){
            for(int k = 0; k < a[i - 1].cells.size(); k++){
                dp[i][j] = min(dp[i][j],distance(a[i].cells[j],a[i - 1].cells[k]) + dp[i - 1][k]);
            }
        }
    }
    return {*min_element(dp[n - 1].begin(),dp[n - 1].end()),{{1,{}}}};
}

pair<int,vector<pair<int,Cell>>> nearest_neighbour_TSP(vector<int>items)
{
    int n=items.size();
    int time=0;
    // path[i] = {item id , cell for the item id}
    vector<pair<int,Cell>>path;
    vector<bool>isItemVisited(n,false);
    Cell currentCell={0,0};
    path.push_back({-1,currentCell});
    while(path.size()<n)
    {
        int nextItem;
        Cell nextCell;
        int nextDistance=inf;
        for(int i=0;i<n;i++)
        {
            if(isItemVisited[i]==true)
                continue;
            Cell bestCell;
            int bestDistance=inf;
            for(auto &cell:allItems[items[i]].cells)
            {
                if(distance(currentCell,cell)<bestDistance)
                {
                    bestDistance=distance(currentCell,cell);
                    bestCell=cell;
                }
            }
            if(bestDistance<nextDistance)
            {
                nextDistance=bestDistance;
                nextCell=bestCell;
                nextItem=i;
            }
        }
        path.push_back({allItems[items[nextItem]].item_number,nextCell});
        time+=nextDistance;
    }
    path.push_back({-1,currentCell});
    time+=distance(currentCell,{0,0});
    time+=n*docking_time;
    return {time,path};
}

// Returns time taken to cater all orders by all robots
pair<int,vector<vector<int>>> caterAllOrders()
{   
    vector<Order>mergedOrders=allOrders;
    cout<<"Number of Orders (Initially) : \n"<<mergedOrders.size()<<endl;
    mergedOrders=greedyMergeOrdersSize(mergedOrders);
    cout<<"Number of Orders (After Greedy_Merging_Size) : \n"<<mergedOrders.size()<<endl;

    for(int i = 0; i < mergedOrders.size(); i++)
    {
        pair<int,vector<pair<int,Cell>>>temp=ordinary_TSP(mergedOrders[i].items);
        mergedOrders[i].time=temp.first;
        mergedOrders[i].optimalpath=temp.second;
    }
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

void cal_for_given_test(){
    take_input();
    
    cout<<"----greedy_tsp:----"<<endl;
    clock_t tStart=clock();

    pair<int,vector<vector<int>>> cateringData = caterAllOrders();
    int totalTimeTaken=cateringData.first;
    vector<vector<int>>robotTasks=cateringData.second; // For each robot, it stores which orders will be catered by that robot

    double exec_time=(double)(clock()-tStart)/CLOCKS_PER_SEC;
    
    cout<<"Code Execution Time (in min) : \n"<<exec_time/60<<endl;
    cout<<"Total Catering Time (in hrs) : \n"<<((totalTimeTaken*1.0)/velocity)/60<<"\n"<<endl;

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
