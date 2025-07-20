#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Missing Number"
	
	long long n;
	cin>>n;
	long long ans = ((n)*(n+1))/2;
	for(int i = 0;i<n-1;i++){
		int temp;
		cin>>temp;
		ans-=temp;
	}
	cout<<ans<<endl;

    return 0;
}
