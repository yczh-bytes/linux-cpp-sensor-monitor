
#include "../include/sensor_data.h"
#include "../include/thread_safe_queue.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>
#include <fstream>
#include <filesystem>


//时间戳生成函数
std::string getTimestamp()
{
 //获取当前时间戳
    auto now = std::chrono::system_clock::now();
    //转换为time_t的格式
    std::time_t time_now_t = std::chrono::system_clock::to_time_t(now);
    //转换为localtime形式
    std::tm* local_tm = std::localtime(&time_now_t);
  
    //使用stringstream进行格式化转化
    std::stringstream oss;
    oss<<std::put_time(local_tm,"%Y-%m-%d %H:%M:%S");
    return oss.str();
}
//获取温度随机数
double getTemperature()
{
    //生成随机种子
    static std::random_device rd;
    //梅森螺旋
    static std::mt19937 gen(rd());
    //确认生成随机数范围
    std::uniform_real_distribution<>distrib(20,40);
    return distrib(gen);
}
void sensorProducer(ThreadSafeQueue<_data>& q)
{
    //生产20条数据
    for(int i=0;i<20;i++)
    {
        _data d;
        int id = 1;
    std::string timeStamp = getTimestamp();
    double tempu = getTemperature(); 
    //依次赋值
    d.id = id;
    d.timestamp = timeStamp;
    d.value = tempu;
    d.unit = "C";
    //休眠
     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    q.push(d);
    }
    //关闭
    q.close();
   

}

void sensorComsumer(ThreadSafeQueue<_data>& q, std::ofstream& logFile)
{
    //写入日志文件
    _data d;
    while(q.pop(d))
    {
        if(d.value>35.0)
        {
            std::cout<<"Temperature too high!"<<std::endl;
            logFile<<"Temperature too high!"<<std::endl;
        }
        else
        {
            std::cout<<"sensorid:"<<d.id<<","<<d.timestamp<<","<<d.value<<","<<d.unit<<std::endl;
            logFile<<"sensorid:"<<d.id<<","<<d.timestamp<<","<<d.value<<","<<d.unit<<std::endl;
        }
    }
}

int main()
{
    //创建 logs 目录
    std::filesystem::create_directory("../logs");
    
    //打开日志文件
    std::ofstream logFile("../logs/sensor_data.log", std::ios::app);
    
    if(!logFile.is_open())
    {
        std::cerr<<"Failed to open log file"<<std::endl;
        return 1;
    }
    
    ThreadSafeQueue<_data> q;
    sensorProducer(q);
    sensorComsumer(q, logFile);
    
    //关闭日志文件
    logFile.close();
    
    return 0;
}