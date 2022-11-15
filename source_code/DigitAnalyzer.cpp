#include "DigitAnalyzer.h"
#include"LexicalAnalyzer.h"

extern std::set<std::string> Digit;

DigitAnalyzer::DigitAnalyzer()
{
    state=this->STATE_START;
}

int DigitAnalyzer::isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{
    if(state!= STATE_START)
        return NOT;

    uint i =start;
    for(;;i++){
        if(i>str.size()){
            return NOT;
        }
        if(state== STATE_START){
            if (str[i] <= '9' && str[i] >= '0') {

            }
            else if (str[i] == '.') {
                state = STATE_1;
            }
            else if (str[i] == 'e') {
                state = STATE_3;
            }
            else {
                state = STATE_TERMINATE;
                break;
            }
        }
        else if (state == STATE_1) {
            if (str[i] <= '9' && str[i] >= '0') {
                state = STATE_2;
            }
            else if (str[i] == 'e') {
                state = STATE_3;
            }
            else if(str[i]=='.'){
                state = STATE_ERROR;
                break;
            }
            else {
                state = STATE_TERMINATE;
                break;
            }
        }
        else if (state == STATE_2) {
            if (str[i] <= '9' && str[i] >= '0') {
            
            }
            else if (str[i] == 'e') {
                state = STATE_3;
            }
            else if(str[i]=='.'){
                state = STATE_ERROR;
                break;
            }
            else {
                state = STATE_TERMINATE;
                break;
            }
        }
        else if (state == STATE_3) {
            if (str[i] <= '9' && str[i] >= '0') {
                state = STATE_4;
            }
            else if(str[i]=='.'||str[i]=='e'){
                state = STATE_ERROR;
                break;
            }
            else {
                state = STATE_TERMINATE;
                break;
            }
        }
        else{//STATE_4
            if (str[i] <= '9' && str[i] >= '0') {

            }
            else if(str[i] == '.' || str[i] == 'e'){
                state = STATE_ERROR;
                break;
            }
            else {
                state = STATE_TERMINATE;
                break;
            }
        }
    }
    if(state==STATE_TERMINATE){//其他接受状态后会一直接受符号直到遇到不符符号直到TERMINATE
        end = i - 1;//回退
        state=STATE_START;
        col = col + (i - start);
        //更新Digit
        string temp=str.substr(start,end-start+1);
        Digit.insert(temp);
        return SUCCESS;
    }
    else{
        state= STATE_START;
        return NOT;
    }
}

void DigitAnalyzer::clear()
{
    state=STATE_START;
}
