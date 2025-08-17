/*
time complexity if O(log n). n = min(a,b);

Worst Case scenario
The numbers are consecutive fibonacci

gcd(F(k+1), F(k)) → gcd(F(k), F(k-1)) → gcd(F(k-1), F(k-2)) → ... → gcd(1, 0)


F(k) ≈ φ^k / √5   where φ ≈ 1.618 (golden ratio)
F(k) ≤ n  ⟹  k = O(log n)


Why is this the worst case ??
We want the smallest decrease in consecutive calls. a = bq + r. r must be larger. or a%b is almost as large as b or a = b + something small.

- F(n+1) = F(n) + F(n-1)
- so F(n+1) % F(n) = F(n-1)
*/

int gcd(int a, int b){
	if(b == 0)return a;
	return gcd(b,a % b);
}
