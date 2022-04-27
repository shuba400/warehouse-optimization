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

vector<Order> CW1_merge(vector<Order>orderList)  
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);

    vector<pair<int,pair<int,int>>>savings_values;
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(orderList[i].getOrderSize()+orderList[j].getOrderSize()>max_capacity_robot)
                continue;
            pair<int,vector<pair<int,Cell>>>temp;
            temp=nearest_neighbour_TSP(orderList[i].items);
            orderList[i].time=temp.first;
            orderList[i].optimalpath=temp.second;
            temp=nearest_neighbour_TSP(orderList[j].items);
            orderList[j].time=temp.first;
            orderList[j].optimalpath=temp.second;
            Order mergedOrder=mergeTwoOrders(orderList[i],orderList[j]);
            temp=nearest_neighbour_TSP(mergedOrder.items);
            mergedOrder.time=temp.first;
            mergedOrder.optimalpath=temp.second;
            int sav1=orderList[i].time;
            int sav2=orderList[j].time;
            int sav12=mergedOrder.time;
            int sav=sav1+sav2-sav12;
            savings_values.push_back({sav,{i,j}});
        }
    }
    sort(savings_values.begin(),savings_values.end());
    reverse(savings_values.begin(),savings_values.end());
    for(auto &it:savings_values)
    {
        int orderIndex1=dsu_find(it.second.first,orderParent);
        int orderIndex2=dsu_find(it.second.second,orderParent);
        if(orderList[orderIndex1].subOrderIndexes.size()>1&&orderList[orderIndex2].subOrderIndexes.size()>1)
            continue;
        if(orderList[orderIndex1].getOrderSize()+orderList[orderIndex2].getOrderSize()>max_capacity_robot)
            continue;
        dsu_merge(orderIndex1,orderIndex2,orderParent,orderList);
    }

    vector<Order>mergedOrders;
    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)==i)
            mergedOrders.push_back(orderList[i]);
    }
    return mergedOrders;
}

vector<Order> CW2_merge(vector<Order>orderList)  
{
    while(1)
    {
        int n=orderList.size();
        vector<int>orderParent(n);
        iota(orderParent.begin(),orderParent.end(),0);

        vector<int>savings_single(n);
        for(int i=0;i<n;i++)
        {
            pair<int,vector<pair<int,Cell>>>temp=nearest_neighbour_TSP(orderList[i].items);
            orderList[i].time=temp.first;
            orderList[i].optimalpath=temp.second;
            savings_single[i]=orderList[i].time;
        }
        vector<pair<int,pair<int,int>>>savings_pair;
        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                if(orderList[i].getOrderSize()+orderList[j].getOrderSize()>max_capacity_robot)
                    continue;
                Order mergedOrder=mergeTwoOrders(orderList[i],orderList[j]);
                pair<int,vector<pair<int,Cell>>>temp=nearest_neighbour_TSP(mergedOrder.items);
                mergedOrder.time=temp.first;
                mergedOrder.optimalpath=temp.second;
                int sav=savings_single[i]+savings_single[j]-mergedOrder.time;
                savings_pair.push_back({sav,{i,j}});
            }
        }

        sort(savings_pair.begin(),savings_pair.end());
        reverse(savings_pair.begin(),savings_pair.end());
        int flag=0;
        for(auto &it:savings_pair)
        {
            int orderIndex1=dsu_find(it.second.first,orderParent);
            int orderIndex2=dsu_find(it.second.second,orderParent);
            if(orderList[orderIndex1].getOrderSize()+orderList[orderIndex2].getOrderSize()>max_capacity_robot)
                continue;
            dsu_merge(orderIndex1,orderIndex2,orderParent,orderList);
            flag++;
            break;
        }
        if(flag==0)
            break;        
        vector<Order>mergedOrders;
        for(int i=0;i<n;i++)
        {
            if(dsu_find(i,orderParent)==i)
                mergedOrders.push_back(orderList[i]);
        }
        orderList=mergedOrders;
    }
    return orderList;
}