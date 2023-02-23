#include <stdio.h>
#include "config.h"

#ifdef USE_MY_COMPILER
  #include <Compiler.h>
#endif
#ifdef USE_DB
  #include <database.h>
#endif

int main(int argc, char** argv) {
  printf("%s VERSION: %d.%d\n", argv[0], APP_VERSION_MAJOR, APP_VERSION_MINOR);
#ifdef USE_DB
  start_database();
#endif
#ifdef USE_MY_COMPILER
  printf("giao: %s\n", (char*)compiler("111"));
#endif
  return 0;
}