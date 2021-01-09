#ifndef Mydeque_h
#define Mydeque_h
#include <bits/stdc++.h>

namespace DS
{
    //连续空间缓冲区大小
    #define max_buff 128

    /*
        *迭代器
    */
    struct LinearTable_iterator
    {
        friend class LinearTable;
        private:
            int* fur;       //指向当前元素位置
            int* first;     //指向当前连续空间头的位置
            int* last;      //指向当前连续空间尾的位置（越界位置）
            int** node;     //当前连续空间在map中的位置
    };

    /*
        *类
    */

    class LinearTable
    {
        friend struct LinearTable_iterator;
        public:
            //构造函数与析构函数
            LinearTable(){create();}
            ~LinearTable(){discreate();};

            //一些typedefine
            typedef LinearTable_iterator iterator;
            typedef const LinearTable_iterator const_iterator;
            typedef size_t size_type;
            typedef ptrdiff_t pointer_type;

            //basic problem
            int front() const;              //查询表首单个元素
            void push_front(int);              //表首插入元素
            void pop_front();               //表首删除元素
            int back() const;              //查询表尾单个元素
            void push_back(int);               //表尾插入元素
            void pop_back();                //表尾删除元素
            size_type size() const;         //询问容器长度
            bool empty() const;             //判断容器是否为空

            //索引
            int& operator[](size_type i);
            const int& operator[](size_type i) const;

            //交换与清零
            void clear();
            void swap(LinearTable& );

        private:
            //map数组，存储连续空间的首地址
            int** Map;          

            //迭代器
            iterator start;         //成员函数begin（）返回它
            iterator finish;        //成员函数end（）返回它

            //内存分配工具
            std::allocator<int> alloc;   //控制连续内存块分配的对象
            std::allocator<int*> alloc_map;   //控制map数组内存的分配

            //辅助检查Map是否不够了
            size_type low_limit,high_limit;
            size_type map_size;
            size_type front_id,back_id;

            //构造函数相关
            void create();
            void discreate();
            void init_iter();       //初始化迭代器
            void del_iter();

            //迭代器跳块操作
            void start_forward(size_type);      //start前移（默认前面空间未构造好）
            void finish_backward(size_type);    //finish后移（默认后面空间没构造好）
            void start_backward(size_type);     //start后移（默认后面是构造完的空间）
            void finish_forward(size_type);     //finish前移（默认前面是构造完的空间）

            //内存调整部分
            void rellocate_Map_back();         //push_back时管理map内存的方法
            void rellocate_Map_front();        //push_front时管理map内存的方法
    };

    /*
        *成员函数实现
    */

    void LinearTable::init_iter()
    {
        start_backward(4);
        finish_backward(4);
    }

    void LinearTable::create()
    {
        Map=alloc_map.allocate(8);
        Map[0]=0;   Map[1]=0;   Map[2]=0;
        Map[5]=0;   Map[6]=0;   Map[7]=0;
        Map[3]=0;
        //从中间开始，分配连续空间，一段给队头，一段给队尾
        Map[4]=alloc.allocate(max_buff);
        //初始化头迭代器和尾迭代器
        init_iter();
        //记录Map数组中空间使用情况，随时准备重新分配空间
        low_limit=-1;   high_limit=8;
        map_size=1;
        front_id=4;     back_id=4;

    }

    void LinearTable::del_iter()
    {
        start.fur=0;    start.first=0;
        start.last=0;   start.node=0;

        finish.fur=0;   finish.first=0;
        finish.last=0;  finish.node=0;

    }

    void LinearTable::discreate()
    {
        del_iter();
        for(size_type i=0;i!=high_limit;i++)
        {
            if(Map[i]!=0)
            {
                for(int j=0;j!=max_buff;j++)
                {
                    alloc.destroy(Map[i]+j);
                }
                alloc.deallocate(Map[i],max_buff);
                Map[i]=0;
            }
        }
        alloc_map.deallocate(Map,high_limit);
        //cout<<"Successfully release the memory"<<endl;
    }

    void LinearTable::clear()
    {
        discreate();
        create();
    }

    void LinearTable::swap(LinearTable& t)
    {
        int** tmp;
        tmp=Map;
        Map=t.Map;
        t.Map=tmp;

        iterator tmp2;
        tmp2=start;
        start=t.start;
        t.start=tmp2;

        tmp2=finish;
        finish=t.finish;
        t.finish=tmp2;

        size_type tmp3;
        tmp3=high_limit;
        high_limit=t.high_limit;
        t.high_limit=tmp3;

        tmp3=map_size;
        map_size=t.map_size;
        t.map_size=tmp3;

        tmp3=front_id;
        front_id=t.front_id;
        t.front_id=tmp3;

        tmp3=back_id;
        back_id=t.back_id;
        t.back_id=tmp3;
    }

