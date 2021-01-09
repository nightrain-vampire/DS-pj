//*对一些辅助操作进行测试
#include "RB_OST_Linear.h"
#define MyLoop for (int i = 0; i != 1e6; ++i)
#define MainLoop for (int j = 1; j <= 1000; ++j)
#define tar "res8.txt"
const int p=1e5;

int main()
{
	clock_t start, finish;
	int stand[(int)1e5];
	int op[(int)1e5];
	std::ofstream mycout;
	mycout.open(tar);

	mycout<<"初始化长度为1e6的线性表："<<std::endl;
	start=clock();
	std::deque<int> a2(int(1e6),90);
	finish=clock();
	mycout<<"deque: "<<finish-start<<std::endl;

	start=clock();
	DS::LinearTable a1(int(1e6),90);
	finish=clock();
	mycout<<"LinearTable: "<<finish-start<<std::endl<<std::endl;

	mycout<<"迭代器遍历："<<std::endl;
	start=clock();
	for(auto i:a2);
	finish=clock();
	mycout<<"deque: "<<finish-start<<std::endl;

	start=clock();
	for(auto i:a1);
	finish=clock();
	mycout<<"LinearTable: "<<finish-start<<std::endl<<std::endl;

	mycout<<"clear()的效率："<<std::endl;
	start=clock();
	a2.clear();
	finish=clock();
	mycout<<"deque: "<<finish-start<<std::endl;

	start=clock();
	a1.clear();
	finish=clock();
	mycout<<"LinearTable: "<<finish-start<<std::endl;

	mycout.close();
}