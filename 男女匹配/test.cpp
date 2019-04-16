#include<iostream>
#include<vector>
#include <windows.h>
#define  COUNT 100
#pragma warning(disable:4996)
using namespace std;

/*
解题思路：
三个类：male类、female类、player类
male:选择一个满意度最大的女生，发送邀请，sendInvite()
female:1.需要记录有多少个男生邀请自己 vector<int>  maleID;
            2.从邀请的男生中选择与自己最匹配的  matchMale();

player:id设置为-1
匹配规则：1.满意度大的优先
                 2.满意度相同的条件下，判断自身条件，大的优先
				 3.满意度和自身条件都相同的条件下，id小的优先
*/

class person
{
public:
	person(int a, int b, int c, int d, int e, int f, int g) :
	id(a),apperance(b),character(c),
	wealth(d),expectapperance(e),expectcharacter(f),expectwealth(g) {}

	int id;//标识
	int apperance;//样貌
	int character;//品格
	int wealth;//财富
	int expectapperance;//期望样貌
	int expectcharacter;//期望品格
	int expectwealth;
};

class male;

class female:public person //女生类
{
public:
	female(int a, int b, int c, int d, int e, int f, int g): person(a, b, c, d, e, f, g) {}

	void recvInvite(int maleid)//接受邀请
	{
		maleID.push_back(maleid);
	}

	int matchMale(vector<male>& malevec);//在接收的邀请中找匹配的男生
	vector<int>& getMaleID()
	{
		return maleID;
	}

private:
	vector<int> maleID;//记录邀请的男生的id
	friend class male;
};

class male :public person //男生类
{
public:
	male(int a, int b, int c, int d, int e, int f, int g) :person(a, b, c, d, e, f, g) {}

	void sendInvite(vector<female> &femalevec, int index)//发出邀请
	{
		femalevec[index].recvInvite(id);//把自己的id告诉自己满意的女生
	}

};

