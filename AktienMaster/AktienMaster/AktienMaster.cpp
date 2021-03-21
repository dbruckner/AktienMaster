#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include "pbplots/pbPlots.hpp"
#include "pbplots/supportLib.hpp"
#include "type.h"
#include "aktie.h"

#define PRIME 1009;


Aktie* hashtableNames[1009];
Aktie* hashtableAbb[1009];



int power(int base, int exponent){
	if(exponent == 0){
		return 1;
	}
	return base * power(base, exponent - 1);
}

int hashFunc(std::string str, int n){

	int sum = 0;

	for(int i = n - 1; i > 0; i--){
		sum = (sum + str[n - i - 1] * power(31, i)) % PRIME;
	}

	return sum;
}

int calculateHashPosition(std::string name, Aktie* hashtable[]){
	int pos = 0;
	pos = hashFunc(name, name.length());


	if(hashtable[pos] != nullptr){
		int counter = 1;
		while(1){
			pos += (counter * counter);
			pos %= PRIME;

			if(hashtable[pos] == nullptr || hashtable[pos]->getName() == "deleted"){
				if(hashtable[pos] != nullptr){
					hashtable[pos]->~Aktie();
					hashtable[pos] = nullptr;
				}
				break;
			}

			counter++;
		}
	}

	return pos;
}

Aktie* insertIntoHashtables(std::string name, std::string WKN, std::string abb){
	Aktie* aktie = new Aktie(name, WKN, abb);

	hashtableNames[calculateHashPosition(name, hashtableNames)] = aktie;
	hashtableAbb[calculateHashPosition(abb, hashtableAbb)] = aktie;
	return aktie;
}


int searchPos(std::string name, Aktie* hashtable[], int type){
	std::string word = name;
	int pos = hashFunc(word, word.length());
	int counter = 1;

	while(1){
		if(type == 2){
			if(hashtable[pos] != nullptr && hashtable[pos]->getAbby() == word){
				return pos;
			}
		} else if(counter == 750){
			return -1;
		} else{
			if(hashtable[pos] != nullptr && hashtable[pos]->getName() == word){
				return pos;
			}
		}

		pos += (counter * counter);
		pos %= PRIME;
		counter++;
	}
}

void deleteAktie(std::string name){
	Aktie* delaktie = new Aktie();

	int posname = searchPos(name, hashtableNames, 1);
	if(posname == -1){
		std::cout << "Aktie wurde nicht gefunden" << std::endl;
	} else{
		std::string abb = hashtableNames[posname]->getAbby();
		int posabby = searchPos(abb, hashtableAbb, 2);

		hashtableNames[posname]->~Aktie();
		hashtableAbb[posabby] = delaktie;
		hashtableNames[posname] = delaktie;
	}
}


int searchAktie(int type){
	std::string userinput = " ";

	std::cout << "Geben Sie die Information der Aktie ein die Sie suchen: ";
	std::cin >> userinput;
	if(type == 2){

		return searchPos(userinput, hashtableAbb, type);
	} else{
		return searchPos(userinput, hashtableNames, type);
	}
}


void addAktie(){
	std::string name = " ";
	int WKN = 0;
	std::string abb = " ";

	std::cout << "Geben Sie bitte den Namen der Firma an: ";
	std::cin >> name;
	while(1){
		if(!cin){
			std::cout << "Bitte geben Sie nur Buchstaben an: ";
			std::cin >> name;
		} else{
			break;
		}
	}

	std::cout << "Geben Sie bitte nun die WKN an:";
	std::cin >> WKN;
	while(1){
		if(!cin){
			std::cout << "Bitte geben Sie nur Zahlen an: ";
			std::cin >> WKN;
		} else{
			break;
		}
	}

	std::cout << "Geben Sie nun bitte zuletzt die abb der von Ihnen ausgew�hlten Firma an: ";
	std::cin >> abb;
	while(1){
		if(!cin){
			std::cout << "Bitte geben Sie nur Buchstaben an: ";
			std::cin >> abb;
		} else{
			break;
		}
	}

	insertIntoHashtables(name, to_string(WKN), abb);
}

int getNumLines(string filename){

	int numLines = 0;
	string line;

	ifstream input(filename);

	while(getline(input, line)){
		numLines++;
	}

	return numLines;
}

void importData(Aktie& inputAktie){

	string filename = "CSV/" + inputAktie.getAbby() + ".csv";

	ifstream input(filename);

	int numLines = getNumLines(filename);

	string uselessLine;
	string nextLine;
	string nextValue;
	double Value;

	for(int i = 0; i < numLines - 30; i++){
		getline(input, uselessLine);
	}

	while(getline(input, nextLine)){

		int i = 0;

		stringstream s(nextLine);
		while(getline(s, nextValue, ',')){

			int n = (i % 7);

			if(n != 0){
				Value = stod(nextValue);
				inputAktie.addValue(n, Value);
			}         else{
				inputAktie.addDate(nextValue);
			}

			i++;
		}

	}



}

void plotCurve(Aktie& inputAktie){



	RGBABitmapImageReference* imgRef = CreateRGBABitmapImageReference();

	vector<double> x;
	vector<double> y;

	vector<double> data = inputAktie.getClose();
	int len = data.size();


	for(int i = len - 30; i < len; i++){
		x.push_back(len - i);
		y.push_back(data.at(i));

	}
	cout << x.size() << endl;
	cout << y.size() << endl;


	DrawScatterPlot(imgRef, 600, 400, &x, &y);

	vector<double>* pngData = ConvertToPNG(imgRef->image);

	string filename = "plots/" + inputAktie.getAbby() + ".png";

	WriteToFile(pngData, filename);
	DeleteImage(imgRef->image);


}

