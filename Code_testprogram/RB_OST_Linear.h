#ifndef RB_OST_Linear_h
#define RB_OST_Linear_h
#include <bits/stdc++.h>
//PREPEND END

//TEMPLATE BEGIN
namespace DS
{
#define BLACK 0
#define RED 1

	//*节点类
	class LinearTable;
	struct LinearTable_Node
	{
		friend class LinearTable;

	public:
		//类型定义
		typedef size_t size_type;

	private:
		int key, color;
		size_type Size;
		LinearTable_Node *p;
		LinearTable_Node *left;
		LinearTable_Node *right;
	};

	//*我的线性表
	class LinearTable
	{
		friend struct LinearTable_Node;

	public:
		//*类型定义
		typedef size_t size_type;
		typedef LinearTable_Node *node;
		typedef const LinearTable_Node *const_node;
		typedef LinearTable_Node Node;
		typedef const LinearTable_Node const_Node;

		//*默认构造函数
		LinearTable()
		{
			create();
		}
		//*默认析构函数
		~LinearTable()
		{
			clear();
			if(nil)
				delete nil;
			Treelimit = NULL;
			Treehead=NULL;
			Treeback=NULL;
			nil=NULL;
		}

		//*拷贝构造函数
		LinearTable(const LinearTable &ta)
		{
			create();
			deep_copy(ta);
		}

		//*批量构造函数
		explicit LinearTable(size_type n, int val)
		{
			create();
			push_back(n, val);
		}

		explicit LinearTable(int n, int val)
		{
			create();
			push_back(n, val);
		}

		//*从其它容器的迭代器构造
		template <class T>
		LinearTable(T begin,T end)
		{
			create();
			T b=begin;
			T e=end;
			while(b!=e)
			{
				push_back(*b);
				b++;
			}
		}

		//*从指针构造
		LinearTable(int *be, int *ed)
		{
			create();
			int *b=be;
			while(b!=ed)
			{
				push_back(*b);
				b++;
			}
		}

		//*移动构造函数
		LinearTable(LinearTable&& t)
		{
			root=t.root;
			nil=t.nil;
			Tree_num=t.Tree_num;
			Treehead=t.Treehead;
			Treeback=t.Treeback;
			Treelimit=t.Treelimit;

			t.root=NULL;
			t.nil=NULL;
			t.Tree_num=0;
			t.Treehead=NULL;
			t.Treeback=NULL;
			t.Treelimit=NULL;
		}

		//*清除与交换
		void clear();
		void swap(LinearTable &);

		//*size与empty
		size_type size() const { return Tree_num; }
		bool empty() const { return Tree_num == 0; }

		//*basic operator
		void push_back(int);
		void push_front(int);
		void pop_back();
		void pop_front();

		//*索引
		int &operator[](size_t);
		const int &operator[](size_t) const;

		//*front()与back()
		int front() const;
		int back() const;

		//*中序打印，用于调试
		void print()
		{
			printf("\n");
			for (int id = 0; id != Tree_num; id++)
				printf("%d ", select(root, id + 1)->key);
			printf("\n");
			printf("\n");
		}

		//*迭代器
	public:
		class iterator
		{
			friend class LinearTable;

