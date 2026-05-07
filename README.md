# Linux C++ Sensor Monitor

这是一个基于 C++17 的 Linux 多线程传感器数据采集与告警系统。

## 当前功能

- 模拟温度传感器数据
- 生产者消费者模型
- 线程安全队列
- 日志文件保存
- 温度超限报警

## 技术栈

- C++17
- std::thread
- std::mutex
- std::condition_variable
- Makefile
- Linux / WSL2

## 编译运行

```bash
make run