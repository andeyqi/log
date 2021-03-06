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
#ifndef __LOGM_H__
#define __LOGM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum logm_loglevel{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERR,
}logm_loglevel_t,*logm_loglevel_p;

typedef enum logm_cmdid{
    CMD_ID_SET_BASE_VALUE = 0,
    CMD_ID_SET_FILE_LIMIT = CMD_ID_SET_BASE_VALUE,              /*0:set log file max size,unit KB*/
    CMD_ID_SET_FILE_PATH,                                       /*1:set log output path*/
    CMD_ID_SET_LOG_LEVEL,                                       /*2:set log output level*/
    CMD_ID_GET_LOG_LEVEL,                                       /*3:get log level value*/
    CMD_ID_MAX_VALUE,
}logm_cmdid_t,*logm_cmdid_p;

extern int dolog2file (logm_loglevel_t level,unsigned int modid,const char *format, ...);

//extern set_logm_parameter(logm_cmdid_t cmdid,void * data);

#ifdef __cplusplus
}
#endif
 
#endif /* end of __LOGM_H__*/
 
