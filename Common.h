#pragma once
#include "pch.h"

#ifndef COMMON_H
#define COMMON_H

// ԭ���Ĳ���ʽ������ʽ�󲿣�����ʽ�Ҳ�)������
typedef std::vector<std::pair<std::string, std::vector<std::string>>> ProducerVecStr;

typedef std::vector<std::pair<int, std::vector<int> > > ProducerVec;

// ����ʽ�Լ�����λ�ã�����Ϊһ����Ŀ, ע��vector�ĵ�һ��Ԫ��Ϊ����ʽ��߷���
typedef std::pair<std::vector<int>, int> Item;

// LR(1)/LALR��Ŀ
typedef std::pair<Item, int> LRitem;

typedef std::unordered_set<LRitem> DFAState;

typedef std::vector<std::unordered_set<LRitem>> DFA;

typedef std::set<Item> StateCore;

typedef std::map<std::pair<int, int>, int> DFATransitionTable;


// ��������մ����ɸ��ݲ�ͬ��.y�ļ�����
const std::string kong = "epsilon";



namespace std {

	template<>
	struct hash<LRitem> {
		inline size_t operator()(const LRitem& s) const {
			return  hash<int>()(s.second);
		}
	};
}



#endif // !COMMON_H
