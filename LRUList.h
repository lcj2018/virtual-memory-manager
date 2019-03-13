#pragma once

struct node {
	unsigned int v;		//Entry，不同LRU划分略有不同
	node *prev, *succ;		//指向前一项和后一项的指针

	node()
	{
		v = 0;
		prev = succ = nullptr;
	}

	node(unsigned int v)
	{
		this->v=v;
		prev=succ=nullptr;
	}
};

class LRUList {
public:
	LRUList(int, int, int = 1, int rsh = 1);

	node *hit(unsigned int);	//找到并移至最前
	bool add(node);	//将node添加至list最前
	node *getTail();
	void clear();
	int length();
private:
	node *remove(unsigned int);	//找到并删除，返回找到的node
	bool delTail();	//删尾

	node *head,*tail;		//头指针、尾指针
	int size, maxsz, valid, len, rsh;		//当前list的大小、大小上限，有效位长度，前部数据项占用位数，位运算时右移的位数
};