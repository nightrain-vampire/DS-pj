//*旨在测试LinearTable，deque随机删插的性能差异
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e5; ++i)
#define MainLoop for (int j = 1; j <= 100; ++j)
#define tar "res4.txt"
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
		a2.clear();
		MyLoop
		{
			a1.push_back(i);
			a2.push_back(i);
		}

		srand((unsigned)time(NULL));
		MyLoop
			stand[i] = rand() % p;
		//*0代表插，1代表删
		srand((unsigned)time(NULL));
		MyLoop
			op[i] = rand() % 2;

		mycout << "Test case" << j << ": " << std::endl;

		//*我的线性表
		start = clock();
		MyLoop
		{
			if(op[i])
				a1.erase(a1.begin()+stand[i]);
			else  
				a1.insert(a1.begin()+stand[i],stand[i]);
		}
		finish = clock();
		mycout << "LinearTable: " << finish - start << std::endl;

		//*deque
		start = clock();
		MyLoop
		{
			if(op[i])
				a2.erase(a2.begin()+stand[i]);
			else  
				a2.insert(a2.begin()+stand[i],stand[i]);
		}
		finish = clock();
		mycout << "deque: " << finish - start << std::endl;

		mycout << std::endl;
	}
	mycout.close();
}