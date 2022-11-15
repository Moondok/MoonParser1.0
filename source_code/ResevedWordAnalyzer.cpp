//保留字识别


#include "ReservedWordAnalyzer.h"
#include <vector>
#include "LexicalAnalyzer.h"

extern std::string ReserveWord[RESERVED_WORD_NUM];

int ReservedWordAnalyzer::isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{      
    uint pos=start;
    uint cur_col = col;
    std::string word="";

    while(1){
        if(isLetter(str[pos])){
            word += str[pos];
            pos++;
            cur_col++;
        }
        else
            break;
    }

    //判断是否为保留字
    auto iter=ReservedWord_Map.find(word);
    if(iter!=ReservedWord_Map.end()){//找到
        end=pos-1;//回退 用于字符串定位
        col = cur_col;
        return iter->second;//在保留字表中位置
    }
    else
        return NOT;//不是保留字
}


ReservedWordAnalyzer::ReservedWordAnalyzer()
{
    //建立保留字表中值与索引的mapping关系
    for(unsigned int i=0;i<RESERVED_WORD_NUM;i++)
        ReservedWord_Map[ReserveWord[i]]=i;
}
