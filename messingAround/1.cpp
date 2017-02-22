#include <iostream>

int main() {
  int a[] = {10, 4, 20, 30};
  int* q = a - 1;
  int* r = q + *(a+1);
  *r = 2;
  q = r - *r/2;
  std::cout<<*q<<" "<<a[3]<<std::endl;
  return 0;
}

template<class InputIterator>
void doubleContents(InputIterator start, InputIterator end) {
  for (InputIterator current = start; current != end; current++) {
    *current = (*current) * 2;
  }
}
