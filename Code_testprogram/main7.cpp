//*旨在测试LinearTable，deque头尾删的性能差异
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e5; ++i)
#define MainLoop for (int j = 1; j <= 1000; ++j)
#define tar "res7.txt"
const int p = 1e5;

int main()
{
	DS::LinearTable a1;
	std::deque<int> a2;

	clock_t start, finish;
	int stand[(int)1e5];
	int op[(int)1e5];
	std::ofstream mycout;
	mycout.open(tar);

	a1.clear();
	a2.clear();

	MainLoop
	{
		a1.clear();
		a2.clear();
		MyLoop
		{
			a1.push_back(i);
			a2.push_back(i);
		}

		//*0代表头删，1代表尾删
		srand((unsigned)time(NULL));
		MyLoop
			op[i] = rand() % 2;

		mycout << "Test case" << j << ": " << std::endl;

		//*我的线性表
		start = clock();
		MyLoop
		{
			if (op[i])
				a1.pop_back();
			else
				a1.pop_front();
		}
		finish = clock();
		mycout << "LinearTable: " << finish - start << std::endl;

		//*deque
		start = clock();
		MyLoop
		{
			if (op[i])
				a2.pop_back();
			else
				a2.pop_front();
		}
		finish = clock();
		mycout << "deque: " << finish - start << std::endl;

		mycout << std::endl;
	}
	mycout.close();
}