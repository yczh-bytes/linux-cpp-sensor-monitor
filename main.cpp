#include <iostream>
#include <thread>

int main()
{
    std::cout<<"hello world"<<std::endl;
    std::thread t([](){
        std::cout<<"Thread is running in wsl"<<std::endl;
    });
    t.join();
    return 0;
}