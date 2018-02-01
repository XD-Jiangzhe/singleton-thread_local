/*
	这是自己封装的一个线程的私有成员类，将底层的
	pthread_key_create
	pthread_key_destroy
	pthread_setspecific
	pthread_getspecific
	函数封装成了一个类
*/
#ifndef HEAD_TRY
#define HEAD_TRY
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

using namespace std;

template <typename T>
class pthread_local
{

	public:
		pthread_local()
		{
			pthread_key_create(&pkey, destroy); 
		}
		~pthread_local()
		{
			pthread_key_delete(pkey);
		}

		T& value()
		{
			T *va;
			va = reinterpret_cast<T*> (pthread_getspecific(pkey));
			if(va == NULL)
			{
				va = new T();
				pthread_setspecific(pkey, va);
				return *va;
			}
			else 
				return *va;
		}
	private:

		static void  destroy(void *x)
		{
			T* key = (T*)x;
			if(key == NULL)
				;
			else
				delete key;
		}
		pthread_key_t pkey;
};
/*class Test
{
	public:
		string getname(){return name;}
		void setname(const string& a){name = a;}
		void addname(const string& a){name += a;}
	private:
	string  name;
};

pthread_local<Test> pl;

void Func()
{
	pl.value().setname("hahaha sb");
	cout<<pl.value().getname()<<endl;
}*/

#endif
/*int main()
{
	thread t(Func);

	pl.value().setname("wocao ni shi sb");
	cout<<pl.value().getname()<<endl;

	t.join();

	return 0;
}
*/

