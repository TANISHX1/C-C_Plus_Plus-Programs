// Demonstration of signed and unsigned integer types in C++
#include <iostream>
using namespace std;
int main()
{
    int a{1000};
  unsigned short int b{334};
 signed long c(110000000);
  cout<<sizeof(a)<<" "<<a<<" "<<
sizeof(b)<<" "<<b<<" "<<sizeof(c)<<" "<<c;
}