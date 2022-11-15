#include "parser.h"
int Parser::check(const std::string path)
{
    //track the max number of stack
   // 1
    unsigned int max_num=0;

	int nodeID = 0;
	int eplisonID = 2147483647;
	std::fstream f;
	f.open(path, std::ios::in);
	std::fstream of;
	of.open("./resI.dot", std::ios::out);
    std::fstream tmp_file;
    tmp_file.open("stack.tmp",std::ios::out);

	while (!itemS.empty()) 
		itemS.pop();
	while (!symbolS.empty())
		symbolS.pop();
	/////////////////////////////////
	itemS.push(0);// I0 into stack
	symbolS.push(word(-1,"#","#", -1));
    //6
    tmp_file<<"p"<<" "<<0<<" "<<"#\n";

	of << "digraph G{\ngraph[dpi=300,autosize=false,size=\"200, 200\"];\noverlap=false; \nspines=true;\nnode[ shape=\"box\"];\n";

	while (f.eof() == false)
	{
		bool findFLAG = false, R_flag = false;
		unsigned int pos2 = 0; 
		unsigned int pos,pos3;
		std::string tmp;

		getline(f, tmp);
		pos = tmp.find_first_of(' ');
		pos2 = tmp.find_last_of(' ');
		pos3 = tmp.find_first_of(' ',pos+1);

		word inputw(atoi(tmp.substr(0, pos - 0).c_str()), tmp.substr(pos3+1,pos2-pos3-1), tmp.substr(pos2 + 1, tmp.size() - pos2 - 1), nodeID);
		
        tmp_file<<"@ "<<inputw.value<<"\n";
		int topState = itemS.top();

		action_struct* a_ptr = LR1_table[topState].action_ptr->next; 
		
		
        while (a_ptr)
        {
			R_flag = false;
            if (a_ptr->symbol_name == inputw.value)

            {
                if (a_ptr->dst == ACC)
                {
					of << "}\n";

                    //4
                    tmp_file<<"b\n";
                    tmp_file<<"#\n";
                    return SYNTAX_SUCCESS;
				}	
				findFLAG = true;
                if (a_ptr->dst < 0)
                {//s
					itemS.push(-a_ptr->dst);
					nodeID++;
					inputw.setID(nodeID);
					symbolS.push(inputw);

                    //7
                    tmp_file<<"p "<<-a_ptr->dst<<" "<<inputw.value<<"\n";


                    //2
                    if(itemS.size()>max_num)
                        max_num=itemS.size();
					break;
				}
                else if (a_ptr->dst > 0)
                {//r
					
					

					//��Լ
					generator x=pure_generator_list[a_ptr->dst];
					
					/*-------------��ͼ------------*/
					//�󸸽��
					nodeID++;
					int fid=nodeID;
					word fword(LINEOFNONT, x.left.name, x.left.name, fid);
					of << "node" << nodeID << "[label=\"" << x.left.name << "\"]\n";
					/*-----------------------------*/
					if (x.right_list.at(0).name != "$") {
						int n = x.right_list.size();
						for (int i = 0; i < n; i++) {

							//���ӽ��
							word topW=symbolS.top();
							//std::cout << topW.id << "\n";
							if(topW.line!=LINEOFNONT)
								of << "node" << topW.id << "[label=\"" << topW.value<<"\\n"<<topW.realV << "\"]\n";
							of << "node" << fid << "->node" << topW.id << "\n";

							symbolS.pop();
							itemS.pop();

                            //8
                            tmp_file<<"b\n";
						}
					}
                    else
                    {
						eplisonID--;
						of << "node" << eplisonID << "[label=\"Eplison\"]\n";
						of << "node" << fid << "->node" << eplisonID << "\n";
					}/**/
					topState = itemS.top();//����ջ��
					symbolS.push(fword);
					
					//����goto��
					goto_struct* g_ptr = LR1_table[topState].goto_ptr->next;
                    while (g_ptr)
                    {
                        if (g_ptr->symbol_name == x.left.name)
                        {
							R_flag = true;
							itemS.push(g_ptr->dst);

                            //9
                            tmp_file<<"p "<<g_ptr->dst<<" "<<fword.value<<"\n";
                            //3

							break;
						}
						g_ptr = g_ptr->next;
					}
					if (R_flag == true) {
						topState = itemS.top();
						a_ptr = LR1_table[topState].action_ptr->next;
						continue;
					}
					break;
				}
			}
			a_ptr = a_ptr->next;
		}
		if (findFLAG == false&&R_flag==false) {
			//of << "}\n";

            ///5
            tmp_file.close();
			return inputw.line;//error
		}
	}

	
}
void Parser::read_grammer(const std::string path)
{
	std::fstream f;
	f.open(path, std::ios::in);
	std::fstream of,of2;
	of.open("./result1.txt", std::ios::out);
	of2.open("./generators.txt", std::ios::out);
	int generators_index = 0;  // be equivalent to 31,
	if (f.is_open())
	{
		std::string tmp;
		unsigned int start_pos = 0; //the start position of the substr;
		unsigned int pos;
		while (f.eof() == false)
		{
			getline(f, tmp);
			pos = tmp.find_first_of(':', 0);
			symbol new_symbol(tmp.substr(0, pos - 0), NON_TERMINATOR, generators_index++);
			num_nontermi++;

			//cram the new non-terminator into our set
			bool r = non_terminators.insert(new_symbol).second;
			if (!r)
			{
				//std::cout << "WARNING" << '\n';
				generators_index--;
			}
			symbols.insert(new_symbol);
			of << new_symbol.name << "\n";
		}
		f.close();
		of.close();
	}

	f.open(path, std::ios::in);
	of.open("./result2.txt", std::ios::out);
	if (f.is_open())
	{
		//in the second pass ,we take in the terminators ,simultaneously we construct generators,
		// besides, in this part we link every non_terminator to generators whose left symbol is current non_terminator 
		//the contradiction is that we want to get the generators by symbol itself, but the generators are changing,we solve it by overloading < between symbols

		generator new_generator;
		std::string tmp;
		unsigned int start_pos = 0;
		unsigned int pos;
		unsigned int order = 0;  // the order the current generator is .
		while (f.eof() == false)
		{

			getline(f, tmp);
			pos = tmp.find_first_of(':', 0);



			symbol new_nonterninator(tmp.substr(0, pos - 0), 1);
			auto ite2 = non_terminators.find(new_nonterninator);  //in virtue of the first pass, ite2 must be existed!
			new_nonterninator.generators_index = ite2->generators_index;
			new_generator.left = new_nonterninator;

			new_generator.right_list.clear();


			start_pos = pos + 1;
			while (pos + 1 < tmp.size())
			{
				start_pos = pos + 1;
				pos = tmp.find_first_of(' ', start_pos);
				symbol new_symbol(tmp.substr(start_pos, pos - start_pos), NON_TERMINATOR);

				if (non_terminators.find(new_symbol) == non_terminators.end())
				{
					new_symbol.type = TERMINATOR;
					num_termi++;
					terminators.insert(new_symbol);
					symbols.insert(new_symbol);
				}
				else
				{
					ite2 = non_terminators.find(new_symbol);
					new_symbol.generators_index = ite2->generators_index;
				}
				of << new_symbol.name << " ";

				new_generator.right_list.emplace_back(new_symbol);
			}
			of << '\n';

			//here we get a new generator, push it into generators_list;
			auto ite = non_terminators.find(new_nonterninator);
			if (ite != non_terminators.end())
			{
				new_generator.order = order;
				if (ite->generators_index >= generators_list.size()) //the generators_list of this non_terminators has been created
				{
					std::vector<generator> new_list;
					new_list.emplace_back(new_generator);
					generators_list.emplace_back(new_list);

				}
				else
				{
					generators_list.at(ite->generators_index).emplace_back(new_generator);
				}

				pure_generator_list.emplace_back(new_generator);// create this one for reduction procudure
			}
			++order;

		}
		f.close();
		of.close();
	}
	for (auto i : pure_generator_list) {
		of2 <<i.order<<" "<< i.left.name << "->";
		for (auto j : i.right_list) {
			of2 << j.name<<" ";
		}
		of2 << std::endl;
	}
}


