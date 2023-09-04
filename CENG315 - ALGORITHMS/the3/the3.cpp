#include "the3.h"


int recursive_sln(int i, char**& arr, int*& len, int &number_of_calls){ //direct recursive
    number_of_calls+=1;
    //your code here

    int current_excluded = 0;
    int current_included = 0;
    int N = i;
    
    if (number_of_calls == 1){
        current_excluded = recursive_sln(N-1,arr,len,number_of_calls);
        for (int i=N-2;i>=0;i--){
            int p = recursive_sln(i,arr,len,number_of_calls);
            if (p > current_excluded) current_excluded = p;
        }
        bool flag = false;
        for (i=N-1;i>=0;i--) if ((arr[i][1] == 'S' && arr[N][0] == 'S') || (arr[i][1] == 'I' && arr[N][0] == 'O') || (arr[i][1] == 'O' && arr[N][0] == 'I')) {flag = true; break;}
        if (flag == true){
            for (i=N-1;i>=0;i--){
                if ((arr[i][1] == 'S' && arr[N][0] == 'S') || (arr[i][1] == 'I' && arr[N][0] == 'O') || (arr[i][1] == 'O' && arr[N][0] == 'I')){
                    flag = true;
                    current_included = recursive_sln(i,arr,len,number_of_calls) + len[N];
                    break;
                }
            }
        }
        if (flag == false) current_included = len[N];
    }
    else{
        for (int j=N-1;j>=0;j--){
            if (arr[j][1] == arr[N][1]){
                current_excluded = recursive_sln(j,arr,len,number_of_calls);
                break;
            }
        }
        bool flag = false;
        for (i=N-1;i>=0;i--) if ((arr[i][1] == 'S' && arr[N][0] == 'S') || (arr[i][1] == 'I' && arr[N][0] == 'O') || (arr[i][1] == 'O' && arr[N][0] == 'I')) {flag = true; break;}
        if (flag == true){
            for (i=N-1;i>=0;i--){
                if ((arr[i][1] == 'S' && arr[N][0] == 'S') || (arr[i][1] == 'I' && arr[N][0] == 'O') || (arr[i][1] == 'O' && arr[N][0] == 'I')){
                    flag = true;
                    current_included = recursive_sln(i,arr,len,number_of_calls) + len[N];
                    break;
                }
            }   
        }
        if (flag == false) current_included = len[N];
    }
    return std::max(current_excluded,current_included);
}





int memoization_sln(int i, char**& arr, int*& len, int**& mem) { //memoization

    if (i==0){
        if (arr[0][1] == 'I') {mem[0][0] = len[0]; mem[0][1]=0; mem[0][2]=0;}
        if (arr[0][1] == 'O') {mem[0][1] = len[0]; mem[0][0]=0; mem[0][2]=0;}
        if (arr[0][1] == 'S') {mem[0][2] = len[0]; mem[0][0]=0; mem[0][1]=0;}
        return std::max(mem[i][0],std::max(mem[i][1],mem[i][2]));
    }
    
    else{
        int temp = memoization_sln(i-1,arr,len,mem);
        int prev_i = mem[i-1][0];
        int prev_o = mem[i-1][1];
        int prev_s = mem[i-1][2];
        int length = len[i];
        
        if (arr[i][0] == 'I' && arr[i][1] == 'I'){
            int tmp = prev_o + length;
            if (tmp > prev_i) prev_i = tmp;
        }
        else if (arr[i][0] == 'I' && arr[i][1] == 'O'){
            int tmp = prev_o + length;
            if (tmp > prev_o) prev_o = tmp;
        } 
        else if (arr[i][0] == 'I' && arr[i][1] == 'S'){
            int tmp = prev_o + length;
            if (tmp > prev_s) prev_s = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'I'){
            int tmp = prev_i + length;
            if (tmp > prev_i) prev_i = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'O'){
            int tmp = prev_i + length;
            if (tmp > prev_o) prev_o = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'S'){
            int tmp = prev_i + length;
            if (tmp > prev_s) prev_s = tmp;
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'I'){
            int tmp = prev_s + length;
            if (tmp > prev_i) prev_i = tmp;         
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'O'){
            int tmp = prev_s + length;
            if (tmp > prev_o) prev_o = tmp;  
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'S'){
            int tmp = prev_s + length;
            if (tmp > prev_s) prev_s = tmp;        
        }
        
        mem[i][0] = prev_i;
        mem[i][1] = prev_o;
        mem[i][2] = prev_s;
        
        return std::max(mem[i][0],std::max(mem[i][1], mem[i][2]));
    }
    
}



int dp_sln(int size, char**& arr, int*& len, int**& mem) { //dynamic programming

    //your code here
    for (int i=0;i<size;i++) {mem[i][0]=0;mem[i][1]=0;mem[i][2]=0;}
    
    if (arr[0][1] == 'I') mem[0][0] = len[0];
    if (arr[0][1] == 'O') mem[0][1] = len[0];
    if (arr[0][1] == 'S') mem[0][2] = len[0];
    
    
    for (int i=1;i<size;i++){
        int prev_i = mem[i-1][0];
        int prev_o = mem[i-1][1];
        int prev_s = mem[i-1][2];
        int length = len[i];
        
        if (arr[i][0] == 'I' && arr[i][1] == 'I'){
            int tmp = prev_o + length;
            if (tmp > prev_i) prev_i = tmp;
        }
        else if (arr[i][0] == 'I' && arr[i][1] == 'O'){
            int tmp = prev_o + length;
            if (tmp > prev_o) prev_o = tmp;
        } 
        else if (arr[i][0] == 'I' && arr[i][1] == 'S'){
            int tmp = prev_o + length;
            if (tmp > prev_s) prev_s = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'I'){
            int tmp = prev_i + length;
            if (tmp > prev_i) prev_i = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'O'){
            int tmp = prev_i + length;
            if (tmp > prev_o) prev_o = tmp;
        }
        else if (arr[i][0] == 'O' && arr[i][1] == 'S'){
            int tmp = prev_i + length;
            if (tmp > prev_s) prev_s = tmp;
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'I'){
            int tmp = prev_s + length;
            if (tmp > prev_i) prev_i = tmp;         
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'O'){
            int tmp = prev_s + length;
            if (tmp > prev_o) prev_o = tmp;  
        }
        else if (arr[i][0] == 'S' && arr[i][1] == 'S'){
            int tmp = prev_s + length;
            if (tmp > prev_s) prev_s = tmp;        
        }

        mem[i][0] = prev_i;
        mem[i][1] = prev_o;
        mem[i][2] = prev_s;
    }
    
    return std::max(mem[size-1][0],std::max(mem[size-1][1], mem[size-1][2]));
}