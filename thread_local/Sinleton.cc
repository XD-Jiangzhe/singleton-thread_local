/*
此为c++设计模式中的单例模式，即全局拥有一个对象，并且它的构造函数，析构函数和拷贝构造函数都是删除的，不可能在其他地方对其进行构造

并且此类对外提供一个唯一的借口，即instance()接口，只可以初始化一次，使用pthread_once函数进行 实现

切记singleton所有的函数都必须是静态的，这样才能做到没有 对象也能调用 对象成员，因为static 成员函数 == 全局函数

*/

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <thread>
#include <iostream> 
#include "try.cc"

using namespace std;

template <typename T>
class Singleton
{
	public:
		static T& instance()			//必须是静态的 这里所有的函数都是静态的，要保证 在main 之前都已经初始化了
		{
			pthread_once(&initflag, &Singleton::init);	
			//初始化一次
			assert(val != NULL);
			return *val;			//使用一个私有的 指针指向 一个堆上的数据成员，全局都只能有这个函数来对起进行访问
		}

	private:
		Singleton()= delete;
		~Singleton()=delete;
		Singleton(const Singleton&) = delete;

		static void destroy()
		{
			typedef char T_must_be_complete_type[sizeof(T)==0 ? -1:1];
			delete val;
			val = NULL;
		}

		static pthread_once_t initflag;
		static T* val;

		static void init()
		{
			val = new T();
			atexit(destroy);		//注册一个 析构函数用来销毁 val的内存
		}
};
//对静态成员函数进行初始化

template<typename T>
pthread_once_t Singleton<T>::initflag = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::val = NULL;

class Test
{
	public:
		string getname(){return name;}
		void setname(const string& a){name = a;}
		void addname(const string& a){name += a;}
	private:
	string  name;
};

#define STL Singleton<pthread_local<Test>>::instance().value();

void Func(const string name)
{
	Test s= STL;
	s.addname(name);
	cout<<s.getname()<<endl;
}

#include <functional>
int main()
{

	Test s= STL;
	s.setname("main thread");
	cout<<s.getname()<<endl;

	thread t(bind(Func, "thread1"));
	thread t1(bind(Func, "thread2"));


	t1.join();
	t.join();
}


/*int main()
{
	thread t(Func);

	Singleton<Test>::instance().addname(" you are sb");

	t.join();

	cout<<Singleton<Test>::instance().getname()<<endl;

	return 0;
}*/


