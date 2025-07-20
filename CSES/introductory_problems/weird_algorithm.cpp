#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Weird Algorithm"
	long long n;
	cin>>n;

	while(n!=1){
		cout<<n<<" ";
		if(n&1){
			n = 3*n + 1;
		}
		else{
			n = n/2;
		}
	}
	cout<<1<<endl;

    return 0;
}
