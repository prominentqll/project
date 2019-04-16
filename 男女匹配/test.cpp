#include<iostream>
#include<vector>
#include <windows.h>
#define  COUNT 100
#pragma warning(disable:4996)
using namespace std;

/*
����˼·��
�����ࣺmale�ࡢfemale�ࡢplayer��
male:ѡ��һ�����������Ů�����������룬sendInvite()
female:1.��Ҫ��¼�ж��ٸ����������Լ� vector<int>  maleID;
            2.�������������ѡ�����Լ���ƥ���  matchMale();

player:id����Ϊ-1
ƥ�����1.����ȴ������
                 2.�������ͬ�������£��ж������������������
				 3.����Ⱥ�������������ͬ�������£�idС������
*/

class person
{
public:
	person(int a, int b, int c, int d, int e, int f, int g) :
	id(a),apperance(b),character(c),
	wealth(d),expectapperance(e),expectcharacter(f),expectwealth(g) {}

	int id;//��ʶ
	int apperance;//��ò
	int character;//Ʒ��
	int wealth;//�Ƹ�
	int expectapperance;//������ò
	int expectcharacter;//����Ʒ��
	int expectwealth;
};

class male;

class female:public person //Ů����
{
public:
	female(int a, int b, int c, int d, int e, int f, int g): person(a, b, c, d, e, f, g) {}

	void recvInvite(int maleid)//��������
	{
		maleID.push_back(maleid);
	}

	int matchMale(vector<male>& malevec);//�ڽ��յ���������ƥ�������
	vector<int>& getMaleID()
	{
		return maleID;
	}

private:
	vector<int> maleID;//��¼�����������id
	friend class male;
};

class male :public person //������
{
public:
	male(int a, int b, int c, int d, int e, int f, int g) :person(a, b, c, d, e, f, g) {}

	void sendInvite(vector<female> &femalevec, int index)//��������
	{
		femalevec[index].recvInvite(id);//���Լ���id�����Լ������Ů��
	}

};

int female::matchMale(vector<male>& malevec)//�ڽ��յ���������ƥ�������
{
	//int len1 = maleID.size();//ѡ�����������ĸ���
	//int len2= malevec.size();//�����ĸ���

	int curSatisfaction = 0;//��ǰ������ȣ���ߵ������
	int preSatisfaction = 0;//ǰһ�ε������
	int index = 0;//��¼�������ߵ��������±�
	

	for (vector<int>::iterator it1 = maleID.begin(); it1 != maleID.end(); ++it1)
	{
		int j = 0;
		for (vector<male>::iterator it2 = malevec.begin(); it2 != malevec.end(); ++it2,++j)//�������е�����
		{
			if (malevec[j].id == *it1)//�±�Ϊj������ѡ������
			{
				/*���������*/
				curSatisfaction = expectapperance * malevec[j].apperance + expectcharacter * malevec[j].character + expectwealth * malevec[j].wealth;
				if (curSatisfaction > preSatisfaction)
				{
					preSatisfaction = curSatisfaction;
					index = j;
				}
				else if (curSatisfaction == preSatisfaction)//�������ȣ��ж���������
				{
					int  curItself = malevec[j].apperance + malevec[j].character + malevec[j].wealth;
					int  preItself = malevec[index].apperance + malevec[index].character + malevec[j].wealth;
					if (curItself > preItself)
					{
						index = j;
					}
					else if (curItself == preItself)//����������ͬ��ѡȡid��С��
					{
						if (malevec[j].id < malevec[index].id)
						{
							index = j;
						}
					}
				}
			}
		}
	}
	return index;//���Լ�ƥ��������ı��
}



class player:public person//������
{
public:
	player(int a, int b, int c, int d, int e, int f, int g, int s) :person(a, b, c, d, e, f, g), sex(s) {}

	int getSex()
	{
		return sex;
	}

	male  makeMale()
	{
		return  male(id, apperance, character, wealth, expectapperance, expectcharacter, expectwealth);
	}
	
	female makeFemale()
	{
		return  female(id, apperance, character, wealth, expectapperance, expectcharacter, expectwealth);
	}
private:
	int sex;//�Ա�
};


class  match
{
public:
	//��ȡ���������ݽ�malevec����
	
	void readMale()
	{
		int id, a, b, c, d, e, f;
		FILE *fp = fopen("male.txt", "r");

		
		for (int i = 0; i < COUNT; ++i)
		{
			fscanf(fp, "%d,%d,%d,%d,%d,%d,%d", &id,&a, &b, &c, &d, &e, &f);
			malevec.push_back(male(id,a, b, c, d, e, f));
		}
		fclose(fp);
	}

