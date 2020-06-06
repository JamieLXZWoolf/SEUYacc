#include "pch.h"
#include "Common.h"

using namespace std;

// �ҵ���������Ҫ��Լ�Ĳ���ʽ�ڲ��ķ��ţ����ڽ���ƽ���Լ��ͻ
string find_dot(int pos, const DFA& dfa, const unordered_set<string>& Left)
{
	for (auto x1 : dfa[pos])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
	{
		auto x = x1.first;
		if (x.first.size() == x.second)
		{
			for (int i = 0; i < x.first.size(); i++)
				if (Left.find(x.first[i]) != Left.end())
					return x.first[i];
		}	//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���
	}
	return "û�ҵ�\n";
}

// ���������ƽ���Լ��
void generate_action_goto_map(const DFA& dfa, const unordered_set<string>& terminal, const unordered_set<string>& noter, 
	const DFATransitionTable& LRState, const string start, const unordered_set<string>& Left, map<vector<string>, int>& Pro_num,
	unordered_map<string, int>& Left_level, map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action)
{
	for (int i = 0; i < dfa.size(); i++) //��action���goto���ʼ��
	{
		for (auto j : terminal) action[i][j] = make_pair("n", 0);    //iΪ��Ŀ�ţ� jΪ�ַ���  ���Ϊ�ƽ����ǹ�Լ��Ŀ���
		for (auto j : noter)  GOTO[i][j] = -1;   //i��Ŀ�ţ�jΪ�ַ���   -1ΪĿ����Ŀ���
	}
	for (auto i : LRState)  //i��first.first�������Ŀ��ţ�first.second���ַ��ţ�second���յ���
	{
		if (terminal.find(i.first.second) != terminal.end())   //���ս��
		{
			if (action[i.first.first][i.first.second].first != "n") //��ʾ�ƽ������
			{
				cout << "error\n";
			}
			action[i.first.first][i.first.second] = make_pair("s", i.second);// ��Լ����Ŀ��
		}
		else  //���ս��
		{
			if (GOTO[i.first.first][i.first.second] != -1)  //�����ƽ� ����
			{
				cout << "error\n";
			}
			GOTO[i.first.first][i.first.second] = i.second; // �ƽ�����Ŀ��  
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
				if (j.first[0] == start)//������ķ���ʼ���ţ�˵���������
				{
					if (action[i]["#"].first != "n")
					{
						cout << "error\n";
					}
					action[i]["#"] = make_pair("acc", 0);
					continue;
				}

				string a = j1.second; //LR(1)������ֻ�е��������Ĵ���


				if (action[i][a].first != "n" && Left_level[find_dot(i, dfa, Left)] < Left_level[a])   //a��ʾ�У�Ϊ�ƽ��Ķ���
				{
					/*
					if( Left_level[find_dot(i)] > Left_level[a])
						action[i][a] = make_pair("r", rInput[j.first]);//��Լ��Ŀ��
					cout << "have an error\n"; */
				}
				else
					action[i][a] = make_pair("r", Pro_num[j.first]);//��Լ��Ŀ��
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

	cout << " ";
	for (auto i : terminal) cout << i << " ";
	cout << endl;
	for (int i = 0; i < dfa.size(); i++)
	{
		cout << i << " ";
		for (auto j : terminal)
		{
			cout << action[i][j].first << action[i][j].second << "  ";
		}
		cout << endl;
	}
	cout << " ";
	for (auto i : noter) cout << i << " ";
	cout << endl;
	for (int i = 0; i < dfa.size(); i++)
	{
		cout << i << " ";
		for (auto j : noter)
		{
			cout << GOTO[i][j] << " ";
		}
		cout << endl;
	}
}