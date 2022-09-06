#include "bigint.hpp"

#include <string>
#include <stdint.h>
#include <iostream>


namespace bigint
{
	BigInt BigInt::mulPow10(const BigInt & number, size_t pow)
	{
		BigInt res;
		delete[] res.data_;
		res.sign_ = number.sign_;
		res.size_ = number.size_ + pow;
		res.data_ = new signed char[res.size_];
		for (size_t i = 0; i < pow; i++)
			res.data_[i] = 0;
		for (size_t i = 0; i < number.size_; i++)
			res.data_[i + pow] = number.data_[i];
		return res;
	}

	BigInt::BigInt()
	{
		sign_ = Sign::Zero;
		size_ = 1;
		data_ = new signed char[size_];
		data_[0] = 0;
	}

	BigInt::BigInt(int32_t number)
	{
		if (number > 0)
			sign_ = Sign::Plus;
		else if (number == 0)
			sign_ = Sign::Zero;
		else
			sign_ = Sign::Minus;

		int64_t num = abs(number);

		for (size_ = 0; num > 0; size_++)
			num /= 10;
		if (number == 0)
			size_ = 1;

		data_ = new signed char[size_];

		num = abs(number);
		for (size_t i = 0; i < size_; i++)
		{
			data_[i] = num % 10;
			num /= 10;
		}
	}

	BigInt::BigInt(const std::string & number)
	{
		if (number.length() == 0)
			throw std::invalid_argument("len must be positive");

		sign_ = Sign::Plus;

		size_t i = 0;
		if (number[0] == '-')
		{
			sign_ = Sign::Minus;
			i++;
		}

		for (size_t j = i; j < number.length(); j++)
		{
			if (number[j] < '0' || number[j] > '9')
			{
				sign_ = Sign::Zero;
				throw std::invalid_argument("number must consists of digits");
			}
		}

		for (; i < number.length() && number[i] == '0'; i++);

		if (i == number.length())
		{
			sign_ = Sign::Zero;
			i--;
		}

		size_ = number.length() - i;
		data_ = new signed char[size_];

		for (size_t j = 0; j < size_; j++)
			data_[j] = number[number.length() - j - 1] - '0';
	}

	BigInt::BigInt(const BigInt & other)
	{
		sign_ = other.sign_;
		size_ = other.size_;
		data_ = new signed char[size_];
		for (size_t i = 0; i < size_; i++)
			data_[i] = other.data_[i];
	}

	BigInt::BigInt(BigInt && other)
	{
		sign_ = other.sign_;
		size_ = other.size_;
		data_ = other.data_;

		other.sign_ = Sign::Zero;
		other.size_ = 0;
		other.data_ = nullptr;
	}

	BigInt::~BigInt()
	{
		if (data_)
			delete[] data_;
	}

	BigInt & BigInt::operator=(const BigInt & other)
	{
		if (this == &other)
			return *this;

		if (data_)
			delete[] data_;

		sign_ = other.sign_;
		size_ = other.size_;
		data_ = new signed char[size_];
		for (size_t i = 0; i < size_; i++)
			data_[i] = other.data_[i];

		return *this;
	}

	BigInt & BigInt::operator=(BigInt && other)
	{
		if (this == &other)
			return *this;

		if (data_)
			delete[] data_;

		sign_ = other.sign_;
		size_ = other.size_;
		data_ = other.data_;

		other.sign_ = Sign::Zero;
		other.size_ = 0;
		other.data_ = nullptr;

		return *this;
	}

	BigInt BigInt::operator+(const BigInt & other) const
	{
		if (sign_ == Sign::Minus && other.sign_ == Sign::Minus)
			return -(-(*this) + -(other));
		if (sign_ == Sign::Minus && other.sign_ == Sign::Zero)
			return *this;
		if (sign_ == Sign::Minus && other.sign_ == Sign::Plus)
			return -(-(*this) + -(other));

		if (sign_ == Sign::Zero)
			return other;

		if (sign_ == Sign::Plus && other.sign_ == Sign::Zero)
			return *this;

		if (sign_ == Sign::Plus && other.sign_ == Sign::Minus)
		{
			if (*this == -other)
			{
				BigInt res;
				return res;
			}

			signed char extra_dig = 0, sum_dig;
			BigInt max_n;
			BigInt min_n;
			if (*this < -other)
			{
				max_n = other;
				min_n = *this;
				for (size_t i = 0; i < max_n.size_; i++)
					max_n.data_[i] = -max_n.data_[i];
			}
			else
			{
				max_n = *this;
				min_n = other;
				for (size_t i = 0; i < min_n.size_; i++)
					min_n.data_[i] = -min_n.data_[i];
			}

			BigInt res;
			delete[] res.data_;
			if (*this < -other)
				res.sign_ = Sign::Minus;
			else
				res.sign_ = Sign::Plus;
			res.size_ = max_n.size_ + 1;
			res.data_ = new signed char[res.size_];
			for (size_t i = 0; i < res.size_; i++)
				res.data_[i] = 0;

			if (res.sign_ == Sign::Plus)
			{
				for (size_t i = 0; i < min_n.size_; i++)
				{
					sum_dig = max_n.data_[i] + min_n.data_[i] + extra_dig;
					if (sum_dig < 0)
					{
						extra_dig = -1;
						sum_dig = 10 + sum_dig;
						res.data_[i] = sum_dig;
					}
					else
					{
						extra_dig = sum_dig / 10;
						sum_dig %= 10;
						res.data_[i] = sum_dig;
					}
				}
				for (size_t i = min_n.size_; i < max_n.size_; i++)
				{
					sum_dig = max_n.data_[i] + extra_dig;
					if (sum_dig < 0)
					{
						extra_dig = -1;
						sum_dig = 10 + sum_dig;
						res.data_[i] = sum_dig;
					}
					else
					{
						extra_dig = sum_dig / 10;
						sum_dig %= 10;
						res.data_[i] = sum_dig;
					}
				}
				if (extra_dig < 0)
					res.data_[max_n.size_] = 10 + extra_dig;

				BigInt tmp(res);
				for (; res.size_ > 1 && res.data_[res.size_ - 1] == 0; res.size_--);
				delete[] res.data_;
				res.data_ = new signed char[res.size_];
				for (size_t i = 0; i < res.size_; i++)
					res.data_[i] = tmp.data_[i];

				return res;
			}
			else
				return -((-other) + (-(*this)));
		}

		signed char extra_dig = 0, sum_dig;
		BigInt max_n;
		BigInt min_n;
		if (*this < other)
		{
			max_n = other;
			min_n = *this;
		}
		else
		{
			max_n = *this;
			min_n = other;
		}

		BigInt res;
		delete[] res.data_;
		res.sign_ = Sign::Plus;
		res.size_ = max_n.size_ + 1;
		res.data_ = new signed char[res.size_];
		for (size_t i = 0; i < res.size_; i++)
			res.data_[i] = 0;

		for (size_t i = 0; i < min_n.size_; i++)
		{
			sum_dig = max_n.data_[i] + min_n.data_[i] + extra_dig;
			extra_dig = sum_dig / 10;
			sum_dig %= 10;
			res.data_[i] = sum_dig;
		}
		for (size_t i = min_n.size_; i < max_n.size_; i++)
		{
			sum_dig = max_n.data_[i] + extra_dig;
			extra_dig = sum_dig / 10;
			sum_dig %= 10;
			res.data_[i] = sum_dig;
		}
		res.data_[max_n.size_] = extra_dig;

		if (extra_dig == 0)
		{
			BigInt tmp(res);
			delete[] res.data_;
			res.size_--;
			res.data_ = new signed char[res.size_];
			for (size_t i = 0; i < res.size_; i++)
				res.data_[i] = tmp.data_[i];
		}

		return res;
	}

