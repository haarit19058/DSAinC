#include <iostream>

using namespace std;

using ll = long long;

vector<int> primeFactorsNaive(ll n){
	/*
		return all prime factors of a number in O(root(n));
		for n numbers it needs O(nroot(n));
	 */

	vector<int> ans;
	for(int i = 2;i * i < n;i++){
		if(n%i==0)ans.push_back(i);
		while(n%i == 0){
			n = n/i;
		}
	}
	return ans;
}

int main(){
	
	

	return 0;
}
