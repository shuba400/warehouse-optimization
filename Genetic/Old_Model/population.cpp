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
    vector<int> index;
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
        currOrder.index.push_back(i);
        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            cin >> tmp.x >> tmp.y;
            currOrder.cells.push_back(tmp);
            L[tmp.x][tmp.y].push_back(i);
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

Order mergeTwoOrders(Order &order1,Order &order2)  //Adds all cells from order2 to order1 
{
    for(int i=1;i<order2.cells.size();i++)
        order1.cells.push_back(order2.cells[i]);
    for(int i = 0; i < order2.index.size(); i++){ 
        order1.index.push_back(order2.index[i]);
    }
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


int distance_between_orders(Order &a,Order &b){
    int ans = 0;
    for(Cell &bc:b.cells){
        int mini = 1e9;
        for(Cell &ac:a.cells){
            mini = min(mini,distance(ac,bc));
        }
        ans += mini;
    }
    return ans;
}

vector<Order> geoMergeOrders(vector<Order>orderList,int distanceThreshold)
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
                    for(int dy=-(distanceThreshold-abs(dx));dy<=(distanceThreshold-abs(dx));dy++)
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
            int smallestDistance= inf;
            for(auto &it:freq)
            {
                int neighbourOrderIndex=it.first;
                int neighbourOrderSize=orderList[neighbourOrderIndex].getOrderSize();
                if(neighbourOrderSize+orderList[curOrderIndex].getOrderSize()>max_capacity_robot) continue;
                int overlapDistance = distance_between_orders(orderList[neighbourOrderIndex],orderList[curOrderIndex]);
                if(smallestDistance>overlapDistance){
                    smallestDistance=overlapDistance;
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

// Returns time taken to cater all orders by all robots
vector<vector<int>> populate_by_GeometryAndGreedy(int distanceThreshold = 5)
{   
    vector<Order>mergedOrders=allOrders;
    mergedOrders=geoMergeOrders(mergedOrders,distanceThreshold);
    mergedOrders=greedyMergeOrdersSize(mergedOrders);
    vector<vector<int>> ans;
    for(auto &m:mergedOrders) ans.push_back(m.index);
    return ans;
}

vector<vector<int>> populate_by_GeometryOnly(int distanceThreshold = 5){
    vector<Order>mergedOrders=allOrders;
    mergedOrders=geoMergeOrders(mergedOrders,distanceThreshold);
    vector<vector<int>> ans;
    for(auto &m:mergedOrders) ans.push_back(m.index);
    return ans;
}

vector<vector<int>> populate_by_GreedyOnly(){
    vector<Order>mergedOrders=allOrders;
    mergedOrders=greedyMergeOrdersSize(mergedOrders);
    vector<vector<int>> ans;
    for(auto &m:mergedOrders) ans.push_back(m.index);
    return ans;
}

void cal_for_given_test(){
    take_input();
    vector<vector<int>> a = populate_by_GeometryOnly(30);
    vector<vector<int>> b = populate_by_GeometryAndGreedy(20);
    vector<vector<int>> c = populate_by_GreedyOnly();

    for(auto &x:a){
        cout << "{ ";
        for(auto &y:x) cout << y << " ";
        cout << " } ";
    }
    cout << endl;
    for(auto &x:b){
        cout << "{ ";
        for(auto &y:x) cout << y << " ";
        cout << " } ";
    }
    cout << endl;
    for(auto &x:c){
        cout << "{ ";
        for(auto &y:x) cout << y << " ";
        cout << " } ";
    }

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
