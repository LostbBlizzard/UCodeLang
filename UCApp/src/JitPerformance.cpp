#include "JitPerformance.hpp"
void JitPerformance::main(Task Test)
{
	switch (Test)
	{
	case Task::Main:
	{
		struct App
		{
			static void init()
			{

			}
			static void deinit()
			{

			}
			
			static int main(int V)
			{
				return 8;
			}
			
		};

		PerformanceInfo<int> Val;

		Val._Init = &App::init;
		Val._DeInit = &App::deinit;
		Val._Native = &App::main;
		Val._UCode = "|main[int V] => 8;";


		size_t Count = 2 << 8;
		for (size_t i = 0; i < 9; i++)
		{
			Count = Count << 1;
			Run(Count, Val, 0);
			std::cout << "Times:" << std::to_string(Count);
		}

		
	}
	break;
	default:
		break;
	}
	{

	}
}