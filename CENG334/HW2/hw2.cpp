#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <iostream>

#include "hw2_output.h"

using namespace std;


// Input will be stored in the following variables globally.
int NROW1, NCOLUMN1, NROW2, NCOLUMN2, NROW3, NCOLUMN3, NROW4, NCOLUMN4;
vector<vector<int>> matrix1, matrix2, matrix3, matrix4;

// Result Matrices
vector<vector<int>> additionResultMatrix1, additionResultMatrix2, multiplicationResultMatrix;

// This will include the semaphores corressponding to each row of result of first addition.
vector<sem_t> rowCompletedSemaphores; 

// This will include the counters corresponding to each column of result of second addition. 
// They keep track of number of calculated entries in each column of result of second adddition.
vector<int> columnReadyEntriesCounters; 

// This will include mutexes for incrementing or reading elements of columnReadyEntriesCounters in a safe way.
vector<pthread_mutex_t> columnReadyEntriesCounterMutexes;

// This will include condition variable for each column of result of second addition for signaling completion. 
vector<pthread_cond_t> columnReady;



// Thread routine for threads for rows of first addition
void* sum1(void* arg){
    int index = *((int*)arg);   
    for (int j=0;j<NCOLUMN1;j++){
        int sum = matrix1[index][j] + matrix2[index][j];
        hw2_write_output(0, index+1, j+1, sum);
        additionResultMatrix1[index][j] = sum;
    }
    sem_post(&rowCompletedSemaphores[index]);
    delete (int*) arg;
    return NULL;
}

// Thread routine for threads for rows of second addition
void* sum2(void* arg){
    int index = *((int*)arg);
    for (int j=0;j<NCOLUMN3;j++){
        int sum = matrix3[index][j] + matrix4[index][j];
        hw2_write_output(1, index+1, j+1, sum);
        additionResultMatrix2[index][j] = sum;
        
        pthread_mutex_lock(&columnReadyEntriesCounterMutexes[j]);
        columnReadyEntriesCounters[j] += 1;
        if (columnReadyEntriesCounters[j] == NROW3) pthread_cond_broadcast(&columnReady[j]);
        pthread_mutex_unlock(&columnReadyEntriesCounterMutexes[j]);
    }
    delete (int*) arg;
    return NULL;
}

// Thread routine for threads for rows of final multiplication
void* multiply(void* arg){
    int index = *((int*)arg);

    sem_wait(&rowCompletedSemaphores[index]);
    // Once you get there, this means that corresponding row is ready.

    for (int j=0;j<NCOLUMN3;j++){ // j iterates over columns of second addition result matrix
        pthread_mutex_lock(&columnReadyEntriesCounterMutexes[j]);
        while (columnReadyEntriesCounters[j] < NROW3){
            //pthread_mutex_unlock(&columnReadyEntriesCounterMutexes[j]);
            pthread_cond_wait(&columnReady[j], &columnReadyEntriesCounterMutexes[j]);
        }
        pthread_mutex_unlock(&columnReadyEntriesCounterMutexes[j]);

        // Once you get there, this means that corresponding column is ready.

        int entry = 0;
        for (int k=0;k<NCOLUMN1;k++){
            int multiplication = additionResultMatrix1[index][k] * additionResultMatrix2[k][j];
            entry += multiplication;
        }
        multiplicationResultMatrix[index][j] = entry;

        hw2_write_output(2, index+1, j+1, entry);
    }

    delete (int*) arg;
    return NULL;
}

