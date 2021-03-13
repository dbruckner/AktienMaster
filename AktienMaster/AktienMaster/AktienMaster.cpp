#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>

#define PRIME 1009;


std::string hashtable[1009];



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

void fillhashtable(int arr) {
    std::string word = "";


    for (int i = 0; i < 1009; i++) {
        hashtable[i] = "no";
    }


    for (int i = 0; i < arr; i++) {
        for (int j = 0; j < 4; j++) {
            int random = rand() % 26 + 97;
            char letter = char(random);
            word.push_back(letter);
        }

        int pos = hashFunc(word, word.length());
        if (hashtable[pos] != "no") {
            int counter = 1;
            while (1) {
                pos += (counter * counter);
                pos %= PRIME;
                
                if (hashtable[pos] == "no") {
                    break;
                }
                counter++;
            }
        }
        hashtable[pos] = word;
        word = "";
    }
}



int main() {
    time_t t;
    srand((unsigned)time(&t));
    
    fillhashtable(1009);
    int counter = 0;
    for (int i = 0; i < 1009; i++) {
        if (hashtable[i] != "no") {
            counter++;
            std::cout << counter << ".) Slot " << i << " is " << hashtable[i] << std::endl;
        }
    }


    //std::cout << hashFunc(key, key.length());

}