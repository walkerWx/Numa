/*
 * fixup.h
 * 	This is a fix up file for functions in REAL library,
 * 	we overload most of the REAL library functions here.
 *
 *  Created on: 2015年4月8日
 *      Author: walker
 */

#ifndef SRC_FIXUP_H_
#define SRC_FIXUP_H_

#include <cmath>
#include <string>
#include <iostream>

/****************************************************************************/
// arithmetic functions
/****************************************************************************/
double power(const double& x, const double& y);
double power(const double& x, int n);
double modulo(const double& x, const double& y);
double maximum(const double& x, const double& y);
double minimum(const double& x, const double& y);

/****************************************************************************/
// roots
/****************************************************************************/
//double sqrt(const double& x);
double root(const double& x, int n);

/****************************************************************************/
// trigonometric functions
/****************************************************************************/
//double sin(const double& x);
//double cos(const double& x);
//double tan(const double& x);
double cotan(const double& x);
double sec(const double& x);
double cosec(const double& x);

/****************************************************************************/
// inverse trigonometric functions
/****************************************************************************/
//double asin(const double& x);
//double acos(const double& x);
//double atan(const double& x);
double acotan(const double& x);
double asec(const double& x);
double acosec(const double& x);

/****************************************************************************/
//hyperbolic functions
/****************************************************************************/
//double sinh(const double& x);
//double cosh(const double& x);
//double tanh(const double& x);
double coth(const double& x);
double sech(const double& x);
double cosech(const double& x);

/****************************************************************************/
// inverse hyperbolic functions
/****************************************************************************/
//double asinh(const double& x);
//double acosh(const double& x);
//double atanh(const double& x);
double acoth(const double& x);
double asech(const double& x);
double acosech(const double& x);

/****************************************************************************/
// exponentiation + logarithm
/****************************************************************************/
//double exp(const double& x);
//double log(const double& x);

/****************************************************************************/
// special constants values
/****************************************************************************/
double pi();   // = 3.141592653...
double euler();   // = 2.718281828...
double ln2();   // = 0.693147180...

double limit (double f(int p));

double limit (double f(int p,const double&),const double&);

double limit (double f(int,const double &,const double&),
    const double& x,
    const double& y);

bool bound(double a, int p);

double iRRAM_exec (double f(const double&), const double&);


using std::string;

#endif /* SRC_FIXUP_H_ */
