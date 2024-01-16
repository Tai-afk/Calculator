#include <iostream>
#include <stack>
#include <math.h>
#include <string>


//The more you increase this number the more precise the calculation is
//Default: 4
const int PRECISION = 4;

//TODO: add decimal functionality
class Operation{
    private:
    double num;
    //Helper functions
    int pow(int base, int power){
        int x = 1;
        if(power == 0) return x;
        for(int i = 0; i < power; i++){
            x *= base;
        }
        return x;
    }
    double abs(double num){
        if(num < 0) return -1 * num;
        else return num;
    }
    void splitAt(double number, int position, double* high, double* low){
        *high = number / (pow(10, position));
        *low = fmod(number, (pow(10, position)));
    }
    int numberOfDigits(int num){
        return floor(log10(num)) + 1;
    }
    double multiply(double x, double y){//Karatsuba algorithm
        if(x < 10 || y < 10){
            return x * y;
        }
        int m = std::max(numberOfDigits(x), numberOfDigits(y));
        int m2 = m / 2;
        double high1 = 0.0, low1 = 0.0, high2, low2;
        splitAt(x, m2, &high1, &low1);
        splitAt(y, m2, &high2, &low2);

        //Recursively compute 3 products
        double z0 = multiply(low1, low2);
        double z1 = multiply((low1 + high1), (low2 + high2));
        double z2 = multiply(high1, high2);

        //Return 3 products
        return (z2 * pow(10, 2 * m2)) + ((z1 - z2 - z0) * pow(10, m2)) + z0;
    }
    
    public:
    //Getter and setter
    double get(){
        return num;
    }
    void set(double n){
        num = n;
    }

    Operation(): num(0){}
    Operation(double x_) : num(x_) {}
    
    Operation operator+(const Operation& obj) {
        Operation opr;
        double add = 0.0;
        int carry = 0, i = 0;
        int x = static_cast<int>(this->num);
        int y = static_cast<int>(obj.num);
        double x_d = this->num - x;
        double y_d = obj.num - y;

        if(x_d + y_d >= 1){
            carry = 1;
            add += (x_d + y_d) - 1;
        }
        else{
            add += (x_d + y_d);
        }
        if(x < 0 || y < 0){
            opr = this->operator-(obj);
            return opr;
        }

        //Integer addition
        while(x > 0 && y > 0){
            int addDig = (x % 10) + (y % 10) + carry;
            if(addDig > 10){
                carry = true;
                addDig -= 10;
            }
            else carry = false;
            add += pow(10, i) * addDig; 
            i++;
            x/=10;
            y/=10;
        }
        while(x > 0){
            add += pow(10,i) * ((x % 10) + carry);
            x /= 10;
            i++;
        }
        while(y > 0){
            add += pow(10,i) * ((y % 10) + carry);
            y /= 10;
            i++;
        }
        opr.num = add;
        return opr;
    }
    Operation operator-(const Operation& obj) {
        Operation opr;
        double res = 0.0;
        int minus = 1, subCarry = 0, i = 0;
        //x will always be the greater number to subtract digits
        int x = static_cast<int>(this->num); 
        int y = static_cast<int>(obj.num);
        double x_d = this->num - x;
        double y_d = obj.num - y; 
        
        //Base case, no subtraction required
        if(x == 0 && x_d == 0){
            opr.set(y);
            return opr;
        }
        else if(y == 0 && y_d == 0){
            opr.set(x);
            return opr;
        }

        if(x < 0 || y < 0){
            if((abs(x) > abs(y) || abs(x_d) > abs(y_d)) && x < 0){
                minus = -1;
            }
            else if((abs(y) > abs(x) || abs(y_d) > abs(x_d)) && y < 0){
                minus = -1;
            }
            //If both numbers negative, add them together and multiply by -1
            if(x < 0 && y < 0){
                Operation a(-y);
                opr.set(-x);
                opr = opr.operator+(a);
                opr.set(-1 * opr.get());
                return opr;
                minus = -1;
            }
            x = abs(x);
            y = abs(y);
        }
        if(y > x){
            int temp = y;
            double temp_d = y_d;
            y = x;
            x = temp;
            y_d = x_d;
            x_d = temp_d;
        }
        if(abs(x_d) < abs(y_d)){
            subCarry = -1;
            res += (1 + abs(x_d) - abs(y_d));
        }else{
            res += (abs(x_d) - abs(y_d));
        }
        while(x > 0 && y > 0){
            int addDig = 0;
            int n_digit = (x % 10) + subCarry;
            int k_digit = y % 10;
            
            if(n_digit < k_digit){
                subCarry = -1;
                addDig += (n_digit + 10) - k_digit;
            }
            else {
                subCarry = 0;
                addDig += n_digit - k_digit;
            }
            res += pow(10, i) * addDig;
            i++;
            x /= 10;
            y /= 10;
        }
        while(x > 0){
            int addDig = 0;
            int n_digit = (x % 10) + subCarry;
            if(n_digit < 0){
                addDig += (n_digit + 10);
                subCarry = -1;
            }else {
                addDig += n_digit;
                subCarry = 0;
            }
            res += pow(10,i) * addDig;
            i++;
            x /= 10;
        }
        res *= minus;
        opr.num = res;
        return opr;
    }  
    Operation operator/(const Operation& obj) {//Newton-raphson algorithm for division
        Operation opr;
        double x = this->num, y = obj.num;
        double reciprocal_estimate = 1 / y;
        int max_iterations = 1000;

        for(int i = 0; i < max_iterations; i++){
            reciprocal_estimate = reciprocal_estimate * (2 - y * reciprocal_estimate);
        }
        double res = x * reciprocal_estimate;
        opr.num = res;
        return opr;
    }
    Operation operator*(const Operation& obj) {
        Operation opr;
        opr.num = multiply(this->num, obj.num);
        return opr;
    }

};

