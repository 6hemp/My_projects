#pragma once

#include <stdexcept>
#include <string>


namespace hemp
{
	class invalid_string_format : public std::invalid_argument
	{
	public:
		explicit invalid_string_format(const std::string & message);
	};


	class non_streamable_param : public std::invalid_argument
	{
	public:
		explicit non_streamable_param(const std::string & message);
	};


	class invalid_params_count : public std::out_of_range
	{
	public:
		explicit invalid_params_count(const std::string & message);
	};


	std::string format(const std::string & str);


	template<typename... Types>
	std::string format(const std::string & str, Types... args);
}
#include "format.tpp"