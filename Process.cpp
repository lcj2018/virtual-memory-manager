#include"Process.h"
#include<cmath>
#include<ctime>
#include<cstdio>
#define ie(i,s,t) for(int i=s;i<=t;++i)

using namespace std;

Process::Process(int sz)
{
	this->sz = sz;
	this->szByte = sz << 20;		//��MB��B
}

void Process::setSz(int sz)
{
	this->sz = sz;
	this->szByte = sz << 20;
}

void Process::randomAddr()
{
	int op, semiSz = szByte >> 1;
	//int cnt = 0;
	ie(i, 1, 50)
	{
		op = rand() % 2;		//�����������10�����ʵ�ַ�Ǵ��ײ�����β������
		if (!op)
		{
			addr[(i - 1) * 10 + 1] = (rand() * (1LL<<15) + rand()) % (semiSz - 9);		//�������һ����ַ
			ie(j, 2, 10)		//����ȡ��ַ
				addr[(i - 1) * 10 + j] = addr[(i - 1) * 10 + 1] + j - 1;
		}
		else {
			addr[(i - 1) * 10 + 1] = (1LL << 32) - semiSz + (rand()*(1LL<<15)+rand()) % (semiSz - 9);		//�������һ����ַ
			//if (addr[(i - 1) * 10 + 1] == 0)++cnt;
			ie(j, 2, 10)		//����ȡ��ַ
				addr[(i - 1) * 10 + j] = addr[(i - 1) * 10 + 1] + j - 1;
		}
	}/*
	printf("%d\n", cnt);
	ie(i, 1, 500)
	{
		printf("%d\n", addr[i]);
		system("pause");
	}
	printf("\n");*/
}

unsigned int Process::getAddr(int pos)
{
	return addr[pos];
}