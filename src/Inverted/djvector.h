#include <iostream>
#include <string>
#include <map>
using namespace std;
const int num_pages = 11; //number of pages

// define vector class
class DJVector {
public:
    DJVector(int sz = num_pages);
    DJVector(DJVector & L);
    ~DJVector() { delete[] data; data = NULL; last = -1; }
    int Size() const { return maxSize; } //return maxsize of vector
    int Search(int & x) const; //find x in vector and return index
    bool getData(int i, int & x) const; // return x as the i_th of a vector
    int Length() const { return last + 1; } //return length of a vector
    bool Insert(int i, int x); //insert x into i_th of a vector
    bool Append(int x); //appen x in the tail of a vector
    bool Remove(int i, int & x); //remove the i_th of a vetor

private:
    int * data;
    int maxSize; //maxsize of a vector
    int last; //index of tail
    friend ostream & operator <<(ostream & os, const DJVector & list);
};
