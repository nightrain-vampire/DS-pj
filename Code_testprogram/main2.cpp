//*旨在测试LinearTable，deque随机访问的性能差异
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e5; ++i)
#define MainLoop for (int j = 1; j <= 1000; ++j)
#define tar "res2.txt"
const int p=1e5;

int main()
{
	DS::LinearTable a1;
	std::deque<int> a2;

	clock_t start, finish;
	int stand[(int)1e5];
	std::ofstream mycout;
	mycout.open(tar);

	MyLoop
	{
		a1.push_back(i);
		a2.push_back(i);
	}

	MainLoop
	{
		srand((unsigned)time(NULL));
		MyLoop
			stand[i] = rand()%p;

		mycout<<"Test case"<<j<<": "<<std::endl;

		//*我的线性表
		start = clock();
		MyLoop
			a1[stand[i]];
		finish = clock();
		mycout << "LinearTable[]: " << finish - start << std::endl;

		//*deque
		start = clock();
		MyLoop
			a2[stand[i]];
		finish = clock();
		mycout << "deque[]: " << finish - start << std::endl;

		mycout<<std::endl;
	}
	mycout.close();
}