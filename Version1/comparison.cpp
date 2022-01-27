#include<bits/stdc++.h>
using namespace std;
const double velocity = 80.4672; // metre per minute
int main(){
    vector<int> randomMerge,sort_approach;
    ifstream inFile1("output_dp_randomMerge.txt"); 
    ifstream inFile2("output_dp_Sorted.txt");
    ifstream inFile3("input.txt");
    int test_case;
    inFile3 >> test_case;
    for(int i = 0; i < test_case; i++){
        string a,b;
        int time = -1;
        getline(inFile1,a);
        getline(inFile1,b);
        time = stoi(b);
        randomMerge.push_back(time);
        getline(inFile2,a);
        getline(inFile2,b);
        time = stoi(b);
        sort_approach.push_back(time);
    }
    cout<<"\n Time in minutes\n";
    cout << "randomMerge" << " ";
    for(auto &x:randomMerge) cout << (x*1.0)/velocity << " ";
    cout << endl;
    cout << "sort_approach" << " ";
    for(auto &x:sort_approach) cout << (x*1.0)/velocity << " ";
    cout << endl;

    cout << "randomMerge" << " ";
    for(int i = 0; i < test_case; i++){
        cout << (sort_approach[i] >= randomMerge[i]) << " ";
    }
    cout << endl;
    cout << "Better_Approach_SORT" << " ";
    for(int i = 0; i < test_case; i++){
        cout << (sort_approach[i] <= randomMerge[i]) << " ";
    }
    cout<<endl;
    // percentage better 
    cout<<"\nPercentage improvement (better one)\n";
    cout << "randomMerge" << " ";
    for(int i = 0; i < test_case; i++){
        if((sort_approach[i] >= randomMerge[i])){
            int diff = sort_approach[i] - randomMerge[i];
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
        if((sort_approach[i] <= randomMerge[i])){
            int diff = randomMerge[i] - sort_approach[i];
            double prcnt = ((diff*1.0)/randomMerge[i])*(100.0);
            cout<<prcnt<<"% ";
        }
        else{
            cout<<"0% ";
        }
    }
}