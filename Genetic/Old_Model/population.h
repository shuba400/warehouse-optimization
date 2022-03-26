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
    for(int i=0;i<order2.cells.size();i++)
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
            for(int j=0;j<orderList[curOrderIndex].cells.size();j++)
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

// void cal_for_given_test(){
//     vector<vector<int>> a = populate_by_GeometryOnly(30);
//     vector<vector<int>> b = populate_by_GeometryAndGreedy(20);
//     vector<vector<int>> c = populate_by_GreedyOnly();
//     for(auto &x:a){
//         cout << "{ ";
//         for(auto &y:x) cout << y << " ";
//         cout << " } ";
//     }
//     cout << endl;
//     for(auto &x:b){
//         cout << "{ ";
//         for(auto &y:x) cout << y << " ";
//         cout << " } ";
//     }
//     cout << endl;
//     for(auto &x:c){
//         cout << "{ ";
//         for(auto &y:x) cout << y << " ";
//         cout << " } ";
//     }
//     return;
// }
