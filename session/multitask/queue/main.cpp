#include <queue>
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

std::queue<int> q;
std::mutex mtx;

void productor(){
	for(int i=0;i<5;i++){
		std::lock_guard<std::mutex> lock(mtx);
		q.push(i);
		std::cout << "Enviado: " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

void consumidor(){
	while(true){
		std::lock_guard<std::mutex> lock(mtx);
		if(!q.empty()){
			std::cout << "Recibido: " << q.front() << std::endl;
			q.pop();
		}
    // La tarea se suspende por 200ms aun cuando haya nuevos datos en la cola
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

int main() {
	std::thread t1(productor), t2(consumidor);
	t1.join(); t2.join();
}

