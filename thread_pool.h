//
//  thread_pool.h
//  XMLParser_
//
//  Created by cpdp on 16/1/8.
//  Copyright (c) 2016年 cpdp. All rights reserved.
//

#ifndef __XMLParser___thread_pool__
#define __XMLParser___thread_pool__

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
*/
#include "common.h"

typedef struct task
{
    void *(*process)(void *arg);
    void *arg;
    struct task *next;
}CThreadTask;

typedef struct
{
    CThreadTask *queue_head;
    CThreadTask *queue_tail;
    
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;
    pthread_t *threads;
    
    int shutdown;
    int max_thread_num;
    int cur_queue_size;
}CThreadPool;

CThreadPool* pool_init(int max_thread_num);
void pool_add_task(CThreadPool *pool, void *(*process)(void *arg), void *arg);
void pool_destroy(CThreadPool *pool);

#endif /* defined(__XMLParser___thread_pool__) */
