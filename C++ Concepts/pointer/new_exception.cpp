// Demonstration of exception handling and nothrow new in C++
#include <iostream>

int main(){
    for(size_t i{};i<10000000;++i){
    /*{
        int *data = new int[1000];
    }*/
  /* try
   {
    int *data = new int[10000000];
   }
   catch(const std::exception& e)
   {
    std::cout<<"Something went wrong : " << e.what() << '\n';
   }}
  std:: cout<<"execution completed\n";*/
  for(size_t{0};i<100;++i)
  {
    int *data = new(std:: nothrow)int[10000000];
    if(data != nullptr){
        std:: cout<<"allocated\n";
    }
    else{
        std::cout<<"data allocation failed\n";
    }
  }

}}