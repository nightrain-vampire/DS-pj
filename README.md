# DS 期末Pj实验报告



## 一、提交文件说明

code_testprogram文件夹下放置了代码，自己写的测试程序与结果。

1. RB_OST_Linear.h中是pj代码。
2. main1~8是自己写的测试程序代码，res1~8是对应的结果输出。
3. Mydeque.h是开始写pj时的版本，可以无视。



## 二、底层结构的选择

实验目标是：实现一个数据结构，它要在插入，删除，索引等各种操作中尽可能拥有良好的性能。除此之外，它还要尽可能的多支持拓展操作。



从pj的要求出发考虑底层结构的选择：（当时确实只想到线性表和树两种结构）

1、选择deque 。

2、选择平衡树 。



选择deque在初期有很多优势。注意到基本操作都是stl deque的原型，那么模仿一下deque的底层架构可以瞬间完美的爆破pj的基本操作，再搞个迭代器，加点其它操作，就能把pj氵过了。（而我一开始是这么想的）



但是选择deque的劣势也十分明显。第一，既然是简单模仿stl容器，那么做出来只会比stl deque差而非更好。这样氵一个山寨版deque应付任务毫无意义，也没有让人肝下去的动力。第二，传统deque的序列操作（随机删插）复杂度为O(n) 。这个问题需要花很大心思才能有效解决，而我当时并没有好主意（现在也没有...）



于是乎，deque的版本就成为废案了（我甚至随机访问迭代器都懒得配了），需要重新考虑底层的选择。

由于之前看《算法导论》时发现有RBT（红黑树）这种东西，于是初生牛犊不怕虎，头脑一热就选它了。



现在回过头来认真分析一下选择红黑树的优劣：

红黑树的优势：

1. 自平衡的二叉搜索树，不会退化成链表。（这是所有平衡树必备的）

2. 树形结构的通用优势之一：性能中庸，删插索引最坏复杂度可以统一为O(log(n))。如果优化的好的话个别情况下能做到O(1)的复杂度。

3. 树形结构的通用优势之二：灵活，方便后续魔改，可操作空间大。

4. 对于RBT，通过维护红黑性质来自平衡，感觉比较直观。（个人意见）

5. 统一空节点为哨兵的机制，对代码实现有帮助（参考学习链表时加空头尾节点的操作）

   

红黑树的劣势：

1. 前置学习时间长，代码量大，debug工作繁重（来自某位写跳表的同学的评价）
2. 红黑树不是完全平衡的。一颗有n个内部节点的红黑树的高度至多可达2lg(n+1)。在某些极端情况下，红黑树操作复杂度的常数可能不太理想。而其它平衡树则没有这种烦恼。



因此，综合利弊，还是RBT显得更有前途。



## 三、进一步思考

所有选择平衡二叉搜索树的同学都会面临一个严峻问题：二叉搜索树落地就是有序的，但我们要模拟的只是一个普普通通的线性表啊，这明显不符合pj的要求。于是乎，光是RBT还是不够的。



这个问题还是好解决的。首先注意到树的中序遍历序列对应了线性表。在标准RBT中，排序的原则是key值的大小；在我们的伪线性表中，排序的标准是插入的位置与顺序。那么问题就解决了。我们在节点x的中序位前插入节点y，相当于让y成为x的左儿子或者让y成为x的前驱的右儿子。（注意到x的前驱的右儿子一定是空的，否则它必不可能是x的前驱）



然而最大的问题在于索引。之前说过了，树形结构索引复杂度O(log(n))，但这是建立在红黑树的插入规则（或者确切的说，是二叉搜索树）上的。在标准的红黑树中找键值为x的节点，只要不停的向左子树或者右子树找，最多找log(n)个节点就能找到。在我们的树中这样做显然不行.......



解决方案有三个：

1. 每个节点上引入新的辅助key值，再套用红黑树那套查找机制。

2. 找到中序遍历位置第一的节点，再不断找其后继直至到达目标位置。

3. 同1，不过是动态维护这个新的key值。

   

方案1肯定不行。静态位置键值在随机删插面前会立刻崩盘，每有删插就把整颗树的键值更新一遍也不现实。

