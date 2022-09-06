#include <string>
#include <thread>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "sort.hpp"


namespace hemp
{
	namespace hemp_private
	{
		void _divide(const std::string & input_file, const std::string & output_file1, const std::string & output_file2, size_t series_size)
		{
			uint64_t num;
			size_t series_count = 0;
			bool eof = false;
			std::ifstream in(input_file, std::ios::binary);
			std::ofstream out1(output_file1, std::ios::binary);
			std::ofstream out2(output_file2, std::ios::binary);
			while(!eof)
			{
				if((series_count++) & 1)
				{
					for (size_t i = 0; i < series_size; ++i)
					{
						eof = !(in.read((char*)&num, sizeof(num)));
						if (eof)
							break;
						out2.write((char *) &num, sizeof(num));
					}
				}
				else
				{
					for (size_t i = 0; i < series_size; ++i)
					{
						eof = !(in.read((char*)&num, sizeof(num)));
						if (eof)
							break;
						out1.write((char *) &num, sizeof(num));
					}
				}
			}
			in.close();
			out1.close();
			out2.close();
		}

		void _merge(const std::string & input_file1, const std::string & input_file2, const std::string & output_file, size_t series_size)
		{
			uint64_t num1;
			uint64_t num2;
			bool eof1;
			bool eof2;
			std::ifstream in1(input_file1, std::ios::binary);
			std::ifstream in2(input_file2, std::ios::binary);
			std::ofstream out(output_file, std::ios::binary);

			eof1 = !(in1.read((char*)&num1, sizeof(num1)));
			eof2 = !(in2.read((char*)&num2, sizeof(num2)));

			while(!eof1 && !eof2)
			{
				size_t i1 = 0;
				size_t i2 = 0;

				while (i1 < series_size && i2 < series_size)
				{
					if (num1 <= num2)
					{
						out.write((char *) &num1, sizeof(num1));
						++i1;
						eof1 = !(in1.read((char*)&num1, sizeof(num1)));
						if (eof1)
							break;
					}
					else
					{
						out.write((char *) &num2, sizeof(num2));
						++i2;
						eof2 = !(in2.read((char*)&num2, sizeof(num2)));
						if (eof2)
							break;
					}
				}

				if (!eof1)
				{
					for (; i1 < series_size; ++i1)
					{
						out.write((char *) &num1, sizeof(num1));
						eof1 = !(in1.read((char *) &num1, sizeof(num1)));
						if (eof1)
							break;
					}
				}

				if (!eof2)
				{
					for (; i2 < series_size; ++i2)
					{
						out.write((char *) &num2, sizeof(num2));
						eof2 = !(in2.read((char *) &num2, sizeof(num2)));
						if (eof2)
							break;
					}
				}
			}

			if (!eof1)
			{
				out.write((char *) &num1, sizeof(num1));
				while(in1.read((char*)&num1, sizeof(num1)))
					out.write((char *) &num1, sizeof(num1));
			}
			if (!eof2)
			{
				out.write((char *) &num2, sizeof(num2));
				while(in2.read((char*)&num2, sizeof(num2)))
					out.write((char *) &num2, sizeof(num2));
			}

			in1.close();
			in2.close();
			out.close();
		}

		void _sort_file(const std::string & input_file, size_t size)
		{
			std::string output_file1;
			std::string output_file2;
			if (input_file[input_file.size() - 1] == '1')
			{
				output_file1 = "./output11";
				output_file2 = "./output12";
			}
			else
			{
				output_file1 = "./output21";
				output_file2 = "./output22";
			}

			size_t series_size = 1;
			while (series_size < size)
			{
				_divide(input_file, output_file1, output_file2, series_size);
				_merge(output_file1, output_file2, input_file, series_size);
				series_size <<= 1;
			}

			std::remove(output_file1.c_str());
			std::remove(output_file2.c_str());
		}
	}


	void sort_file(const std::string & input_file, const std::string & output_file)
	{
		uint64_t num;
		size_t size = 0;
		std::string input_file1 = "./input1";
		std::string input_file2 = "./input2";
		std::ifstream in(input_file, std::ios::binary);
		while(in.read((char*)&num, sizeof(num)))
			size++;
		in.close();

		hemp_private::_divide(input_file, input_file1, input_file2, 1);

		std::thread thread1(hemp_private::_sort_file, input_file1, (size >> 1) + (size & 1));
		std::thread thread2(hemp_private::_sort_file, input_file2, (size >> 1));
		thread1.join();
		thread2.join();

		hemp_private::_merge(input_file1, input_file2, output_file, size);

		std::remove(input_file1.c_str());
		std::remove(input_file2.c_str());
	}

	bool is_sorted(const std::string & input_file)
	{
		uint64_t cur, prev;
		std::ifstream in(input_file, std::ios::binary);
		if (!(in.read((char*)&prev, sizeof(prev))))
		{
			in.close();
			return true;
		}
		while(in.read((char*)&cur, sizeof(cur)))
		{
			if(cur < prev)
			{
				in.close();
				return false;
			}
			prev = cur;
		}
		in.close();
		return true;
	}
}