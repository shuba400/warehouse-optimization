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
    for(auto & x : path){
        cout<<x<<" ";
    }
    exit(0);
    time+=distance(currentCell,{0,0});
    time+=n*docking_time;
    return {time,path};
}