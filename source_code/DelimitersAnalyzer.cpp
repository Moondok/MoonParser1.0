//界符识别
#include"common.h"
#include "DelimitersAnalyzer.h"

extern std::string Delimiter[DELIMITER_NUM];

int DelimitersAnalyzer::searchDelimiter(std::string ch)//判断是否在界符表里
{
    for(int i=0;i<DELIMITER_NUM;i++){
        if(ch.find(Delimiter[i])!=-1)
            return i;
    }
    return NOT;
}

int DelimitersAnalyzer::isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{
    uint pos=start;
    if(searchDelimiter(&str[pos])==NOT){
        return NOT;
    }
    else{
        start=end=pos;
        col++;
        return SUCCESS;
    }
}