int main(){
    hw2_init_output(); // called at the start of the program

    vector<pthread_t> additionThreads1; // Threads responsible for rows of the first addition
    vector<pthread_t> additionThreads2; // Threads responsible for rows of the second addition
    vector<pthread_t> multiplicationThreads; // Threads responsible for final multiplication

    // Take input
    cin >> NROW1 >> NCOLUMN1;
    for (int i=0;i<NROW1;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN1;j++){
            row.push_back(-1);
        }
        matrix1.push_back(row);
    }
    for (int i=0;i<NROW1;i++){
        for (int j=0;j<NCOLUMN1;j++){
            cin >> matrix1[i][j];
        }
    }

    cin >> NROW2 >> NCOLUMN2;
    for (int i=0;i<NROW2;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN2;j++){
            row.push_back(-1);
        }
        matrix2.push_back(row);
    }    
    for (int i=0;i<NROW2;i++){
        for (int j=0;j<NCOLUMN2;j++){
            cin >> matrix2[i][j];
        }
    }

    cin >> NROW3 >> NCOLUMN3;
    for (int i=0;i<NROW3;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN3;j++){
            row.push_back(-1);
        }
        matrix3.push_back(row);
    } 
    for (int i=0;i<NROW3;i++){
        for (int j=0;j<NCOLUMN3;j++){
            cin >> matrix3[i][j];
        }
    }    

    cin >> NROW4 >> NCOLUMN4;
    for (int i=0;i<NROW4;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN4;j++){
            row.push_back(-1);
        }
        matrix4.push_back(row);
    }     
    for (int i=0;i<NROW4;i++){
        for (int j=0;j<NCOLUMN4;j++){
            cin >> matrix4[i][j];
        }
    } 

    // Initialize result matrices
    for (int i=0;i<NROW1;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN1;j++){
            row.push_back(-1);
        }
        additionResultMatrix1.push_back(row);
    }
    for (int i=0;i<NROW3;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN3;j++){
            row.push_back(-1);
        }
        additionResultMatrix2.push_back(row);
    }
    for (int i=0;i<NROW1;i++){
        vector<int> row;
        for (int j=0;j<NCOLUMN3;j++){
            row.push_back(-1);
        }
        multiplicationResultMatrix.push_back(row);
    }

    // Initialize rowCompletedSemaphores
    for (int i=0;i<NROW1;i++){
        sem_t semaphore;
        sem_init(&semaphore, 0, 0);
        rowCompletedSemaphores.push_back(semaphore);
    }

    // Initialize columnReadyEntriesCounters
    for (int i=0;i<NCOLUMN3;i++) columnReadyEntriesCounters.push_back(0);

    // Initialize columnReadyEntriesCounterMutexes
    for (int i=0;i<NCOLUMN3;i++){
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        columnReadyEntriesCounterMutexes.push_back(mutex);
    }

    // Initialize columnReady
    for (int i=0;i<NCOLUMN3;i++){
        pthread_cond_t conditionVariable;
        pthread_cond_init(&conditionVariable, NULL);
        columnReady.push_back(conditionVariable);
    }

    // Fill thread vectors
    for (int i=0;i<NROW1;i++){pthread_t thread; additionThreads1.push_back(thread);}
    for (int i=0;i<NROW3;i++){pthread_t thread; additionThreads2.push_back(thread);}
    for (int i=0;i<NROW1;i++){pthread_t thread; multiplicationThreads.push_back(thread);}

    // Create threads
    for (int i=0;i<NROW1;i++){
        int* index = new int; // Do not forget to free this memory !
        *index = i; // I started rows from 0 but do not forget to pass index + 1 to hw2_write_output so that they start from 1 !
        if (pthread_create(&additionThreads1[i], NULL, &sum1, index) != 0) cerr << "An error occured during thread creation!" << endl; 
    }       
    for (int i=0;i<NROW3;i++){
        int* index = new int; // Do not forget to free this memory !
        *index = i; // I started rows from 0 but do not forget to pass index + 1 to hw2_write_output so that they start from 1 !
        if (pthread_create(&additionThreads2[i], NULL, &sum2, index) != 0) cerr << "An error occured during thread creation!" << endl;
    } 
    for (int i=0;i<NROW1;i++){
        int* index = new int;
        *index = i;
        if (pthread_create(&multiplicationThreads[i], NULL, &multiply, index) != 0) cerr << "An error occured during thread creation!" << endl; 
    }

    // Join threads
    for (int i=0;i<NROW1;i++) if (pthread_join(additionThreads1[i], NULL) != 0) cerr << "An error occured during thread join!" << endl;
    for (int i=0;i<NROW3;i++) if (pthread_join(additionThreads2[i], NULL) != 0) cerr << "An error occured during thread join!" << endl;
    for (int i=0;i<NROW1;i++) if (pthread_join(multiplicationThreads[i], NULL) != 0) cerr << "An error occured during thread join!" << endl;

    // Print Final Multiplication Result Matrix
    for (int i=0;i<NROW1;i++){
        for (int j=0;j<NCOLUMN3;j++) cout << multiplicationResultMatrix[i][j] << " ";
        cout << endl;
    }

    // Destroy rowCompletedSemaphores
    for (int i=0;i<NROW1;i++) sem_destroy(&rowCompletedSemaphores[i]);

    // Destroy columnReadyEntriesCounterMutexes
    for (int i=0;i<NCOLUMN3;i++) pthread_mutex_destroy(&columnReadyEntriesCounterMutexes[i]);

    // Destroy columnReady
    for (int i=0;i<NCOLUMN3;i++) pthread_cond_destroy(&columnReady[i]);

    return 0;
}
