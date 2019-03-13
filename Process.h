#pragma once
class Process
{
public:
	Process(int = 0);		//构造函数，设置进程大小

	void setSz(int);		//设置进程大小
	void randomAddr();		//为该进程生成500个访问地址
	unsigned int getAddr(int);		//获取第i个访问地址

private:
	int sz, szByte;		//大小
	unsigned int addr[501];		//访问地址
};