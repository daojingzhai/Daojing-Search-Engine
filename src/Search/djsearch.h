#include"common.h"
#include<string>

using namespace std;


enum CMD_LIST{
  CMD_NONE,CMD_AND,CMD_OR
};

int init_base();
void init_search();
void search_main_loop();
void read_input(string ipt);
int search_engine();
int CMD_AND_ENGINE();
int CMD_OR_ENGINE();
int CMD_NONE_ENGINE();
void END_DEALER();
void search_string(string tar,int *p);
void suc_search_dealer();