		public:
			iterator() : ptr(NULL) {}
			iterator(node P) : ptr(P) {}
			iterator(const iterator &it) { ptr = it.ptr; }
			//*赋值
			iterator &operator=(const iterator &it)
			{
				if (this != &it)
					ptr = it.ptr;
				return *this;
			}
			//*前++
			iterator &operator++()
			{
				//*确保不是哨兵节点或空节点
				if (ptr != NULL && ptr->p != NULL)
				{
					//*如果右儿子不是空的
					if (ptr->right != NULL && ptr->right->p != NULL)
					{
						node tmp = ptr->right;
						while (tmp->left->p != NULL)
							tmp = tmp->left;
						ptr = tmp;
					}
					else
					{
						node fa = ptr->p;
						while (fa && fa->p != NULL && ptr == fa->right)
						{
							ptr = fa;
							fa = fa->p;
						}
						ptr = fa;
					}
				}
				return *this;
			}
			//*后++
			iterator &operator++(int)
			{
				iterator temp = *this;
				++(*this);
				return temp;
			}
			//*前--
			iterator &operator--()
			{
				if (ptr != NULL && ptr->p != NULL)
				{
					if (ptr->left != NULL && ptr->left->p != NULL)
					{
						node tmp = ptr->left;
						while (tmp->right != NULL && tmp->right->p != NULL)
							tmp = tmp->right;
						ptr = tmp;
					}
					else
					{
						node fa = ptr->p;
						while (fa && fa->p != NULL && ptr == fa->left)
						{
							ptr = fa;
							fa = fa->p;
						}
						ptr = fa;
					}
				}
				return *this;
			}
			//*后--
			iterator &operator--(int)
			{
				iterator temp = *this;
				--(*this);
				return temp;
			}
			//* ==,!=
			bool operator==(const iterator &it)
			{
				return ptr == it.ptr;
			}
			bool operator!=(const iterator &it)
			{
				return ptr != it.ptr;
			}
			//*
			int &operator*() { return ptr->key; }
			const int &operator*() const { return ptr->key; }

			iterator &operator+=(size_t n)
			{
				node myroot = getroot();
				size_type r = iter_rank(myroot, ptr);
				node res = iter_select(myroot, r + n);
				ptr = res;
				return *this;
			}

			iterator operator+(size_t n)
			{
				iterator res = ptr;
				res += n;
				return res;
			}

			iterator &operator-=(size_t n)
			{
				node myroot = getroot();
				size_type r = iter_rank(myroot, ptr);
				node res = iter_select(myroot, r - n);
				ptr = res;
				return *this;
			}

			iterator operator-(size_t n)
			{
				iterator res = ptr;
				res -= n;
				return res;
			}

			size_t operator-(const iterator &it)
			{
				node myroot = getroot();
				size_t r1 = iter_rank(myroot, ptr);
				size_t r2 = iter_rank(myroot, it.ptr);
				return r1 - r2;
			}

			bool operator<(const iterator &it)
			{
				node myroot = getroot();
				size_t r1 = iter_rank(myroot, ptr);
				size_t r2 = iter_rank(myroot, it.ptr);

				if (((*this) != it) && (it.ptr->p == NULL))
					return true;
				if (this->ptr->p == NULL)
					return false;
				return (r1 < r2);
			}

			bool operator>(const iterator &it)
			{
				node myroot = getroot();
				size_t r1 = iter_rank(myroot, ptr);
				size_t r2 = iter_rank(myroot, it.ptr);

				if (((*this) != it) && (this->ptr->p == NULL))
					return true;
				if (it.ptr->p == NULL)
					return false;
				return (r1 > r2);
			}

			bool operator<=(const iterator &it)
			{
				return ((*this) == it) || ((*this) < it);
			}

			bool operator>=(const iterator &it)
			{
				return ((*this) == it) || ((*this) > it);
			}

			int &operator[](size_type id)
			{
				return *((*this) + id);
			}

			const int &operator[](size_type id) const
			{
				iterator res = ptr;
				res += id;
				return *res;
			}

		private:
			node ptr;

			node getroot() const
			{
				if (ptr == NULL || ptr->p == NULL)
					return NULL;
				node tmp = ptr;
				while (tmp->p->p != NULL)
					tmp = tmp->p;
				return tmp;
			}

			node iter_select(node x, size_type i) const
			{
				if (i == 0)
					return NULL;
				if (x != NULL)
				{
					size_type r;
					if (x->left != NULL)
						r = x->left->Size + 1;
					else
						r = 1;

					if (i == r)
						return x;
					else if (i < r)
						return iter_select(x->left, i);
					else
						return iter_select(x->right, i - r);
				}
				else
					return NULL;
			}

