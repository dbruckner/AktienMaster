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

/*
* Function to calcualte the power of a value of a number that is given   
* 
* @param base		the base of the calcuation
* @param exponent	the exponent the calcualtion
* 
* @return sum		the calcualted value
*/

int power(int base, int exponent){
	if(exponent == 0){
		return 1;
	}
	return base * power(base, exponent - 1);
}

/*
* This function creates the hashvalue based on the word 
* 
* @param str	Word to which the hashvalue is caclulated
* @param n		length of the word
* 
* @return sum	returns the completed hashvalue 
*/


int hashFunc(std::string str, int n){

	int sum = 0;

	for(int i = n - 1; i > 0; i--){
		sum = (sum + str[n - i - 1] * power(31, i)) % PRIME;
	}
	

	return sum;
}

/*
* Function that calculates the next free position of the hashtables based on the hashvalue
* 
* @param name		name or abb of the stock  
* @param hashtable	the hashtable that is looked at
* 
* @return pos		the posisition of the stock in the hashtable
*/


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


/*
* function that is called when a new stock is added to the hashtable, it creates a new stock object and calls functions to add them to the hashtable
* 
* @param name	name of the stock
* @param wkn	wkn-number of the stock
* @param abb	abbriviation of the stock
* 
* @return aktie	the object of the class
* 
*/

Aktie* insertIntoHashtables(std::string name, std::string WKN, std::string abb){
	Aktie* aktie = new Aktie(name, WKN, abb);

	hashtableNames[calculateHashPosition(name, hashtableNames)] = aktie;
	hashtableAbb[calculateHashPosition(abb, hashtableAbb)] = aktie;
	return aktie;
}

/*
* this function calculates the position of a searched stock
* 
* @param name		name of the stock 
* @param hashtable	the hashtable that searched
* @param type		if the stock is searched with the name or the abbriviation
* 
* @return pos		the position of the stock in the hashtable
* 
*/

int searchPos(std::string name, Aktie* hashtable[], int type){
	std::string word = name;
	int pos = hashFunc(word, word.length());
	int counter = 1;

	while(1){
		if(type == 2){
			if(hashtable[pos] != nullptr && hashtable[pos]->getAbby() == word){
				return pos;
			} else if(counter == 1010){
				return -1;
			}
		} else if(counter == 1010){
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

/*
* This function gets the position based on the abb of the stock
* 
* @param abb	abb of the stock
*/

void deleteAktie(std::string abb){
	Aktie* delaktie = new Aktie();

	int posname = searchPos(abb, hashtableAbb, 2);
	if(posname == -1){
		std::cout << "Aktie wurde nicht gefunden" << std::endl;
	} else{
		std::string name = hashtableAbb[posname]->getName();
		int posabby = searchPos(name, hashtableNames, 1);

		hashtableAbb[posname]->~Aktie();
		hashtableAbb[posabby] = delaktie;
		hashtableNames[posname] = delaktie;
	}
}

/*
* gets the input of the stock from the the user and calls the search functions
* 
* @param type			differates if the stock should be searched by name(1) or by abb(2) 
* 
* @return seachpos()	the position returned by searchpos()
*/


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

/*
* gets the inputs from the user for a new stock and calls the functions needed to complete said task
*/

void addAktie(){
	std::string name = " ";
	int WKN = 0;
	std::string abb = " ";

	std::cout << "Geben Sie bitte den Namen des Unternehmens an: ";
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

	std::cout << "Geben Sie bitte zuletzt das Kurzel des Unternehmens an: ";
	std::cin >> abb;
	while(1){
		if(!cin){
			std::cout << "Bitte geben Sie nur Buchstaben an: ";
			std::cin >> abb;
		} else{
			break;
		}
	}

	Aktie* ak = insertIntoHashtables(name, to_string(WKN), abb);

	std::cout << ak->getName() << "was added to the database" std::endl;

}

/*
* Function to calcualte the number of lines in a given file   
* 
* @param string filename	the name of the file
* 
* @return int numLines		the number lines in the file
*/

int getNumLines(string filename){

	int numLines = 0;
	string line;

	ifstream input(filename);

	while(getline(input, line)){
		numLines++;
	}

	return numLines;
}

/*
* Function to fill an Aktie object with data from a file   
* 
* @param Aktie& inputAktie	reference to an Aktie object
* 
*/


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
				if (nextValue == "null") {
					Value = 0;
				}else {
					Value = stod(nextValue);
				}
				inputAktie.addValue(n, Value);
			}else{
				inputAktie.addDate(nextValue);
			}

			i++;
		}

	}



}

/*
* Function to plot the data of an Aktie object in png-file  
* 
* @param Aktie& inputAktie	reference to an Aktie object
* 
*/

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

/*
* Function to save the data from the generated hash-table in a .txtfile 
*/

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

/*
* Main funtion dispalys a menu and calls other functions based on user input
*/


int main(){

	for(int i = 0; i < 1009; i++){
		hashtableNames[i] = nullptr;
		hashtableAbb[i] = nullptr;
	}

	std::cout << "Willkommen im Aktienmaster Ihrem persoenlichen Aktien Tool!" << std::endl;
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
			std::cout << "Wollen Sie nach dem Namen(1) oder nach dem Kuerzel(2) suchen?" << std::endl;
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
			cout << "Geben sie das Kuerzel der Aktie an deren Werte Sie importieren mochten" << endl;
			cin >> kurzel;
			int pos = searchPos(kurzel, hashtableAbb, 2);
			importData(*hashtableAbb[pos]);
			cout<<"Daten aus "<<kurzel<<".csv importiert"<<endl;
			
		} else if(userinput == 4){
			string kurzel;
			cout << "Geben sie das Kuerzel der Aktie an die Sie anzeigen mochten" << endl;
			cin >> kurzel;
			int pos = searchPos(kurzel, hashtableAbb, 2);
			plotCurve(*hashtableAbb[pos]);
			cout<<"Schlusskurse in "<<kurzel<<".png dargestellt"<<endl;
			
		} else if(userinput == 5){
			std::cout << "Geben Sie das Kuerzel der Aktie die Sie loeschen wollen" << endl;
			std::string delname = " ";
			std::cin >> delname;
			deleteAktie(delname);
			cout<<"Aktie geloescht"<<endl;
		} else if(userinput == 6){
			saveToFile();
			cout<<"Daten gespeichert"<<endl;
		} else if(userinput == 7){
			loadDatabase();
			cout<<"Daten geladen"<<endl;
		} else if(userinput == 0){
			for(int i = 0; i < 1009; i++){
				if(hashtableNames[i] != nullptr){
					hashtableNames[i]->~Aktie();
				}
			}
			break;
		} else{
			std::cout << "Bitte geben Sie eine der angegebenen Zahlen ein" << std::endl;
		}
	}
}
