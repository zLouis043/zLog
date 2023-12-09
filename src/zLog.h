/*
MIT License

Copyright (c) 2023 zLouis043

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

HOW TO USE THE LIBRARY:
    - Define the library implementation and include the header file:
        "#define ZLOG_IMPLEMENTATION
        #include "zlog.h"
    - Init the logger with the zlog_init() function by giving a name to the logger
    - Set the pattern if you want by calling the zlog.set_pattern() function and set the pattern with the format 
    specifiers:
        - {D} = day
        - {M} = month
        - {Y} = year
        - {h} = hours
        - {m} = minutes
        - {s} = seconds
        - {f} = the function where the log is being called
        - {l} = the location where the log is being called (the file and the line number)
        - {n} = the name of the logger
        - {t} = the tag of the log level 
    - Use the various log macros for the different log levels and log outputs (console (zlog) or a specific file (zflog))
    - Set the flags if you want the log messages in the console output have colors with the zlog.set_flags() | zlog.unset_flags() functions
      with the ZLOG_USE_COLORS flag
      - Set the flags if you want the debug log messages to be print or not with the zlog.set_flags() | zlog.unset_flags() functions
      with the ZLOG_DEBUG flag
*/

#ifndef ZLOG_H_
#define ZLOG_H_

#include <stdint.h>

/*
 List of ansi colors used by the logger.
*/

#define ANSI_COLOR_RED     "\x1b[0;31m"
#define ANSI_COLOR_GREEN   "\x1b[0;32m"
#define ANSI_COLOR_YELLOW  "\x1b[0;33m"
#define ANSI_COLOR_BLUE    "\x1b[0;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN    "\x1b[0;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
    Level of logging.
*/

typedef enum {
    L_INFO = 0,
    L_DEBUG,
    L_TRACE,
    L_WARNING,
    L_ERROR,
    L_FATAL
}LogLevel;

/*
    Look up table for the tag of every level of logging 
*/

static const char * log_tag[] = {
    [L_INFO] = "INFO",
    [L_DEBUG] = "DEBUG",
    [L_TRACE] = "TRACE",
    [L_WARNING] = "WARNING",
    [L_ERROR] = "ERROR",
    [L_FATAL] = "FATAL"
};

/*
    Look up table for the color of every level of logging
*/

static const char * log_color[] = {
    [L_INFO] = ANSI_COLOR_GREEN,
    [L_DEBUG] = ANSI_COLOR_YELLOW,
    [L_TRACE] = ANSI_COLOR_CYAN,
    [L_WARNING] = ANSI_COLOR_YELLOW,
    [L_ERROR] = ANSI_COLOR_RED,
    [L_FATAL] = ANSI_COLOR_RED
};

/*
    Bit position of every flag
*/

typedef enum BIT_flags{
    ZLOG_BIT_DEBUG = 0,
    ZLOG_BIT_USE_COLORS = 1,
    ZLOG_BIT_CHECK_COLOR = 4
}LogBitFlags;

/*!  
    bit field that contains the flags for the log system

    0   0   0       0         0        0           0           0 
            |       |         |        |           |           |
            NAME    COLORS    DEBUG    FUNCTION    LOCATION    TIME

    ZLOG_TIME = SHOW THE TIME WHEN THE MESSAGE HAS BEEN LOGGED
    ZLOG_LOCATION = SHOW THE FILE LOCATION AND THE LINE WHERE THE MESSAGE HAS BEEN LOGGED
    ZLOG_FUNCTION = SHOW THE FUNCTION WHERE THE MESSAGE HAS BEEN LOGGED
    ZLOG_DEBUG = SHOW THE MESSAGE LOGGED WITH A LOG LEVEL SET TO DEBUG MODE
    ZLOG_USE_COLORS = LOG THE MESSAGE AND THE OTHER INFORMATIONS WITH COLORS, ONLY ON THE CONSOLE AND NOT THE FILE MODE
*/
typedef enum flags{
    ZLOG_DEBUG = 1 << ZLOG_BIT_DEBUG,
    ZLOG_USE_COLORS = 1 << ZLOG_BIT_USE_COLORS,
    ZLOG_CHECK_COLOR = 1 << ZLOG_BIT_CHECK_COLOR,
    ZLOG_ALL = ZLOG_USE_COLORS | ZLOG_DEBUG 
}LogFlags;

