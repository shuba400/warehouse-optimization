double get_fitness(genotype &member)
{
    double fitness=1.0/caterAllOrders(member.itemSequence).first;
    return fitness;
}

void initialize ( )
{
  population[0]=get_greedy_merged_member();
//   population[1]=get_greedy_merged_nearest_negihbour_tsp_member();
  for(int i = 1 ; i < POPSIZE ; ++i)
    population[i]= get_random_member();
}

int roulette_wheel_selection(vector<double>&probability)
{
    int n=probability.size();
    double rand=r8_uniform_ab(0.0,1.0);
    int lo=0,hi=n-1;
    // TTTTTFFFF
    while(lo<hi)
    {
        int mid=(lo+hi+1)/2;
        if(rand<=probability[mid])
            lo=mid;
        else
            hi=mid-1;
    }
    return lo;
}

vector<pair<int,int>> select_parent_pairs(int numParents)
{
    vector<pair<int,int>>parents;
    double sum=0;
    for(int i=0;i<POPSIZE;i++)
        sum+=population[i].fitness;
    vector<double>probability(POPSIZE);
    for(int i=0;i<POPSIZE;i++)
        probability[i]=population[i].fitness/sum;
    for(int i=1;i<POPSIZE;i++)
        probability[i]+=probability[i-1];
    for(int i=0;i<numParents;i++)
    {
        int parent1=roulette_wheel_selection(probability);
        int parent2=roulette_wheel_selection(probability);
        while(parent2==parent1)
            parent2=roulette_wheel_selection(probability);
        parents.push_back({parent1,parent2});
    }
    return parents;
}

struct genotype get_valid_child(struct genotype& child){
    struct genotype finalChild;
    (finalChild.gene).clear();
    (finalChild.itemSequence).clear();
    map<int,int>vis;

    for(int i = 0 ; i < (child.gene).size() ; ++i){
        bool ok = true;
        for(auto & x : (child.gene)[i]){
            // is the current order already included
            if(vis.find(x)!=vis.end()){
                // if yes then ignore current batch
                ok = false;
                break;
            }
        }
        if(ok){
          // all orders in current batch appear for first time
            for(auto & x : (child.gene)[i]){
                vis[x] = 1;
            }
            (finalChild.gene).push_back((child.gene)[i]);
            (finalChild.itemSequence).push_back((child.itemSequence)[i]);
        }
    }
    if(finalChild.gene.size()!=finalChild.itemSequence.size()){
      cout<<"error in get_valid_child\n";
      exit(0);
    }
    // adding the orders batch A/C : first fit rule 
    vector<int>left_orders;
    for(auto & currBatch : child.gene){
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
        // for(auto & currBatch : finalChild.gene){
        for(int i = 0 ; i < (finalChild.gene).size() ; ++i){
            int curr_batch_capacity = 0;
            // find capacity of current batch
            // for(auto & currOrder : currBatch){
            for(auto & currOrder : (finalChild.gene)[i]){
                curr_batch_capacity+=allOrders[currOrder].getOrderSize();
            }

            if(curr_batch_capacity + req_capacity <= max_capacity_robot){
                // current left order is included in already existing batch
                (finalChild.gene)[i].push_back(curr_left_order);

                // add all the items in current left order to the item sequence of this batch
                for(auto & item_in_curr_left_order : allOrders[curr_left_order].items){
                  (finalChild.itemSequence)[i].push_back(item_in_curr_left_order);
                }
                alloted = true;
                break;
            }
        }
        if(!alloted){
            (finalChild.gene).push_back({curr_left_order});
            vector<int>items_in_curr_left_order;

            for(auto & x : allOrders[curr_left_order].items){
              items_in_curr_left_order.push_back(x);
            }
            (finalChild.itemSequence).push_back(items_in_curr_left_order);
        }
    }
    if(finalChild.gene.size()!=finalChild.itemSequence.size()){
      cout<<"error in get_valid_child function\n";
      exit(0);
    }
    return finalChild;
}

