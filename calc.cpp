#include "calc.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std; 
int main(){
    Calculator calc;
    string in;
    calc.Display();
    cout << "Input what you want to calculate" << endl;
    getline(cin,in);
    while(!calc.validateInput()){
        cout << "Invalid input, input again" << endl;
        getline(cin,in);
    }
    calc.setInput(in);
    calc.parseInput();
    Operation opr = calc.Result();
    cout << "And the result issss: ";
    cout << opr.get() << endl;
}
