#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "zLog.h"

zlogger zlog = {0};

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