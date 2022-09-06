#include <gtest/gtest.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>

#include "bigint.hpp"


class TestBigInt : public ::testing::Test
{
public:
	bigint::BigInt n0;
	bigint::BigInt n_pos1;
	bigint::BigInt n_neg1;
	bigint::BigInt n_pos2;
	bigint::BigInt n_neg2;
	bigint::BigInt n_big1;
	bigint::BigInt n_big2;
protected:
	virtual void SetUp()
	{
		n0 = bigint::BigInt(0);
		n_pos1 = bigint::BigInt(300);
		n_neg1 = bigint::BigInt(-322);
		n_pos2 = bigint::BigInt(420);
		n_neg2 = bigint::BigInt(-666);
		n_big1 = bigint::BigInt("1234567890987654321234567890987654321");
		n_big2 = bigint::BigInt("27182818284590452353602874713526624977572");
	}

	virtual void TearDown() {}
};

TEST_F(TestBigInt, test_bigint_constructors)
{
	bigint::BigInt num1;
	ASSERT_EQ(num1, bigint::BigInt(0));

	bigint::BigInt num2(42);
	ASSERT_EQ(num2, bigint::BigInt(42));

	bigint::BigInt num3 = -69;
	ASSERT_EQ(num3, bigint::BigInt(-69));

	bigint::BigInt num4("1234567890");
	ASSERT_EQ(num4, bigint::BigInt(1234567890));

	bigint::BigInt num5("-0000000000");
	ASSERT_EQ(num5, bigint::BigInt(0));

	bigint::BigInt num6("-1234567890987654321234567890987654321");
	ASSERT_EQ(num6, bigint::BigInt("-1234567890987654321234567890987654321"));

	ASSERT_THROW(bigint::BigInt num("123a456"), std::invalid_argument);

	bigint::BigInt num7(num6);
	ASSERT_EQ(num7, num6);

	bigint::BigInt num8 = std::move(num7);
	ASSERT_NE(num7, num6);
	ASSERT_EQ(num8, num6);
}

TEST_F(TestBigInt, test_bigint_assign)
{
	bigint::BigInt num1 = 146;
	bigint::BigInt num2 = 228;
	num2 = num1;
	ASSERT_EQ(num1, num2);
	ASSERT_EQ(num1, bigint::BigInt(146));
}

TEST_F(TestBigInt, test_bigint_move)
{
	bigint::BigInt num1 = -280;
	bigint::BigInt num2 = 300;
	num2 = std::move(num1);
	ASSERT_NE(num1, bigint::BigInt(-280));
	ASSERT_EQ(num2, bigint::BigInt(-280));
}

TEST_F(TestBigInt, test_bigint_plus_bigint)
{
	ASSERT_EQ(n_pos1 + n_pos2, bigint::BigInt(720));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_pos1 + n_neg1, bigint::BigInt(-22));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 + n_pos2, bigint::BigInt(98));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_neg1 + n_neg2, bigint::BigInt(-988));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_neg2, bigint::BigInt(-666));

	ASSERT_EQ(n_pos1 + n0, n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 + n_pos1, n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 + n0, n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 + n_neg1, n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 + n0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 + n_big2, bigint::BigInt("27184052852481440007924109281417612631893"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
	ASSERT_EQ(n_big2, bigint::BigInt("27182818284590452353602874713526624977572"));
}

TEST_F(TestBigInt, test_bigint_plus_int)
{
	ASSERT_EQ(n_pos1 + 420, bigint::BigInt(720));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_pos1 + (-322), bigint::BigInt(-22));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_neg1 + 420, bigint::BigInt(98));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 + (-666), bigint::BigInt(-988));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_pos1 + 0, n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n0 + 300, n_pos1);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 + 0, n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n0 + (-322), n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 + 0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 + 300, bigint::BigInt("1234567890987654321234567890987654621"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
}

TEST_F(TestBigInt, test_bigint_minus_bigint)
{
	ASSERT_EQ(n_pos1 - n_pos2, bigint::BigInt(-120));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_pos1 - n_neg1, bigint::BigInt(622));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 - n_pos2, bigint::BigInt(-742));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_neg1 - n_neg2, bigint::BigInt(344));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_neg2, bigint::BigInt(-666));

	ASSERT_EQ(n_pos1 - n0, n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 - n_pos1, -n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 - n0, n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 - n_neg1, -n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 - n0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 - n_big2, bigint::BigInt("-27181583716699464699281640145635637323251"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
	ASSERT_EQ(n_big2, bigint::BigInt("27182818284590452353602874713526624977572"));
}

TEST_F(TestBigInt, test_bigint_minus_int)
{
	ASSERT_EQ(n_pos1 - 420, bigint::BigInt(-120));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_pos1 - (-322), bigint::BigInt(622));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_neg1 - 420, bigint::BigInt(-742));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 - (-666), bigint::BigInt(344));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_pos1 - 0, n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n0 - 300, -n_pos1);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 - 0, n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n0 - (-322), -n_neg1);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 - 0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 - 300, bigint::BigInt("1234567890987654321234567890987654021"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
}

