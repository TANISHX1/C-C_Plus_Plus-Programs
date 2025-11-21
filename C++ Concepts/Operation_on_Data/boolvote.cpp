// vote eligibility based on age
#include <iostream>
#include <cctype>
using namespace std;
bool validinput(string input){
    for(char ch:input)
    if(!isdigit(ch)) return false;

    return !input.empty();
}
int main(){
    int age;
    bool limit;string input;
    do
    {
        cout<<"Enter the age : ";
        cin>>input;
        if(!validinput(input))cout<<"Enter a valid input:/\n";
    
    } while (!validinput(input));
    age = stoi(input);
    
    age>=18 ?limit = true:limit = false;
    if(limit)
    cout<<"Eligible for voting!...\n";
    else 
    cout<<"Not Eligible for voting !..\n";
    return 0;
}