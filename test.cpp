#include <iostream>
#include "calc.h"

#define IS_TRUE(x) {if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;}

Operation x;
bool function1(Operation x, Operation y){
    return 10 == (x + y).get();
}
void test_add(){
    //IS_TRUE(!function1(Operation x(5), Operation y(5)));
}