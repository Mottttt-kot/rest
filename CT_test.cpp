#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include "twomodule.h"
#include "rapidxml.hpp"
#include "linux/kernel.h"
#include "linux/module.h"
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

#define DEBUG
using namespace rapidxml;
using namespace std;
int main()
{
	long KBA1 = 0L;
	long KBA2 = 0L;
	long KBA3 = 0L;
	long KBA4 = 0L;
	long UST_overload = 0L;
	long UST_underloading = 0L;
	long UST = 0L;
	long RAB = 0L;
	long timer = 0L;

	map<string, long> map_k;
	map<string, bool> res;

	res["overload"] = false;
	res["underloading"] = false;
	res["two_overload"] = false;
	res["two_underloading"] = false;

	xml_document<> doc;
	xml_node<>* root_node = NULL;

	root_node = NULL;
	ifstream theFile("input_config.xml");
	if (theFile.is_open())
	{
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		doc.parse<0>(&buffer[0]);
		root_node = doc.first_node("MyData");

		for (xml_node<>* var_node = root_node->first_node("VAR");
			var_node; var_node = var_node->next_sibling())
		{
			for (xml_node<>* var_value_node = var_node->first_node("Value"); var_value_node; var_value_node = var_value_node->next_sibling())
			{
				string str = var_node->first_attribute("var_type")->value();

				if (str == "UST_overload")
				{
					UST_overload = stol(var_value_node->value());
				}
				else if (str == "UST_underloading")
				{
					UST_underloading = stol(var_value_node->value());
				}
				else if (str == "UST")
				{
					UST = stol(var_value_node->value());
				}
				else if (str == "RAB")
				{
					RAB = stol(var_value_node->value());
				}
				else if (str == "timer")
				{
					timer = stol(var_value_node->value());
				}

			}

		}
		buffer.clear();
		root_node->remove_all_nodes();
		doc.clear();
		doc.remove_all_attributes();
		doc.remove_all_nodes();
		theFile.close();
	}
	else
	{
		cout << "Error read file!" << endl;
		exit(-1);
	}
	while (true)
	{
		string key;
		string value;
		ifstream file("input.txt");
		while (file >> key >> value)
			map_k[key] = stol(value);
		algor(res, map_k, UST_overload, UST_underloading, UST, RAB);
#ifdef DEBUG
		cout << "###############################################################################" << endl;
		cout << "UST_overload: " << UST_overload << endl;
		cout << "UST_underloading: " << UST_underloading << endl;
		cout << "UST: " << UST << endl;
		cout << "RAB: " << RAB << endl;
		for (auto& itr : map_k)
		{
			cout << itr.first << " " << itr.second << endl;
		}

		for (auto& itr : res)
		{
			cout << itr.first << " " << itr.second << endl;
		}
		cout << "###############################################################################" << endl;
#endif // DEBUG

		std::this_thread::sleep_for(std::chrono::seconds(timer));
	}



}
