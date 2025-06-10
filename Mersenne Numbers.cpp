#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;


// to define class, I used this article: https://habr.com/ru/articles/172285/
class big_integer {
    // radix (1 000 000 000)
    static const int BASE = 1000000000;
 
    // internal storage for big_integer number
    std::vector<int> _digits;
 
    // sign
    bool _is_negative;
 
    void _remove_leading_zeros();
    void _shift_right();
 
public:
    // class exception thrown when dividing by zero
    class divide_by_zero: public std::exception {  };
 
    big_integer();
    big_integer(std::string);
    big_integer(signed char);
    big_integer(unsigned char);
    big_integer(signed short);
    big_integer(unsigned short);
    big_integer(signed int);
    big_integer(unsigned int);
    big_integer(signed long);
    big_integer(unsigned long);
    big_integer(signed long long);
    big_integer(unsigned long long);
 
    friend std::ostream& operator <<(std::ostream&, const big_integer&);
    operator std::string() const;
    const big_integer operator +() const;
    const big_integer operator -() const;
    const big_integer operator ++();
    const big_integer operator ++(int);
    const big_integer operator --();
    const big_integer operator --(int);
    friend bool operator ==(const big_integer&, const big_integer&);
    friend bool operator <(const big_integer&, const big_integer&);
    friend bool operator !=(const big_integer&, const big_integer&);
    friend bool operator <=(const big_integer&, const big_integer&);
    friend bool operator >(const big_integer&, const big_integer&);
    friend bool operator >=(const big_integer&, const big_integer&);
    friend const big_integer operator +(big_integer, const big_integer&);
    big_integer& operator +=(const big_integer&);
    friend const big_integer operator -(big_integer, const big_integer&);
    big_integer& operator -=(const big_integer&);
    friend const big_integer operator *(const big_integer&, const big_integer&);
    big_integer& operator *=(const big_integer&);
    friend const big_integer operator /(const big_integer&, const big_integer&);
    big_integer& operator /=(const big_integer&);
    friend const big_integer operator %(const big_integer&, const big_integer&);
    big_integer& operator %=(const big_integer&);
 
    bool odd() const;
    bool even() const;
    const big_integer pow(big_integer) const;
};
 
// creates a long integer with value 0
big_integer::big_integer() {
    this->_is_negative = false;
}
 
// creates a long integer from a C++ string
big_integer::big_integer(std::string str) {
    if (str.length() == 0) {
        this->_is_negative = false;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->_is_negative = true;
        }
        else {
            this->_is_negative = false;
        }
 
        for (long long i = str.length(); i > 0; i -= 9) {
            if (i < 9)
                this->_digits.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->_digits.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }
 
        this->_remove_leading_zeros();
    }
}
 
// removes leading zeros
void big_integer::_remove_leading_zeros() {
    while (this->_digits.size() > 1 && this->_digits.back() == 0) {
        this->_digits.pop_back();
    }
 
    if (this->_digits.size() == 1 && this->_digits[0] == 0) this->_is_negative = false;
}
 
// prints a number to the output stream
std::ostream& operator <<(std::ostream& os, const big_integer& bi) {
    if (bi._digits.empty()) os << 0;
    else {
        if (bi._is_negative) os << '-';
        os << bi._digits.back();
        char old_fill = os.fill('0');
        for (long long i = static_cast<long long>(bi._digits.size()) - 2; i >= 0; --i) os << std::setw(9) << bi._digits[i];
        os.fill(old_fill);
    }
 
    return os;
}
 
// compares two numbers for equality
bool operator ==(const big_integer& left, const big_integer& right) {
    if (left._is_negative != right._is_negative) return false;
    if (left._digits.empty()) {
        if (right._digits.empty() || (right._digits.size() == 1 && right._digits[0] == 0)) return true;
        else return false;
    }
    
    if (right._digits.empty()) {
        if (left._digits.size() == 1 && left._digits[0] == 0) return true;
        else return false;
    }
 
    if (left._digits.size() != right._digits.size()) return false;
    for (size_t i = 0; i < left._digits.size(); ++i) if (left._digits[i] != right._digits[i]) return false;
 
    return true;
}
 
// returns a copy of the passed number
const big_integer big_integer::operator +() const {
    return big_integer(*this);
}
 
// returns the passed number with a different sign
const big_integer big_integer::operator -() const {
    big_integer copy(*this);
    copy._is_negative = !copy._is_negative;
    return copy;
}
 
