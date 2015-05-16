#include <iRRAM.h>

using namespace std;
using namespace iRRAM;


void compute() { 
  
  int i;
  float f;

  RATIONAL r;

  REAL single = 1.03;

  REAL const * const r;

  int (*fn)(const REAL r[], double);
  
  REAL multi_1, multi_2;

  const REAL const_before;
  
  REAL const const_after;

  REAL* pointer;

  const REAL* pointer_to_const;
  
  REAL * const const_pointer = &single;

  REAL& reference;

  const REAL& const_reference;
}

void f(){
  REAL single = REAL(1);
  
  REAL multi_1, multi_2;

  const REAL const_before;
  
  REAL const const_after;

  REAL* pointer;

  const REAL* pointer_to_const;
  
  REAL * const const_pointer = &single;

  REAL& reference;

  const REAL& const_reference;
}
  REAL single; REAL multi_1, multi_2;

  const REAL const_before;
  
  REAL const const_after;

  REAL* pointer;

  const REAL* pointer_to_const;
  
  REAL * const const_pointer = &single;

  REAL& reference;

  const REAL& const_reference;
