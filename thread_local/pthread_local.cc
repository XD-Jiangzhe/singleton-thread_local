/*
	这是自己封装的一个线程的私有成员类，将底层的
	pthread_key_create
	pthread_key_destroy
	pthread_setspecific
	pthread_getspecific
	函数封装成了一个类
*/
#ifndef PTHREAD_LOCAL_HPP
#define PTHREAD_LOCAL_HPP

#include <pthread.h>
#include <iostream>
#include <functional>
#include <thread>

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>  

template <typename T>//这里存储的线程私有成员类型为T
class pthread_local
{
	public:

		pthread_local()
		{
//			pthread_key_create(&pkey, std::bind(&pthread_local::distructor, this, std::placeholders::_1));
			pthread_key_create(&pkey, distructor);		//distructor 在离开线程的时候被调用，参数只有一个，就是key所对应的堆中的地址
		}
		~pthread_local()
		{
			pthread_key_delete(pkey);
		}

		T& value()
		{
			T* va;
			if((va= reinterpret_cast<T*>(pthread_getspecific(pkey))) == NULL)
			{
				va = new T();
				pthread_setspecific(pkey, (void *)va);			//这里要进行绑定，将起绑定在一个空的对象上
				return *va;
			}
			else
				return *va;
		}

		void  setval(T a)
		{
			pthread_setspecific(pkey, &a);
		}

	private:
		 static void distructor(void *x)		//因为 pthread_key_create 中赋值给key的都是  堆内存，所以可以这样写,key 指向了一块每个 线程自己申请的堆内存
		//这就是为什么要加static的原因，static 函数没有this指针，而上面的那个函数是不能有this参数的
		{
			T* obj = reinterpret_cast<T*>(x);
			typedef char T_is_a_complete_class[sizeof(T)== 0? -1: 1];
			T_is_a_complete_class dummy; (void ) dummy;
			delete obj;
		}

	private:
		pthread_key_t pkey;		

};

#endif
using namespace std;

class Test
{
	public:
		Test()
		{
			pid_t ttid = syscall(186);//这里指的是ttid的获取。224 是 系统调用的号码
			printf("tid = %d, constructing\n", ttid);
		}
		~Test()
		{
			printf("tid = %ld\n distructing\n", syscall(186));
		}
		const std::string getname()const{return name;}

		void setname(const string n){name = n;}

	private:
		std::string name;
};

pthread_local<Test> testobj1;
pthread_local<Test> testobj2;

void print()
{

}

pthread_local<int> a;

void add()
{
	testobj1.value().setname("hahahah");
	cout<<testobj1.value().getname()<< " tid: ";
	printf("%ld\n", (long int)syscall(SYS_gettid));
}

int main()
{
	thread t(add);
	
	testobj1.value().setname("sb");
	cout<<testobj1.value().getname()<<" tid:";
	printf("%ld\n",(long int)syscall(SYS_gettid) );
	


	t.join();
	return 0;
}







