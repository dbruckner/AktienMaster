# include "aktie.h"
# include <iostream>

using namespace std;


Aktie::Aktie(string name, string WKN, string abb) {

    this->name = name;
    this->WKN = WKN;
    this->abb = abb;

}

void Aktie::printAktie() {

    cout<< this->name << "  " << this->WKN << "  " <<  this->abb << endl;


}

void Aktie::printVector(int type) {

        vector<float> printIt;

        switch(type) {
        case 1:
            printIt = this->open;
            break;
        case 2:
            printIt = this->high;
            break;
        case 3:
            printIt = this->low;
            break;
        case 4:
            printIt = this->close;
            break;
        case 5:
            printIt = this->volume;
            break;
        case 6:
            printIt = this->adjClose;
            break;
        default:

            break;
        }

        for (float x : printIt)
        cout << x << " ";


}

void Aktie::addValue(int type, float value) {

    switch(type) {
        case 1:
            this->open.push_back(value);
            break;
        case 2:
            this->high.push_back(value);
            break;
        case 3:
            this->low.push_back(value);
            break;
        case 4:
            this->close.push_back(value);
            break;
        case 5:
            this->volume.push_back(value);
            break;
        case 6:
            this->adjClose.push_back(value);
            break;
        default:

            break;


    }

}


