#include<iostream>
#include<cstdio>
#include<vector>
#include<cstring>
#include<ctime>
#include"LRUList.h"
#include"Process.h"
#define ie(i,s,t) for(int i=s;i<=t;++i)
#define de(i,s,t) for(int i=s;i>=t;--i)
#define reset(a) memset(a,0,sizeof(a))
#define PT1 9
#define PT2 10
#define PFHEAD 12
#define OFFSET 13
#define INF 1234567890
#define pb push_back
using namespace std;

struct pageFrameE
{
	unsigned int b[4097];
}tbE;

const int memery_size=32*1024*1024;
const int page_size=8*1024;
const int TLBsize=16;
const int cntPageFrame=memery_size/page_size;
const unsigned int bit1 = (1 << PT1) - 1;
const unsigned int bit2 = (1 << PT2) - 1;
const unsigned int bit3 = (1 << OFFSET) - 1;
const unsigned int tlbbit = (1 << PT1 + PT2) - 1;
LRUList TLB(TLBsize, PT1 + PT2 + PFHEAD, 1, PFHEAD + 1), pageFrame(cntPageFrame, 12, 0, 0);//???
int memSet[] = { 0,1,32,64,128,256 };
Process pro[6];
unsigned int pt1bit, pt2bit, offsetbit;
int TLBhit, TLBfault, PThit, PTfault, x, TLBhead, finalAddr;
string str;
int pt1[522], cntFault[6];
pageFrameE mem[4097];
int belong[4097];
vector<unsigned int> pt2;
node *tmp;
bool flag;

void init()
{
	ie(i, 1, 5)pro[i].setSz(memSet[i]);		//设置五个进程的大小
}

void readPageTable(int pr, bool pass)
{
	pt2.clear();
	str = "page_table_"; str += (char)(48 + pr); str += ".txt";	//生成文件名
	freopen(str.c_str(), "r", stdin);		//打开文件
	int t;
	while (scanf("%d %d", &t, &x), !pass&&x >= 0)	pt1[t] = x;	//读取一级页表
	int tot = 0; 
	while (!pass&&scanf("%*d %d", &x) != EOF)pt2.push_back(x);		//读取二级页表
	if (pass)reset(pt1);		//如果第一轮，清空初始化
	freopen("CON", "r", stdin);
}

void outAddr(int pr)
{
	str = "addr_seq_"; str += (char)(48 + pr); str += ".txt";	//生成文件名
	freopen(str.c_str(), "w", stdout);		//打开文件
	ie(i, 1, 500)		//输出地址
		printf("%x\n", pro[pr].getAddr(i));
	freopen("CON", "w", stdout);		//关闭文件
}

void hitPageFrame(int addr)
{
	int addr2=addr&((1LL<<OFFSET)-1);
	int addr1=(addr>>OFFSET)&((1LL<<12)-1);
	pageFrame.hit(addr1);
}

void TLBwork()
{
	tmp = TLB.hit(TLBhead);		//在TLB中查找
	if (tmp != nullptr)++TLBhit;	//找到了，累计TLBhit
	else {
		++TLBfault; return;
	}
	finalAddr = (((tmp->v>>1)&((1LL<<PFHEAD)-1)) << OFFSET) | offsetbit;		//TLB找到对应项，合成物理地址
	hitPageFrame(finalAddr); flag = true;
}

void TLBfaultDeal()
{
	unsigned int TLBEntry=((TLBhead<<PFHEAD)|(finalAddr>>OFFSET))<<1|1;
	TLB.add(node(TLBEntry));
}

void PTwork()
{
	if ((pt1[pt1bit] & 1) == 0)		//第一级页表没有找到对应有效项，出现fault
	{
		++PTfault; return;
	}
	if ((pt2[((pt1[pt1bit] >> 1)&bit2)+pt2bit] & 1) == 0)		//第二级页表没有找到对应有效项，出现fault
	{
		++PTfault; return;
	}
	//attention!
	++PThit;
	finalAddr = (((pt2[((pt1[pt1bit] >> 1)&bit2)+pt2bit]>>1) & ((1<<PFHEAD)-1)) << OFFSET) + offsetbit;	//Page Table找到对应有效项，合成物理地址
	hitPageFrame(finalAddr); flag = true;
	TLBfaultDeal();
}