class Calculator{
    private:
    std::stack<Operation> operations;
    std::string input;
    public:
    void setInput(std::string in){
        input = in;
    }
    bool validateInput(){
        for(unsigned int i = 0; i < input.size(); i++){
            if(!std::isdigit(input[i])){
                if(input[i] != '+' || input[i] != '-'
                || input[i] != '*' || input[i] != '/'
                || input[i] != '.' || input[i] != ' '
                || input[i] != '='){
                    return false;
                }
            }
        }
        return true;
    }
    void parseInput(){
        char sign = '+';
        Operation opr;
        std::string numStr = "";
        double num = 0.0;
        for(unsigned int i = 0; i < input.size(); i++){
            char c = input[i];
            if(std::isspace(c)) continue;
            if(std::isdigit(c) || c == '.'){
                //Multiply 10 to each digit
                /*
                num = 10 * num + (c - '0');
                opr.set(num);
                */
                numStr += input[i];
            }
            //TODO: fix case -1.1+0.9
            if ((!std::isdigit(c) || i == input.size() - 1) && c != '.') { 
                if(!numStr.empty()){
                    numStr = numStr.substr(0, numStr.find('.') + PRECISION);
                    num += std::stod(numStr);
                    opr.set(num);
                }
                
                switch (sign) {
                    case '+': {
                        operations.push(opr);
                        break;
                    }
                    case '-': {
                        opr.set(-num);
                        operations.push(opr);
                        break;
                    }
                    case '*': {
                        Operation pre;
                        pre.set(operations.top().get());
                        operations.pop();
                        operations.push(opr * pre);
                        break;
                    }
                    case '/': {
                        Operation pre;
                        pre.set(operations.top().get());
                        operations.pop();
                        operations.push(opr / pre);
                        break;
                    }
                }
                sign = c;
                numStr = "";
                num = 0.0;
            }
        }
    }
    Operation Result(){
        Operation res(0);
        while(!operations.empty()){
            res = res + operations.top();
            operations.pop();
        }
        return res;
    }
    void Display(){
    std::cout << "  _____________________\n";
    std::cout << " |  _________________  |\n";
    std::cout << " | |                 | |\n";
    if(!input.empty()){
        std::cout << " | |   " + input + "| |\n";
    }
    std::cout << " | |                 | |\n";
    std::cout << " | |                 | |\n";
    std::cout << " | |                 | |\n";
    std::cout << " | |                 | |\n";
    std::cout << " | |_________________| |\n";
    std::cout << " |  ___ ___ ___   ___  |\n";
    std::cout << " | | 7 | 8 | 9 | | + | |\n";
    std::cout << " | |___|___|___| |___| |\n";
    std::cout << " | | 4 | 5 | 6 | | - | |\n";
    std::cout << " | |___|___|___| |___| |\n";
    std::cout << " | | 1 | 2 | 3 | | x | |\n";
    std::cout << " | |___|___|___| |___| |\n";
    std::cout << " | | . | 0 | = | | / | |\n";
    std::cout << " | |___|___|___| |___| |\n";
    std::cout << " |_____________________|\n";
    }
    void Clear(){
        
    }
};