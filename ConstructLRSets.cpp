#include "pch.h"
#include "Common.h"

using namespace std;

// ���忪ʼ�����ɸ��ݲ�ͬ��.y�ļ�����
void set_start(string a, ProducerVecStr& pro, vector<string>& startobj)
{
	pair<string, vector<string> > start2;

	start2.first = "start";
	start2.second.push_back(a);    //�Ҳ�ļ���
	pro.insert(pro.begin(), start2);

	string temp2 = "start";  //�������ڵ�һ������չ�Ĳ���ʽ
	startobj.push_back(temp2);
	temp2 = a;
	startobj.push_back(temp2);
}

// ������÷��ս���ļ��ϣ�������ʽ��ߵ��ַ��ļ���
void get_no_terminal(ProducerVecStr& pro, unordered_set<string>& noter)
{
	string s;
	for (int i = 0; i < pro.size(); i++)
	{
		noter.insert(pro[i].first);
	}
}

// ��temp_first�������ʽΪs�ַ�����first��֮�����s��firstȥtemp_first����
void build_temp_first(unordered_set<string>& temp_first, map<string, unordered_set<string>>& First, vector<string> s)
{
	temp_first.clear();//��ʱʹ�ã�ÿ�����
	bool flag = 1; //��ǿ�Ԫ��
	for (int i = 0; i < s.size(); i++)
	{
		flag = 0;
		for (auto j : First[s[i]])
		{
			if (j != kong) temp_first.insert(j);
			else flag = 1;
		}
		if (flag == 0) break;
	}
	if (flag == 1)  temp_first.insert(kong);
}

// ����first��
void get_first_functions(ProducerVecStr& pro, unordered_set<string>& noter, map<string, unordered_set<string>>& First, unordered_set<string>& terminal)
{
	bool flag = 1;//�Ƿ�ı�
	while (flag)
	{
		flag = 0;
		for (auto no : noter)
		{
			for (int i = 0; i < pro.size(); i++)
			{
				if (pro[i].first == no)
				{
					vector<string> j = pro[i].second;
					bool flag = 0; //��ǿ�Ԫ��
					for (auto ch : j)
					{
						if (noter.find(ch) == noter.end()) // A terminal
						{
							if (First[no].insert(ch).second) flag = 1; // An insertion is made
							flag = 0; // ?
						}
						else // Is a nonterminal
						{
							for (auto k : First[ch])
							{
								if (k != kong)
								{
									if (First[no].insert(k).second) flag = 1;
									flag = 0;
								}
								else flag = 1;
							}

						}
						if (flag == 0) break;

					}
					if (flag == 1 && First[no].insert(kong).second) flag = 1;
				}
			}
			for (auto i : terminal) First[i].insert(i);
		}

	}

	/*����Ϊ��ӡ���Բ���
	cout << "һ��" << endl;
		for (auto i : First)
		{
			string ch = i.first;
			if (noter.find(ch) != noter.end())
			{
			cout << "���Ϊ" << ch << "->";

			for (auto j : First[ch])
			{
					cout << j << " ";
			}
			cout << endl;
			}
		}
		*/
}

// Ϊ�˶���Ĳ���ʽһ����ţ�����֮�����ɹ�Լ����ž������(����˳��
void build_pro_num(map<vector<string>, int>& Pro_num, ProducerVecStr& pro)
{
	for (int i = 0; i < pro.size(); i++)
	{
		vector<string> temp = pro[i].second;   //����ʽ����
		temp.insert(temp.begin(), pro[i].first);
		Pro_num.insert(make_pair(temp, i));

		cout << "���: " << i << "  ";
		cout << pro[i].first << "->";
		for (int j = 0; j < pro[i].second.size(); j++)
			cout << pro[i].second[j] << " ";
		cout << endl;
	}
}

