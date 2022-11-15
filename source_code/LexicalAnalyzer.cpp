#include <string>
#include"common.h"
#include "util.h"
#include "LexicalAnalyzer.h"


std::string Delimiter[DELIMITER_NUM]=
{
    "(",")",
    "{", "}",
    "[","]",
    ";"
};



// std::vector<const char*> Delimiter=
// {
//     "(",")",
//     "{", "}",
//     "[","]"
// };

string Operator[OPERATOR_NUM]=
{
    "+", "-", "*", "/", "%","++","--",                      //算术运算符 7
    ">", "<","==",">=", "<=","!=",                          //关系运算符 6
    "&&", "||","!",                                         //逻辑运算符 3
    "&","|","~","^","<<",">>",                              //位运算符 6
    "+=","-=","*=","/=","%=","&=","|=","^=",">>=","<<=",    //赋值运算符 11
    "\?",":",                                               //三目？:
    ",",                                                     //逗号 1
    ".","->"
};

// std::vector<const char*> Operator=
// {
//     "+", "-", "*", "/", "%","++","--",                      //算术运算符 7
//     ">", "<","==",">=", "<=","!=",                          //关系运算符 6
//     "&&", "||","!",                                         //逻辑运算符 3
//     "&","|","~","^","<<",">>",                              //位运算符 6
//     "+=","-=","*=","/=","%=","&=","|=","^=",">>=","<<=",    //赋值运算符 11
//     "\?",":",                                               //三目？:
//     ",",                                                     //逗号 1
//     ".","->"
// };

string ReserveWord[RESERVED_WORD_NUM]=
{
        "int", "double", "char", "float", "break", "continue",
    "while", "if", "else", "for", "return",
    "function"
};
// std::vector<const char*> ReserveWord=
// {
//     "int", "double", "char", "float", "break", "continue",
//     "while", "if", "else", "for", "return"
// };

std::set<std::string> Identifier;//标识符
std::set<std::string> Digit;

// std::vector<char*> ConstStr;

int LexicalAnalyzer::analyze(std::string & in_str,std::string & out_str,int &pre_line,int &pre_col)
{
    //预处理
    if(preprocessor.preprocess(in_str,out_str,pre_line,pre_col)!=1){
        string out_path = "./Wrong_info.txt";
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        outfile <<pre_line << ' ' << pre_col << ' ' << "Preprocess!";
        return PreProcess_NOT;//预处理报错
    };

    
    //调各dfa识别出token
    int sign=RIGHT_STATUS;//是否出错
    uint start = 0, end = -1;
    uint line = 1, col = 0;
    stringstream out_stream;
    uint last_line = line, last_col = col;

    while(1){
        //每次由[start,end]跳到[end+1, ]
        start=end+1;

        if(start>=out_str.size()){
            break;
        }
        //去掉所有当前空格和换行
        {
            uint pos=start;
            uint cur_col=col;
            uint cur_line=line;
            while(out_str[pos]==' '||out_str[pos]=='\n'||out_str[pos]=='\r'){
                if(out_str[pos]==' '||out_str[pos]=='\r'){
                    pos++;
                    cur_col++;
                }
                else{//'\n'
                    pos++;
                    cur_line++;
                    cur_col=0;
                }
                if(pos>=out_str.size()){
                    break;
                }
            }
            start=pos;
            end=pos;
            line=cur_line;
            col=cur_col;
        }
        if(start>=out_str.size()){
            sign=RIGHT_STATUS;
            break;
        }

        //预判当前字符来确定进哪个DFA
        int enter_dfa=-1;
        {
            char ch=out_str[start];//得到当前的，在外部判断去哪个dfa，方便报错
            //界符delimiter
            if(isDelimiter(ch)){
                enter_dfa=DELIMITER;
            }
            //数字digit
            else if(isDigit(ch)){
                enter_dfa=DIGIT;
            }
            //运算符operator
            else if(isOperator(ch)){
                enter_dfa=OPERATOR;
            }
            //其他就随便进入 标识符，关键字
        }

        //调用各dfa
        if(enter_dfa==-1){//没有选择
            int i; 
            for(i=0;i<5;i++){
                if (i == OPERATOR || i == DELIMITER || i == DIGIT)
                    continue;
                int result=DFAlist[i]->isAccepted(out_str,start,end,line,col);
                if(result==NOT){//这个dfa不行
                    continue;
                }
                else{
                    //成功找到
                    enter_dfa=i;
                    break;
                }
            }
            if(i>=5){
                end = start + 5;//往后几个，定位局部
                sign=change_to_sign(DELIMITER);  //制成任意一个报错
            }
        }
        else{//有要选择的dfa
            int result=DFAlist[enter_dfa]->isAccepted(out_str,start,end,line, col);
            if(result==NOT){//报特定的dfa错误
                sign=change_to_sign(enter_dfa);
            }
        }

        //判断是否终止识别
        if(sign!=RIGHT_STATUS){
            break;//sign中包含哪个错误类型
        }
        else{
            //输出token
            //根据特定要求输出——有的是operator,有的是assign operator 在type部分修改（例如左右括号就不属于
            string value=out_str.substr(start,end-start+1);
            int sign = change_to_sign(enter_dfa);
            string type;
            sign_to_output(sign, value, type);
            out_stream << line << ' ' << col << ' ' << type << ' ' << value << "\n";
            //cout<< line << ' ' << col << ' ' <<type << ' ' << value << "\n";
            last_line = line;
            last_col = col;
        }
    }
    //输出到文件
    if (sign == RIGHT_STATUS) {
        string out_path = "./token_result.txt";
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        outfile << out_stream.str();
        outfile << last_line << ' ' << 1 << ' ' << "#" << ' ' << "#" << "\n";
        outfile.close();
    }
    else {
        string out_path = "./Wrong_info.txt";
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        string value = out_str.substr(start, end - start + 1);
        string type;
        sign_to_output(sign, value, type);
        outfile << last_line << ' ' << last_col+1 << ' ' << type << ' ' << value << "\n";
        outfile.close();
    }

    //如果都没识别到->报错
    return sign;
}


LexicalAnalyzer::LexicalAnalyzer()
{
//    DFAlist[0]=new ReservedWordAnalyzer;
//    DFAlist[1]=new IdentifierAnalyzer;
//    DFAlist[2]=new OperatorAnalyzer;
//    DFAlist[3]=new DelimitersAnalyzer;
//    DFAlist[4]=new DigitAnalyzer;
   DFAlist[RESERVED_WORD]=new ReservedWordAnalyzer;
   DFAlist[IDENTIFIER]=new IdentifierAnalyzer;
   DFAlist[OPERATOR]=new OperatorAnalyzer;
   DFAlist[DELIMITER]=new DelimitersAnalyzer;
   DFAlist[DIGIT]=new DigitAnalyzer;

    




}

LexicalAnalyzer::~LexicalAnalyzer()
{
    for(unsigned int i=0;i<5;i++)
    {
        if(DFAlist[i]!=nullptr)
        {
            delete DFAlist[i];
            DFAlist[i]=nullptr;
        }
    }
}

