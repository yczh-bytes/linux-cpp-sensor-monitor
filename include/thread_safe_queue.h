#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue
{
private:
std::mutex _mutex;
std::queue<T> _queue;
std::condition_variable cond;
bool close_ = false;
public:
void push(const T& value)
{
    //上锁
    std::lock_guard<std::mutex> lock(_mutex);
    //查看输出通道是否关闭
    if(close_)
    {
        return;
    }

    _queue.push(value);
    //唤醒休眠队列
    cond.notify_one();
}
bool pop(T& value)
{
   //要使用unique_lock配合wait唤醒使用
    std::unique_lock<std::mutex> lock(_mutex);
     //lambda表达式等待，队列为空或者生成关闭
    cond.wait(lock,[this]{
        return close_||!_queue.empty();
    });

    //为空返回错
    if(_queue.empty())
    {
        return false;
    }
    //赋值第一个
    value=_queue.front();
    //删除第一个
    _queue.pop();
    return true;
}
//关闭队列
void close()
{
    std::lock_guard<std::mutex> lock(_mutex);
    close_=true;
    //唤醒所有队列，队列已经关闭
    cond.notify_all();
}
};

