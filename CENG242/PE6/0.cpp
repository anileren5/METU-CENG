#include <iostream>

class A{
    public:
        int x;
};

class B:public A{
    public:
        int y;
};

void f(A par){
    std::cout << par.x;
}

int main(){
    A k;
    k.x = 5;
    f(k);
}