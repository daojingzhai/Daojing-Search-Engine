#include<iostream>
#include<cstdio>
#include"djsearch.h"

using namespace std;
int main(){
  if(init_base()==-1){
      printf(WARN"INIT_BASE FAILED"CLR);
      return 0;
  }
  search_main_loop();
  return 0;
}