TEST_F(TestBigInt, test_bigint_multiply_bigint)
{
	ASSERT_EQ(n_pos1 * n_pos2, bigint::BigInt(126000));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_pos1 * n_neg1, bigint::BigInt(-96600));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 * n_pos2, bigint::BigInt(-135240));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_pos2, bigint::BigInt(420));

	ASSERT_EQ(n_neg1 * n_neg2, bigint::BigInt(214452));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_neg2, bigint::BigInt(-666));

	ASSERT_EQ(n_pos1 * n0, n0);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * n_pos1, n0);
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 * n0, n0);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * n_neg1, n0);
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * n0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 * n_big2, bigint::BigInt("33559034640707482218442819271510448844682297872592307897258569670725513888612"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
	ASSERT_EQ(n_big2, bigint::BigInt("27182818284590452353602874713526624977572"));
}

TEST_F(TestBigInt, test_bigint_multiply_int)
{
	ASSERT_EQ(n_pos1 * 420, bigint::BigInt(126000));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_pos1 * (-322), bigint::BigInt(-96600));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));

	ASSERT_EQ(n_neg1 * 420, bigint::BigInt(-135240));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_neg1 * (-666), bigint::BigInt(214452));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));

	ASSERT_EQ(n_pos1 * 0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * 300, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_neg1 * 0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * (-322), n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n0 * 0, n0);
	ASSERT_EQ(n0, bigint::BigInt(0));

	ASSERT_EQ(n_big1 * 300, bigint::BigInt("370370367296296296370370367296296296300"));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
}

TEST_F(TestBigInt, test_bigint_unary_minus)
{
	ASSERT_EQ(-n0, bigint::BigInt(0));
	ASSERT_EQ(-n_pos1, bigint::BigInt(-300));
	ASSERT_EQ(-n_neg1, bigint::BigInt(322));
	ASSERT_EQ(-n_big1, bigint::BigInt("-1234567890987654321234567890987654321"));

	ASSERT_EQ(n0, bigint::BigInt(0));
	ASSERT_EQ(n_pos1, bigint::BigInt(300));
	ASSERT_EQ(n_neg1, bigint::BigInt(-322));
	ASSERT_EQ(n_big1, bigint::BigInt("1234567890987654321234567890987654321"));
}

TEST_F(TestBigInt, test_bigint_less)
{
	ASSERT_TRUE(n_neg2 < n_neg1);
	ASSERT_TRUE(n_neg1 < n0);
	ASSERT_TRUE(n_neg1 < n_pos1);
	ASSERT_TRUE(n0 < n_pos1);
	ASSERT_TRUE(n_pos1 < n_pos2);
	ASSERT_TRUE(n_big1 < n_big2);

	ASSERT_FALSE(n_neg1 < n_neg2);
	ASSERT_FALSE(n0 < n_neg1);
	ASSERT_FALSE(n_pos1 < n_neg1);
	ASSERT_FALSE(n_pos1 < n0);
	ASSERT_FALSE(n_pos2 < n_pos1);
	ASSERT_FALSE(n_big2 < n_big1);

	ASSERT_FALSE(n_pos1 < n_pos1);
	ASSERT_FALSE(n_neg1 < n_neg1);
	ASSERT_FALSE(n0 < n0);
	ASSERT_FALSE(n_big2 < n_big2);
}

TEST_F(TestBigInt, test_bigint_less_equal)
{
	ASSERT_TRUE(n_neg2 <= n_neg1);
	ASSERT_TRUE(n_neg1 <= n0);
	ASSERT_TRUE(n_neg1 <= n_pos1);
	ASSERT_TRUE(n0 <= n_pos1);
	ASSERT_TRUE(n_pos1 <= n_pos2);
	ASSERT_TRUE(n_big1 <= n_big2);

	ASSERT_TRUE(n_pos1 <= n_pos1);
	ASSERT_TRUE(n_neg1 <= n_neg1);
	ASSERT_TRUE(n0 <= n0);
	ASSERT_TRUE(n_big2 <= n_big2);

	ASSERT_FALSE(n_neg1 <= n_neg2);
	ASSERT_FALSE(n0 <= n_neg1);
	ASSERT_FALSE(n_pos1 <= n_neg1);
	ASSERT_FALSE(n_pos1 <= n0);
	ASSERT_FALSE(n_pos2 <= n_pos1);
	ASSERT_FALSE(n_big2 <= n_big1);
}

