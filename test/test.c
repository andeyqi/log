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
#include "logm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TEST_MODULE_ID 0

#define logm_debug(fmt, ...)         dolog2file(LOG_DEBUG,TEST_MODULE_ID,fmt,##__VA_ARGS__)
#define logm_info(fmt, ...)          dolog2file(LOG_INFO,TEST_MODULE_ID,fmt,##__VA_ARGS__)
#define logm_warning(fmt, ...)       dolog2file(LOG_WARNING,TEST_MODULE_ID,fmt,##__VA_ARGS__)
#define logm_err(fmt, ...)           dolog2file(LOG_ERR,TEST_MODULE_ID,fmt,##__VA_ARGS__)

void * start_routine (void *data)
{
    while(1)
    {
        logm_debug("<thread id is %lu>\n",pthread_self());
        //usleep(100*1000);
    }
}

int main(int argc,char ** argv)
{
    
 int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    pthread_t pid;
    int i = 0;
    for(i = 0 ; i < 10 ; i++)
    {
        pthread_create(&pid,NULL,start_routine,NULL);
        if(pid < 0)
        {
            printf("create thread failed i = [%d]\n",i);
        }
            
    }
    while(1)
    {
        logm_debug("hello %d\n",1);
        logm_info("hello %d\n",2);         
        logm_warning("hello %d\n",3);       
        logm_err("hello %d\n",4); 
        //sleep(1);
    }
    return 0;
}
