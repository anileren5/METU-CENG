#include "the7.h"

// Implement your solution after this line

void CanCatch(int n, std::vector<Road> roads, int s, int d, int x, int y, int l, int printPath) {
    std::vector<int> path;
    // int cost = INT_MAX;
    int m = roads.size();

    // Creation of Adjacency Matrix
    
    std::vector<std::vector<int>> adj;
    for (int i=0;i<n;i++){
        std::vector<int> tmp;
        adj.push_back(tmp);
    }
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            adj[i].push_back(INT_MAX);
        }
    }
    for (int i=0;i<m;i++){
        int e1 = roads[i].buildings.first;
        int e2 = roads[i].buildings.second;
        adj[e1][e2] = roads[i].time;
        adj[e2][e1] = roads[i].time;
    }
    
    

    // From s to Others
    std::vector<int> d_s;
    std::vector<bool> found_s;
    std::vector<int> track_s(n);
    track_s[s] = -1;
    for (int i=0;i<n;i++){
        d_s.push_back(INT_MAX);
        found_s.push_back(false);
    }
    d_s[s] = 0;
    
    for (int i=0; i<n-1;i++){
        int min = INT_MAX;
        int min_index;
        for (int j=0; j<n;j++){
            if (found_s[j] == false && d_s[j] <= min){
                min = d_s[j];
                min_index = j;
            }
        }
        
        found_s[min_index] = true;
        
        for (int j=0; j<n;j++){
            if (found_s[j] == false && adj[min_index][j] != INT_MAX && d_s[min_index] != INT_MAX && (d_s[min_index] + adj[min_index][j]) < d_s[j]){
                d_s[j] = d_s[min_index] + adj[min_index][j]; 
                track_s[j] = min_index;
            }
        }
    }
    
    
    
    // From x to Others
    std::vector<int> d_x;
    std::vector<bool> found_x;
    std::vector<int> track_x(n);
    track_x[x] = -1;
    for (int i=0;i<n;i++){
        d_x.push_back(INT_MAX);
        found_x.push_back(false);
    }
    d_x[x] = 0;
    
    for (int i=0; i<n-1;i++){
        int min = INT_MAX;
        int min_index;
        for (int j=0; j<n;j++){
            if (found_x[j] == false && d_x[j] <= min){
                min = d_x[j];
                min_index = j;
            }
        }
        
        found_x[min_index] = true;
        
        for (int j=0; j<n;j++){
            if (found_x[j] == false && adj[min_index][j] != INT_MAX && d_x[min_index] != INT_MAX && (d_x[min_index] + adj[min_index][j]) < d_x[j]){
                d_x[j] = d_x[min_index] + adj[min_index][j]; 
                track_x[j] = min_index;
            }
        }
    }
    
    
    // From y to Others
    std::vector<int> d_y;
    std::vector<bool> found_y;
    std::vector<int> track_y(n);
    track_y[y] = -1;
    for (int i=0;i<n;i++){
        d_y.push_back(INT_MAX);
        found_y.push_back(false);
    }
    d_y[y] = 0;
    
    for (int i=0; i<n-1;i++){
        int min = INT_MAX;
        int min_index;
        for (int j=0; j<n;j++){
            if (found_y[j] == false && d_y[j] <= min){
                min = d_y[j];
                min_index = j;
            }
        }
        
        found_y[min_index] = true;
        
        for (int j=0; j<n;j++){
            if (found_y[j] == false && adj[min_index][j] != INT_MAX && d_y[min_index] != INT_MAX && (d_y[min_index] + adj[min_index][j]) < d_y[j]){
                d_y[j] = d_y[min_index] + adj[min_index][j];
                track_y[j] = min_index;
            }
        }
    }
    
    
    // Note: Cat can always reach X, Y, and D from S.
    
    
    int c1 = d_s[x] + d_x[y] + d_y[d];
    if (d_s[x] == INT_MAX || d_x[y] == INT_MAX || d_y[d] == INT_MAX) c1 = INT_MAX;
    
    int c2 = d_s[y] + d_y[x] + d_x[d];
    if (d_s[x] == INT_MAX || d_x[y] == INT_MAX || d_y[d] == INT_MAX) c2 = INT_MAX;
    
    if (c1 <= l || c2 <= l){
        std::cout << "YES BOTH " << std::min(c1,c2) << "MINUTES" << std::endl;
        if (c1 <= c2 && printPath){
            std::vector<int> s_to_x;
            std::vector<int> x_to_y;
            std::vector<int> y_to_d;
            
            int cur_v = x;
            while (cur_v != -1){
                s_to_x.insert(s_to_x.begin(), cur_v);
                cur_v = track_s[cur_v];
            }

            
            cur_v = y;
            while (cur_v != -1){
                x_to_y.insert(x_to_y.begin(), cur_v);
                cur_v = track_x[cur_v];
            }

            
            cur_v = d;
            while (cur_v != -1){
                y_to_d.insert(y_to_d.begin(), cur_v);
                cur_v = track_y[cur_v];
            }

            std::vector<int> s_to_d;
            int s1 = s_to_x.size();
            int s2 = x_to_y.size();
            int s3 = y_to_d.size();
            
            for (int i=0;i<s1;i++) s_to_d.push_back(s_to_x[i]);
            for (int i=1;i<s2;i++) s_to_d.push_back(x_to_y[i]);
            for (int i=1;i<s3;i++) s_to_d.push_back(y_to_d[i]);

            PrintRange(s_to_d.begin(), s_to_d.end());
        }
        else if (c2 < c1 && printPath){
            std::vector<int> s_to_y;
            std::vector<int> y_to_x;
            std::vector<int> x_to_d;
            
            int cur_v = y;
            while (cur_v != -1){
                s_to_y.insert(s_to_y.begin(), cur_v);
                cur_v = track_s[cur_v];
            }
            
            cur_v = x;
            while (cur_v != -1){
                y_to_x.insert(y_to_x.begin(), cur_v);
                cur_v = track_y[cur_v];
            }
            
            cur_v = d;
            while (cur_v != -1){
                x_to_d.insert(x_to_d.begin(), cur_v);
                cur_v = track_x[cur_v];
            }
            
            std::vector<int> s_to_d;
            int s1 = s_to_y.size();
            int s2 = y_to_x.size();
            int s3 = x_to_d.size();
            
            for (int i=0;i<s1;i++) s_to_d.push_back(s_to_y[i]);
            for (int i=1;i<s2;i++) s_to_d.push_back(y_to_x[i]);
            for (int i=1;i<s3;i++) s_to_d.push_back(x_to_d[i]);
            
            PrintRange(s_to_d.begin(), s_to_d.end());
        }
    }
    
    else{
        int c3 = d_s[x] + d_x[d];
        int c4 = d_s[y] + d_y[d];
        if (c3 <= c4 && c3 <= l){
            std::cout << "YES PRINTER " << c3 << "MINUTES" << std::endl;
            if (printPath){
                std::vector<int> s_to_x;
                std::vector<int> x_to_d;
                
                int cur_v = x;
                while (cur_v != -1){
                    s_to_x.insert(s_to_x.begin(), cur_v);
                    cur_v = track_s[cur_v];
                }
                
                cur_v = d;
                while (cur_v != -1){
                    x_to_d.insert(x_to_d.begin(), cur_v);
                    cur_v = track_x[cur_v];
                }
                
                std::vector<int> s_to_d;
                int s1 = s_to_x.size();
                int s2 = x_to_d.size();

                for (int i=0;i<s1;i++) s_to_d.push_back(s_to_x[i]);
                for (int i=1;i<s2;i++) s_to_d.push_back(x_to_d[i]);

                PrintRange(s_to_d.begin(), s_to_d.end());
            }
        }

        else if (c4 < c3 && c4 <= l ){
            std::cout << "YES DORM " << c4 << "MINUTES" << std::endl;
            if (printPath){
                std::vector<int> s_to_y;
                std::vector<int> y_to_d;
                
                int cur_v = y;
                while (cur_v != -1){
                    s_to_y.insert(s_to_y.begin(), cur_v);
                    cur_v = track_s[cur_v];
                }
                
                cur_v = d;
                while (cur_v != -1){
                    y_to_d.insert(y_to_d.begin(), cur_v);
                    cur_v = track_y[cur_v];
                }
                
                std::vector<int> s_to_d;
                int s1 = s_to_y.size();
                int s2 = y_to_d.size();

                for (int i=0;i<s1;i++) s_to_d.push_back(s_to_y[i]);
                for (int i=1;i<s2;i++) s_to_d.push_back(y_to_d[i]);

                PrintRange(s_to_d.begin(), s_to_d.end());
            }
        }
        else{
            int c5 = d_s[d];
            if (c5 <= l){
                std::cout << "YES DIRECTLY "<< c5 << "MINUTES" << std::endl;
                if (printPath){
                    std::vector<int> s_to_d;
                     
                    int cur_v = d;
                    while (cur_v != -1){
                        s_to_d.insert(s_to_d.begin(), cur_v);
                        cur_v = track_s[cur_v];
                    }
                     
                    PrintRange(s_to_d.begin(), s_to_d.end());
                }
            }
            else std::cout << "IMPOSSIBLE" << std::endl;
        }
    }

    
}