方案2也是比较离谱。树能维持一个中庸性能的关键就在于它的许多操作都是从根往下，把本来会O(n)的操作降成了O(log(n))。方案2的索引效率甚至比不上单链表。

方案3是唯一正确的。（虽然上面的描述是正确的废话，不过正解就是如此。）传统的RBT的架构无法解决pj带来的索引问题，我们需要进行拓展。在算法导论上，这种被拓展过后的RBT被称为顺序统计树。



## 四、拓展---RB_OST

###### 名字是我自己起的......



顺序统计树名字听着十分高大上，但其实只是多了个键值和两个操作的红黑树。

```c++
	//*节点类
	struct LinearTable_Node
	{
		friend class LinearTable;

	public:
		//类型定义
		typedef size_t size_type;

	private:
		int key, color;
        //对于节点x，记以x为根的子树的节点个数为size。
		size_type Size;                      
		LinearTable_Node *p;
		LinearTable_Node *left;
		LinearTable_Node *right;
	};
```



为了方便叙述，引入一个听起来高大上的概念：



### def：秩

定义：一个元素的秩为在中序遍历树时输出的位置。

规定哨兵的秩为0 。由定义：以x为根的子树中的结点x的秩为x->left->size+1 。



于是，索引问题迎刃而解了。只要搜集节点上的size，动态计算秩并定位目标位置即可。

下面是OST中新增的两个操作：

```c++
	//找到以节点x为根的子树中秩为i的节点
	LinearTable::node LinearTable::select(node x, size_type i) const
	{
		if (x != NULL)
		{
			size_type r;
            //看看在该子树中，已经有多少个节点排在它的中序位前了
			if (x->left != NULL)
				r = x->left->Size + 1;
			else
				r = 1;
			
            //找到了！
			if (i == r)
				return x;
            //i<r,说明要往左子树去
			else if (i < r)
				return select(x->left, i);
           	//i>r,说明要往右子树去
            //注意有r个节点已经排在右子树前
			else
				return select(x->right, i - r);
		}
		else
			return nil;
	}
	
	//给定一个节点，确定它在整颗树中的秩
	LinearTable::size_type LinearTable::rank(node x)
	{
		if (x != NULL && x != nil)
		{
            //初始化：看看在该树中，已经有多少个节点排在它的中序位前了
			size_type r;
			if (x->left != NULL)
				r = x->left->Size + 1;
			else
				r = 1;

			node y = x;
            //循环（从x统计到根节点）
			while (y != root)
			{
                //情况一：x是父亲的右儿子，那么x->p与x->p的左子树的中序位置都比x前。
				if (y == y->p->right)
				{
					if (y->p->left != NULL)
						r += (y->p->left->Size + 1);
					else
						r += 1;
				}
                //情况二：x是父亲的左儿子，那么x->p与x->p的右子树的中序位置都不会超过x。
                //直接向上跳
				y = y->p;
			}
			return r;
		}
		else
			return 0;
	}
```



上面的想法很美好，但若是不能动态维护这些size，那所有的努力都白费了。因此在实现红黑树的基本操作中，注意要对size进行相应的维护。



## 五、RBT相关操作的实现

###### 这些操作都应该是LinearTable类的私有成员函数。



经过前面的不懈思考，终于成功把pj变成了RBT代码实现（不是）。下面需要实现RBT的基本操作与辅助操作。

（这里列一些比较重要的）



### 1.公共哨兵

1）颜色永远为black，size永远为0 。

2）key值被我设成了0x7fffffff 。

3）永远没有父亲，左儿子，右儿子 。

4）是根节点的父亲。

5）是所有叶节点的儿子。



### 2.前驱后继等

1）后继：中序位置中节点x的后一位

2）前驱：中序位置中节点x的前一位



