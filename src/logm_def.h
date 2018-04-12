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
#ifndef __LOGM_DEF_H__
#define __LOGM_DEF_H__

#include "logm.h"

#define PER_LOG_LIMIT   (200)
#define KB              (1024)

typedef struct logm_tcb{
    int              len_limit;
    logm_loglevel_t  level;
    char*            path;
    char*            file_name;
}logm_tcb_t,*logm_tcb_p;

typedef struct logm_struct{
    logm_tcb_t tcb; 
    int fd;
    pthread_mutex_t lock;
    int is_initd;
    int (*init)(logm_tcb_p);
    int (*cleanup)(logm_tcb_p);
    int (*ctrl)(unsigned int cmdid,void * data);
    //int (*logout)(logm_loglevel_t level,const char *format, ...);
}logm_struct_t,*logm_struct_p;

#endif /* end of __LOGM_DEF_H__ */
