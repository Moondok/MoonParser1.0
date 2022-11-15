#ifndef IDENTIFIERANALYZER_H
#define IDENTIFIERANALYZER_H

#include "common.h"
#include "util.h"
#include"abstractdfa.h"

class IdentifierAnalyzer: public AbstractDFA
{
private:
    uint cur_map_index=0;//Token_Map有几个

public:
    void InsertToken(std::string ch);
    // virtual  int isAccepted (ScanBuffer & ourScanBuffer);
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};





#endif // IDENTIFIERANALYZER_H
