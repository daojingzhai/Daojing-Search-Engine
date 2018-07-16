using namespace std;

//build inverted table
bool inverted_index(map<string, DJVector*> & index, const map<int,int> & indegree, const string & dir) {
    cout << "creating inverted index..." << endl;

    for (int i = 0; i < num_pages; ++i) {
        fstream vocab;
        vocab.open(dir+to_string(i)+".txt", fstream::in | ios::binary);
        assert(!vocab.fail());

        while (!vocab.eof()) {
            string line;
            getline(vocab, line);
            if (line == "\n") continue;
            map<string, DJVector*>::iterator it;
            it = index.find(line);
            if (it == index.end()) {
                DJVector *list = new DJVector;
                list->Append(i);
                index[line] = list;
            }
            else
                (*index[line]).Append(i);
        }
        vocab.close();
        cout.precision(1);
        double percent = i * 100 / num_pages;
        if(i % 100 == 0 && i != 0)
            cout << fixed << percent << '%' << endl;
    }

    fstream file;
    file.open("../Data/index.txt", fstream::out);
    assert(!file.fail());
    map<string, DJVector*>::iterator it;
    for (it = index.begin(); it != index.end(); ++it) {
        file << (*it).first << ' ' << *((*it).second) << endl;
    }
    file.close();

    cout << "done" << endl;

    return true;
}
