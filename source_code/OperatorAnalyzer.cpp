//运算符识别

#include"common.h"
#include"util.h"
#include"OperatorAnalyzer.h"
#include"LexicalAnalyzer.h"

extern std::string Operator[OPERATOR_NUM];

OperatorAnalyzer::OperatorAnalyzer()
{
    for(unsigned int i=0;i<OPERATOR_NUM;i++)
        Operator_Map[Operator[i]]=i;
}

// int OperatorAnalyzer::Search_Operator(char* ch)
// {
//     //查表看是否是运算符
//     auto iter=Operator_Map.find(ch.c_str());
//     if(iter!=Operator_Map.end())
//         return iter->second;
//     else
//         return NOT;
// }


int OperatorAnalyzer::isAccepted (const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{
    uint pos=start;
    uint cur_col = col;

    //DFA 转移
    char ch=str[pos];//进来一定是Operator
    //while(isOperator(ch))
    {
        if(ch==','||ch=='.'||ch=='?'||ch==':'){
            end=pos;
            //return Search_Operator((&ch).c_str());
        }
        else if(ch=='+'){
            char ch1=str[pos+1];
            if(ch1=='+'){
                end=pos+1;
                // return Search_Operator((char*)"++");
            }
            else if(ch1=='='){
                end=pos+1;
                // return Search_Operator((char*)"+=");
            }
            else{
                end=pos;
                // return Search_Operator(&ch);
            }
        }
        else if(ch=='-'){
            char ch1=str[pos+1];
            if(ch1=='-'){
                end=pos+1;
                // return Search_Operator((char*)"--");
            }
            else if(ch1=='='){
                end=pos+1;
                // return Search_Operator((char*)"-=");
            }
            else if(ch1=='>'){
                end=pos+1;
                // return Search_Operator((char*)"->");
            }
            else{
                end=pos;
                // return Search_Operator(&ch);
            }
        }
        else if(ch=='*'||ch=='/'||ch=='%'||ch=='='||ch=='!'||ch=='~'||ch=='^'){
            char ch1=str[pos+1];
            if(ch1=='='){
                end=pos+1;
                std::string temp=&ch;
                temp+="=";
                // return Search_Operator((char*)temp.c_str());
            }
            else{
                end=pos;
                // return Search_Operator(&ch);
            }
        }
        else if(ch=='>'){
            char ch1=str[pos+1];
            if(ch1=='>'){
                char ch2=str[pos+2];
                if(ch2=='='){
                    end=pos+2;
                    // return Search_Operator((char*)">>=");
                }
                else{
                    end=pos+1;
                    // return Search_Operator((char*)">>");
                }
            }
            else if (ch1 == '=') {
                end = pos + 1;
                // return Search_Operator((char*)">=");
            }
            else{
                end=pos;
                // return Search_Operator(&ch);
            }
        }
        else if(ch=='<'){
            char ch1=str[pos+1];
            if(ch1=='<'){
                char ch2=str[pos+2];
                if(ch2=='='){
                    end=pos+2;
                    //return Search_Operator((char*)"<<=");
                }
                else{
                    end=pos+1;
                    //return Search_Operator((char*)"<<");
                }
            }
            else if (ch1 == '=') {
                end = pos + 1;
                // return Search_Operator((char*)"<=");
            }
            else{
                end=pos;
                //return Search_Operator(&ch);
            }
        }
        else if(ch=='&'){
            char ch1=str[pos+1];
            if(ch1=='&'){
                end=pos+1;
                //return Search_Operator((char*)"&&");
             }
            else if(ch=='='){
                end=pos+1;
                //return Search_Operator((char*)"&=");
            }
            else{
                end=pos;
                //return Search_Operator(&ch);
            }
        }
        else if(ch=='|'){
            char ch1=str[pos+1];
            if(ch1=='|'){
                end=pos+1;
                //return Search_Operator((char*)"||");
             }
            else if(ch=='='){
                end=pos+1;
                //return Search_Operator((char*)"|=");
            }
            else{
                end=pos;
                //return Search_Operator(&ch);
            }
        }
        else{//都没匹配到
            return NOT;
            //break;
        }
    }
    //成功
    col = cur_col + (end - pos) + 1;
    return SUCCESS;
}
