#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Removing Digits"
    int n;
    cin>>n;

    int steps = 0;
    while(n>0){
        steps++;
        // find teh largest digit
        int temp = n;
        int largest = 0;
        while(temp>0){
            int dig = temp % 10;
            largest = max(largest,dig);
            temp = temp/10;
        }
        n = n - largest;
    }

    cout<<steps<<endl;

    return 0;
}
