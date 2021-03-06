#include "pch.h"
#include "Common.h"

using namespace std;


extern map<vector<int>, int> r_num;

// 找到集簇中需要规约的产生式内部的符号，便于解决移进规约冲突
int find_dot(int& pos, const DFA& dfa, const unordered_set<int>& Left_num)
{
	for (auto x1 : dfa[pos])    //x是一个OBJ  first是产生式  second是·的位置
	{
		auto x = x1.first;
		if (x.first.size() == x.second)
		{
			for (int i = 0; i < x.first.size(); i++)
				if (Left_num.find(x.first[i]) != Left_num.end())
					return x.first[i];
		}	//如果已经到了最后一个位置了，说明是规约项目，不用后移
	}
	return -1;
}


//找到集簇中，预测符是a的，要规约的产生式中，最早出现的产生式序号
int find_pro(int& pos, const DFA& dfa, int a) {
	int max = -1;
	for (auto x1 : dfa[pos])    //x是一个OBJ  first是产生式  second是·的位置
	{
		auto x = x1.second;
		auto x2 = x1.first;
		//是需要规约的产生式，并且后面的预测符是边上的
		if (x == a && x2.first.size() == x2.second)  
		{
			if (r_num[x1.first.first] > max)
				max = r_num[x1.first.first];
		}
	}

	return max;
}


// 用来生成移进规约表

void generate_action_goto_map(const DFA& dfa, const unordered_set<string>& terminal, const unordered_set<string>& noter, 
	DFATransitionTable& LRState, const string & start, const unordered_set<string>& Left, map<vector<int>, int>& r_num,
	unordered_map<string, int>& Left_level, map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action, 
	const unordered_set<int>& terminal_num,unordered_map<int, string> &ns_map, unordered_map<string, int>& sn_map, const unordered_set<int>& Left_num)
{
	for (int i = 0; i < dfa.size(); i++) //将action表和goto表初始化
	{
		for (auto j : terminal) action[i][j] = make_pair("n", 0);    //i为项目号， j为字符号  结果为移进还是规约，目标号
		for (auto j : noter)  GOTO[i][j] = -1;   //i项目号，j为字符号   -1为目标项目编号
	}

		//for (int i = 0; i < dfa.size(); i++) {
		//	auto s = dfa[i];
		//	for (auto lritm : s) {
		//		// i 为一个LR项目
		//		if (lritm.first.second == lritm.first.first.size()) {
		//			cout << "REDUCE" << endl;
		//			if (ns_map[lritm.first.first[0]] == "start" && ns_map[lritm.second] == "#") {
		//				action[i][ns_map[lritm.second]] = make_pair("acc", 0);
		//			}
		//			else {
		//				action[i][ns_map[lritm.second]] = make_pair("r", r_num[lritm.first.first]);
		//				GOTO[i][ns_map[lritm.first.first[0]]] = LRState[{i, lritm.first.first[0]}];
		//			}
		//		}
		//		else {
		//			int a = lritm.first.first[lritm.first.second];
		//			int b = lritm.second;
		//			if (terminal_num.find(a) != terminal_num.end()) {
		//				// terminal
		//				if (LRState.find({ i, a }) != LRState.end()) {
		//					int to = LRState[{i, a}];
		//					action[i][ns_map[a]] = make_pair("s", to);
		//				}
		//			}
		//		}
		//	}
		//}

	for (auto i : LRState)  //i的first.first是起点项目编号，first.second是字符号，second是终点编号
	{
		if (terminal_num.find(i.first.second) != terminal_num.end())   //是终结符
		{
			if (action[i.first.first][ns_map[i.first.second]].first != "n") //表示移进则出错
			{
				cout << "error\n";
			}
			action[i.first.first][ns_map[i.first.second]] = make_pair("s", i.second);// 规约成项目号
		}
		else  //非终结符
		{
			if (GOTO[i.first.first][ns_map[i.first.second]] != -1)  //不能移进 出错
			{
				cout << "error\n";
			}
			GOTO[i.first.first][ns_map[i.first.second]] = i.second; // 移进到项目号  
		}
	}

	for (int i = 0; i < dfa.size(); i++)   //移进
	{
		DFAState x = dfa[i];
		for (auto j1 : x)
		{
			auto j = j1.first;
			if (j.first.size() == j.second)
			{
				if (ns_map[j.first[0]] == "start")// 如果是文法开始符号，说明分析完成? 错误，应该是增广开始符号！否则无法输出开始产生式！
				{
					if (action[i]["#"].first != "n")
					{
						cout << "error\n";
					}
					action[i]["#"] = make_pair("acc", 0);
					continue;
				}

				int a = j1.second; //LR(1)分析，只有当接下来的词是


				if (action[i][ns_map[a]].first != "n" && (((action[i][ns_map[a]].first == "s"))))
				{ 
					cout << "RR!" << endl;
					cout << action[i][ns_map[a]].first << action[i][ns_map[a]].second << endl;
					continue;
					/*
					if( Left_level[find_dot(i)] > Left_level[a])
						action[i][a] = make_pair("r", rInput[j.first]);//规约项目，
					cout << "have an error\n"; */
				}
				else
					action[i][ns_map[a]] = make_pair("r", r_num[j.first]);//规约项目，
					//用的产生式编号通过rInput找
			}
		}

	}

}