#include <iRRAM.h>

using namespace std;
using namespace iRRAM;

void compute() {

  REAL a = REAL(3.14);
  REAL b = REAL(0.5);
  REAL c = REAL(256);
  
  REAL d;

  d = a + b;
  d = a - b;
  d = a * b;
  d = a / b;

  d = power(a,b);
  d = power(a,3);
  d = modulo(b,c);
  d = maximum(a,b);
  d = minimum(a,b);

  d = sqrt(a);
  d = root(a, 3);

  d = sin(a);
  d = cos(a);
  d = tan(a);
  d = cotan(a);
  d = sec(a);
  d = cosec(a);

  d = atan(b);
  d = asin(b);
  d = acos(b);
  d = acotan(b);
  d = asec(b);
  d = acosec(b);

  d = sinh(a);
  d = cosh(a);
  d = tanh(a);
  d = coth(a);
  d = sech(a);
  d = cosech(a);

  d = asinh(a);
  d = acosh(a);
  d = atanh(a);
  d = acoth(a);
  d = asech(a);
  d = acosech(a);

  d = exp(a);
  d = log(a);
}
