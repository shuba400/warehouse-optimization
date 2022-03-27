int shortest_path_time(vector<int> arr){ //takes item index (index in that was being used in main item vector)
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
