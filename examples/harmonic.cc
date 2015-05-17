#include "iRRAM.h"

using namespace iRRAM;


void compute(){
  int count;
  cout << "Harmonic series: s=sum_i=1^n 1/i \n";
  cout << "How many values: ";
  cin  >> count;

  REAL   xr= 0;
  double xd= 0;
  for (int i=1;i<=count;i++ ) {
    if (i%(count/100)==0)
    cout << double(xr)    <<" "
         << xd            <<" "
	 << double(xr)-xd <<" "
	 << i             <<"\n" ;
    xr+=REAL(1)/i;
    xd+=    1.0/i;
  }
};
