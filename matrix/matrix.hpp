#pragma once

#include <cstddef>
#include <stdint.h>
#include <iostream>


namespace matrix
{
	class Matrix
	{
	private:
		class ProxyRow
		{
		private:
			size_t m_;
			int32_t *data_;
		public:
			ProxyRow();

			explicit ProxyRow(size_t m, int32_t def = 0);

			ProxyRow(const ProxyRow &pr);

			size_t GetM() const;

			const ProxyRow &operator=(const ProxyRow &pr);

			int32_t &operator[](size_t j);

			~ProxyRow();
		};

		size_t n_;
		size_t m_;
		ProxyRow *rows_;
	public:
		static const size_t MaxSize = 100000000;

		Matrix();

		Matrix(size_t n, size_t m, int32_t def = 0);

		Matrix(const Matrix &mt);

		size_t GetRowsCount() const;

		size_t GetColumnsCount() const;

		const Matrix &operator=(const Matrix &mt);

		ProxyRow &operator[](size_t i) const;

		const Matrix &operator*=(int32_t num);

		Matrix operator+(const Matrix &mt) const;

		bool operator==(const Matrix &mt) const;

		bool operator!=(const Matrix &mt) const;

		friend std::ostream &operator<<(std::ostream &os, const Matrix &mt);

		~Matrix();
	};

	std::ostream &operator<<(std::ostream &os, const Matrix &mt);
}