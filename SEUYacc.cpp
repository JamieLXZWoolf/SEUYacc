// SEUYacc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Common.h"

using namespace std;

// ����ս��
unordered_set<string> terminal;

// ��ŷ��ս��
unordered_set<string> noter;

// ���еĲ���ʽ
ProducerVecStr pro;

// ����������
unordered_set<string> Left;

// ������ȼ�
unordered_map<string, int> Left_level;

// first���ϣ�follow����
map<int, unordered_set<int>> First;

// ͨ������ʽ�õ���ţ���input�෴

// DFAת����, pair<��Ŀ���, �ս��/���ս��>, �ƽ���Ŀ���, i״̬ʶ��һ��x�󵽴�j״̬.
DFATransitionTable LRState;

// ��Ŀ�� �淶��
DFA dfa;

// action��, action[��Ŀ�����][�ս��]<pair �ƽ�/��Լ����Ŀ��/����ʽ���>
map<int, map<string, pair<string, int>>> action;

// GOTO��goto[��Ŀ�����][���ս��]=��Ŀ�����
map<int, map<string, int>> GOTO;

// ��ſ�ʼ����ʽ
vector<int> startobj;

// ��ʼ��
string start, definition, code;

//����Ϊ�ṩ����ӳ�������ӵĽṹ
unordered_map<string, int> sn_map;
unordered_map<int, string> ns_map;
unordered_set<int> terminal_num;
unordered_set<int> noter_num;
unordered_set<int> Left_num;
ProducerVec pro_num;
map < vector<int>, int > r_num;
map<int, vector<int>> wp_map;

int  divide;


int read_yacc_file(const string& fileName, string& start, unordered_set<string>& terminal, ProducerVecStr& pro, string& definition, string& code, unordered_set<string>& Left, unordered_map<string, int>& Left_level);

void set_start(string& a, ProducerVecStr& pro, vector<int>& startobj);

void add_start(string& a, vector<int>& startobj, unordered_map<string, int>& sn_map);

void get_no_terminal(ProducerVecStr& pro, unordered_set<string>& noter);

void get_terminal(ProducerVecStr& pro, unordered_set<string>& noter, unordered_set<string>& terminal);

void build_pro_num(unordered_set<string>& terminal, unordered_map<string, int>&  sn_map, unordered_map<int, string> & ns_map, unordered_set<int> & terminal_num, int & divide,
	unordered_set<string> &  noter, unordered_set<string>&  Left, unordered_set<int> & noter_num, unordered_set<int>&  Left_num, ProducerVecStr & pro, ProducerVec & pro_num,
	map<vector<int>, int>&  r_num, map<int, vector<int>> & wp_map);


void merge_LR1_sets_into_LALR_sets(DFA& dfa, DFATransitionTable& LRState);

void generate_action_goto_map(const DFA& dfa, const unordered_set<string>& terminal, const unordered_set<string>& noter,
	const DFATransitionTable& LRState, const string &start, const unordered_set<string>& Left, map<vector<int>, int>& Pro_num,
	unordered_map<string, int>& Left_level, map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action,
	const unordered_set<int>& terminal_num, unordered_map<int, string> &ns_map, unordered_map<string, int>& sn_map, const unordered_set<int>& Left_num);

void write_parser_code(map<int, map<string, int>>& GOTO, map<int, map<string, pair<string, int>>>& action,
	const unordered_set<string>& terminal, const unordered_set<string>& noter, const ProducerVecStr pro, const string& definition, const string& code);

void construct_LR1_sets();   //�������ɼ���֮���ת����ϵ

void first_operation();


int main(int argc, char const* argv[])
{
	string file_name;

	if (argc != 2) {
		cout << "ERROR: invalid arguments number!\n";
		return -1;
	}
	else
	{
		file_name = string(argv[1]);
	}

	

	read_yacc_file(file_name, start, terminal, pro, definition, code, Left, Left_level);
	cout << "START:" << start << endl;
	set_start(start, pro, startobj);

	get_no_terminal(pro, noter); // ������÷��ս���ļ��ϣ�������ʽ��ߵ��ַ��ļ���
	get_terminal(pro, noter, terminal);//���Ƿ��ս�����ַ������ս��
	build_pro_num( terminal, sn_map,  ns_map,  terminal_num, divide,noter,  Left,  noter_num,  Left_num, pro, pro_num,r_num,wp_map); // Ϊ�˶���Ĳ���ʽһ�����(����˳��

	add_start(start,  startobj, sn_map);

	first_operation();

	
	cout << "һ��" << endl;
	for (auto i : First)
	{
		int ch = i.first;
			cout << "���Ϊ" << ns_map[ch] << "->";
			for (auto j : First[ch])
			{
				cout << ns_map[j] << " ";
			}
			cout << endl;
	}

	cout << "Getting LR1..." << endl;


	construct_LR1_sets(); // ��������״̬֮���ת����ϵ
	cout << "Converting..." << endl;
	merge_LR1_sets_into_LALR_sets(dfa, LRState);
	generate_action_goto_map(dfa, terminal, noter, LRState, start, Left, r_num, Left_level, GOTO, action,
		 terminal_num,  ns_map, sn_map,  Left_num); // ���������ƽ���Լ��
	write_parser_code(GOTO, action, terminal, noter, pro, definition, code);

	return 0;
}