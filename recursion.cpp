#include<set>
#include<map>
#include<list>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#include<complex>
#include<sstream>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<numeric>
#include<utility>
#include<functional>
#include<stdio.h>
#include<assert.h>
#include<memory.h>


#define clr(v, d)           memset(v, d, sizeof(v))
#define EL printf("\n")
#define OK printf("OK")
#define pb push_back
#define mp make_pair
#define ep emplace_back
#define X  first
#define Y  second
#define fillchar(a,x) memset(a, x, sizeof(a))
#define FOR(i,l,r) for (int i=l;i<=r;i++)
#define FORD(i,r,l) for (int i=r;i>=l;i--)
using namespace std;
typedef pair<long, long> pii;
typedef long long ll;
const ll OO = 1e13;

const int base =10000000000;
typedef vector<int> BigInt;

void Set(BigInt &a) {
	while (a.size() > 1 && a.back() == 0) a.pop_back();
}

void Print(BigInt a) {
	Set(a);
	printf("%d", (a.size() == 0) ? 0 : a.back());
	FORD(i, a.size() - 2, 0) printf("%09d", a[i]); EL;
}



BigInt Integer(string s) {
	BigInt ans;
	if (s[0] == '-') return ans;
	if (s.size() == 0) { ans.pb(0); return ans; }
	while (s.size() % 9 != 0) s = '0' + s;
	for (int i = 0; i<s.size(); i += 9) {
		int v = 0;
		for (int j = i; j<i + 9; j++) v = v * 10 + (s[j] - '0');
		ans.insert(ans.begin(), v);
	}
	Set(ans);
	return ans;
}

BigInt Integer(char c[]) {
	string s = "";
	FOR(i, 0, strlen(c) - 1) s = s + c[i];
	return Integer(s);
}

BigInt Integer(ll x) {
	string s = "";
	while (x > 0) s = char(x % 10 + '0') + s, x /= 10;
	return Integer(s);
}

BigInt Integer(int x) {
	return Integer((ll)x);
}









bool operator < (BigInt a, BigInt b) {
	Set(a);
	Set(b);
	if (a.size() != b.size()) return (a.size() < b.size());
	FORD(i, a.size() - 1, 0)
		if (a[i] != b[i]) return (a[i] < b[i]);
	return false;
}

bool operator > (BigInt a, BigInt b) {
	return (b < a);
}

bool operator == (BigInt a, BigInt b) {
	return (!(a < b) && !(b < a));
}

bool operator <= (BigInt a, BigInt b) {
	return (a < b || a == b);
}

bool operator >= (BigInt a, BigInt b) {
	return (b < a || b == a);
}

bool operator < (BigInt a, int b) {
	return (a < Integer(b));
}

bool operator >(BigInt a, int b) {
	return (a > Integer(b));
}

bool operator == (BigInt a, int b) {
	return (a == Integer(b));
}

bool operator >= (BigInt a, int b) {
	return (a >= Integer(b));
}

bool operator <= (BigInt a, int b) {
	return (a <= Integer(b));
}



BigInt operator + (BigInt a, BigInt b) {
	Set(a);
	Set(b);
	BigInt ans;
	int carry = 0;
	FOR(i, 0, max(a.size(), b.size()) - 1) {
		if (i < a.size()) carry += a[i];
		if (i < b.size()) carry += b[i];
		ans.pb(carry%base);
		carry /= base;
	}
	if (carry) ans.pb(carry);
	Set(ans);
	return ans;
}

BigInt operator + (BigInt a, int b) {
	return a + Integer(b);
}

BigInt operator ++ (BigInt &a) { // ++a
	a = a + 1;
	return a;
}

void operator += (BigInt &a, BigInt b) {
	a = a + b;
}

void operator += (BigInt &a, int b) {
	a = a + b;
}




BigInt operator - (BigInt a, BigInt b) {
	Set(a);
	Set(b);
	BigInt ans;
	int carry = 0;
	FOR(i, 0, a.size() - 1) {
		carry += a[i] - (i < b.size() ? b[i] : 0);
		if (carry < 0) ans.pb(carry + base), carry = -1;
		else ans.pb(carry), carry = 0;
	}
	Set(ans);
	return ans;
}

BigInt operator - (BigInt a, int b) {
	return a - Integer(b);
}

void operator -- (BigInt &a) { // --a
	a = a - 1;
}

void operator -= (BigInt &a, BigInt b) {
	a = a + b;
}

void operator -= (BigInt &a, int b) {
	a = a - b;
}



int operator % (BigInt a, int b) {
	Set(a);
	if (b == 0) return -1;
	int ans = 0;
	FORD(i, a.size() - 1, 0)
		ans = (ans*(base%b) + a[i] % b) % b;
	return ans;
}

BigInt operator / (BigInt a, int b) {
	Set(a);
	BigInt ans;
	ll cur = 0ll;
	FORD(i, a.size() - 1, 0) {
		cur = (cur*(ll)base + (ll)a[i]);
		ans.insert(ans.begin(), cur / b);
		cur %= b;
	}
	Set(ans);
	return ans;
}



struct s
{
	long long x, y;
};
long long gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }
long mod(long a, long b) { return (a%b + b) % b; }


bool isPowerOfTwo(BigInt x)
{
	while (((x % 2) == 0) && x > 1)
		x = x / 2;
	return (x == 1);
}
BigInt odd(BigInt i);
BigInt even(BigInt i);

map<BigInt, BigInt>mymap;
int main()
{
	while (true)
	{
		//Taking String input 
		string s;
		cin >> s;
		// Make Function to take String and cut it to integers 
		BigInt n = Integer(s);
		// Check if it is Power of 2 or not 
		//made function Power of 2 because easy for us if it is pwer of 2 
		// will return 
		if (!isPowerOfTwo(n) && n % 2 == 0)
		{
			BigInt result = odd(n / 2);
			Print(result);
		}
		else if (n % 2 == 0)
			Print(even(n));
		else
			Print(odd(n));
	}
}

BigInt odd(BigInt i)
{
	//Print(i);
	BigInt l = Integer("1");
	if (i == 1 || i == 0)
		return l;


	if (mymap.count(i))
		return mymap[i];

	BigInt res;
	BigInt n1 = (i - 1) / 2;
	if (n1 % 2 == 0)
		res = odd(n1 - 1) + even(n1);
	else
		res = even(n1 - 1) + odd(n1);

	return mymap[i] = res;


}

BigInt even(BigInt i)
{
	//Print(i);

	BigInt l = Integer("1");

	if (i == 0 || isPowerOfTwo(i))
		return l;


	if (mymap.count(i))
		return mymap[i];

	BigInt res = odd(i / 2);


	return mymap[i] = res;

}

