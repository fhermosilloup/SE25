#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> cola;

void productor() {
	for (int i = 1; i <= 5; i++) {
		std::unique_lock<std::mutex> lock(mtx);
		cola.push(i);
		cv.notify_one();  // avisa al consumidor
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void consumidor() {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [] { return !cola.empty(); }); // espera hasta que haya datos
		int dato = cola.front(); cola.pop();
		std::cout << "Consumido: " << dato << "\n";
	}
}

int main() {
	std::thread t1(productor), t2(consumidor);
	t1.join(); t2.join();
}
