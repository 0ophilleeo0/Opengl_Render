#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    static T& GetInstacnce(){
        static T m_singleton;
        return m_singleton;
    }

    Singleton(const Singleton&) = delete;
    const Singleton& operator=(const Singleton) = delete;

private:
    Singleton() = default;
    ~Singleton() = default;
};

#endif