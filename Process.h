#pragma once
class Process
{
public:
	Process(int = 0);		//���캯�������ý��̴�С

	void setSz(int);		//���ý��̴�С
	void randomAddr();		//Ϊ�ý�������500�����ʵ�ַ
	unsigned int getAddr(int);		//��ȡ��i�����ʵ�ַ

private:
	int sz, szByte;		//��С
	unsigned int addr[501];		//���ʵ�ַ
};