	//��ȡŮ�������ݽ�fmalevec����
	void readFemale()
	{
		int id, a, b, c, d, e, f;
		FILE *fp = fopen("female.txt", "r");

		for (int i = 0; i < COUNT; ++i)
		{
			fscanf(fp, "%d,%d,%d,%d,%d,%d,%d", &id,&a, &b, &c, &d, &e, &f);
			femalevec.push_back(female(id,a, b, c, d, e, f));
		}
		fclose(fp);
	}

	//�����ǵ����ݽ�playervec
	void  readPlayer()
	{ 

		int sex, a, b, c, d, e, f;
		FILE *fp = fopen("players.txt", "r");
		
		for (int i = 0; i < COUNT;++i)
		{
			fscanf(fp, "%d,%d,%d,%d,%d,%d,%d", &sex,&a, &b, &c, &d, &e, &f);
			playervec.push_back(player(-1, a, b, c, d, e, f, sex));
		}
		fclose(fp);
	}

	//���е�������ѡŮ������������
	void maleSendInvite(vector<male> &malevec)//Ҫ�����������֮�������
	{
		int curSatisfaction = 0;
		int preSatisfaction= 0;

		int   femaleIndex = 0;//��¼Ҫ���������Ů�����±�
		int    maleIndex = 0;//�������±�

	

		for (vector<male>::iterator mit = malevec.begin(); mit != malevec.end(); ++mit, ++maleIndex)
		{

			curSatisfaction = 0;
			preSatisfaction = 0;
			int i = 0;
			if (mit->id == -2)//�����Ѿ�����ƥ���˵�����
			{
				continue;
			}
			for (vector<female>::iterator fit = femalevec.begin(); fit != femalevec.end(); ++fit,++i)
			{
			
				if (fit->id == -2)
				{
					continue;
				}

				curSatisfaction = mit->expectapperance*fit->apperance +mit->expectcharacter* fit->character + mit->expectwealth*fit->wealth;
				if (curSatisfaction > preSatisfaction)//�Ƚ������
				{
					preSatisfaction = curSatisfaction;
					femaleIndex = i;
				}
				else if (curSatisfaction == preSatisfaction)//�Ƚ�Ů������������
				{
					int curmItself = fit->apperance + fit->character + fit->wealth;
					int premItself = femalevec[femaleIndex].apperance + femalevec[femaleIndex].character + femalevec[femaleIndex].wealth;

					if (curmItself > premItself)
					{
						femaleIndex = i;
					}
					else if (curmItself == premItself)//����������ͬ���Ƚ�id
					{
						if (fit->id < femalevec[femaleIndex].id)
						{
							femaleIndex = i;
						}
					}
				}
			}
			malevec[maleIndex].sendInvite(femalevec, femaleIndex);//�±�ΪmaleIndex���±�ΪfemaleIndex��Ů����������
		}

	}

	//������������Ů��ƥ��ʧ�ܵ���������ƥ��

	void FailMatch(int maxindex)//��������������Ů�����±�
	{
		//�����±��ҵ�Ҫƥ�������������
		vector<int> faileMaleId = femalevec[maxindex].getMaleID();//faileMaleId�д�ŵ�������������Ů����Ӧ��������id
		//�Ѹ�Ů���Ķ�Ӧ������������ȫ����������������һ��ͬ����Ů������ȥ�����������������Ӱ��
		femalevec[maxindex].getMaleID().clear();
		//�����������������ݣ�id��ȾͰѸ��������������ݷ���vector<male> faileMale;
		vector<male> failMale;
		for (vector<int>::iterator it1 = faileMaleId.begin(); it1 != faileMaleId.end(); ++it1)
		{
			int j = 0;
			for (vector<male>::iterator it2 = malevec.begin(); it2 != malevec.end(); ++it2,++j)//����ȫ����������id��ͬ��
			{
				if (*it1 == malevec[j].id)//�ҵ��ͰѸ�������������ӽ�faileMale
				{
					failMale.push_back(malevec[j]);
				}
			}
		}
		
		//�����ʧ�ܵ����������������
		maleSendInvite(failMale);
	}



	/*������������Ů����ʼ�������
	�����ɺ󽫸öԵ�id��Ϊ��һ����ǣ��´�ѡ���޳���һ��
	Ȼ���ѡ���Ů�������������и�δ��Գɹ����������Ž���ƥ�䣬
	�����������ý����ٴ�ѡ����Ϊ��������������û��ѡ���Ů����ѡ������Ů����
	
	������ѡ������������Ů����ʼ��ԡ�
	ֱ���䵽���ǻ���100��ȫ��ƥ��ɹ�����һ�ֽ���
	����֮����������Ҫ���ԭλ��
	*/

