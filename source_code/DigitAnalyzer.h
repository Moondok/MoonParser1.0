#ifndef DIGITANALYZER_H
#define DIGITANALYZER_H
#include<iostream>
#include "abstractdfa.h"
using namespace std;
//class AbstractDFA
//{
////    virtual int isAccepted (const std::string & str, unsigned int & start, unsigned int & end,unsigned int & line)=0;
//    virtual int isAccepted (ScannerBuffer sbuffer)=0;
//    virtual void preprocess(std::string & in_str,std::string &out_str);
//};
class DigitAnalyzer : public AbstractDFA
{
private:
    //const int STARTSTATE=0;
    //const int EORPOINTSTATE=1;
    //const int ERRORSTATE=2;
    //const int RECSTATE=3;
    //const int NUMSTATE=4;
    const int STATE_START = 0;
    const int STATE_1 = 1;
    const int STATE_2 = 2;
    const int STATE_3 = 3;
    const int STATE_4 = 4;
    const int STATE_ERROR = 5;
    const int STATE_TERMINATE = 6;
    int state;
public:
    DigitAnalyzer();
    // virtual int isAccepted(ScanBuffer& sbuffer);// override;
    // virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line);
    void clear();//回到初始状态
    int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};

#endif // DIGITANALYZER_H
