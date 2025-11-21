// Check whether a number is even or odd using boolean function
#include<iostream>
#include<cctype>
using namespace std;
bool validinput(string num){
    for(char ch:num)
    {if(!isdigit(ch))
        return false;
    }    
      return !num.empty();
    
}
int main(){
    bool reminder;
    int number;string input;
    do
    {
        cout<<"Enter a number:\t";
    cin>>input;
    if(!validinput(input))
    {
      cout<<"Enter a valid input:/ \n";
    }
    } while (!validinput(input));

    number = stoi(input);
    reminder = number%2;
    if(reminder)
    cout<<"Entered number is odd !..";
    else 
    cout<<"Entered number is even !..";
    return 0;
}