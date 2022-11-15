#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <stack>
//#include <unordered_set>


const bool NON_TERMINATOR = 1;
const bool TERMINATOR = 0;
const int ACC = -10086;
#define SYNTAX_SUCCESS -1

#define LINEOFNONT -2

struct action_struct
{
    std::string symbol_name;
    int dst;
    action_struct() :symbol_name("head"), dst(-1) {}
    action_struct(const std::string& s, const int d) :symbol_name(s), dst(d) {}
    action_struct* next = nullptr;
};

struct goto_struct
{
    std::string symbol_name;
    int dst;
    goto_struct* next = nullptr;
    goto_struct() :symbol_name("head"), dst(-1) {}
    goto_struct(const std::string& s, const int d) :symbol_name(s), dst(d) {}
};

struct movement
{
    action_struct* action_ptr;
    goto_struct* goto_ptr;

    movement()
    {
        action_ptr = new action_struct("head", -1);
        goto_ptr = new goto_struct("head", -1);
    }
};

struct for_verify_LR1
{
    int index;
    std::string name;
    int dst;

    bool operator < (const for_verify_LR1& a) const
    {
        if (index != a.index)
            return index < a.index;
        else
            return name < a.name;
    }

    for_verify_LR1(int i, std::string n, int d) :index(i), name(n), dst(d)
    {

    }
};

class symbol  //
{


public:

    std::string name;
    bool type = 0;  //0 presents terminator ,vice versa
    int generators_index = -1;
    symbol()
    {

    }
    symbol(std::string n, bool t = 0, int i = -1) :name(n), type(t), generators_index(i)
    {};

    bool operator <(const symbol& a) const
    {
        return this->name < a.name;
    }

    /*bool operator ==(const symbol& a) const
    {
        return this->name == a.name;
    }*/


};


class generator
{
public:
    symbol left;
    std::vector<symbol> right_list;
    unsigned int order = 0;

    bool operator<(const generator& a) const
    {
        return this->order < a.order; //same order, same content
    }

    generator(symbol l, std::vector<symbol> r_list, unsigned int o) :left(l), right_list(r_list), order(o)
    {}
    generator(const generator& a) :left(a.left), right_list(a.right_list), order(a.order)
    {}
    generator()
    { }
};


class item  // point and prospect symbols are included
{
public:
    const generator base;
    unsigned int index;
    std::set<std::string> prospect_symbols;

    bool operator<(const item& a) const
    {
        if (this->base < a.base || a.base < this->base)
        {
            return this->base < a.base;
        }
        else if (this->index<a.index || this->index>a.index)
            return this->index < a.index;
        else
        {
            /*	std::set<std::string> tmp;
                std::set_intersection(this->prospect_symbols.begin(), this->prospect_symbols.begin(),
                    a.prospect_symbols.begin(), a.prospect_symbols.end(), std::inserter(tmp, tmp.begin()));
    */
            if (this->prospect_symbols.size() != a.prospect_symbols.size())
                return this->prospect_symbols.size() < a.prospect_symbols.size();

            if (this->prospect_symbols == a.prospect_symbols)
                return false;

            bool tag = false;//???
            auto ite1 = this->prospect_symbols.begin();
            auto ite2 = a.prospect_symbols.begin();

            for (; ite1 != this->prospect_symbols.end(); ite1++)
            {

                if (*ite1 < *ite2 || *ite2 < *ite1)
                    return *ite1 < *ite2;
                ite2++;
                //return false;

            }
            return tag;
            //return false;
        }

    }

    item(const generator& b, unsigned int i) :base(b), index(i) {}
    //item(const generator& b, unsigned int i,std::set<symbol>p) :base(b), index(i),prospect_symbols(p) {}
};

class item_group
{
public:
    int id;
    std::set<item> items;


    bool operator <(const item_group& a)const
    {
        if (this->items.size() != a.items.size())
            return items.size() < a.items.size();
        std::set<item>::iterator ite = this->items.begin();

        bool tag = false;//???


        for (auto const& i : a.items)
        {
            if (*ite < i || i < *ite)
                return *ite < i;
            ite++;
        }
        return tag;
    }

    item_group(int i) :id(i) {}

    item_group() { }



};
class word
{
public:
    int line;
    int id;
    std::string value;
    std::string realV;
    word(int line, std::string value, std::string realV, int id) {
        this->line = line;
        this->value = value;
        this->realV = realV;
        this->id=id;
    }
    void setID(int id) {
        this->id = id;
    }
    word(const word& c) {//���ƹ��캯��
        this->line = c.line;
        this->value = c.value;
        this->realV = c.realV;
        this->id=c.id;
    }
};

class Parser
{
public:

    void read_grammer(const std::string path);

    //	std::vector<std::vector<generator>> generators_list;
    void get_symbol_first(const symbol& a);
    void get_all_symbol_first();
    void get_closure(item_group& group);
    void get_sequence_first(const std::vector<std::string>& seq, std::vector<std::string>& re);
    void get_item_group_list();
    void item_group_go(const item_group& scr, item_group& dst, std::string input);
    void print_DFA();
    int check(const std::string path);
    std::set<symbol> non_terminators;
    std::set<symbol> terminators;
    std::set<symbol> symbols;
    std::vector<std::vector<generator>> generators_list;  // each unit represent the generators whose left symbol is the same one, we set this structure to create first set conveniently
    std::vector<generator> pure_generator_list;    // contains every generators in order;

    std::stack<int> itemS;// item state
    std::stack<word> symbolS;//
    std::set<item_group> item_groups;
    //each map represents the actions an item_group will take meeting diffrent symbols
    //the vector means the different item_groups
    std::vector < std::map<std::string, int>> go_map;

    void get_LR1_table();
    void print_LR1_table();


private:

    unsigned int num_termi = 0;
    unsigned int num_nontermi = 0;
    std::vector < std::pair<std::map<std::string, int>, std::map<std::string, int>>> table;
    std::vector< movement> LR1_table;

    //std::set<generator> generators;

    std::map<std::string, std::set<symbol>> symbol2first;


};

#endif // PARSER_H
