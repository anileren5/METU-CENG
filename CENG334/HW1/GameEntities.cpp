#include "GameEntities.hpp"


coordinate Obstacle::getCoordinate(){
    coordinate Location;
    Location.x = this->x;
    Location.y = this->y;
    return Location;
}
int Obstacle::getDurability(){return this->durability;}
bool Obstacle::isDestructable(){return this->destructability;}
bool Obstacle::isDestructed(){return this->destructed;}
Obstacle::~Obstacle(){}
Obstacle::Obstacle(int argX, int argY, int argDurability){
    this->x = argX;
    this->y = argY;
    this->durability = argDurability;
    this->destructability = (durability == (-1)) ? false : true;
    this->destructed = false;
}
void Obstacle::damage(){
    if (this->isDestructable() == false) return ;
    else {
        (this->durability) -= 1;
        if ((this->durability) == 0) this->destructed = true;
    }
}






coordinate Bomber::getCoordinate(){
    coordinate Location;
    Location.x = this->x;
    Location.y = this->y;
    return Location;
}
string Bomber::getExecutablePath(){return this->argv[0];}
int Bomber::get_argc(){return this->argv.size();}
vector<string> Bomber::get_argv(){return this->argv;}
bool Bomber::isAlive(){return alive;}
void Bomber::die(){this->alive = false;}
Bomber::~Bomber(){}
Bomber::Bomber(int argX, int argY, vector<string> _argv){
    this->x = argX;
    this->y = argY;
    this->argv = _argv;
    this->alive = true;
    this->pid = -1;
    this->markedForDie = false;
    this->markedForWin = false;
}
pid_t Bomber::getPid(){return this->pid;}
void Bomber::setPid(pid_t argpid){this->pid = argpid;}
bool Bomber::isMarkedForDie(){return this->markedForDie;}
bool Bomber::isMarkedForWin(){return this->markedForWin;}
void Bomber::markForDie(){this->markedForDie = true;}
void Bomber::unmarkForDie(){this->markedForDie = false;}
void Bomber::setCoordinate(coordinate c){this->x = c.x; this->y = c.y;}
void Bomber::markForWin(){this->markedForWin = true;}
void Bomber::unmarkForWin(){this->markedForWin = false;}




coordinate Bomb::getCoordinate(){
    coordinate Location;
    Location.x = this->x;
    Location.y = this->y;
    return Location;
}
Bomb::Bomb(int argX, int argY, int argRadius, int argDuration){
    this->exploded = false;
    this->pid = -1;
    this->x = argX;
    this->y = argY;
    this->radius = argRadius;
    this->duration = argDuration;
}
Bomb::~Bomb(){}
pid_t Bomb::getPid(){return this->pid;}
void Bomb::setPid(pid_t argpid){this->pid = argpid;}
bool Bomb::isExploded(){return this->exploded;}
void Bomb::explode(){this->exploded = true;}
int Bomb::getRadius(){return this->radius;}



