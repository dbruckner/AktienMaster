#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include "type.h"
#include "aktie.h"

#define PRIME 1009;


Aktie* hashtable[1009];
Aktie* hashtableabb[1009];



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

int calculatePos(std::string name) {
	std::string word = name;

	int pos = hashFunc(word, word.length());
	pos %= PRIME;
	if (hashtable[pos] != nullptr) {
		int counter = 1;
		while (1) {
			pos += (counter * counter);
			pos %= PRIME;

			if (hashtable[pos] == nullptr) {
				break;
			}
			
			counter++;
		}
	}
	return pos;

}

int searchPos(std::string name) {
	std::string word = name;
	int pos = hashFunc(word, word.length());
	pos %= PRIME;
	int counter = 1;
	if (hashtable[pos] != nullptr && hashtable[pos]->getName() == word) {
		return pos;
	}else {
		while (1) {
			pos += (counter * counter);
			pos %= PRIME;
			if (hashtable[pos] != nullptr && hashtable[pos]->getName() == word) {
				return pos;
			}
			else if (counter == 1000) {
				return -1;
			}
			
		}
	}

}


int searchAktie() {
	std::string userinput = " ";

	std::cout << "Geben Sie den Namen der Aktie ein die Sie suchen: ";
	std::cin >> userinput;

	return searchPos(userinput);
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
	std::cout << "Geben Sie nun bitte zuletzt die abb der von Ihnen ausgew�hlten Firma an: ";
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

	Aktie* aktie = new Aktie(name, to_string(WKN), abb);
	hashtable[calculatePos(name)] = aktie;

}



int main() {

	for (int i = 0; i < 1009; i++) {
		hashtable[i] = nullptr;
	}
	time_t t;
	srand((unsigned)time(&t));

	std::cout << "Willkommen im Aktienmaster Ihr pers�hnliches Aktien Tool!" << std::endl;
	int userinput;

	while (1) {
		std::cout << "Was wollen sie tun?" << std::endl << "(1) neue Aktien hinzuf�gen (2) einen Index suchen (3+) in Progress..: ";
		std::cin >> userinput;
		if (userinput == 1) {
			addAktie();
		}
		else if(userinput == 2) {
			int pos = searchAktie();
			if (pos != -1) {
				hashtable[pos]->printAktie();
			}
			else {
				std::cout << "Aktie kann nicht gefunden werden" << std::endl;
			}
		}
		else {
			std::cout << "Bitte geben Sie einen der angef�hrten Nummern ein" << std::endl;
		}
	}


	//fillhashtable(1009);





	/*
	for (int i = 0; i < 1009; i++) {
		if (hashtable[i] != "no") {
			counter++;
			std::cout << counter << ".) Slot " << i << " is " << hashtable[i] << std::endl;
		}
	}







	Aktie test("TEST", "123", "TST");

	test.addValue(high, 5.1);

	test.printVector(high);

	test.printAktie();
	*/


	//std::cout << hashFunc(key, key.length());

}