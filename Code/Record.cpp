#include "Record.h"

Record::Record() {
        numColoumns = 0;
        line = "";
        //next = nullptr;
    }
Record::Record(str l) {
        numColoumns = 0;
        line = l;
        insertRecord(line);
        //next = nullptr;
    }
void Record::insertRecord(str l) { //str to record
        int index = 0;
        line = l;
        str insert;
        for (int i = 0; l[i] != '\0' && l[i] != '\n'; i++) {
            if (l[i] == ',') {
                insert += '\0';
                coloumns.push_back(insert);
                insert = "";
                numColoumns++;
            }
            else {
                insert += l[i];
            }
        }
    }
    str Record::operator[](int index) {
        return coloumns[index];
    }
void Record::display() {
        int s = coloumns.getSize();
        for (int i = 0; i < s; i++)
            cout << i << ". " << coloumns[i] << endl;
    }