/*
int main() //test main
{
	Parser parser;
	parser.read_grammer("SynGra.txt");

	//std::cout<<(symbol("a", 0, 1)==symbol("a", 1, 0));
	std::set<symbol> set1;
	set1.insert(symbol("a", 0, 1));
	bool r = set1.insert(symbol("a", 1, 0)).second;

	/*for (auto ite = parser.non_terminators.begin(); ite != parser.non_terminators.end(); ite++)
	{
		int index = ite->generators_index;
		std::cout << ite->name << " " << index << ":\n";


		for (auto ite1 = parser.generators_list.at(index).begin(); ite1 != parser.generators_list.at(index).end(); ite1++)
		{
			std::cout << ite1->left.name << " " << ite1->left.generators_index << " --> ";
			for (auto ite2 = ite1->right_list.begin(); ite2 != ite1->right_list.end(); ite2++)
			{
				std::cout << ite2->name << " " << ite2->generators_index << " ";
			}
			std::cout << '\n' << '\n';
		}

	}

	std::set<int> set3 = { 0,1,2,3 };
	std::set<int> set4 = { 3,4,5,6 };
	std::set<int> set5 = { 0,1,2,3 };
	//std::set_union(set3.begin(), set3.end(), set4.begin(), set4.end(), std::inserter(set3,set3.begin()));
	std::cout << (set3 == set5) << " fuck";*/

	/*
	parser.get_all_symbol_first();
	parser.get_item_group_list();

	parser.get_LR1_table();
	parser.print_LR1_table();

	/*for (auto i : parser.pure_generator_list)
	{
		std::cout << i.order << " : " << i.left.name << " --> ";
		for (auto j : i.right_list)
			std::cout << j.name << " ";
		std::cout << "\n";
	}*/
