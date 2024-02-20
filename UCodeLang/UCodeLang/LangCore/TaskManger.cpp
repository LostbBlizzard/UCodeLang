#include "TaskManger.hpp"

UCodeLangStart

void some()
{
	TaskManger man;
	TaskManger::Threads(man);
	man.init();


	auto num = man.NewWeakMutex(int(5));
	

	auto func = []()
	{

	};
	man.AddTask(std::function<void()>(func), {}).Wait();
}

UCodeLangEnd