// checks if the left operand is less than the right operand
bool operator <(const big_integer& left, const big_integer& right) {
    if (left == right) return false;
    if (left._is_negative) {
        if (right._is_negative) return ((-right) < (-left));
        else return true;
    }
    else if (right._is_negative) return false;
    else {
        if (left._digits.size() != right._digits.size()) {
            return left._digits.size() < right._digits.size();
        }
        else {
            for (long long i = left._digits.size() - 1; i >= 0; --i) {
                if (left._digits[i] != right._digits[i]) return left._digits[i] < right._digits[i];
            }
            
            return false;
        }
    }
}
 
// compares two numbers for inequality
bool operator !=(const big_integer& left, const big_integer& right) {
    return !(left == right);
}
 
// checks if the left operand is less than or equal to the right operand
bool operator <=(const big_integer& left, const big_integer& right) {
    return (left < right || left == right);
}
 
// checks if the left operand is greater than the right operand
bool operator >(const big_integer& left, const big_integer& right) {
    return !(left <= right);
}
 
// checks if the left operand is greater than or equal to the right operand
bool operator >=(const big_integer& left, const big_integer& right) {
    return !(left < right);
}
 
// adds two numbers
const big_integer operator +(big_integer left, const big_integer& right) {
    if (left._is_negative) {
        if (right._is_negative) return -(-left + (-right));
        else return right - (-left);
    }
    else if (right._is_negative) return left - (-right);
    int carry = 0;
    for (size_t i = 0; i < std::max(left._digits.size(), right._digits.size()) || carry != 0; ++i) {
        if (i == left._digits.size()) left._digits.push_back(0);
        left._digits[i] += carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] >= big_integer::BASE;
        if (carry != 0) left._digits[i] -= big_integer::BASE;
    }
 
    return left;
}
 
// adds a new number to the current number
big_integer& big_integer::operator +=(const big_integer& value) {
    return *this = (*this + value);
}
 
// prefix increment
const big_integer big_integer::operator++() {
    return (*this += 1);
}
 
// converts a number to a string
big_integer::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}
 
// converts signed char to big_integer
big_integer::big_integer(signed char c) {
    if (c < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(c));
}
 
// converts unsigned char to big_integer
big_integer::big_integer(unsigned char c) {
    this->_is_negative = false;
    this->_digits.push_back(c);
}
 
// converts signed short to big_integer
big_integer::big_integer(signed short s) {
    if (s < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(s));
}
 
// converts unsigned short to big_integer
big_integer::big_integer(unsigned short s) {
    this->_is_negative = false;
    this->_digits.push_back(s);
}
 