int female::matchMale(vector<male>& malevec)//在接收的邀请中找匹配的男生
{
	//int len1 = maleID.size();//选择她的男生的个数
	//int len2= malevec.size();//男生的个数

	int curSatisfaction = 0;//当前的满意度：最高的满意度
	int preSatisfaction = 0;//前一次的满意度
	int index = 0;//记录满意度最高的男生的下标
	

	for (vector<int>::iterator it1 = maleID.begin(); it1 != maleID.end(); ++it1)
	{
		int j = 0;
		for (vector<male>::iterator it2 = malevec.begin(); it2 != malevec.end(); ++it2,++j)//遍历所有的男生
		{
			if (malevec[j].id == *it1)//下标为j的男生选择了她
			{
				/*计算满意度*/
				curSatisfaction = expectapperance * malevec[j].apperance + expectcharacter * malevec[j].character + expectwealth * malevec[j].wealth;
				if (curSatisfaction > preSatisfaction)
				{
					preSatisfaction = curSatisfaction;
					index = j;
				}
				else if (curSatisfaction == preSatisfaction)//满意度相等，判断自身条件
				{
					int  curItself = malevec[j].apperance + malevec[j].character + malevec[j].wealth;
					int  preItself = malevec[index].apperance + malevec[index].character + malevec[j].wealth;
					if (curItself > preItself)
					{
						index = j;
					}
					else if (curItself == preItself)//自条件还相同，选取id最小的
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
	return index;//与自己匹配的男生的标号
}



class player:public person//主角类
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
	int sex;//性别
};


class  match
{
public:
	//读取男生的数据进malevec容器
	
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

	//读取女生的数据进fmalevec容器
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

	//读主角的数据进playervec
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

	//所有的男生挑选女生并发送邀请
	void maleSendInvite(vector<male> &malevec)//要传新添加数据之后的数据
	{
		int curSatisfaction = 0;
		int preSatisfaction= 0;

		int   femaleIndex = 0;//记录要发送邀请的女生的下标
		int    maleIndex = 0;//男生的下标

	

		for (vector<male>::iterator mit = malevec.begin(); mit != malevec.end(); ++mit, ++maleIndex)
		{

			curSatisfaction = 0;
			preSatisfaction = 0;
			int i = 0;
			if (mit->id == -2)//跳过已经进行匹配了的数据
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
				if (curSatisfaction > preSatisfaction)//比较满意度
				{
					preSatisfaction = curSatisfaction;
					femaleIndex = i;
				}
				else if (curSatisfaction == preSatisfaction)//比较女生的自身条件
				{
					int curmItself = fit->apperance + fit->character + fit->wealth;
					int premItself = femalevec[femaleIndex].apperance + femalevec[femaleIndex].character + femalevec[femaleIndex].wealth;

					if (curmItself > premItself)
					{
						femaleIndex = i;
					}
					else if (curmItself == premItself)//自身条件相同，比较id
					{
						if (fit->id < femalevec[femaleIndex].id)
						{
							femaleIndex = i;
						}
					}
				}
			}
			malevec[maleIndex].sendInvite(femalevec, femaleIndex);//下标为maleIndex给下标为femaleIndex的女生发送邀请
		}

	}

	//给邀请数最多的女生匹配失败的男生进行匹配

	void FailMatch(int maxindex)//传入邀请数最多的女生的下标
	{
		//根据下标找到要匹配的男生的数据
		vector<int> faileMaleId = femalevec[maxindex].getMaleID();//faileMaleId中存放的是邀请数最大的女生对应的男生的id
		//把该女生的对应的男生的数据全部清楚，否则进行下一轮同样的女生传进去，她的邀请数会产生影响
		femalevec[maxindex].getMaleID().clear();
		//遍历所有男生的数据，id相等就把该男生的所有数据放在vector<male> faileMale;
		vector<male> failMale;
		for (vector<int>::iterator it1 = faileMaleId.begin(); it1 != faileMaleId.end(); ++it1)
		{
			int j = 0;
			for (vector<male>::iterator it2 = malevec.begin(); it2 != malevec.end(); ++it2,++j)//遍历全部男生，找id相同的
			{
				if (*it1 == malevec[j].id)//找到就把该男生的数据添加进faileMale
				{
					failMale.push_back(malevec[j]);
				}
			}
		}
		
		//给配对失败的男生进行重新配对
		maleSendInvite(failMale);
	}



	/*给邀请数最多的女生开始进行配对
	配对完成后将该对的id置为做一个标记，下次选择剔除这一对
	然后从选择该女生的男生队列中给未配对成功的男生接着进行匹配，
	其他男生不用进行再次选择，因为其他男生本来就没有选择该女生，选了其他女生。
	
	这样再选出邀请数最多的女生开始配对。
	直到配到主角或者100对全部匹配成功，第一轮结束
	结束之后所有数据要归回原位。
	*/

	//找邀请数最多的女生,返回该女生的下标
	int maxFemale()
	{
		int curLen = 0;
		int preLen = 0;
		int index = 0;//标记邀请数最大的女生的下标
		int i = 0;
		for (vector<female>::iterator it = femalevec.begin(); it != femalevec.end(); ++it,++i)
		{
			curLen = femalevec[i].getMaleID().size();
			if (curLen > preLen)
			{
				preLen = curLen;
				index = i;
			}
			else if (curLen == preLen)//比较自身条件
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


	//所有的男生和所有的女生开始配对
	void beginMatch()
	{
		//读取主角的数据
		readPlayer();
		int index = 0;
		
		//外层循环用来给每个主角进行配对
		while (index<100)
		{
			/*将男生和女生的数据读取放在内层循环中是因为每次的匹配会改变相应的数据，因此每一轮新的循环就要进行数据恢复*/
			readFemale();//读取男生的数据
			readMale();//读取女生的数据
			//判断主角是男生还是女生，并添加进相应的容器
			if (playervec[index].getSex() == 0)//0代表女生
			{
				femalevec.push_back(playervec[index].makeFemale());
			}
			else //1代表男生
			{
				malevec.push_back(playervec[index].makeMale());
			}
           
			//所有男向女发送邀请
			maleSendInvite(malevec);
			int count = 0;
			//开始匹配
			while (count<100)
			{
				int maxindex = maxFemale();//得到邀请数最大的女生的下标
				int maleindex = femalevec[maxindex].matchMale(malevec);//该女生挑选的男生

				//判断该男生是否为主角
				if (femalevec[maxindex].id == -1||malevec[maleindex].id == -1)//是主角，结束本次循环
				{
					cout << "第" << index + 1 << "组player加入：" << malevec[maleindex].id <<":"<< femalevec[maxindex].id << endl;
					break;
				}
				else//不是主角，给剩下的人继续进行配对
				{
					//首先将配对成功的剔除出去
					malevec[maleindex].id = -2;
					femalevec[maxindex].id = -2;

					FailMatch(maxindex);
				}
				count++;
			}
			if (count == 100)//表示该主角没有配对到
			{
				cout << "第" << index + 1 << "组player加入：" << endl;
			}

			index++;//进行第二轮循环
			//将malevec和femalevec清空，以便恢复原始数据
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