```c++
	//找到以x为根的子树中中序位最前的节点
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

	//找到以x为根的子树中中序位最后的节点
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
            //右儿子不是哨兵，则右子树中中序位最前的就是后继
			if (x->right != nil)
				return Minimun(x->right);
			else
			{
                //记录x的父亲为pa
				node pa = x->p;
                //pa与x同时向上跳。当x是pa的左儿子时，pa是x的后继
				while (pa && pa != nil && x == pa->right)
				{
					x = pa;
					pa = x->p;
				}
				return pa;
			}
		}
	}
	
	//找到节点x的前驱
	LinearTable::node LinearTable::findpredecessor(node x)
	{
		if (x == NULL)
			return nil;
		else
		{
            //左儿子不是哨兵，则左子树中中序位最后的就是前驱
			if (x->left != nil)
				return Maximun(x->left);
			else
			{
                //记录x的父亲为pa
				node pa = x->p;
                //pa与x同时向上跳。当x是pa的右儿子时，pa是x的前驱
				while (pa && pa != nil && x == pa->left)
				{
					x = pa;
					pa = x->p;
				}
				return pa;
			}
		}
	}
```



PS：课上助教提到了线索化二叉树来让前驱后继的查找变为O(1)。没来得及实现。



### 3.旋转操作

###### 红黑树的好处还在于，许多代码都是左右对称的。实现了一部分另一部分直接左右对调就可以。



红黑树的旋转是其维持红黑性质平衡的重要手段，这里仅讲一下右旋。

```c++
	//右旋
	void LinearTable::right_rotate(node x)
	{
		if (x->left == nil)
			return;
		
        //y指向x的左儿子，x的左儿子换成x的左儿子的右儿子
		node y = x->left;
		x->left = y->right;
        //若y的右儿子不是哨兵，父亲置为x
		if (y->right != nil)
			y->right->p = x;
        //y的父亲变为x的父亲
		y->p = x->p;
        //如果x的父亲是哨兵，说明x原来为根
        //此时根应该是y
		if (x->p == nil)
			root = y;
        //否则，y替换掉x的位置
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;
		
        //x变成y的右儿子
		y->right = x;
		x->p = y;
		
        //维护size。小心x可能已经指向哨兵了。
		y->Size = x->Size;
		x->Size = 1;
		if (x->left != NULL)
			x->Size += x->left->Size;
		if (x->right != NULL)
			x->Size += x->right->Size;
	}
```

左旋类似，直接把代码中的left换成right，right换成left就可以了。



### 4.插入维护

插入操作十分简单。由于pj要求是在指定位置x前插，因此让新节点y成为x的左儿子或者让y成为x的前驱的右儿子即可。注意我们永远让新节点的颜色一开始是红色。（算法导论上还作为思考题问为啥，我没想出来orz）



由于我一开始写的时候没有想清楚架构，先实现了头尾插，然后才实现随机插。重复造了些轮子，拖累了工作效率。



插入维护比较重要。有了它，红黑树就能在新增节点后自我平衡。

```c++
	//插入修复自平衡
	//z代表新节点
	void LinearTable::insert_fixup(node z)
	{
        //若z的父亲是红色，说明要修复下去
		while (z->p->color == RED)
		{
			if (z->p == z->p->p->left)
			{
                //y是z的叔节点
				node y = z->p->p->right;
                //情况一：z的叔节点是红色
				if (y->color == RED)
				{
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else
				{
                    //情况二：z的叔结点y是黑色的且z是右孩子
                    //可立刻通过一个左旋变成情况三
					if (z == z->p->right)
					{
						z = z->p;
						left_rotate(z);
					}
                    //情况三：z的叔结点y是黑色的且z是左孩子
					z->p->color = BLACK;
					z->p->p->color = RED;
					right_rotate(z->p->p);
				}
			}
            //与上面一个分支是类似的，只要把代码中左换成右，右换成左即可。
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
        //保证根的颜色是黑的
		root->color = BLACK;
	}
```



### 5.删除及维护

红黑树的删除及其维护是最难的一部分，这里试图按照《算法导论》的顺序分成三部分来讲：



#### 1）辅助程序：子树替换

```c++
	//用以v为根的子树替换以u为根的子树
	void LinearTable::transplant(node u, node v)
	{
        //如果u是根节点，那么根节点更新为v
		if (u->p == nil)
			root = v;
        //否则，v接替u成为父亲对应位置上的儿子
		else if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;
		
        //与书上不同的是，我不会改变哨兵的父亲是空的这一性质
		if (v != nil)
			v->p = u->p;
		
        //特判：此时根节点指向哨兵，说明树空了。根节点应该指向NULL
		if (root == nil)
			root = NULL;
	}
```



#### 2）delete

