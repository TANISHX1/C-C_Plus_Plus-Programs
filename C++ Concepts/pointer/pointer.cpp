// Demonstration of basic pointer usage in C++
#include<iostream>
using namespace std;

int main (){
    int *p_number{};
    double *p_number1{};
    int num{20};
    p_number = &num;
    
    cout<<"size of(int pointer):"<<sizeof(p_number)<<"B  "<<endl<<"size of(double pointer): "<<sizeof(p_number1)<<"B"<<endl;

    cout<<"Value of num: "<<num<<"  Address: "<<&num<<endl;
    cout<<"Value of p_number: "<<p_number<<" |  Address: "<<&p_number<<
    //dereferencing a pointer (means we are reading the value of variable whose address in stored in pointer)
    " |  value at (*p_number): "<<*p_number<<endl;
    cout<<"Memory Addrss[p_number]:"<<&p_number<<endl;
    cout<<"Memory Addrss[p_number1]:"<<&p_number1<<endl;
    return 0;
}