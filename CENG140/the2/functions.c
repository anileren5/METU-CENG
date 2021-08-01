#include <stdlib.h>
#include <stdio.h>

#define n_sector 3


float*** create_sector_times(unsigned int* n_drivers, unsigned int* n_laps){
    int i,j,k;
    float ***table;
    int number_of_drivers,number_of_laps;
    
    scanf("%u %u",n_drivers,n_laps);
    number_of_drivers = *n_drivers;
    number_of_laps = *n_laps;

    table = malloc(sizeof(float**)*number_of_drivers);
    
    for (i=0;i<number_of_drivers;++i){
        *(table+i) = malloc(sizeof(float*)*number_of_laps);
        for (j=0;j<number_of_laps;++j){
            *(*(table+i)+j) = malloc(sizeof(float)*3);
        }
    }

    for (i=0;i<number_of_drivers;++i){
        for (j=0;j<number_of_laps;++j){
            for (k=0;k<3;++k) {
                scanf("%f",&table[i][j][k]);
            }       
        }
    }

    return table;
}


unsigned int** create_positions(unsigned int* p_drivers, unsigned int* n_races){
    int i,j;
    unsigned int number_of_drivers,number_of_races;
    unsigned int** table;
    
    scanf("%u %u",p_drivers,n_races);
    number_of_drivers = *p_drivers;
    number_of_races = *n_races;
    
    table = malloc(sizeof(unsigned int*)*number_of_drivers);

    for (i=0;i<number_of_drivers;++i){
        *(table+i) = malloc(sizeof(unsigned int)*number_of_races);
    }

    for (i=0;i<number_of_drivers;++i){
        for (j=0;j<number_of_races;++j){
            scanf("%u",&table[i][j]);
        }
    }

    return table;
}


float** calculate_lap_times(float*** sector_times, unsigned int n_drivers, unsigned int n_laps){
    int i,j,k;
    float** lap_times;
    lap_times = malloc(sizeof(float*)*n_drivers);

    for (i=0;i<n_drivers;++i){
        *(lap_times+i) = malloc(sizeof(float)*n_laps);
    }

    for (i=0;i<n_drivers;++i){
        for (j=0;j<n_laps;++j){
            float current_lap_time;
            current_lap_time = 0;
            for (k=0;k<3;++k){
                current_lap_time += sector_times[i][j][k];
            }
            lap_times[i][j] = current_lap_time; 
        }
    }

    return lap_times;
}


unsigned int find_fastest_lap(float** lap_times, unsigned int n_drivers, unsigned int n_laps){
    float absolute_value(float x,float y);
    int i,j;
    float* bests; /* This array contains the fastest lap times for each drivers. */
    float the_best; /* This is the really fastest lap time in the table. */
    unsigned int driver_id; /* This is the id of the driver who has the fastest lap time (starting from 0).*/ 
    bests = malloc(sizeof(float)*n_drivers);

    for (i=0;i<n_drivers;++i){
        float min; /* This variable will hold the fastest lap time of current driver */
        min = lap_times[i][0];
        for (j=0;j<n_laps;++j){
            if (lap_times[i][j]<min) min = lap_times[i][j];
        }
        bests[i] = min;
    }

    the_best = bests[0];
    for (i=0;i<n_drivers;++i){
        if (bests[i]<the_best) the_best = bests[i];
    }


    for (i=0;i<n_drivers;++i){
        if (absolute_value(bests[i],the_best) <= 0.0001){
            driver_id = i;
            break; /* Explained as below. */
        }
        /* Thanks to this loop, i can find the the id of driver who has fastest lap time. In case two driver has the same lap time, driver with smaller id is returned */
    }

    return driver_id;
}


