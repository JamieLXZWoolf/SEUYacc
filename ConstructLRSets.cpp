#include "pch.h"
#include "Common.h"

using namespace std;


extern map<int, unordered_set<int>> First;
extern ProducerVec pro_num;
extern int  divide;
extern unordered_set<int> terminal_num;
extern unordered_set<int> noter_num;

// DFAת����, pair<��Ŀ���, �ս��/���ս��>, �ƽ���Ŀ���, i״̬ʶ��һ��x�󵽴�j״̬.
extern DFATransitionTable LRState;

// ��Ŀ�� �淶��
extern DFA dfa;

extern vector<int> startobj;

// ��ʼ��
extern string start, definition, code;

extern unordered_map<string, int> sn_map;
extern unordered_map<int, string> ns_map;

extern map < vector<int>, int > r_num;
extern map<int, vector<int>> wp_map;

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

	/*
	string temp2 = "declarations";  //�������ڵ�һ������չ�Ĳ���ʽ
	startobj.push_back(sn_map[temp2]);
	temp2 = "declaration";
	startobj.push_back(sn_map[temp2]);

	temp2 = "declarations";
	startobj.push_back(sn_map[temp2]);

	*/

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

	//cout << "divide: --------------  " << divide << endl;

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

	//for (auto st : sn_map)
	//{
	//	cout << st.first << " -+-" << st.second << endl;
	//}




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

		//cout << "���: " << i << "  ";
		//cout << ns_map[pro_num[i].first] << "->";
		//for (int j = 0; j < pro_num[i].second.size(); j++)
		//	cout << ns_map[pro_num[i].second[j]] << " ";
		//cout << endl;
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






void intersection(unordered_set<int>& leftSet, const unordered_set<int>& rightSet) {
	leftSet.insert(rightSet.cbegin(), rightSet.cend());
}



//����setΪ��
void first_symbol(unordered_set<int>& inputSet, const int& symbol, unordered_set<int >& handlingSymbol) {
	if (First.find(symbol) != First.end()) {//֮ǰ����ˣ�ֱ�Ӽ��룬�����Ż������������������
		intersection(inputSet, First[symbol]);
		return;
	}

	if (symbol == -1) return;                       //�����-1�ǲ���ռ�
	if (symbol < divide) {//symol���ս��
		inputSet.insert(symbol);
		return;
	}                 //ֱ�Ӽ���
	else {//symbol�Ƿ��ս��
		handlingSymbol.insert(symbol);    //����Ѿ��������
		for (int i = 0; i < pro_num.size(); i++)
		{
			if (pro_num[i].first == symbol)
			{            //�ӵ�first��ʼ��֮����second�����Ĳ���ʽ
							   //��������symbol����ߵĲ���ʽ
				auto producer = pro_num[i];   //��һ������ʽ
				if (producer.second.size() == 1 && producer.second[0] == -1) {//symbol->epsilon
					inputSet.insert(-1);   //���ؿմ�
					continue;
				}
				//��������ʽ�ұ�������
				int j = 1;
				unordered_set<int> tempSet;
				while (j <= producer.second.size()) {//�������һ���������epsilon
					tempSet.clear();
					if (handlingSymbol.find(producer.second[j - 1]) != handlingSymbol.end()) {//��ֹ��ݹ�
						break;
					}
					first_symbol(tempSet, producer.second[j - 1], handlingSymbol);
					if (tempSet.find(-1) == tempSet.end()) {
						intersection(inputSet, tempSet);
						break;
					}
					if (j == producer.second.size()) {//���һ���epsilon
						inputSet.insert(-1);

						break;
					}
					tempSet.erase(-1);
					intersection(inputSet, tempSet);
					++j;
				}
			}
		}
	}
}


void first_operation() {
	unordered_set<int> firstSet, stackSet;

	for (int symbol : terminal_num) {
		firstSet.clear();
		stackSet.clear();
		first_symbol(firstSet, symbol, stackSet); //�󵥸�first
		First.emplace(symbol, firstSet);  // symbol������first��
	}

	for (int symbol : noter_num) {
		firstSet.clear();
		stackSet.clear();
		first_symbol(firstSet, symbol, stackSet); //�󵥸�first
		First.emplace(symbol, firstSet);  // symbol������first��
	}



}

