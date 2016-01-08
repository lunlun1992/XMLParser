//
//  thread_pool.c
//  XMLParser_
//
//  Created by cpdp on 16/1/8.
//  Copyright (c) 2016年 cpdp. All rights reserved.
//

#include "thread_pool.h"


void *thread_routine(void *arg)
{
    CThreadTask *task;
    CThreadPool *pool = (CThreadPool*)arg;
    while (1)
    {
        pthread_mutex_lock(&(pool->queue_lock));
        // wait until task queue is not empty
        while (!pool->shutdown && 0 == pool->cur_queue_size)
        {
            pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
        }
        
        // exit right now because data will be released after shutdown
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&(pool->queue_lock));
            pthread_exit(0);
        }
        
        // take a task from queue
        pool->cur_queue_size--;
        task = pool->queue_head;
        pool->queue_head = task->next;
        pthread_mutex_unlock(&(pool->queue_lock));
        
        // execute the task
        (*(task->process))(task->arg);
        free(task);
        task = NULL;
    }
}


CThreadPool* pool_init(int max_thread_num)
{
    int i;
    CThreadPool *pool = (CThreadPool *)malloc(sizeof(CThreadPool));
    
    pool->shutdown = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->cur_queue_size = 0;
    
    pthread_mutex_init(&(pool->queue_lock), NULL);
    pthread_cond_init(&(pool->queue_ready), NULL);

    pool->max_thread_num = max_thread_num;
    pool->threads = (pthread_t *)malloc(max_thread_num * sizeof(pthread_t));
    for (i = 0; i < max_thread_num; i++)
    {
        pthread_create(&(pool->threads[i]), NULL, thread_routine, pool);
    }
        
    return pool;
}


void pool_add_task(CThreadPool *pool, void *(*process)(void *arg), void *arg)
{
    CThreadTask *task = (CThreadTask *)malloc(sizeof(CThreadTask));
    task->process = process;
    task->arg = arg;
    task->next = NULL;
    
    // add a task to the queue
    pthread_mutex_lock(&(pool->queue_lock));
    if (pool->queue_head != NULL)
    {
        pool->queue_tail->next = task;
        pool->queue_tail = task;
    }
    else
    {
        pool->queue_head = pool->queue_tail = task;
    }
    
    pool->cur_queue_size++;
    pthread_mutex_unlock(&(pool->queue_lock));
    pthread_cond_signal(&(pool->queue_ready)); // siganl threads that task waiting
}


void pool_destroy(CThreadPool *pool)
{
    int i;
    CThreadTask *task;
    if (pool->shutdown)
        return;
    
    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->queue_ready)); // wakeup all threads waiting for tasks
    
    for (i = 0; i < pool->max_thread_num; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    
    i = 1;
    while (pool->queue_head != NULL)
    {
        task = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        
        (*(task->process))(task->arg);
        free(task);
    }
    pthread_mutex_destroy(&(pool->queue_lock));
    pthread_cond_destroy(&(pool->queue_ready));
    
    free(pool);
    pool = NULL;
}

