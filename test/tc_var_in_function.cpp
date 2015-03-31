#include <iRRAM.h>

using namespace std;
using namespace iRRAM;


void compute() { 
  REAL single;
  
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
  REAL single;
  
  REAL multi_1, multi_2;

  const REAL const_before;
  
  REAL const const_after;

  REAL* pointer;

  const REAL* pointer_to_const;
  
  REAL * const const_pointer = &single;

  REAL& reference;

  const REAL& const_reference;
}