unsigned int find_driver_fastest_lap(float** sector_times_of_driver,unsigned int n_laps){
    float absolute_value(float x,float y);
    int i,j;    
    float* lap_times_of_driver; /* This array will contain lap times of given driver. */
    float fastest_lap_time; /* This will hold the minimum value of lap times in lap_times_of_driver. */
    unsigned int lap_number; /* This will hold the number of fastest lap and will be returned */ 
    
    lap_times_of_driver = malloc(sizeof(float)*n_laps);

    for (i=0;i<n_laps;++i){
        float lap_time;
        lap_time = 0;
        for (j=0;j<3;++j){
            lap_time += sector_times_of_driver[i][j];
        }
        lap_times_of_driver[i] = lap_time;
    }

    fastest_lap_time = lap_times_of_driver[0];
    for (i=0;i<n_laps;++i){
        if (lap_times_of_driver[i]<fastest_lap_time) fastest_lap_time = lap_times_of_driver[i];
    } 

    for (i=0;i<n_laps;++i){
        if (absolute_value(lap_times_of_driver[i],fastest_lap_time) <= 0.0001){
            /* By this loop, in case laps have the same time, the lap with the smaller number will be chosen. */
            lap_number = i;
            break;
        }
    }

    return lap_number;
}


float* selection_sort(float* arr, unsigned int len, char ord){
    int i;
    float* copied_arr;
    copied_arr = malloc(sizeof(float)*len);
    for (i=0;i<len;++i) *(copied_arr+i) = *(arr+i);
    if (len == 1) return copied_arr;
    
    if (ord == 'A')
    {
        int start_of_unsorted,min_index_in_unsorted,cursor;

        for (start_of_unsorted = 0;start_of_unsorted <= (len-2);++start_of_unsorted){
            float temporary;
            min_index_in_unsorted = start_of_unsorted;
            for (cursor = start_of_unsorted+1;cursor <= (len-1);cursor++){
                if (copied_arr[cursor] < copied_arr[min_index_in_unsorted]){
                    min_index_in_unsorted = cursor;
                }
            }

            temporary = copied_arr[start_of_unsorted];
            copied_arr[start_of_unsorted] = copied_arr[min_index_in_unsorted];
            copied_arr[min_index_in_unsorted] = temporary;
        }
    }

    else if (ord == 'D')
    {
        
        int i,start_of_unsorted,min_index_in_unsorted,cursor;
        float* copy_of_copied_array;
        for (start_of_unsorted = 0;start_of_unsorted <= (len-2);++start_of_unsorted){
            float temporary;
            min_index_in_unsorted = start_of_unsorted;
            for (cursor = start_of_unsorted+1;cursor <= (len-1);cursor++){
                if (copied_arr[cursor] < copied_arr[min_index_in_unsorted]){
                    min_index_in_unsorted = cursor;
                }
            }

            temporary = copied_arr[start_of_unsorted];
            copied_arr[start_of_unsorted] = copied_arr[min_index_in_unsorted];
            copied_arr[min_index_in_unsorted] = temporary;

        } /* Now we have increasing order, we need to reverse it. */

        copy_of_copied_array = malloc(sizeof(float)*len);
        for (i=0;i<len;++i) copy_of_copied_array[i] = copied_arr[i];

        for (i=0;i<len;++i) copy_of_copied_array[len-1-i] = copied_arr[i];

        for (i=0;i<len;++i) copied_arr[i] = copy_of_copied_array[i];
    }

    return copied_arr;
}


unsigned int* find_finishing_positions(float** lap_times, unsigned int n_drivers, unsigned int n_laps){
    unsigned int* helper_7(float* array,unsigned int len);
    int i,j;
    float* complete_times;
    unsigned int* rankings; /* Rankigs that will be returned. */
    complete_times = malloc(sizeof(float)*n_drivers);
    rankings = malloc(sizeof(unsigned int)*n_drivers);

    for (i=0;i<n_drivers;++i){
        float sum = 0;
        for (j=0;j<n_laps;++j){
            sum += lap_times[i][j];
        }
        complete_times[i] = sum;
    }

    rankings = helper_7(complete_times,n_drivers);
    return rankings;

}

unsigned int* helper_7(float* array,unsigned int len){
    unsigned int* indices; /* Now it is corresponding indices, it will sorted, at the end will be returned */
    int i,j;
    indices = malloc(sizeof(unsigned int)*len);
    for (i=0;i<len;i++) indices[i] = i;
    for (j=0;j<=len;++j){
        for (i=0;i<(len-1);i++){
            if ((array[i]-array[i+1])>0.0001){
                float temp;
                unsigned int temp_index;
                temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
                temp_index = indices[i];
                indices[i] = indices[i+1];
                indices[i+1] = temp_index;

            }
        }
    }
    return indices;
}


