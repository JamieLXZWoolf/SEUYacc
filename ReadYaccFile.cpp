#include "pch.h"
#include "Common.h"

using namespace std;

// ��������.y�ļ�
int read_yacc_file(const string& fileName, string& start, unordered_set<string>& terminal, ProducerVecStr& pro, string& definition, string& code) {
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
	} while (str != "%left" && str != "%type" && str != "%%" && str != "%start");  // ����token����

	if (str == "%start") {
		in >> str;
		start = str;
	}

	do
	{
		in >> str;

	} while (str != "%left" && str != "%%");           // ����м�������ı�


	//��ʼ�������ʽ
	in >> str;
	int counter = 0;
	do {
		/*cout << str << endl;*/
		pair<string, vector<string>> p;

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

			while (str != "|" && str != ";")
			{
				/*cout << str << endl;*/
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