/*
	std::cout<<'\n'<<parser.check("token_result.txt");
	parser.print_DFA();
}*/


void Parser::get_symbol_first(const symbol& a)
{
	//if (a.name == "operator")
		//std::cout << "g";

	if (symbol2first.find(a.name) != symbol2first.end())
		return;

	//start establishing
	std::set<symbol> first_set;

	//if a is an terminator
	if (a.type == TERMINATOR)
	{
		first_set.insert(a);
		symbol2first.insert(std::make_pair(a.name, first_set));
		return;
	}



	int index = a.generators_index;
	// i is a generator

	bool have_epsilon = true;
	for (auto const& i : generators_list.at(index))
	{
		have_epsilon = true;
		for (auto sym : i.right_list)
		{
			if (sym.type == TERMINATOR)
			{
				have_epsilon = false;
				first_set.insert(sym);
				break;
			}
			//non-terminator
			if (symbol2first.find(sym.name) == symbol2first.end())
				get_symbol_first(sym);

			auto const& tmp = symbol2first.find(sym.name);  //tmp is an iterator of map
			if (tmp->second.find(symbol("$")) == tmp->second.end()) // the episilon is not in the first set of sym
			{
				have_epsilon = false;
				std::set_union(first_set.begin(), first_set.end(), tmp->second.begin(), tmp->second.end(), std::inserter(first_set, first_set.begin()));
				break;
			}
			else
			{
				std::set<symbol> temp = tmp->second;
				temp.erase(symbol("$", TERMINATOR, -1));  //ERASE THE EPISILON when merging the first set
				std::set_union(first_set.begin(), first_set.end(), temp.begin(), temp.end(), std::inserter(first_set, first_set.begin()));
			}

		}
		if (have_epsilon == true)
		{
			first_set.insert(symbol("$", TERMINATOR, -1));
		}
	}
	symbol2first.insert(std::make_pair(a.name, first_set));
	return;
}

void Parser::get_all_symbol_first()
{
	for (auto const& i : non_terminators)
		get_symbol_first(i);
	for (auto const& i : terminators)
		get_symbol_first(i);


	std::fstream of;
	of.open("./first_set.txt", std::ios::out);
	for (auto const& i : symbol2first)
	{
		of << "FIRST['" << i.first << "'] = { ";
		for (std::set<symbol>::iterator ite = i.second.begin(); ite != i.second.end(); ite++)
		{
			of << "'" << ite->name << "'" << " ";
		}
		of << "}\n";
	}

	of.close();
}

