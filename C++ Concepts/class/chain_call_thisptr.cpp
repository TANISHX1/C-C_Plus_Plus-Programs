/*
 * chain_call_thisptr.cpp
 * Purpose: Demonstrates method chaining using the `this` pointer.
 * Description: Defines a `primary` class with setter-style member functions
 * that return `this` (a pointer to the current object) so calls can be chained
 * (fluent interface). Shows basic class design, member functions, and I/O.
 * Concepts used: classes, member functions, `this` pointer, method chaining,
 *                and iostream for output.
 */
#include <iostream>
using namespace std;
class primary {
  int age;
  string name;
  string region;

public:
  primary() = default;
  primary *get_name(string name) {
    this->name = name;
    return this;
  }
  primary *get_age(int age) {
    this->age = age;
    return this;
  }
  primary *get_region(string region) {
    this->region = region;
    return this;
  }
  void print() {
    cout << " Name   : " << this->name << endl;
    cout << " Age    : " << this->age << endl;
    cout << " Region : " << this->region << endl;
  }
};
int main() {
  primary obj;
  // chained caling
  obj.get_name("Tanish Shivhare")->get_age(20)->get_region("India")->print();
  return 0;
}
