#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/poll.h>


#include "logging.h"
#include "message.h"

#include "GameEntities.hpp"

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

using namespace std;

int abs_diff(int x, int y){return x >= y ? x - y : y - x;}
int manhattan_distance(coordinate c1, coordinate c2){return abs_diff(c1.x, c2.x) + abs_diff(c1.y, c2.y);}

int main(int argc,char* argv[]){
    int map_width, map_height, obstacle_count, bomber_count;
    vector<Obstacle> obstacles;
    vector<Bomber> bombers;
    vector<Bomb> bombs;
    vector<vector<int>> bomber_map, obstacle_map, bomb_map, obstacle_duration_map;
    vector<char**> thrash1; // !!!!!!!!!!!! Free this memory at the end
    vector<int> thrash1c;

    // 1) Read the input information about the game from standard input

    // Read first line
    cin >> map_width >> map_height >> obstacle_count >> bomber_count;

    // Create Maps for quick access
    for (int i=0;i<map_height;i++){
        vector<int> temp;
        for (int j=0;j<map_width;j++) temp.push_back(-1);
        bomber_map.push_back(temp);
        obstacle_map.push_back(temp);
        bomb_map.push_back(temp);
        vector<int> temp2;
        for (int j=0;j<map_width;j++) temp2.push_back(-2); // - 2 for no obstacle, -1 for non-destructable obstacle, 0,1,2,... for destructable obstacle
        obstacle_duration_map.push_back(temp2); 
    }

    // Read obstacle information
    for (int i=0;i<obstacle_count;i++){
        int x, y, durability;
        cin >> x >> y >> durability;
        obstacles.push_back(Obstacle(x, y, durability));
    }

    // Read bomber information
    for (int i=0;i<bomber_count;i++){
        int x, y, argc;
        cin >> x >> y >> argc;
        vector<string> argv;
        for (int j = 0; j<argc; j++){
            string arg;
            cin >> arg;
            argv.push_back(arg);
        }
        bombers.push_back(Bomber(x, y, argv));
    }


    // 2) Create pipes for bombers
    int** bomber_pipes = new int*[bomber_count];  // !!!!!!!!!!!! Free this memory at the end
    for (int i=0; i<bomber_count; i++) bomber_pipes[i] = new int[2];
    for (int i=0; i<bomber_count; i++) PIPE(bomber_pipes[i]);

    // Bomber write to pipes[i][0] and controller reads from pipes[i][1]
    // Controller write to pipes[i][1] and bomber reads from pipes[i][0] 

    
    // 3) Fork the bomber processes
    for (int i=0;i<bomber_count;i++){
        int argc = bombers[i].get_argc();
        char** argv = new char*[argc + 1];
        for (int j=0;j<argc;j++){
            argv[j] = new char[bombers[i].get_argv()[j].length() + 1]; // +1 is for \0
            strcpy(argv[j], bombers[i].get_argv()[j].c_str());
        }
        argv[argc] = NULL; // Now, argv can be passed to exec as an argument
        thrash1.push_back(argv);
        thrash1c.push_back(argc);
        
        pid_t pid = fork();
        bombers[i].setPid(pid);
        if (pid == 0){
            bombers[i].setPid(getpid());

            // 4) Redirect the bomber standard input and output to the pipe

            // Bomber write to pipes[i][0] and controller reads from pipes[i][1]
            // Controller write to pipes[i][1] and bomber reads from pipes[i][0] 
            close(bomber_pipes[i][1]); // unnecessary
            dup2(bomber_pipes[i][0], 0);
            dup2(bomber_pipes[i][0], 1);
            close(bomber_pipes[i][0]); // unnecessary

            // 5) Execute bomber executable with its arguments
            execv(argv[0], argv);
            cerr << "An error occured during execv function call for bomber!" << endl;
        }
    }

    // Map bombers and obstacles into maps
    for (int i=0;i<bomber_count;i++){
        // if 0 -> bomber not exist at that coordinate , otherwise value at that coordinate gives the pid of bomber which means bomber exists
        coordinate c = bombers[i].getCoordinate();
        int x = c.x;
        int y = c.y;
        bomber_map[y][x] = bombers[i].getPid();
    }

    for (int i=0;i<obstacle_count;i++){
        // -1: obstacle not exists , 1: obstacle exist
        coordinate c = obstacles[i].getCoordinate();
        int x = c.x;
        int y = c.y;
        obstacle_map[y][x] = 1;
        obstacle_duration_map[y][x] = obstacles[i].getDurability();
    }


    if (bomber_count == 1){
        im incoming_message_from_bomber;
        om outgoing_message_to_bomber;
        imp incoming_message_print_bomber;
        omp output_message_print_bomber;
        read(bomber_pipes[0][1], &incoming_message_from_bomber, sizeof(incoming_message_from_bomber));
        incoming_message_print_bomber.pid = bombers[0].getPid();
        incoming_message_print_bomber.m = &incoming_message_from_bomber;
        print_output(&incoming_message_print_bomber, NULL, NULL, NULL); 
        outgoing_message_to_bomber.type = BOMBER_WIN;
        output_message_print_bomber.m = &outgoing_message_to_bomber;
        output_message_print_bomber.pid = bombers[0].getPid();
        write(bomber_pipes[0][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));   
        print_output(NULL, &output_message_print_bomber, NULL, NULL);
        waitpid(bombers[0].getPid(), NULL, 0);
        return 0;
    }



    // Bomb Pipes
    vector<int*> bomb_pipes;

    // 6 Main Loop: while there are more than one bomber remaining;
    int current_bomber_count = bomber_count;
    bool winner_detected = false; // This will be helpful in case last n bomber die in a turn and furthest bomber will win

    bool this_bomb_causes_game_to_finish = false;
    while (!((winner_detected == true) && (current_bomber_count == 1))){
        // 6.1. Select or poll the bomb pipes to see there is any input
        // 6.2. Read and act according the message
        // 6.3. Remove any obstacles if their durability is zero and mark killed bombers
        // 6.4. Reap exploded bombs

        for (int i=0;i<bombs.size();i++){
            if (this_bomb_causes_game_to_finish == true){ // LAST MINUTE CHANGE
                if (bombs[i].isExploded() == true) continue;
                struct pollfd fds[1];
                int x;

                fds[0].fd = bomb_pipes[i][1];
                fds[0].events = POLLIN;

                x = poll(fds, 1, 0);
                
                if (x <= 0) continue;
                if (!(fds[0].revents & POLLIN)) continue;  

                im incoming_message_from_bomb;
                imp incoming_message_print_bomb;

                read(bomb_pipes[i][1], &incoming_message_from_bomb, sizeof(incoming_message_from_bomb));
                incoming_message_print_bomb.pid = bombs[i].getPid();
                incoming_message_print_bomb.m = &incoming_message_from_bomb;
                print_output(&incoming_message_print_bomb, NULL, NULL, NULL);
                
                if (incoming_message_from_bomb.type == BOMB_EXPLODE){
                    coordinate c = bombs[i].getCoordinate();
                    int x = c.x;
                    int y = c.y;
                    int radius = bombs[i].getRadius();

                    bombs[i].explode();
                    bomb_map[y][x] = -1;                

                    for (int k=y+1;k<=y+radius;k++){
                        if ((k < 0) || (k > (map_height - 1))) break;
                        if (obstacle_map[k][x] == 1){
                            if (obstacle_duration_map[k][x] == -1){
                                obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;
                            }
                            else {
                                obstacle_duration_map[k][x] -= 1;
                                if (obstacle_duration_map[k][x] == 0) obstacle_map[k][x] = -1;
                                obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;
                            } 
                        }
                    }                

                    for (int k=y-1;k>=y-radius;k--){
                        if ((k < 0) || (k > (map_height - 1))) break;
                        if (obstacle_map[k][x] == 1){
                            if (obstacle_duration_map[k][x] == -1){
                                obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                                print_output(NULL, NULL, &obstacle, NULL);                            
                                break;
                            }
                            else {
                                obstacle_duration_map[k][x] -= 1;
                                if (obstacle_duration_map[k][x] == 0) obstacle_map[k][x] = -1;
                                obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;                            
                            }
                        }
                    }

                    for (int k=x+1;k<=x+radius;k++){
                        if ((k < 0) || (k > (map_width - 1))) break;
                        if (obstacle_map[y][k] == 1){
                            if (obstacle_duration_map[y][k] == -1){
                                obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[y][k];
                                print_output(NULL, NULL, &obstacle, NULL);                            
                                break;
                            }
                            else {
                                obstacle_duration_map[y][k] -= 1;
                                if (obstacle_duration_map[y][k] == 0) obstacle_map[y][k] = -1;
                                obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[y][k];
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;
                            }
                        }
                    }                
                    
                    for (int k=x-1;k>=x-radius;k--){
                        if ((k < 0) || (k > (map_width - 1))) break;
                        if (obstacle_map[y][k] == 1){
                            if (obstacle_duration_map[y][k] == -1){
                                obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[y][k];
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;
                            }
                            else {
                                obstacle_duration_map[y][k] -= 1;
                                if (obstacle_duration_map[y][k] == 0) obstacle_map[y][k] = -1;
                                obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                                obstacle.remaining_durability = obstacle_duration_map[y][k];
                                print_output(NULL, NULL, &obstacle, NULL);
                                break;
                            }
                        }
                    }  
                }

                if (waitpid(bombs[i].getPid(), NULL, 0) == -1) cerr << "Bomb with pid " << bombs[i].getPid() << " could not be reaped!" << endl;

                continue;
            }
            if (bombs[i].isExploded() == true) continue;

            struct pollfd fds[1];
            int x;

            fds[0].fd = bomb_pipes[i][1];
            fds[0].events = POLLIN;

            x = poll(fds, 1, 0);
            
            if (x <= 0) continue;
            if (!(fds[0].revents & POLLIN)) continue;


            im incoming_message_from_bomb;
            imp incoming_message_print_bomb;

            read(bomb_pipes[i][1], &incoming_message_from_bomb, sizeof(incoming_message_from_bomb));
            incoming_message_print_bomb.pid = bombs[i].getPid();
            incoming_message_print_bomb.m = &incoming_message_from_bomb;
            print_output(&incoming_message_print_bomb, NULL, NULL, NULL); 

            if (incoming_message_from_bomb.type == BOMB_EXPLODE){
                coordinate c = bombs[i].getCoordinate();
                int x = c.x;
                int y = c.y;
                int radius = bombs[i].getRadius();

                bombs[i].explode();
                bomb_map[y][x] = -1;

                vector<pid_t> bombers_in_range;
                vector<int> distances;
                
                if (bomber_map[y][x] != -1) {bombers_in_range.push_back(bomber_map[y][x]); distances.push_back(0);}

                for (int k=y+1;k<=y+radius;k++){
                    if ((k < 0) || (k > (map_height - 1))) break;
                    if (obstacle_map[k][x] == 1){
                        if (obstacle_duration_map[k][x] == -1){
                            obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;
                        }
                        else {
                            obstacle_duration_map[k][x] -= 1;
                            if (obstacle_duration_map[k][x] == 0) obstacle_map[k][x] = -1;
                            obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;
                        } 
                    }
                    if (bomber_map[k][x] != -1){
                        bombers_in_range.push_back(bomber_map[k][x]);
                        distances.push_back(k - y);
                    }
                }

                for (int k=y-1;k>=y-radius;k--){
                    if ((k < 0) || (k > (map_height - 1))) break;
                    if (obstacle_map[k][x] == 1){
                        if (obstacle_duration_map[k][x] == -1){
                            obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                            print_output(NULL, NULL, &obstacle, NULL);                            
                            break;
                        }
                        else {
                            obstacle_duration_map[k][x] -= 1;
                            if (obstacle_duration_map[k][x] == 0) obstacle_map[k][x] = -1;
                            obsd obstacle; coordinate c; c.x = x; c.y = k; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[k][x]; 
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;                            
                        }
                    }
                    if (bomber_map[k][x] != -1){
                        bombers_in_range.push_back(bomber_map[k][x]);
                        distances.push_back(y - k);
                    }
                }


                for (int k=x+1;k<=x+radius;k++){
                    if ((k < 0) || (k > (map_width - 1))) break;
                    if (obstacle_map[y][k] == 1){
                        if (obstacle_duration_map[y][k] == -1){
                            obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[y][k];
                            print_output(NULL, NULL, &obstacle, NULL);                            
                            break;
                        }
                        else {
                            obstacle_duration_map[y][k] -= 1;
                            if (obstacle_duration_map[y][k] == 0) obstacle_map[y][k] = -1;
                            obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[y][k];
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;
                        }
                    }
                    if (bomber_map[y][k] != -1){
                        bombers_in_range.push_back(bomber_map[y][k]);
                        distances.push_back(k - x);
                    }
                }

                for (int k=x-1;k>=x-radius;k--){
                    if ((k < 0) || (k > (map_width - 1))) break;
                    if (obstacle_map[y][k] == 1){
                        if (obstacle_duration_map[y][k] == -1){
                            obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[y][k];
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;
                        }
                        else {
                            obstacle_duration_map[y][k] -= 1;
                            if (obstacle_duration_map[y][k] == 0) obstacle_map[y][k] = -1;
                            obsd obstacle; coordinate c; c.x = k; c.y = y; obstacle.position = c;
                            obstacle.remaining_durability = obstacle_duration_map[y][k];
                            print_output(NULL, NULL, &obstacle, NULL);
                            break;
                        }
                    }
                    if (bomber_map[y][k] != -1){
                        bombers_in_range.push_back(bomber_map[y][k]);
                        distances.push_back(x - k);
                    }
                }                

                vector<int> removed_already_markeds;

                for (int k=0;k<bombers_in_range.size();k++){
                    for (int m=0;m<bombers.size();m++){
                        if (bombers_in_range[k] == bombers[m].getPid()){
                            if (bombers[m].isMarkedForDie() == false){
                                removed_already_markeds.push_back(bombers[m].getPid());
                            }
                            break;
                        }
                    }
                }

                bombers_in_range = removed_already_markeds;


                if (bombers_in_range.size() == 0) continue;

                int n_alive_bombers = 0;
                for (int k=0;k<bombers.size();k++) if (bombers[k].isAlive() && (bombers[k].isMarkedForDie() == false)) n_alive_bombers++;

                if ((n_alive_bombers - bombers_in_range.size()) >= 2){
                    for (int k=0;k<bombers_in_range.size();k++){
                        for (int m=0;m<bombers.size();m++){
                            if (bombers_in_range[k] == bombers[m].getPid()){
                                bombers[m].markForDie();
                                break;
                            }
                        }
                    }
                }
                else if ((n_alive_bombers - bombers_in_range.size()) == 1){
                    for (int k=0;k<bombers_in_range.size();k++){
                        for (int m=0;m<bombers.size();m++){
                            if (bombers_in_range[k] == bombers[m].getPid()){
                                bombers[m].markForDie();
                                break;
                            }
                        }
                    }
                    for (int k=0;k<bombers.size();k++){
                        if (bombers[k].isAlive() && (bombers[k].isMarkedForDie() == false)){
                            bombers[k].markForWin();
                            break;
                        }
                    }
                    this_bomb_causes_game_to_finish = true;                    
                }
                else if ((n_alive_bombers - bombers_in_range.size()) == 0){
                    int d = distances[0];
                    int p = 0;
                    for (int k=0;k<distances.size();k++){
                        if (distances[k] > d){
                            d = distances[k];
                            p = k;
                        }
                    }
                    bombers[p].markForWin();
                    for (int k=0;k<bombers_in_range.size();k++){
                        for (int m=0;m<bombers.size();m++){
                            if (bombers_in_range[k] == bombers[m].getPid() && (bombers[m].isMarkedForWin() != false)){
                                bombers[m].markForDie();
                                break;
                            }
                        }                        
                    }
                    this_bomb_causes_game_to_finish = true;
                }

                // Reap exploded bombs
                if (waitpid(bombs[i].getPid(), NULL, 0) == -1) cerr << "Bomb with pid " << bombs[i].getPid() << " could not be reaped!" << endl;
                // else cerr << "Bomb with pid " << bombs[i].getPid() << " reaped ! D" << endl;
            }
        }

        // 6.5. Select or poll the bomber pipes to see if there is any input
        // 6.6. Read and act according to the message unless the bomber is marked or killed

        for (int i=0;i<bombers.size();i++){            
            if (bombers[i].isAlive() == false) continue;

            struct pollfd fds[1];
            int x;

            fds[0].fd = bomber_pipes[i][1];
            fds[0].events = POLLIN;

            x = poll(fds, 1, 0);
            
            if (x <= 0) continue;
            if (!(fds[0].revents & POLLIN)) continue;


            im incoming_message_from_bomber;
            om outgoing_message_to_bomber;
            imp incoming_message_print_bomber;
            omp output_message_print_bomber;
            read(bomber_pipes[i][1], &incoming_message_from_bomber, sizeof(incoming_message_from_bomber));
            incoming_message_print_bomber.pid = bombers[i].getPid();
            incoming_message_print_bomber.m = &incoming_message_from_bomber;
            print_output(&incoming_message_print_bomber, NULL, NULL, NULL);

            if (bombers[i].isMarkedForWin() == true){
                bombers[i].unmarkForWin();
                winner_detected = true;
                outgoing_message_to_bomber.type = BOMBER_WIN;
                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));  
                print_output(NULL, &output_message_print_bomber, NULL, NULL);                 
                if (waitpid(bombers[i].getPid(), NULL, 0) == -1) cerr << "Bomber with pid " << bombers[i].getPid() << " could not be reaped!" << endl;
                // else cerr << "Bomber reaped with id " << bombers[i].getPid() << " reaped ! A" << endl;
                continue;
            }

            if (bombers[i].isMarkedForDie() == true){
                bombers[i].unmarkForDie();
                bombers[i].die();
                current_bomber_count--;
                bomber_map[bombers[i].getCoordinate().y][bombers[i].getCoordinate().x] = -1;
                outgoing_message_to_bomber.type = BOMBER_DIE;
                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));  
                print_output(NULL, &output_message_print_bomber, NULL, NULL);
                if (waitpid(bombers[i].getPid(), NULL, 0) == -1) cerr << "Bomber with pid " << bombers[i].getPid() << " could not be reaped!" << endl;
                // else cerr << "Bomber reaped with id " << bombers[i].getPid() << " reaped ! C" << endl;
                continue;                
            }

            if (incoming_message_from_bomber.type == BOMBER_START){
                outgoing_message_to_bomber.type = BOMBER_LOCATION;
                outgoing_message_to_bomber.data.new_position = bombers[i].getCoordinate();
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));
                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                print_output(NULL, &output_message_print_bomber, NULL, NULL);
            }
            else if (incoming_message_from_bomber.type == BOMBER_MOVE){
                coordinate target_position = incoming_message_from_bomber.data.target_position;
                coordinate current_position = bombers[i].getCoordinate();
                coordinate result_position = target_position;

                if (target_position.x > (map_width - 1) || (target_position.y > (map_height - 1)) || (target_position.x < 0) || (target_position.y < 0)) result_position = current_position;
                else if (manhattan_distance(current_position, target_position) != 1) result_position = current_position;
                else if (!((current_position.x == target_position.x) || (current_position.y == target_position.y))) result_position = current_position;
                else if ((obstacle_map[target_position.y][target_position.x]) == 1) result_position = current_position;
                else if ((bomber_map[target_position.y][target_position.x]) != -1) result_position = current_position;

                bombers[i].setCoordinate(result_position);                

                outgoing_message_to_bomber.type = BOMBER_LOCATION;
                outgoing_message_to_bomber.data.new_position = result_position;
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));
                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                print_output(NULL, &output_message_print_bomber, NULL, NULL);

                bomber_map[current_position.y][current_position.x] = -1;
                bomber_map[result_position.y][result_position.x] = bombers[i].getPid();


            }
            else if (incoming_message_from_bomber.type == BOMBER_PLANT){
                if (bomb_map[bombers[i].getCoordinate().y][bombers[i].getCoordinate().x] != -1){ // A bomb alrady has been planted at this location
                    outgoing_message_to_bomber.type = BOMBER_PLANT_RESULT;
                    outgoing_message_to_bomber.data.planted = 0;
                    write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));
                    output_message_print_bomber.pid = bombers[i].getPid();
                    output_message_print_bomber.m = &outgoing_message_to_bomber;
                    print_output(NULL, &output_message_print_bomber, NULL, NULL);
                    continue;
                }
                
                long interval = incoming_message_from_bomber.data.bomb_info.interval;
                unsigned int radius = incoming_message_from_bomber.data.bomb_info.radius;
                string interval_string = to_string(interval);
                string radius_string = to_string(radius);

                char* interval_cstring = new char[interval_string.length() + 1];
                strcpy(interval_cstring, interval_string.c_str());


                char** argv = new char*[3];
                char* arg0 = new char[7];
                arg0[0] = '.'; arg0[1] = '/'; arg0[2] = 'b'; arg0[3] = 'o'; arg0[4] = 'm'; arg0[5] = 'b'; arg0[6] = '\0';

                argv[0] = arg0;
                argv[1] = interval_cstring;
                argv[2] = NULL;

                Bomb bomb = Bomb(bombers[i].getCoordinate().x, bombers[i].getCoordinate().y, radius, interval);

                int* fd  = new int[2];
                PIPE(fd);         

                pid_t bpid = fork();
                bomb.setPid(bpid);

                bomb_pipes.push_back(fd); 

                if (bpid == 0){
                    close(fd[1]); // unnecessary
                    dup2(fd[0], 0);
                    dup2(fd[0], 1);
                    close(fd[0]); // unnecessary

                    execv(argv[0], argv);      

                    cerr << "An error occured during execv function call for bomb!" << endl;
                }
                
                bombs.push_back(bomb);
                bomb_map[bombers[i].getCoordinate().y][bombers[i].getCoordinate().x];

                outgoing_message_to_bomber.type = BOMBER_PLANT_RESULT;
                outgoing_message_to_bomber.data.planted = 1;
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));
                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                print_output(NULL, &output_message_print_bomber, NULL, NULL);

                bomb_map[bombers[i].getCoordinate().y][bombers[i].getCoordinate().x] = bpid;

                if (interval_cstring != NULL) delete[] interval_cstring;
                if (arg0 != NULL) delete[] arg0;
                if (argv != NULL) delete[] argv; 
            }
            else if (incoming_message_from_bomber.type == BOMBER_SEE){
                coordinate c = bombers[i].getCoordinate();
                int x = c.x;
                int y = c.y;
                int b = 0;

                vector<coordinate> obstacles_in_vision;
                vector<coordinate> bombs_in_vision;
                vector<coordinate> bombers_in_vision;

                if (bomb_map[y][x] != -1){  // You forgot too check current location for vision. Added later!
                    coordinate c;
                    c.x = x;
                    c.y = y;
                    bombs_in_vision.push_back(c);
                }

                // +y obstacle check
                bool plus_y_obs_check = false;
                for (int i=y+1;i<=y+3;i++){
                    if ((i < 0) || (i > (map_height-1))) break;
                    if (obstacle_map[i][x] == 1){
                        b = i - y;
                        coordinate c;
                        c.x = x;
                        c.y = i;
                        obstacles_in_vision.push_back(c);
                        plus_y_obs_check = true;
                        break;
                    }
                }
                if (plus_y_obs_check){
                    for (int i=y+1;i<=y+b-1;i++){
                        if ((i < 0) || (i > (map_height-1))) break;
                        if (bomber_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombs_in_vision.push_back(c);
                        }
                    }                    
                }
                else{
                    for (int i=y+1;i<=y+3;i++){
                        if ((i < 0) || (i > (map_height-1))) break;
                        if (bomber_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombs_in_vision.push_back(c);                    
                        }
                    }
                }



                // -y obstacle check
                bool minus_y_obs_check = false;
                for (int i=y-1;i>=y-3;i--){
                    if ((i < 0) || (i > (map_height-1))) break;
                    if (obstacle_map[i][x] == 1){
                        b = y - i;
                        coordinate c;
                        c.x = x;
                        c.y = i;
                        obstacles_in_vision.push_back(c);
                        minus_y_obs_check = true;
                        break;                        
                    }
                }
                if (minus_y_obs_check){
                    for (int i=y-1;i>=y-b+1;i--){
                        if ((i < 0) || (i > (map_height-1))) break;
                        if (bomber_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombs_in_vision.push_back(c);
                        }
                    }                   
                }
                else{
                    for (int i=y-1;i>=y-3;i--){
                        if ((i < 0) || (i > (map_height-1))) break;
                        if (bomber_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[i][x] != -1){
                            coordinate c;
                            c.x = x;
                            c.y = i;
                            bombs_in_vision.push_back(c);
                        }
                    } 
                }


                
                // +x obstacle check
                bool plus_x_obs_check = false;
                for (int i=x+1;i<=x+3;i++){
                    if ((i < 0) || (i > (map_width-1))) break;
                    if (obstacle_map[y][i] == 1){
                        b = i - x;
                        coordinate c;
                        c.x = i;
                        c.y = y;
                        obstacles_in_vision.push_back(c);
                        plus_x_obs_check = true;
                        break;
                    }
                }

                if (plus_x_obs_check){
                    for (int i=x+1;i<=x+b-1;i++){
                        if ((i < 0) || (i > (map_width-1))) break;
                        if (bomber_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombs_in_vision.push_back(c);
                        }                        
                    }
                }
                else{
                    for (int i=x+1;i<=x+3;i++){
                        if ((i < 0) || (i > (map_width-1))) break;
                        if (bomber_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombs_in_vision.push_back(c);
                        }                        
                    }
                }



                // -x obstacle check
                bool minus_x_obs_check = false;
                for (int i=x-1;i>=x-3;i--){
                    if ((i < 0) || (i > (map_width-1))) break;
                    if (obstacle_map[y][i] == 1){
                        b = x - i;
                        coordinate c;
                        c.x = i;
                        c.y = y;
                        obstacles_in_vision.push_back(c);
                        minus_x_obs_check = true;
                        break;
                    }
                }
                if (minus_x_obs_check){
                    for (int i=x-1;i>=x-b+1;i--){
                        if ((i < 0) || (i > (map_width-1))) break;
                        if (bomber_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombs_in_vision.push_back(c);
                        }                        
                    }
                }
                else {
                    for (int i=x-1;i>=x-3;i--){
                        if ((i < 0) || (i > (map_width-1))) break;
                        if (bomber_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombers_in_vision.push_back(c);
                        }
                        if (bomb_map[y][i] != -1){
                            coordinate c;
                            c.x = i;
                            c.y = y;
                            bombs_in_vision.push_back(c);
                        }                        
                    }
                }

                unsigned int object_count = obstacles_in_vision.size() + bombers_in_vision.size() + bombs_in_vision.size();
                outgoing_message_to_bomber.type = BOMBER_VISION;
                outgoing_message_to_bomber.data.object_count = object_count;
                write(bomber_pipes[i][1], &outgoing_message_to_bomber, sizeof(outgoing_message_to_bomber));

                for (int k=0;k<obstacles_in_vision.size();k++){
                    object_data obj_d;
                    obj_d.position = obstacles_in_vision[k];
                    obj_d.type = OBSTACLE;
                    write(bomber_pipes[i][1], &obj_d, sizeof(obj_d));               
                }

                for (int k=0;k<bombers_in_vision.size();k++){
                    object_data obj_d;
                    obj_d.position = bombers_in_vision[k];
                    obj_d.type = BOMBER;
                    write(bomber_pipes[i][1], &obj_d, sizeof(obj_d));               
                }



                for (int k=0;k<bombs_in_vision.size();k++){
                    object_data obj_d;
                    obj_d.position = bombs_in_vision[k];
                    obj_d.type = BOMB;
                    write(bomber_pipes[i][1], &obj_d, sizeof(obj_d));               
                }


                output_message_print_bomber.pid = bombers[i].getPid();
                output_message_print_bomber.m = &outgoing_message_to_bomber;
                if (object_count == 0){
                    print_output(NULL, &output_message_print_bomber, NULL, NULL);
                }
                else {
                    od* objects = new od[object_count];
                    int i = 0;
                    for (int j=0;j<bombers_in_vision.size();j++){
                        objects[i].position = bombers_in_vision[j];
                        objects[i].type = BOMBER;
                        i++;
                    }
                    for (int j=0;j<bombs_in_vision.size();j++){
                        objects[i].position = bombs_in_vision[j];
                        objects[i].type = BOMB;
                        i++;
                    }
                    for (int j=0;j<obstacles_in_vision.size();j++){
                        objects[i].position = obstacles_in_vision[j];
                        objects[i].type = OBSTACLE;
                        i++;
                    }
                    print_output(NULL, &output_message_print_bomber, NULL, objects);

                    delete[] objects;
                }
            }
        }

        // 6.7. Inform marked bombers
        // 6.8. Reap informed killed bombers


        // 6.9. Sleep for 1 milisecond
        usleep(1000);
    }

    // WAIT FOR REMAINING BOMBS TO EXPLODE AND REAP THEM
    int w;
    while ((w = wait(NULL)) != -1){
        im incoming_message;
        imp incoming_message_print;
        incoming_message.type = BOMB_EXPLODE;
        incoming_message_print.pid = w;
        incoming_message_print.m = &incoming_message;
        print_output(&incoming_message_print, NULL, NULL, NULL); 
        // cerr << "Bomb reaped with id " << w << " reaped B!" << endl;
    };

    // Memory deallocation
    for (int i=0;i<bomber_count;i++) delete[] bomber_pipes[i];
    for (int i=0;i<bomb_pipes.size();i++) delete[] bomb_pipes[i];
    //delete[] thrash1[0][0]; delete[] thrash1[0][1];  delete[] thrash1[0][2]; delete[] thrash1[0][3];
    //delete[] thrash1[1][0]; delete[] thrash1[1][1];  delete[] thrash1[1][2]; delete[] thrash1[1][3];
    for (int i=0;i<thrash1.size();i++){
        for (int j=0;j<thrash1c[i];j++){
            if (thrash1[i][j] != NULL) delete[] thrash1[i][j];
        }
    }
    for (int i=0;i<thrash1.size();i++) if (thrash1[i] != NULL) delete[] thrash1[i];
    if (bomber_pipes != NULL) delete[] bomber_pipes;
    return 0;
}


     