void loadDatabase(){
	string filename = "CSV/database.txt";

	ifstream input(filename);

	string header;

	string name;
	string wkn;
	string abb;


	string nextLine;
	string nextValue;
	double Value;
	while(1){

		if(std::getline(input, header)){
			stringstream headerstream(header);

			std::getline(headerstream, name, ',');
			std::getline(headerstream, wkn, ',');
			std::getline(headerstream, abb, ',');

			Aktie* aktie = insertIntoHashtables(name, wkn, abb);


			int c = 0;

			for(int i = 0; i < 30; i++){

				std::getline(input, nextLine);
				stringstream s(nextLine);
				while(std::getline(s, nextValue, ',')){

					int n = (c % 7);

					if(n != 0){
						Value = stod(nextValue);
						aktie->addValue(n, Value);
					} else{
						aktie->addDate(nextValue);
					}
					c++;
				}
			}
		} else{
			break;
		}
	}
}


void saveToFile(){

	ofstream myfile;
	myfile.open("CSV/database.txt");

	for(int i = 0; i < 1009; i++){
		if(hashtableNames[i] != nullptr){
			if(hashtableNames[i]->getName() != "deleted"){


				string first = hashtableNames[i]->getName() + "," + hashtableNames[i]->getWKN() + "," + hashtableNames[i]->getAbby() + "\n";

				vector<string> dateValues = hashtableNames[i]->getDate();
				vector<double> openValues = hashtableNames[i]->getVector(open);
				vector<double> highValues = hashtableNames[i]->getVector(high);
				vector<double> lowValues = hashtableNames[i]->getVector(low);
				vector<double> closeValues = hashtableNames[i]->getVector(close);
				vector<double> adjCloseValues = hashtableNames[i]->getVector(adjClose);
				vector<double> volumeValues = hashtableNames[i]->getVector(volume);

				myfile << first;

				int len = dateValues.size();

				for(int j = 0; j < len; j++){
					string dataLine = dateValues.at(j) + "," +
						to_string(openValues.at(j)) + "," +
						to_string(highValues.at(j)) + "," +
						to_string(lowValues.at(j)) + "," +
						to_string(closeValues.at(j)) + "," +
						to_string(adjCloseValues.at(j)) + "," +
						to_string(volumeValues.at(j)) + "\n";

					myfile << dataLine;
				}

			}

		}
	}
	myfile.close();
}


int main(){

	for(int i = 0; i < 1009; i++){
		hashtableNames[i] = nullptr;
		hashtableAbb[i] = nullptr;
	}

	std::cout << "Willkommen im Aktienmaster Ihr persoehnliches Aktien Tool!" << std::endl;
	int userinput;

	while(1){
		cout << "Was wollen sie tun?" << endl
			<< "(1) neue Aktien hinzufuegen" << endl
			<< "(2) einen Index suchen" << endl
			<< "(3) Kursdaten aus einer csv-Datei in eine Aktie Importieren" << endl
			<< "(4) Kurve anzeigen" << endl
			<< "(5) Aktie loeschen" << endl
			<< "(6) Daten speichern" << endl
			<< "(7) Daten laden" << endl
			<< "(0) Progam beenden" << endl;
		std::cin >> userinput;
		if(userinput == 1){
			addAktie();
		} else if(userinput == 2){
			std::cout << "Wollen Sie nach dem Namen oder nach der Abb suchen? (1) nach dem Namen (2) nach der Abb" << std::endl;
			int type = 0;
			int pos = 0;
			std::cin >> type;
			if(type == 2){
				pos = searchAktie(type);
			} else{
				pos = searchAktie(type);
			}
			if(pos != -1 && type == 1){
				hashtableNames[pos]->printAktie();
			} else if(pos != -1 && type == 2){
				hashtableAbb[pos]->printAktie();
			} else{
				std::cout << "Aktie kann nicht gefunden werden" << std::endl;
			}
		} else if(userinput == 3){
			string kurzel;
			cout << "Geben sie das Kurzel der Aktie an deren Werte Sie importieren mochten" << endl;
			cin >> kurzel;
			int pos = searchPos(kurzel, hashtableAbb, 2);
			importData(*hashtableAbb[pos]);
			//hashtableAbb[pos]->printVector(high);
		} else if(userinput == 4){
			string kurzel;
			cout << "Geben sie das Kurzel der Aktie an die Sie anzeigen mochten" << endl;
			cin >> kurzel;
			int pos = searchPos(kurzel, hashtableAbb, 2);
			plotCurve(*hashtableAbb[pos]);
			//hashtableAbb[pos]->printVector(high);
		} else if(userinput == 5){
			std::cout << "Geben Sie den Namen der Aktie die Sie l�schen wollen" << endl;
			std::string delname = " ";
			std::cin >> delname;
			deleteAktie(delname);
		} else if(userinput == 6){
			saveToFile();
		} else if(userinput == 7){
			loadDatabase();
		} else if(userinput == 0){
			for(int i = 0; i < 1009; i++){
				if(hashtableNames[i] != nullptr){
					hashtableNames[i]->~Aktie();
					hashtableNames[i] = nullptr;
				}
			}
			break;
		} else{
			std::cout << "Bitte geben Sie eine der angef�hrten Nummern ein" << std::endl;
		}
	}
}
