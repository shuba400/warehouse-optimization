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

// -----------------------------
int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;
int max_capacity_robot;
int number_of_total_items;

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

#include "population.h"

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

int ordinary_TSP(vector<int> arr){ //takes item index (index in that was being used in main item vector)
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
    int max_cell = 20;
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

int cater_curr_order(Order curr_order){
    // minimum time required to cater this order by a single robot;
    if(curr_order.getOrderSize()==0){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"Zero Size"<<endl;
      return -1234;
    }
    if(curr_order.getOrderSize()>max_capacity_robot){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"sizeExceeded"<<endl;
      return -1234;
    }
    curr_order.time=ordinary_TSP(curr_order.items);
    return curr_order.time;
}

// pass orders list and get the time AQ FCFS 
pair<int,vector<vector<int>>> caterAllOrders(vector<Order>mergedOrder){   

    // vector<Order> mergedOrder=merge_orders_asc_size();

    // Priority queue will store the earliest free time of all robots
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>robotFreeTimes;

    // initially all the robots are free at t = 0
    for(int i = 0 ; i < num_of_robots ; ++i){
        robotFreeTimes.push({0,i});
    }

    int totalTime = 0;     // total time taken to cater all order
    vector<vector<int>>robotTasks(num_of_robots);

    // cater all the order one by one
    // allot the earliest free robot to current order
    for(int i = 0 ; i < mergedOrder.size() ; ++i){
        // pick the earliest free robot 
        int earliestFreeRobot=robotFreeTimes.top().second;
        int freeTime=robotFreeTimes.top().first;
        robotFreeTimes.pop();
        robotTasks[earliestFreeRobot].push_back(0);

        // assign ith task to that robot
        // int cateringTime = mergedOrder[i].time;
        int cateringTime = cater_curr_order(mergedOrder[i]);
        int finishTime = freeTime + cateringTime;
        totalTime = max(totalTime,finishTime);

        // current robot will be free after finishing the ith task
        robotFreeTimes.push({finishTime,earliestFreeRobot});
    }
    // cout<<"Total Time"<<totalTime<<endl;
    return {totalTime,robotTasks};
}

// vector<Order> will be passed to caterAllOrders
vector<Order>getOrderVector(vector<vector<int>>batchVector){
  vector<Order>orderVector;
  for(auto & currBatch : batchVector){
    // currBatch is list of index of orders 
    Order currOrder;
    for(auto & ind : currBatch){
      // for all orders in curr Batch push all indices of items into currOrder
      for(auto & currItem : allOrders[ind].items){
        currOrder.items.push_back(currItem);
      }
    }
    orderVector.push_back(currOrder);
  }
  return orderVector;
}


vector<vector<int>>get_valid_child(vector<vector<int>>&child){
    vector<vector<int>>finalChild;
    map<int,int>vis;
    // child[i] : vector of orders which are merged
    // child.size() is the total number of batches

    for(int i = 0 ; i < child.size() ; ++i){
        bool ok = true;
        for(auto & x : child[i]){
            // is the current order already included
            if(vis.find(x)!=vis.end()){
                // if yes then ignore current batch
                ok = false;
                break;
            }
        }
        if(ok){
          // all orders in current batch appear for first time
            for(auto & x : child[i]){
                vis[x] = 1;
            }
            finalChild.push_back(child[i]);
        }
    }

    // adding the orders batch A/C : first fit rule 
    vector<int>left_orders;
    for(auto & currBatch : child){
        for(auto & currOrder : currBatch){
            if(vis.find(currOrder)==vis.end()){
                left_orders.push_back(currOrder);
            }
        }
    }
    for(auto & curr_left_order : left_orders){
        // get size of current left order 
        int req_capacity = allOrders[curr_left_order].getOrderSize();

        bool alloted = false;
        for(auto & currBatch : finalChild){
            int curr_batch_capacity = 0;

            // find capacity of current batch
            for(auto & currOrder : currBatch){
                curr_batch_capacity+=allOrders[currOrder].getOrderSize();
            }

            if(curr_batch_capacity + req_capacity <= max_capacity_robot){
                currBatch.push_back(curr_left_order);
                alloted = true;
                break;
            }
        }
        if(!alloted){
            finalChild.push_back({curr_left_order});
        }
    }
    return finalChild;
}

// ---------------------------------------------------------------------------------------

// 
//  Change any of these parameters to match your needs 
//
# define POPSIZE 150
# define MAXGENS 50
// # define NVARS 3
# define PXOVER 0.4
# define PMUTATION 0.2
//
//  Each GENOTYPE is a member of the population, with
//  gene: a string of variables,
//  fitness: the fitness
//
struct genotype
{
  vector<vector<int>>gene;
  double fitness;
};

struct genotype population[POPSIZE+1];
struct genotype newpopulation[POPSIZE+1]; 

int main ( );
void crossover ( int &seed );
void elitist ( );
void evaluate ( );
int i4_uniform_ab ( int a, int b, int &seed );
void initialize ( string filename, int &seed );
void keep_the_best ( );
void mutate ( int &seed );
double r8_uniform_ab ( double a, double b, int &seed );
void report ( int generation );
void selector ( int &seed );
void timestamp ( );
void Xover ( int one, int two, int &seed ,int ind1,int ind2);
vector<vector<int>>random_batching(int &seed);

int main ( )
{
    freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    take_input();
    int seed;
    auto tx = caterAllOrders(allOrders);
    cout<<"FCFS (no merging , no sorting)"<<endl;
    cout<<(double)(tx.first*1.0)/80.4672<<endl;

    string filename = "input1.txt";
    int generation;
    int i;

    timestamp ( );
    cout << "\n";
    cout << "SIMPLE_GA:\n";
    cout << "  C++ version\n";
    cout << "  A simple example of a genetic algorithm.\n";

    seed = 123456789;

    initialize ( filename, seed );

    evaluate ( );

    keep_the_best ( );

    for ( generation = 0; generation < MAXGENS; generation++ )
    {
      selector ( seed );
      // crossover ( seed );
      // mutate ( seed );
      evaluate ( );
      report ( generation );
      elitist ( );
    }

    cout << "\n";
    cout << "  Best member after " << MAXGENS << " generations:\n";
    cout << "\n";

    cout << "\n";
    cout << "  Best fitness = " << (double)(population[POPSIZE].fitness*1.0)/80.4672 << "\n";
    cout << "\n";
    cout << "SIMPLE_GA:\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    timestamp ( );

    return 0;
}

void elitist ( )
// 
//  Purpose:
//
//    ELITIST stores the best member of the previous generation.
//
//  Discussion:
//
//    The best member of the previous generation is stored as 
//    the last in the array. If the best member of the current 
//    generation is worse then the best member of the previous 
//    generation, the latter one would replace the worst member 
//    of the current population.
//
{
  int i;
  double best;
  int best_mem;
  double worst;
  int worst_mem;

  best = population[0].fitness;
  worst = population[0].fitness;

  for ( i = 0; i < POPSIZE - 1; ++i )
  {
    if ( population[i+1].fitness < population[i].fitness )
    {

      if ( best <= population[i].fitness )
      {
        best = population[i].fitness;
        best_mem = i;
      }

      if ( population[i+1].fitness <= worst )
      {
        worst = population[i+1].fitness;
        worst_mem = i + 1;
      }

    }
    else
    {

      if ( population[i].fitness <= worst )
      {
        worst = population[i].fitness;
        worst_mem = i;
      }

      if ( best <= population[i+1].fitness )
      {
        best = population[i+1].fitness;
        best_mem = i + 1;
      }

    }

  }
// 
//  If the best individual from the new population is better than 
//  the best individual from the previous population, then 
//  copy the best from the new population; else replace the 
//  worst individual from the current population with the 
//  best one from the previous generation                     
//
  if ( population[POPSIZE].fitness <= best )
  {
    population[POPSIZE].gene = population[best_mem].gene;
    population[POPSIZE].fitness = population[best_mem].fitness;
  }
  else
  {
    population[worst_mem].gene = population[POPSIZE].gene;
    population[worst_mem].fitness = population[POPSIZE].fitness;
  } 

  return;
}
//****************************************************************************80

void evaluate ( )

//****************************************************************************80
// 
//  Purpose:
//    EVALUATE implements the user-defined valuation function
{
  // add logic here 
  int member;
  int i;
  // double x[NVARS+1];

  for ( member = 0; member < POPSIZE; member++ )
  {
    population[member].fitness = -caterAllOrders(getOrderVector(population[member].gene)).first;
  }
  return;
}
//****************************************************************************80

int i4_uniform_ab ( int a, int b, int &seed )

//****************************************************************************80
//
//  Purpose:
//
//    I4_UNIFORM_AB returns a scaled pseudorandom I4 between A and B.
//
//  Discussion:
//
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//

//  Parameters:
//
//    Input, int A, B, the limits of the interval.
//
//    Input/output, int &SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, int I4_UNIFORM, a number between A and B.
//
{
  int c;
  const int i4_huge = 2147483647;
  int k;
  float r;
  int value;

  if ( seed == 0 )
  {
    cerr << "\n";
    cerr << "I4_UNIFORM_AB - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }
//
//  Guarantee A <= B.
//
  if ( b < a )
  {
    c = a;
    a = b;
    b = c;
  }

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 )
  {
    seed = seed + i4_huge;
  }

  r = ( float ) ( seed ) * 4.656612875E-10;
//
//  Scale R to lie between A-0.5 and B+0.5.
//
  r = ( 1.0 - r ) * ( ( float ) a - 0.5 ) 
    +         r   * ( ( float ) b + 0.5 );
//
//  Use rounding to convert R to an integer between A and B.
//
  value = round ( r );
//
//  Guarantee A <= VALUE <= B.
//
  if ( value < a )
  {
    value = a;
  }
  if ( b < value )
  {
    value = b;
  }

  return value;
}
//****************************************************************************80

vector<vector<int>>random_batching(int &seed){
  // create a random merging of order
  set<int>includedOrders;

  vector<vector<int>>batches;
  vector<int>currBatch;
  int currBatchItems = 0;

  while(includedOrders.size()<num_of_orders){
    // pick a random order which is not selected
    // int orderIndex = i4_uniform_ab(0,num_of_orders-1,seed);
    int orderIndex = rand(0,num_of_orders-1);
    while(includedOrders.find(orderIndex)!=includedOrders.end()){
      orderIndex++;
      if(orderIndex==num_of_orders)orderIndex = 0;
    }

    // we have choosen orderIndex as the next order to include, 
    // either in a new batch or in existing batch depending upon curr batch capacity
    if(allOrders[orderIndex].items.size() + currBatchItems <= max_capacity_robot){
      currBatchItems+=allOrders[orderIndex].items.size();
      currBatch.push_back(orderIndex);
    }
    else{
      batches.push_back(currBatch);
      currBatch.clear();
      currBatchItems = allOrders[orderIndex].items.size();
      currBatch.push_back(orderIndex);
    }
    includedOrders.insert(orderIndex);
  }
  if(currBatchItems>0)batches.push_back(currBatch);
  return batches;
}


void initialize ( string filename, int &seed )

//****************************************************************************80
// 
//  Purpose:
// for initial population
//
{
  double a = 0.2;
  double b = 0.2;
  double c = 0.6;

  // map to ensure initial population is distinct
  map<vector<vector<int>>,int>alreadyIncluded;

  // first member is merging by greedy only 
  population[0].gene = populate_by_GreedyOnly();
  alreadyIncluded[population[0].gene]++;

  // 20 % of the population is random batching
  for(int i = 1 ; i <= POPSIZE ; ++i){
    population[i].gene = random_batching(seed);
    // if this member is being repeated mark it , by making its fitness value as -999
    if(alreadyIncluded.find(population[i].gene)!=alreadyIncluded.end()){
      population[i].fitness = -999;
    }
    alreadyIncluded[population[i].gene] = 1;
  }

  // orders which were marked as repeated, randomly generate those batches
  for(int i = 0 ; i< POPSIZE ; ++i){
    if(population[i].fitness==-999){
      population[i].gene = random_batching(seed);
    }
  }

  //fitness value of all members are initialized as -inf
  for(int i = 0 ; i <= POPSIZE ; ++i){
    population[i].fitness = -inf;
  }
  
  // // random batching - if we want only random batching without any heuristic for initial population
  // for(int i = 0 ; i <= POPSIZE ; ++i){
  //   population[i].gene = random_batching(seed);
  //   population[i].fitness = -9999999;
  // }

  // input.close ( );

  return;
}
//****************************************************************************80

void keep_the_best ( )

//****************************************************************************80
//  Purpose:
//    KEEP_THE_BEST keeps track of the best member of the population. 
//  Discussion:
//    Note that the last entry in the array Population holds a 
//    copy of the best individual.
//
{
  int cur_best;
  int mem;
  int i;

  cur_best = 0;

  for ( mem = 0; mem < POPSIZE; mem++ )
  {
    if ( population[POPSIZE].fitness < population[mem].fitness )
    {
      cur_best = mem;
      population[POPSIZE].fitness = population[mem].fitness;
    }
  }
// 
//  Once the best member in the population is found, copy the genes.
//
  population[POPSIZE].gene = population[cur_best].gene;

  return;
}
//****************************************************************************80
void myMutation(int ind , int& seed){
  vector<vector<int>>parent = newpopulation[ind].gene;
  int n = parent.size();
  int b1 = rand()%n;
  int b2 = b1;
  while(n!=1 && b2==b1){
      b2 = i4_uniform_ab(0,n-1,seed);
      if(b2!=b1){
        break;
      }
      b2++;
      if(b2==n)b2 = 0;
  }
  // mutant parent will store the new batches after mutation
  vector<vector<int>>mutant_parent;
  for(int i = 0 ; i < n ; ++i){
    // adding all batches, except b1 and b2
      if(i==b1 || i==b2){
          continue;
      }
      mutant_parent.push_back(parent[i]);
  }
  n = mutant_parent.size();

  // delete batches b1 and b2
  vector<pair<int,int>>deleted_orders;
  for(auto & curr_order : parent[b1]){
      deleted_orders.push_back({allOrders[curr_order].getOrderSize(), curr_order});
  }
  for(auto & curr_order : parent[b2]){
      deleted_orders.push_back({allOrders[curr_order].getOrderSize(), curr_order});
  }
  // sort the deleted orders according to their size
  sort(deleted_orders.begin(),deleted_orders.end(),greater<pair<int,int>>());

  for(auto & curr_del_order : deleted_orders){
      // find the best fit for current order in any batch 
      bool alloted = false;
      int curr_del_order_size = curr_del_order.first;
      int best_fit_size = 1e9;
      int best_fit_index = -1;

      for(int i = 0 ; i < mutant_parent.size() ; ++i){
          // calculate remaining capacity of current batch
          int capacity_used = 0;
          for(auto & currOrder : mutant_parent[i]){
              capacity_used+=allOrders[currOrder].getOrderSize();
          }
          int remaining_capacity = max_capacity_robot - capacity_used;
          if(curr_del_order_size<=remaining_capacity){
              if(remaining_capacity<best_fit_size){
                  best_fit_size = remaining_capacity;
                  best_fit_index = i;
                  alloted = true;
              }
          }
      }   

      if(alloted){
          mutant_parent[best_fit_index].push_back(curr_del_order.second);
      }
      else{
          mutant_parent.push_back({curr_del_order.second});
      }
  }
  newpopulation[ind].gene = mutant_parent;
  // population[ind].fitness = -caterAllOrders(getOrderVector(population[ind].gene)).first;
}

double r8_uniform_ab ( double a, double b, int &seed )
//****************************************************************************80
//  Purpose:
//
//    R8_UNIFORM_AB returns a scaled pseudorandom R8.
//
//  Discussion:
//
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//
{
  int i4_huge = 2147483647;
  int k;
  double value;

  if ( seed == 0 )
  {
    cerr << "\n";
    cerr << "R8_UNIFORM_AB - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit ( 1 );
  }

  k = seed / 127773;

  seed = 16807 * ( seed - k * 127773 ) - k * 2836;

  if ( seed < 0 )
  {
    seed = seed + i4_huge;
  }

  value = ( double ) ( seed ) * 4.656612875E-10;

  value = a + ( b - a ) * value;

  return value;
}
//****************************************************************************80

void report ( int generation )

//****************************************************************************80
//  Purpose:
//    REPORT reports progress of the simulation. 
{
  double best_val;

  if ( generation == 0 )
  {
    cout << "\n";
    // cout << "  Generation       Best            Average       Standard \n";
    // cout << "  number           value           fitness       deviation \n";
     cout << "  Generation       Best \n";
    cout << "   number           value \n";
    cout << "\n";
  }

  best_val = population[POPSIZE].fitness;

  cout << "  " << setw(8) << generation 
       << "  " << setw(14) << -(double)(best_val*1.0)/80.4672 <<" minutes \n";
  return;
}
//****************************************************************************80

void selector ( int &seed )

//****************************************************************************80
// 
//  Purpose: transition from xth generation to (x+1)th generation
// 
{
  // sort the populaton according to fitness value
  sort(population,population+POPSIZE+1,[](const struct genotype& a, const struct genotype& b)->bool{
    return a.fitness<b.fitness;
  });

  for(int i = 0 ; i <= POPSIZE ; ++i)newpopulation[i] = population[i];

  int top = 0.2*POPSIZE;
  for(int i = POPSIZE-1; i>=POPSIZE-top ; --i){
    newpopulation[i] = population[i];
  }
  
  // choose remaining population 
  for(int i = 0 ; i < POPSIZE - top ; ++i,i++){
    int topParent = i4_uniform_ab(POPSIZE-top,POPSIZE-1,seed);
    int nonTopParent = i4_uniform_ab(0,POPSIZE-top-1,seed);

    // this part needs to be handled 
    Xover(topParent,nonTopParent,seed,i,i+1);
  }

  for(int i = 0 ; i < POPSIZE ; ++i){
    double prob = r8_uniform_ab(0.0,1.0,seed);
    if(prob<PMUTATION){
      myMutation(i,seed);
    }
  }

//  Overwrite the old population with the new one.
//
  for ( int i = 0; i < POPSIZE; i++ )
  {
    population[i] = newpopulation[i]; 
  }

  return;     
}
//****************************************************************************80

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    May 31 2001 09:45:54 AM
//

{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

void Xover ( int one, int two, int &seed ,int ind1,int ind2)

//****************************************************************************80
//  Purpose:
//    XOVER performs crossover of the two selected parents. 
{

  // ind1, ind2 represents the index of the new population in which crossed child will be stored
  vector<vector<int>>parent1 = population[one].gene;
  vector<vector<int>>parent2 = population[two].gene;

  // select the crossing section
  int n1 = parent1.size();
  int l1 = i4_uniform_ab ( 0, n1-1, seed );
  int r1 = i4_uniform_ab ( 0, n1-1, seed );
  if(r1<l1)swap(r1,l1);

  int n2 = parent2.size();
  int l2 = i4_uniform_ab ( 0, n2-1, seed );
  int r2 = i4_uniform_ab ( 0, n2-1, seed );
  if(r2<l2)swap(r2,l2);

  vector<vector<int>>child1,child2;

  // step 3 
  // make child1
    for(int i = 0 ; i < l1 ; ++i){
        child1.push_back(parent1[i]);
    }
    for(int i = l2 ; i<= r2 ; ++i){
        child1.push_back(parent2[i]);
    }
    for(int i = l1 ; i<n1 ; ++i){
        child1.push_back(parent1[i]);
    }

    // make child2
    for(int i = 0 ; i < l2 ; ++i){
        child2.push_back(parent2[i]);
    }
    for(int i = l1 ; i<= r1 ; ++i){
        child2.push_back(parent1[i]);
    }
    for(int i = l2 ; i<n2 ; ++i){
        child2.push_back(parent2[i]);
    }

    // these two childs generated may be  invalid
  // population[one].gene = get_valid_child(child1);
  // population[two].gene = get_valid_child(child2);
  newpopulation[ind1].gene = get_valid_child(child1);
  newpopulation[ind2].gene = get_valid_child(child2);
  return;
}