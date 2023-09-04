#include "the5.h"

/* 
    in the5.h "struct Room" is defined as below:
    
    struct Room {
        int id;
        char content;
        vector<Room*> neighbors;
    };

*/
struct Node{
    struct Room* room;
    bool visited = false;
};

bool helperVisited(vector<Node*>& MyMaze, int s_id){
    for (int i=0;i<MyMaze.size();i++) if (MyMaze[i]->room->id == s_id) return MyMaze[i]->visited;
    return false;
}

void search(Node* current_node, vector<int>& path, stack<Node*>& path_stack, vector<Node*>& MyMaze, bool& found){
    if (found) return;
    current_node->visited = true;
    path.push_back(current_node->room->id);
    if (current_node->room->content == '*') {found = true; return;}
    else{
        bool is_there_a_way_to_go = false;
        for (int i = 0;i<current_node->room->neighbors.size();i++) if (helperVisited(MyMaze, current_node->room->neighbors[i]->id) == false) {is_there_a_way_to_go = true; break;}
        if (is_there_a_way_to_go){
            path_stack.push(current_node);
            for (int i = 0;i<current_node->room->neighbors.size();i++){
                if (helperVisited(MyMaze, current_node->room->neighbors[i]->id) == false && current_node->room->neighbors[i]->content != '*'){
                    for (int j=0;j<MyMaze.size();j++){
                        if (MyMaze[j]->room->id == current_node->room->neighbors[i]->id){
                            search(MyMaze[j], path, path_stack, MyMaze, found);
                            break;
                        }
                    }
                }
                else if (helperVisited(MyMaze, current_node->room->neighbors[i]->id) == false && current_node->room->neighbors[i]->content == '*'){
                    for (int j=0;j<MyMaze.size();j++){
                        if (MyMaze[j]->room->id == current_node->room->neighbors[i]->id){
                            search(MyMaze[j], path, path_stack, MyMaze, found);
                            return ;
                        }
                    }
                    return ;
                }
            }
        }
        else{
            if (path_stack.empty() == false){
                Node* backtrack_node = path_stack.top();
                path_stack.pop();
                search(backtrack_node, path, path_stack, MyMaze, found);
            }
        }
    }
}

vector<int> maze_trace(vector<Room*> maze){ 
    vector<int> path;
    vector<Node*> MyMaze;
    for (int i=0;i<maze.size();i++){
        Node* new_node = new Node();
        new_node->room = maze[i];
        new_node->visited = false;
        MyMaze.push_back(new_node);
    }
    stack<Node*> path_stack;
    bool found = false;
    search(MyMaze[0], path, path_stack, MyMaze, found);
    while(!path_stack.empty()){path.push_back(path_stack.top()->room->id); path_stack.pop();}
    return path;
}