#include <iostream>
#include <thread>
#include <mutex>

int var = 10; // Recurso compartido
std::mutex mtx;

void threadA(void)
{
	while(true)
	{
		mtx.lock();
		var = var + 10;
		mtx.unlock();
	}
}

void threadB()
{
	while(true)
	{
		mtx.lock();
		var = var + 5;
		mtx.unlock();
	}
}


int main(void)
{
	std::thread t1(threadA);}
	std::thread t2(threadB);
	
	t1.join();
	t2.join();
	
	return 0;
}
