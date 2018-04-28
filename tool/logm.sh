#!/bin/bash
#set -x

domain_l=""
domain_f=""
logm_shm_fd="/dev/shm/logm"
ctrl_flag=0

logm_usage()
{
    echo "logm <-l>[0~3] <-m>[ >=0 ] <-g>"
}

get_logm_info()
{
    if [ -f ${logm_shm_fd} ];then
        cat ${logm_shm_fd}
    else
        echo "file ${logm_shm_fd} is not exist"
    fi  
}

set_logm_param()
{
    if [ -f ${logm_shm_fd} ];then
        echo "${domain_l}:${domain_f}"  > ${logm_shm_fd}
    else
        echo "file ${logm_shm_fd} is not exist"
    fi  
}

while getopts "l:f:g" opt; do  
  case $opt in  
    l)  
        domain_l=$OPTARG
        ctrl_flag=1
      ;;  
    f)  
        domain_f=$OPTARG
        ctrl_flag=1
      ;;  
    g)
      get_logm_info
      ;;  
    \?)  
      logm_usage
      ;;  
  esac  
done

if [ $ctrl_flag -eq 1 ];then
    set_logm_param
    if [ -f toolsync ];then
        ./toolsync
    else
        echo -e "\033[31m can't find logmsync file \033[0m"
    fi   
fi
