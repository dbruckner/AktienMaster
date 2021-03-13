#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include "aktie.h";

#define PRIME 1009;


aktie* hashtable[1008];



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


int main() {
    std::string key = "AAPL";

    std::cout << hashFunc(key, key.length());

}