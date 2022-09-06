#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include "format.hpp"


namespace hemp
{
	namespace hemp_private
	{
		template<typename Type>
		void _get_params(std::vector <std::string> & vec, std::set<size_t> & nums, size_t depth, Type arg)
		{
			if (nums.contains(depth))
			{
				std::stringstream sst;
				sst << arg;
				vec.push_back(sst.str());
			}
			else
				vec.push_back("");
		}


		template<typename Type, typename... Types>
		void _get_params(std::vector <std::string> & vec, std::set<size_t> & nums, size_t depth, Type arg, Types... args)
		{
			if (nums.contains(depth))
			{
				std::stringstream sst;
				sst << arg;
				vec.push_back(sst.str());
			}
			else
				vec.push_back("");
			_get_params(vec, nums, depth + 1, args...);
		}


		template<typename... Types>
		std::ostream & operator<<(std::ostream &, Types...)
		{
			throw non_streamable_param("param is not streamable");
		}
	}


	template<typename... Types>
	std::string format(const std::string & str, Types... args)
	{
		size_t len = str.length();
		size_t min_param_count = 0;
		size_t stage = 0;
		size_t start_num;
		std::set<size_t> nums;
		for (size_t i = 0; i < len; ++i)
			switch (stage)
			{
				case 0:
					if (str[i] == '}')
						throw invalid_string_format("wrong bracket sequence");
					else if (str[i] == '{')
					{
						start_num = i + 1;
						stage = 1;
					}
					break;
				case 1:
					if (str[i] == '}')
					{
						if (i == start_num)
							throw invalid_string_format("no digit in brackets");
						size_t num = std::stoull(str.substr(start_num, i - start_num));
						nums.insert(num);
						if (num + 1 > min_param_count)
							min_param_count = num + 1;
						stage = 0;
					}
					else if ('0' > str[i] || str[i] > '9')
						throw invalid_string_format("expected not negative digit in brackets");
					break;
			}
		if (stage == 1)
			throw invalid_string_format("wrong bracket sequence");

		std::vector<std::string> params;
		hemp_private::_get_params(params, nums, 0, args...);
		if (params.size() < min_param_count)
			throw invalid_params_count("missing parameters");

		std::stringstream sst;
		for (size_t i = 0; i < len; ++i)
		{
			if (str[i] != '{')
			{
				sst << str[i];
				continue;
			}
			size_t j = i + 1;
			for (; str[j] != '}'; ++j);
			sst << params[std::stoull( str.substr(i + 1, j - i - 1) )];
			i = j;
		}

		return sst.str();
	}
}