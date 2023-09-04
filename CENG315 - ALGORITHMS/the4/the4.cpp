#include "the4.h"


int dp_sln(char**& arr1, char**& arr2, int nrow, int ncol1, int ncol2, int**& mem){
    mem[0][0] = 0;
    
    // Initialize first row of mem
    for (int i=0;i<ncol2;i++){
        // examine ith column of box-2 and count non empty cells
        int n_non_empty = 0;
        for (int j=0;j<nrow;j++) if (arr2[j][i] != '-') n_non_empty++;
        mem[0][i+1] = mem[0][i] + n_non_empty;
    }
    
    // Initialize first column of mem
    for (int i=0;i<ncol1;i++){
        // examine ith column of box1 and count non empty cells
        int n_non_empty = 0;
        for (int j=0;j<nrow;j++) if (arr1[j][i] != '-') n_non_empty++;
        mem[i+1][0] = mem[i][0] + n_non_empty;
    }
    
    // Iterate over the sub-grid, removing first row and first column of the initial grid
    for (int i=1;i<=ncol1;i++){
        for (int j=1;j<=ncol2;j++){
            // Target cell of the mem is mem[i][j] 
            // Left cell is mem[i][j-1]
            // Top cell is mem[i-1][j]
            // Left corner cell is mem[i-1][j-1]
            
            
            int insertion_cost = 0;
            for (int k=0;k<nrow;k++) if (arr2[k][j-1] != '-') insertion_cost++;
            int cost1 = insertion_cost + mem[i][j-1];
            
            int deletion_cost = 0;
            for (int k=0;k<nrow;k++) if (arr1[k][i-1] != '-') deletion_cost++;
            int cost2 = deletion_cost + mem[i-1][j];
            
            int possibility1_cost = 0; // insertion and deletion
            for (int k=0;k<nrow;k++) if (arr2[k][j-1] != '-') possibility1_cost++;
            for (int k=0;k<nrow;k++) if (arr1[k][i-1] != '-') possibility1_cost++;

            int possibility2_cost = 0; // replacement
            for (int k=0;k<nrow;k++){
                if (arr1[k][i-1] != arr2[k][j-1]){
                    if ((arr1[k][i-1] != '-') && (arr2[k][j-1] != '-')) possibility2_cost+=1;
                    else if ((arr1[k][i-1] == '-') && (arr2[k][j-1] != '-')) possibility2_cost+=2;
                    else if ((arr1[k][i-1] != '-') && (arr2[k][j-1] == '-')) possibility2_cost+=2;
                }
            }
            
            
        
            // Check if reordering is possible
            char* column1 = new char[nrow];
            char* column2 = new char[nrow];
            char* column1_c = new char[nrow];
            char* column2_c = new char[nrow];
            
            for (int k=0;k<nrow;k++) column1[k] = arr1[k][i-1];
            for (int k=0;k<nrow;k++) column2[k] = arr2[k][j-1];
            for (int k=0;k<nrow;k++) column1_c[k] = arr1[k][i-1];
            for (int k=0;k<nrow;k++) column2_c[k] = arr2[k][j-1];


            // Sort both columns
            for (int a=1;a<nrow;a++){
                int b = a;
                while (b>=1){
                    if (column1[b-1] > column1[b]){
                        int temp = column1[b-1];
                        column1[b-1] = column1[b];
                        column1[b] = temp;
                        b--;
                    }
                    else break;
                }
            }
            
            for (int a=1;a<nrow;a++){
                int b = a;
                while (b>=1){
                    if (column2[b-1] > column2[b]){
                        int temp = column2[b-1];
                        column2[b-1] = column2[b];
                        column2[b] = temp;
                        b--;
                    }
                    else break;
                }
            }
            
            bool is_reordering_possible = true;
            for (int k=0;k<nrow;k++) if (column1[k] != column2[k]) {is_reordering_possible = false; break;}
            
            if (is_reordering_possible == true){
                int possibility3_cost = 0;
                for (int k=0;k<nrow;k++) if (column1_c[k] != column2_c[k]) possibility3_cost++;
                int cost3 = std::min(possibility1_cost, std::min(possibility2_cost, possibility3_cost)) + mem[i-1][j-1];
                int net_min_cost = std::min(cost1, std::min(cost2, cost3));
                mem[i][j] = net_min_cost;
                        
            }
            else{
                int cost3 = std::min(possibility1_cost, possibility2_cost) + mem[i-1][j-1];
                int net_min_cost = std::min(cost1, std::min(cost2, cost3));
                mem[i][j] = net_min_cost;
            }
            
        
            delete[] column1;
            delete[] column2;
            delete[] column1_c;
            delete[] column2_c;
        }
    }

    return mem[ncol1][ncol2];
}