			size_type iter_rank(node myroot, node x)
			{
				if (x != NULL && x->p != NULL)
				{
					//node myroot=getroot();
					size_type r;
					if (x->left != NULL)
						r = x->left->Size + 1;
					else
						r = 1;

					node y = x;
					while (y != myroot && y->p != NULL)
					{
						if (y == y->p->right)
						{
							if (y->p->left != NULL)
								r += (y->p->left->Size + 1);
							else
								r += 1;
						}
						y = y->p;
					}
					return r;
				}
				else
					return 0;
			}
		};

		//*迭代器复制构造
		LinearTable(const iterator &f, const iterator &l)
		{
			create();
			deep_copy(f, l);
		}

		//*迭代器操作
		iterator begin() const; //返回开头位置的迭代器
		iterator last() const;	//返回最后一个元素的迭代器
		iterator end() const;	//返回越界迭代器

		//*调试用---迭代器打印
		void iter_print()
		{
			printf("\n");
			for (iterator i = begin(); i != end(); ++i)
				printf("%d ", *i);
			printf("\n");
			printf("\n");
		}

		//*为了oj的测试，随机后插
		void insert(size_type k, int val);

		//*随机单删，单插
		iterator erase(const iterator &);
		iterator erase(const iterator &, const iterator &);
		void insert(const iterator &, int val);
		void sorted_insert(int val); //有序插入
		void insert(const iterator &, size_type, int);
		void insert(const iterator &, const iterator &, const iterator &);

		//*批量插入与删除
		void pop_front(size_type);
		void pop_back(size_type);
		void push_front(size_type, int);
		void push_back(size_type, int);
		void push_front(const iterator &, const iterator &);
		void push_back(const iterator &, const iterator &);

		//*运算符重载
		LinearTable &operator=(const LinearTable &ta)
		{
			if (&ta != this)
				deep_copy(ta);
			return *this;
		}

		//*区间极值
		int min(const iterator &l, const iterator &r, size_type k);
		int min(size_type l, size_type r, size_type k);

		//*只是为了oj上的测试提供的接口
		int at(int x)
		{
			node tmp = select(root, x + 1);
			return tmp->key;
		}
		int modify(int x, int y)
		{
			node tmp = select(root, x + 1);
			int ans = tmp->key;
			tmp->key = y;
			return ans;
		}

		//*为了和nsh的测试对接
		//*函数来自于他的oj提供的
		int Rand(int a, int b, int c)
		{
			static int last = 0;
			return last = (a * last * last + b * last + c) % 1000000007;
		}

		void batch_push_pop_test()
		{
			int seed_a, seed_b, seed_c;
			int n;
			scanf("%d%d%d%d", &n, &seed_a, &seed_b, &seed_c);
			LinearTable q;
			for (int i = 1; i <= n; i++)
			{
				if ((i ^ seed_c) & 1)
					q.push_back(i, Rand(seed_a, seed_b, seed_c));
				else
					q.push_front(i, Rand(seed_a, seed_b, seed_c));
			}
			for (int i = 1; i <= n; i++)
			{
				int siz = q.size();
				printf("%d\n", q[(Rand(seed_a, seed_b, seed_c) % siz + siz) % siz]);
				if (Rand(seed_a, seed_b, seed_c) & 1)
					q.pop_back(i);
				else
					q.pop_front(i);
				printf("%d\n", q.size());
			}
		}

	private:
		node root; //*根节点
		node nil;  //*哨兵节点

		size_type Tree_num; //红黑树的节点数

		iterator Treehead, Treeback, Treelimit;

		//*构造
		void create();

		//*递归析构
		void discreate(node x);

		//*返回以节点x为根的rank最小或最大的节点
		node Minimun(node x);
		node Maximun(node x);

		//*找到中序遍历的前驱与后继
		node findsuccessor(node x);
		node findpredecessor(node x);

		//*左右旋
		void left_rotate(node x);
		void right_rotate(node x);

		//*查找以节点x为根的包含第i小（从1开始）关键字的节点
		node select(node x, size_type i) const;
		//*给定一个节点x，返回它的秩
		size_type rank(node x);

		//*插入操作
		void insert_fixup(node z);
		void Tree_insert_front(node x, int val); //在指定节点前插
		void Tree_insert_back(node x, int val);	 //在指定节点后插

