// Demonstration of null pointer safety in C++
#include<iostream>
#include<iomanip>
using namespace std;

int main(){
    int b=10;
    cout<<left;
    //null pointer     
    int *ptr{};
    if(ptr!= nullptr)
    {
        cout<<setw(40)<<" pointer points to a valid address "<<": "<<ptr<<endl;
        cout<<setw(40)<<" value  "<<": "<<*ptr<<endl;
    }
    else{
        cout<<" points to invalid address"<<endl;
    }
    
    delete ptr;
}