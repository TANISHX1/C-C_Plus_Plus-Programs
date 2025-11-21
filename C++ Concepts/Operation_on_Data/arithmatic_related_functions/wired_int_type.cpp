// Demonstration of arithmetic operations on short int and char types in C++
#include<iostream>
using namespace std;

int main()
{
    short int v1{10};
    short int v2{20};

    char c1{40};
    char c2{50};

auto result1 = v1+v2;
auto result2 = c1+c2;

cout<<"Size of v1  "<<sizeof(v1)<<"  Bytes\n"; 
cout<<"Size of v2  "<<sizeof(v2)<<"  Bytes\n";     
cout<<"Size of c1  "<<sizeof(c1)<<"  Bytes\n";
cout<<"Size of c2  "<<sizeof(c2)<<"  Bytes\n";
cout<<"Size of Result1  "<<sizeof(result1)<<"  Bytes\n";
cout<<"Size of Result2  "<<sizeof(result2)<<"  Bytes\n";
return 0;
}