//DO NOOOOOOOOOOOOOOOOOOOOOOOOT TOUCH please
vector<vector<vector<vector<int>>>> dp; //dp[mask][last_element][last_index] = {distance,last_element,last_index}
vector<Item> a;
int tot_elements;

int recur(int mask,int curr_element,int curr_index){
    if(mask == 0) return inf;
    if(dp[mask][curr_element][curr_index][0] != -1) return dp[mask][curr_element][curr_index][0];
    vector<int> arr = {inf,0,0};
    for(int i = 0; i < tot_elements; i++){
        if(((mask>>i)&1) == 0 || curr_element == i) continue;
        for(int j = 0; j < a[i].cells.size(); j++){
            arr = min(arr,{recur(mask^(1<<curr_element),i,j) + distance(a[curr_element].cells[curr_index],a[i].cells[j]) + docking_time,i,j});
        }
    }
    dp[mask][curr_element][curr_index] = arr;
    return dp[mask][curr_element][curr_index][0];

}

pair<int,vector<pair<int,Cell>>> tsp(vector<int> r){
    dp.clear();
    a.clear();
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
            tmp = min(tmp,{recur(tot_masks - 1,i,j) + distance(a[i].cells[j],{0,0}) + docking_time,i,j});
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
    return {tmp[0],ans};
}
//END of DO NOOOOOOOOOOOOOOOOOT TOUCH
