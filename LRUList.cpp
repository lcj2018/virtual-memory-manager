#include "LRUList.h"

LRUList::LRUList(int msz, int len, int valid, int rsh)
{
	head = tail = new node(); size = 0; maxsz = msz;
	this->len = len; this->valid = valid; this->rsh = rsh;
}

node *LRUList::hit(unsigned int v)
{
	node *p = remove(v);		//�ȳ����ҵ�����
	if (p != nullptr)		//����ҵ��ˣ��Ѹ���ŵ���ǰ
	{ 
		p->succ = head; head->prev = p;
		head = p;
	}
	return p;		//���ظ���
}

void LRUList::clear()
{
	node *p = tail, *tp = p; 
	while (p->prev != nullptr)		//�Ӻ���ǰ��ա��ͷ��ڴ�
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
	if (maxsz > size)++size;		//�����û�ﵽ���ޣ�ֱ�����
	else 	delTail();		//�ﵽ���ޣ�ɾ��β��
	tmp->succ = head; head->prev = tmp;		//�޸�list���ײ�
	head = tmp;
	return true;
}

bool LRUList::delTail()
{
	if (size == 0)return false;
	node *tmp = tail;
	tail = tail->prev;
	tail->succ = nullptr;
	delete tmp;		//��Ҫע���޸�ǰһ��ĺ��
	return true;
}

node *LRUList::remove(unsigned int v)
{
	if (size == 0)return nullptr;
	node *p = head;
	while (p != nullptr)
	{
		if (p->v && ((p->v&((1LL << len + valid) - 1)) >> rsh) == v)		//ȡ����Ч��������жԱ�
		{
			if (p->prev)
			{
				p->prev->succ = p->succ;
				if (p->succ == nullptr)tail = p->prev;		//��ʱ�޸�βָ��
			}
			if (p->succ)
			{
				p->succ->prev = p->prev;
				if (p->prev == nullptr)head = p->succ;		//��ʱ�޸�ͷָ��
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