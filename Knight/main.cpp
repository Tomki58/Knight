#include <utility>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>


using namespace std;
// position
using pos = pair<char, int>; 

// position validation
bool valid_position(pos next)
{
	return (next.first < 'a' || next.first > 'h'
		|| next.second < 1 || next.second > 8) ? false : true;
}

// test generator
string test_generator_1(char& lit, int& num)
{
	if (!valid_position(pos{ lit, num })) { num = 1; lit = 'a'; }
	if (num == 9)
	{
		num = 1;
		lit++;
	}
	return std::string(1, lit) + (char)(num++ + '0');
}

string test_generator_2(char& lit, int& num, int& counter)
{
	if (counter == 65) 
	{
		num++; counter = 1;
	}
	if (num == 9)
	{
		num = 1;
		lit++;
	}
	counter++;
	return std::string(1, lit) + (char)(num + '0');
}
// check position in cache
bool check_pos(pos& cur, vector<pos> cache)
{
	auto i = cache.begin();
	for (; i != cache.end() && cur != *i; i++);
	return (i == cache.end()) ? true : false;
}



int knight(string start, string finish, vector<pos>& cache) 
{
	cout << start << '\t' << finish << endl;

	if (start == finish) return 0;

	pos cur_pos = { start[0], atoi(&start[1]) },
		fin_pos = { finish[0], atoi(&finish[1]) };

	auto dif_x = fin_pos.first - cur_pos.first; // horizontal difference
	auto dif_y = fin_pos.second - cur_pos.second; // vertical difference

	int numberOfSteps{ 0 },// step num
		minSteps{ 32 }; // min step num
	

	while (1)
	{
		int newDif_x, newDif_y;
		int min_dif{ _MAX_INT_DIG };
		vector<pos>* next_pos = new vector<pos>; // all possible pos
		vector<pos>* golden_pos = new vector<pos>; // golden pos
		vector<pos>  same_val_pos; // pos with the same dist 

		same_val_pos.clear(); // clearing the same_val


		// filling the vector with possible poses
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

		// filling the golden pos vector
		for (auto& i : *next_pos)
		{
			if (i == fin_pos) return ++numberOfSteps;
			newDif_x = fin_pos.first - i.first; newDif_y = fin_pos.second - i.second;
			if (newDif_y != 0 && (abs((double)newDif_x / newDif_y) == 2 || abs((double)newDif_x / newDif_y) == 0.5))
				golden_pos->emplace_back(i);
		}

		// checking the golden pos vector
		if (!golden_pos->empty())
		{
			int old_mid_diff;
			for (auto& i : *golden_pos)
			{
				old_mid_diff = (abs(fin_pos.first - i.first) > abs(fin_pos.second - i.second)) ?
					abs(fin_pos.second - i.second) : abs(fin_pos.first - i.first);
				if (min_dif > old_mid_diff) min_dif = old_mid_diff;
			}
		}

		if (min_dif != _MAX_INT_DIG)
			return 1 + abs(min_dif) + numberOfSteps;

		min_dif = abs(fin_pos.first - next_pos->at(0).first) + abs(fin_pos.second - next_pos->at(0).second); // сумма разниц координат
		int min_idx = 0; // minimum distance index
		int cnt_same_val = 0, idx_same_val = min_idx;
		same_val_pos.push_back(*next_pos->begin());

		// turning to the pos with minimum distance
		for (auto i = next_pos->begin() + 1; i != next_pos->end(); i++)
		{
			if (abs(fin_pos.first - (*i).first) + abs(fin_pos.second - (*i).second) < min_dif
				&& check_pos(*i, cache))
			{
				min_dif = abs(fin_pos.first - (*i).first) + abs(fin_pos.second - (*i).second);
				min_idx = i - next_pos->begin();
				same_val_pos.clear(); same_val_pos.push_back(*i);
			} else
			if (abs(fin_pos.first - (*i).first) + abs(fin_pos.second - (*i).second) == min_dif
				&& check_pos(*i, cache) && i - next_pos->begin() != min_idx)
				same_val_pos.push_back(*i);
			cur_pos = next_pos->at(min_idx);
		}

		cache.push_back(cur_pos);

		numberOfSteps++; // incrementing the steps

		// checking the same_val_pos vector to call the function again
		if (!same_val_pos.empty())
		{
			for (auto& i : same_val_pos)
			{
				cache.push_back(i);
				int result = knight(string(1, i.first) + to_string(i.second), finish, cache);
				if (result < minSteps)
					minSteps = result;
			}
			return minSteps + numberOfSteps;
		}

		// clearing the containers
		next_pos->clear();
		golden_pos->clear();
	}

	return 0;
}

int main()
{
	char lit_1{ 'a' }; int num_1{ 1 }; // data for 1st generator
	char lit_2{ 'a' }; int num_2{ 1 }, counter{ 1 }; // data for 2nd generator
	vector<pos> cache;
	while (1)
	{
		cache.clear();
		cout << to_string(knight(test_generator_1(lit_1, num_1),
			test_generator_2(lit_2, num_2, counter), cache)) + "\n";
	}
}