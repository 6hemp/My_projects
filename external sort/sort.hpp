#pragma once

#include <string>


namespace hemp
{
	void sort_file(const std::string & input_file, const std::string & output_file);

	bool is_sorted(const std::string & input_file);
}