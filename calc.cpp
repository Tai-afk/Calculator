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
    cout << "Type \"EXIT\" to exit the program" << endl;
    getline(cin,in);
    while(in != "EXIT"){
        while(!calc.validateInput()){
            cout << "Invalid input, input again" << endl;
            getline(cin,in);
        }
        calc.setInput(in);
        calc.parseInput();
        Operation opr = calc.Result();
        cout << "And the result is: ";
        cout << opr.get() << endl << endl;
        cout << "Input what you want to calculate" << endl;
        cout << "Type \"EXIT\" to exit the program" << endl;
        getline(cin, in);
    }
}
