#include <iostream>

template<typename T>
class Singleton{

    public:
        static T% instance()
        {
            static T local ;
            return local;
        }
        Singleton() = delete;
        ~Singleton() = delete;
    private:

        
};