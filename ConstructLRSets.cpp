#include "pch.h"
#include "Common.h"

using namespace std;

// ���忪ʼ�����ɸ��ݲ�ͬ��.y�ļ�����
void set_start(string& a, ProducerVecStr& pro, vector<int>& startobj)
{
	pair<string, vector<string> > start2;

	start2.first = "start";
	start2.second.push_back(a);    //�Ҳ�ļ���
	pro.insert(pro.begin(), start2);

}

void add_start(string& a, vector<int>& startobj, unordered_map<string, int> &sn_map)
{

	string temp2 = "start";  //�������ڵ�һ������չ�Ĳ���ʽ
	startobj.push_back(sn_map[temp2]);
	temp2 = a;
	startobj.push_back(sn_map[temp2]);

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


//���Ƿ��ս�����ַ������ս��
void get_terminal(ProducerVecStr& pro, unordered_set<string>& noter, unordered_set<string>& terminal)
{

	for (int i = 0; i < pro.size(); i++)
	{

		for (int j = 0; j < pro[i].second.size(); j++)
			if (noter.find(pro[i].second[j]) == noter.end())
				terminal.insert(pro[i].second[j]);
	}
}




// Ϊ�˶���Ĳ���ʽһ����ţ�����֮�����ɹ�Լ����ž������(����˳��
void build_pro_num(unordered_set<string> & terminal, unordered_map<string, int>&  sn_map, unordered_map<int, string> & ns_map, unordered_set<int> & terminal_num, int & divide,
	unordered_set<string> &  noter, unordered_set<string>&  Left, unordered_set<int>&  noter_num, unordered_set<int>&  Left_num, ProducerVecStr & pro, ProducerVec  & pro_num,
	map<vector<int>, int> & r_num ,map<int, vector<int>> & wp_map) //Ϊ�˶���Ĳ���ʽһ����ţ�����֮�����ɹ�Լ����ž������(����˳��
{
	int count = 0;

	for (auto st : terminal)
	{
		sn_map.insert(make_pair(st, count));
		ns_map.insert(make_pair(count, st));
		terminal_num.insert(sn_map[st]);
		count++;
	}

	divide = count;

	cout << "divide: --------------  " << divide << endl;

	for (auto st : noter)
	{
		sn_map.insert(make_pair(st, count));
		ns_map.insert(make_pair(count, st));
		noter_num.insert(sn_map[st]);
		count++;

	}

	for (auto st : Left)
	{
		Left_num.insert(sn_map[st]);
	}

	for (auto st : sn_map)
	{
		cout << st.first << " -+-" << st.second << endl;
	}




	//����pro_num

	for (auto temp_pro : pro)
	{
		vector<int> temp_vect;
		temp_vect.clear();
		for (auto temp_pair : temp_pro.second)
		{
			temp_vect.push_back(sn_map[temp_pair]);

		}
		pro_num.push_back(make_pair(sn_map[temp_pro.first], temp_vect));
	}


	for (int i = 0; i < pro_num.size(); i++)
	{
		vector<int> temp = pro_num[i].second;   //����ʽ����
		temp.insert(temp.begin(), pro_num[i].first);
		r_num.insert(make_pair(temp, i));

		cout << "���: " << i << "  ";
		cout << ns_map[pro_num[i].first] << "->";
		for (int j = 0; j < pro_num[i].second.size(); j++)
			cout << ns_map[pro_num[i].second[j]] << " ";
		cout << endl;
	}


	for (auto s : noter_num)
	{

		for (int i = 0; i < pro_num.size(); i++)    //�������еĲ���ʽ
		{
			if (pro_num[i].first == s)    //�ҳ�������ս���Ĳ���ʽ
			{
				wp_map[s].push_back(i);
			}
		}

	}





}


// ��temp_first�������ʽΪs�ַ�����first��֮�����s��firstȥtemp_first����
void build_temp_first(unordered_set<int>& temp_first, map<int, unordered_set<int>>& First, vector<int>&  s, unordered_map<string, int> & sn_map)
{
	temp_first.clear();//��ʱʹ�ã�ÿ�����
	bool flag = 1; //��ǿ�Ԫ��
	for (int i = 0; i < s.size(); i++)
	{
		flag = 0;
		for (auto j : First[s[i]])
		{
			if (j != sn_map[kong]) temp_first.insert(j);
			else flag = 1;
		}
		if (flag == 0) break;
	}
	if (flag == 1)  temp_first.insert(sn_map[kong]);
}

// ����first��
void get_first_functions(ProducerVec& pro_num, unordered_set<int>& noter_num, map<int, unordered_set<int>>& First, 
	unordered_set<int>& terminal_num, unordered_map<string, int>&  sn_map)
{
	bool flag = 1;//�Ƿ�ı�
	while (flag)
	{
		flag = 0;
		for (auto no : noter_num)
		{
			for (int i = 0; i < pro_num.size(); i++)
			{
				if (pro_num[i].first == no)
				{
					vector<int> j = pro_num[i].second;
					bool flag = 0; //��ǿ�Ԫ��
					for (auto ch : j)
					{
						if (noter_num.find(ch) == noter_num.end()) // A terminal
						{
							if (First[no].insert(ch).second) flag = 1; // An insertion is made
							flag = 0; // ?
						}
						else // Is a nonterminal
						{
							for (auto k : First[ch])
							{
								if (k != sn_map[kong])
								{
									if (First[no].insert(k).second) flag = 1;
									flag = 0;
								}
								else flag = 1;
							}

						}
						if (flag == 0) break;

					}
					if (flag == 1 && First[no].insert(sn_map[kong]).second) flag = 1;
				}
			}
			for (auto i : terminal_num) First[i].insert(i);
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

//��Ŀ�հ����ڲ���չ
unordered_set<LRitem> inter_extend(unordered_set <LRitem> & x,int &divide,
	map<int, vector<int>>&  wp_map, ProducerVec & pro_num,
	unordered_map<string, int>&  sn_map, map<int, unordered_set<int>>& First, unordered_map<int, string> & ns_map)
{
	unordered_set<LRitem> extended;
	queue<LRitem> queue;
	unordered_set<int> temp_first;


	for (auto temp : x) {//����ʽ�ȶ���ջ

		queue.push(temp);
		extended.insert(temp);
	}

	Item item1;
	LRitem lritem1;

	while (!queue.empty())  // ֱ�����ϲ�������Ϊֹ
	{

		
		lritem1 = queue.front();
		item1 = queue.front().first;
		if (item1.second >= item1.first.size())
		{//����ĩβ��������һ������ʽ
			queue.pop();

			continue;
		}

		else {

			int s = item1.first[item1.second];    //�õ��˵����ķ���





			if (s < divide)//���ս����û�бհ���������һ��
			{
				queue.pop();
				//cout << "gan��  " << ns_map[s] << " ";
				continue;
			}


			queue.pop();
			//cout << "gan��  " << ns_map[s] << " ";





			for (auto i : wp_map[s])    //�ҳ�������ս���Ĳ���ʽ
			{
			

				vector<int> temp = pro_num[i].second;   //����ʽ����


				temp.insert(temp.begin(), s);


				vector<int> xx;
				//����Ŀ[A->x��by,a]���ڱհ�ans,B->R�ǲ���ʽ
				//��first��Ra) �е�ÿ���ַ���b����[B->r��b]����ans�У�����
				xx.assign(item1.first.begin() + item1.second + 1, item1.first.end());


				build_temp_first(temp_first, First, xx, sn_map);
			

				if (temp_first.find(sn_map[kong]) != temp_first.end()) {//��epsilon
					temp_first.erase(sn_map[kong]);
					temp_first.insert(lritem1.second);
				}

				LRitem TEMP;



				for (auto b : temp_first)//getfirst ����õ��Ľ����se��  
				{

					TEMP.second = b;
					TEMP.first.first = temp;
					TEMP.first.second = 1;



					auto findIt = extended.find(TEMP);
					if (findIt != extended.end()) //����һ�����ˣ� �Ͳ��ٴ���
						continue;

				

					queue.push(TEMP);
					extended.insert(TEMP);
				}
				//cout << "һ�ν���" << endl;

				/*
					if (queue.push(make_pair(make_pair(temp, 1), b)).second)  //�������ɹ�
					{
						f = 1;   //�������ɹ���f=1������������
					}
				*/
				// temp.insert(temp.begin(), s);
			}



		}
	}

	//cout <<"-";

	/*
	cout << "��" << endl;
	for (auto LRitem2 : extended)   //jΪһ��item
	{
		auto item2 = LRitem2.first;
		vector<int> temp = item2.first;

		cout << ns_map[temp[0]]<< "->";

		for (int k = 1; k < temp.size(); k++)
		{
			if (k == item2.second) cout << "�� ";
			cout << ns_map[temp[k]] << " ";
		}
		if (item2.second == temp.size()) cout << "�� ";
		cout << "," << ns_map[LRitem2.second ]<< endl;
		

	}
	cout << endl;


	if (extended.size() > 1000)
	{
		cout << extended.size() << "�ڲ���Ŀ" << endl;
	}

	*/

	return extended;

}



void outer_extend(int &pos, int& xx, DFA& dfa, DFATransitionTable& LRState,
	const unordered_set<string>& noter, const ProducerVecStr& pro,
	map<int, unordered_set<int>>& First, const unordered_set<string>& terminal,
	int &divide, map<int, vector<int>> &wp_map, ProducerVec & pro_num,
	unordered_map<string, int> & sn_map, unordered_map<int, string> & ns_map)
//����ÿһ����Ŀ����ÿһ���ķ����ţ����Ƿ���ܵõ�һ������Ŀ
//������Ծͼ��뵽������   pos����Ŀ��ţ� xxΪ��Ҫ���Ե��ں�����ӵ��ս��/���ս��
{
	unordered_set<LRitem> ans;

	for (auto state1 : dfa[pos])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
	{
		auto LRitem1 = state1.first;
		if (LRitem1.first.size() == LRitem1.second) continue;
		//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���

		unordered_set<LRitem> outer_temp;
		if (LRitem1.first[LRitem1.second] == xx)
		{

			outer_temp.insert(make_pair(make_pair(LRitem1.first, LRitem1.second + 1), state1.second));

		}
		unordered_set<LRitem> temp = inter_extend(outer_temp, divide,wp_map, pro_num,  sn_map,  First,ns_map);//���������ƶ�һλ�ıհ�
		ans.insert(temp.begin(), temp.end());//set�����Զ�ȥ��
	}

	if (ans.empty()) return;  //�õ����ǿյģ���ôֱ�ӷ���

	int loc = find(dfa.begin(), dfa.end(), ans) - dfa.begin(); //find�����������е�λ�ã���ȥ��ʼλ�õõ����
	if (loc != dfa.size())
	{
		LRState[make_pair(pos, xx)] = loc;   //�ҵ����Ѿ����ڵ���Ŀ����ô�ƽ�

	}
	else
	{
		dfa.push_back(ans);     //�ҵ��Ĳ����Ѿ����ڵ���Ŀ����ô����DFA������
		LRState[make_pair(pos, xx)] = dfa.size() - 1;        //�ƽ����µļ�����
	}

}





// �������ɼ���֮���ת����ϵ
void construct_LR1_sets(const vector<int> &startobj, const unordered_set<string>& terminal, const unordered_set<string>& noter,
	const ProducerVecStr& pro, map<int, unordered_set<int>>& First, DFA& dfa, DFATransitionTable& LRState, unordered_map<string, int> &sn_map,
	int& divide, map<int, vector<int>> &wp_map, ProducerVec &pro_num ,unordered_map<int, string>& ns_map)
{

	unordered_set<LRitem> statu;

	statu.insert(make_pair(make_pair(startobj, 1), sn_map["#"]));

	statu = inter_extend(statu, divide, wp_map, pro_num, sn_map, First, ns_map); //��ʼ��Ŀ�ıհ�����Ϊһ����Ŀ����
	


	//set<LRitem> statu = inter_extend(make_pair(make_pair(startobj, 1), sn_map["#"])); //��ʼ��Ŀ�ıհ�����Ϊһ����Ŀ����
	unordered_set<int> after_set;

	dfa.push_back(statu);  // ����DFA�У��õ�һ�����Ϊ0����Ϊλ��Ϊ0��

	for (int i = 0; i < dfa.size(); i++)  //����ÿһ����ֱ��DFA��������Ϊֹ  push back ���Զ������������һ��
	{
		cout << dfa.size() << endl;

		after_set.clear();

		for (auto state1 : dfa[i])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
		{

			auto LRitem1 = state1.first;

			if (LRitem1.first.size() == LRitem1.second) continue;

			after_set.insert(LRitem1.first[LRitem1.second]);
		}

		for (auto t : after_set) outer_extend(i, t,dfa,  LRState,
			 noter,  pro,
			 First, terminal,
			 divide,wp_map,  pro_num,
			 sn_map,ns_map);
	}

	
	//����Ϊ��ӡ���ز���
	/*
	for (int i = 0; i < dfa.size(); i++)
	{
		cout << i << endl;
		for (auto state1 : dfa[i])   //jΪһ��obj
		{
			auto LRitem1 = state1.first;
			vector<int> temp = LRitem1.first;
			cout << ns_map[temp[0]] << "->";
			for (int k = 1; k < temp.size(); k++)
			{
				if (k == LRitem1.second) cout << "�� ";
				cout << ns_map[temp[k]] << " ";
			}
			if (LRitem1.second == temp.size()) cout << "�� ";
			cout << "," << ns_map[state1.second] << endl;
		}
		cout << endl;
	}

	//����Ϊ��ӡ�ƽ�����     
	for (auto i : LRState)
	{
		cout << i.first.first << "����" << ns_map[i.first.second] << "�ƽ���" << i.second << endl;
	}

	*/
}



StateCore get_core(const DFAState& LRset) {
	StateCore rst;
	for (auto i : LRset) {
		rst.insert(i.first);
	}
	return rst;
}

// �ϲ�ͬ����
void merge_LR1_sets_into_LALR_sets( DFA& dfa, DFATransitionTable& LRState) {
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