// Author: Jordan Miller (McGill University, student no. 260513815)
// March 29, 2017
// Compile w/ C++11 or later

#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <functional>

using namespace std;

class IComparer {
public:
  virtual bool isBefore(int a, int b) {
    return true;
  }
};

typedef bool (*compFunction)(int, int);
void mergeSortHelperWithPointers(vector<int>&, compFunction);
void sortWithPointers(int*, int, compFunction);
void mergeWithPointers(vector<int>&, vector<int>&, vector<int>&, compFunction);
void mergeSortHelperWithInterfaces(vector<int>&, IComparer*);
void sortWithInterfaces(int*, int, IComparer*);
void mergeWithInterfaces(vector<int>&, vector<int>&, vector<int>&, IComparer*);
void mergeSortHelperWithStandardFunction(vector<int>&, function<bool(int,int)>);
void sortWithStandardFunction(int*, int, function<bool(int,int)>);
void mergeWithStandardFunction(vector<int>&, vector<int>&, vector<int>&, function<bool(int,int)>);
function<bool(int,int)> generateNearestTo(int);
bool isLessThan(int, int);
bool isGreaterThan(int, int);
void testSortWithPointers(int);
void testSortWithInterfaces(int);
void testSortWithLambdas(int);
void printIntArray(int*, int);

int main(int argc, char* argv[]) {

  int arraySize = 10;
  testSortWithPointers(arraySize);
  testSortWithInterfaces(arraySize);
  testSortWithLambdas(arraySize);

  return 0;
}

class LessThanComparer : public IComparer {
public:
  bool isBefore(int a, int b) {return a < b;}
};

class GreaterThanComparer : public IComparer {
public:
  bool isBefore(int a, int b) {return a > b;}
};

class IsClosestToComparer : public IComparer {
private:
  int center;
public:
  IsClosestToComparer(int pCenter) {
    center = abs(pCenter);
  }
  bool isBefore(int a, int b) {
    return (abs(abs(a) - center) < abs(abs(b) - center));
  }
};

function<bool(int,int)> generateNearestTo(int center) {
  return [center](int a, int b) {
    return (abs(abs(a)-abs(center)) < abs(abs(b) - abs(center)));};
}

void testSortWithPointers(int arraySize) {
  int* targetArray = new int[arraySize];

  for (int i = 0; i < arraySize; i++) {
    int x = rand() % arraySize;
    targetArray[i] = x;
  }

  cout<<"FUNCTION POINTER SORT TEST\nOriginal Array: "<<endl;
  printIntArray(targetArray, arraySize);

  cout<<"Forwards: "<<endl;
  sortWithPointers(targetArray, arraySize, isLessThan);
  printIntArray(targetArray, arraySize);

  cout<<"Backwards: "<<endl;
  sortWithPointers(targetArray, arraySize, isGreaterThan);
  printIntArray(targetArray, arraySize);
}

void testSortWithInterfaces(int arraySize) {
  int* targetArray = new int[arraySize];

  for (int i = 0; i < arraySize; i++) {
    int x = rand() % arraySize;
    targetArray[i] = x;
  }

  cout<<"INTERFACE SORT TEST\nOriginal Array: "<<endl;
  printIntArray(targetArray, arraySize);

  cout<<"Forwards: "<<endl;
  sortWithInterfaces(targetArray, arraySize, new LessThanComparer());
  printIntArray(targetArray, arraySize);

  cout<<"Backwards: "<<endl;
  sortWithInterfaces(targetArray, arraySize, new GreaterThanComparer());
  printIntArray(targetArray, arraySize);

  cout<<"Closest to 5: "<<endl;
  sortWithInterfaces(targetArray, arraySize, new IsClosestToComparer(5));
  printIntArray(targetArray, arraySize);
}

void testSortWithLambdas(int arraySize) {
  int* targetArray = new int[arraySize];

  for (int i = 0; i < arraySize; i++) {
    int x = rand() % arraySize;
    targetArray[i] = x;
  }

  cout<<"LAMBDA SORT TEST\nOriginal Array: "<<endl;
  printIntArray(targetArray, arraySize);

  cout<<"Forwards: "<<endl;
  sortWithStandardFunction(targetArray, arraySize, isLessThan);
  printIntArray(targetArray, arraySize);

  cout<<"Backwards: "<<endl;
  sortWithStandardFunction(targetArray, arraySize, isGreaterThan);
  printIntArray(targetArray, arraySize);

  cout<<"Nearest to 3: "<<endl;
  sortWithStandardFunction(targetArray, arraySize, generateNearestTo(3));
  printIntArray(targetArray, arraySize);

  cout<<"Nearest to 10: "<<endl;
  sortWithStandardFunction(targetArray, arraySize, generateNearestTo(10));
  printIntArray(targetArray, arraySize);
}

