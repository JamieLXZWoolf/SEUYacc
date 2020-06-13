#include "pch.h"
#include "Common.h"

using namespace std;

// �ҵ���������Ҫ��Լ�Ĳ���ʽ�ڲ��ķ��ţ����ڽ���ƽ���Լ��ͻ
int find_dot(int& pos, const DFA& dfa, const unordered_set<int>& Left_num)
{
	for (auto x1 : dfa[pos])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
	{
		auto x = x1.first;
		if (x.first.size() == x.second)
		{
			for (int i = 0; i < x.first.size(); i++)
				if (Left_num.find(x.first[i]) != Left_num.end())
					return x.first[i];
		}	//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���
	}
	return -1;
}

// ���������ƽ���Լ��

void generate_action_goto_map(const DFA& dfa, const unordered_set<string>& terminal, const unordered_set<string>& noter, 
	const DFATransitionTable& LRState, const string & start, const unordered_set<string>& Left, map<vector<int>, int>& r_num,
	unordered_map<string, int>& Left_level, map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action, 
	const unordered_set<int>& terminal_num,unordered_map<int, string> &ns_map, unordered_map<string, int>& sn_map, const unordered_set<int>& Left_num)
{
	for (int i = 0; i < dfa.size(); i++) //��action���goto���ʼ��
	{
		for (auto j : terminal) action[i][j] = make_pair("n", 0);    //iΪ��Ŀ�ţ� jΪ�ַ���  ���Ϊ�ƽ����ǹ�Լ��Ŀ���
		for (auto j : noter)  GOTO[i][j] = -1;   //i��Ŀ�ţ�jΪ�ַ���   -1ΪĿ����Ŀ���
	}
	for (auto i : LRState)  //i��first.first�������Ŀ��ţ�first.second���ַ��ţ�second���յ���
	{
		if (terminal_num.find(i.first.second) != terminal_num.end())   //���ս��
		{
			if (action[i.first.first][ns_map[i.first.second]].first != "n") //��ʾ�ƽ������
			{
				cout << "error\n";
			}
			action[i.first.first][ns_map[i.first.second]] = make_pair("s", i.second);// ��Լ����Ŀ��
		}
		else  //���ս��
		{
			if (GOTO[i.first.first][ns_map[i.first.second]] != -1)  //�����ƽ� ����
			{
				cout << "error\n";
			}
			GOTO[i.first.first][ns_map[i.first.second]] = i.second; // �ƽ�����Ŀ��  
		}
	}

	for (int i = 0; i < dfa.size(); i++)   //�ƽ�
	{
		DFAState x = dfa[i];
		for (auto j1 : x)
		{
			auto j = j1.first;
			if (j.first.size() == j.second)
			{
				if (ns_map[j.first[0]] == "start")// ������ķ���ʼ���ţ�˵���������? ����Ӧ�������㿪ʼ���ţ������޷������ʼ����ʽ��
				{
					if (action[i]["#"].first != "n")
					{
						cout << "error\n";
					}
					action[i]["#"] = make_pair("acc", 0);
					continue;
				}

				int a = j1.second; //LR(1)������ֻ�е��������Ĵ���


				if (action[i][ns_map[a]].first != "n" && Left_level[ns_map[find_dot(i, dfa, Left_num)]] < Left_level[ns_map[a]])   //a��ʾ�У�Ϊ�ƽ��Ķ���
				{
					/*
					if( Left_level[find_dot(i)] > Left_level[a])
						action[i][a] = make_pair("r", rInput[j.first]);//��Լ��Ŀ��
					cout << "have an error\n"; */
				}
				else
					action[i][ns_map[a]] = make_pair("r", r_num[j.first]);//��Լ��Ŀ��
					//�õĲ���ʽ���ͨ��rInput��

				/*
				for(auto a:Follow[j.first[0]]) //SLR(1)�ǣ���Ҫ����һ���ַ�����follow���ϵ���r
				//for (auto a : terminal)//LR(9)������������Լ��Ŀʱ������Ҫ����һ���ַ���ֱ�ӽ��й�Լ
					//���������е��ս������r��
				{

				}*/
			}
		}

	}

	//cout << " ";
	//for (auto i : terminal) cout << i << " ";
	//cout << endl;
	//for (int i = 0; i < dfa.size(); i++)
	//{
	//	cout << i << " ";
	//	for (auto j : terminal)
	//	{
	//		cout << action[i][j].first << action[i][j].second << "  ";
	//	}
	//	cout << endl;
	//}
	//cout << " ";
	//for (auto i : noter) cout << i << " ";
	//cout << endl;
	//for (int i = 0; i < dfa.size(); i++)
	//{
	//	cout << i << " ";
	//	for (auto j : noter)
	//	{
	//		cout << GOTO[i][j] << " ";
	//	}
	//	cout << endl;
	//}
}