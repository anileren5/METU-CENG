#include "the1.h"
#include <climits>
class Partition{
    private:
		int* arr;
		int capacity; // maximum size
        int size;
	public:
	    int* getArray(){return arr;}
        int getSize(){return size;}
        int getHead(){return this->arr[0];}
		bool isEmpty(){return (size == 0);}
		void createPartition(int capacity){
            // Initialize the partition like a constructor but for ease it is implemented seperately.
            this->capacity = capacity;
			this->arr = new int[capacity];
            this->size = capacity;
        }
        int RemoveHead(){
            // Remove the min element and return it. 
            // Shift
            int head = this->getHead();
            int tracker = 0; 
            while (tracker < (size-1)){
                arr[tracker] = arr[tracker+1];
                tracker++;
            }
            size--;
            arr[0] = (this->isEmpty()) ? INT_MAX : arr[0]; 
            return head;
        }

};
void swapf(Partition* p1, Partition* p2){
    Partition temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
void MinHeapify(Partition* partitions, int size, int i, long& comparison, long& swap){
    if (size == 1) return;
    else{
        int l = 2 * i;
        int r = 2 * i + 1;
        int smallest = i;
        // Check left child
        if ((l < (size + 1)) && (!partitions[l].isEmpty())){
            comparison++;
            if (partitions[l].getHead() < partitions[smallest].getHead()) smallest = l;
        }
        // Check right child
        if ((r < (size + 1)) && (!partitions[r].isEmpty())){
            comparison++;
            if (partitions[r].getHead() < partitions[smallest].getHead()) smallest = r;
        }
        // Go down
        if (smallest != i){
            swap++;
            swapf(&partitions[smallest], &partitions[i]);
            MinHeapify(partitions, size,smallest,comparison,swap);
        }         
    }
}
int kWayMergeSortWithHeap(int* arr, int K, int size, long& comparison, long& swap){
  	int number_of_calls = 1;
	if(size < K){
        for (int i=1;i<size;i++){
            int j = i;
            while (j>=1){
                comparison++;
                if (arr[j-1] > arr[j]){
                    swap++;
                    int temp = arr[j-1];
                    arr[j-1] = arr[j];
                    arr[j] = temp;
                    j--;
                }
                else break;
            }
		}
		return number_of_calls;
	}
	Partition* partitions = new Partition[K+1];
    for (int i=1;i<K+1;i++){
	    partitions[i].createPartition(size/K);
	    for(int j = 0;j < size/K;j++){
	        partitions[i].getArray()[j] = arr[(i-1)*(size/K) +j];
	    }
	}
	for (int i=1;i<K+1;i++) number_of_calls += kWayMergeSortWithHeap(partitions[i].getArray(),K,size/K,comparison,swap);
	for(int i= K/2;i > 0;i--) MinHeapify(partitions,K,i,comparison,swap);
	int i = 0;
	while (!partitions[1].isEmpty()){
	    arr[i] = partitions[1].getHead();
	    partitions[1].RemoveHead();
	    MinHeapify(partitions,K,1,comparison,swap);
	    i++;
	}
    return number_of_calls;
}