pair<genotype,genotype> crossover ( int parent_index1, int parent_index2)
{
  struct genotype parent1 = population[parent_index1];
  struct genotype parent2 = population[parent_index2];
  
  int n1 = parent1.gene.size();
  int l1 = i4_uniform_ab ( 0, n1-1 );
  int r1 = i4_uniform_ab ( 0, n1-1 );
  if(r1<l1)swap(r1,l1);
  
  int n2 = parent2.gene.size();
  int l2 = i4_uniform_ab ( 0, n2-1 );
  int r2 = i4_uniform_ab ( 0, n2-1 );
  if(r2<l2)swap(r2,l2);
  
  genotype child1,child2;
  for(int i = 0 ; i < l1 ; ++i){
        (child1.gene).push_back(parent1.gene[i]);
        (child1.itemSequence).push_back(parent1.itemSequence[i]);
    }
    for(int i = l2 ; i<= r2 ; ++i){
        (child1.gene).push_back((parent2.gene)[i]);
        (child1.itemSequence).push_back(parent2.itemSequence[i]);
    }
    for(int i = l1 ; i<n1 ; ++i){
        (child1.gene).push_back(parent1.gene[i]);
        (child1.itemSequence).push_back(parent1.itemSequence[i]);
    }

    // make child2
    for(int i = 0 ; i < l2 ; ++i){
        (child2.gene).push_back(parent2.gene[i]);
        (child2.itemSequence).push_back(parent2.itemSequence[i]);
    }
    for(int i = l1 ; i<= r1 ; ++i){
        (child2.gene).push_back(parent1.gene[i]);
        (child2.itemSequence).push_back(parent1.itemSequence[i]);
    }
    for(int i = l2 ; i<n2 ; ++i){
        (child2.gene).push_back(parent2.gene[i]);
        (child2.itemSequence).push_back(parent2.itemSequence[i]);
    }

    child1=get_valid_child(child1);
    child2=get_valid_child(child2);
    return {child1,child2};
}

vector<int> two_opt(vector<int>& arr){
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

void mutate_item_sequence(genotype &member){
    for(int i = 0 ; i < member.itemSequence.size() ; ++i){
        member.itemSequence[i] = two_opt(member.itemSequence[i]);
    }
}

void mutate_batching(genotype &member){
  struct genotype parent = member;
  int n = parent.gene.size();
  int b1 = rand()%n;
  int b2 = b1;
  while(n!=1 && b2==b1){
      b2 = i4_uniform_ab(0,n-1);
      if(b2!=b1){
        break;
      }
      b2++;
      if(b2==n)b2 = 0;
  }
  struct genotype mutant_parent;
  for(int i = 0 ; i < n ; ++i){
      if(i==b1 || i==b2){
          continue;
      }
      (mutant_parent.gene).push_back(parent.gene[i]);
      (mutant_parent.itemSequence).push_back(parent.itemSequence[i]);
  }
  n = mutant_parent.gene.size();

  // delete batches b1 and b2
  vector<pair<int,int>>deleted_orders;
  for(auto & curr_order : parent.gene[b1]){
      deleted_orders.push_back({allOrders[curr_order].getOrderSize(), curr_order});
  }
  for(auto & curr_order : parent.gene[b2]){
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

      for(int i = 0 ; i < mutant_parent.gene.size() ; ++i){
          // calculate remaining capacity of current batch
          int capacity_used = 0;
          for(auto & currOrder : mutant_parent.gene[i]){
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
          mutant_parent.gene[best_fit_index].push_back(curr_del_order.second);
          // yahan se suru krna hai 
          for(auto & x : allOrders[curr_del_order.second].items)
            mutant_parent.itemSequence[best_fit_index].push_back(x);
      }
      else{
          mutant_parent.gene.push_back({curr_del_order.second});
          vector<int>allItems;
          for(auto & x : allOrders[curr_del_order.second].items)
            allItems.push_back(x);
          mutant_parent.itemSequence.push_back(allItems);
      }
  }
  member = mutant_parent;
}

void keep_the_best(vector<genotype>&new_population)
{
    sort(new_population.begin(),new_population.end(),[](const struct genotype& a, const struct genotype& b)->bool{
        return a.fitness>b.fitness;
    });
    for(int i=0;i<POPSIZE;i++)
        population[i]=new_population[i];
}

void report ( int generation )
{
    int best_member=-1;
    double best_fitness;
    for(int i=0;i<POPSIZE;i++)
    {
        if(best_member==-1||population[i].fitness>best_fitness)
        {
            best_member=i;
            best_fitness=population[i].fitness;
        }
    }
    // double catering_time=caterAllOrders(getOrderVector(population[best_fitness].gene)).first;
    double catering_time = 1/best_fitness;
    catering_time=(catering_time*1.0)/velocity;
    cout<<"Generation "<<generation<<"  ---->  "<<catering_time/60<<" hrs\n";
}