#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <iterator>
#include <memory>
#include <initializer_list>

#include "vect.hpp"


class Person
{
	std::string name;
	size_t age;

public:
	Person(){}

	Person(std::string name, size_t age) : name(name), age(age) {}

	Person(const Person & other) : name(other.name), age(other.age) {}

	Person & operator=(const Person & other)
	{
		name = other.name;
		age = other.age;
		return *this;
	}

	bool operator==(const Person & other) const
	{
		return other.name == name && other.age == age;
	}

	bool operator!=(const Person & other) const
	{
		return other.name != name || other.age != age;
	}

	~Person(){}
};


class TestVect : public ::testing::Test
{
protected:
	virtual void SetUp(){}

	virtual void TearDown() {}
};


TEST_F(TestVect, test_vect_ctr)
{
	ASSERT_NO_THROW(hemp::Vector<size_t> v1{});
	ASSERT_NO_THROW(hemp::Vector<std::string> v2(10));
	ASSERT_NO_THROW(hemp::Vector<char> v3(10, 'a'));
	ASSERT_NO_THROW(hemp::Vector<int> v4({5, 10, 2, 7}));
	hemp::Vector<int> v4({5, 10, 2, 7});
	ASSERT_NO_THROW(hemp::Vector<int> v5 = v4);
}


TEST_F(TestVect, test_vect_assign)
{
	hemp::Vector<int> v1 = {1, 2, 3, 4};
	hemp::Vector<int> v2(1);
	v2 = v1;
	ASSERT_EQ(v1.size(), v2.size());
	for (size_t i = 0; i < v1.size(); ++i)
		ASSERT_EQ(v1[i], v2[i]);
}


TEST_F(TestVect, test_vect_access)
{
	hemp::Vector<int> v1(10);
	for(size_t i = 0; i < v1.size(); ++i)
		v1[i] = i;
	for (size_t i = 0; i < v1.size(); ++i)
		ASSERT_EQ(v1[i], i);
}


TEST_F(TestVect, test_vect_push_back)
{
	hemp::Vector<int> v1(10);
	for(size_t i = 0; i < 5; ++i)
		v1.push_back(i);
	for (size_t i = 0; i < 5; ++i)
		ASSERT_EQ(v1[i], i);
	ASSERT_EQ(v1.size(), 5);

	hemp::Vector<int> v2(10);
	for(size_t i = 0; i < 15; ++i)
		v2.push_back(-i);
	for (size_t i = 0; i < 15; ++i)
		ASSERT_EQ(v2[i], -i);
	ASSERT_EQ(v2.size(), 15);
}


TEST_F(TestVect, test_vect_pop_back)
{
	hemp::Vector<unsigned> v1(10);
	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);
	for (size_t i = 0; i < 15; ++i)
		ASSERT_EQ(v1[i], i);
	ASSERT_EQ(v1.size(), 15);

	for(size_t i = 0; i < 5; ++i)
		v1.pop_back();
	ASSERT_EQ(v1.size(), 10);
}


TEST_F(TestVect, test_vect_emplace_back)
{
	hemp::Vector<Person> v1(10);
	//ASSERT_EQ(sizeof("azaza"), 6);
	for(size_t i = 0; i < 15; ++i)
		v1.emplace_back("azaza", i);
	for (size_t i = 0; i < 15; ++i)
		ASSERT_EQ(v1[i], Person("azaza", i));
	ASSERT_EQ(v1.size(), 15);
}


TEST_F(TestVect, test_vect_empty)
{
	hemp::Vector<unsigned> v1(10);
	ASSERT_TRUE(v1.empty());

	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);
	ASSERT_FALSE(v1.empty());

	for(size_t i = 0; i < 15; ++i)
		v1.pop_back();
	ASSERT_TRUE(v1.empty());
}


TEST_F(TestVect, test_vect_size)
{
	hemp::Vector<unsigned> v1(10, 0);
	ASSERT_EQ(v1.size(), 10);
	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);
	ASSERT_EQ(v1.size(), 25);

	for(size_t i = 0; i < 5; ++i)
		v1.pop_back();
	ASSERT_EQ(v1.size(), 20);
}


TEST_F(TestVect, test_vect_clear)
{
	hemp::Vector<unsigned> v1(10);
	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);
	ASSERT_EQ(v1.size(), 15);
	ASSERT_EQ(v1.capacity(), 16);

	v1.clear();
	ASSERT_EQ(v1.size(), 0);
	ASSERT_EQ(v1.capacity(), 0);

	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);
	ASSERT_EQ(v1.size(), 15);
	ASSERT_EQ(v1.capacity(), 16);
}


TEST_F(TestVect, test_vect_it)
{
	hemp::Vector<unsigned> v1(10);
	for(size_t i = 0; i < 15; ++i)
		v1.push_back(i);

	auto v_begin = v1.begin();
	auto v_rbegin = v1.rbegin();
	auto v_end = v1.end();
	auto v_rend = v1.end();

	size_t i = 0;
	for (auto it = v_begin; it < v_end; ++it)
		ASSERT_EQ(*it, i++);

	i = 14;
	for (auto it = v_rbegin; it > v_rend; --it)
		ASSERT_EQ(*it, i--);
}


TEST_F(TestVect, test_vect_resize)
{
	hemp::Vector<signed> v1{};
	v1.resize(10);
	ASSERT_EQ(v1.capacity(), 16);
	ASSERT_EQ(v1.size(), 0);

	for(size_t i = 0; i < 25; ++i)
		v1.push_back(i);
	ASSERT_EQ(v1.capacity(), 32);
	ASSERT_EQ(v1.size(), 25);

	v1.resize(40);
	ASSERT_EQ(v1.capacity(), 64);
	ASSERT_EQ(v1.size(), 25);

	v1.resize(10);
	ASSERT_EQ(v1.capacity(), 16);
	ASSERT_EQ(v1.size(), 10);
	for (size_t i = 0; i < 10; ++i)
		ASSERT_EQ(v1[i], i);
}


TEST_F(TestVect, test_vect_reserve)
{
	hemp::Vector<short> v1(10);
	ASSERT_EQ(v1.capacity(), 16);

	v1.reserve(10);
	ASSERT_EQ(v1.capacity(), 32);

	v1.reserve(20);
	ASSERT_EQ(v1.capacity(), 64);
}


TEST_F(TestVect, test_vect_capacity)
{
	hemp::Vector<short> v1(10);
	ASSERT_EQ(v1.capacity(), 16);
	for(size_t i = 0; i < 25; ++i)
		v1.push_back(i);
	ASSERT_EQ(v1.capacity(), 32);

	for(size_t i = 0; i < 20; ++i)
		v1.pop_back();
	ASSERT_EQ(v1.capacity(), 16);
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}