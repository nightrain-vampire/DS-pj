//*旨在测试multiset，LinearTable，优先队列，deque生成有序序列的用时
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e5; ++i)
#define MainLoop for (int j = 1; j <= 100; ++j)
#define tar "res1.txt"

int main()
{
	std::multiset<int> a1;
	DS::LinearTable a2;
	std::priority_queue<int, std::vector<int>, std::less<int>> a3;
	std::deque<int> a4;

	clock_t start, finish;
	int stand[(int)1e5];
	std::ofstream mycout;
	mycout.open(tar);

	MainLoop
	{
		srand((unsigned)time(NULL));
		MyLoop
			stand[i] = rand();
		
		a1.clear();
		a2.clear();
		while(!a3.empty())
			a3.pop();
		a4.clear();

		mycout<<"Test case"<<j<<": "<<std::endl;
		//*多重集合
		start = clock();
		MyLoop
			a1.insert(stand[i]);
		finish = clock();
		mycout << "multiset: " << finish - start << std::endl;

		//*我的线性表
		start = clock();
		MyLoop
			a2.sorted_insert(stand[i]);
		finish = clock();
		mycout << "LinearTable: " << finish - start << std::endl;

		//*优先队列
		start = clock();
		MyLoop
			a3.push(stand[i]);
		finish = clock();
		mycout << "priority_queue: " << finish - start << std::endl;

		//*deque
		start = clock();
		MyLoop
			a4.push_back(stand[i]);
		finish = clock();
		mycout << "deque (push_back): " << finish - start << std::endl;
		start = clock();
		std::sort(a4.begin(), a4.end());
		finish = clock();
		mycout << "deque (sort): " << finish - start << std::endl;

		mycout<<std::endl;
	}
	mycout.close();
}