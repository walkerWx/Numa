/*
 * fixup.cpp
 *
 *  Created on: 2015年4月8日
 *      Author: walker
 */

#include "fixup.h"

#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <stdlib.h>

/****************************************************************************/
// arithmetic functions
/****************************************************************************/
double power(const double& x, const double& y) {
	return pow(x, y);
}
double square(const double& x) {
  return x*x;
}
//double power(const double& x, int n) {
//	return pow(x, n);
//}
double modulo(const double& x, const double& y) {
	return x - round(x / y) * y;
}
double maximum(const double& x, const double& y) {
	return std::max(x, y);
}
double minimum(const double& x, const double& y) {
	return std::min(x, y);
}

/****************************************************************************/
// roots
/****************************************************************************/
//double sqrt(const double& x);
double root(const double& x, int n) {
	if (n == 0) {
		return 0;
	} else {
		return pow(x, 1.0 / n);
	}
}

/****************************************************************************/
// trigonometric functions
/****************************************************************************/
//double sin(const double& x);
//double cos(const double& x);
//double tan(const double& x);
double cotan(const double& x) {
	return 1.0 / tan(x);
}
double sec(const double& x) {
	return 1.0 / cos(x);
}
double cosec(const double& x) {
	return 1.0 / sin(x);
}

/****************************************************************************/
// inverse trigonometric functions
/****************************************************************************/
//double asin(const double& x);
//double acos(const double& x);
//double atan(const double& x);
double acotan(const double& x) {
	return atan(1.0 / x);
}
double asec(const double& x) {
	return acos(1.0 / x);
}
double acosec(const double& x) {
	return asin(1.0 / x);
}

/****************************************************************************/
//hyperbolic functions
/****************************************************************************/
//double sinh(const double& x);
//double cosh(const double& x);
//double tanh(const double& x);
double coth(const double& x) {
	return 1.0 / tanh(x);
}
double sech(const double& x) {
	return 1.0 / cosh(x);
}
double cosech(const double& x) {
	return 1.0 / sinh(x);
}

/****************************************************************************/
// inverse hyperbolic functions
/****************************************************************************/
//double asinh(const double& x);
//double acosh(const double& x);
//double atanh(const double& x);
double acoth(const double& x) {
	return atanh(1.0 / x);
}
double asech(const double& x) {
	return acosh(1.0 / x);
}
double acosech(const double& x) {
	return asinh(1.0 / x);
}

/****************************************************************************/
// exponentiation + logarithm
/****************************************************************************/
//double exp(const double& x);
//double log(const double& x);
/****************************************************************************/
// special constants values
/****************************************************************************/
double pi() {
	return 3.141596253;
}
double euler() {
	return 2.718281828;
}
double ln2() {
	return 0.693147180;
}

double limit (double f(int p)) {
  return f(-10);
}

double limit(double f(int p,const double&),const double& a) {
  return f(-10,a);
}

double limit (double f(int p,const double&,const double&),
    const double& x,
    const double& y) {
  return f(-10 , x, y);
}


bool bound(double a, int p) {
  return a < pow(2,p);
}

int upperbound(const double& x) {
  return log(fabs(x)) / log(2.0) + 1;
}

int size(const double& x) {
  return 100;
}

bool positive(double x, int p) {
  return x > 0;
}

double iRRAM_exec (double f(const double&), const double& p) {
  return f(p);
}

int iRRAM_exec (int f(const int&),const int& p) {
  return f(p);
}

int choose (const bool x1,
            const bool x2,
            const bool x3,
            const bool x4,
            const bool x5,
            const bool x6 ) {
  if (x1) return 1; else 
    if (x2) return 2; else 
      if (x3) return 3; else 
        if (x4) return 4; else 
          if (x5) return 5; else 
            return 6;

}

// we should implement methods to calculate lipschitz number here
// but for convinence, we just return a specific number
double    lipschitz  (double f(const double&),
                           int lip,
                           bool on_domain(const double&),
                           const double& x){
  return f(x);
}

double    lipschitz  (double f(const double&),
                           double lip_f(const double&),
                           bool on_domain(const double&),
                           const double& x) {
  return f(x);
}

double    lipschitz  (double f(const double&,const double&),
                           int lip,
                           bool on_domain(const double&,const double&),
                           const double& x,
                           const double& y) {
  return f(x,y);
}

double    lipschitz  (double f(int, const double&),
                           int lip,
                           bool on_domain(int k,const double&),
                           int k,
                           const double& x) {
  return f(k,x);
}

double    lipschitz  (double f(int, const double&,const double&),
                           int lip,
                           bool on_domain(int k,const double&,const double&),
                           int k,
                           const double& x,
                           const double& y) { 
  return f(k,x,y);
}

double    lipschitz (double f(const double&),
                          double lip_f(const double&),
                          const double& x) {
  return f(x);
}

double approx(const double& x,int p) {
  return x;
}

int module (double f(const double& x), const double& y,int p) {
  return p;
}

void continous_begin() {}
void continous_end() {}
using std::string;
