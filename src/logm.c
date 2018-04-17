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
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "logm_def.h"
#include "logm_cfg.h"
#include <sys/mman.h>

#ifdef USE_TIME_MODE_YY_MM_DD_HH_MM_MM
#include <sys/timeb.h>
#endif

logm_struct_p g_logm_obj_p;

#ifdef USE_MONITOR_TASK
int log2file_ctrl(int cmdid,void* data);

/**
 * Task to control log status.
 *
 * @param data task parameter.
 * @return NULL
 */
 
void* logtofile_monitor_task(void * data)
{
    pthread_detach(pthread_self());
    int fd,ret;
    char* pstr;
    logm_loglevel_t current_level,level; 
    
    fd = shm_open( "logm", (O_RDWR | O_CREAT ), 0666 );
    if( fd < 0 )
    {
        return NULL;
    }
    
    ret = ftruncate(fd, 1024);
    if( ret < 0 )
    {
        return NULL;
    }
    
    pstr = (char*)mmap( NULL, 1024, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    if( pstr == (char*)((void *)-1) )
    {
        return NULL;
    }
    
    while(1)
    {
        sscanf(pstr,"%d",(int*)&level);
        if(!log2file_ctrl(CMD_ID_GET_LOG_LEVEL,(void*)&current_level))
        {
            if(level != current_level)
                log2file_ctrl(CMD_ID_SET_LOG_LEVEL,(void*)&level);
        }
        
        usleep(100*1000);
    }
    return NULL;
}
#endif
/**
 * Init log manager
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
#ifdef USE_MONITOR_TASK
    {
        pthread_t tid;
        pthread_create(&tid,NULL,logtofile_monitor_task,NULL);
    }
#endif
    free(temp_file_path);
    return 0;  
}

/**
 *  Clean up log manager
 *
 * @param level log level
 * @param format Format of the format string
 * @return write data length
 */

static int log2file_cleanup(logm_tcb_t * param)
{
    if((g_logm_obj_p->fd) > 0)
    {
        close(g_logm_obj_p->fd);
        g_logm_obj_p->fd = -1;
    }
    return 0;
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
    pthread_mutex_lock(&g_logm_obj_p->lock);
    if(!g_logm_obj_p->is_initd)
    {
        g_logm_obj_p->init(NULL);
    }
#ifndef USE_TIME_MODE_YY_MM_DD_HH_MM_MM
    struct  timeval tv;
    /* Get system time */
    gettimeofday(&tv,NULL);
    len = snprintf(buff,PER_LOG_LIMIT,"[%ld:%ld][%s]",tv.tv_sec,tv.tv_usec/1000,logLeverStr[level]);
#else
    struct tm * ptm;
    struct timeb curr;
    ftime(&curr);
    ptm = localtime(&curr.time);
    len = snprintf(buff,PER_LOG_LIMIT,"[%4d-%02d-%02d %02d:%02d:%02d :%3d][%s]",ptm->tm_year + 1900, ptm->tm_mon + 1,\
                    ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec,curr.millitm,logLeverStr[level]);
#endif
    va_list args;
    /* Package data */
    va_start(args, format);
    len += vsnprintf(&buff[len],PER_LOG_LIMIT-len,format,args);
    va_end(args);
    /* Write to file */
    if(g_logm_obj_p->tcb.level <= level)
    {
        if((len + lseek(g_logm_obj_p->fd, 0L, SEEK_CUR)) > g_logm_obj_p->tcb.len_limit*KB)
        {
            lseek(g_logm_obj_p->fd, 0L, SEEK_SET);
        }
        if(unlikely(len != write(g_logm_obj_p->fd,buff,len)));
            return -1;
    }
    pthread_mutex_unlock(&g_logm_obj_p->lock);
    return 0;
}

/**
 * Write log to file.
 *
 * @param cmdid command code set to log.
 * @param data The data get from log manager or set to log manager.
 * @return result of action.
 */
 
int log2file_ctrl(int cmdid,void* data)
{
    if(cmdid < CMD_ID_SET_BASE_VALUE || cmdid > CMD_ID_MAX_VALUE)
    {   
        goto para_error;
    }
    switch (cmdid)
    {
    case CMD_ID_SET_LOG_LEVEL:
    {
        if(NULL == data || *((logm_loglevel_p)data) < LOG_DEBUG || *((logm_loglevel_p)data) > LOG_ERR)
            goto para_error;
         g_logm_obj_p->tcb.level = *((logm_loglevel_p)data);
    }
        break;
    case CMD_ID_GET_LOG_LEVEL:
    {
        if(NULL == data)
            goto para_error;
        *((logm_loglevel_p)data) = g_logm_obj_p->tcb.level;
    }
        break;
    default:
        break;
    }
    return 0;
para_error:
    return -1;
}

logm_struct_t g_logm_obj = {
    .tcb = {
            .len_limit  = 2048,
            .path       = "./",
            .file_name  = "logm.log",
            .level      = LOG_DEBUG,
        },
    .name = "log2file",
    .fd = -1,
    .is_initd = 0,
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .init       = log2file_init,
    .cleanup    = log2file_cleanup,
    .logout     = dolog2file,
};
logm_struct_p g_logm_obj_p = &g_logm_obj;