float* find_time_diff(float** lap_times, unsigned int n_drivers, unsigned int n_laps, unsigned int driver1,unsigned int driver2){
    int i;
    float accumulator;
    float* differences; /* This will be returned */
    float* lap_times_of_driver1;
    float* lap_times_of_driver2;
    differences = malloc(sizeof(float)*n_laps);
    lap_times_of_driver1 = malloc(sizeof(float)*n_laps);
    lap_times_of_driver2 = malloc(sizeof(float)*n_laps);

    lap_times_of_driver1 = lap_times[driver1];
    lap_times_of_driver2 = lap_times[driver2];

    accumulator = 0;
    for (i=0;i<n_laps;i++){
        float current_difference;
        current_difference = lap_times_of_driver1[i]-lap_times_of_driver2[i];
        accumulator += current_difference;
        differences[i] = accumulator;
    }
    
    return differences;
}


unsigned int* calculate_total_points(unsigned int** positions, unsigned int p_drivers, unsigned int n_races){
    int i,j;
    unsigned int* position_point_mapping;
    unsigned int* total_points; /* This will be returned */
    
    position_point_mapping = malloc(sizeof(unsigned int)*p_drivers);
    for (i=0;i<p_drivers;++i) scanf("%u",(position_point_mapping+i));

    total_points = malloc(sizeof(unsigned int)*p_drivers);

    for (i=0;i<p_drivers;++i){
        unsigned int total_point_of_current_driver;
        total_point_of_current_driver = 0;
        for (j=0;j<n_races;++j) total_point_of_current_driver += position_point_mapping[positions[i][j]-1];
        total_points[i] = total_point_of_current_driver;
    }

    return total_points ;
}


unsigned int find_season_ranking(unsigned int* total_points, unsigned int p_drivers, unsigned int id){
    unsigned int* mysort(unsigned int* array,unsigned int len);
    int i,n_seen;
    unsigned int* total_points_decreasing;
    unsigned int* copy_total_points;
    unsigned int point_of_given_driver;
    unsigned int rank;  /* This will be returned. */
    copy_total_points = malloc(sizeof(unsigned int)*p_drivers);
    for (i=0;i<p_drivers;i++) copy_total_points[i] = total_points[i];
    total_points_decreasing = mysort(copy_total_points,p_drivers);
    point_of_given_driver = total_points[id];
    n_seen = 0;
    for (i=0;i<p_drivers;i++) if (total_points[i] == point_of_given_driver) n_seen+=1;

    if (n_seen == 1){
        unsigned int index;
        for (i=0;i<p_drivers;i++){
            if (total_points_decreasing[i] == point_of_given_driver){
                index = i;
            }
        }
        rank = index+1;
    }

    else if (n_seen > 1){
        unsigned int first_seen_index;
        int x;
        unsigned int* ids_with_desired_point;
        ids_with_desired_point = malloc(sizeof(unsigned int)*n_seen);
        x=0;
        for (i=0;i<p_drivers;i++) {
            if (total_points[i] == point_of_given_driver){
                ids_with_desired_point[x] = i;
                x+=1;
            }
        }

        for (i=0;i<p_drivers;i++){
            if (total_points_decreasing[i] == point_of_given_driver){
                first_seen_index = i;
                break;
            }
        }

        for (i=0;i<n_seen;i++){
            if (ids_with_desired_point[i] == id) break;
            else first_seen_index+=1;
        }

        rank = first_seen_index + 1;
    }

    return rank;
}

unsigned int* mysort(unsigned int* array,unsigned int len){
    int i,j;
    unsigned int* sorted_array;
    sorted_array = malloc(sizeof(unsigned int)*len);
    for (i=0;i<len;i++) sorted_array[i] = array[i];
    for (j=0;j<=len;++j){
        for (i=0;i<(len-1);i++){
            if (sorted_array[i]<sorted_array[i+1]){
                unsigned int temp;
                temp = sorted_array[i];
                sorted_array[i] = sorted_array[i+1];
                sorted_array[i+1] = temp;
            }
        }
    }  
    return sorted_array;
}

float absolute_value(float x,float y){
    if (x>=y) return (x-y);
    else return (y-x);
}