void printIntArray(int* targetArray, int arraySize) {
  cout<<"["<<targetArray[0];
  for (int i = 1; i < arraySize; i++) cout<<"; "<<targetArray[i];
  cout<<"]"<<endl;
}

bool isLessThan(int a, int b) {return a < b;}

bool isGreaterThan(int a, int b) {return a > b;}

void sortWithPointers(int* p, int length, compFunction func) {
  vector<int> q(p, p + length);
  mergeSortHelperWithPointers(q, func);
  for (int i = 0; i < length; i++) {
    p[i] = q.at(i);
  }
}

void mergeWithPointers(vector<int>& a, vector<int>& b, vector<int>& p, compFunction func) {
  p.erase(p.begin(), p.end());
  unsigned int aIndex = 0;
  unsigned int bIndex = 0;
  while (aIndex < a.size() && bIndex < b.size()) {
    int x = a.at(aIndex);
    int y = b.at(bIndex);
    if (func(x,y)) {
      p.push_back(a.at(aIndex++));
    } else {
      p.push_back(b.at(bIndex++));
    }
  }
  while (bIndex < b.size()) {
    p.push_back(b.at(bIndex++));
  }
  while (aIndex < a.size()) {
    p.push_back(a.at(aIndex++));
  }
}

void mergeSortHelperWithPointers(std::vector<int>& q, compFunction func) {
  if (q.size() > 1) {
    vector<int> lo(q.begin(), q.begin() + q.size()/2);
    mergeSortHelperWithPointers(lo, func);
    vector<int> hi(q.begin() + q.size()/2, q.end());
    mergeSortHelperWithPointers(hi, func);
    mergeWithPointers(lo, hi, q, func);
  }
}

void sortWithInterfaces(int* p, int length, IComparer* compare) {
  vector<int> q(p, p + length);
  mergeSortHelperWithInterfaces(q, compare);
  for (int i = 0; i < length; i++) {
    p[i] = q.at(i);
  }
}

void mergeWithInterfaces(vector<int>& a, vector<int>& b, vector<int>& p, IComparer* compare) {
  p.erase(p.begin(), p.end());
  unsigned int aIndex = 0;
  unsigned int bIndex = 0;
  while (aIndex < a.size() && bIndex < b.size()) {
    int x = a.at(aIndex);
    int y = b.at(bIndex);
    if (compare->isBefore(x,y)) {
      p.push_back(a.at(aIndex++));
    } else {
      p.push_back(b.at(bIndex++));
    }
  }
  while (bIndex < b.size()) {
    p.push_back(b.at(bIndex++));
  }
  while (aIndex < a.size()) {
    p.push_back(a.at(aIndex++));
  }
}

void mergeSortHelperWithInterfaces(std::vector<int>& q, IComparer* compare) {
  if (q.size() > 1) {
    vector<int> lo(q.begin(), q.begin() + q.size()/2);
    mergeSortHelperWithInterfaces(lo, compare);
    vector<int> hi(q.begin() + q.size()/2, q.end());
    mergeSortHelperWithInterfaces(hi, compare);
    mergeWithInterfaces(lo, hi, q, compare);
  }
}

void sortWithStandardFunction(int* p, int length, function<bool(int,int)> func) {
  vector<int> q(p, p + length);
  mergeSortHelperWithStandardFunction(q, func);
  for (int i = 0; i < length; i++) {
    p[i] = q.at(i);
  }
}

void mergeWithStandardFunction(vector<int>& a, vector<int>& b, vector<int>& p, function<bool(int,int)> func) {
  p.erase(p.begin(), p.end());
  unsigned int aIndex = 0;
  unsigned int bIndex = 0;
  while (aIndex < a.size() && bIndex < b.size()) {
    int x = a.at(aIndex);
    int y = b.at(bIndex);
    if (func(x,y)) {
      p.push_back(a.at(aIndex++));
    } else {
      p.push_back(b.at(bIndex++));
    }
  }
  while (bIndex < b.size()) {
    p.push_back(b.at(bIndex++));
  }
  while (aIndex < a.size()) {
    p.push_back(a.at(aIndex++));
  }
}

void mergeSortHelperWithStandardFunction(std::vector<int>& q, function<bool(int,int)> func) {
  if (q.size() > 1) {
    vector<int> lo(q.begin(), q.begin() + q.size()/2);
    mergeSortHelperWithStandardFunction(lo, func);
    vector<int> hi(q.begin() + q.size()/2, q.end());
    mergeSortHelperWithStandardFunction(hi, func);
    mergeWithStandardFunction(lo, hi, q, func);
  }
}