void Parser::get_closure(item_group& group)
{
	std::set<item> is_visited;
	std::set<item> is_unvisited;
	//std::vector<item> tmp;
	std::vector<std::string> tmp;
	std::vector<std::string> re;
	//std::vector<item> t;
	for (auto ite = group.items.begin(); ite != group.items.end(); ite++)
	{
		is_unvisited.insert(*ite);
	}
	while (is_unvisited.empty() == false)
	{

		tmp.clear();
		re.clear();
		std::set<item>::iterator ite = is_unvisited.begin();
		is_visited.insert(*ite);

		//ONLY when a non-terminator behind the point , we take actions
		if (ite->index < ite->base.right_list.size() && ite->base.right_list.at(ite->index).type == NON_TERMINATOR)
		{
			for (unsigned int i = ite->index + 1; i < ite->base.right_list.size(); i++)
				tmp.emplace_back(ite->base.right_list.at(i).name); // i is an index
			for (auto const& i : ite->prospect_symbols)
			{
				tmp.emplace_back(i);  // i is a string
				get_sequence_first(tmp, re);
				tmp.pop_back();
			}

			std::set<std::string> tails;
			for (auto i : re)
				tails.insert(i);

			symbol B_symbol = ite->base.right_list.at(ite->index); // in accordance with the mark in textbook
			int generators_index = non_terminators.find(B_symbol)->generators_index;
			for (const auto& i : generators_list.at(generators_index))
			{
				generator new_one(i);
				item new_item(new_one, 0);
				new_item.prospect_symbols = tails;

				if (is_visited.find(new_item) == is_visited.end())
					is_unvisited.insert(new_item);
			}

		}
		is_unvisited.erase(*ite);
	}

	std::set_union(group.items.begin(), group.items.end(), is_visited.begin(), is_visited.end(), std::inserter(group.items, group.items.begin()));

}

void Parser::get_sequence_first(const std::vector<std::string>& seq, std::vector<std::string>& re)
{
	for (auto const& i : seq)
	{
		std::set<symbol> tmp = this->symbol2first.find(i)->second;

		//no epsilon
		if (tmp.find(symbol("$", TERMINATOR, -1)) == tmp.end())
		{
			for (const auto& j : tmp)
				re.emplace_back(j.name);
			break;
		}
		for (const auto& j : tmp)
		{
			if (j.name != "$")
				re.emplace_back(j.name);  //no break here
		}

		//if(this->symbol2first.find(i)->second.find)
	}
}

void Parser::get_item_group_list()
{
	//special for "#"
	std::fstream of;
	of.open("item_group.txt", std::ios::out);
	std::set<symbol> tmp;
	tmp.insert(symbol("#", TERMINATOR, -1));
	symbol2first.insert(std::make_pair("#", tmp));

	//first item group
	item_group first_group(0);
	symbol first_symbol("sstart", NON_TERMINATOR);
	int index = non_terminators.find(first_symbol)->generators_index;
	item first_item(generators_list.at(index).at(0), 0);
	first_item.prospect_symbols.insert("#");
	first_group.items.insert(first_item);

	get_closure(first_group);
	item_groups.insert(first_group);

	std::map< std::string, int> firstmap;
	go_map.emplace_back(firstmap);

	std::queue<item_group> q;
	q.push(first_group);

	int id_track = 0;
	while (q.empty() == false)
	{
		item_group tmp1 = q.front();

		of << tmp1.id << " ";
		for (auto i : tmp1.items)
		{
			of << "(" << i.base.order << " , '" << i.base.left.name << " , " << "(";
			for (auto j : i.base.right_list)
				of << "'" << j.name << "',";
			of << ") " << i.index << " (";
			for (auto j : i.prospect_symbols)
				of << "'" << j << "'";
			of << ')' << ')' << ",  ";

		}
		of << "\n";
		//item_groups.insert(tmp1);  //!!!!!!!!!!!!!!!!
		q.pop();

		for (auto i : symbols)// non_terminators)// transmit to different sets
		{
			if (i.name == "$")
				continue;
			item_group new_one;
			item_group_go(tmp1, new_one, i.name); ///here we get a brand new group set .then we should figure out if it is existed;

			if (new_one.items.size() != 0 && item_groups.find(new_one) == item_groups.end())//a new set
			{

				new_one.id = ++id_track;

				/** here we link different item groups and restore them into the"go_map"**/
				std::map<std::string, int> new_map;
				go_map.emplace_back(new_map);
				go_map.at(tmp1.id).insert(std::make_pair(i.name, new_one.id)); // nobody will know what it means


				//std::cout << "c status number: " << id_track << "\n";
				item_groups.insert(new_one);
				q.push(new_one);
			}
			else
			{
				if (new_one.items.size() != 0)
				{
					int dst_id = item_groups.find(new_one)->id;
					go_map.at(tmp1.id).insert(std::make_pair(i.name, dst_id)); // nobody will know what it means *2
				}
			}
		}
	}
	of.close();


}

