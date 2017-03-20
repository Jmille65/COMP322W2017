#include <stddef.h>
#include <iostream>
#include <string.h>

template <class T> class DynamicArrayIterator;

template <class T>
class DynamicArray {
  private:
    int aSize;
    int aCapacity;
    int aPosition;
    T *aArray;

  public:
    DynamicArray<T>() {
      aArray = new T[10];
      aSize = 10;
      aCapacity = 10;
      aPosition = 0;
    }

    // attempts to get the destructor to not give malloc problems
    // full credit to StackOverflow/reading about the Rule of Three
    DynamicArray<T>(const DynamicArray<T> &pObj) { //copy constructor
      aArray = new T[pObj.aCapacity];
      for(int i = 0; i < pObj.aCapacity; i++) {
        memcpy(&aArray[i], &pObj.aArray[i], sizeof(T));
      }
      aSize = pObj.aSize;
      aCapacity = pObj.aCapacity;
      aPosition = pObj.aPosition;
    }

    DynamicArray<T>& operator =(const DynamicArray<T> &pObj) { // also rule of 3
      aArray = new T[pObj.aCapacity];
      for(int i = 0; i < pObj.aCapacity; i++) {
        memcpy(&aArray[i], &pObj.aArray[i], sizeof(T));
      }
      aSize = pObj.aSize;
      aCapacity = pObj.aCapacity;
      aPosition = pObj.aPosition;
      return *this;
    }

    ~DynamicArray<T>() { //all other fields are taken care of automatically
      delete [] aArray;
    }

    T& operator [](int pIndex) {
      return aArray[pIndex];
    }

    int size() {
      return aSize;
    }

    void add(T pItem) {
      if (aPosition == aCapacity) {
        T* newArray = new T[aCapacity*2];
        for (int i = 0; i < aSize; i++) {
          newArray[i] = aArray[i];
        }
        delete aArray;
        aArray = newArray;
        aCapacity = aCapacity*2;
        aArray = newArray;
      }
      aArray[aPosition++] = pItem;
    }

    T remove(int pIndex) {
      T temp = aArray[pIndex];
      for (int i = pIndex; i < aCapacity; i++) {
        aArray[i] = aArray[i+1];
      }
      // aArray[(aPosition--)-1] = NULL;
      // optional- throws compiler warning using clang
      aPosition--;
      return temp;
    }

    DynamicArrayIterator<T> begin() {
      return DynamicArrayIterator<T>(this, 0, 1);
    }

    DynamicArrayIterator<T> end() {
      return DynamicArrayIterator<T>(this, aPosition, 1);
    }

    DynamicArrayIterator<T> r_begin() {
      return DynamicArrayIterator<T>(this, aPosition-1, -1);
    }

    DynamicArrayIterator<T> r_end() {
      return DynamicArrayIterator<T>(this, -1, -1);
    }
};

template <class T>
class DynamicArrayIterator {
  private:
    int aCurrentPosition;
    DynamicArray<T> aArray;
    int aDirection;

  public:
    DynamicArrayIterator<T>(DynamicArray<T> *pArray, int pIndex, int pDirection) {
      aArray = *pArray;
      aCurrentPosition = pIndex;
      aDirection = pDirection;
    }

    bool operator ==(DynamicArrayIterator pIterator) {
      if (aCurrentPosition == pIterator.aCurrentPosition) {
        return true;
      } else {
        return false;
      }
    }

    bool operator !=(DynamicArrayIterator pIterator) {
      if (aCurrentPosition == pIterator.aCurrentPosition) {
        return false;
      } else {
        return true;
      }
    }

    void operator ++() {
      aCurrentPosition += aDirection;
    }

    T& operator *() {
      return aArray[aCurrentPosition];
    }
};

int main() {
  DynamicArray<int> foo;
  foo.add(3);
  foo.add(2);
  std::cout<<foo[1]<<std::endl;
  foo[1] = 10;
  std::cout<<foo[1]<<std::endl;

  for(DynamicArrayIterator<int> a = foo.begin(); a != foo.end(); ++a) {
    std::cout<<*a<<std::endl;
  }

  foo.remove(0);
  foo.add(7);

  for(DynamicArrayIterator<int> a = foo.r_begin(); a != foo.r_end(); ++a) {
    std::cout<<*a<<std::endl;
  }
}