	BigInt BigInt::operator+(const int32_t & other) const
	{
		return *this + BigInt(other);
	}

	BigInt BigInt::operator-(const BigInt & other) const
	{
		return *this + (-other);
	}

	BigInt BigInt::operator-(const int32_t & other) const
	{
		return *this + (-BigInt(other));
	}

	BigInt BigInt::operator*(const BigInt & other) const
	{
		if (sign_ == Sign::Minus && other.sign_ == Sign::Minus)
			return (-(*this)) * (-other);
		if (sign_ == Sign::Minus && other.sign_ == Sign::Zero)
			return other;
		if (sign_ == Sign::Minus && other.sign_ == Sign::Plus)
			return -((-(*this)) * other);

		if (sign_ == Sign::Zero)
			return *this;

		if (sign_ == Sign::Plus && other.sign_ == Sign::Minus)
			return -((*this) * (-other));
		if (sign_ == Sign::Plus && other.sign_ == Sign::Zero)
			return other;

		BigInt res;
		for (size_t i = 0; i < size_; i++)
		{
			BigInt term;
			for (size_t j = 0; j < size_t(data_[i]); j++)
				term = term + other;
			res = res + BigInt::mulPow10(term, i);
		}

		return res;
	}

	BigInt BigInt::operator*(const int32_t & other) const
	{
		return *this * BigInt(other);
	}

	BigInt BigInt::operator-() const
	{
		BigInt res(*this);

		if (res.sign_ == Sign::Plus)
			res.sign_ = Sign::Minus;
		else if (res.sign_ == Sign::Minus)
			res.sign_ = Sign::Plus;

		return res;
	}

	bool BigInt::operator<(const BigInt & other) const
	{
		if (sign_ == Sign::Minus && other.sign_ == Sign::Minus)
			return -other < -(*this);
		if (sign_ == Sign::Minus && other.sign_ == Sign::Zero)
			return true;
		if (sign_ == Sign::Minus && other.sign_ == Sign::Plus)
			return true;

		if (sign_ == Sign::Zero && other.sign_ == Sign::Minus)
			return false;
		if (sign_ == Sign::Zero && other.sign_ == Sign::Zero)
			return false;
		if (sign_ == Sign::Zero && other.sign_ == Sign::Plus)
			return true;

		if (sign_ == Sign::Plus && other.sign_ == Sign::Minus)
			return false;
		if (sign_ == Sign::Plus && other.sign_ == Sign::Zero)
			return false;

		if (size_ < other.size_)
			return true;
		if (size_ > other.size_)
			return false;

		for (size_t i = 0; i < size_; i++)
		{
			if (data_[size_ - i - 1] < other.data_[size_ - i - 1])
				return true;
			if (data_[size_ - i - 1] > other.data_[size_ - i - 1])
				return false;
		}
		return false;
	}

	bool BigInt::operator<=(const BigInt & other) const
	{
		return !(other < *this);
	}

	bool BigInt::operator>(const BigInt & other) const
	{
		return other < *this;
	}

	bool BigInt::operator>=(const BigInt & other) const
	{
		return !(*this < other);
	}

	bool BigInt::operator==(const BigInt & other) const
	{
		return !(*this < other) && !(other < *this);
	}

	bool BigInt::operator!=(const BigInt & other) const
	{
		return *this < other || other < *this;
	}

	std::ostream & operator<<(std::ostream & os, const BigInt & num)
	{
		if (num.sign_ == BigInt::Sign::Zero)
		{
			os << 0;
			return os;
		}

		if (num.sign_ == BigInt::Sign::Minus)
			os << '-';
		for (size_t i = 0; i < num.size_; i++)
			os << char(num.data_[num.size_ - i - 1] + '0');
		return os;
	}
}