#include<bits/stdc++.h>
using namespace std;
#define FASTIO	ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)

const double velocity = 80.4672; // metre per minute
int main(){
    FASTIO;
    vector<int> fcfs_approach,sort_approach;
    ifstream inFile1("output_bruteForce_fcfsGreedy_v0.txt"); 
    ifstream inFile2("output_bruteForce_sortedGreedy_v0.txt");
    ifstream inFile3("input.txt");
    int test_case;
    inFile3 >> test_case;
    for(int i = 0; i < test_case; i++){
        string a,b;
        int time = -1;
        getline(inFile1,a);
        getline(inFile1,b);
        time = stoi(b);
        fcfs_approach.push_back(time);
        getline(inFile2,a);
        getline(inFile2,b);
        time = stoi(b);
        sort_approach.push_back(time);
    }
    cout<<"\n Time in minutes\n";
    cout << "fcfs_approach" << " ";
    for(auto &x:fcfs_approach) cout << (x*1.0)/velocity << " ";
    cout << endl;
    cout << "sort_approach" << " ";
    for(auto &x:sort_approach) cout << (x*1.0)/velocity << " ";
    cout << endl;

    cout << "Better_Approach_FCFS" << " ";
    for(int i = 0; i < test_case; i++){
        cout << (sort_approach[i] >= fcfs_approach[i]) << " ";
    }
    cout << endl;
    cout << "Better_Approach_SORT" << " ";
    for(int i = 0; i < test_case; i++){
        cout << (sort_approach[i] <= fcfs_approach[i]) << " ";
    }
    cout<<endl;
    // percentage better 
    cout<<"\nPercentage improvement (better one)\n";
    cout << "FCFS" << " ";
    for(int i = 0; i < test_case; i++){
        if((sort_approach[i] >= fcfs_approach[i])){
            int diff = sort_approach[i] - fcfs_approach[i];
            double prcnt = ((diff*1.0)/sort_approach[i])*(100.0);
            cout<<prcnt<<"% ";
        }
        else{
            cout<<"0% ";
        }
    }
    cout << endl;
    cout << "SORT" << " ";
    for(int i = 0; i < test_case; i++){
        if((sort_approach[i] <= fcfs_approach[i])){
            int diff = fcfs_approach[i] - sort_approach[i];
            double prcnt = ((diff*1.0)/fcfs_approach[i])*(100.0);
            cout<<prcnt<<"% ";
        }
        else{
            cout<<"0% ";
        }
    }
}