```c++
	//删除节点z
	void LinearTable::Tree_delete(node z)
	{
		node y, x;
        //不会对哨兵或空节点操作
		if (z == nil || z == NULL)
			return;

		//先调整一次迭代器
        //啊这，好像讲的顺序乱了。这边维护首尾迭代器，但我还没讲迭代器......
		if (Tree_num == 1)
			Treehead = Treeback = nil;
		else
		{
			if (Treehead == z)
				Treehead = findsuccessor(z);
			else if (Treeback == z)
				Treeback = findpredecessor(z);
		}
        
        //记被删的节点为z 。y指向树中被删/移的节点，x指向y的子节点。
		y = z;
		int y_original_color = y->color;
		//如果子节点少于2个
		if (z->left == nil || z->right == nil)
		{
            //删除z，将儿子提上来 。
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
			//此时y指向z，x指向z的儿子。
            
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
		//否则，y应该是z的后继
		else
		{
			node tmp;
			y = findsuccessor(z);
			y_original_color = y->color;
			x = y->right; //既然y是后继，则只可能有右儿子
            //如果y和z直接是父子关系，直接用y换掉z
			if (y->p == z)
			{
				transplant(z, y);
				y->left = z->left;
				y->left->p = y;
				y->color = z->color;
				tmp = y->p;
			}
            //否则，先将y移动到和z直接是父子关系的位置，再同上
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
        //当y原来是黑色且x不是哨兵时，可能不平衡。试着调用删除程序
		if (y_original_color == BLACK && x != nil)
			delete_fixup(x);

		Tree_num--;
	}
```



#### 3）删除维护

```c++
	//红黑树自平衡系统的最后一环
	//负责在删除操作后修复红黑性质，维持平衡
	void LinearTable::delete_fixup(node x)
	{
        //当x不是根且x是黑色时
		while (x != root && x->color == BLACK)
		{
            //记w为x的兄弟节点
			node w;
            //如果x是父亲的左儿子
			if (x == x->p->left)
			{
				w = x->p->right;
                //注意，w为哨兵时，说明x的兄弟为空。此时没有修复的必要，直接退出即可
                //否则就会re
				if (w == nil)
					break;
                //情况1：x的兄弟节点w是红色的
                //可转化为情况2或3或4处理
				if (w->color == RED)
				{
					w->color = BLACK;
					x->p->color = RED;
					left_rotate(x->p);
					w = x->p->right;
				}
				if(w==nil)
					break;
                //情况2：x的兄弟节点w是黑的，且w的两个儿子是黑的
				if (w->left->color == BLACK && w->right->color == BLACK)
				{
					w->color = RED;
					x = x->p;
				}
				else
				{
                    //情况3：x的兄弟节点w是黑的，且w左儿子红，右儿子黑
                    //可转化为情况4处理
					if (w->right->color == BLACK)
					{
						w->left->color = BLACK;
						w->color = RED;
						right_rotate(w);
						w = x->p->right;
					}
                    //情况4：x的兄弟节点w是黑的，且w的右儿子是红的
					w->color = x->p->color;
					x->p->color = BLACK;
					w->right->color = BLACK;
					left_rotate(x->p);
					x = root;
				}
			}
            //和上面部分是完全对称的，直接左右对调即可
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
        //如果x原来是红色，直接涂成黑色
		x->color = BLACK;
	}
```



## 六、LinearTable的实现

在LinearTable下封装了上面实现的RBT操作函数，通过适当的调用方式和接口，可以完成我们所谓的线性表的操作。



### 1.简单操作

1）构造：new一个哨兵，root指针设为NULL 。

2）析构：递归删除内部节点和叶节点，最后删除哨兵。

3）clear()：递归删除非nil节点。

4）size()：读取private 数据成员Treenum（会随树的删插自动变化）

5）empty()：return size()==0 ;

6）swap()：交换两个表的所有private数据成员。

7）back()&&front()：用数据成员记录表首表尾的节点，删插时动态维护。调用back()与front()时，直接返回即可。



### 2.单元素头尾插入与删除

1）pop操作可以直接用RBT的删除操作来实现。

2）push_back和push_front需要自己修改RBT的insert操作。例如push_back()：直接找到尾节点，让新节点成为尾节点的右儿子即可。

