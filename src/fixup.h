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
#include <iomanip>

typedef unsigned int SIZETYPEMANTISSA;
typedef int SIZETYPEEXPONENT;

const int MANTISSA_BITS = (int) (8*sizeof(SIZETYPEMANTISSA)) ;
const int DIFF_BITS     = 3; 
const int BIT_RANGE     = 8; 
const int BIT_RANGE2    = 8; 
const int GUARD_BITS    =  MANTISSA_BITS - DIFF_BITS;

inline unsigned int scale(const unsigned int w,const int p) {return ((p<=GUARD_BITS)?(w>>p):0); };

struct sizetype { SIZETYPEMANTISSA  mantissa; SIZETYPEEXPONENT exponent; } ;

inline void sizetype_set(sizetype& x,const int mantissa,const int exponent) 
{
  x.exponent=exponent;
  x.mantissa=mantissa;
}

inline void sizetype_add_wo_norm(sizetype& x,const sizetype& y,const sizetype& z) 
{
  if ( y.exponent >  z.exponent) {
    x.exponent= y.exponent;
    x.mantissa= y.mantissa + scale(z.mantissa,(x.exponent-z.exponent))  + 1 ;
  } else {
    x.exponent= z.exponent;
    x.mantissa= z.mantissa + scale(y.mantissa,(x.exponent-y.exponent))  + 1 ;
  }
}

inline void sizetype_add(sizetype& x,const sizetype& y,const sizetype& z) 
{
  sizetype_add_wo_norm(x,y,z);
}

inline void sizetype_mult(sizetype& x,const sizetype& y,const sizetype& z) 
{ unsigned long long lmantissa=
     ((unsigned long long)(y.mantissa))*z.mantissa;
  x.exponent=y.exponent+z.exponent;
  x.mantissa=lmantissa+1;
}
/****************************************************************************/
// arithmetic functions
/****************************************************************************/
double power(const double& x, const double& y);
double square(const double& x);
//double power(const double& x, int n);
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
int upperbound(const double&);
int size(const double&);
bool positive(double x,int p);

double iRRAM_exec (double f(const double&), const double&);
int iRRAM_exec (int f(const int&),const int&);

int choose (const bool x1 = false,
            const bool x2 = false,
            const bool x3 = false,
            const bool x4 = false,
            const bool x5 = false,
            const bool x6 = false );

// reduced error propagation: ------------------------

double    lipschitz  (double f(const double&),
                           int lip,
                           bool on_domain(const double&),
                           const double& x);

double    lipschitz  (double f(const double&),
                           double lip_f(const double&),
                           bool on_domain(const double&),
                           const double& x);

double    lipschitz  (double f(const double&,const double&),
                           int lip,
                           bool on_domain(const double&,const double&),
                           const double& x,
                           const double& y);

double    lipschitz  (double f(int, const double&),
                           int lip,
                           bool on_domain(int k,const double&),
                           int k,
                           const double& x);

double    lipschitz  (double f(int, const double&,const double&),
                           int lip,
                           bool on_domain(int k,const double&,const double&),
                           int k,
                           const double& x,
                           const double& y);

double    lipschitz (double f(const double&),
                          double lip_f(const double&),
                          const double& x);

double approx(const double&,int);

int module (double f(const double&),const double&,int);

void continous_begin();
void continous_end();
using std::string;

#endif /* SRC_FIXUP_H_ */