void first_string(unordered_set<int>& inputSet, const vector<int>& symbolsVec) {


	int i = 1;
	if (symbolsVec.size() == 0) {
		inputSet.insert(-1);
		return;
	}
	while (i <= symbolsVec.size()) {//�������һ���������epsilon
		auto & tempRef = First[symbolsVec[i - 1]];
		if (tempRef.find(-1) == tempRef.end()) {
			intersection(inputSet, tempRef);
			break;
		}
		if (i == symbolsVec.size()) {//���һ���epsilon
			inputSet.insert(-1);
			break;
		}
		tempRef.erase(-1);
		intersection(inputSet, tempRef);
		++i;
	}
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
				continue;
			}

			queue.pop();

			for (auto i : wp_map[s])    //�ҳ�������ս���Ĳ���ʽ
			{

				vector<int> temp = pro_num[i].second;   //����ʽ����
				temp.insert(temp.begin(), s);
				vector<int> xx;
				//����Ŀ[A->x��by,a]���ڱհ�ans,B->R�ǲ���ʽ
				//��first��Ra) �е�ÿ���ַ���b����[B->r��b]����ans�У�����
				xx.assign(item1.first.begin() + item1.second + 1, item1.first.end());
				//xx.push_back(lritem1.second);

				temp_first.clear();

				first_string(temp_first, xx);
				
				if (temp_first.find(-1) != temp_first.end()) {//��epsilon
					temp_first.erase(-1);
					temp_first.insert(lritem1.second);
				}
				
				/*
				cout <<"Ԥ��"<< endl;
				for (auto LRitem2 : temp_first)   //jΪһ��item
				{
						cout << ns_map[LRitem2] << " ";
				}
				cout << endl;
				*/
				

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




void outer_extend(int & pos, map<int, unordered_set<LRitem> >& newStateMap)
//����ÿһ����Ŀ����ÿһ���ķ����ţ����Ƿ���ܵõ�һ������Ŀ
//������Ծͼ��뵽������   pos����Ŀ��ţ� xxΪ��Ҫ���Ե��ں�����ӵ��ս��/���ս��
{
	unordered_set<LRitem> ans;
	LRitem newItem;
	int edge;
	for (const auto &state1 : dfa[pos])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
	{
		auto LRitem1 = state1.first;
		if (LRitem1.first.size() == LRitem1.second) continue;
		//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���

		edge = LRitem1.first[LRitem1.second];  //���ϵķ���

		newItem.first = LRitem1;
		newItem.first.second++;

		newItem.second = state1.second;

		auto findIt = newStateMap.find(edge);
		if (findIt == newStateMap.end()) {//û�б������ñߣ��½�һ��������
			unordered_set<LRitem> newState;
			newState.insert(newItem);//������µļӵ�������
			newStateMap.emplace(edge, newState);
		}
		else {//����������ñߣ����µ�lritem�ӵ����еļ�����
			findIt->second.insert(newItem);
		}
	}

	/*
	for (auto state1 : newStateMap)   //jΪһ��obj
	{
		auto LRitem1 = state1.first;
		cout << "������" << ns_map[state1.first] << endl;

		for (auto state1 : state1.second)   //jΪһ��obj
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
	}
	cout << "һ����չ�ĳ�ʼ������" << endl;
	//system("pause");
	*/

}

void construct_LR1_sets()   //�������ɼ���֮���ת����ϵ
{


	//vector<string> temp3;
	//temp3.push_back("#");

	//unordered_set<LRitem> statu = inter_extend(make_pair(make_pair(startobj, 1), sn_map["#"])); //��ʼ��Ŀ�ıհ�����Ϊһ����Ŀ����
	unordered_set<LRitem> statu;
	statu.insert(make_pair(make_pair(startobj,1), sn_map["#"]));
	statu = inter_extend(statu, divide, wp_map, pro_num, sn_map, First, ns_map); //��ʼ��Ŀ�ıհ�����Ϊһ����Ŀ����


	queue<int> unhandledStates;    //��һ��û�����״̬
	unhandledStates.push(0);

	dfa.push_back(statu);  // ����DFA�У��õ�һ�����Ϊ0����Ϊλ��Ϊ0��

	//cout << "״̬��------------: " << dfa.size() << endl;
	map<int, unordered_set<LRitem> > newStateMap;

	int stateNumCounter = 1;

	while (!unhandledStates.empty()) {

		
		int i = unhandledStates.front();
		unhandledStates.pop();

		newStateMap.clear();
		/*
		cout << "״̬��: " << dfa.size() << endl;
		if (dfa.size() > 200) system("pause");
		*/

		outer_extend(i, newStateMap);
		//after_set.clear();
		//for (auto state1 : DFA[i])    //x��һ��OBJ  first�ǲ���ʽ  second�ǡ���λ��
		//{
		//	auto LRitem1 = state1.first;
		//	if (LRitem1.first.size() == LRitem1.second) continue;
			//����Ѿ��������һ��λ���ˣ�˵���ǹ�Լ��Ŀ�����ú���
			//after_set.insert(LRitem1.first[LRitem1.second]);
		//for (auto t : after_set)

		for (auto & p : newStateMap) {

			p.second=inter_extend(p.second, divide, wp_map, pro_num, sn_map, First, ns_map);
			int edgeToInt = -1;
			// ����Ƿ������ͬ��״̬
			for (const auto & s : dfa) {
				if (s == p.second ) {
					edgeToInt = find(dfa.begin(), dfa.end(), s) - dfa.begin(); //find�����������е�λ�ã���ȥ��ʼλ�õõ����  s.numberInt;
					
					/*
					for (auto state1 : s)   //jΪһ��obj
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
					cout << "===================" << endl;
					for (auto state1 : p.second)   //jΪһ��obj
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
					

					cout << "����״̬��" << edgeToInt << endl;
					*/
					break;


				}
			}
			if (edgeToInt == -1)//������Ҫ�½�״̬��
			{

				edgeToInt = stateNumCounter;
				unhandledStates.push(stateNumCounter);
				//cout << "��Ҳ��״̬��" << edgeToInt << endl;
				stateNumCounter++;
				dfa.push_back(p.second);     //�ҵ��Ĳ����Ѿ����ڵ���Ŀ����ô����DFA������
			}
			LRState[make_pair(i, p.first)] = edgeToInt;
			/*cout << dfa.size() << endl;*/
		}
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