3）虽说我们已经动态维护好了首尾节点的位置，这四个操作应该可以O(1)了。但由于涉及一条树链上size的维护，操作的最优最坏复杂度为O(log(n))。



### 3.索引

1）直接调用顺序统计树的查找操作即可完成。注意索引从0开始，而我们定义的秩从1开始。

2）对于非法索引：（＜0||≥ size()），我的处理方式是返回nil的信息。

3）最坏为O(logn)。



### 4.随机访问迭代器

1）class iterator，为calss LinearTable 的子类。

2）支持关系操作符 <、<=、> 和 >= 。

3）支持迭代器与整型数值 n 之间的加法和减法操作符。

4）支持两个迭代器之间的减法操作符。

5）支持下标操作符[]与取值操作符* 。

6）支持++iter，iter++，--iter，iter--。

7）稳定性：只有被删除的迭代器失效。其它位置的迭代器不受删插影响。

8）begin()：表首迭代器
     end()：越界迭代器（指向nil）
     last()：表尾迭代器

9）所有实现都与节点秩的计算，前驱后继查找，查找某个秩的节点这些操作有关。



### 5.单元素随机插入删除

1）erase()：重载了多个版本，应对不同的参数类型。
      模仿STL list，erase会返回下一个位置的迭代器。

2）insert()：在指定位置前插。同样重载了多个版本。

3）sorted_insert()：标准RBT的插入实现。

4）与push_back()和push_front()同理，O(logn)已是本设计架构下最优的最坏复杂度。



### 6.批量随机插入删除

1）虽然各个版本都实现了，但实现得十分十分十分粗糙没有新意。就是重复调用对应的单插单删操作。

2）若树高为n，数据量为m，则最坏复杂度平均下来为O(mlog(n))。

3）考虑过类似“插子树”的这些方法，但都想不到满意的解法。



### 7.深拷贝

1）不仅要拷贝数据内容，还应该有属于自己的独立的内存。

2）与之对应的是浅拷贝，仅仅拷贝了指针。最终结果是两个对象共享内存。

3）具体实现时，采用了上面效率贼低的批量插入。



### 8.复制构造与赋值

1）调用deep_copy()深拷贝进行实现。

2）重载了迭代器构造和对象常引用构造两个版本。

3）赋值时应该规避自我赋值的情况。



### 9.支持foreach语法

这个其实把迭代器做好了就自然而然实现了：

1. begin() end()

2. 类对象有自己的迭代器

3. 迭代器重载 !=

4. 迭代器重载 前++

5. 迭代器重载 *(解引用)

> http://ftxtool.org/2016/01/23/23/



### 10.批量构造

同样是调用批量插入实现的。在实验中证明效率奇低。



### 11.移动构造

1）第一次接触移动构造的概念。和上面提到的浅拷贝类似。

2）不同的是移动构造中，被拷贝的对象将会丧失对原有内存的掌控权，具体实现就是指针对象全变为NULL。

3）当深拷贝的时空效率比较低，而我们又不care被拷贝对象，采用移动构造抢内存是比较安全快捷的方法。



### 12.其它构造方法

#### 1.从int* 指针构造

实现的也很朴素。遍历+push_back即可。



#### 2.从泛型容器的迭代器进行构造

也没有什么高超技巧，迭代器遍历+push_back即可。



### 七、理论性能分析

### 1.常见stl的性能

| **容器\操作** |    **[]**     |   **列表**    | **头部** | **尾部** | **迭代器** |
| :-----------: | :-----------: | :-----------: | :------: | :------: | :--------: |
|  **vector**   |   **O(1)**    |   **O(n)**    |    \     | **O(1)** |  **随机**  |
|   **list**    |       \       |   **O(1)**    | **O(1)** | **O(1)** |  **双向**  |
|   **deque**   |   **O(1)**    |   **O(n)**    | **O(1)** | **O(1)** |  **随机**  |
|    **map**    | **O(log(n))** | **O(log(n))** |    \     |    \     |  **双向**  |
|    **set**    |       \       | **O(log(n))** |    \     |    \     |  **双向**  |



#### 2.LinearTable性能分析

##### O(1)级复杂度：

