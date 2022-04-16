mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
    return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
    return uniform_int_distribution<T>()(rng);
}

vector<int> two_opt(vector<int> arr){
    if(arr.size() == 1) return arr;
    int n = arr.size();
    int a = rand(0,n - 2);
    int b = rand(a + 1,n - 1);
    vector<int> ans;
    for(int i = 0; i < a; i++) ans.push_back(arr[i]);
    for(int i = b; i >= a; i--) ans.push_back(arr[i]);
    for(int i = b + 1; i < n; i++) ans.push_back(arr[i]);
    return ans;
}
