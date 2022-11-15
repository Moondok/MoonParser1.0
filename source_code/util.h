#ifndef UTIL_H
#define UTIL_H
const unsigned int WIDTH=1200;
const unsigned int HEIGHT=900;

#include<iostream>
/*********************判断是否为字母********************/
bool isLetter(char letter);

/*****************判断是否为数字************************/
bool isDigit(char digit);
bool isDelimiter(char ch);
bool isOperator(char ch);

int change_to_sign(int result);//转为对应标志
void sign_to_output(int sign, std::string& in_sign, std::string& output);//转为对应type

#endif // UTIL_H