void PTfaultDeal(int pr)
{
	if ((pt1[pt1bit] & 1) == 0)		//在查询第一级就出现fault
	{
		pt2.push_back(0);
		int beg = pt2.size() - 1;
		pt1[pt1bit]=beg<<1|1;			//链接一级和二级
		ie(i, 1, 1023)pt2.push_back(0);
	}
	if ((pt2[((pt1[pt1bit] >> 1)&bit1)+pt2bit] & 1) == 0)		//第二级页表出现fault
	{
		int p=pageFrame.length();
		if(p<cntPageFrame)		//页框还没有满
		{
			++p;
			pageFrame.add(node(p));		//新分配一个页框
			belong[p] = pr;		//标记页框隶属进程
			pt2[((pt1[pt1bit]>>1)&bit1)+pt2bit]=p<<1|1;		//链接二级页表和物理页框
			finalAddr = (p << OFFSET) | offsetbit;
		}
		else {		//页框已经满了
			tmp=pageFrame.getTail();		//拿出LRU的最后一项
			pt2[((pt1[pt1bit]>>1)&bit1)+pt2bit]=tmp->v<<1|1;		//回收页框给新的页表
			//**************change to other process pagetable
			str="page_table_";str+=(char)(48+belong[tmp->v]);str+=".txt";
			freopen(str.c_str(), "r", stdin);
			freopen("tmp", "w", stdout);
			//查找修改
			int t;
			while (scanf("%d %d", &t, &x), x >= 0)printf("%d %d\n", t, x);	//跳过一级页表
			printf("%d %d\n", t, x);
			while (scanf("%d %d", &t, &x) != EOF)
			{
				if ((x >> 1) == (finalAddr >> OFFSET))printf("%d 0\n", t);
				else printf("%d %d\n", t, x);
			}
			freopen("CON", "r", stdin);
			freopen("tmp", "r", stdin);
			freopen(str.c_str(), "w", stdout);
			//回存
			while (scanf("%d %d", &t, &x) != EOF)printf("%d %d\n", t, x);
			freopen("CON","r",stdin);
			freopen("CON","w",stdout);
			//complete
			pageFrame.add(node(tmp->v));		//更新LRUList
			belong[tmp->v]=pr;		//标记页框隶属进程
			finalAddr = (tmp->v << OFFSET) + offsetbit;
		}
	}
	TLBfaultDeal();
}

void backup(int pr)
{
	str="page_table_";str+=(char)(48+pr);str+=".txt";		//生成文件名
	freopen(str.c_str(),"w",stdout);		//打开文件
	ie(i, 0, 512)printf("%d %d\n", i, pt1[i]);	//输出一级页表
	printf("%d %d\n", INF, -INF);		//作为一级和二级的分界
	int tot = 0, len = pt2.size() - 1;
	ie(i, 0, len)printf("%d %d\n", ++tot, pt2[i]);		//输出二级页表
	freopen("CON","w",stdout);		//关闭文件
}

void work()
{
	ie(cir, 1, 3)
	{
		ie(pr, 1, 5)
		{
			TLB.clear();	//清空TLB
			pro[pr].randomAddr();	//获得随机地址500
			outAddr(pr);	//输出获得的地址
			readPageTable(pr, cir == 1);		//读取页表，如果第一次执行，则清空初始化
			FILE *out; str = "visit_seq_"; str += (char)(48 + pr); str += ".txt";		//生成文件名
			out=fopen(str.c_str(),"w");		//打开文件
			TLBhit = TLBfault = PThit = PTfault = 0;
			ie(i, 1, 500)
			{
				flag = false;
				unsigned int addr = pro[pr].getAddr(i);	//拿出第i个地址
				pt1bit = (addr>>OFFSET+PT2)&bit1;		//位运算获取pt1地址
				pt2bit = (addr>>OFFSET)&bit2;		//位运算获取pt2地址
				offsetbit = addr&bit3;		//位运算获取offset
				TLBhead = (addr >> OFFSET)&tlbbit;		//获取TLB virtual page number
				TLBwork();
				if (!flag)PTwork();
				if (!flag)PTfaultDeal(pr);
				fprintf(out, "%d\t%d\t%d/%d\t%d/%d\n", TLBhead, finalAddr >> OFFSET, TLBhit, TLBfault, PThit, PTfault);//输出该地址访问信息
			}
			fclose(out);		//关闭文件
			backup(pr);		//备份page table
			printf("Process %d's page fault rate:\t%.2lf%% %d\n", pr, 1.*PTfault / 5, PTfault);
			cntFault[pr] += PTfault;
		}
		printf("***************************************\n");
	}
	ie(i, 1, 5)printf("Process %d's total page fault rate:\t%.2lf%%\n", i, cntFault[i] / 15.);
}

int main()
{
	srand((unsigned int)(time(NULL)));
	init();
	work();
	system("pause");
	return 0;
}