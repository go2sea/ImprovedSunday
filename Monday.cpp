#pragma once
#include<iostream>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
using namespace std;

#define random(x) (rand()%x)
#define MAXNUM 5			//字符取值可能数
#define SHORTLEN 5000		//短串长度
#define LONGLEN 123456789	//长串长度


////////////////////////////////////////////////////////////////////////
//*********************Suday算法：***********************

void getMaxIndex(int maxindex[], char* shortList, int shortLength)
{
	for (int i = 0; i < MAXNUM; i++)
		maxindex[i] = -1;
	for (int i = 0; i < shortLength; i++)
	{
		if (maxindex[shortList[i]] < i)
			maxindex[shortList[i]] = i;
	}
}

void getSundayStep(int *sundaystep, int *maxindex, int shortLen)
{
	for (int i = 0; i < MAXNUM; i++)
		sundaystep[i] = (maxindex[i] == -1 ? shortLen + 1 : shortLen - maxindex[i]);
}


long long Sunday(char *shortList, char *longList, int *sundaystep, int shortLength, long long longLength)
{
	long long lp = 0;//longList指针
	long long sp = 0;//shortList指针
	long long lhead = lp - sp;//longList中与shortList首元素对齐的元素下标
	long long lrear = lhead + shortLength - 1;//longList中与shortList尾元素对齐的元素下标
	long long result = -1;//longList中shortList的位置
	long long count = 0;//移动次数

	while (lrear < longLength)
	{
		count++;
		if (shortList[sp] == longList[lp])
		{
			if (lp == lrear)
			{
				cout << "sunday比较次数：" << count << endl;
				return lhead;
			}
			lp++;
			sp++;
		}
		else
		{
			if (lrear + 1 >= longLength)//检查到末尾了，跳出循环
			{
				cout << "sunday比较次数：" << count << endl;
				return -1;
			}
			lhead += sundaystep[longList[lrear + 1]];
			lrear = lhead + shortLength - 1;
			lp = lhead;
			sp = 0;
		}
	}
	cout << "sunday比较次数：" << count << endl;
	return result;
}


//*********************Monday算法：***********************

void getstep(char shortList[], char longList[], int shortLen, long long longLen, bool** flag, int **step)
{
	//flag[i][j]=true表示short子串中 i-j至i子串(长度为j+1) 与 shortlen-j-1至shortlen-1子串(长度为j+1)相同
	//对flag数组赋值
	for (int i = 0; i < shortLen; i++)
	{
		for (int j = 0; j < shortLen; j++)
		{
			flag[i][j] = true;
			if (i - j < 0)
			{
				flag[i][j] = false;
				continue;
			}
			for (int k = 0; k <= j; k++)
				if (shortList[i - k] != shortList[shortLen - 1 - k])
				{
				flag[i][j] = false;
				break;
				}
		}
	}
	//int maxindex[256];//存shortList中出现有的字符在shortList中最后一次出现的位置，同时也标记那些字符出现在了shortList中
	int **index = new int*[shortLen];//index[i][j]=k表示：0至i子段中，j字符在shortList中最后一次出现的位置为k
	for (int i = 0; i < shortLen; i++)
		index[i] = new int[MAXNUM];

	for (int i = 0; i < shortLen; i++)//每一个子段
		getMaxIndex(index[i], shortList, i + 1);

	//count=0即已匹配的情况单独赋值
	for (int c = 0; c < MAXNUM; c++)
		step[c][0] = shortLen - index[shortLen - 1][c];

	//根据index数组和flag数组设置step数组
	//step[c][count]=k表示：尾部后一字符为c 且 已匹配尾部的j个字符时，移动步长为k
	for (int c = 0; c < MAXNUM; c++)//每一个字符
	{
		for (int count = 1; count < shortLen; count++)//每个已匹配长度
		{
			bool set = false;
			for (int end = shortLen - 1; end >= 0 && !set; end--)//每个子段
			{
				int tempcount = count;
				if (index[end][c]>0)//的最后出现位置大于0（即出现过且不在0位置出现）
					if (flag[index[end][c] - 1][min(count, index[end][c]) - 1] == true)//且此位置向前count个字符可以匹配
					{
					step[c][count] = shortLen - index[end][c];
					set = true;
					break;
					}
				if (index[end][c] == 0)
				{
					step[c][count] = shortLen;
					set = true;
					break;
				}
			}
		}

	}
}

