// Demonstration of numeric limits for various data types in C++
#include<iostream>
#include <limits>
#include <iomanip>
using namespace std;
// for more info visit cppreference.com 
int main(){
    cout<<"For signed Range:\n"; cout<<left;
    cout<<setw(40)<<"The range for short is from "<<setw(15)<<numeric_limits<short>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<short>::max()<<endl;
    cout<<setw(40)<<"The range for int is from "<<setw(15)<<numeric_limits<int>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<int>::max()<<endl;
    cout<<setw(40)<<"The range for float is from "<<setw(15)<<numeric_limits<float>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<float>::max()<<endl;
    cout<<setw(40)<<"The range for double is from "<<setw(15)<<numeric_limits<double>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<double>::max()<<endl;
    cout<<setw(40)<<"The range for long double is from "<<setw(15)<<numeric_limits<long double>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<long double>::max()<<endl;
    cout<<"----------------------------------------------------------------------------\n";
    cout<<"For Unsigned Range:\n"; 
    cout<<setw(40)<<"The range for short is from "<<setw(15)<<numeric_limits<unsigned short>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<unsigned short>::max()<<endl;
    cout<<setw(40)<<"The range for int is from "<<setw(15)<<numeric_limits<unsigned int>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<unsigned int>::max()<<endl;
    cout<<setw(40)<<"The range for float is from "<<setw(15)<<numeric_limits< float>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<float>::max()<<endl;
    cout<<setw(40)<<"The range for double is from "<<setw(15)<<numeric_limits< double>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<double>::max()<<endl;
    cout<<setw(40)<<"The range for long double is from "<<setw(15)<<numeric_limits< long double>::min()<<setw(10)<<" To "<<setw(15)<<numeric_limits<long double>::max()<<endl;

}