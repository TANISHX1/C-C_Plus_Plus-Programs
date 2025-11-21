// Demonstration of dynamic memory allocation using 'new' and 'delete' in C++
#include<iostream>
#include<iomanip>
using namespace std;
int main(){
/*  int *ptr; //don't do such things in pointer. 
    *ptr = 45;  //it well crash ,modify or corrupt the data 
    cout<<*ptr;
    int *ptr1{};
    *ptr1 = 67;
    cout<<*ptr1;
    return 0;
*/
//dyanimic heap memory
//use this  (using 'new' givs a allocated memory in heap not in stack. 
//if we declared a variable or pointer normally .it is stored in stack memory)
int *ptr  {new int};
int *ptr1 {new int(20)};//use direct initialization
int *ptr2 {new int{30}};//use uniform initalization
cout<<left;

cout<<setw(20)<<"Pointer :"<<setw(20)<<"Value :"<<setw(20)<<"Address :"<<endl;
cout<<setw(20)<<"Value of ptr"<<setw(20)<<*ptr<<setw(20)<<ptr<<endl;//Junk value
cout<<setw(20)<<"Value of ptr1"<<setw(20)<<*ptr1<<setw(20)<<ptr1<<endl;
cout<<setw(20)<<"Value of ptr 2"<<setw(20)<<*ptr2<<setw(20)<<ptr2<<endl;

//to delete the heap allocated memory or to return the allocated memory to the OS
delete ptr;//don't delete a particular pointer twice it can cause anything
ptr =  nullptr;//reset your pointer
delete ptr1;
ptr1 = nullptr;
delete ptr2;
ptr2 = nullptr;
cout<<endl;
cout<<"Addresses of pointers after deleting\n";
cout<<setw(20)<<"Address of ptr : "<<setw(20)<<ptr<<endl;
cout<<setw(20)<<"Address of ptr : "<<setw(20)<<ptr1<<endl;
cout<<setw(20)<<"Address of ptr : "<<setw(20)<<ptr2<<endl;
cout<<endl;
ptr  = new int;
ptr1 = new int (34);
ptr2 = new int {50};

cout<<setw(20)<<"Pointer :"<<setw(20)<<"Value :"<<setw(20)<<"Address :"<<endl;
cout<<setw(20)<<"Value of ptr"<<setw(20)<<*ptr<<setw(20)<<ptr<<endl;//Junk value
cout<<setw(20)<<"Value of ptr1"<<setw(20)<<*ptr1<<setw(20)<<ptr1<<endl;
cout<<setw(20)<<"Value of ptr 2"<<setw(20)<<*ptr2<<setw(20)<<ptr2<<endl;

delete ptr;//don't delete a particular pointer twice it can cause anything
ptr =  nullptr;//reset your pointer
delete ptr1;
ptr1 = nullptr;
delete ptr2;
ptr2 = nullptr;
cin.get();
return 0;
}