typedef enum {
    
    DAY,
    MONTH,
    YEAR,
    HOUR,
    MINUTE,
    SECOND,
    FUNCTION,
    LOCATION,
    TAG,
    PATTER_COUNT

}PatternType;



/*!
    Struct that contains every bit of information about the log system and its functions

    @param level the current log level of the logger
    @param flags the bitfield that contains all the flags used by the logger
    @param mode the mode in which the logger will print the message in the file:
                - "a" to append the message to the file.
                - "w" to overwrite the file.
    @param Stream the current stream in which the logger is printing the message
    @param pattern the pattern of the log message 

    @param set_level function that sets the log level of the logger

    @param set_file_write_mode function that sets the mode of writing the message into the file
    @param open_file function the opens a file and set it as the new stream
    @param clear_file function the clears the file 
    @param close_stream function the closes the current stream of the logger
    @param set_output_stream function the sets the output stream of the logger

    @param get_flags functions that return the value of the flags
    @param set_flags function that set the specified flags of the logger
    @param unset_flags function that unset the specified flags of the logger
    @param flip_flags function that flips the value of the specified flags of the logger
    
    @param set_pattern function that sets the pattern of the log message
*/
typedef struct {

    const char *name;
    LogLevel level;
    uint8_t flags;
    const char * mode;
    FILE* Stream;
    char * pattern;
    
    void (*set_level)(LogLevel level);

    void (*set_file_write_mode)(const char * mode);
    void (*open_file)(const char* filename);
    void (*clear_file)(const char* filename);
    void (*close_stream)();
    void (*set_output_stream)(FILE* Stream);

    uint8_t (*get_flags)();
    void (*set_flags)(LogFlags flags);
    void (*unset_flags)(LogFlags flags);
    void (*flip_flags)(LogFlags flags);

    void (*set_pattern)(char* pattern);

}zlogger;

/*
    The logger used globally
*/

zlogger zlog;

/*!
    Macro used to check a specific bit mask between the flags 
    @param flag the flag to check 
*/

#define CHECK_FLAG(flag) (zlog.flags & (1 << flag))

/*!
    Function that initialize the logger
    @param log_name The name of the logger
*/

void zlog_init(const char* log_name);

/*!
    Base function to log a message to the console or a file
    @param filename the file where the log is being called
    @param line the line where the log is being called
    @param fun_name the function where the log is being called
    @param fmt the string to format and print 
    @param ... the various args used to format the string 
*/

void zlog_(const char* filename, size_t line, const char* fun_name, const char* fmt, ...);

/*!
    Macro that will log a message to the console at the current log level defined 
    @param ... the message to log 
*/

#define zlog(...)                       zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)   

/*!
    Macro that will log a message to a file at the current log level defined 
    @param output_file the file where the log message is being printed
    @param ... the message to log 
*/

#define zflog(output_file, ...)         zlog.open_file(output_file); \
                                        if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){\
                                            zlog.unset_flags(ZLOG_USE_COLORS);\
                                            zlog.set_flags(ZLOG_CHECK_COLOR);\
                                        }\
                                        zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
                                        zlog.close_stream();\
                                        if(CHECK_FLAG(ZLOG_BIT_CHECK_COLOR)){\
                                            zlog.set_flags(ZLOG_USE_COLORS);\
                                        }

/*!
    Macro that will log a message to the console with a specified level
    @param level the level of the log 
    @param ... The message to be logged
*/

#define _zlog(level, ...)   zlog.set_level(level);\
                            zlog.set_output_stream(stderr);\
                            zlog(__VA_ARGS__)

