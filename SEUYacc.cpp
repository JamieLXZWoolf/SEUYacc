// SEUYacc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Common.h"

using namespace std;

int read_yacc_file(const string& fileName, string& start, unordered_set<string>& terminal, ProducerVecStr& pro, vector<string>& funcVec);

void set_start(string a, ProducerVecStr& pro, vector<string>& startobj);

void get_no_terminal(ProducerVecStr& pro, unordered_set<string>& noter);

void get_first_functions(ProducerVecStr& pro, unordered_set<string>& noter, map<string, unordered_set<string>>& First, unordered_set<string>& terminal);

void build_pro_num(map<vector<string>, int>& Pro_num, ProducerVecStr& pro);

void construct_LR1_sets(const vector<string>& startobj, const unordered_set<string>& terminal, const unordered_set<string>& noter,
	const ProducerVecStr& pro, map<string, unordered_set<string>>& First, DFA& dfa, DFATransitionTable& LRState);

void merge_LR1_sets_into_LALR_sets(DFA& dfa, DFATransitionTable& LRState);

void generate_action_goto_map(const DFA& dfa, const unordered_set<string>& terminal, const unordered_set<string>& noter,
	const DFATransitionTable& LRState, const string start, const unordered_set<string>& Left, map<vector<string>, int>& Pro_num,
	unordered_map<string, int>& Left_level, map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action);


int main()
{
	// ����ս��
	unordered_set<string> terminal;
	
	// ��ŷ��ս��
	unordered_set<string> noter;
	
	// ��������
	vector<string> funcVec;
	
	// ���еĲ���ʽ
	ProducerVecStr pro;
	
	// ����������
	unordered_set<string> Left;

	// ������ȼ�
	unordered_map<string, int> Left_level;

	// first���ϣ�follow����
	map<string, unordered_set<string>> First;

	// ͨ������ʽ�õ���ţ���input�෴
	map<vector<string>, int> Pro_num;
	
	// DFAת����, pair<��Ŀ���, �ս��/���ս��>, �ƽ���Ŀ���, i״̬ʶ��һ��x�󵽴�j״̬.
	DFATransitionTable LRState; 

	// ��Ŀ�� �淶��
	DFA dfa;

	// action��, action[��Ŀ�����][�ս��]<pair �ƽ�/��Լ����Ŀ��/����ʽ���>
	map<int, map<string, pair<string, int>>> action;
	
	// GOTO��goto[��Ŀ�����][���ս��]=��Ŀ�����
	map<int, map<string, int>> GOTO;

	// ��ſ�ʼ����ʽ
	vector<string> startobj;

	// ��ʼ��
	string start;

	read_yacc_file("yacc.y", start, terminal, pro, funcVec);
	set_start(start, pro, startobj);
	get_no_terminal(pro, noter); // ������÷��ս���ļ��ϣ�������ʽ��ߵ��ַ��ļ���
	build_pro_num(Pro_num, pro); // Ϊ�˶���Ĳ���ʽһ�����(����˳��
	get_first_functions(pro, noter, First, terminal); // ����first��
	cout << "Getting LR1..." << endl;
	construct_LR1_sets(startobj, terminal, noter, pro, First, dfa, LRState); // ��������״̬֮���ת����ϵ
	cout << "Converting..." << endl;
	merge_LR1_sets_into_LALR_sets(dfa, LRState);
	generate_action_goto_map(dfa, terminal, noter, LRState, start, Left, Pro_num, Left_level, GOTO, action); // ���������ƽ���Լ��

	return 0;
}