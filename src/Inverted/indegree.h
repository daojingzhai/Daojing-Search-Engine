#include <assert.h>
#include <fstream>
#include <sstream>
using namespace std;


bool split_string(const string & str, string & url) {
    int start = 0;
    for (int i = 0; i < str.length(); ++i)
        if (str[i] == ' ') {
            start = i + 1;
            break;
        }
    url = str.substr(start);
    return true;
}

bool indegree_table(map<int, int> & table, const string & dir) {
    cout << "creating indegree table..." << endl;
    for (int i = 0; i < num_pages; ++i)
        table[i] = 0;

    //read database.txt, creat indegree table
    fstream database, indegree;
    database.open(dir, fstream::in | ios::binary);
    assert(!database.fail());
    indegree.open("../Data/indegree.txt", fstream::out);
    assert(!indegree.fail());

    while (!database.eof()) {
        string line, indegre_string;
        getline(database, line);
        split_string(line, indegre_string);

    // use map iterator
        map<int, int>::iterator iter;
        stringstream input(indegre_string);
        int index;
        while (input >> index) {
            iter = table.find(index);
            if (iter == table.end())
                table[index] = 1;
            else
                (*iter).second++;
        }
    }

    map<int, int>::iterator it;
    for (it = table.begin(); it != table.end(); ++it)
        indegree << (*it).first << ' ' << (*it).second << endl;

    database.close();
    indegree.close();
    cout << "done" << endl;

    return true;
}