/*!
    Macro that will log a message to a file with a specified level
    @param output_file the name of the output file 
    @param level the level of the log 
    @param ... The message to be logged
*/                            

#define _zflog(output_file, level, ...)     zlog.set_level(level); \
                                            zflog(output_file, ##__VA_ARGS__) \

/*!
    Logs to the console the info message
    @param ... The message to be logged
*/               
#define zlog_info(...)      _zlog(L_INFO,    ##__VA_ARGS__)
/*!
    Logs to the console the debug message
    @param ... The message to be logged
*/  
#define zlog_debug(...)     _zlog(L_DEBUG,   ##__VA_ARGS__)
/*!
    Logs to the console the trace message
    @param ... The message to be logged
*/  
#define zlog_trace(...)     _zlog(L_TRACE,   ##__VA_ARGS__)
/*!
    Logs to the console the warning message
    @param ... The message to be logged
*/  
#define zlog_warning(...)   _zlog(L_WARNING, ##__VA_ARGS__)
/*!
    Logs to the console the error message
    @param ... The message to be logged
*/  
#define zlog_error(...)     _zlog(L_ERROR,   ##__VA_ARGS__)
/*!
    Logs to the console the fatal message
    @param ... The message to be logged
*/  
#define zlog_fatal(...)     _zlog(L_FATAL,   ##__VA_ARGS__)

/*!
    Logs into the output_file the info message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_info(output_file, ...)    _zflog(output_file,  L_INFO,      ##__VA_ARGS__)
/*!
    Logs into the output_file the debug message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_debug(output_file, ...)   _zflog(output_file,  L_DEBUG,     ##__VA_ARGS__)
/*!
    Logs into the output_file the trace message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_trace(output_file, ...)   _zflog(output_file,  L_TRACE,     ##__VA_ARGS__)
/*!
    Logs into the output_file the warning message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_warning(output_file, ...) _zflog(output_file,  L_WARNING,   ##__VA_ARGS__)
/*!
    Logs into the output_file the error message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_error(output_file, ...)   _zflog(output_file,  L_ERROR,     ##__VA_ARGS__)
/*!
    Logs into the output_file the fatal message
    @param output_file the name of the output file 
    @param ... The message to be logged
*/
#define zflog_fatal(output_file, ...)   _zflog(output_file,  L_FATAL,     ##__VA_ARGS__)

#endif /* ZLOG_H_ */

#ifdef ZLOG_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

static uint8_t zlog_get_flag(){
    return zlog.flags;
}

static void zlog_set_flags(LogFlags flags){

    zlog.flags |= (uint8_t)flags;

}

static void zlog_unset_flags(LogFlags flags){

    zlog.flags &= ~(uint8_t)flags;

}

static void zlog_flip_flags(LogFlags flags){

    zlog.flags ^= (uint8_t)flags;

}

static void zlog_set_level(LogLevel level){
    zlog.level = level;
}

static void zlog_set_file_write_mode(const char * mode){
    zlog.mode = mode;
}

static void zlog_open_file(const char* filename){

    FILE *fp = fopen(filename, zlog.mode);

    if(!fp){
        zlog_fatal("Couldn't open file: %s", filename);
        exit(1);
    }

    zlog.Stream = fp;

}

static void zlog_close_stream(){
    fclose(zlog.Stream);
    zlog.Stream = stderr;
}

static void zlog_set_output_stream(FILE* Stream){

    zlog.Stream = Stream;

}

static void zlog_clear_file(const char* filename){

    FILE *fp = fopen(filename, "w");
    fclose(fp);

}

static void zlog_set_pattern(char* pattern){
    
    zlog.pattern = pattern;

}

void zlog_init(const char* log_name){

    zlog.name = log_name;
    zlog.level = L_INFO;
    zlog.flags = ZLOG_ALL;
    zlog.Stream = stderr;
    zlog.mode = "a";

    zlog.set_level = zlog_set_level;
    zlog.set_file_write_mode = zlog_set_file_write_mode;
    zlog.open_file = zlog_open_file;
    zlog.clear_file = zlog_clear_file;
    zlog.close_stream = zlog_close_stream;
    zlog.set_output_stream = zlog_set_output_stream;
    zlog.get_flags = zlog_get_flag;
    zlog.set_flags = zlog_set_flags;
    zlog.unset_flags = zlog_unset_flags;
    zlog.flip_flags = zlog_flip_flags;
    zlog.set_pattern = zlog_set_pattern;

    zlog.set_pattern("{D}/{M}/{Y} {h}:{m}:{s} | {f} @ {l} | {n} | {t} > ");
 
}

static void zlog_log_pattern(const char * filename, const char* fun_name, size_t line){

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char * pattern = zlog.pattern;

    while(*pattern){

        if(*pattern == '{'){

            pattern++;

            if(*pattern == 'D'){

                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                if(tm.tm_mday < 10){
                    fprintf(zlog.Stream, "0%d", tm.tm_mday);
                }else{
                    fprintf(zlog.Stream, "%d", tm.tm_mday);
                }

            }else if(*pattern == 'M'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                if(tm.tm_mon < 10){
                    fprintf(zlog.Stream, "0%d", tm.tm_mon + 1);
                }else{
                    fprintf(zlog.Stream, "%d", tm.tm_mon + 1);
                }

            }else if(*pattern == 'Y'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                fprintf(zlog.Stream, "%d", tm.tm_year + 1900);

            }else if(*pattern == 'h'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                if(tm.tm_hour < 10){
                    fprintf(zlog.Stream, "0%d", tm.tm_hour);
                }else{
                    fprintf(zlog.Stream, "%d", tm.tm_hour);
                }

            }else if(*pattern == 'm'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                if(tm.tm_min < 10){
                    fprintf(zlog.Stream, "0%d", tm.tm_min);
                }else{
                    fprintf(zlog.Stream, "%d", tm.tm_min);
                }

            }else if(*pattern == 's'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_YELLOW);
                }

                if(tm.tm_sec < 10){
                    fprintf(zlog.Stream, "0%d", tm.tm_sec);
                }else{
                    fprintf(zlog.Stream, "%d", tm.tm_sec);
                }

            }else if(*pattern == 'f'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_MAGENTA);
                }

                fprintf(zlog.Stream, "%s", fun_name);

            }else if(*pattern == 'l'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_MAGENTA);
                }

                fprintf(zlog.Stream, "%s:%zu", filename, line);

            }else if(*pattern == 'n'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", ANSI_COLOR_MAGENTA);
                }

                fprintf(zlog.Stream, "%s", zlog.name);

            }else if(*pattern == 't'){
                
                pattern++;

                if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
                    fprintf(zlog.Stream, "%s", log_color[zlog.level]);
                }

                fprintf(zlog.Stream, "[%s]", log_tag[zlog.level]);

            }

            if(*pattern != '}'){
                fprintf(stderr, ANSI_COLOR_RED"[FATAL]"ANSI_COLOR_RESET" Invalid pattern: missing closing bracket in %s @ %s:%zu", fun_name, filename, line);
                exit(1);
            }

        }else {

            fwrite(pattern, 1, 1, zlog.Stream);

        }

        pattern++;

        if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)) fprintf(zlog.Stream, "%s", ANSI_COLOR_RESET);

    }

}


void zlog_(const char * filename, size_t line, const char * fun_name, const char* fmt, ...){
    
    if(!(CHECK_FLAG(ZLOG_BIT_DEBUG)) && zlog.level == L_DEBUG) return;

    zlog_log_pattern(filename, fun_name, line);

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vfprintf(zlog.Stream, fmt, arg_ptr);
    va_end(arg_ptr);
    
}

#endif /* ZLOG_IMPLEMENTATION */