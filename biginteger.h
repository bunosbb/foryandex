#include <iostream>
#include <vector>
#include <string>

class BigInteger;
const BigInteger operator / (const BigInteger& a, const BigInteger& b);
const BigInteger operator - (const BigInteger& a, const BigInteger& b);
const BigInteger operator * (const BigInteger& a, const BigInteger& b);
std::istream& operator >> (std::istream& S, BigInteger& a);
std::ostream& operator << (std::ostream& S, const BigInteger& a);
class BigInteger {
private:
    std::vector <int> _digit;
    bool _subzero;
    void _add(const size_t& a) {
        while (_digit.size() <= a) {
            _digit.push_back(0);
        }
        return;
    }
    void _clean() {
        while (_digit.size() > 1 && _digit.back() == 0) {
            _digit.pop_back();
        }
        if (_digit.size() == 1 && _digit[0] == 0)
            _subzero = false;
    }
    const BigInteger _multiplyTen(const size_t& a) const {
        BigInteger ans;
        ans._digit.clear();
        for (size_t i = 0; i < a; ++i)
            ans._digit.push_back(0);
        for (size_t i = 0; i < _digit.size(); ++i)
            ans._digit.push_back(_digit[i]);
        ans._subzero = false;
        return ans;
    }
public:
    const BigInteger absolute () const {
        BigInteger ans = *this;
        ans._subzero = false;
        return ans;
    }
    BigInteger () {
        _subzero = false;
        _digit.push_back(0);
    }
    BigInteger (const int& b) {
        _subzero = false;
        int a = b;
        if (a < 0) {
            a = -a;
            _subzero = true;
        }
        _digit.clear();
        while (a > 0) {
            _digit.push_back(a % 10);
            a /= 10;
        }
        if (_digit.size() == 0) {
            _digit.push_back(0);
        }
    }
    BigInteger (const BigInteger& a) {
        _digit = a._digit;
        _subzero = a._subzero;
    }
    explicit operator bool () const {
        return !(_digit.size() == 1 && _digit[0] == 0);
    }
    std::string toString() const {
        std::string _s;
        if (_subzero)
            _s = "-";
        for (size_t i = _digit.size() - 1; i + 1 != 0; i--)
            _s += '0' + _digit[i];
        return _s;
    }
    void fromString(const std::string& snumber) {
        _digit.clear();
        _subzero = false;
        for (size_t i = 0; i < snumber.size(); ++i) {
            if (snumber[snumber.size() - i - 1] != '-') {
                _add(i);
                _digit[i] = (snumber[snumber.size() - i - 1] - '0');
            } else
                _subzero = true;
        }
    }
    BigInteger& operator = (const BigInteger& a) {
        _digit = a._digit;
        _subzero = a._subzero;
        return *this;
    }
    bool operator == (const BigInteger& a) const {
        return _digit == a._digit && !(_subzero - a._subzero);
    }
    bool operator != (const BigInteger& a) const {
        return !(_digit == a._digit && !(_subzero - a._subzero));
    }
    bool operator > (const BigInteger& a) const {
        if (_subzero && !a._subzero)
            return false;
        if (!_subzero && a._subzero)
            return true;
        bool bigger;
        if (_digit.size() > a._digit.size())
            bigger = true;
        if (_digit.size() < a._digit.size())
            bigger = false;
        if (_digit.size() == a._digit.size()) {
            if (*this == a)
                return false;
            for (size_t i = _digit.size() - 1; i + 1 != 0; i--) {
                if (_digit[i] != a._digit[i]) {
                    bigger = _digit[i] > a._digit[i];
                    break;
                }
            }
        }
        return (_subzero && a._subzero) ^ bigger;
    }
    bool operator >= (const BigInteger& a) const {
        return *this > a || *this == a;
    }
    bool operator < (const BigInteger& a) const {
        return !(*this >= a);
    }
    bool operator <= (const BigInteger& a) const {
        return !(*this > a);
    }
    const BigInteger operator - () const {
        BigInteger ans = *this;
        ans._subzero = !ans._subzero;
        ans._clean();
        return ans;
    }
    BigInteger& operator += (const BigInteger& a) {
        BigInteger aa = a;
        if (_subzero == aa._subzero) {
            for (size_t i = 0; i < _digit.size() || i < aa._digit.size(); ++i) {
                if (aa._digit.size() > i)
                    _add(i);
                if (aa._digit.size() > i)
                    _digit[i] += aa._digit[i];
                if (_digit[i] > 9) {
                    _add(i + 1);
                    _digit[i + 1] += _digit[i] / 10;
                    _digit[i] %= 10;
                } else {
                    if (i >= aa._digit.size())
                        break;
                }
            }
        } else {
            BigInteger b;
            if (absolute() < a.absolute()) {
                b = *this;
                *this = a;
            } else
                b = a;
            for (size_t i = 0; i < _digit.size(); ++i) {
                if (b._digit.size() > i)
                    _digit[i] -= b._digit[i];
                if (_digit[i] < 0) {
                    _add(i + 1);
                    _digit[i + 1]--;
                    _digit[i] += 10;
                } else {
                    if (i >= aa._digit.size())
                        break;
                }
            }
        }
        _clean();
        return *this;
    }
    BigInteger& operator -= (const BigInteger& a) {
        return *this += -a;
    }
    BigInteger& operator *= (const BigInteger& a) {
        BigInteger res = 0;
        if (a == (BigInteger)1) 
            return *this;
        if (*this == (BigInteger)1) 
            return *this = a;
        for (size_t i = 0; i < _digit.size(); ++i) {
            if (_digit[i]) {
                for (size_t j = 0; j < a._digit.size(); ++j) {
                    res._add(i + j);
                    res._digit[i + j] += _digit[i] * a._digit[j];
                }
            }
        }
        for (size_t i = 0; i < res._digit.size(); ++i) {
            if (res._digit[i] > 9) {
                res._add(i + 1);
                res._digit[i + 1] += res._digit[i] / 10;
                res._digit[i] %= 10;
            }
        }
        res._subzero = _subzero ^ a._subzero;
        res._clean();
        return *this = res;
    }
    BigInteger& operator ++ () {
        return *this += 1;
    }
    const BigInteger operator ++ (int) {
        BigInteger ans = *this;
        ++*this;
        return ans;
    }
    BigInteger& operator -- () {
        return *this -= 1;
    }
    const BigInteger operator -- (int) {
        BigInteger ans = *this;
        --*this;
        return ans;
    }
    BigInteger& operator /= (const BigInteger& a) {
        if (a == 0)
            return *this;
        BigInteger ans = 0;
        BigInteger one = 1;
        bool kk = _subzero ^ a._subzero;
        *this = absolute();
        for (size_t i = _digit.size() - a._digit.size(); _digit.size() >= a._digit.size() && i + 1 != 0; --i) {
            one = a.absolute()._multiplyTen(i);
            while (*this >= one) {
                *this -= one;
                ans._add(i);
                ans._digit[i]++;
            }
        }
        ans._subzero = kk;
        ans._clean();
        return *this = ans;
    }
    const BigInteger& operator %= (const BigInteger& a) {
        return *this = *this - (*this / a) * a;
    }
};

std::istream& operator >> (std::istream& in, BigInteger& a) {
    std::string snumber = "";
    while (in.peek() != '-' && (in.peek() < '0' || in.peek() > '9')) {
        in.get();
    }
    std::string s = "";
    if (in.peek() == '-')
        snumber += in.get();
    while (in.peek() >= '0' && in.peek() <= '9') {
        snumber += in.get();
    }
    a.fromString(snumber);
    return in;
}
std::ostream& operator << (std::ostream& out, const BigInteger& a) {
    out << a.toString();
    return out;
}

const BigInteger operator + (const BigInteger& a, const BigInteger& b) {
    BigInteger ans = a;
    return (ans += b);
}
const BigInteger operator - (const BigInteger& a, const BigInteger& b) {
    BigInteger ans = a;
    return ans -= b;
}
const BigInteger operator * (const BigInteger& a, const BigInteger& b) {
    BigInteger res = b;
    return (res *= a);
}
const BigInteger operator / (const BigInteger& a, const BigInteger& b) {
    BigInteger res = a;
    return (res /= b);
}
const BigInteger operator % (const BigInteger& a, const BigInteger& b) {
    BigInteger res = a;
    return (res %= b);
}