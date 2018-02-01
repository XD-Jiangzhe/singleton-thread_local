#singleton-thread_local

使用pthread 一系列函数 封装一个singleton 对象，和thread_local 对象 及其两者结合的实现

对linux下的singleton设计模式中的singleton类的简单实现

使用了pthread_once 函数，类中所有的成员和方法都是静态，保证了在main之前就对其进行初始化

       #include <pthread.h>

       pthread_once_t once_control = PTHREAD_ONCE_INIT;

       int pthread_once(pthread_once_t *once_control, void (*init_routine) (void));
           
       PTHREAD_ONCE_INIT

singleton对象 是全局单例对象， 只有一个对外接口，无析构无拷贝和初始化构造函数
在init()的时候需要有一个 atexit 函数 设置它的析构函数，在程序结束的时候或是退出的时候调用析构函数



封装thread_local模板类，即 线程私有成员的时候，使用了一下函数
      
       #include <pthread.h>

       int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *));

       int pthread_key_delete(pthread_key_t key);

       int pthread_setspecific(pthread_key_t key, const void *pointer);

       void * pthread_getspecific(pthread_key_t key);
将以上四个函数进行了封装来管理 跟key绑定的对象

数据成员只有一个key， 通过setval 和value 来设置和获取跟这个key绑定的 堆内存，切记！！！是堆内存，所以要有析构，在创建key的时候
要指定 析构函数，static deleter，如果不是static 的话要使用bind 来绑定为 类的函数对象，每次都忘记是类方法，deleter的参数切记一定要是对象的地址
即 &T ,然后再deleter中直接释放

std::bind(&thread_local::deleter, this ,std::placeholders::_1);

最后一个是将以上两者结合起来变成了如下形式：

typedef singleton<thread_local<Test>> STL;
      
     

这种形式 指一个全局的 变量，拥有一个线程私有成员对象， 所以每个线程中都有自己的Test 的版本。