void Parser::item_group_go(const item_group& scr, item_group& dst, std::string input)
{
	std::vector<std::string> temp1;
	std::vector<std::string> temp2;
	std::set<std::string> s;
	for (const auto& i : scr.items)
	{
		temp1.clear();
		temp2.clear();
		s.clear();
		if (i.index < i.base.right_list.size() && input == i.base.right_list.at(i.index).name) //at least one symbol behind the point 
		{
			item tmp(i.base, i.index + 1);  // the point's position move 

			//for (unsigned int cnt = i.index + 2; cnt < i.base.right_list.size(); cnt++)
			//	temp1.emplace_back(i.base.right_list.at(cnt).name);
			for (auto j : i.prospect_symbols)
			{
				temp1.emplace_back(j);
				//get_sequence_first(temp1, temp2);
				//temp1.pop_back();
			}

			for (auto const& j : temp1)
				s.insert(j);
			tmp.prospect_symbols = s;// i.prospect_symbols;
			dst.items.insert(tmp);
		}
	}

	//here we derive items from dst's items itself
	get_closure(dst);
}

void Parser::get_LR1_table()
{
	for (const auto& item_group_unit : item_groups)
	{
		movement mov;
		LR1_table.emplace_back(mov);
	}

    //std::cout << "here " << LR1_table.size();


	for (const auto& item_group_unit : item_groups)
	{
		std::set<for_verify_LR1> s;

		int index = item_group_unit.id;
		for (const auto& item_unit : item_group_unit.items)
		{
			if (item_unit.index < item_unit.base.right_list.size())
			{

				symbol a_symbol = item_unit.base.right_list.at(item_unit.index);

				if (a_symbol.type == TERMINATOR)
				{
					auto ite = go_map.at(index).find(a_symbol.name);
					if (ite != go_map.at(index).end()) //we get it
					{
						int dst = ite->second * -1;

						auto  it = s.find(for_verify_LR1(index, a_symbol.name, dst));
						if (it == s.end())
						{
							s.insert(for_verify_LR1(index, a_symbol.name, dst));

							action_struct* new_action = new action_struct(a_symbol.name, dst);
							//link this new unit to the index-th position of LR1 map

							//insert into the head!
							action_struct* tmp = LR1_table.at(index).action_ptr->next;
							new_action->next = tmp;
							LR1_table.at(index).action_ptr->next = new_action;
						}
						else
						{
							if (it->dst != dst)
							{
								std::cerr << "THIS IS NOT LR1 GRAMMER!" << index << " " << a_symbol.name << "\n";
								break;
							}

						}





					}
				}
			}

			if (item_unit.base.left.name == "sstart" && item_unit.index == 1)
			{
				//here we judge the prospect symbols
				if (item_unit.prospect_symbols.size() == 1)
				{
					for (const auto& i : item_unit.prospect_symbols)
					{
						if (i == "#")
						{

							auto  it = s.find(for_verify_LR1(index, "#", ACC));
							if (it == s.end())
							{
								s.insert(for_verify_LR1(index, "#", ACC));

								action_struct* new_action = new action_struct("#", ACC);
								//link this new unit to the index-th position of LR1 map

								//insert into the head!
								action_struct* tmp = LR1_table.at(index).action_ptr->next;
								new_action->next = tmp;
								LR1_table.at(index).action_ptr->next = new_action;
							}
							else
							{
								if (it->dst != ACC)
								{
									std::cerr << "THIS IS NOT LR1 GRAMMER!" << index << " " << "#" << "\n";
									break;
								}

							}

							/*
							action_struct* new_action = new action_struct("#", ACC);// r project
							action_struct* tmp = LR1_table.at(index).action_ptr->next;
							new_action->next = tmp;
							LR1_table.at(index).action_ptr->next = new_action;

							/*const auto& it = s.find(for_verify_LR1(index, "#"));
							if (it == s.end())
								s.insert(*it);
							else
								std::cerr << "THIS IS NOT LR1 GRAMMER!\n";*/
						}
					}
				}
			}
			else
			{

				//r 2rd principle
				if (item_unit.index == item_unit.base.right_list.size())
				{
					if (item_unit.base.right_list.at(0).name != "$")
					{
						for (const auto& symbol_unit : item_unit.prospect_symbols)
						{

							auto  it = s.find(for_verify_LR1(index, symbol_unit, item_unit.base.order));
							if (it == s.end())
							{
								s.insert(for_verify_LR1(index, symbol_unit, item_unit.base.order));

								action_struct* new_action = new action_struct(symbol_unit, item_unit.base.order);
								//link this new unit to the index-th position of LR1 map

								//insert into the head!
								action_struct* tmp = LR1_table.at(index).action_ptr->next;
								new_action->next = tmp;
								LR1_table.at(index).action_ptr->next = new_action;
							}
							else
							{
								if (it->dst != item_unit.base.order)
								{
									std::cerr << "THIS IS NOT LR1 GRAMMER!" << index << " " << symbol_unit << "\n";
									break;
								}

							}
							/*
							action_struct* new_action = new action_struct(symbol_unit, item_unit.base.order);// r project
							action_struct* tmp = LR1_table.at(index).action_ptr->next;
							new_action->next = tmp;
							LR1_table.at(index).action_ptr->next = new_action;

							/*const auto& it = s.find(for_verify_LR1(index, symbol_unit));
							if (it == s.end())
								s.insert(*it);
							else
								std::cerr << "THIS IS NOT LR1 GRAMMER!\n";*/
						}
						/*
						for (const auto& symbol_unit : terminators)
						{
							action_struct* new_action = new action_struct(symbol_unit.name, item_unit.base.order);// r project
							action_struct* tmp = LR1_table.at(index).action_ptr->next;
							new_action->next = tmp;
							LR1_table.at(index).action_ptr->next = new_action;
						}*/
					}
				}
				// speciallly cope with $
				if (item_unit.base.right_list.at(0).name == "$")
				{
					for (const auto& symbol_unit : item_unit.prospect_symbols)
					{

						auto  it = s.find(for_verify_LR1(index, symbol_unit, item_unit.base.order));
						if (it == s.end())
						{
							s.insert(for_verify_LR1(index, symbol_unit, item_unit.base.order));

							action_struct* new_action = new action_struct(symbol_unit, item_unit.base.order);
							//link this new unit to the index-th position of LR1 map

							//insert into the head!
							action_struct* tmp = LR1_table.at(index).action_ptr->next;
							new_action->next = tmp;
							LR1_table.at(index).action_ptr->next = new_action;
						}
						else
						{
							if (it->dst != item_unit.base.order)
							{
								std::cerr << "THIS IS NOT LR1 GRAMMER!" << index << " " << symbol_unit << "\n";
								break;
							}

						}

						/*
						action_struct* new_action = new action_struct(symbol_unit, item_unit.base.order);// r project
						action_struct* tmp = LR1_table.at(index).action_ptr->next;
						new_action->next = tmp;
						LR1_table.at(index).action_ptr->next = new_action;

						/**const auto& it = s.find(for_verify_LR1(index, symbol_unit));
						if (it == s.end())
							s.insert(*it);
						else
							std::cerr << "THIS IS NOT LR1 GRAMMER!\n";*/
					}
				}
			}




			/*
			auto ite = go_map.at(index).find(item_unit.base.left.name);
			if (ite != go_map.at(index).end())
			{
				int dst = ite->second;
				goto_struct* new_goto = new goto_struct(item_unit.base.left.name,dst);// goto project
				goto_struct* tmp = LR1_table.at(index).goto_ptr->next;
				new_goto->next = tmp;
				LR1_table.at(index).goto_ptr->next = new_goto;

			}
			*/

		}
		for (const auto& i : non_terminators)
		{
			auto ite = go_map.at(index).find(i.name);
			if (ite != go_map.at(index).end())
			{


				int dst = ite->second;

				auto  it = s.find(for_verify_LR1(index, i.name, dst));
				if (it == s.end())
				{
					s.insert(for_verify_LR1(index, i.name, dst));

					goto_struct* new_goto = new goto_struct(i.name, dst);
					//link this new unit to the index-th position of LR1 map

					//insert into the head!
					goto_struct* tmp = LR1_table.at(index).goto_ptr->next;
					new_goto->next = tmp;
					LR1_table.at(index).goto_ptr->next = new_goto;
				}
				else
				{
					if (it->dst != dst)
					{
						std::cerr << "THIS IS NOT LR1 GRAMMER!" << index << " " << i.name << "\n";
						break;
					}

				}

				/*
				goto_struct* new_goto = new goto_struct(i.name, dst);// goto project
				goto_struct* tmp = LR1_table.at(index).goto_ptr->next;
				new_goto->next = tmp;
				LR1_table.at(index).goto_ptr->next = new_goto;
				*/

			}
		}
	}
}

