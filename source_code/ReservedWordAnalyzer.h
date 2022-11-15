#ifndef RESERVEDWORDANALYZER_H
#define RESERVEDWORDANALYZER_H

#include "common.h"
#include "util.h"
#include "abstractdfa.h"
//#include "LexicalAnalyzer.h"



class ReservedWordAnalyzer: public AbstractDFA
{
private:
    std::map<std::string, int> ReservedWord_Map;

public:
    ReservedWordAnalyzer();
    // virtual int isAccepted (ScanBuffer & ourScanBuffer);//override virtual function
    virtual int isAccepted (const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;//override virtual function
};



#endif // RESERVEDWORDANALYZER_H
