#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;
    return gcd(b, a%b);
}

long long mul(long long a, long long b, long long m)
{
    if (b == 1)
        return a;
    if (b%2 == 0)
    {
        long long t = mul(a, b/2, m);
        return (2 * t) % m;
    }
    return (mul(a, b-1, m) + a) % m;
}

long long pows(long long a, long long b, long long m){
    if (b == 0)
        return 1;
    if (b%2 == 0)
    {
        long long t = pows(a, b/2, m);
        return mul(t , t, m) % m;
    }  
    return (mul(pows(a, b-1, m) , a, m)) % m;
}

bool fermat(long long x)
{
    if (x == 2)
        return true;
    srand(time(NULL));
    for (int i = 0;i < 100; ++i)
    {
        long long a = (rand() % (x - 2)) + 2;
        if (gcd(a, x) != 1)
            return false;   
        if( pows(a, x-1, x) != 1)  
            return false;   
    }
    return true;
}

int main() {
    vector<long long> a;
    for (int i = 2; i < 35; ++i)
    {
        if (fermat(i))
        {
            a.push_back(pow(2,i)-1);
        }
    }
    int index_to_a = 1;
    cout << "List of Mersenn Numbers" << endl;
    for (int n : a)
        cout << index_to_a++ << ") " << n << '\n';
    return 0;
}
