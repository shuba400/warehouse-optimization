vector<Order> FCFS_merge(vector<Order>orderList)  
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);

    for(int i=1;i<n;i++)
    {
        int last=dsu_find(i-1,orderParent);
        int cur=dsu_find(i,orderParent);
        if(orderList[last].getOrderSize()+orderList[cur].getOrderSize()<=max_capacity_robot)
            dsu_merge(last,cur,orderParent,orderList);
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

vector<Order> LSOS_merge(vector<Order>orderList)  
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);
    sort(orderList.begin(),orderList.end(),compOrdersDescSize);

    for(int i=1;i<n;i++)
    {
        int last=dsu_find(i-1,orderParent);
        int cur=dsu_find(i,orderParent);
        if(orderList[last].getOrderSize()+orderList[cur].getOrderSize()<=max_capacity_robot)
            dsu_merge(last,cur,orderParent,orderList);
    }

    vector<Order>mergedOrders;
    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)==i)
            mergedOrders.push_back(orderList[i]);
    }
    return mergedOrders;
}

vector<Order> SLOS_merge(vector<Order>orderList)  
{
    int n=orderList.size();
    vector<int>orderParent(n);
    iota(orderParent.begin(),orderParent.end(),0);
    sort(orderList.begin(),orderList.end(),compOrdersAscSize);

    for(int i=1;i<n;i++)
    {
        int last=dsu_find(i-1,orderParent);
        int cur=dsu_find(i,orderParent);
        if(orderList[last].getOrderSize()+orderList[cur].getOrderSize()<=max_capacity_robot)
            dsu_merge(last,cur,orderParent,orderList);
    }

    vector<Order>mergedOrders;
    for(int i=0;i<n;i++)
    {
        if(dsu_find(i,orderParent)==i)
            mergedOrders.push_back(orderList[i]);
    }
    return mergedOrders;
}