// Demonstration of dangling pointers in C++
#include <iostream>
using namespace std;
int main(){
   { //case 1 : uninitialized pointer
    int *ptr;//Dangling uninitialized pointer
    cout<< "uninitalized pointer:"<<endl;
    cout<<"pointer: " <<ptr<<endl;
    cout<<"pointer value: "<<*ptr<<endl; //modifing uninitalized ptr can cause crash
}
cout<<"---------------------------------------------------------\n";
{
    //case 2: deleted pointer
    int *ptr {new int{67}};
    cout<<"pointer(before delete): "<<*ptr<<endl;
    delete ptr;
    cout<<"pointer (After delete): "<<*ptr<<endl;
    ptr = nullptr;
}
cout<<"---------------------------------------------------------\n";

{
    //multiple pointers pointing to same address
    int *ptr{new int{100}};
    int *ptr1{ptr};

    cout<<" ptr  - "<<ptr<<"                      | Value of ptr  - "<<*ptr<<endl;
    cout<<" ptr1(before deleting ptr) - "<<ptr1<<" | Value of ptr1(before deleting ptr) - "<<*ptr1<<endl;
    delete ptr;
        cout<<endl <<" ptr1(After deleting ptr) -  "<<ptr1<<" | Value of ptr1(After deleting ptr)  - "<<*ptr1<<endl;
}
}