#include "mythreadpool.h"
#include "iostream"
#include "thread"
#include "stdlib.h"
#include <chrono>

class TestJob:public BaseTask{
public:
    int i;
    TestJob(int mi):i(mi){}; 
    virtual void Do(){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << std::this_thread::get_id() <<"\n";
    }
};


int main(){
    //线程池已开始
    ThreadPool thPool(10, 200);

    std::shared_ptr<BaseTask> task1 = std::make_shared<TestJob>(1);
    std::shared_ptr<BaseTask> task2 = std::make_shared<TestJob>(2);
    std::shared_ptr<BaseTask> task3 = std::make_shared<TestJob>(3);
    std::shared_ptr<BaseTask> task4 = std::make_shared<TestJob>(4);
    std::shared_ptr<BaseTask> task5 = std::make_shared<TestJob>(5);

    thPool.AddJob(task1);
    thPool.AddJob(task2);
    thPool.AddJob(task3);
    thPool.AddJob(task4);
    thPool.AddJob(task5);

    std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    return 0;
}