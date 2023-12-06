# zLog
 Simple single header library for the logging system

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/zLouis043/zLog/main)
![GitHub top language](https://img.shields.io/github/languages/top/zLouis043/zLog)

## How-To
Just include the header file your project and define its implementation: 

```c

#define ZLOG_IMPLEMENTATION
#include "zLog.h"

```

## Test and functionalities 

### To build and run the test file

```console

$ ./bar.bat 

```

### Example 

```c

#include <stdio.h>

#define ZLOG_IMPLEMENTATION
#include "src/zLog.h"

int main(void){

   zlog_init();

   zlog.set_flags(ZLOG_ALL | ZLOG_DEBUG);

   zlog_info("Hello World!\n");
   zlog_debug("Hello World!\n");
   zlog_trace("Hello World!\n");
   zlog_warning("Hello World!\n");
   zlog_error("Hello World!\n");
   zlog_fatal("Hello World!\n");

   return 0; 
}

Output > 6/12/2023 16:56:29 | main @ C:zLog\main.c:14 | [INFO] > Hello World!
         6/12/2023 16:56:29 | main @ C:zLog\main.c:14 | [DEBUG] > Hello World!
         6/12/2023 16:56:29 | main @ C:zLog\main.c:14 | [TRACE] > Hello World!
         6/12/2023 16:56:29 | main @ C:zLog\main.c:15 | [WARNING] > Hello World!
         6/12/2023 16:56:29 | main @ C:zLog\main.c:16 | [ERROR] > Hello World!  
         6/12/2023 16:56:29 | main @ C:zLog\main.c:17 | [FATAL] > Hello World! 
```
