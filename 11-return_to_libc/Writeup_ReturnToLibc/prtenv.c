#include <stdio.h>
#include <stdlib.h>

void get_env(char* name) {
  char* value = getenv(name);
  if(value){
    printf("0x%x\n", (unsigned int)value);
  }
}

int main(){
  get_env("MY_GLOB_VAR");
  return 1;
}