long long monday(char shortList[], char longList[], int** step, int shortLen, long long  longLen)
{
	long long lhead = 0;//longList中与shortList首元素对齐的元素下标
	long long lrear = shortLen - 1;//longList中与shortList尾元素对齐的元素下标
	long long lp = lrear;//longList指针
	long long sp = shortLen - 1;//shortList指针
	long long result = -1;//longList中shortList的位置
	long long count = 0;
	while (lrear < longLen)
	{
		count++;
		if (shortList[sp] == longList[lp])
		{
			if (lp == lhead)
			{
				cout << "monday比较次数：" << count << endl;
				return lhead;
			}
			lp--;
			sp--;
		}
		else
		{
			if (lrear >= longLen - 1)//检查到末尾了，跳出循环
			{
				cout << "monday比较次数：" << count << endl;
				return -1;
			}
			lhead += step[longList[lrear + 1]][lrear - lp];//lrear-lp为已匹配长度
			lrear = lhead + shortLen - 1;
			lp = lrear;
			sp = shortLen - 1;
		}
	}
	cout << "monday比较次数：" << count << endl;
	return result;
}


//*********************KMP算法：***********************

void getNext(char shortList[], int next[], int shortLen)
{
	next[0] = -1;
	int pointer = -1;//初始值是前面子串的前缀长度（前面子串前缀后第一个元素位置）
	//pointer最终指向添加shortList[index]之前的子串的最长前缀后的第一个元素
	int index = 0;//next数组指针，从1开始为next[index]赋值
	while (index <= shortLen - 1)
	{
		if (pointer == -1 || shortList[index] == shortList[pointer])
		{
			pointer++;//前缀长度++（位置后移一位）
			index++;
			//if (shortList[index] == shortList[pointer])//index不匹配，若pionter位置与他相同，也一定不匹配，应避免这种pointer
			//	next[index] = next[pointer];//从头开始，每个index都不与他对应的pointer位置元素相同，所以若出现相同，只需一次向前迭代即可保证不同了
			//else
			next[index] = pointer;//不同，不匹配就跳到pointer位置看shortList[pointer]是否匹配
		}
		else
			//从未加shortList[index]之前的子串的前缀中，寻找更小的前缀（前缀的前缀），该前缀与后缀的后缀相同，然后看能不能匹配
			pointer = next[pointer];
	}
}

long long KMP(char longList[], char shortList[], int next[], long long  longLen, int shortLen)
{
	long long lp = 0;//longList中的指针
	int sp = 0;//shortList中的指针
	long long count = 0;
	while (lp < longLen && sp < shortLen)
	{
		count++;
		if (sp == -1 || longList[lp] == shortList[sp])//都不匹配重新检查，或者匹配
		{
			lp++;
			sp++;
		}
		else//取前面与 #lp之前子串的某后缀# 相同的某前缀后的第一个元素
		{
			sp = next[sp];
		}
	}
	cout << "KMP比较次数：   " << count << endl;
	if (sp == shortLen)
		return(long long)(lp - shortLen);
	else
		return -1;
}


