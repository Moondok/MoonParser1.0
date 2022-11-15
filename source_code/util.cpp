#include"util.h"
#include "LexicalAnalyzer.h"

extern std::string Delimiter[DELIMITER_NUM];
extern std::string Operator[OPERATOR_NUM];


bool isLetter(char letter)
 {//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
     return((letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z')|| letter=='_')?true:false;
 }
 /*********************判断是否为字母********************/


 /*****************判断是否为数字************************/
 bool isDigit(char digit)
 {
     return (digit >= '0'&&digit <= '9')?true:false;
 }
/*****************判断是否为数字************************/


 bool isDelimiter(char ch)
 {
    for(int i=0;i<DELIMITER_NUM;i++){
        if(Delimiter[i][0]==ch){
            return true;
        }
    }
    return false;
 }

 bool isOperator(char ch)
 {
     for(int i=0;i<OPERATOR_NUM;i++){
        if(Operator[i][0]==ch){
            return true;
        }
    }
    return false;
 }


 int change_to_sign(int result)//用0-4转移到100-500
 {
    int sign;
    switch(result){
        case RESERVED_WORD:
            sign=RESERVED_WORD_SIGN;
            break;
        case IDENTIFIER:
            sign=IDENTIFIER_SIGN;
            break;
        case OPERATOR:
            sign=OPERATOR_SIGN;
            break;
        case DELIMITER:
            sign=DELIMITER_SIGN;
            break;
        case DIGIT:
            sign=DIGIT_SIGN;
            break;
        default:
            sign= DELIMITER_SIGN;
            break;
    }
    return sign;
 }

 void sign_to_output(int sign,std::string & in_sign,std::string &output)
 {
     if (sign == RESERVED_WORD_SIGN) {
         //if (in_sign == "int" || in_sign == "double" || in_sign == "float" || in_sign == "char")
         //    output = "type_specifier";
         //else
         //    output = "None";
         output = in_sign;
     }
     else if (sign == IDENTIFIER_SIGN) {
         output = "identifier";
     }
     else if (sign == OPERATOR_SIGN) {
         output = in_sign;
     }
     else if (sign == DELIMITER_SIGN) {
         output = in_sign;
     }
     else{
         output = "number";
     }
 }

