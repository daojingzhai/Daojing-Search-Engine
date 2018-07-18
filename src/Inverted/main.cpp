#include "djvector.h"
#include "indegree.h"
#include "inverted_index.h"
using namespace std;

int main()
{
    const string database_dir = "../Data/database.txt"; //database direction
    const string vocab_dir = "../Data/vocab/"; //vocab direction

    map<int, int> indegree;
    map<string, DJVector*> index;

    indegree_table(indegree, database_dir); //creat indegree table
    inverted_index(index, indegree, vocab_dir); //creat inverted index

    return 0;
}
