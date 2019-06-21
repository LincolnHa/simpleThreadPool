#include "mythreadpool.h"
#include "mutex"
#include "pthread.h"

//初始化线程池
ThreadPool::ThreadPool(int threadNum, int maxJobNum)
{
    m_threadNum = threadNum;
    m_maxJobNum = maxJobNum;
    stop = false;
    if (m_threadNum > 100)
    {
        m_threadNum = 100;
    }

    if (m_maxJobNum > 2000)
    {
        m_maxJobNum = 2000;
    }

    //开始创建多个线程, 并且各个线程等待 任务进来, 如果有任务则争抢然后 执行任务
    for (int i = 0; i < m_threadNum; i++)
    {
        m_threads.emplace_back(
            [this] {
                std::cout <<"thisid:" <<std::this_thread::get_id() <<"\n";
                //正常运行, 非停止
                while (!stop)
                { 
                    
                    //将要执行的任务
                    BaseTask *task;

                    //区域锁, lock出了大括号就 会解锁
                    {
                        std::unique_lock<std::mutex> lock(this->m_mutex);

                        //等待唤起
                        this->condition.wait(lock,
                                             [this] { return this->stop || !this->m_QueTask.empty(); }); //如果第二个参数为true, 则将不阻塞(为的是在设置了 stop 后, 能把剩余的任务执行完, 然后才结束)

                        //唤起后该线程池已设置stop了, 并且所有任务已经执行完了, 则退出
                        if (this->stop && this->m_QueTask.empty())
                            return;

                        //从队列里获取 任务
                        task = this->m_QueTask.front().get();  
                        
                        this->m_QueTask.pop();
                    }
 
                    //执行任务
                    if(task == NULL){
                        printf("task is null");
                    }

                    task->Do();
                }
            });
    }
}

//添加任务
int ThreadPool::AddJob(std::shared_ptr<BaseTask> task)
{
    m_mutex.lock();

    //超过最大任务数
    if (m_QueTask.size() >= m_maxJobNum)
    {
        return -2;
    }

    m_mutex.unlock();

    //入队
    m_QueTask.emplace(task); 
    //printf("添加任务, 数量:%d", m_QueTask.size());

    //唤起一个线程
    condition.notify_one();

    return 0;
}

//停止线程池
void ThreadPool::Stop()
{
    stop = true;
    condition.notify_all();

    for (int i = 0; i < m_threadNum; i++)
    {
        m_threads[i].join();
    }
}

ThreadPool::~ThreadPool()
{
    Stop();
}