		//*删除操作
		void transplant(node u, node v);
		void delete_fixup(node x);
		void Tree_delete(node x);

		//*深拷贝
		void deep_copy(const LinearTable &);
		void deep_copy(const iterator &f, const iterator &l);
	};

	//*成员函数实现
	void LinearTable::create()
	{
		root = NULL;
		Tree_num = 0;
		nil = new Node;
		nil->Size = 0;
		nil->key = 0x7fffffff;
		nil->left = NULL;
		nil->right = NULL;
		nil->p = NULL;
		nil->color = BLACK;

		Treehead = nil;
		Treeback = nil;
		Treelimit = nil;
	}

	void LinearTable::discreate(node x)
	{
		if (x != NULL && x != nil)
		{
			discreate(x->left);
			discreate(x->right);
			delete x;
		}
	}

	void LinearTable::clear()
	{
		discreate(root);
		Treehead = nil;
		Treeback = nil;
		Tree_num = 0;
		root = NULL;
	}

	void LinearTable::swap(LinearTable &t)
	{
		std::swap(root,t.root);
		std::swap(nil,t.nil);
		std::swap(Tree_num,t.Tree_num);
		std::swap(Treehead,t.Treehead);
		std::swap(Treeback,t.Treeback);
		std::swap(Treelimit,t.Treelimit);
	}

	LinearTable::node LinearTable::Minimun(node x)
	{
		if (x != NULL)
		{
			node tmp = x;
			while (tmp->left != nil)
				tmp = tmp->left;
			return tmp;
		}
		else
			return nil;
	}

	LinearTable::node LinearTable::Maximun(node x)
	{
		if (x != NULL)
		{
			node tmp = x;
			while (tmp->right != nil)
				tmp = tmp->right;
			return tmp;
		}
		else
			return nil;
	}

	//找到节点x的后继
	LinearTable::node LinearTable::findsuccessor(node x)
	{
		if (x == NULL)
			return nil;
		else
		{
			if (x->right != nil)
				return Minimun(x->right);
			else
			{
				node pa = x->p;
				while (pa && pa != nil && x == pa->right)
				{
					x = pa;
					pa = x->p;
				}
				return pa;
			}
		}
	}

	LinearTable::node LinearTable::findpredecessor(node x)
	{
		if (x == NULL)
			return nil;
		else
		{
			if (x->left != nil)
				return Maximun(x->left);
			else
			{
				node pa = x->p;
				while (pa && pa != nil && x == pa->left)
				{
					x = pa;
					pa = x->p;
				}
				return pa;
			}
		}
	}

	void LinearTable::left_rotate(node x)
	{
		if (x->right == nil)
			return;

		node y = x->right;
		x->right = y->left;
		if (y->left != nil)
			y->left->p = x;
		y->p = x->p;
		if (x->p == nil)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;

		y->left = x;
		x->p = y;

		y->Size = x->Size;
		x->Size = 1;
		if (x->left != NULL)
			x->Size += x->left->Size;
		if (x->right != NULL)
			x->Size += x->right->Size;
	}

	void LinearTable::right_rotate(node x)
	{
		if (x->left == nil)
			return;

		node y = x->left;
		x->left = y->right;
		if (y->right != nil)
			y->right->p = x;
		y->p = x->p;
		if (x->p == nil)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;

		y->right = x;
		x->p = y;

		y->Size = x->Size;
		x->Size = 1;
		if (x->left != NULL)
			x->Size += x->left->Size;
		if (x->right != NULL)
			x->Size += x->right->Size;
	}

	LinearTable::node LinearTable::select(node x, size_type i) const
	{
		if (x != NULL)
		{
			size_type r;
			if (x->left != NULL)
				r = x->left->Size + 1;
			else
				r = 1;

			if (i == r)
				return x;
			else if (i < r)
				return select(x->left, i);
			else
				return select(x->right, i - r);
		}
		else
			return nil;
	}

