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
        void addValue(int type, float value);
        void addDate(string date);

        std::string getName();
        std::string getAbby();

    private:
        string name;
        string WKN;
        string abb;
        vector<string> date;
        vector<float> open;
        vector<float> high;
        vector<float> low;
        vector<float> close;
        vector<float> volume;
        vector<float> adjClose;



};



#endif // AKTIE_H_INCLUDED


#ifndef AKTIE_H_INCLUDED
#define AKTIE_H_INCLUDED



#endif // AKTIE_H_INCLUDED
