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

enum logLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERR,
};

struct log_manage_paras
{
    /* log file control start*/
    /* log file control end */
}log_manage_paras_t,*log_manage_paras_p;

struct log_manage
{
    int (*init)();
    int (*cleanup)();
    int (*ctrl)();
}log_manage_t,*log_manage_p;

/**
 *
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

int log2file_init(struct logCtrl * initObj)
{
    fd = open(LOG_PATH,O_RDWR | O_CREAT | O_TRUNC, 0777);
    if(fd < 0)
    {
        return -1;
    }
    gLogCtrlObj.fd = fd;
    return 0;
}

/**
 *
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

int log2file_cleanup(struct logCtrl * initObj)
{
    fd = open(LOG_PATH,O_RDWR | O_CREAT | O_TRUNC, 0777);
    if(fd < 0)
    {
        return -1;
    }
    gLogCtrlObj.fd = fd;
    return 0;
}

/**
 * Write log to file.
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

int dolog2file (enum logLevel level,const char *format, ...)
{
    static const char * logLeverStr[] = {"DEBUG","INFO","WARNING","ERROR"};
    char buff[PER_LOG_LIMIT] = {0};
    int len = 0;
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
    if(gLogCtrlObj.level <= level)
    {
        if((len + lseek(gLogCtrlObj.fd, 0L, SEEK_CUR)) > gLogCtrlObj.logSize)
        {
            lseek(gLogCtrlObj.fd, 0L, SEEK_SET);
        }
        write(gLogCtrlObj.fd,buff,len);
    }
    return 0;
}






