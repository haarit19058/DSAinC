#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

const int N = 1e7 + 5;

void classicSieve(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
}

void linearSieve(int n) {
    vector<int> spf(n + 1);
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > spf[i] || i * p > n) break;
            spf[i * p] = p;
        }
    }
}

int main() {
    int n = 1e9;

    auto start = high_resolution_clock::now();
    classicSieve(n);
    auto end = high_resolution_clock::now();
    cout << "Classic Sieve Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    linearSieve(n);
    end = high_resolution_clock::now();
    cout << "Linear Sieve Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}

