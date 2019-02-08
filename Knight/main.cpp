#include <utility>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
// field position
using pos = pair<char, int>; 

// validation
bool valid_position(pos next)
{
	return (next.first < 'a' || next.first > 'h'
		|| next.second < 1 || next.second > 8) ? false : true;
}

// generator of position sequence 
string test_generator(char& lit, int& num)
{
	if (!valid_position(pos{ lit, num })) { num = 1; lit = 'a'; }
	if (num == 9)
	{
		num = 1;
		lit++;
	}
	return std::string(1, lit) + (char)(num++ + '0');
}



int knight(std::string start, std::string finish) 
{
	cout << start << '\t' << finish << '\n';
	if (start == finish) return 0;

	pos cur_pos{ start[0], atoi(&start[1]) },
					fin_pos{finish[0], atoi(&finish[1])};

	auto dif_x = fin_pos.first - cur_pos.first; // horizontal difference
	auto dif_y = fin_pos.second - cur_pos.second; // vertical difference

	int numberOfSteps{ 0 }; // steps number
	

	while (1)
	{
		int newDif_x, newDif_y;
		int min_dif{ _MAX_INT_DIG };
		vector<pos>* next_pos = new vector<pos>; // all possible positions
		vector<pos>* golden_pos = new vector<pos>; // "golden" positions

		// filling the vector of possible positions
		if (valid_position({ cur_pos.first + 2, cur_pos.second + 1 }))
			next_pos->push_back(pos{ cur_pos.first + 2, cur_pos.second + 1 });

		if (valid_position({ cur_pos.first + 2, cur_pos.second - 1 }))
			next_pos->push_back(pos{ cur_pos.first + 2, cur_pos.second - 1 });

		if (valid_position({ cur_pos.first - 2, cur_pos.second + 1 }))
			next_pos->push_back(pos{ cur_pos.first - 2, cur_pos.second + 1 });

		if (valid_position({ cur_pos.first - 2, cur_pos.second - 1 }))
			next_pos->push_back(pos{ cur_pos.first - 2, cur_pos.second - 1 });

		if (valid_position({ cur_pos.first + 1, cur_pos.second + 2 }))
			next_pos->push_back(pos{ cur_pos.first + 1, cur_pos.second + 2 });

		if (valid_position({ cur_pos.first + 1, cur_pos.second - 2 }))
			next_pos->push_back(pos{ cur_pos.first + 1, cur_pos.second - 2 });

		if (valid_position({ cur_pos.first - 1, cur_pos.second + 2 }))
			next_pos->push_back(pos{ cur_pos.first - 1, cur_pos.second + 2 });

		if (valid_position({ cur_pos.first - 1, cur_pos.second - 2 }))
			next_pos->push_back(pos{ cur_pos.first - 1, cur_pos.second - 2 });

		// searching for the "golde" positions
		for (auto& i : *next_pos)
		{
			if (i == fin_pos) return ++numberOfSteps;
			newDif_x = fin_pos.first - i.first; newDif_y = fin_pos.second - i.second;
			if (newDif_y != 0 && (abs((double)newDif_x / newDif_y) == 2 || abs((double)newDif_x / newDif_y) == 0.5))
				golden_pos->emplace_back(i);
		}

		// searching for the "golden" position with min difference 
		if (!golden_pos->empty())
			for (auto& i : *golden_pos)
			{
				min_dif = (abs(fin_pos.first - i.first) > abs(fin_pos.second - i.second)) ?
					fin_pos.second - i.second : fin_pos.first - i.first;
			}

		if (min_dif != _MAX_INT_DIG)
			return 1 + abs(min_dif) + numberOfSteps;

		min_dif = abs(fin_pos.first - next_pos->at(0).first) + abs(fin_pos.second - next_pos->at(0).second); // summ of positions difference
		int min_idx = 0; // min difference index

		// step to the minimum difference position
		for (auto i = next_pos->begin() + 1; i != next_pos->end(); i++)
		{
			if (abs(fin_pos.first - (*i).first) + abs(fin_pos.second + (*i).second) < min_dif)
			{
				min_dif = abs(fin_pos.first - (*i).first) + abs(fin_pos.second + (*i).second);
				min_idx = i - next_pos->begin();
			}
			cur_pos = next_pos->at(min_idx);
		}

		numberOfSteps++; // increasing the result

		delete next_pos;
		delete golden_pos;
	}

	return 0;
}

int main()
{
	char lit{ 'a' }; int num{ 1 };
	while (1)
	{
		std::cout << knight(test_generator(lit, num), "h8") << endl;
	}
}