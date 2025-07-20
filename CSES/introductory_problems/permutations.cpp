#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Permutations"
	
	int n;
	cin >> n;

	if(n == 2 || n == 3){
		cout<<"NO SOLUTION\n";
		return 0;
	}
	if(n == 4){
		cout<<"2 4 1 3\n";
		return 0;
	}

	for(int i = 1;i<=n;i+=2){
		cout<<i<<" ";
	}

	for(int i = 2;i<=n;i+=2){
		cout<<i<<" ";
	}

	cout<<endl;

    return 0;
}




/*
 *
 *
 * #include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;

    if (n == 1) {
        cout << "1\n";
    } else if (n == 2 || n == 3) {
        cout << "NO SOLUTION\n";
    } else {
        // Print even numbers first
        for (int i = 2; i <= n; i += 2) {
            cout << i << " ";
        }
        // Then print odd numbers
        for (int i = 1; i <= n; i += 2) {
            cout << i << " ";
        }
        cout << "\n";
    }

    return 0;
}

 *
 *
 *
 */
