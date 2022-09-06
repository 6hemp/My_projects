#include <stdexcept>

#include "format.hpp"


namespace hemp
{
	invalid_string_format::invalid_string_format(const std::string & message) : std::invalid_argument(message){}


	non_streamable_param::non_streamable_param(const std::string & message) : std::invalid_argument(message){}


	invalid_params_count::invalid_params_count(const std::string & message) : std::out_of_range(message){}


	std::string format(const std::string & str)
	{
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (str[i] == '}')
				throw invalid_string_format("unexpected bracket");
			if (str[i] == '{')
			{
				size_t j = i + 1;
				for (; j < str.length() && str[j] != '}'; ++j)
					if (str[j] < '0' || str[j] > '9')
						throw invalid_string_format("unexpected bracket");
				if (j < str.length() && j != i + 1)
					throw invalid_params_count("missing parameters");
				throw invalid_string_format("unexpected bracket");
			}
		}
		std::string ans = str;
		return ans;
	}
}