// ��Ŀ�հ����ڲ���չ
DFAState inter_extend(LRitem x, const unordered_set<string>& noter, const ProducerVecStr& pro, map<string, unordered_set<string>>& First, const unordered_set<string>& terminal)
{
	DFAState extended;  //��Ҫ���صıհ�
	extended.insert(x);
	bool f = 1;
	unordered_set<string> temp_first;

	while (f)  // ֱ�����ϲ�������Ϊֹ
	{
		f = 0;  //�����������f=1
		for (auto lritem1 : extended)
		{
			auto item1 = lritem1.first;          //x������һ����Ŀ
			if (item1.second >= item1.first.size())  //���λ�õ��ڲ���ʽ�ķ�������  
			{
				f = 0;   //�ɹ���Լ��û������
				/*
				cout << "kai" << endl;
				for(int w=0;w<x.first.size();w++)
				cout << x.first[w] << endl;
				cout << x.second << endl;
				cout << "guan" << endl;
				*/
				break;
			}


			string s = item1.first[item1.second];    //�õ��˵����ķ���

			if (noter.find(s) != noter.end())    //����������Ƿ��ս��
			{

				for (int i = 0; i < pro.size(); i++)    //�������еĲ���ʽ
				{

					if (pro[i].first == s)    //�ҳ�������ս���Ĳ���ʽ
					{
						vector<string> temp = pro[i].second;   //����ʽ����
						temp.insert(temp.begin(), s);
						//������ʽ�󲿷��ŷ��뿪ͷ

						vector<string> xx;
						//����Ŀ[A->x��by,a]���ڱհ�ans,B->R�ǲ���ʽ
						//��first��Ra) �е�ÿ���ַ���b����[B->r��b]����ans�У�����
						xx.assign(item1.first.begin() + item1.second + 1, item1.first.end());
						xx.push_back(lritem1.second);
						build_temp_first(temp_first, First, xx);
						for (auto b : temp_first)//getfirst ����õ��Ľ����se��  
							if (extended.insert(make_pair(make_pair(temp, 1), b)).second)  //�������ɹ�
							{
								f = 1;   //�������ɹ���f=1������������
							}

						// temp.insert(temp.begin(), s);
					}
				}
			}
			else if (terminal.find(s) == terminal.end()) cout << s << endl;
		}
	}

	//for (auto LRitem2 : extended)   //jΪһ��item
	//{
	//	auto item2 = LRitem2.first;
	//	vector<string> temp = item2.first;
	//	cout << temp[0] << "->";
	//	for (int k = 1; k < temp.size(); k++)
	//	{
	//		if (k == item2.second) cout << "�� ";
	//		cout << temp[k] << " ";
	//	}
	//	if (item2.second == temp.size()) cout << "�� ";
	//	cout << "," << LRitem2.second << endl;
	//}
	//cout << endl;

	return extended;

}

// ����ÿһ����Ŀ����ÿһ���ķ����ţ����Ƿ���ܵõ�һ������Ŀ, ������Ծͼ��뵽�����С�pos����Ŀ��ţ� xxΪ��Ҫ���Ե��ں�����ӵ��ս��/���ս��
void outer_extend(int pos, string xx, DFA& DFA, DFATransitionTable& LRState, const unordered_set<string>& noter, const ProducerVecStr& pro, map<string, unordered_set<string>>& First, const unordered_set<string>& terminal)
{
	DFAState ans;
	for (auto state1 : DFA[pos])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
	{
		auto LRitem1 = state1.first;
		if (LRitem1.first.size() == LRitem1.second) continue;
		//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���

		if (LRitem1.first[LRitem1.second] == xx)
		{
			DFAState temp = inter_extend(make_pair(make_pair(LRitem1.first, LRitem1.second + 1), state1.second), noter, pro, First, terminal);//���������ƶ�һλ�ıհ�
			ans.insert(temp.begin(), temp.end());//set�����Զ�ȥ��
		}
	}
	if (ans.empty()) return;  //�õ����ǿյģ���ôֱ�ӷ���

	int loc = find(DFA.begin(), DFA.end(), ans) - DFA.begin(); //find�����������е�λ�ã���ȥ��ʼλ�õõ����
	if (loc != DFA.size())
	{
		LRState[make_pair(pos, xx)] = loc;   //�ҵ����Ѿ����ڵ���Ŀ����ô�ƽ�
	}
	else
	{
		DFA.push_back(ans);     //�ҵ��Ĳ����Ѿ����ڵ���Ŀ����ô����DFA������
		LRState[make_pair(pos, xx)] = DFA.size() - 1;        //�ƽ����µļ�����
	}

}