void main()
{
	srand((unsigned int)time(0));
	int shortLen = SHORTLEN;
	long long longLen = LONGLEN;
	char* shortList = new char[shortLen];
	char* longList = new char[longLen];
	for (int i = 0; i < shortLen; i++)
	{
		unsigned char c = random(MAXNUM);
		shortList[i] = c;
	}
	for (long long i = 0; i < (long long)(longLen - shortLen); i++)
	{
		unsigned char c = random(MAXNUM);
		longList[i] = c;
	}
	for (int i = 0; i < shortLen; i++)
		longList[i + longLen - shortLen] = shortList[i];

	/*int shortLen = 3;
	int longLen = 6;
	char shortList[3] = { 'a', 'a', 'b' };
	char longList[6] = { 'a', 'b', 'a' ,'a', 'b', 'c' };*/

	//########################################
	//########################################
	//########################################
	int maxindex[MAXNUM];//存shortList中出现有的字符在shortList中最后一次出现的位置，同时也标记那些字符出现在了shortList中
	getMaxIndex(maxindex, shortList, shortLen);
	int sundaystep[MAXNUM];
	getSundayStep(sundaystep, maxindex, shortLen);//移动步长
	clock_t sunday_start_time = clock();//sunday开始时间
	long long Sunday_result;
	//for (int i = 0; i < 100;i++)

	Sunday_result = Sunday(shortList, longList, sundaystep, shortLen, longLen);
	clock_t sunday_end_time = clock();//sudnay结束时间
	if (Sunday_result < 0)
		cout << "未在文本串中找到模式串！" << endl;
	else
		cout << "模式串在文本串中的起始下标为：" << (long long)Sunday_result << endl;
	//检验是否相同
	for (int i = 0; i < shortLen; i++)
		if (longList[Sunday_result + i] != shortList[i])
			cout << "!!!!!!!!!!!!!!!" << endl;
	cout << "The Sunday run time is: " << (sunday_end_time - sunday_start_time) << " clock-ticks!" << endl;//输出运行时间

	//########################################
	//########################################
	//########################################
	//flag[i][j]=true表示short子串中 i-j至i子串(长度为j+1) 与 shortlen-j-1至shortlen-1子串(长度为j+1)相同
	bool **flag = new bool*[shortLen];
	for (int i = 0; i < shortLen; i++)
		flag[i] = new bool[shortLen];
	//设置step数组，step[c][k]=x表示：当c字符失配且当前从后往前匹配对了k个字符时，最大移动距离（保证不会漏）为x
	int **step = new int*[MAXNUM];
	for (int c = 0; c < MAXNUM; c++)
	{
		step[c] = new int[shortLen];
		for (int i = 0; i < shortLen; i++)
			step[c][i] = shortLen + 1;
	}

	getstep(shortList, longList, shortLen, longLen, flag, step);
	long long Monday_result;
	clock_t monday_start_time = clock();//monday开始时间
	//for (int i = 0; i < 100;i++)//做100遍
	Monday_result = monday(shortList, longList, step, shortLen, longLen);
	clock_t monday_end_time = clock();//sudnay结束时间

	if (Monday_result < 0)
		cout << "未在文本串中找到模式串！" << endl;
	else
		cout << "模式串在文本串中的起始下标为：" << (long long)Monday_result << endl;
	cout << "The Monday run time is: " << (monday_end_time - monday_start_time) << " clock-ticks!" << endl;//输出运行时间

	//########################################
	//########################################
	//########################################
	int *next = new int[shortLen + 1];
	//next[index]存index前面子串中前缀后的第一个元素位置，不匹配时换这个位置的元素看是否匹配
	//next是对应子串最长前缀长度数组的右移（左端补-1）
	getNext(shortList, next, shortLen);
	clock_t KMP_start_time = clock();//KMP开始时间
	long long KMP_result = KMP(longList, shortList, next, longLen, shortLen);
	clock_t KMP_end_time = clock();//KMP开始时间
	if (KMP_result < 0)
		cout << "未在文本串中找到模式串！" << endl;
	else
		cout << "模式串在文本串中的起始下标为：" << KMP_result << endl;
	cout << "The KMP run time is: " << (KMP_end_time - KMP_start_time) << " clock-ticks!" << endl;//输出运行时间

	system("pause");
}