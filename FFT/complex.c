#include <math.h>
#include "complex.h"

Complex polarToCartesian(double modulus, double argumentRadians) {
    Complex cartesian;

    cartesian.re = modulus * cos(argumentRadians);
    cartesian.im = modulus * sin(argumentRadians);

    return cartesian;
}

double complexModulus(Complex complex) {
    return sqrt(complex.re * complex.re + complex.im * complex.im);
}

double complexArgument(Complex complex) {
	return atan(complex.im / complex.re);
}

Complex complexAdd(Complex left, Complex right) {
    Complex sum;

    sum.re = left.re + right.re;
    sum.im = left.im + right.im;
    
    return sum;
}

Complex complexSub(Complex left, Complex right) {
    Complex difference;

    difference.re = left.re - right.re;
    difference.im = left.im - right.im;
    
    return difference;
}

Complex complexMult(Complex left, Complex right) {
    Complex product;

    product.re = left.re * right.re - left.im * right.im;
    product.im = left.re * right.im + left.im * right.re;
    
    return product;
}