#include <stdio.h>

#define ZLOG_IMPLEMENTATION
#include "src/zLog.h"

int main(void){

   zlog_init("zlogger");

   zlog.set_pattern("{f} @ {l} | {t} > {n} > ");

   zflog_info("log-output.txt","Hello World!\n");
   zflog_debug("log2-output.txt","Hello World!\n");
   zlog_trace("Hello World!\n");
   zlog_warning("Hello World!\n");
   zlog_error("Hello World!\n");
   zlog_fatal("Hello World!\n");

   system("pause");

   return 0; 
}
