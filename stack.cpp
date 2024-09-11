#define MAX_STACK_SIZE 100000
#include <iostream>

template <typename T>
class Stack {
private:
    T list[MAX_STACK_SIZE];
    int top;

public:
    Stack() : top(-1) {}

    bool isFull() const {
        return top >= MAX_STACK_SIZE - 1;
    }

    bool isempty() const {
        return top ==  - 1;
    }

    void push ( T value) {
        if( isFull() ) {
            std::cout<< "stack lleno" << std::endl;
        }
        else{
            top++;
            list[top] = value;
        }
    }

    T tops (){
        if (isempty()) {
            std::cout << "stack  vacio" << std::endl;
            return T();
        }
        else
        {
            return list[top];
        }
    }

    T pop (){
        if (isempty()) {
            std::cout << "stack  vacio" << std::endl;
            return T();
        }
        else{
            return list[top--];
        }
    }

};