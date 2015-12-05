#pragma once
#include<iostream>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
using namespace std;

#define random(x) (rand()%x)
#define MAXNUM 5			//�ַ�ȡֵ������
#define SHORTLEN 5000		//�̴�����
#define LONGLEN 123456789	//��������


////////////////////////////////////////////////////////////////////////
//*********************Suday�㷨��***********************

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
	long long lp = 0;//longListָ��
	long long sp = 0;//shortListָ��
	long long lhead = lp - sp;//longList����shortList��Ԫ�ض����Ԫ���±�
	long long lrear = lhead + shortLength - 1;//longList����shortListβԪ�ض����Ԫ���±�
	long long result = -1;//longList��shortList��λ��
	long long count = 0;//�ƶ�����

	while (lrear < longLength)
	{
		count++;
		if (shortList[sp] == longList[lp])
		{
			if (lp == lrear)
			{
				cout << "sunday�Ƚϴ�����" << count << endl;
				return lhead;
			}
			lp++;
			sp++;
		}
		else
		{
			if (lrear + 1 >= longLength)//��鵽ĩβ�ˣ�����ѭ��
			{
				cout << "sunday�Ƚϴ�����" << count << endl;
				return -1;
			}
			lhead += sundaystep[longList[lrear + 1]];
			lrear = lhead + shortLength - 1;
			lp = lhead;
			sp = 0;
		}
	}
	cout << "sunday�Ƚϴ�����" << count << endl;
	return result;
}


//*********************Monday�㷨��***********************

