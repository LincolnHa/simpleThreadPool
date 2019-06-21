#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <iostream>
//基础任务
class BaseTask
{
public:
    virtual void Do(){printf("BaseTask");}; //任务
};

//线程池类
class ThreadPool
{
public:
    ThreadPool(int threadNum, int maxJobNum); //线程池初始化(创建好多个线程, 并且各个线程在等待 通知, 一旦有任务, 则拿出, 然后做 任务里的 方法 do  (需条件变量控制 任务争抢))
    ~ThreadPool();                            //析构线程池
    int AddJob(std::shared_ptr<BaseTask> task);               //外部添加任务, 唤起线程池里的某个 线程.
    void Stop();                                   //停止线程池的执行(在这之前会把所有的任务执行完)

private:
    int m_threadNum; //线程数量
    int m_maxJobNum; //最大任务数

    std::vector<std::thread> m_threads; //线程数组
    std::queue<std::shared_ptr<BaseTask>> m_QueTask;   //任务队列

    std::mutex m_mutex;                //互斥量
    std::condition_variable condition; //条件变量

    bool stop; //是否停止所有线程
};

#endif