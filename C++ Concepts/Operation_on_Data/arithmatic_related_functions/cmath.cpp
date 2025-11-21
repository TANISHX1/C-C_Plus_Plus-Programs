// Demonstration of arithmetic related functions from cmath library
#include<iostream>
#include<cmath>
using namespace std;

int main (){
    int a,b;
    float c,pi=3.14;
    cout<<"Enter the values of A,B,C\t";
    cin>>a>>b>>c;
    cout<<"value of a:"<<a<<endl;
    cout<<"Exponetial form of a : "<<exp(a)<<endl;
    cout<<"power  A to the B: "<<pow(a,b)<<endl;
    cout<<"Square root of A: "<<sqrt(a)<<endl;
    cout<<"Tantheta of 45degree: "<<tan(pi/4)<<endl;
    cout<<"Tantheta of 45degree: "<<sin(pi/4)<<endl;
    cout<<"Tantheta of 45degree: "<<cos(pi/4)<<endl;
    cout<<"Log of A(have base 2):"<<log2(4)<<endl;
    printf("Roundoff Value of %.4f is : ",c);cout<<round(c)<<endl;
    
}