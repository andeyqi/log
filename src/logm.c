/*
 * This file is part of the log Library.
 *
 * Copyright (c) 2016-2017, andeyqi, <qihengchuan8888@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Initialize function and other general function.
 * Created on: 2018-4-11
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "logm_def.h"
#include "logm_cfg.h"
#include <pthread.h>

logm_struct_p g_logm_obj_p;

/**
 *
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

static int log2file_init(logm_tcb_t * param)
{  
    int fd,len;
    char* temp_file_path;
    
    /*check log output path*/
    if(access(g_logm_obj_p->tcb.path,F_OK))
    {
        printf("path err\n");
        return -1;
    }
    len = strlen(g_logm_obj_p->tcb.path)+strlen(g_logm_obj_p->tcb.file_name)+1;
    temp_file_path = (char*)malloc(len);
    snprintf(temp_file_path,len,"%s%s",g_logm_obj_p->tcb.path,g_logm_obj_p->tcb.file_name);
    /* open log output file  */
    fd = open(temp_file_path,O_RDWR | O_CREAT | O_TRUNC, 0777);
    if(fd < 0)
    {
        return -1;
    }
    g_logm_obj_p->fd = fd;
    g_logm_obj_p->is_initd = 1;
    free(temp_file_path);
    return 0;  
}

/**
 *
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

static int log2file_cleanup(logm_tcb_t * param)
{
#if 0   
    fd = open(LOG_PATH,O_RDWR | O_CREAT | O_TRUNC, 0777);
    if(fd < 0)
    {
        return -1;
    }
    gLogCtrlObj.fd = fd;
    return 0;
#endif  
}

/**
 * Write log to file.
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

int dolog2file (logm_loglevel_t level,const char *format, ...)
{  
    static const char * const logLeverStr[] = {"DEBUG","INFO","WARNING","ERROR"};
    char buff[PER_LOG_LIMIT] = {0};
    int len = 0;
    
    if(!g_logm_obj_p->is_initd)
    {
        g_logm_obj_p->init(NULL);
    }
    struct  timeval tv;
    /* Get system time */
    gettimeofday(&tv,NULL);
    va_list args;
    len = snprintf(buff,PER_LOG_LIMIT,"[%ld:%ld][%s]",tv.tv_sec,tv.tv_usec/1000,logLeverStr[level]);
    /* Package data */
    va_start(args, format);
    len += vsnprintf(&buff[len],PER_LOG_LIMIT-len,format,args);
    va_end(args);
    /* Write to file */
    if(g_logm_obj_p->tcb.level <= level)
    {
        if((len + lseek(g_logm_obj_p->fd, 0L, SEEK_CUR)) > g_logm_obj_p->tcb.len_limit)
        {
            lseek(g_logm_obj_p->fd, 0L, SEEK_SET);
        }
        write(g_logm_obj_p->fd,buff,len);
    }
    return 0;
}

logm_struct_t g_logm_obj = {
    .tcb = {
            .len_limit  = 2048,
            .path       = "./",
            .file_name  = "logm.log",
            .level      = LOG_DEBUG,
        },
    .fd = -1,
    .is_initd = 0,
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .init       = log2file_init,
    .cleanup    = log2file_cleanup,
    //.logout     = dolog2file,
};
logm_struct_p g_logm_obj_p = &g_logm_obj;
