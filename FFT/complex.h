#ifndef _COMPLEX_H
#define _COMPLEX_H

    typedef struct complex {
        double re;
        double im;
    } Complex;

    Complex polarToCartesian(double, double);
    double  complexModulus(Complex);
    double 	complexArgument(Complex);
    Complex complexAdd(Complex, Complex);
    Complex complexSub(Complex, Complex);
    Complex complexMult(Complex, Complex);

#endif