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
    tmp.cells.clear();
    tmp.cells.push_back({0,0});
    a.push_back(tmp);
    int n = a.size();
    vector<vector<int>> dp(n + 1,vector<int>(6+1,inf));
    dp[0][0] = 0;
    for(int i= 1; i < n; i++){
        for(int j = 0; j < a[i].cells.size(); j++){
            for(int k = 0; k < a[i - 1].cells.size(); k++){
                dp[i][j] = min(dp[i][j],distance(a[i].cells[j],a[i - 1].cells[k]) + dp[i - 1][k]);
            }
        }
    }
    return {*min_element(dp[n - 1].begin(),dp[n - 1].end())+arr.size()*docking_time,{{1,{}}}};
}

int recur(int mask,int curr_element,int curr_index,vector<vector<vector<vector<int>>>>&dp,vector<Item>&a,int tot_elements){
    if(mask == 0) return inf;
    if(dp[mask][curr_element][curr_index][0] != -1) return dp[mask][curr_element][curr_index][0];
    vector<int> arr = {inf,0,0};
    for(int i = 0; i < tot_elements; i++){
        if(((mask>>i)&1) == 0 || curr_element == i) continue;
        for(int j = 0; j < a[i].cells.size(); j++){
            arr = min(arr,{recur(mask^(1<<curr_element),i,j,dp,a,tot_elements) + distance(a[curr_element].cells[curr_index],a[i].cells[j]),i,j});
        }
    }
    dp[mask][curr_element][curr_index] = arr;
    return dp[mask][curr_element][curr_index][0];
}
pair<int,vector<pair<int,Cell>>> optimal_TSP(vector<int> r){
    vector<vector<vector<vector<int>>>> dp; //dp[mask][last_element][last_index] = {distance,last_element,last_index}
    vector<Item> a;
    int tot_elements;
    Item tt;
    tt.cells.push_back({0,0});
    a.push_back(tt);
    for(int i = 0; i < r.size(); i++){
        a.push_back(allItems[r[i]]);
    }
    tot_elements = a.size();
    int tot_masks = (1<<tot_elements);
    vector<int> tmp = {-1,0,0};
    dp.resize(tot_masks,vector<vector<vector<int>>>(tot_elements,vector<vector<int>>(6,tmp)));
    tmp[0] = inf;
    dp[1][0][0] = {0,-1,-1};
    for(int i = 1; i < tot_elements; i++){
        for(int j = 0; j < a[i].cells.size(); j++){
            tmp = min(tmp,{recur(tot_masks - 1,i,j,dp,a,tot_elements) + distance(a[i].cells[j],{0,0}),i,j});
        }
    }
    int main_mask = tot_masks - 1;
    int last_element = tmp[1];
    int last_cell = tmp[2];
    vector<pair<int,Cell>> ans;
    while(last_element > 0){
        ans.push_back({last_element,a[last_element].cells[last_cell]});
        int new_last_element = dp[main_mask][last_element][last_cell][1];
        int new_last_cell = dp[main_mask][last_element][last_cell][2];
        main_mask ^= (1<<last_element);
        last_element = new_last_element;
        last_cell = new_last_cell;
    }
    for(auto &x:ans) x.first = r[x.first - 1];
    return {tmp[0]+r.size()*docking_time,ans};
}
