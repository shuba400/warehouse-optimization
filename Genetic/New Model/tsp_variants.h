pair<int,vector<pair<int,Cell>>> nearest_neighbour_TSP(vector<int>items)
{
    int n=items.size();
    int time=0;
    // path[i] = {item id , cell for the item id}
    vector<pair<int,Cell>>path;
    vector<bool>isItemVisited(n,false);
    Cell currentCell={0,0};
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
        isItemVisited[nextItem] = true;
        path.push_back({items[nextItem],nextCell});
        currentCell=nextCell;
        time+=nextDistance;
    }
    time+=distance(currentCell,{0,0});
    time+=n*docking_time;
    return {time,path};
}

int ordinary_TSP(vector<int>item_numbers)
{
    vector<Item>items;
    Item tmp;
    tmp.cells.push_back({0,0});
    items.push_back(tmp);
    for(auto &x:item_numbers){
        tmp.cells.clear();
        for(auto &c:allItems[x].cells){
            tmp.cells.push_back(c);
        } 
        items.push_back(tmp);
    }
    int n = items.size();
    vector<vector<int>> dp(n + 1,vector<int>(max_cells_in_item+1,inf));
    dp[0][0] = 0;
    for(int i= 1; i < n; i++){
        for(int j = 0; j < items[i].cells.size(); j++){
            for(int k = 0; k < items[i - 1].cells.size(); k++){
                dp[i][j] = min(dp[i][j],distance(items[i].cells[j],items[i - 1].cells[k]) + dp[i - 1][k]);
            }
        }
    }
    return *min_element(dp[n - 1].begin(),dp[n - 1].end());
}
