//*旨在测试LinearTable批量头尾插的性能差异
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e5; ++i)
#define MainLoop for (int j = 1; j <= 100; ++j)
#define tar "res5.txt"
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

	MainLoop
	{
		a1.clear();

		srand((unsigned)time(NULL));
		MyLoop
			stand[i] = rand() % 11 ;
		//*0代表头插，1代表尾插
		srand((unsigned)time(NULL));
		MyLoop
			op[i] = rand() % 2;

		mycout << "Test case" << j << ": " << std::endl;

		//*我的线性表
		start = clock();
		MyLoop
		{
			if(op[i])
				a1.push_back(stand[i],op[i]);
			else  
				a1.push_front(stand[i],op[i]);
		}
		finish = clock();
		mycout << "LinearTable: " << finish - start << std::endl;

		mycout << std::endl;
	}
	mycout.close();
}