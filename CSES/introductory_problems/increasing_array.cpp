#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Increasing Array"
	int n;
	cin>>n;

	int curr = -1;
	int prev = -1;
	long long ans = 0;
	cin>>prev;
	for(int i = 1;i<n;i++){
		cin>>curr;
		if(curr<prev){
			ans+=prev-curr;
			curr = prev;
		}
		prev = curr;

	}

	cout << ans << endl;
    return 0;
}
