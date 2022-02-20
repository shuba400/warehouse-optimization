#include<bits/stdc++.h>
# include <cmath>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <ctime>
# include <cstring>

using namespace std;

// -----------------------------
const int inf = 1e9;

int ROWS,COLS;
int docking_time;    //==> T ==> dist + D 
int num_of_robots;
int num_of_orders;
int velocity;
int max_capacity_robot;

// ---------------------------------------------------------------------------------------
// class to store details of a single cell
struct Cell{
    int x,y; 
    bool operator<(const Cell &o)  const {
        return (x < o.x) || (x == o.x && y < o.y);
    }
    friend ostream& operator << (ostream &os, const Cell &m) {return os << "{" << m.x << "," << m.y << "}";}
};

int distance(Cell a,Cell b){
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// class to store all the details related to a single order
struct Order{
    vector<Cell>cells;  // to store coordinates of each item in current order
    vector<Cell> optimalpath;
    vector<int> index;
    int time;
    bool dp_done;

    Order(){   
        dp_done = 0;
        time = 0;
    }

    int getOrderSize(){
        return cells.size();
    }
};
vector<Order>allOrders;
vector<vector<vector<int>>>L;
#include "population.h"

//This part calculate TSP, DO NOT TOUCH
vector<vector<pair<int,pair<int,int>>>> dp;
pair<int,pair<int,int>> main_info;
vector<Cell> dp_cells;
int recur_relation(int mask,int last_element){
    if(mask == 0) return inf;
    if(dp[mask][last_element].first != -1) return dp[mask][last_element].first;
    dp[mask][last_element].first = inf;
    for(int i = 0; i < dp_cells.size(); i++){
        if(((mask>>i)&1) == 0 || last_element == i) continue;
        pair<int,pair<int,int>> temp = {recur_relation(mask^(1<<last_element),i) + distance(dp_cells[i],dp_cells[last_element]) + docking_time,{(mask^(1<<last_element)),i}};

        dp[mask][last_element] = min(dp[mask][last_element],temp);
    }
    return dp[mask][last_element].first;
}

void start_recurrence(Order &o){
    dp_cells.clear();
    dp.clear();
    o.optimalpath.clear();
    o.time=0;
    main_info = {inf,{-1,-1}};
    dp_cells.push_back({0,0});
    for(auto &x:o.cells) dp_cells.push_back(x);
    int tot_number_of_element = dp_cells.size();                          
    pair<int,pair<int,int>> filler_value = {-1,{-1,-1}};  //used to intialize the dp states
    dp.resize(1<<tot_number_of_element,vector<pair<int,pair<int,int>>>(tot_number_of_element,filler_value));
    dp[1][0] = {0,{-1,-1}};   //current mask = 0000001, current last cell is  0, this is basically saying that when element is at position 0 , t = 0 intially
    for(int i = 1; i < tot_number_of_element; i++){
        pair<int,pair<int,int>> p = {recur_relation((1<<tot_number_of_element) - 1,i) + distance(dp_cells[0],dp_cells[i]) + docking_time,{(1<<tot_number_of_element) - 1,i}};
        main_info = min(main_info,p);
    }
    o.time = main_info.first;
    int main_mask = main_info.second.first;
    int last_element = main_info.second.second;
    while(last_element > -1){
        o.optimalpath.push_back(dp_cells[last_element]);
        int new_last_element = dp[main_mask][last_element].second.second;
        int new_main_mask = dp[main_mask][last_element].second.first;
        last_element = new_last_element;
        main_mask = new_main_mask;
    }
    o.optimalpath.pop_back();
    reverse(o.optimalpath.begin(),o.optimalpath.end());
    return;
}
//End of do not touch
int cater_curr_order(Order curr_order){
    // minimum time required to cater this order by a single robot;
    if(curr_order.getOrderSize()==0){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"Zero Size\n";
      return 0;
    }
    if(curr_order.getOrderSize()>max_capacity_robot){
      cout<<curr_order.getOrderSize()<<endl;
      cout<<"sizeExceeded\n";
      return 0;
    }
    start_recurrence(curr_order);
    int min_time = curr_order.time;
    return min_time;
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
void take_input(){

    cin>>ROWS>>COLS;
    cin>>docking_time>>velocity >> max_capacity_robot;
    cin>>num_of_robots>>num_of_orders;
    allOrders.clear();
    vector<int>tempv;
    L.assign(ROWS+1,vector<vector<int>>(COLS+1,tempv));
    for(int i = 0 ; i < num_of_orders;++i){

        int currOrderSize;
        cin>>currOrderSize;
        Order currOrder = Order();   //this will be the main human cell, for now I am assuming this is 0,0
        Cell tmp;
        tmp.x = 0;
        tmp.y = 0;
        // currOrder.cells.push_back(tmp);
        currOrder.index.push_back(i);
        for(int j = 0 ; j < currOrderSize ; ++j){
            // x and y coordinates of items in current order
            cin >> tmp.x >> tmp.y;
            currOrder.cells.push_back(tmp);
            L[tmp.x][tmp.y].push_back(i);
        }
        allOrders.push_back(currOrder);
    }    
}

// vector<Order> will be passed to caterAllOrders
vector<Order>getOrderVector(vector<vector<int>>batchVector){
  vector<Order>orderVector;
  for(auto & currBatch : batchVector){
    // currBatch is list of index of orders 
    Order currOrder;
    for(auto & ind : currBatch){
      // for all orders in curr Batch push all indices of items into currOrder
      for(auto & currCell : allOrders[ind].cells){
        currOrder.cells.push_back(currCell);
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
# define POPSIZE 200
# define MAXGENS 50
// # define NVARS 3
# define PXOVER 0.4
# define PMUTATION 0.2
//
//  Each GENOTYPE is a member of the population, with
//  gene: a string of variables,
//  fitness: the fitness
//  upper: the variable upper bounds,
//  lower: the variable lower bounds,
//  rfitness: the relative fitness,
//  cfitness: the cumulative fitness.
//
struct genotype
{
  vector<vector<int>>gene;
  // double gene[NVARS];
  // vector<vector<int>>gene;
  double fitness;
  // double fitness;
  vector<double>upper;
  vector<double>lower;
  // double upper[NVARS];
  // double lower[NVARS];
  double rfitness;
  double cfitness;
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
//****************************************************************************80

int main ( )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN supervises the genetic algorithm.
//
//  Discussion:
//
//    Each generation involves selecting the best 
//    members, performing crossover & mutation and then 
//    evaluating the resulting population, until the terminating 
//    condition is satisfied   
//
//    This is a simple genetic algorithm implementation where the 
//    evaluation function takes positive values only and the 
//    fitness of an individual is the same as the value of the 
//    objective function.  

//  Parameters:
//
//    MAXGENS is the maximum number of generations.
//
//    NVARS is the number of problem variables.
//
//    PMUTATION is the probability of mutation.
//
//    POPSIZE is the population size. 
//
//    PXOVER is the probability of crossover.                          
//
{

    freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    take_input();
    int seed;
    auto tx = caterAllOrders(allOrders);
    cout<<"FCFS (no merging , no sorting)"<<endl;
    cout<<(double)(tx.first*1.0)/80.4672<<endl;
    // vector<vector<int>>batch = random_batching(seed);
    // cout<<endl;
    // cout<<"Total number of batches : "<<batch.size()<<endl;
    // for(auto & x : batch){
    //   cout<<x.size()<<endl;
    //   for(auto & y : x){
    //     cout<<y<<" ";
    //   }
    //   cout<<endl;
    // }
    // return 0;

  // int a = 9, b = 100000000;
  // int p = rand();
  // int r = i4_uniform_ab(a,b,p);
  // int r1 = i4_uniform_ab(a,b,p);
  // for(int i = 0 ; i < 100 ; ++i){
  //   r = i4_uniform_ab(a,b,p);
  //   cout<<r<<endl;
  // }



  string filename = "input1.txt";
  int generation;
  int i;

  timestamp ( );
  cout << "\n";
  cout << "SIMPLE_GA:\n";
  cout << "  C++ version\n";
  cout << "  A simple example of a genetic algorithm.\n";

//   if ( NVARS < 2 )
//   {
//     cout << "\n";
//     cout << "  The crossover modification will not be available,\n";
//     cout << "  since it requires 2 <= NVARS.\n";
//   }

  seed = 123456789;

  initialize ( filename, seed );

  evaluate ( );

  keep_the_best ( );

  for ( generation = 0; generation < MAXGENS; generation++ )
  {
    selector ( seed );
    // crossover ( seed );
    // mutate ( seed );
    report ( generation );
    evaluate ( );
    elitist ( );
  }

  cout << "\n";
  cout << "  Best member after " << MAXGENS << " generations:\n";
  cout << "\n";

  // for ( i = 0; i < NVARS; i++ )
  // {
  //   cout << "  var(" << i << ") = " << population[POPSIZE].gene[i] << "\n";
  // }

  cout << "\n";
  cout << "  Best fitness = " << (double)(population[POPSIZE].fitness*1.0)/80.4672 << "\n";
//
//  Terminate.
//
  cout << "\n";
  cout << "SIMPLE_GA:\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
  timestamp ( );

  return 0;
}
//****************************************************************************80

void crossover ( int &seed )

//****************************************************************************80
// 
//  Purpose:
//
//    CROSSOVER selects two parents for the single point crossover.
//

//
//  Local parameters:
//
//    Local, int FIRST, is a count of the number of members chosen.
//
//  Parameters:
//
//    Input/output, int &SEED, a seed for the random number generator.
//
{
  // const double a = 0.0;
  // const double b = 1.0;
  // int mem;
  // int one;
  // int first = 0;
  // double x;

  // for ( mem = 0; mem < POPSIZE; ++mem )
  // {
  //   x = r8_uniform_ab ( a, b, seed );

  //   if ( x < PXOVER )
  //   {
  //     ++first;

  //     if ( first % 2 == 0 )
  //     {
  //       Xover ( one, mem, seed );
  //     }
  //     else
  //     {
  //       one = mem;
  //     }

  //   }
  // }
  return;
}
//****************************************************************************80

void elitist ( )

//****************************************************************************80
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

//  Local parameters:
//
//    Local, double BEST, the best fitness value.
//
//    Local, double WORST, the worst fitness value.
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
    // for ( i = 0; i < NVARS; i++ )
    // {
      // population[POPSIZE].gene[i] = population[best_mem].gene[i];
    // }
    population[POPSIZE].gene = population[best_mem].gene;
    population[POPSIZE].fitness = population[best_mem].fitness;
  }
  else
  {
    // for ( i = 0; i < NVARS; i++ )
    // {
    //   population[worst_mem].gene[i] = population[POPSIZE].gene[i];
    // }
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
//
//    EVALUATE implements the user-defined valuation function
//
//  Discussion:
//
//    Each time this is changed, the code has to be recompiled.
//    The current function is:  x[1]^2-x[1]*x[2]+x[3]
//

{
  // add logic here 
  int member;
  int i;
  // double x[NVARS+1];

  for ( member = 0; member < POPSIZE; member++ )
  {
    // for ( i = 0; i < NVARS; i++ )
    // {
      // x[i+1] = population[member].gene[i];
    // } 
    // population[member].fitness = ( x[1] * x[1] ) - ( x[1] * x[2] ) + x[3];

    // pass the vecto<Order> to caterAllOrder , it will return time and path
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
    int orderIndex = rand();
    orderIndex%=num_of_orders;
    while(includedOrders.find(orderIndex)!=includedOrders.end()){
      orderIndex++;
      if(orderIndex==num_of_orders)orderIndex = 0;
    }

    // we have choosen orderIndex as the next order to include, 
    // either in a new batch or in existing batch depending upon curr batch capacity
    if(allOrders[orderIndex].cells.size() + currBatchItems <= max_capacity_robot){
      currBatchItems+=allOrders[orderIndex].cells.size();
      currBatch.push_back(orderIndex);
    }
    else{
      batches.push_back(currBatch);
      currBatch.clear();
      currBatchItems = allOrders[orderIndex].cells.size();
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
//
//    INITIALIZE initializes the genes within the variables bounds. 
//
//  Discussion:
//
//    It also initializes (to zero) all fitness values for each
//    member of the population. It reads upper and lower bounds 
//    of each variable from the input file `gadata.txt'. It 
//    randomly generates values between these bounds for each 
//    gene of each genotype in the population. The format of 
//    the input file `gadata.txt' is 
//
//      var1_lower_bound var1_upper bound
//      var2_lower_bound var2_upper bound ...
//

//  Parameters:
//
//    Input, string FILENAME, the name of the input file.
//
//    Input/output, int &SEED, a seed for the random number generator.
//
{
  // add logic here
  int i;
  // ifstream input;
  int j;
  double lbound;
  double ubound;

  // input.open ( filename.c_str ( ) );

  // if ( !input )
  // {
  //   cerr << "\n";
  //   cerr << "INITIALIZE - Fatal error!\n";
  //   cerr << "  Cannot open the input file!\n";
  //   exit ( 1 );
  // }
// 
//  Initialize variables within the bounds 
//
  // for ( i = 0; i < NVARS; i++ )
  // {
  //   input >> lbound >> ubound;

  //   for ( j = 0; j < POPSIZE; j++ )
  //   {
  //     population[j].fitness = 0;
  //     population[j].rfitness = 0;
  //     population[j].cfitness = 0;
  //     population[j].lower[i] = lbound;
  //     population[j].upper[i]= ubound;
  //     population[j].gene[i] = r8_uniform_ab ( lbound, ubound, seed );
  //   }
  // }
  double a = 0.2;
  double b = 0.2;
  double c = 0.6;
  for(int i = 0 ; i <= POPSIZE ; ++i){
    population[i].gene = random_batching(seed);
    population[i].fitness = -9999999;
  }
  // for(int i = 0 ; i <= 0.2*POPSIZE ; ++i){
  //   population[i].gene = populate_by_GreedyOnly();
  // }
  // for(int i = 0.2*POPSIZE+1;i<=0.4*POPSIZE;++i){
  //   int d = i4_uniform_ab(5,20,seed);
  //   population[i].gene = populate_by_GeometryOnly(d);
  // }
  // for(int i = 0.4*POPSIZE+1 ; i<POPSIZE ; ++i){
  //   int d = i4_uniform_ab(5,20,seed);
  //   population[i].gene = populate_by_GeometryAndGreedy(d);
  // }
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
// 
//  Purpose:
//
//    KEEP_THE_BEST keeps track of the best member of the population. 
//
//  Discussion:
//
//    Note that the last entry in the array Population holds a 
//    copy of the best individual.
//

//  Local parameters:
//
//    Local, int CUR_BEST, the index of the best individual.
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
  // for ( i = 0; i < NVARS; i++ )
  // {
    // population[POPSIZE].gene[i] = population[cur_best].gene[i];
  // }
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
void mutate ( int &seed )

//****************************************************************************80
// 
//  Purpose:
//
//    MUTATE performs a random uniform mutation. 
//
//  Discussion:
//
//    A variable selected for mutation is replaced by a random value 
//    between the lower and upper bounds of this variable.
//
//
//  Parameters:
//
//    Input/output, int &SEED, a seed for the random number generator.
//
{
  // const double a = 0.0;
  // const double b = 1.0;
  // int i;
  // int j;
  // double lbound;
  // double ubound;
  // double x;

  // for ( i = 0; i < POPSIZE; i++ )
  // {
    // add logic here 

    // for ( j = 0; j < NVARS; j++ )
    // {
    //   x = r8_uniform_ab ( a, b, seed );
    //   if ( x < PMUTATION )
    //   {
    //     lbound = population[i].lower[j];
    //     ubound = population[i].upper[j];  
    //     population[i].gene[j] = r8_uniform_ab ( lbound, ubound, seed );
    //   }
    // }
  // }
  // for ( i = 0; i < POPSIZE; i++ )
  // {
  //   x = r8_uniform_ab ( a, b, seed );
  //   if ( x < PMUTATION )
  //   {
  //     myMutation(i,seed);
  //   }
  // }
  // return;
}
//****************************************************************************80

double r8_uniform_ab ( double a, double b, int &seed )

//****************************************************************************80
//
//  Purpose:
//
//    R8_UNIFORM_AB returns a scaled pseudorandom R8.
//
//  Discussion:
//
//    The pseudorandom number should be uniformly distributed
//    between A and B.
//

//  Parameters:
//
//    Input, double A, B, the limits of the interval.
//
//    Input/output, int &SEED, the "seed" value, which should NOT be 0.
//    On output, SEED has been updated.
//
//    Output, double R8_UNIFORM_AB, a number strictly between A and B.
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
// 
//  Purpose:
//
//    REPORT reports progress of the simulation. 
//
//  Local parameters:
//
//    Local, double avg, the average population fitness.
//
//    Local, best_val, the best population fitness.
//
//    Local, double square_sum, square of sum for std calc.
//
//    Local, double stddev, standard deviation of population fitness.
//
//    Local, double sum, the total population fitness.
//
//    Local, double sum_square, sum of squares for std calc.
//
{
  double avg;
  double best_val;
  int i;
  double square_sum;
  double stddev;
  double sum;
  double sum_square;

  if ( generation == 0 )
  {
    cout << "\n";
    // cout << "  Generation       Best            Average       Standard \n";
    // cout << "  number           value           fitness       deviation \n";
     cout << "  Generation       Best \n";
    cout << "   number           value \n";
    cout << "\n";
  }

  sum = 0.0;
  sum_square = 0.0;

  for ( i = 0; i < POPSIZE; i++ )
  {
    sum = sum + population[i].fitness;
    sum_square = sum_square + population[i].fitness * population[i].fitness;
  }

  avg = sum / ( double ) POPSIZE;
  square_sum = avg * avg * POPSIZE;
  stddev = sqrt ( ( sum_square - square_sum ) / ( POPSIZE - 1 ) );
  best_val = population[POPSIZE].fitness;

  cout << "  " << setw(8) << generation 
       << "  " << setw(14) << -(double)(best_val*1.0)/80.4672 <<" minutes \n";
      //  << "  " << setw(14) << avg 
      //  << "  " << setw(14) << stddev << "\n";

  return;
}
//****************************************************************************80

void selector ( int &seed )

//****************************************************************************80
// 
//  Purpose:
//
//    SELECTOR is the selection function.
//
//  Discussion:
//
//    Standard proportional selection for maximization problems incorporating 
//    the elitist model.  This makes sure that the best member always survives.
//
//  Parameters:
//
//    Input/output, int &SEED, a seed for the random number generator.
//
{
  const double a = 0.0;
  const double b = 1.0;
  int i;
  int j;
  int mem;
  double p;
  double sum;
//
//  Find the total fitness of the population.
//
//   sum = 0.0;
//   for ( mem = 0; mem < POPSIZE; mem++ )
//   {
//     sum = sum + population[mem].fitness;
//   }
// //
// //  Calculate the relative fitness of each member.
// //
//   for ( mem = 0; mem < POPSIZE; mem++ )
//   {
//     population[mem].rfitness = population[mem].fitness / sum;
//   }
// // 
// //  Calculate the cumulative fitness.
// //
//   population[0].cfitness = population[0].rfitness;
//   for ( mem = 1; mem < POPSIZE; mem++ )
//   {
//     population[mem].cfitness = population[mem-1].cfitness +       
//       population[mem].rfitness;
//   }
// // 
// //  Select survivors using cumulative fitness. 
// //
//   for ( i = 0; i < POPSIZE; i++ )
//   { 
//     p = r8_uniform_ab ( a, b, seed );
//     if ( p < population[0].cfitness )
//     {
//       newpopulation[i] = population[0];      
//     }
//     else
//     {
//       for ( j = 0; j < POPSIZE; j++ )
//       { 
//         if ( population[j].cfitness <= p && p < population[j+1].cfitness )
//         {
//           newpopulation[i] = population[j+1];
//         }
//       }
//     }
//   }
//


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
  for ( i = 0; i < POPSIZE; i++ )
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
// 
//  Purpose:
//
//    XOVER performs crossover of the two selected parents. 
//
//  Local parameters:
//
//    Local, int point, the crossover point.
//
//  Parameters:
//
//    Input, int ONE, TWO, the indices of the two parents.
//
//    Input/output, int &SEED, a seed for the random number generator.
//
{
  // add logic here 
  int i;
  int point;
  double t;
  



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
  newpopulation[two].gene = get_valid_child(child1);
  newpopulation[two].gene = get_valid_child(child2);

// 
//  Select the crossover point.
//
//   point = i4_uniform_ab ( 0, NVARS - 1, seed );
// //
// //  Swap genes in positions 0 through POINT-1.
// //
//   for ( i = 0; i < point; i++ )
//   {
//     t                       = population[one].gene[i];
//     population[one].gene[i] = population[two].gene[i];
//     population[two].gene[i] = t;
//   }

  return;
}