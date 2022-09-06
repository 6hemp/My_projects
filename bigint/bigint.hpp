#pragma once

#include <string>
#include <stdint.h>
#include <iostream>


namespace bigint
{
	class BigInt
	{
	private:
		enum class Sign {Minus, Zero, Plus};
	public:
		BigInt();

		BigInt(int32_t number);

		BigInt(const std::string & number);

		BigInt(const BigInt & other);

		BigInt(BigInt && other);

		~BigInt();

		BigInt & operator=(const BigInt & other);

		BigInt & operator=(BigInt && other);

		BigInt operator+(const BigInt & other) const;

		BigInt operator+(const int32_t & other) const;

		BigInt operator-(const BigInt & other) const;

		BigInt operator-(const int32_t & other) const;

		BigInt operator*(const BigInt & other) const;

		BigInt operator*(const int32_t & other) const;

		BigInt operator-() const;

		bool operator<(const BigInt & other) const;

		bool operator<=(const BigInt & other) const;

		bool operator>(const BigInt & other) const;

		bool operator>=(const BigInt & other) const;

		bool operator==(const BigInt & other) const;

		bool operator!=(const BigInt & other) const;

		friend std::ostream & operator<<(std::ostream & os, const BigInt & num);

	private:
		static BigInt mulPow10(const BigInt & number, size_t pow);

	private:
		Sign sign_ = Sign::Zero;
		size_t size_ = 0;
		signed char * data_ = nullptr;
	};

	std::ostream & operator<<(std::ostream & os, const BigInt & num);
}