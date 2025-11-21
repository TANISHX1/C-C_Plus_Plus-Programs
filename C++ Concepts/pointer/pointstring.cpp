// Demonstration of pointer to string literal in C++
#include<iostream>
using namespace std;
int main(){
    //char *message {...} is not a correct way of using .it will give compiler error in some compilers
    //because char *message{...} is a type of char array that can be modifiable .
    const char *message{"HEllo world"};
    cout<<"Message: "<<message<<endl;
    cout<<"*Message: "<<*message<<endl;//(char*message{..} points to the first char of *message)
    
    return 0;
}