TEST_F(TestBigInt, test_bigint_greater)
{
	ASSERT_TRUE(n_neg1 > n_neg2);
	ASSERT_TRUE(n0 > n_neg1);
	ASSERT_TRUE(n_pos1 > n_neg1);
	ASSERT_TRUE(n_pos1 > n0);
	ASSERT_TRUE(n_pos2 > n_pos1);
	ASSERT_TRUE(n_big2 > n_big1);

	ASSERT_FALSE(n_neg2 > n_neg1);
	ASSERT_FALSE(n_neg1 > n0);
	ASSERT_FALSE(n_neg1 > n_pos1);
	ASSERT_FALSE(n0 > n_pos1);
	ASSERT_FALSE(n_pos1 > n_pos2);
	ASSERT_FALSE(n_big1 > n_big2);

	ASSERT_FALSE(n_pos1 > n_pos1);
	ASSERT_FALSE(n_neg1 > n_neg1);
	ASSERT_FALSE(n0 > n0);
	ASSERT_FALSE(n_big2 > n_big2);
}

TEST_F(TestBigInt, test_bigint_greater_equal)
{
	ASSERT_TRUE(n_neg1 >= n_neg2);
	ASSERT_TRUE(n0 >= n_neg1);
	ASSERT_TRUE(n_pos1 >= n_neg1);
	ASSERT_TRUE(n_pos1 >= n0);
	ASSERT_TRUE(n_pos2 >= n_pos1);
	ASSERT_TRUE(n_big2 >= n_big1);
	
	ASSERT_TRUE(n_pos1 >= n_pos1);
	ASSERT_TRUE(n_neg1 >= n_neg1);
	ASSERT_TRUE(n0 >= n0);
	ASSERT_TRUE(n_big2 >= n_big2);

	ASSERT_FALSE(n_neg2 >= n_neg1);
	ASSERT_FALSE(n_neg1 >= n0);
	ASSERT_FALSE(n_neg1 >= n_pos1);
	ASSERT_FALSE(n0 >= n_pos1);
	ASSERT_FALSE(n_pos1 >= n_pos2);
	ASSERT_FALSE(n_big1 >= n_big2);
}

TEST_F(TestBigInt, test_bigint_equal)
{
	ASSERT_TRUE(n_pos1 == n_pos1);
	ASSERT_TRUE(n_neg1 == n_neg1);
	ASSERT_TRUE(n0 == n0);
	ASSERT_TRUE(n_big2 == n_big2);

	ASSERT_FALSE(n0 == n_pos1);
	ASSERT_FALSE(n0 == n_neg1);
	ASSERT_FALSE(n_pos1 == n_neg1);
	ASSERT_FALSE(n_big1 == n_big2);
}

TEST_F(TestBigInt, test_bigint_not_equal)
{
	ASSERT_TRUE(n0 != n_pos1);
	ASSERT_TRUE(n0 != n_neg1);
	ASSERT_TRUE(n_pos1 != n_neg1);
	ASSERT_TRUE(n_big1 != n_big2);

	ASSERT_FALSE(n_pos1 != n_pos1);
	ASSERT_FALSE(n_neg1 != n_neg1);
	ASSERT_FALSE(n0 != n0);
	ASSERT_FALSE(n_big2 != n_big2);
}

TEST_F(TestBigInt, test_bigint_output)
{
	std::ostringstream oss;
	oss << n0;
	ASSERT_EQ(oss.str(), "0");

	oss.str(std::string());
	oss << n_pos1;
	ASSERT_EQ(oss.str(), "300");

	oss.str(std::string());
	oss << n_neg1;
	ASSERT_EQ(oss.str(), "-322");

	oss.str(std::string());
	oss << n_big1;
	ASSERT_EQ(oss.str(), "1234567890987654321234567890987654321");

	oss.str(std::string());
	oss << -(n_big1 * n_big2 + 3);
	ASSERT_EQ(oss.str(), "-33559034640707482218442819271510448844682297872592307897258569670725513888615");
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}