	LinearTable::size_type LinearTable::rank(node x)
	{
		if (x != NULL && x != nil)
		{
			size_type r;
			if (x->left != NULL)
				r = x->left->Size + 1;
			else
				r = 1;

			node y = x;
			while (y != root)
			{
				if (y == y->p->right)
				{
					if (y->p->left != NULL)
						r += (y->p->left->Size + 1);
					else
						r += 1;
				}
				y = y->p;
			}
			return r;
		}
		else
			return 0;
	}

	void LinearTable::insert_fixup(node z)
	{
		while (z->p->color == RED)
		{
			if (z->p == z->p->p->left)
			{
				node y = z->p->p->right;
				if (y->color == RED)
				{
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else
				{
					if (z == z->p->right)
					{
						z = z->p;
						left_rotate(z);
					}
					z->p->color = BLACK;
					z->p->p->color = RED;
					right_rotate(z->p->p);
				}
			}
			else
			{
				node y = z->p->p->left;
				if (y->color == RED)
				{
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else
				{
					if (z == z->p->left)
					{
						z = z->p;
						right_rotate(z);
					}
					z->p->color = BLACK;
					z->p->p->color = RED;
					left_rotate(z->p->p);
				}
			}
		}
		root->color = BLACK;
	}

	void LinearTable::push_back(int val)
	{
		node z = new Node;
		z->key = val;
		z->color = RED;
		z->Size = 1;
		z->p = NULL;
		node y = nil;
		node x = root;
		if (x != NULL)
		{
			while (x != nil && x != NULL)
			{
				y = x;
				y->Size = y->Size + 1;
				x = x->right;
			}
		}
		z->p = y;
		if (y == nil)
			root = z;
		else
		{
			y->right = z;
		}
		z->left = nil;
		z->right = nil;
		insert_fixup(z);
		Tree_num++;

		Treeback = z;
		if (Treehead == nil)
			Treehead = z;
	}

	void LinearTable::push_front(int val)
	{
		node z = new Node;
		z->key = val;
		z->color = RED;
		z->Size = 1;
		z->p = NULL;
		node y = nil;
		node x = root;
		if (x != NULL)
		{
			while (x != nil && x != NULL)
			{
				y = x;
				y->Size = y->Size + 1;
				x = x->left;
			}
		}
		z->p = y;
		if (y == nil)
			root = z;
		else
		{
			y->left = z;
		}
		z->left = nil;
		z->right = nil;
		insert_fixup(z);
		Tree_num++;

		Treehead = z;
		if (Treeback == nil)
			Treeback = z;
	}

	void LinearTable::pop_back()
	{
		Tree_delete(Treeback.ptr);
	}

	void LinearTable::pop_front()
	{
		Tree_delete(Treehead.ptr);
	}

	void LinearTable::transplant(node u, node v)
	{
		if (u->p == nil)
			root = v;
		else if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;

		if (v != nil)
			v->p = u->p;

		if (root == nil)
			root = NULL;
	}

	void LinearTable::delete_fixup(node x)
	{
		while (x != root && x->color == BLACK)
		{
			node w;
			if (x == x->p->left)
			{
				w = x->p->right;
				if (w == nil)
					break;
				if (w->color == RED)
				{
					w->color = BLACK;
					x->p->color = RED;
					left_rotate(x->p);
					w = x->p->right;
				}
				if(w==nil)
					break;
				if (w->left->color == BLACK && w->right->color == BLACK)
				{
					w->color = RED;
					x = x->p;
				}
				else
				{
					if (w->right->color == BLACK)
					{
						w->left->color = BLACK;
						w->color = RED;
						right_rotate(w);
						w = x->p->right;
					}
					w->color = x->p->color;
					x->p->color = BLACK;
					w->right->color = BLACK;
					left_rotate(x->p);
					x = root;
				}
			}
			else
			{
				w = x->p->left;
				if (w == nil)
					break;
				if (w->color == RED)
				{
					w->color = BLACK;
					x->p->color = RED;
					right_rotate(x->p);
					w = x->p->left;
				}
				if(w==nil)
					break;
				if (w->left->color == BLACK && w->right->color == BLACK)
				{
					w->color = RED;
					x = x->p;
				}
				else
				{
					if (w->left->color == BLACK)
					{
						w->right->color = BLACK;
						w->color = RED;
						left_rotate(w);
						w = x->p->left;
					}
					w->color = x->p->color;
					x->p->color = BLACK;
					w->left->color = BLACK;
					right_rotate(x->p);
					x = root;
				}
			}
		}
		x->color = BLACK;
	}

	void LinearTable::Tree_delete(node z)
	{
		node y, x;
		if (z == nil || z == NULL)
			return;

		//先调整一次迭代器
		if (Tree_num == 1)
			Treehead = Treeback = nil;
		else
		{
			if (Treehead == z)
				Treehead = findsuccessor(z);
			else if (Treeback == z)
				Treeback = findpredecessor(z);
		}

		y = z;
		int y_original_color = y->color;
		//如果子节点少于2个
		if (z->left == nil || z->right == nil)
		{
			node tmp;
			if (z->left == nil)
			{
				x = z->right;
				transplant(z, z->right);
			}
			else
			{
				x = z->left;
				transplant(z, z->left);
			}

			tmp = z->p;
			z->p = NULL;
			z->left = NULL;
			z->right = NULL;
			delete z;

			//更新size
			while (tmp != nil)
			{
				tmp->Size -= 1;
				tmp = tmp->p;
			}
		}
		//y应该是z的后继
		else
		{
			node tmp;
			y = findsuccessor(z);
			y_original_color = y->color;
			x = y->right; //既然y是后继，则只可能有右儿子
			if (y->p == z)
			{
				transplant(z, y);
				y->left = z->left;
				y->left->p = y;
				y->color = z->color;
				tmp = y->p;
			}
			else
			{
				tmp = y->p;
				transplant(y, y->right);
				y->right = z->right;
				y->right->p = y;
				transplant(z, y);
				y->left = z->left;
				y->left->p = y;
				y->color = z->color;
			}

			z->p = NULL;
			z->left = NULL;
			z->right = NULL;
			delete z;

			//更新size
			while (tmp != nil)
			{
				tmp->Size -= 1;
				tmp = tmp->p;
			}
		}
		if (y_original_color == BLACK && x != nil)
			delete_fixup(x);

		Tree_num--;
	}

	int LinearTable::front() const
	{
		return *Treehead;
	}

	int LinearTable::back() const
	{
		return *Treeback;
	}

	int &LinearTable::operator[](size_t id)
	{
		if (id >= Tree_num || id < 0)
			return nil->key;
		else
		{
			node tmp = select(root, id + 1);
			return tmp->key;
		}
	}

	const int &LinearTable::operator[](size_t id) const
	{
		if (id >= Tree_num || id < 0)
			return nil->key;
		else
		{
			node tmp = select(root, id + 1);
			return tmp->key;
		}
	}

	LinearTable::iterator LinearTable::begin() const
	{
		return Treehead;
	}

	LinearTable::iterator LinearTable::end() const
	{
		return Treelimit;
	}

	LinearTable::iterator LinearTable::last() const
	{
		return Treeback;
	}

	LinearTable::iterator LinearTable::erase(const iterator &it)
	{
		iterator res = it;
		++res;
		Tree_delete(it.ptr);
		return res;
	}

	LinearTable::iterator LinearTable::erase(const iterator &f, const iterator &l)
	{
		iterator res = f;
		while (res != l)
			res = erase(res);
		res = erase(res);
		return res;
	}

	void LinearTable::Tree_insert_front(node x, int val)
	{
		//在越界迭代器前插入
		if (x == nil)
			push_back(val);
		else
		{
			node targ = findpredecessor(x);
			//如果前驱是nil，证明是头节点
			if (targ == nil)
				push_front(val);
			else
			{
				node z = new Node;
				z->color = RED;
				z->key = val;
				z->Size = 1;
				z->left = nil;
				z->right = nil;

				if (targ->right != nil)
				{
					targ = x;
					targ->left = z;
				}
				else
				{
					targ->right = z;
				}
				z->p = targ;

				//更新维护size
				node tmp = targ;
				while (tmp && tmp != nil)
				{
					tmp->Size += 1;
					tmp = tmp->p;
				}

				insert_fixup(z);
				Tree_num++;
			}
		}
	}

	void LinearTable::Tree_insert_back(node x, int val)
	{
		//在越界迭代器前插入
		if (x == nil)
			push_front(val);
		else
		{
			node targ = findsuccessor(x);
			//如果前驱是nil，证明是头节点
			if (targ == nil)
				push_back(val);
			else
			{
				node z = new Node;
				z->color = RED;
				z->key = val;
				z->Size = 1;
				z->left = nil;
				z->right = nil;

				if (targ->left != nil)
				{
					targ = x;
					targ->right = z;
				}
				else
				{
					targ->left = z;
				}
				z->p = targ;

				//更新维护size
				node tmp = targ;
				while (tmp && tmp != nil)
				{
					tmp->Size += 1;
					tmp = tmp->p;
				}

				insert_fixup(z);
				Tree_num++;
			}
		}
	}

	void LinearTable::insert(const iterator &it, int val)
	{
		Tree_insert_front(it.ptr, val);
	}

	void LinearTable::insert(const iterator &it, size_type push_size, int val)
	{
		while (push_size--)
			Tree_insert_front(it.ptr, val);
	}

	void LinearTable::insert(const iterator &it, const iterator &f, const iterator &l)
	{
		for (iterator i = f; i <= l; ++i)
			Tree_insert_front(it.ptr, i.ptr->key);
	}

	void LinearTable::insert(size_type x, int val)
	{
		node tar = select(root, x);
		Tree_insert_back(tar, val);
	}

	void LinearTable::sorted_insert(int val)
	{
		node y = nil;
		node x = root;

		node z = new Node;
		z->color = RED;
		z->key = val;
		z->left = nil;
		z->right = nil;
		z->Size = 1;

		while (x != nil && x != NULL)
		{
			y = x;
			y->Size += 1;
			if (z->key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		z->p = y;

		if (y == nil)
		{
			root = z;
			Treeback = Treehead = z;
		}
		else if (z->key < y->key)
		{
			y->left = z;
			if (Treehead == y)
				Treehead = z;
		}
		else
		{
			y->right = z;
			if (Treeback == y)
				Treeback = z;
		}

		insert_fixup(z);
		Tree_num++;
	}

	void LinearTable::pop_front(size_type pop_size)
	{
		while (pop_size--)
			pop_front();
	}

	void LinearTable::pop_back(size_type pop_size)
	{
		while (pop_size--)
			pop_back();
	}

	void LinearTable::push_front(size_type push_size, int value)
	{
		while (push_size--)
			push_front(value);
	}

	void LinearTable::push_front(const iterator &f, const iterator &l)
	{
		for (iterator i = f; i <= l; ++i)
			push_front(i.ptr->key);
	}

	void LinearTable::push_back(size_type push_size, int value)
	{
		while (push_size--)
			push_back(value);
	}

	void LinearTable::push_back(const iterator &f, const iterator &l)
	{
		for (iterator i = f; i <= l; ++i)
			push_back(i.ptr->key);
	}

	void LinearTable::deep_copy(const LinearTable &ta)
	{
		clear();
		iterator tmp = nil;
		insert(tmp, ta.begin(), ta.last());
	}

	void LinearTable::deep_copy(const iterator &f, const iterator &l)
	{
		clear();
		iterator tmp = nil;
		insert(tmp, f, l);
	}

	int LinearTable::min(const iterator &l, const iterator &r, size_type k)
	{
		size_type counter = 0;
		int helper[Tree_num];
		for (iterator i = l; i <= r; ++i)
			helper[counter++]=*i;
		std::sort(helper,helper+counter);
		
		return helper[k];
	}

	int LinearTable::min(size_type l, size_type r, size_type k)
	{
		iterator left = select(root, l + 1);
		iterator right = select(root, r + 1);
		return min(left, right, k);
	}
} // namespace DS
//TEMPLATE END
#endif


