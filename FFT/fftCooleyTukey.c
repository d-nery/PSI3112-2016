#include <stdlib.h>
#include "complex.h"
#include "fftCooleyTukey.h"

#define PI 	3.1415926535897932

Complex* dft(Complex* x, int nSamples) {
    Complex* X = (Complex*) malloc(nSamples * sizeof(Complex));
    Complex* nthRoot = (Complex*) malloc(nSamples * sizeof(Complex));
    int k, n;

    for(k = 0; k < nSamples; k++)
        nthRoot[k] = polarToCartesian(1, -2 * PI * k / nSamples);

    for(k = 0; k < nSamples; k++) {
        X[k].re = X[k].im = 0.0;
        
        for(n = 0; n < nSamples; n++)
            X[k] = complexAdd(X[k], complexMult(x[n], nthRoot[(n * k) % nSamples]));
    }
    
    free(nthRoot);
    
    return X;
}

Complex* fft(Complex* x, int nSamples) {
    Complex* X = (Complex*) malloc(nSamples * sizeof(Complex));
    Complex *d, *e, *D, *E;
    int k;

    if (nSamples == 1) {
        X[0] = x[0];
        
        return X;
    }

    e = (Complex*) malloc((nSamples / 2) * sizeof(Complex));
    d = (Complex*) malloc((nSamples / 2) * sizeof(Complex));
    
    for(k = 0; k < nSamples / 2; k++) {
        e[k] = x[2 * k];
        d[k] = x[2 * k + 1];
    }

    E = fft(e, nSamples / 2);
    D = fft(d, nSamples / 2);
    
    for(k = 0; k < nSamples / 2; k++)
        D[k] = complexMult(polarToCartesian(1, -2.0 * PI * k / nSamples), D[k]);

    for(k = 0; k < nSamples / 2; k++) {
        X[k]       = complexAdd(E[k], D[k]);
        X[k + nSamples / 2] = complexSub(E[k], D[k]);
    }

    free(D);
    free(E);
    
    return X;
}