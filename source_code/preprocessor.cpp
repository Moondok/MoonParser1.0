//预处理器
#include"common.h"
#include"preprocessor.h"

int Preprocessor::preprocess(std::string &in_str,std::string &out_str,int &line,int &col)
{
    //预处理
        out_str = "";
        unsigned int index = 0;
        int cur_col = col;
        int cur_line = line;
        while (index < in_str.size()) {
            char ch = in_str[index];
            //去注释
            if (ch == '/') {
                char ch1 = in_str[++index];
                if (index >= in_str.size()) {
                    line = cur_line;
                    col = cur_col;
                    return NOT;
                }
                if (ch1 == '/') {// "//"
                    while ((ch1 = in_str[++index]) != '\n') {
                        cur_col++;
                    }
                    out_str += '\n';
                    cur_line++;
                    cur_col = 0;
                }
                else if (ch1 != '*') {
                    out_str += '/';
                    out_str += ch1;
                    cur_col++;
                }
                else {// "/*    */"
                    char ch2 = in_str[++index];
                    if (index >= in_str.size()) {
                        line = cur_line;
                        col = cur_col;
                        return NOT;
                    }
                    while (1) {
                        while (ch2 != '*') {
                            if (ch2 == '\n') {
                                cur_line++;
                                cur_col = 0;
                                out_str += '\n';
                            }
                            ch2 = in_str[++index];
                            cur_col++;
                            if (index >= in_str.size()) {
                                line = cur_line;
                                col = cur_col;
                                return NOT;
                            }
                        }
                        ch2 = in_str[++index];
                        cur_col++;
                        if (index >= in_str.size()) {
                            line = cur_line;
                            col = cur_col;
                            return NOT;
                        }
                        if (ch2 == '/')
                            break;
                        else if (ch2 == '\n') {
                            cur_line++;
                            cur_col = 0;
                            out_str += '\n';
                        }
                    }
                }
            }

            //去Tab->空格
            else if (ch == '\t') {
                out_str += ' ';
                cur_col++;
            }
            else if (ch == '\n') {
                out_str += '\n';
                cur_line++;
                cur_col = 0;
            }
            //正常
            else {
                out_str += ch;
                cur_col++;
            }
            index++;
        }
        line = cur_line;
        col = cur_col;
        return 1;//正常返回
}


int Preprocessor::isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col)
{
    return 0;
}
