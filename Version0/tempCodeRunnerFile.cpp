        while(last_element > -1){
            optimalpath.push_back(locations[last_element]);
            int new_last_element = dp[main_mask][last_element].second.second;
            int new_main_mask = dp[main_mask][last_element].second.first;
            last_element = new_last_element;
            new_main_mask = main_mask;
        }