// �������ɼ���֮���ת����ϵ
void construct_LR1_sets(const vector<string> &startobj, const unordered_set<string>& terminal, const unordered_set<string>& noter, 
	const ProducerVecStr& pro, map<string, unordered_set<string>>& First, DFA& dfa, DFATransitionTable& LRState)
{
	//vector<string> temp3;
	//temp3.push_back("#");
	DFAState statu = inter_extend(make_pair(make_pair(startobj, 1), "#"), noter, pro, First, terminal); //��ʼ��Ŀ�ıհ�����Ϊһ����Ŀ����
	dfa.push_back(statu);  // ����DFA�У��õ�һ�����Ϊ0����Ϊλ��Ϊ0��

	for (int i = 0; i < dfa.size(); i++)  //����ÿһ����ֱ��DFA��������Ϊֹ  push back ���Զ������������һ��
	{
		for (auto t : terminal) outer_extend(i, t, dfa, LRState, noter, pro, First, terminal);
		for (auto n : noter) outer_extend(i, n, dfa, LRState, noter, pro, First, terminal);   //��̽���еķ���
		cout << dfa.size() << endl;
	}

	//����Ϊ��ӡ���ز���
	//for (int i = 0; i < dfa.size(); i++)
	//{
	//	cout << i << endl;
	//	for (auto state1 : dfa[i])   //jΪһ��obj
	//	{
	//		auto LRitem1 = state1.first;
	//		vector<string> temp = LRitem1.first;
	//		cout << temp[0] << "->";
	//		for (int k = 1; k < temp.size(); k++)
	//		{
	//			if (k == LRitem1.second) cout << "�� ";
	//			cout << temp[k] << " ";
	//		}
	//		if (LRitem1.second == temp.size()) cout << "�� ";
	//		cout << "," << state1.second << endl;
	//	}
	//	cout << endl;
	//}

	//����Ϊ��ӡ�ƽ�����     
	//for (auto i : LRState)
	//{
	//	cout << i.first.first << "����" << i.first.second << "�ƽ���" << i.second << endl;
	//}

}


StateCore get_core(const DFAState& LRset) {
	StateCore rst;
	for (auto i : LRset) {
		rst.insert(i.first);
	}
	return rst;
}

// �ϲ�ͬ����
void merge_LR1_sets_into_LALR_sets(DFA& dfa, DFATransitionTable& LRState) {
	DFA newDFA;
	DFATransitionTable newLRState;

	map<StateCore, set<int>> core2StatesNum;
	vector<StateCore> ordered;

	for (int i = 0; i < dfa.size(); i++) {
		auto state = dfa[i];
		auto core = get_core(state);
		if (core2StatesNum.find(core) == core2StatesNum.end()) { // �½�
			set<int> newSet = { i };
			core2StatesNum[core] = newSet;
			ordered.push_back(core);
		}
		else {
			core2StatesNum[core].insert(i);
		}
	}

	map<int, set<int>> newDFA2OldDFA; // һ�Զ�
	map<int, int> oldDFA2NewDFA; // ���һ

	int newIdx = 0;

	// �ϲ������ݲ�����ת�����ȼ�¼�¾�״̬��ӳ���ϵ
	for (auto c : ordered) {
		DFAState newState;
		set<int> sns = core2StatesNum[c];
		newDFA2OldDFA[newIdx] = sns;
		for (auto i : sns) {
			oldDFA2NewDFA[i] = newIdx;
			DFAState oldState = dfa[i];
			newState.insert(oldState.begin(), oldState.end());
		}
		newDFA.push_back(newState);
		newIdx++;
	}

	for (auto m : LRState) {
		auto oldFrom = m.first;
		auto oldTo = m.second;
		
		auto newFrom = make_pair(oldDFA2NewDFA[oldFrom.first], oldFrom.second);
		auto newTo = oldDFA2NewDFA[oldTo];
		newLRState.insert({ newFrom, newTo });
	}
	dfa = newDFA;
	LRState = newLRState;
}