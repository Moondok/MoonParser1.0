#ifndef COMMON_H
#define COMMON_H

#include<Windows.h>
#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<string.h>
#include <vector>
#include<fstream>
#include<sstream>
//#include <winerror.h>
#include <set>

using uint=unsigned int;

const int CANNOT_ENTER=-2;
const int NOT=-1;
const int SUCCESS=1;
const int PreProcess_NOT=-50;
const int NOT_BOUND=1000;//原来不在表中
const int RIGHT_STATUS=10000;

const int RESERVED_WORD=0;
const int IDENTIFIER=1;
const int OPERATOR=2;
const int DELIMITER=3;
const int DIGIT=4;

const int RESERVED_WORD_SIGN=100;
const int IDENTIFIER_SIGN=200;
const int OPERATOR_SIGN=300;
const int DELIMITER_SIGN=400;
const int DIGIT_SIGN=500;

const int RESERVED_WORD_NUM=12;
const int OPERATOR_NUM=37;
const int DELIMITER_NUM=7;

struct token
{
    unsigned int row;//起始行
    unsigned int column;//起始列
    std::string token_name;
    std::string type; //token_type是一个枚举类
};


#endif // COMMON_H
