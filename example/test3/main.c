#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include "info.c"


int main() {

  long long timeo=0;
  while(1){

struct timespec ts; 
clock_gettime(CLOCK_MONOTONIC, &ts);
long long time = ts.tv_sec;

  if(time!=timeo){
  timeo = time;

int *sp = get_system_metrics();

for(int i=0;i<80;i++){
    
        printf("parm %i -- %i\n",i,sp[i]);
  }}}

return 0;}
