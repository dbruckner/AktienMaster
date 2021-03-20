#ifndef AKTIE_H_INCLUDED
#define AKTIE_H_INCLUDED
#include <vector>
#include <string>

using namespace std;

class Aktie {

    public:
        Aktie();
        Aktie(string name, string WKN, string abb);
        void printAktie();

        void printVector(int type);
        void addValue(int type, double value);
        void addDate(string date);

        std::string getName();
        std::string getAbby();

        vector<double> getClose();
        vector<string> getDate();

    private:
        string name;
        string WKN;
        string abb;
        vector<string> date;
        vector<double> open;
        vector<double> high;
        vector<double> low;
        vector<double> close;
        vector<double> volume;
        vector<double> adjClose;



};



#endif // AKTIE_H_INCLUDED


#ifndef AKTIE_H_INCLUDED
#define AKTIE_H_INCLUDED



#endif // AKTIE_H_INCLUDED
