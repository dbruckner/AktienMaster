#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include "type.h"
#include "aktie.h"

#define PRIME 1009;


Aktie* hashtableNames[1009];
Aktie* hashtableAbb[1009];



int power(int base, int exponent) {
	if (exponent == 0) {
		return 1;
	}
	return base * power(base, exponent - 1);
}

int hashFunc(std::string str, int n) {

	int sum = 0;

	for (int i = n - 1; i > 0; i--) {
		sum = (sum + str[n - i - 1] * power(31, i)) % PRIME;
	}

	return sum;
}

int calculateHashPosition(std::string name, Aktie* hashtable[]) {
	int pos = 0;
	pos = hashFunc(name, name.length());


	if (hashtable[pos] != nullptr) {
		int counter = 1;
		while (1) {
			pos += (counter * counter);
			pos %= PRIME;

			if (hashtable[pos] == nullptr || hashtable[pos]->getName() == "deleted") {
				if (hashtable[pos] != nullptr) {
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

void insertIntoHashtables(std::string name, std::string WKN ,std::string abb) {
	Aktie* aktie = new Aktie(name, WKN, abb);

	hashtableNames[calculateHashPosition(name, hashtableNames)] = aktie;
	hashtableAbb[calculateHashPosition(abb, hashtableAbb)] = aktie;

}


int searchPos(std::string name, Aktie* hashtable[], int type) {
	std::string word = name;
	int pos = hashFunc(word, word.length());
	int counter = 1;
		
	while (1) {
		if (type == 2) {
			if (hashtable[pos] != nullptr && hashtable[pos]->getAbby() == word) {
				return pos;
			}
		}else {
			if (hashtable[pos] != nullptr && hashtable[pos]->getName() == word) {
				return pos;
			}
		}
		
		pos += (counter * counter);
		pos %= PRIME;
			
	}
}

void deleteAktie(std::string name) {
	Aktie* delaktie = new Aktie();

	int posname = searchPos(name, hashtableNames, 1);
	std::string abb = hashtableNames[posname]->getAbby();
	int posabby = searchPos(abb, hashtableAbb, 2);

	hashtableNames[posname]->~Aktie();
	hashtableAbb[posabby] = delaktie;
	hashtableNames[posname] = delaktie;
}


int searchAktie(int type) {
	std::string userinput = " ";

	std::cout << "Geben Sie die Information der Aktie ein die Sie suchen: ";
	std::cin >> userinput;
	if (type == 2) {
		
		return searchPos(userinput, hashtableAbb, type);
	}else {
		return searchPos(userinput, hashtableNames, type);
	}
}


void addAktie() {
	std::string name = " ";
	int WKN = 0;
	std::string abb = " ";

	std::cout << "Geben Sie bitte den Namen der Firma an: ";
	std::cin >> name;
	while (1) {
		if (!cin) {
			std::cout << "Bitte geben Sie nur Buchstaben an: ";
			std::cin >> name;
		}
		else {
			break;
		}
	}

	std::cout << "Geben Sie bitte nun die WKN an:";
	std::cin >> WKN;
	while (1) {
		if (!cin) {
			std::cout << "Bitte geben Sie nur Zahlen an: ";
			std::cin >> WKN;
		}
		else {
			break;
		}
	}

	std::cout << "Geben Sie nun bitte zuletzt die abb der von Ihnen ausgewählten Firma an: ";
	std::cin >> abb;
	while (1) {
		if (!cin) {
			std::cout << "Bitte geben Sie nur Buchstaben an: ";
			std::cin >> abb;
		}
		else {
			break;
		}
	}

	insertIntoHashtables(name, to_string(WKN) ,abb);
}


int main() {

	for (int i = 0; i < 1009; i++) {
		hashtableNames[i] = nullptr;
		hashtableAbb[i] = nullptr;
	}

	std::cout << "Willkommen im Aktienmaster Ihr persöhnliches Aktien Tool!" << std::endl;
	int userinput;

	while (1) {
		std::cout << "Was wollen sie tun?" << std::endl << "(1) neue Aktien hinzufügen (2) einen Index suchen (3+) in Progress..: ";
		std::cin >> userinput;
		if (userinput == 1) {
			addAktie();
		}
		else if(userinput == 2) {
			std::cout << "Wollen Sie nach dem Namen oder nach der Abb suchen? (1) nach dem Namen (2) nach der Abb" << std::endl;
			int type = 0;
			int pos = 0;
			std::cin >> type;
			if (type == 2) {
				pos = searchAktie(type);
			}else {
				pos = searchAktie(type);
			}
			if (pos != -1 && type == 1) {
				hashtableNames[pos]->printAktie();
			}else if(pos != -1 && type == 2){
				hashtableAbb[pos]->printAktie();
			}else {
				std::cout << "Aktie kann nicht gefunden werden" << std::endl;
			}
		}
		else {
			std::cout << "Bitte geben Sie einen der angeführten Nummern ein" << std::endl;
		}
	}
}