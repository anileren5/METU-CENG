#ifndef GAME_ENTITIES_HPP
#define GAME_ENTITIES_HPP

#include <vector>
#include <string>

#include "message.h"

using namespace std;

class Obstacle{
    private:
        int x;
        int y;
        int durability;
        bool destructability;
        bool destructed;
    
    public:
        Obstacle(int argX, int argY, int argDurability);
        ~Obstacle();
        coordinate getCoordinate();
        int getDurability();
        bool isDestructable();
        bool isDestructed();
        void damage();
};


class Bomber{
    private:
        int x;
        int y;        
        vector<string> argv;
        bool alive;
        pid_t pid;    
        bool markedForDie;
        bool markedForWin;


    public:
        Bomber(int argX, int argY, vector<string> _argv);
        ~Bomber();
        string getExecutablePath();
        int get_argc();
        vector<string> get_argv();
        bool isAlive();
        void die();
        coordinate getCoordinate();
        pid_t getPid();
        void setPid(pid_t argpid);
        void setCoordinate(coordinate c);
        bool isMarkedForDie();
        bool isMarkedForWin();
        void markForDie();
        void unmarkForDie();
        void markForWin();
        void unmarkForWin();
};

class Bomb{
    private:
        int x;
        int y;
        int radius;
        int duration;
        pid_t pid;
        bool exploded;
    
    public:
        Bomb(int argX, int argY, int argRadius, int argDuration);
        ~Bomb();
        coordinate getCoordinate();
        pid_t getPid();
        void setPid(pid_t argpid);
        bool isExploded();
        void explode();
        int getRadius();
};


#endif