void getstep(char shortList[], char longList[], int shortLen, long long longLen, bool** flag, int **step)
{
	//flag[i][j]=true��ʾshort�Ӵ��� i-j��i�Ӵ�(����Ϊj+1) �� shortlen-j-1��shortlen-1�Ӵ�(����Ϊj+1)��ͬ
	//��flag���鸳ֵ
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
	//int maxindex[256];//��shortList�г����е��ַ���shortList�����һ�γ��ֵ�λ�ã�ͬʱҲ�����Щ�ַ���������shortList��
	int **index = new int*[shortLen];//index[i][j]=k��ʾ��0��i�Ӷ��У�j�ַ���shortList�����һ�γ��ֵ�λ��Ϊk
	for (int i = 0; i < shortLen; i++)
		index[i] = new int[MAXNUM];

	for (int i = 0; i < shortLen; i++)//ÿһ���Ӷ�
		getMaxIndex(index[i], shortList, i + 1);

	//count=0����ƥ������������ֵ
	for (int c = 0; c < MAXNUM; c++)
		step[c][0] = shortLen - index[shortLen - 1][c];

	//����index�����flag��������step����
	//step[c][count]=k��ʾ��β����һ�ַ�Ϊc �� ��ƥ��β����j���ַ�ʱ���ƶ�����Ϊk
	for (int c = 0; c < MAXNUM; c++)//ÿһ���ַ�
	{
		for (int count = 1; count < shortLen; count++)//ÿ����ƥ�䳤��
		{
			bool set = false;
			for (int end = shortLen - 1; end >= 0 && !set; end--)//ÿ���Ӷ�
			{
				int tempcount = count;
				if (index[end][c]>0)//��������λ�ô���0�������ֹ��Ҳ���0λ�ó��֣�
					if (flag[index[end][c] - 1][min(count, index[end][c]) - 1] == true)//�Ҵ�λ����ǰcount���ַ�����ƥ��
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
	long long lhead = 0;//longList����shortList��Ԫ�ض����Ԫ���±�
	long long lrear = shortLen - 1;//longList����shortListβԪ�ض����Ԫ���±�
	long long lp = lrear;//longListָ��
	long long sp = shortLen - 1;//shortListָ��
	long long result = -1;//longList��shortList��λ��
	long long count = 0;
	while (lrear < longLen)
	{
		count++;
		if (shortList[sp] == longList[lp])
		{
			if (lp == lhead)
			{
				cout << "monday�Ƚϴ�����" << count << endl;
				return lhead;
			}
			lp--;
			sp--;
		}
		else
		{
			if (lrear >= longLen - 1)//��鵽ĩβ�ˣ�����ѭ��
			{
				cout << "monday�Ƚϴ�����" << count << endl;
				return -1;
			}
			lhead += step[longList[lrear + 1]][lrear - lp];//lrear-lpΪ��ƥ�䳤��
			lrear = lhead + shortLen - 1;
			lp = lrear;
			sp = shortLen - 1;
		}
	}
	cout << "monday�Ƚϴ�����" << count << endl;
	return result;
}


//*********************KMP�㷨��***********************

void getNext(char shortList[], int next[], int shortLen)
{
	next[0] = -1;
	int pointer = -1;//��ʼֵ��ǰ���Ӵ���ǰ׺���ȣ�ǰ���Ӵ�ǰ׺���һ��Ԫ��λ�ã�
	//pointer����ָ�����shortList[index]֮ǰ���Ӵ����ǰ׺��ĵ�һ��Ԫ��
	int index = 0;//next����ָ�룬��1��ʼΪnext[index]��ֵ
	while (index <= shortLen - 1)
	{
		if (pointer == -1 || shortList[index] == shortList[pointer])
		{
			pointer++;//ǰ׺����++��λ�ú���һλ��
			index++;
			//if (shortList[index] == shortList[pointer])//index��ƥ�䣬��pionterλ��������ͬ��Ҳһ����ƥ�䣬Ӧ��������pointer
			//	next[index] = next[pointer];//��ͷ��ʼ��ÿ��index����������Ӧ��pointerλ��Ԫ����ͬ��������������ͬ��ֻ��һ����ǰ�������ɱ�֤��ͬ��
			//else
			next[index] = pointer;//��ͬ����ƥ�������pointerλ�ÿ�shortList[pointer]�Ƿ�ƥ��
		}
		else
			//��δ��shortList[index]֮ǰ���Ӵ���ǰ׺�У�Ѱ�Ҹ�С��ǰ׺��ǰ׺��ǰ׺������ǰ׺���׺�ĺ�׺��ͬ��Ȼ���ܲ���ƥ��
			pointer = next[pointer];
	}
}

long long KMP(char longList[], char shortList[], int next[], long long  longLen, int shortLen)
{
	long long lp = 0;//longList�е�ָ��
	int sp = 0;//shortList�е�ָ��
	long long count = 0;
	while (lp < longLen && sp < shortLen)
	{
		count++;
		if (sp == -1 || longList[lp] == shortList[sp])//����ƥ�����¼�飬����ƥ��
		{
			lp++;
			sp++;
		}
		else//ȡǰ���� #lp֮ǰ�Ӵ���ĳ��׺# ��ͬ��ĳǰ׺��ĵ�һ��Ԫ��
		{
			sp = next[sp];
		}
	}
	cout << "KMP�Ƚϴ�����   " << count << endl;
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
	int maxindex[MAXNUM];//��shortList�г����е��ַ���shortList�����һ�γ��ֵ�λ�ã�ͬʱҲ�����Щ�ַ���������shortList��
	getMaxIndex(maxindex, shortList, shortLen);
	int sundaystep[MAXNUM];
	getSundayStep(sundaystep, maxindex, shortLen);//�ƶ�����
	clock_t sunday_start_time = clock();//sunday��ʼʱ��
	long long Sunday_result;
	//for (int i = 0; i < 100;i++)

	Sunday_result = Sunday(shortList, longList, sundaystep, shortLen, longLen);
	clock_t sunday_end_time = clock();//sudnay����ʱ��
	if (Sunday_result < 0)
		cout << "δ���ı������ҵ�ģʽ����" << endl;
	else
		cout << "ģʽ�����ı����е���ʼ�±�Ϊ��" << (long long)Sunday_result << endl;
	//�����Ƿ���ͬ
	for (int i = 0; i < shortLen; i++)
		if (longList[Sunday_result + i] != shortList[i])
			cout << "!!!!!!!!!!!!!!!" << endl;
	cout << "The Sunday run time is: " << (sunday_end_time - sunday_start_time) << " clock-ticks!" << endl;//�������ʱ��

	//########################################
	//########################################
	//########################################
	//flag[i][j]=true��ʾshort�Ӵ��� i-j��i�Ӵ�(����Ϊj+1) �� shortlen-j-1��shortlen-1�Ӵ�(����Ϊj+1)��ͬ
	bool **flag = new bool*[shortLen];
	for (int i = 0; i < shortLen; i++)
		flag[i] = new bool[shortLen];
	//����step���飬step[c][k]=x��ʾ����c�ַ�ʧ���ҵ�ǰ�Ӻ���ǰƥ�����k���ַ�ʱ������ƶ����루��֤����©��Ϊx
	int **step = new int*[MAXNUM];
	for (int c = 0; c < MAXNUM; c++)
	{
		step[c] = new int[shortLen];
		for (int i = 0; i < shortLen; i++)
			step[c][i] = shortLen + 1;
	}

	getstep(shortList, longList, shortLen, longLen, flag, step);
	long long Monday_result;
	clock_t monday_start_time = clock();//monday��ʼʱ��
	//for (int i = 0; i < 100;i++)//��100��
	Monday_result = monday(shortList, longList, step, shortLen, longLen);
	clock_t monday_end_time = clock();//sudnay����ʱ��

	if (Monday_result < 0)
		cout << "δ���ı������ҵ�ģʽ����" << endl;
	else
		cout << "ģʽ�����ı����е���ʼ�±�Ϊ��" << (long long)Monday_result << endl;
	cout << "The Monday run time is: " << (monday_end_time - monday_start_time) << " clock-ticks!" << endl;//�������ʱ��

	//########################################
	//########################################
	//########################################
	int *next = new int[shortLen + 1];
	//next[index]��indexǰ���Ӵ���ǰ׺��ĵ�һ��Ԫ��λ�ã���ƥ��ʱ�����λ�õ�Ԫ�ؿ��Ƿ�ƥ��
	//next�Ƕ�Ӧ�Ӵ��ǰ׺������������ƣ���˲�-1��
	getNext(shortList, next, shortLen);
	clock_t KMP_start_time = clock();//KMP��ʼʱ��
	long long KMP_result = KMP(longList, shortList, next, longLen, shortLen);
	clock_t KMP_end_time = clock();//KMP��ʼʱ��
	if (KMP_result < 0)
		cout << "δ���ı������ҵ�ģʽ����" << endl;
	else
		cout << "ģʽ�����ı����е���ʼ�±�Ϊ��" << KMP_result << endl;
	cout << "The KMP run time is: " << (KMP_end_time - KMP_start_time) << " clock-ticks!" << endl;//�������ʱ��

	system("pause");
}