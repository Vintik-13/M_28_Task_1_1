/*Задание 1. Заплыв на 100 метров

Что нужно сделать
Реализуйте симуляцию состязаний по заплыву на 100 метров.
Всего шесть дорожек и шесть пловцов. Имена пловцов указываются вначале из
стандартного ввода. Каждый из них плывёт с разной скоростью, которая также
задаётся пользователем при старте, через стандартный ввод в метрах в секунду.
Каждую секунду в стандартный вывод выдаётся информация о том, сколько
проплыл тот или иной пловец.
Как только все пловцы коснулись 100-метровой отметки, заплыв заканчивается и
выводится таблица с итоговыми результатами, отсортированная по возрастанию
итогового времени заплыва.*/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <thread>
#include <chrono>
#include <mutex>

const int NUMBERTRACKS{ 6 };
const int DISTANCE{ 100 };

class Swimmer
{
	std::string name;
	int speed;

public:
	Swimmer() : name{ "" }, speed{ 0 } {}

	Swimmer(std::string name, int speed) : name(name), speed(speed) {}

	void SetName(std::string name) { this->name = name; }

	void SetSpeed(int speed) { this->speed = speed; }

	std::string GetNameSwimmer() { return name; }

	int GetSpeedSwwimmer() { return speed; }

	~Swimmer() {}
};

std::mutex access;

std::vector<Swimmer*> finish;

void Start(Swimmer* s) {

	int d{ 0 };
	while (((DISTANCE - d) - s->GetSpeedSwwimmer() > 0)) {

		access.lock();

		access.unlock();

		d += s->GetSpeedSwwimmer();

		access.lock();

		std::cout << "Swimmer - " << s->GetNameSwimmer() << " swam by " << d << " meters distance" << std::endl;

		access.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	access.lock();

	std::cout << s->GetNameSwimmer() << " - FINISH!!!" << std::endl;

	finish.push_back(s);

	access.unlock();
}

bool IsSpeed(std::string speed) {

	std::regex pattern("\\d*");

	return std::regex_match(speed.c_str(), pattern) ? true : false;
}

Swimmer InputSwimmer() {

	Swimmer tmp;

	std::cout << "Input name swimmer: ";

	std::string name;

	std::getline(std::cin, name);

	tmp.SetName(name);

	std::cout << "Input speed swimmer: ";

	std::string speed;

	std::getline(std::cin, speed);

	while (!IsSpeed(speed)) {

		std::cout << "Inter a positive integer: ";

		std::getline(std::cin, speed);
	}

	tmp.SetSpeed(std::stoi(speed));

	return tmp;
}


int main()
{
	std::vector<Swimmer> start;

	for (int i = 0; i < NUMBERTRACKS; i++) {

		std::cout << "Inter the data of " << i + 1 << " swimmer" << std::endl;

		start.push_back(InputSwimmer());
	}

	std::thread track1(Start, &start[0]);

	std::thread track2(Start, &start[1]);

	std::thread track3(Start, &start[2]);

	std::thread track4(Start, &start[3]);

	std::thread track5(Start, &start[4]);

	std::thread track6(Start, &start[5]);

	track1.join();

	track2.join();

	track3.join();

	track4.join();

	track5.join();

	track6.join();

	access.lock();

	for (std::vector<Swimmer*>::iterator it = finish.begin(); it != finish.end(); it++) {

		int r{ 1 };

		std::cout << r << " place " << (*it)->GetNameSwimmer() << std::endl;
	}

	access.unlock();
}