	//������������Ů��,���ظ�Ů�����±�
	int maxFemale()
	{
		int curLen = 0;
		int preLen = 0;
		int index = 0;//�������������Ů�����±�
		int i = 0;
		for (vector<female>::iterator it = femalevec.begin(); it != femalevec.end(); ++it,++i)
		{
			curLen = femalevec[i].getMaleID().size();
			if (curLen > preLen)
			{
				preLen = curLen;
				index = i;
			}
			else if (curLen == preLen)//�Ƚ���������
			{
				int curItself = femalevec[i].apperance + femalevec[i].character + femalevec[i].wealth;
				int preItself = femalevec[index].apperance + femalevec[index].character + femalevec[index].wealth;

				if (curItself > preItself)
				{

					index = i;
				}
				else if (curItself == preItself)
				{
					if (femalevec[i].id < femalevec[index].id)
					{
						index = i;
					}
				}
			}
		}
		return index;
	}


	//���е����������е�Ů����ʼ���
	void beginMatch()
	{
		//��ȡ���ǵ�����
		readPlayer();
		int index = 0;
		
		//���ѭ��������ÿ�����ǽ������
		while (index<100)
		{
			/*��������Ů�������ݶ�ȡ�����ڲ�ѭ��������Ϊÿ�ε�ƥ���ı���Ӧ�����ݣ����ÿһ���µ�ѭ����Ҫ�������ݻָ�*/
			readFemale();//��ȡ����������
			readMale();//��ȡŮ��������
			//�ж���������������Ů��������ӽ���Ӧ������
			if (playervec[index].getSex() == 0)//0����Ů��
			{
				femalevec.push_back(playervec[index].makeFemale());
			}
			else //1��������
			{
				malevec.push_back(playervec[index].makeMale());
			}
           
			//��������Ů��������
			maleSendInvite(malevec);
			int count = 0;
			//��ʼƥ��
			while (count<100)
			{
				int maxindex = maxFemale();//�õ�����������Ů�����±�
				int maleindex = femalevec[maxindex].matchMale(malevec);//��Ů����ѡ������

				//�жϸ������Ƿ�Ϊ����
				if (femalevec[maxindex].id == -1||malevec[maleindex].id == -1)//�����ǣ���������ѭ��
				{
					cout << "��" << index + 1 << "��player���룺" << malevec[maleindex].id <<":"<< femalevec[maxindex].id << endl;
					break;
				}
				else//�������ǣ���ʣ�µ��˼����������
				{
					//���Ƚ���Գɹ����޳���ȥ
					malevec[maleindex].id = -2;
					femalevec[maxindex].id = -2;

					FailMatch(maxindex);
				}
				count++;
			}
			if (count == 100)//��ʾ������û����Ե�
			{
				cout << "��" << index + 1 << "��player���룺" << endl;
			}

			index++;//���еڶ���ѭ��
			//��malevec��femalevec��գ��Ա�ָ�ԭʼ����
			malevec.clear();
			femalevec.clear();
		}
	}

private:
	vector<male> malevec;
	vector<female> femalevec;
	vector<player> playervec;

};
class MyTimer
{
public:
	MyTimer()
	{
		QueryPerformanceFrequency(&_freq);
		costTime = 0.0;
	}
	void Start()
	{
		for (int i = 0; i < EN_NUMER; ++i)
		{
			QueryPerformanceCounter(&_array[i]._begin);
		}
	}
	void Stop()
	{
		for (int i = 0; i < EN_NUMER; ++i)
		{
			QueryPerformanceCounter(&_array[i]._end);
		}
	}
	void Reset()
	{
		costTime = 0.0;
	}
	void showTime()
	{
		double allTime = 0.0;
		for (int i = 0; i < EN_NUMER; ++i)
		{
			allTime += (((double)_array[i]._end.QuadPart - (double)_array[i]._begin.QuadPart) / (double)_freq.QuadPart);
		}
		costTime = allTime / EN_NUMER;
		costTime *= 1000000;

		if ((((int)costTime) / 1000000) > 0)
		{
			cout << costTime / 1000000 << " s" << endl;
		}
		else if (((int)costTime) / 1000 > 0)
		{
			cout << costTime / 1000 << " ms" << endl;
		}
		else
		{
			cout << costTime << " us" << endl;
		}
	}
private:
	class Array
	{
	public:
		LARGE_INTEGER _begin;
		LARGE_INTEGER _end;
	};
	enum { EN_NUMER = 5 };
	LARGE_INTEGER _freq;
	double costTime;
	Array _array[EN_NUMER];
};
int main()
{
	MyTimer timer;
	timer.Start();

	match match1;
	match1.beginMatch();

	timer.Stop();
	timer.showTime();
	getchar();
	return 0;


}