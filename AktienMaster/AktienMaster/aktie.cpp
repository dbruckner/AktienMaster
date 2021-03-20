# include "aktie.h"
# include <iostream>

using namespace std;


Aktie::Aktie() {
    this->name = "deleted";
}


Aktie::Aktie(string name, string WKN, string abb) {

    this->name = name;
    this->WKN = WKN;
    this->abb = abb;

}

void Aktie::printAktie() {

    cout<< this->name << "  " << this->WKN << "  " <<  this->abb << endl;


}

std::string Aktie::getName() {
    return this->name;
}

std::string Aktie::getAbby() {
    return this->abb;
}

vector<double> Aktie::getClose() {
    return this->close;
}

vector<string> Aktie::getDate() {
    return this->date;
}


void Aktie::printVector(int type) {

        vector<double> printDouble;

        vector<string> printString;

        switch(type) {
        case 0:
            printString = this->date;
            break;
        case 1:
            printDouble = this->open;
            break;
        case 2:
            printDouble = this->high;
            break;
        case 3:
            printDouble = this->low;
            break;
        case 4:
            printDouble = this->close;
            break;
        case 5:
            printDouble = this->adjClose;
            break;
        case 6:
            printDouble = this->volume;
            break;
        default:

            break;
        }

        if (type != 0) {
            for (double x : printDouble)
            cout << x << "\n";
        }
        else {
            for (string x : printString)
            cout << x << "\n";
        }


}

void Aktie::addValue(int type, double value) {

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
            this->adjClose.push_back(value);
            break;
        case 6:
            this->volume.push_back(value);
            break;
        default:

            break;


    }

}

void Aktie::addDate(string date) {

    this->date.push_back(date);


}


