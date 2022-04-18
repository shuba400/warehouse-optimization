// #pragma GCC optimize("O2")
#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
    return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
    return uniform_int_distribution<T>()(rng);
}
const int inf = 1e9;
int seed=12345678;
#include "random_values.h"

// -----------------------------
int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
double velocity;
int max_capacity_robot;
int number_of_total_items;
int max_cells_in_item=5;

// Structure to store details of a single cell
struct Cell{
    int x,y; 
    bool operator<(const Cell &o)  const {
        return (x < o.x) || (x == o.x && y < o.y);
    }
    friend ostream& operator << (ostream &os, const Cell &m) {return os << "{" << m.x << "," << m.y << "}";}
};

// Structure to store details of a single item
struct Item{
    int item_number;
    vector<Cell>cells;  // cells where the item is availavle
};

// Structure to store all the details related to a single order
struct Order{
    vector<int>items;  // Item Numbers of all items in the order
    vector<pair<int,Cell>>optimalpath;  // Contains the sequence of steps robot needs to follow to cater the order
                                        // Each element (item_number,cell) : Go to this cell and pick up this item
    vector<int>subOrderIndexes;
    int time;
    Order(){   
        time = 0;
    }
    int getOrderSize(){
        return items.size();
    }
};

int distance(Cell a,Cell b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}

vector<Order>allOrders;
vector<Item>allItems;

void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity>>max_capacity_robot;
    cin>>num_of_robots>>num_of_orders;
    cin>>number_of_total_items;

    allOrders.clear();
    allItems.clear();    

    for(int i=0;i<number_of_total_items;i++)
    {
        int num_cells_in_item;
        cin>>num_cells_in_item;
        Item currItem=Item();
        currItem.item_number=i;
        for(int j=0;j<num_cells_in_item;j++)
        {
            Cell currCell;
            cin>>currCell.x>>currCell.y;
            currItem.cells.push_back(currCell);
        }
        allItems.push_back(currItem);
    }

    for(int i = 0 ; i < num_of_orders;++i)
    {
        int currOrderSize;
        cin>>currOrderSize;
        Order currOrder = Order();
        currOrder.subOrderIndexes.push_back(i);
        for(int j = 0 ; j < currOrderSize ; ++j)
        {
            int item_number;
            cin>>item_number;
            currOrder.items.push_back(item_number);
        }
        allOrders.push_back(currOrder);
    }    
}

#include "order_catering_functions.h"

# define POPSIZE 1200
# define MAXGENS 100
# define PMUTATION 0.4
struct genotype
{
  vector<vector<int>>gene;
  vector<vector<int>>itemSequence;
  double fitness;
};

vector<genotype>population(POPSIZE);

#include "initial_population.h"
#include "genetic_functions.h"

int main ( )
{
    freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    take_input();
    cout<<"Genetic New Model\n"<<endl;
    initialize (  );
    for (int generation = 1; generation <=MAXGENS; generation++ )
    {
      // evaluating fitness value of all population
      for(auto &member:population)
        member.fitness=get_fitness(member);
      if(generation==1){
          report(0);
      }
      vector<pair<int,int>>parent_pairs=select_parent_pairs(POPSIZE/2);
      vector<genotype>children;
      for(auto &it:parent_pairs)
      {
        pair<genotype,genotype>temp=crossover(it.first,it.second);
        children.push_back(temp.first);
        children.push_back(temp.second);
      }
      vector<genotype>new_population=population;
      for(auto &child:children)
      {
        mutate_member(child);
        new_population.push_back(child);
      }
      for(int i=0;i<0.2*POPSIZE;i++)
      {
        genotype random_child;
        random_child.gene=random_batching();
        fill_item_sequence(random_child);
        new_population.push_back(random_child);
      }
      for(auto &member:new_population)
        member.fitness=get_fitness(member);
      keep_the_best(new_population);
      report ( generation);
    }

    return 0;
}