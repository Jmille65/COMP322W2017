#include <iostream>
#include <vector>
int main() {
  int * p = new int(5);
  int *q = p;
  delete p;
  delete q;
}
