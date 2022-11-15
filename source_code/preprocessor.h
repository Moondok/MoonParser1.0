#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include"common.h"
#include "abstractdfa.h"
#include <string>

class Preprocessor: public AbstractDFA
{
public:
    int preprocess(std::string & in_str,std::string &out_str, int& line, int& col);
    // virtual int isAccepted(ScanBuffer & ourScanBuffer);
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;

};


#endif // PREPROCESSOR_H
