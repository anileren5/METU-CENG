#include "stdio.h"
#include "lab2.h"

/* TASK SOLUTIONS */


void to_lower(char * sentence){
    int i;
    for (i=0;;++i){
        if (*(sentence+i) == '!') break;
        else if (*(sentence+i)<='Z' && *(sentence+i)>='A') *(sentence+i)+=('a'-'A');
    }
}

void print_sentence(char * sentence){
    int i;
    for (i=0;;++i){
        if (*(sentence+i)=='!') break;
        else printf("%c",*(sentence+i));
    }
    printf("\n");
}

void substrings(char * sentence, char * substr, int * arr){
    int len_sub,len_sen,i;
    int append_index = 0;
    len_sub=len_sen=0;
    
    for (i=0;;++i){
        if (*(substr+i) == '!') break;
        else len_sub++;
    }
    
    for (i=0;;++i){
        if (*(sentence+i) == '!') break;
        else len_sen++;
    }
    
    for (i=0;i<(len_sen-len_sub);++i){
        int flag = 1;
        int j;
        for (j=0;j<len_sub;++j){
            if (*(sentence+i+j) != *(substr+j)) flag = 0;
        }
        
        if (flag == 1){
            arr[append_index] = i;
            ++append_index;
        }
    }
}

void removeX(char * sentence, int * arr, int count){
    void shift(char* sentence,int position,int amount);
    int i;
    int j=0;
    for (i=0;i<10;++i){
        if (arr[i]==0) break;
        else shift(sentence,arr[i]-(count*j),count);
        j++;
    }
}

void addSome(char * sentence, int * arr, char * substr){
    void add(char* sentence,int position,char* substr);
    int i,len_sub;
    len_sub=0;
    for (i=0;;++i){
        if (*(substr+i)=='!') break;
        else len_sub+=1;
    }
    
    for (i=0;i<10;++i){
        if (arr[i]==0) break;
        else add(sentence,arr[i]+(i*len_sub),substr);
    }
}

void battle_ship(char map[mapWidth][mapHeight], int prev_moves[prevCount][2], int next_moves[nextCount][2]){
    int i,j;
    int append_index = 0;
    for (i=0;;++i){
        int x,y;
        if (prev_moves[i][0] == -1 && prev_moves[i][1] == -1) break;
        x= prev_moves[i][0];
        y= prev_moves[i][1];
        map[x][y] = 'H';
    }
    
    for (i=0;i<mapWidth;++i){
        for (j=0;j<mapHeight;++j){
            if (map[i][j]=='X'){
                next_moves[append_index][0]=i;
                next_moves[append_index][1]=j;
                append_index++;
            }
        }
    }
    next_moves[append_index][0]=-1;
    next_moves[append_index][1]=-1;
}

void shift(char* sentence,int position,int amount){
    int i,len_sen; 
    for (i=0;;++i){
        if (sentence[i] == '!') break;
        else len_sen++;
    }
    for (i=(position);i<=len_sen;++i) *(sentence+i) = *(sentence+i+amount);
}

void add(char* sentence,int position,char* substr){
    int len_sub,len_sen,i;
    len_sub=len_sen=0;
    
    for (i=0;;++i){
        if (*(sentence+i)=='!') break;
        else len_sen++;
    }
    
    for (i=0;;++i){
        if (*(substr+i)=='!') break;
        else len_sub++;
    }
    
    for (i=len_sen;i>=position;--i){
        *(sentence+i+len_sub) = *(sentence+i);
    }
    
    for (i=0;i<len_sub;++i) *(sentence+position+i) = *(substr+i);
}