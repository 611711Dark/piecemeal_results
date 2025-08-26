#include "stdio.h"
#include "math.h"
#include <math.h>
typedef double (*TrigFunc)(double) ;
double diff_funcation(TrigFunc func,double value){
  double result;
  result=(func(value+(1e-9))-func(value))/(1e-9);
  return result;
}
void compare(const char* name,TrigFunc func,double input,double compare_value){
  double diff_value;
  diff_value = diff_funcation(func,input);
  printf("compare(%s): diff_value=%.9f  diff=%.9f;  error=%e\n",name,diff_value,compare_value,fabs(diff_value-compare_value));
}

double complex_func(double x){
  double result;
  result = (pow(x,2)+1)*log(2*x+3);
  return result;
}

int main(){
  float x=1.14514;
  //scanf("%f",&x);  
  compare("sinx",sin ,x ,cos(x) );
  compare("e^x",exp ,x ,exp(x) );
  compare("lnx",log ,fabs(x) ,1/fabs(x) );
  compare("cosx",cos ,x ,-sin(x) );
  compare("(x^2+1)*ln(2*x+3)", complex_func, x, (2*x*log(2*x+3)+(pow(x,2)+1)*2/(2*x+3)));
  return 0;
}
