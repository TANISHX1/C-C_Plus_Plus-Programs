// Demonstration of precision and special floating-point values in C++
#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    float num1{1.1234567890123456789f};
    double num2{1.12345678901234567890};
    long double num3{1.12345678901234567890L};
    cout<<"size of num1 : "<<sizeof(num1)<<endl;
    cout<<"size of num2 : "<<sizeof(num2)<<endl;
    cout<<"size of num3 : "<<sizeof(num3)<<endl;
    //precision
    cout<<setprecision(20);
    cout<<"num1 : "<<num1<<endl;
    cout<<"num2 : "<<num2<<endl;
    cout<<"num3 : "<<num3<<endl;
   double num4{1.92e5};
    cout<<"size of num1 : "<<num4<<endl;
    double num5 {5.6};
    double num6{};
    double num7{};
    double num8{-5.6};
    cout<<num5<<" / "<<num6<<"  yields : "<<(num5/num6)<<endl;
    cout<<num8<<" / "<<num6<<"  yields : "<<(num8/num6)<<endl;
    cout<<num7<<" / "<<num6<<"  yields : "<<(num7/num6)<<endl;
    return 0;
}