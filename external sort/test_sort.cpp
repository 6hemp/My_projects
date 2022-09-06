#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>

#include "sort.hpp"


class TestSort : public ::testing::Test
{
protected:
	virtual void SetUp(){}

	virtual void TearDown() {}
};


TEST_F(TestSort, test_is_sorted)
{
	std::ofstream out1("./output1", std::ios::binary);
	for (uint64_t i = 0; i < 1000; ++i)
		out1.write((char*)&i, sizeof(i));
	out1.close();
	ASSERT_TRUE(hemp::is_sorted("./output1"));
	std::remove("./output1");

	std::ofstream out2("./output2", std::ios::binary);
	for (uint64_t i = 0; i < 1000; i += 7)
		out2.write((char*)&i, sizeof(i));
	out2.close();
	ASSERT_TRUE(hemp::is_sorted("./output2"));
	std::remove("./output2");

	uint64_t val = 15;
	std::ofstream out3("./output3", std::ios::binary);
	for (uint64_t i = 0; i < 1000; ++i)
	out3.write((char*)&val, sizeof(val));
	out3.close();
	ASSERT_TRUE(hemp::is_sorted("./output3"));
	std::remove("./output3");

	std::ofstream out4("./output4", std::ios::binary);
	for (uint64_t i = 1000; i > 0; --i)
		out4.write((char*)&i, sizeof(i));
	out4.close();
	ASSERT_FALSE(hemp::is_sorted("./output4"));
	std::remove("./output4");

	std::ofstream out5("./output5", std::ios::binary);
	for (uint64_t i = 0; i < 1000; ++i)
		out5.write((char*)&i, sizeof(i));
	out5.write((char*)&val, sizeof(val));
	out5.close();
	ASSERT_FALSE(hemp::is_sorted("./output5"));
	std::remove("./output5");
}


TEST_F(TestSort, test_sort_back_order)
{
	std::ofstream out("./input_bo", std::ios::binary);
	for (uint64_t i = 1000; i > 10; i -= 3)
		out.write((char*)&i, sizeof(i));
	out.close();

	hemp::sort_file("./input_bo", "./output_bo");

	uint64_t num, val = 13;
	std::ifstream in("./output_bo", std::ios::binary);
	while(in.read((char*)&num, sizeof(num)))
	{
		ASSERT_EQ(num, val);
		val += 3;
	}
	ASSERT_EQ(val, 1003);
	in.close();

	std::remove("./input_bo");
	std::remove("./output_bo");
}


TEST_F(TestSort, test_sort_same)
{
	uint64_t val = 15;
	std::ofstream out("./input_s", std::ios::binary);
	for (uint64_t i = 0; i < 1000; ++i)
		out.write((char*)&val, sizeof(val));
	out.close();

	hemp::sort_file("./input_s", "./output_s");

	uint64_t num, size = 0;
	std::ifstream in("./output_s", std::ios::binary);
	while(in.read((char*)&num, sizeof(num)))
	{
		ASSERT_EQ(num, 15);
		++size;
	}
	ASSERT_EQ(size, 1000);
	in.close();

	std::remove("./input_s");
	std::remove("./output_s");
}


TEST_F(TestSort, test_sort_ordered)
{
	std::ofstream out("./input_o", std::ios::binary);
	for (uint64_t i = 10; i < 1000; i += 4)
	out.write((char*)&i, sizeof(i));
	out.close();

	hemp::sort_file("./input_o", "./output_o");

	uint64_t num, val = 10;
	std::ifstream in("./output_o", std::ios::binary);
	while(in.read((char*)&num, sizeof(num)))
	{
		ASSERT_EQ(num, val);
		val += 4;
	}
	ASSERT_EQ(val, 1002);
	in.close();

	std::remove("./input_o");
	std::remove("./output_o");
}


TEST_F(TestSort, test_sort_mixed)
{
	std::vector<uint64_t> vec;
	for (uint64_t i = 0; i < 1000; i += 7)
		vec.push_back(i);
	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(vec), std::end(vec), rng);

	std::ofstream out("./input_m", std::ios::binary);
	for (uint64_t i = 0; i < vec.size(); ++i)
		out.write((char*)&vec[i], sizeof(vec[i]));
	out.close();

	hemp::sort_file("./input_m", "./output_m");

	uint64_t num, val = 0;
	std::ifstream in("./output_m", std::ios::binary);
	while(in.read((char*)&num, sizeof(num)))
	{
		ASSERT_EQ(num, val);
		val += 7;
	}
	ASSERT_EQ(val, 1001);
	in.close();

	std::remove("./input_m");
	std::remove("./output_m");
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}