#ifndef THREADLOCAL_SINGLETON
#define THREADLOCAL_SINGLETON

#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class Delete;

template<typename T>
class thread_singleton
{
	public:
		static T& instance()			//这里没必要考虑val的线程安全性，因为每一个thread 都有一个val的对象
		{
			if(val == NULL)
			{
				val = new T();
				return *val;
			}
			else
			{
				return *val;
			}
		}	

	private:
		thread_singleton() = delete;
		thread_singleton(const T&) = delete;
		~thread_singleton() = delete;

		static void distructor(void *obj)
		{
			assert(obj == val);
			
			typedef char tc[sizeof(T)==0? -1:1];
			tc dummy; (void) dummy;

			delete  val;
			val = 0;
		}

	class Deleter
	{
		public:
			Deleter()
			{
				pthread_key_create(&pkey, &thread_singleton::distructor);
			}
/*这里value 没有必要了，因为instance 已经能够代替value来访问 __thread *val 了			
			T& value()
			{
				T* temp;
				temp = pthread_getspecific(pkey);
				if(temp == NULL)
				{
					temp =new T();
					pthread_setspecific(pkey, temp);
					return *temp;
				}
				else
					return *temp;
			}*/

			void set(T *obj)
			{
				assert(pthread_getspecific(pkey) == NULL);
				pthread_setspecific(pkey, obj);
			}

			~Deleter()
			{
				pthread_key_delete(pkey);
			}

		private:
			pthread_key_t pkey;
	};


	static __thread T* val;			//这里声明为线程局部变量
	static Deleter delete_;			//这里的deleter 其实只是起到了一个 提供析构函数的作用
};

template<typename T>
__thread T* thread_singleton<T>::val = 0;

template<typename T>
typename thread_singleton<T>::Deleter thread_singleton<T>::delete_ = typename thread_singleton<T>::Deleter();


#endif

int main()
{
	return 0;
}