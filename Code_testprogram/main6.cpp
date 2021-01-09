//*旨在测试LinearTable批量头尾删的性能差异
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e6; ++i)
#define MyLoop2 for(int i=0;i!= 10000; ++i)
#define MainLoop for (int j = 1; j <= 100; ++j)
#define tar "res6.txt"
const int p = 1e5;

int main()
{
	DS::LinearTable a1;
	std::deque<int> a2;

	clock_t start, finish;
	int stand[(int)1e4];
	int op[(int)1e4];
	std::ofstream mycout;
	mycout.open(tar);

	MainLoop
	{
		a1.clear();
		MyLoop
			a1.push_back(i);

		srand((unsigned)time(NULL));
		MyLoop2
			stand[i] = rand() % 101 ;
		//*0代表头删，1代表尾删
		srand((unsigned)time(NULL));
		MyLoop2
			op[i] = rand() % 2;

		mycout << "Test case" << j << ": " << std::endl;

		//*我的线性表
		start = clock();
		MyLoop2
		{
			if(op[i])
				a1.pop_back(stand[i]);
			else  
				a1.pop_front(stand[i]);
		}
		finish = clock();
		mycout << "LinearTable: " << finish - start << std::endl;

		mycout << std::endl;
	}
	mycout.close();
}