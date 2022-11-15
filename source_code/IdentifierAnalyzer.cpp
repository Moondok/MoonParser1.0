//标识符识别
#include "util.h"
#include "IdentifierAnalyzer.h"
#include <minwindef.h>

extern std::set<std::string> Identifier;


void IdentifierAnalyzer::InsertToken(std::string ch)//插入标识符表里
{
    //动态加入标识符
    Identifier.insert(ch);
}

int IdentifierAnalyzer::isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{
    uint pos=start;
    uint cur_col = col;
    std::string word="";
    while(1){
        if(isLetter(str[pos])||(isDigit(str[pos]))){//由于传入前已经特判，所以一定不是数字开头
            pos++;
            cur_col++;
            word+=str[pos];
        }
        else 
            break;
    }
    end=pos-1;
    col = cur_col;
    if (word == "")
        return NOT;

    //成功-返回位置
    InsertToken(word);
    return SUCCESS;
}