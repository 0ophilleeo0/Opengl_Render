#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
public:
    static Singleton* GetInstacnce(){
        if (m_singleton == nullptr) {
            m_singleton = new Singleton;
        }

        return m_singleton;
    }

private:
    Singleton(){}
    ~Singleton(){}

    Singleton(const Singleton&);
    const Singleton& operator=(const Singleton);

private:
    static Singleton* m_singleton;
};

Singleton* Singleton::m_singleton = nullptr;

#endif