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
    ZLOG_BIT_TIME = 0,
    ZLOG_BIT_LOCATION = 1,
    ZLOG_BIT_FUNCTION = 2,
    ZLOG_BIT_DEBUG = 3,
    ZLOG_BIT_USE_COLORS = 4
}LogBitFlags;

/*!  
    bit field that contains the flags for the log system

    0   0   0   0         0        0           0           0 
                |         |        |           |           |
                COLORS    DEBUG    FUNCTION    LOCATION    TIME

    ZLOG_TIME = SHOW THE TIME WHEN THE MESSAGE HAS BEEN LOGGED
    ZLOG_LOCATION = SHOW THE FILE LOCATION AND THE LINE WHERE THE MESSAGE HAS BEEN LOGGED
    ZLOG_FUNCTION = SHOW THE FUNCTION WHERE THE MESSAGE HAS BEEN LOGGED
    ZLOG_DEBUG = SHOW THE MESSAGE LOGGED WITH A LOG LEVEL SET TO DEBUG MODE
    ZLOG_USE_COLORS = LOG THE MESSAGE AND THE OTHER INFORMATIONS WITH COLORS, ONLY ON THE CONSOLE AND NOT THE FILE MODE
*/
typedef enum flags{
    ZLOG_TIME = 1 << ZLOG_BIT_TIME,
    ZLOG_LOCATION = 1 << ZLOG_BIT_LOCATION,
    ZLOG_FUNCTION = 1 << ZLOG_BIT_FUNCTION,
    ZLOG_DEBUG = 1 << ZLOG_BIT_DEBUG,
    ZLOG_USE_COLORS = 1 << ZLOG_BIT_USE_COLORS,
    ZLOG_ALL = ZLOG_TIME | ZLOG_LOCATION | ZLOG_FUNCTION | ZLOG_USE_COLORS
}LogFlags;

/*!
    Struct that contains every bit of information about the log system and its functions

    @param level the current log level of the logger
    @param flags the bitfield that contains all the flags used by the logger
    @param mode the mode in which the logger will print the message in the file:
                - "a" to append the message to the file.
                - "w" to overwrite the file.
    @param Stream the current stream in which the logger is printing the message

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
    @
*/
typedef struct {

    LogLevel level;
    uint8_t flags;
    const char * mode;
    FILE* Stream;
    
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

}zlogger;

/*
    The logger used globally
*/

zlogger zlog;

/*
    Macro used to check a specific bit mask between the flags 
*/

#define CHECK_FLAG(flag) (zlog.flags & (1 << flag))

/*
    Function that initialize the logger
*/

void zlog_init();

/*
    Base function to log a message to the console or a file
*/

void zlog_(const char* filename, size_t line, const char* fun_name, const char* fmt, ...);

/*
    Macro that will log a message to the console at the current log level defined 
*/

#define zlog(...)                       zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)   

/*
    Macro that will log a message to a file at the current log level defined 
*/

#define zflog(output_file, ...)         zlog.open_file(output_file); \
                                        zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
                                        zlog.close_stream()

/*
    Macro that will log a message to the console with a specified level 
*/

#define _zlog(level, ...)   zlog.set_level(level);\
                            zlog.set_output_stream(stderr);\
                            zlog(__VA_ARGS__)

/*
    Macro that will log a message to a file with a specified level
*/                            

#define _zflog(output_file, level, ...)     zlog.set_level(level); \
                                            zflog(output_file, __VA_ARGS__) \
                
#define zlog_info(...)      _zlog(L_INFO,    __VA_ARGS__)
#define zlog_debug(...)     _zlog(L_DEBUG,   __VA_ARGS__)
#define zlog_trace(...)     _zlog(L_TRACE,   __VA_ARGS__)
#define zlog_warning(...)   _zlog(L_WARNING, __VA_ARGS__)
#define zlog_error(...)     _zlog(L_ERROR,   __VA_ARGS__)
#define zlog_fatal(...)     _zlog(L_FATAL,   __VA_ARGS__)

#define zflog_info(output_file, ...)    _zflog(output_file,  L_INFO,      __VA_ARGS__)
#define zflog_debug(output_file, ...)   _zflog(output_file,  L_DEBUG,     __VA_ARGS__)
#define zflog_trace(output_file, ...)   _zflog(output_file,  L_TRACE,     __VA_ARGS__)
#define zflog_warning(output_file, ...) _zflog(output_file,  L_WARNING,   __VA_ARGS__)
#define zflog_error(output_file, ...)   _zflog(output_file,  L_ERROR,     __VA_ARGS__)
#define zflog_fatal(output_file, ...)   _zflog(output_file,  L_FATAL,     __VA_ARGS__)

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

    zlog.unset_flags(ZLOG_USE_COLORS);

}