诸如：size(),	empty(),	swap(),	begin(),
end(),	front(),	back()等。



##### 单元素删插操作复杂度：

1）push系列，pop系列，insert，erase，sorted_insert
2）[]



最坏：O(log(n))		平均性能：介于O(log(n))与O(1)之间



##### 迭代器性能：

1）自增自减：本质是找到当前节点的后继/前驱。
     最坏：O(log(n))		平均：O(log(n))与O(1)之间

2）遍历：O(n+)

3）其它所有操作（涉及节点秩的计算）：
     最坏：O(log(n)+)		平均：O(log(n))



##### 其它操作：

复杂度比较差，设计也没有什么巧妙之处，故不作分析。



## 八、实验论证

### 1.oj测试



从oj的测试结果来看，性能在RBT的预期范围内。由于常数优化等没有做好，成绩显得比较差。但多数操作仍在O(log(n))的量级上，没有太离谱。



### 2.自我测试

测试程序与结果已附在包中上交。

采用C++的clock()函数计时，单位为毫秒。

采用srand()与rand()生成伪随机数列测试。

mainx.cpp的对应输出结果在resx.txt中。



#### 1）main1

测试内容：生成有序线性表的效率

参与对象：多重集合，优先队列，LinearTable，deque

数据量级：1e5，100组

结论：优先队列最优，多重集最差。LinearTable的sorted_insert操作在多组数据中表现优于对deque进行快排。



#### 2）main2

测试内容：随机索引访问的效率

参与对象：LinearTable，deque

数据量级：1e5，1000组

结论：deque的速度快过LinearTable。后者耗时是前者的4倍左右，符合预期性能。



#### 3）main3

测试内容：头插尾插的效率

参与对象：LinearTable，deque

数据量级：1e5，1000组

结论：deque的速度极快，比LinearTable的速度快了一个量级。这也符合预期性能。



#### 4）main4

测试内容：随机删插的效率

参与对象：LinearTable，deque

数据量级：1e5，100组

结论：这回LinearTable完胜deque。O(log(n))与O(n)的差距十分明显。



#### 5）main5

测试内容：批量头尾插的效率

参与对象：LinearTable

数据量级：1e5，100组

结论：效率一般，是朴素实现批量操作的预期结果。



#### 6）main6

测试内容：批量头尾删的效率

参与对象：LinearTable

数据量级：1e6，100组

结论：效率一般，是朴素实现批量操作的预期结果。



#### 7）main7

测试内容：头插尾插的效率

参与对象：LinearTable，deque

数据量级：1e5，1000组

结论：同第三组的结果。deque的速度比LinearTable的速度快了一个量级。



#### 8）main8

测试内容：批量初始化，迭代器遍历，clear()效率

参与对象：LinearTable，deque

数据量级：1e6，各1组

结论：stl deque在这些方面都明显做的比LinearTable好。这一方面是因为stl容器都会开优化，另一方面也是因为我自己的优化工作没有做到位，设计不巧妙。



## 九、总结

1.各种数据结构都有自己的优势，树形真的不见得就比线性高贵。全能的数据结构是不存在的。

2.红黑顺序统计树为底层的线性表性能应该不止这么点，还有许多的优化空间，可惜我没有做好。

（比如线索化以提高前驱后继的查找速度，提升就很明显了）

3.当数据量足够大时，O(log(n))也会接近O(1)而非O(n)。就尽可能多支持操作并优化复杂度而言，牺牲头插尾插头删尾删与索引的效率，换取随机删插操作O(log(n))的复杂度是值得的。

4.平时我们习惯了用大O表示法表示复杂度，对常数却不太关心。这次pj表明常数也十分重要。当理论复杂度的量级相同时，有策略的优化，精巧的设计往往能给代码进一步提速。当复杂度无法改善时，优化常数是重要的改进方式。



## 十、参考资料

1）《算法导论》chapter 12~14

2）https://blog.csdn.net/baidu_35800355/article/details/105831154

3）https://www.cnblogs.com/xiaojianliu/articles/9022239.html

4）https://www.runoob.com/cplusplus/cpp-overloading.html

5）https://www.cnblogs.com/williamjie/p/11192895.html

6）C++程序设计语言（第4部分：标准库）