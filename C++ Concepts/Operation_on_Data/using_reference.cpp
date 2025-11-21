// Demonstration of reference variables in C++
#include<iostream>
#include<iomanip>
using namespace std;

int main()
{
    int value1 = 456;
    double value2 = 4567.545;
    int & ref_value1{value1};
    int & ref_value2 = value1;
    double & ref_value3{value2};
    cout<<left;
    cout<<setw(40)<<"value of ref_value1"<<ref_value1 <<endl;    
    cout<<setw(40)<<"value of ref_value2"<<ref_value2 <<endl;    
    cout<<setw(40)<<"value of ref_value3"<<ref_value3 <<endl;    
    cout<<setw(40)<<"&value1"<<&value1 <<endl;    
    cout<<setw(40)<<"&value2"<<&value2 <<endl;
    cout<<setw(40)<<"&ref_value1"<<&ref_value1 <<endl;
    cout<<setw(40)<<"&ref_value2"<<&ref_value2 <<endl;
    cout<<setw(40)<<"&ref_value3"<<&ref_value3 <<endl;
    cout<<setw(40)<<"size of(int)"<<sizeof(int) <<endl;
    cout<<setw(40)<<"size of(&int)"<<sizeof(int&) <<endl;
    cout<<setw(40)<<"size of(ref_value1)"<<&ref_value1 <<endl;
    return 0;
 }
