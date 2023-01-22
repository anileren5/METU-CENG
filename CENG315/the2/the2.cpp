#include "the2.h"
#include <cmath>
// You may write your own helper functions here


int lastDigit(int n){return n%10;};

int parser(int n, int groupSize, int group_index){ // group index starts from 0
    int group = 0;
    int start_index = group_index * groupSize;
    int d = 1;
    for (int i=0; i<start_index;i++) n /= 10;
    for (int i=0;i<groupSize;i++){group += (lastDigit(n) * d); d*=10; n/=10;}
    return group;
}

void countingSort(int* A, bool ascending, int arraySize, int groupSize, int group_index, long& numberOfIterations){
    int size = pow(10, groupSize); 
    int* B = new int[arraySize];
    int* C = new int[size];
    int x = 0;
    for (int i=0;i<size;i++) C[i] = 0;
    for (int i=0;i<arraySize;i++) {C[parser(A[i],groupSize,group_index)]++;numberOfIterations++;}
    for (int i=1;i<size;i++) {C[i]= C[i-1] + C[i];numberOfIterations++;}
    for (int i=arraySize-1;i>=0;i--){
        B[C[parser(A[i], groupSize, group_index)]-1] = A[i];
        C[parser(A[i], groupSize, group_index)]--;
        numberOfIterations++;
    }
    for (int i=0;i<arraySize;i++) {A[i] = B[i]; numberOfIterations++;}    
    delete[] B;
    delete[] C;
}

long multiDigitRadixSort(int* arr, bool ascending, int arraySize, int groupSize, int maxDigitLength){
    long numberOfIterations = 0;
    int m = maxDigitLength / groupSize;
    int n = maxDigitLength % groupSize;
    if (n) m++;
    long k = 0;
    for (int i=0;i<m;i++) countingSort(arr, ascending, arraySize, groupSize, i, numberOfIterations);
    

    while (maxDigitLength){
        if (maxDigitLength < groupSize){
            k += ((3*arraySize) + pow(10,maxDigitLength)) - 1;
            maxDigitLength = 0;
        }
        else{
            k += ((3 * arraySize) + pow(10,groupSize)-1);
            maxDigitLength = maxDigitLength - groupSize;
        }
    }
    numberOfIterations = k;

    if (!ascending){
        int* R = new int[arraySize];
        for (int i=0;i<arraySize;i++) R[i] = arr[arraySize - i - 1];
        for (int i=0;i<arraySize;i++) arr[i] = R[i];
        delete[] R;
    }

    return numberOfIterations;
}