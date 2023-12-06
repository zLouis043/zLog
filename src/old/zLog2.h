#pragma once 

#include <stdint.h>

#define ANSI_COLOR_RED     "\x1b[0;31m"
#define ANSI_COLOR_GREEN   "\x1b[0;32m"
#define ANSI_COLOR_YELLOW  "\x1b[0;33m"
#define ANSI_COLOR_BLUE    "\x1b[0;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN    "\x1b[0;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum {
    L_INFO = 0,
    L_DEBUG,
    L_TRACE,
    L_WARNING,
    L_ERROR,
    L_FATAL
}LogLevel;

static const char * log_tag[] = {
    [L_INFO] = "INFO",
    [L_DEBUG] = "DEBUG",
    [L_TRACE] = "TRACE",
    [L_WARNING] = "WARNING",
    [L_ERROR] = "ERROR",
    [L_FATAL] = "FATAL"
};

static const char * log_color[] = {
    [L_INFO] = ANSI_COLOR_GREEN,
    [L_DEBUG] = ANSI_COLOR_YELLOW,
    [L_TRACE] = ANSI_COLOR_CYAN,
    [L_WARNING] = ANSI_COLOR_YELLOW,
    [L_ERROR] = ANSI_COLOR_RED,
    [L_FATAL] = ANSI_COLOR_RED
};

typedef enum BIT_flags{
    ZLOG_BIT_TIME = 0,
    ZLOG_BIT_LOCATION = 1,
    ZLOG_BIT_FUNCTION = 2,
    ZLOG_BIT_DEBUG = 3,
    ZLOG_BIT_USE_COLORS = 4
}LogBitFlags;

/*  
    bit field that contains the flags for the log system

    0   0   0   0         0        0           0           0 
                |         |        |           |           |
                COLORS    DEBUG    FUNCTION    LOCATION    TIME
*/

typedef enum flags{
    ZLOG_TIME = 1 << ZLOG_BIT_TIME,
    ZLOG_LOCATION = 1 << ZLOG_BIT_LOCATION,
    ZLOG_FUNCTION = 1 << ZLOG_BIT_FUNCTION,
    ZLOG_DEBUG = 1 << ZLOG_BIT_DEBUG,
    ZLOG_USE_COLORS = 1 << ZLOG_BIT_USE_COLORS,
    ZLOG_ALL = ZLOG_TIME | ZLOG_LOCATION | ZLOG_FUNCTION | ZLOG_USE_COLORS
}LogFlags;

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

extern zlogger zlog;

#define CHECK_FLAG(flag) (zlog.flags & (1 << flag))

void zlog_init();
void zlog_(const char* filename, size_t line, const char* fun_name, const char* fmt, ...);

#define zlog(...)                       zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)   

#define zflog(output_file, ...)         zlog.open_file(output_file); \
                                        zlog_(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);\
                                        zlog.close_stream()

#define _zlog(level, ...)   zlog.set_level(level);\
                            zlog.set_output_stream(stderr);\
                            zlog(__VA_ARGS__)
                            

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

