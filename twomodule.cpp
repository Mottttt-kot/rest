#include "twomodule.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <fstream>
#define DEBUG

using namespace std;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//- флаг перегруз (при первом выполнении условия для такого сигнала)
//- флаг недогруз(при первом выполнении условия для такого сигнала)
//- сигнал перегруз(после появления флага выждать еще 10 мин, проверить, что условия все еще выполняются и тогда выдать сигнал)
//- сигнал недогруз(после появления флага выждать еще 10 мин, проверить, что условия все еще выполняются и тогда выдать сигнал)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//- Потребление электроэнергии по КВА 1 (исходно взять значение 100)
//-Потребление электроэнергии по КВА 2 (исходно взять значение 200)
//- Потребление электроэнергии по КВА 3 (исходно взять значение 100)
//- Потребление электроэнергии по КВА 4 (исходно взять значение 300)
//- UST перегр. = 132
//- UST недогр. = 114
//- UST погр. = 18
//- RAB норм. = 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void write_in_file(map<string, bool>& result)
{
	ofstream out("result.txt");
	if (out.is_open())
	{
		out << "#######################################" << endl;
		for (auto& itr : result)
		{
			out << itr.first << " " << itr.second << std::endl;
		}
		out << "#######################################" << endl;
	}
	out.close();

}
void clean_flag(map<string, bool>& result)
{
	for (auto& itr : result)
	{
		itr.second = false;
	}
}
void algor(map<string, bool>& result, map<string, long>& map_k, const long UST_overload,
	const long UST_underloading, const long UST, const long RABB_n) {
	long long sum_K = accumulate(begin(map_k), end(map_k), 0L,
		[](const long previous, const pair<const string, long>& p) { return previous + p.second; });

	map<string, long>::iterator best = max_element(begin(map_k), end(map_k),
		[](const pair<const string, long>& a, const pair<const string, long>& b)->bool { return a.second < b.second; });
	long max_k = best->second;
	int RABB = 0;
#ifdef DEBUG
	cout << "###############################################################################" << endl;
	cout << "MAX K: " << max_k << endl << "SUM K: " << sum_K << endl;
	cout << "###############################################################################" << endl;
#endif // DEBUG
	if (max_k > UST)
	{
		RABB = count_if(begin(map_k), end(map_k), [&max_k](const pair<string, long>& a)
			{
				return a.second > (max_k / 2);
			});
		if (max_k > UST_overload)
		{
			if (result["overload"])
			{
				result["two_overload"] = true;
			}
			result["overload"] = true;
			write_in_file(result);
			return;
		}
		else
		{
			if (RABB == 1)
			{
				clean_flag(result);
				write_in_file(result);
				return;
			}
			else if (RABB >= 2)
			{
				if (RABB == RABB_n)
				{
					clean_flag(result);
					write_in_file(result);
					return;
				}
				else
				{
					if ((sum_K / (RABB - 1)) > UST_underloading)
					{
						clean_flag(result);
						write_in_file(result);
						return;
					}
					else
					{
						if (result["underloading"])
						{
							result["two_underloading"] = true;
						}
						result["underloading"] = true;
					}
				}
			}
		}

	}
	else 
	{
		clean_flag(result);
	}
	write_in_file(result);

}