// converts signed int to big_integer
big_integer::big_integer(signed int i) {
    if (i < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(i) % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(std::abs(i));
}
 
// converts unsigned int to big_integer
big_integer::big_integer(unsigned int i) {
    this->_digits.push_back(i % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(i);
}
 
// converts signed long to big_integer
big_integer::big_integer(signed long l) {
    if (l < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(l) % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(std::abs(l));
}
 
// converts unsigned long to big_integer
big_integer::big_integer(unsigned long l) {
    this->_digits.push_back(l % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(l);
}
 
// converts signed long long to big_integer
big_integer::big_integer(signed long long l) {
    if (l < 0) { this->_is_negative = true; l = -l; }
    else this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}
 
// converts unsigned long long to big_integer
big_integer::big_integer(unsigned long long l) {
    this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}
 
// postfix increment
const big_integer big_integer::operator ++(int) {
    *this += 1;
    return *this - 1;
}
 
// prefix decrement
const big_integer big_integer::operator --() {
    return *this -= 1;
}
 
// prepostfix decrement
const big_integer big_integer::operator --(int) {
    *this -= 1;
    return *this + 1;
}
 
// subtracts two numbers
const big_integer operator -(big_integer left, const big_integer& right) {
    if (right._is_negative) return left + (-right);
    else if (left._is_negative) return -(-left + right);
    else if (left < right) return -(right - left);
    int carry = 0;
    for (size_t i = 0; i < right._digits.size() || carry != 0; ++i) {
        left._digits[i] -= carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] < 0;
        if (carry != 0) left._digits[i] += big_integer::BASE;
    }
    left._remove_leading_zeros();
    return left;
}
 
// subtracts a new number from the current number
big_integer& big_integer::operator -=(const big_integer& value) {
    return *this = (*this - value);
}
 
// multiplies two numbers
const big_integer operator *(const big_integer& left, const big_integer& right) {
    big_integer result;
    result._digits.resize(left._digits.size() + right._digits.size());
    for (size_t i = 0; i < left._digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right._digits.size() || carry != 0; ++j) {
            long long cur = result._digits[i + j] + 
                left._digits[i] * 1LL * (j < right._digits.size() ? right._digits[j] : 0) + carry;
            result._digits[i + j] = static_cast<int>(cur % big_integer::BASE);
            carry = static_cast<int>(cur / big_integer::BASE);
        }
    }
 
    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}
 
// multiplies the current number by the specified number
big_integer& big_integer::operator *=(const big_integer& value) {
    return *this = (*this * value);
}
 
// shifts all digits to the right by 1 (multiplies by BASE)
void big_integer::_shift_right() {
    if (this->_digits.size() == 0) {
        this->_digits.push_back(0);
        return;
    }
    this->_digits.push_back(this->_digits[this->_digits.size() - 1]);
    for (size_t i = this->_digits.size() - 2; i > 0; --i) this->_digits[i] = this->_digits[i - 1];
    this->_digits[0] = 0;
}
 
// divides two numbers
const big_integer operator /(const big_integer& left, const big_integer& right) {
    if (right == 0) throw big_integer::divide_by_zero();
    big_integer b = right;
    b._is_negative = false;
    big_integer result, current;
    result._digits.resize(left._digits.size());
    for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i) {
        current._shift_right();
        current._digits[0] = left._digits[i];
        current._remove_leading_zeros();
        int x = 0, l = 0, r = big_integer::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            big_integer t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        result._digits[i] = x;
        current = current - b * x;
    }
    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}
 
// divides the current number by the specified number
big_integer& big_integer::operator /=(const big_integer& value) {
    return *this = (*this / value);
}
 
// returns the remainder from dividing two numbers
const big_integer operator %(const big_integer& left, const big_integer& right) {
    big_integer result = left - (left / right) * right;
    if (result._is_negative) result += right;
    return result;
}
 
// assigns the current number the remainder of the division by another number
big_integer& big_integer::operator %=(const big_integer& value) {
    return *this = (*this % value);
}
 
// checks if the current number is odd
bool big_integer::odd() const {
    if (this->_digits.size() == 0) return false;
    return this->_digits[0] & 1;
}
 
// checks if the current number is even
bool big_integer::even() const {
    return !this->odd();
}
 
// raises the current number to the specified power
const big_integer big_integer::pow(big_integer n) const {
    big_integer a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result *= a;
        a *= a;
        n /= 2;
    }
    return result;
}

// 

big_integer gcd(big_integer a, big_integer b)
{
    if (b == 0)
        return a;
    return gcd(b, a%b);
}

big_integer mul(big_integer a, big_integer b, big_integer m)
{
    if (b == 1)
        return a;
    if (b%2 == 0)
    {
        big_integer t = mul(a, b/2, m);
        return (2 * t) % m;
    }
    return (mul(a, b-1, m) + a) % m;
}

big_integer pows(big_integer first_op, big_integer second_op, big_integer m){
    if (second_op == 0)
        return 1;
    if (second_op%2 == 0)
    {
        big_integer t = pows(first_op, second_op/2, m);
        return mul(t , t, m) % m;
    }  
    return (mul(pows(first_op, second_op-1, m), first_op, m)) % m;
}

bool fermat(big_integer x)
{
    if (x == 2)
        return true;
    srand(time(NULL));
    for (int i = 0; i < 100; ++i)
    {
        big_integer a = (rand() % (x - 2)) + 2;
        if (gcd(a, x) != 1)
            return false;   
        if( pows(a, x-1, x) != 1)  
            return false;   
    }
    return true;
}


int main() {
    /** Sample test for big_integers class 
	  big_integer bi("2"), bi2 = 100;
	  cout << bi.pow(1000) << endl;
	  big_integer f = 1;
	  for (big_integer i = 2; i <= bi2; i+=1) f *= i;
	    cout << f << endl;
    **/
    big_integer base_2 = 2;
    cout << "List of Mersenn Numbers" << endl;
    int index_to_a = 1;
    for (big_integer i = 2; i < 1000; ++i)
    {
        if (fermat(i))
        {
            cout << index_to_a++ << ") " << i << " | " << base_2.pow(i)-1 << '\n';
        }
    }
    return 0;
}
