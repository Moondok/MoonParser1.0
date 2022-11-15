#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include "common.h"
#include <vector>
#include "abstractdfa.h"

// #include <wronginfo.h>
// #include <acceptinfo.h>
#include "IdentifierAnalyzer.h"
#include "DelimitersAnalyzer.h"
#include "OperatorAnalyzer.h"
#include "ReservedWordAnalyzer.h"
#include "Digitanalyzer.h"

#include "preprocessor.h"

class LexicalAnalyzer
{
private:
    // std::string inBuffer;
    // ScanBuffer ourScanBuffer;
    // char* inputBuffer;//存储着预处理之后的字符串
    // unsigned int inputBufferPtr=0;  //当前取用到输入缓冲区的位置


    Preprocessor preprocessor;
    AbstractDFA* DFAlist[5];
    static const int MAX_DIGIT_NUM=500;
    static const int MAX_IDENTIFIER_NUM=500;

public:
    LexicalAnalyzer();
    ~LexicalAnalyzer();
    int analyze(std::string &in_str,std::string &out_str, int& pre_line, int& pre_col);
    // std::string &get(){return inBuffer; }

    
    token digit_table[MAX_DIGIT_NUM];
    token identifier_table[MAX_IDENTIFIER_NUM];
    unsigned int line=0; //行数
    unsigned int start=0,end=0;//出错起始位置

// signals:
//     void wrong(WrongInfo info);
//     void accept(AcceptInfo info);


};




#endif // LEXICALANALYZER_H
