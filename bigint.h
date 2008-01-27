#ifndef __BIGINT_H
#define __BIGINT_H

#include <string>
#include <iostream>
class BigInt
{
    private:
        char *digits;
        int size;            // number of used bytes (digits)
        int capacity;        // size of digits
        int sign;            // -1, 0 or +1

    public:
        /** Creates a BigInt with initial value n and initial capacity cap **/
        BigInt( int n, int cap );

        /** Creates a BigInt with initial value n **/
        BigInt( int n );

        /** Creates a BigInt with initial value floor( d ) **/
        BigInt( long double d );

        /** Creates a BigInt with value 0 **/
        BigInt();

        /** Creates a BigInt by reading the value from a string **/
        BigInt( std::string s );

        /** Creates a BigInt by reading the value from a C string **/
        BigInt( const char s[] );

        /** Copy constructor **/
        BigInt( const BigInt &n );

        /** Assignment operators **/
        const BigInt &operator=( const BigInt &n );
        const BigInt &operator=( int n );

        /** Cleans up **/
        ~BigInt();

        /** Removes any leading zeros and adjusts the sign **/
        void normalize();

        /** Returns the sign of n: -1, 0 or 1 **/
        static int sig( int n );

        /** Returns the sign of n: -1, 0 or 1 **/
        static int sig( long double n );

        /** Returns the number of decimal digits **/
        inline int length() { return size; }

        /** Arithmetic **/
        BigInt operator++();
        BigInt operator++( int );
        BigInt operator--();
        BigInt operator--( int );
        BigInt operator-();
        BigInt operator+ ( int n    );
        BigInt operator+ ( BigInt n );
        BigInt&operator+=( int n    );
        BigInt&operator+=( BigInt n );
        BigInt operator- ( int n    );
        BigInt operator- ( BigInt n );
        BigInt&operator-=( int n    );
        BigInt&operator-=( BigInt n );
        BigInt operator* ( int n    );
        BigInt operator* ( BigInt n );
        void   operator*=( int n    );
        void   operator*=( BigInt n );
        BigInt operator/ ( int n    );
        BigInt operator/ ( BigInt n );
        void   operator/=( int n    );
        void   operator/=( BigInt n );
        int    operator% ( int n    );
        BigInt operator% ( BigInt n );
        void   operator%=( int n    );
        void   operator%=( BigInt n );
        int divide( int n );              // Divides storing quotient in *this and returning the remainder
        BigInt divide( BigInt n );        // Divides storing quotient in *this and returning the remainder
        BigInt operator* ( long double n ); // Multiplies by a double and truncates (always under-estimates!)
        void   operator*=( long double n ); // Multiplies by a double and truncates (always under-estimates!)

        /** Bitwise arithmetic **/
        BigInt operator<< ( int n    );
        void   operator<<=( int n    );
        BigInt operator>> ( int n    );   // Works differently for negative numbers
        void   operator>>=( int n    );   // Works differently for negative numbers
/*
        BigInt operator&  ( int n    );
        BigInt operator&  ( BigInt n );
        void   operator&= ( int n    );
        void   operator&= ( BigInt n );
        BigInt operator|  ( int n    );
        BigInt operator|  ( BigInt n );
        void   operator|= ( int n    );
        void   operator|= ( BigInt n );
        BigInt operator^  ( int n    );
        BigInt operator^  ( BigInt n );
        void   operator^= ( int n    );
        void   operator^= ( BigInt n );
        BigInt operator~();
*/
        /** Concatenation ;-) **/
        BigInt operator,( int n );
        BigInt operator,( BigInt n );

        /** Casting **/
        bool operator!();
        operator bool();
        //operator int();   //XXX: Don't do this!!! It takes precedence over operator+(int,BigInt)
        operator std::string();

        /** Comparison **/
        bool operator<( BigInt n );
        bool operator>( BigInt n );
        bool operator==( BigInt n );
        bool operator<=( BigInt n );
        bool operator>=( BigInt n );
        bool operator<( int n );
        bool operator>( int n );
        bool operator==( int n );
        bool operator<=( int n );
        bool operator>=( int n );
        int compare( BigInt n );

        /** Returns the lowest value as an integer (watch out for overflow) **/
        int toInt();

        /** Returns the value as a decimal string **/
        std::string toString();

        /** Outputs decimal value to stdout **/
        void print();

        /** Outputs the decimal value, with commas **/
        void printWithCommas( std::ostream &out );

    private:
        /** Expansion **/
        void grow();

    /** I/O Friends **/
    friend std::istream &operator>>( std::istream &in, BigInt &n );
    friend std::ostream &operator<<( std::ostream &out, BigInt n );

    /** Logarithms **/
    friend long double log2( BigInt x, long double epsilon );
    inline friend long double log( BigInt x, long double epsilon );
    inline friend long double log10( BigInt x, long double epsilon );
    inline friend long double lg( BigInt x, long double epsilon );
    inline friend long double ln( BigInt x, long double epsilon );
};


template <typename T> T t_power (T x, int n) {
    if (n==0) return 1;
    if (n==1) return x;
    if (n%2) return x * t_power(x, n-1);
    T tmp = t_power(x, n/2);
    return tmp*tmp;
}

template <typename T> T t_modulo (const T x_x, const T y) {
    // FIXME (ivucica#1#) ugly slow hack. repeated subtraction "algorithm".

    for (T x = x_x; x > y; x -= y);
    return y;
}

#endif
