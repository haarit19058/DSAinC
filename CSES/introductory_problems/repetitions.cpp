#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // TODO: solve "Repetitions"
	string s;
	cin>>s;
	int ans = 1;
	int maxAns = 1;
	for(int i = 1;i<s.size();i++){
		if(s[i] == s[i-1]){
			ans++;
		}
		else{
			maxAns = max(ans,maxAns);
			ans = 1;
		}
	}
	maxAns = max(ans,maxAns);

	cout<<maxAns<<endl;

    return 0;
}