static void zlog_close_stream(){
    fclose(zlog.Stream);
    zlog.Stream = stderr;
}

static void zlog_set_output_stream(FILE* Stream){
    zlog.Stream = Stream;

    if(zlog.Stream == stderr || zlog.Stream == stdout){
        zlog.set_flags(ZLOG_USE_COLORS);
    } 
}

static void zlog_clear_file(const char* filename){

    FILE *fp = fopen(filename, "w");
    fclose(fp);

}

void zlog_init(){
    
    zlog.level = L_INFO;
    zlog.flags = 0;
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
 
}

static void zlog_time(){

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
        fprintf(zlog.Stream, "%s%d%s/%s%d%s/%s%d%s %s%d%s:%s%d%s:%s%d%s | ",    ANSI_COLOR_YELLOW, tm.tm_mday, ANSI_COLOR_RESET,
                                                                                ANSI_COLOR_YELLOW, tm.tm_mon + 1, ANSI_COLOR_RESET, 
                                                                                ANSI_COLOR_YELLOW, tm.tm_year + 1900, ANSI_COLOR_RESET, 
                                                                                ANSI_COLOR_YELLOW, tm.tm_hour, ANSI_COLOR_RESET, 
                                                                                ANSI_COLOR_YELLOW, tm.tm_min, ANSI_COLOR_RESET, 
                                                                                ANSI_COLOR_YELLOW, tm.tm_sec, ANSI_COLOR_RESET);
    }else {
        fprintf(zlog.Stream, "%d/%d/%d %d:%d:%d | ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour,tm.tm_min, tm.tm_sec);
    }
}

static void zlog_location(const char* filename, size_t line){

    if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
        fprintf(zlog.Stream, "%s%s%s:%s%zu%s | ", ANSI_COLOR_MAGENTA, filename, ANSI_COLOR_RESET, 
                                    ANSI_COLOR_MAGENTA, line, ANSI_COLOR_RESET);
    }else {
        fprintf(zlog.Stream, "%s:%zu | ", filename, line);
    }

}

static void zlog_location_and_function(const char* filename, size_t line, const char* fun_name){

    if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
        fprintf(zlog.Stream, "%s%s @ %s%s:%s%zu%s | ", ANSI_COLOR_MAGENTA, fun_name, filename, ANSI_COLOR_RESET, 
                                    ANSI_COLOR_MAGENTA, line, ANSI_COLOR_RESET);
    }else {
        fprintf(zlog.Stream, "%s @ %s:%zu | ", fun_name, filename, line);
    }

}

static void zlog_function(const char* fun_name){

    if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){
        fprintf(zlog.Stream, "%sIn function%s : %s@%s%s | ", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET , ANSI_COLOR_MAGENTA, fun_name, ANSI_COLOR_RESET);
    }else {
       fprintf(zlog.Stream, "In function : @%s | ", fun_name); 
    }
}


void zlog_(const char * filename, size_t line, const char * fun_name, const char* fmt, ...){
    
    if(!(CHECK_FLAG(ZLOG_BIT_DEBUG)) && zlog.level == L_DEBUG) return;

    if(CHECK_FLAG(ZLOG_BIT_TIME)){
        zlog_time();
    }

    if(CHECK_FLAG(ZLOG_BIT_LOCATION) && CHECK_FLAG(ZLOG_BIT_FUNCTION)){
        zlog_location_and_function(filename, line, fun_name);
    }else if(CHECK_FLAG(ZLOG_BIT_LOCATION)){
        zlog_location(filename, line);
    }else if(CHECK_FLAG(ZLOG_BIT_FUNCTION)){
        zlog_function(fun_name);
    }

    if(CHECK_FLAG(ZLOG_BIT_USE_COLORS)){  
        fprintf(zlog.Stream, "%s[%s] "ANSI_COLOR_RESET"> ", log_color[zlog.level], log_tag[zlog.level]);
    }else {
        fprintf(zlog.Stream, "[%s] > ", log_tag[zlog.level]);

    }

    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vfprintf(zlog.Stream, fmt, arg_ptr);
    va_end(arg_ptr);
    
}

#endif /* ZLOG_IMPLEMENTATION */