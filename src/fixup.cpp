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

/****************************************************************************/
// arithmetic functions
/****************************************************************************/
double power(const double& x, const double& y) {
	return pow(x, y);
}
double power(const double& x, int n) {
	return pow(x, n);
}
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

double limit (double f(int p,const double&,const double&),
    const double& x,
    const double& y) {
  return f(-10 , x, y);
}
