#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <fstream>
// #include <algorithm> //only used to compare builtin sort as well

using namespace std;

void mergeSortHelper(vector<int>&);
void mergeSort(int*, int);
void merge(vector<int>&, vector<int>&, vector<int>&);
void bubbleSort(int*, int);
void builtInSort(int*, int);

int main(int argc, char* argv[]) {
  clock_t bubbleTicks = 0;
  clock_t mergeTicks = 0;
  // clock_t builtInTicks = 0;
  int bubbleTime = 0;
  int mergeTime = 0;
  // int builtInTime = 0;
  int arraySize = 10;
  ofstream output ("comparison.tsv");
  output<<"n\tbubbleSort\tmergeSort\n";
  cout<<"Sorting... \n";
  while (bubbleTime < 8000000) { //8 seconds
    int* bubbleTarget = new int[arraySize];
    int* mergeTarget = new int[arraySize];
    // int* builtInTarget = new int[arraySize];

    for (int i = 0; i < arraySize; i++) {
      int x = rand() % arraySize;
      bubbleTarget[i] = x;
      mergeTarget[i] = x;
      // builtInTarget[i] = x;
    }

    // I know the spec said to use milliseconds, but I'm using microseconds
    // mergeSort is so efficient that until about n = 1000, it rounds to 0ms, and built-in is even better
    bubbleTicks = clock();
    bubbleSort(bubbleTarget, arraySize);
    bubbleTime = 1000000.0*(clock() - bubbleTicks)/CLOCKS_PER_SEC;
    mergeTicks = clock();
    mergeSort(mergeTarget, arraySize);
    mergeTime = 1000000.0*(clock() - mergeTicks)/CLOCKS_PER_SEC;
    // builtInTicks = clock();
    // builtInSort(builtInTarget, arraySize);
    // builtInTime = 1000000.0*(clock() - builtInTicks)/CLOCKS_PER_SEC;
    output<<arraySize<<" \t"<<bubbleTime<<"\t"<<mergeTime<<endl;
    delete[] bubbleTarget;
    delete[] mergeTarget;
    // delete[] builtInTarget;
    arraySize = arraySize * 2;
  }
  output.close();
  cout<<"Finished! See comparison.txt for details. \n";
  return 0;
}

// I know it's not necessary to use vectors, and arrays would have been faster
// I used them for mergesort though, and to not for bubblesort would have given an unfair advantage due to overhead
void bubbleSort(int* p, int length) {
  vector<int> q(p, p + length);
  bool swapped = true;
  while (swapped == true) {
    swapped = false;
    for (unsigned int i = 1; i <= q.size() - 1; i++) {
      if (q.at(i-1) > q.at(i)) {
        int temp = q[i-1];
        q[i-1] = q[i];
        q[i] = temp;
        swapped = true;
      }
    }
  }
  for (int i = 0; i < length; i++) {
    p[i] = q.at(i);
  }
}

void mergeSort(int* p, int length) {
  vector<int> q(p, p + length);
  mergeSortHelper(q);
  for (int i = 0; i < length; i++) {
    p[i] = q.at(i);
  }
}

void merge(vector<int>& a, vector<int>& b, vector<int>& p) {
  p.erase(p.begin(), p.end());
  unsigned int aIndex = 0;
  unsigned int bIndex = 0;
  while (aIndex < a.size() && bIndex < b.size()) {
    int x = a.at(aIndex);
    int y = b.at(bIndex);
    if (x < y) {
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

void mergeSortHelper(std::vector<int>& q) {
  if (q.size() > 1) {
    vector<int> lo(q.begin(), q.begin() + q.size()/2);
    mergeSortHelper(lo);
    vector<int> hi(q.begin() + q.size()/2, q.end());
    mergeSortHelper(hi);
    merge(lo, hi, q);
  }
}

// only for my own gratification
// void builtInSort(int* p, int length) {
//   vector<int> q(p, p + length);
//   sort(q.begin(), q.end());
//   for (int i = 0; i < length; i++) {
//     p[i] = q.at(i);
//   }
// }
