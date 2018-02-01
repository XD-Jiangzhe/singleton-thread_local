#singleton-thread_local
使用pthread 一系列函数 封装一个singleton 对象，和thread_local 对象 及其两者结合的实现

对linux下的singleton设计模式中的singleton类的简单实现

使用了pthread_once 函数，类中所有的成员和方法都是静态，保证了在main之前就对其进行初始化

singleton对象 是全局单例对象， 只有一个对外接口，无析构无拷贝和初始化构造函数
在init()的时候需要有一个 atexit 函数 设置它的析构函数，在程序结束的时候或是退出的时候调用析构函数


封装thread_local即 线程私有成员的时候，使用了一下函数
      
       #include <pthread.h>

       int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *));

       int pthread_key_delete(pthread_key_t key);

       int pthread_setspecific(pthread_key_t key, const void *pointer);

       void * pthread_getspecific(pthread_key_t key);
将key 设置为静态
