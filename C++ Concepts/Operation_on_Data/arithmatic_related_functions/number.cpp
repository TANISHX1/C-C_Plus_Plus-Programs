// Demonstration of different number representations in C++
#include<iostream>
using namespace std;
int main()
{
    int n1 = 15;
    int n2 = 017;
    int n3 = 0x0f;
    int n4 = 0b00001111;
    cout <<n1<<n2<<n3<<n4;
    return 0;
}