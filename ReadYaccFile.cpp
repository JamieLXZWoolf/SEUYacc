#include "pch.h"
#include "Common.h"

using namespace std;

// ��������.y�ļ�
int read_yacc_file(const string& fileName, string& start, unordered_set<string>& terminal, ProducerVecStr& pro,  string& definition, string& code, unordered_set<string>& Left, unordered_map<string, int>& Left_level) {
	
	int ambiguity = 0;//�����ж�yacc��ʽ���� 0Ϊ
	int lines = 0;  //��ʶ������������
	int opLevel = 0; //��ʶ���������ȼ�

	ifstream in;//����myyacc�ļ�
	//cout << "fa";
	in.open(fileName, ios::in);
	if (!in)
	{
		cout << "ERROR: Can't open Yacc file!" << endl;
		return 1;
	}
	string str;
	int i = 0;//��ʾstatement�еڼ���

	getline(in, str);
	if (str == "%{") {
		getline(in, str);
		while (str != "%}" && !in.eof())
		{
			definition += str + "\n";
			getline(in, str);
		}
		if (in.eof()) {
			cout << "ERROR: incomplete code definition area." << endl;
			return 0;
		}
	}

	do
	{
		in >> str;
	} while (str != "%token");

	/*������token����token��*/
	in >> str;
	do
	{
		if (str == "%token")
		{
			in >> str;
			// cout << str << endl;
		}
		//cout << str << endl;
		terminal.insert(str);
		in >> str;
	} while (str != "%left"&&str != "%type"&&str != "%%"&&str != "%start");  //���� token����



	do
	{

		if (str == "%start")
		{
			cout << "---------------------------------------------------------------" << endl;
			ambiguity = 2; //�����%start���Զ����ɣ�û�еĻ���Ѳ���ʽ��һ������start
			break;
		}

		in >> str;
		if (str == "%left")
		{
			ambiguity = 1; //��%left˵�����ķ��Ƕ������ķ�����Ҫ����������ȼ�
			cout << "bi" << endl;
			break;
		}



		if (str == "%%")
		{
			ambiguity = 0; //û��%left˵�����ķ����Ƕ������ķ�������Ҫ����������ȼ�
			break;
		}
	} while (1);           //����м�������ı�



	if (ambiguity == 1)
	{

		cout << "fa1" << endl;
		cout << str << endl;

		do
		{
			//cout << str<<endl;
			if (str == "%left")
			{
				in >> str;
				opLevel++;
			}

			if (str == "/*")
			{
				in >> str;
				while (str != "*/")
					in >> str;
				if (str != "*/")
				{
					in >> str;
				}

				if (str == "%left")
				{
					in >> str;
					opLevel++;
				}
			}

			if (str != "*/")
			{
				Left.insert(str);
				Left_level.insert(make_pair(str, opLevel));

			}

			in >> str;

		} while (str != "%start"&&str != "%%");

	}


	if (ambiguity == 2)
	{

		cout << "�ڶ���" << endl;
		cout << str << endl;

		do
		{
			//cout << str<<endl;
			if (str == "%start")
			{
				in >> str;
			}

			if (str == "/*")
			{
				in >> str;
				while (str != "*/")
					in >> str;
				if (str != "*/")
				{
					in >> str;
				}

				if (str == "%left")
				{
					in >> str;
					opLevel++;
				}
			}

			if (str != "*/")
			{
				start = str;
			}

			in >> str;

		} while (str != "%%");

	}


	bool flat_start = true;

	//��ʼ�������ʽ
	in >> str;
	int counter = 0;
	do {
		cout << "fa" << endl;
		cout << str << endl;
		pair<string, vector<string> > p;

		if (ambiguity == 1 && flat_start) {
			start = str; //û��start�ģ���һ��������ʽ
			flat_start = false;

			//	cout << "start:" << start << endl;
		}
		p.first = str;  //���
		in >> str;   //ȫ��:

		if (str != ":")
		{
			cout << str << endl;
			cout << "ERROR: NO \":\" " << endl;
			return -1;
		}
		in >> str;  //��һ��ð�ź� �ո��Ķ���

		do {
			if (str == "|")
			{
				pro.push_back(p);
				p.second.clear();
				in >> str;
			}

			while (str != "|"&&str != ";")
			{
				cout << str << endl;
				p.second.push_back(str);    //�Ҳ�ļ���
				in >> str;  //�ո��Ķ���

			}
		} while (str != ";");
		pro.push_back(p);
		in >> str;  //ÿ�Σ���ߵĵ���

	} while (str != "%%");
	//���뺯������
	do {
		getline(in, str);
		code += str + "\n";
	} while (!in.eof());

	terminal.insert("#");//���ս�����#

	//cout << "���end" << endl;
	return 1;

}