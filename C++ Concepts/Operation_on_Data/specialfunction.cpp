// Demonstration of special functions in C++ iostream library
#include<iostream>
#include<ios>
#include<iomanip>
#include<ostream>
using namespace std;

void precision (int value)
{
   auto a =3.112345678901234567812345678098765432;

   cout<<"Defult precision: "<<a<<endl;
   cout<<"------------------------+------------------------\n";
   cout<<setprecision(value);
   printf("precision(%d): ",value);cout<<a<<endl;value+=10;
   cout<<"------------------------+------------------------\n";
   cout<<setprecision(value);
   printf("precision(%d): ",value);cout<<a<<endl;value+=10;
   cout<<"------------------------+------------------------\n";
   cout<<setprecision(value);
   printf("precision(%d): ",value);cout<<a<<endl;
}

void diff_no_system()
{
    int    num1{1234};
    int    num2{-1234};
    double num3{498.986};
    cout<<"Defult base format:"<<endl;
    cout<<"num1: "<<num1<<endl;
    cout<<"num2: "<<num2<<endl;
    cout<<"num3: "<<num3<<endl;
    cout<<"------------------------+------------------------\n";
    cout<<"numbers in dec :"<<endl;
    cout<<"num1: "<<dec <<num1<<endl;
    cout<<"num2: "<<dec <<num2<<endl;
    cout<<"num3: "<<dec <<num3<<endl;
    cout<<"------------------------+------------------------\n";
    cout<<"numbers in hex :"<<endl;
    cout<<"num1: "<<hex <<num1<<endl;
    cout<<"num2: "<<hex <<num2<<endl;
    cout<<"num3: "<<hex <<num3<<endl;
    cout<<"------------------------+------------------------\n";
    cout<<"numbers in oct :"<<endl;
    cout<<"num1: "<<oct <<num1<<endl;
    cout<<"num2: "<<oct <<num2<<endl;
    cout<<"num3: "<<oct <<num3<<endl;
}
void booluen_function()
{
    bool condition1{true};
    bool condition2{false};
    //normally
    cout<<"condition1: "<<condition1<<endl;
    cout<<"Condition2: "<<condition2<<endl<<endl;
    //if wants to forcefull print true/false instead of 1/0
    cout<<boolalpha;
    cout<<"condition1: "<<condition1<<endl;
    cout<<"Condition2: "<<condition2<<endl<<endl;
    //if wants to print in 1/0
    cout<<noboolalpha;
    cout<<"condition1: "<<condition1<<endl;
    cout<<"Condition2: "<<condition2<<endl<<endl;
}

void formatted_table (int length)
{
   cout<<left;
   //cout<<setfill('-');
   cout<<setw(length)<<"NAME"<<setw(length)<<"CITY"<<setw(length)<<"COUNTRY"<<endl;
   cout<<setw(length)<<"Tanish"<<setw(length)<<"Katni"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Yash B."<<setw(length)<<"Katni"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Yash V."<<setw(length)<<"Katni"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Raghav"<<setw(length)<<"Bhind"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Mohit"<<setw(length)<<"Gwalior"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Harshit"<<setw(length)<<"Jabalpur"<<setw(length)<<"India"<<endl;
   cout<<setw(length)<<"Prasun"<<setw(length)<<"Gwalior"<<setw(length)<<"India"<<endl;
}

int main(){
  formatted_table(20);
  cout<<"----------------------------------------------------------------------------------------------------\n";
  booluen_function();
  cout<<"----------------------------------------------------------------------------------------------------\n";
  diff_no_system();
  cout<<"----------------------------------------------------------------------------------------------------\n";
  precision(10);
   return 0;
}