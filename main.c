#include <stdio.h>

#define ZLOG_IMPLEMENTATION
#include "src/zLog.h"

int main(void){

   zlog_init("zlog");

   //zlog.unset_flags(ZLOG_USE_COLORS);

   printf("%d\n", zlog.get_flags());

   zflog_info("log-output.txt","Hello World!\n");
   zflog_debug("log2-output.txt","Hello World!\n");
   zlog_trace("Hello World!\n");
   zlog_warning("Hello World!\n");
   zlog_error("Hello World!\n");
   zlog_fatal("Hello World!\n");

   return 0; 
}
