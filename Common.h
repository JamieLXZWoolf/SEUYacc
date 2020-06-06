#pragma once
#include "pch.h"

#ifndef COMMON_H
#define COMMON_H

// ԭ���Ĳ���ʽ������ʽ�󲿣�����ʽ�Ҳ�)������
typedef std::vector<std::pair<std::string, std::vector<std::string>>> ProducerVecStr;

// ����ʽ�Լ�����λ�ã�����Ϊһ����Ŀ, ע��vector�ĵ�һ��Ԫ��Ϊ����ʽ��߷���
typedef std::pair<std::vector<std::string>, int> Item;

// LR(1)/LALR��Ŀ
typedef std::pair<Item, std::string> LRitem;

typedef std::set<LRitem> DFAState;

typedef std::vector<DFAState> DFA;

typedef std::set<Item> StateCore;

typedef std::map<std::pair<int, std::string>, int> DFATransitionTable;

// ��������մ����ɸ��ݲ�ͬ��.y�ļ�����
const std::string kong = "eplsion";

#endif // !COMMON_H
