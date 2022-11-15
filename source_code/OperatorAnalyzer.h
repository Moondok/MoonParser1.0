#ifndef OPERATORANALYZER_H
#define OPERATORANALYZER_H

#include"common.h"
#include"abstractdfa.h"

class OperatorAnalyzer: public AbstractDFA
{
private:
     std::map<std::string, int> Operator_Map;
public:
    OperatorAnalyzer();
    int Search_Operator(std::string ch);
    // virtual  int isAccepted (ScanBuffer & ourScanBuffer);
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};



#endif // OPERATORANALYZER_H
