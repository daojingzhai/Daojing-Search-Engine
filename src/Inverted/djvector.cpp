#include "djvector.h"
using namespace std;

DJVector::DJVector(int sz) {
    if (sz > 0) {
        maxSize = sz;
        last = -1;
        data = new int[maxSize];
    }
}

DJVector::DJVector(DJVector & L) {
    int value;
    maxSize = L.Size();
    last = L.Length() - 1;
    data = new int[maxSize];
    for (int i = 1; i <= last + 1; ++i) {
      L.getData(i, value);
      data[i - 1] = value;
    }
}

int DJVector::Search(int & x) const {
    for (int i = 0; i <= last; ++i)
        if (data[i] == x)
            return i + 1;
    return 0;
}

bool DJVector::getData(int i, int & x) const {
    if (i < 1 || i > last + 2) return false;
    x = data[i];
    return true;
}

bool DJVector::Append(int x) {
    if (last == maxSize - 1) return false;
    ++last;
    data[last] = x;
    return true;
}

bool DJVector::Insert(int i, int x) {
    if (last == maxSize - 1) return false;
    if (i < 1 || i > last + 2) return false;
    for (int j = last; j >= i - 1; --j)
        data[j + 1] = data[j];
    data[i - 1] = x;
    ++last;
    return true;
}

bool DJVector::Remove(int i, int & x) {
    if (last == -1) return false;
    if (i < 1 || i > last + 1) return false;
    x = data[i - 1];
    for (int j = i; j <= last; j++)
        data[j - 1] = data[j];
    --last;
    return true;
}

ostream & operator <<(ostream & os, const DJVector & list) {
    for (int i = 0; i < list.last; ++i)
        os << list.data[i] << ' ';
    os << list.data[list.last];
    return os;
}