void Parser::print_LR1_table()
{
	std::fstream of;
	of.open("./LR1_table.txt", std::ios::out);
	for (unsigned int i = 0; i < LR1_table.size(); i++)
	{
		// traverse 2 link lists
		of << std::setw(5) << i << "\n";
		of << "ACTION\n";
		action_struct* track = LR1_table.at(i).action_ptr->next;
		while (track != nullptr)
		{
			of << track->symbol_name << " ";
			if (track->dst < 0)//s
				of << "s" << -1 * track->dst;
			else
				of << "r" << 1 * track->dst;
			of << "  ||   ";
			track = track->next;
		}

		of << "\nGOTO\n";

		goto_struct* track2 = LR1_table.at(i).goto_ptr->next;
		while (track2 != nullptr)
		{
			of << track2->symbol_name << " ";
			//if (track->dst < 0)//s
				//of << "s" << -1 * track->dst;
			//else
			of << 1 * track2->dst;
			of << "  ||   ";
			track2 = track2->next;
		}
		of << "\n\n";
	}
}

void Parser::print_DFA()
{
	std::fstream of;
	of.open("./DFA.dot", std::ios::out);

	of << "digraph G{\n";

	of << "graph[dpi=500, autosize=false,size=\"150,150\"];\n";

	of << "overlap=false;\nspines=true;\n";

	of << "node [shape=box];\n";
	of << "edge[lblstyle = \"above, sloped\"];\n";

	for (const auto& unit : item_groups)
	{
		of << "node" << unit.id << "[label=\" ";

		//write the items into a node
		/*
		for (auto item_unit : unit.items)
		{
			of << item_unit.base.left.name << " -> ";
			for (unsigned int i = 0; i < item_unit.base.right_list.size(); i++)
			{
				if (i == item_unit.index)
					of << ".";
				if (item_unit.base.right_list.at(i).name != "$")
					of << item_unit.base.right_list.at(i).name;
			}
			of << "  ";

			//prospect symbol
			for (auto prospect_unit : item_unit.prospect_symbols)
				of << prospect_unit << " | ";
			of << "\\n";
		}
		*/
		of << unit.id;
		of << "\"]";
		of << "\n";
	}

	//here we start to output the relationships between item_groups
	of << "\n";

	int cnt = 0;
	for (const auto item_group_map : go_map)
	{
		for (const auto& go_relation : item_group_map)
		{
			std::string label = go_relation.first;
			of << "node" << cnt << "->" << "node" << go_relation.second << "\n ";// [label = \"" << label << "\",constraint=false]\n";
		}
	}
	of << "}";

	of.close();
}
