#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class Singleton
{
protected:
    static volatile Singleton *pSingleton;
    std::string _value;
    static std::mutex _mutex; // For Thread-safe
    Singleton(const std::string value) : _value(value)
    {
    }

public:
    Singleton(Singleton &other) = delete;

    void operator=(const Singleton &) = delete;

    static Singleton *GetInstance(const std::string &value);

    std::string value() const
    {
        return _value;
    }
};
Singleton *Singleton::pSingleton = nullptr;
std::mutex Singleton::_mutex; //  For Thread-safe
Singleton *Singleton::GetInstance(const std::string &value)
{
    if (pSingleton == nullptr)
    {
        std::lock_guard<std::mutex> lock(_mutex); //  For Thread-safe
        if (pSingleton == nullptr)
        {
            pSingleton = new Singleton(value);
        }
    }
    return pSingleton;
}

void ThreadFoo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton *singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton *singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}

int main()
{
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();
}