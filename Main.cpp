
#include <thread>
#include "Memory.h"
#include "Hack.h"

using namespace std;

Memory mem = Memory::Memory();

int main()
{
	
	Hack me = Hack();
	
	
	std::thread thread1(&Hack::esp, &me);
	std::thread thread2(&Hack::triggerBot, &me);
	std::thread thread3(&Hack::aimAtEntitySmoothing, &me);
	
	
	thread1.join();
	thread2.join();
	thread3.join();
	
	
	
}