    void LinearTable::rellocate_Map_back()
    {
        //新Map数组的大小
        size_type new_size=((high_limit)<<1);
        size_type half_size=((new_size-map_size)>>1);
        
        //空间复制
        int** new_Map=alloc_map.allocate(new_size); 
        std::uninitialized_copy(Map+front_id,Map+high_limit,new_Map+half_size);

        //释放原来的Map数组
        for(size_type i=0;i!=high_limit;i++)
            Map[i]=0;
        alloc_map.deallocate(Map,high_limit);

        //重置指针和参数
        Map=new_Map;
        high_limit=new_size;
        front_id=half_size;
        back_id=front_id+map_size-1;

        //处理新的Map
        for(int i=0;i!=front_id;i++)
            Map[i]=0;
        for(int i=back_id;i!=new_size;i++)
            Map[i]=0;
    }

    void LinearTable::rellocate_Map_front()
    {
        //新Map数组的大小
        size_type new_size=((high_limit)<<1);
        size_type half_size=((new_size-map_size)>>1);
        
        //空间复制
        int** new_Map=alloc_map.allocate(new_size); 
        std::uninitialized_copy(Map,Map+high_limit,new_Map+half_size+1);

        //释放原来的Map数组
        for(size_type i=0;i!=high_limit;i++)
            Map[i]=0;
        alloc_map.deallocate(Map,high_limit);

        //重置指针和参数
        Map=new_Map;
        high_limit=new_size;
        front_id=half_size;
        back_id=front_id+map_size-1;

        //处理新的Map
        for(int i=0;i<=front_id;i++)
            Map[i]=0;
        for(int i=back_id+1;i!=new_size;i++)
            Map[i]=0;
    }

    void LinearTable::start_forward(size_type id)
    {
        start.node=&Map[id];
        start.first=*start.node;  
        start.last=*start.node+max_buff-1;
        start.fur=start.last;
    }

    void LinearTable::finish_backward(size_type id)
    {
        finish.node=&Map[id];
        finish.first=*finish.node;
        finish.last=*finish.node+max_buff-1;
        finish.fur=finish.first;
    }

    void LinearTable::start_backward(size_type id)
    {
        start.node=&Map[id];
        start.first=*start.node;
        start.last=*start.node+max_buff-1;
        start.fur=start.first;
    }

    void LinearTable::finish_forward(size_type id)
    {
        finish.node=&Map[id];
        finish.first=*finish.node;
        finish.last=*finish.node+max_buff-1;
        finish.fur=finish.last;
    }

    LinearTable::size_type LinearTable::size() const
    {
        return ((back_id-front_id-1)*max_buff+(start.last-start.fur+finish.fur-finish.first+1));
    }

    bool LinearTable::empty() const
    {
        return (*this).size()==0;
    }

    int LinearTable::front() const
    {
        return (*this)[0];
    }

    int LinearTable::back() const
    {
        size_type id=(*this).size();
        return (*this)[id-1];
    }

    void LinearTable::push_front(int val)
    {
        start.fur--;
        //迭代器不需要跳块
        if(start.fur>=start.first)
        {
            alloc.construct(start.fur,val);
        }
        //需要跳块
        else
        {
            front_id--;
            map_size++;
            if(front_id==low_limit)
            {
                rellocate_Map_front();
            }
            if(Map[front_id]==0)
                Map[front_id]=alloc.allocate(max_buff);
            start_forward(front_id);
            alloc.construct(start.fur,val);
        }
    }

    void LinearTable::push_back(int val)
    {
        if(finish.fur<=finish.last)
        {
            alloc.construct(finish.fur,val);
        }
        else
        {
            back_id++;
            map_size++;
            if(back_id==high_limit)
            {
                rellocate_Map_back();
            }
            if(Map[back_id]==0)
                Map[back_id]=alloc.allocate(max_buff);
            finish_backward(back_id);
            alloc.construct(finish.fur,val);
        }
        finish.fur++;
    }

    void LinearTable::pop_front()
    {
        if(start.fur!=finish.fur)
        {
            start.fur++;
            if(start.fur>start.last)
            {
                front_id++;
                map_size--;
                start_backward(front_id);
            }
        }
    }

    void LinearTable::pop_back()
    {
        if(start.fur!=finish.fur)
        {
            finish.fur--;
            if(finish.fur<finish.first)
            {
                back_id--;
                map_size--;
                finish_forward(back_id);
            }
        }
    }

    int& LinearTable::operator[](size_type id)
    {
        if(id>=(*this).size()||id<0)
            throw std::logic_error("Warning! Index is out of range!");
        
        if(front_id==back_id)
            return *(start.fur+id);
        else
        {
            if(start.fur+id<=start.last)
                return *(start.fur+id);
            else
            {
                id-=(start.last-start.fur+1);
                //计算从start开始要移动的块数
                size_type num=id/max_buff;
                size_type extra=id%max_buff;
                return Map[front_id+num+1][extra];
            }
        }
    }

    const int& LinearTable::operator[](size_type id) const
    {
        if(id>=(*this).size()||id<0)
            throw std::logic_error("Warning! Index is out of range!");

        if(front_id==back_id)
            return *(start.fur+id);
        else
        {
            if(start.fur+id<=start.last)
                return *(start.fur+id);
            else
            {
                id-=(start.last-start.fur+1);
                //计算从start开始要移动的块数
                size_type num=id/max_buff;
                size_type extra=id%max_buff;
                return Map[front_id+num+1][extra];
            }
        }
    }
}
#endif