#include "LRUList.h"

LRUList::LRUList(int msz, int len, int valid, int rsh)
{
	head = tail = new node(); size = 0; maxsz = msz;
	this->len = len; this->valid = valid; this->rsh = rsh;
}

node *LRUList::hit(unsigned int v)
{
	node *p = remove(v);		//先尝试找到该项
	if (p != nullptr)		//如果找到了，把该项放到最前
	{ 
		p->succ = head; head->prev = p;
		head = p;
	}
	return p;		//返回该项
}

void LRUList::clear()
{
	node *p = tail, *tp = p; 
	while (p->prev != nullptr)		//从后往前清空、释放内存
	{
		tp = p->prev;
		p->prev->succ = nullptr;
		delete p; --size;
		p = tp;
	}
	head->v = 0; tail = head;
}

bool LRUList::add(node ele)
{
	node *tmp = new node();
	tmp->v = ele.v;
	if (maxsz > size)++size;		//如果还没达到上限，直接添加
	else 	delTail();		//达到上限，删除尾部
	tmp->succ = head; head->prev = tmp;		//修改list的首部
	head = tmp;
	return true;
}

bool LRUList::delTail()
{
	if (size == 0)return false;
	node *tmp = tail;
	tail = tail->prev;
	tail->succ = nullptr;
	delete tmp;		//需要注意修改前一项的后继
	return true;
}

node *LRUList::remove(unsigned int v)
{
	if (size == 0)return nullptr;
	node *p = head;
	while (p != nullptr)
	{
		if (p->v && ((p->v&((1LL << len + valid) - 1)) >> rsh) == v)		//取出有效数据项进行对比
		{
			if (p->prev)
			{
				p->prev->succ = p->succ;
				if (p->succ == nullptr)tail = p->prev;		//及时修改尾指针
			}
			if (p->succ)
			{
				p->succ->prev = p->prev;
				if (p->prev == nullptr)head = p->succ;		//及时修改头指针
			}
			p->prev = nullptr;
			return p;
		}
		p = p->succ;
	}
	return nullptr;
}

node *LRUList::getTail()
{
	return tail;
}

int LRUList::length()
{
	return size;
}