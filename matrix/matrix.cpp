#include <cstddef>
#include <stdint.h>
#include <iostream>
#include <new>
#include <stdexcept>

#include "matrix.hpp"

//ProxyRow
namespace matrix
{
	Matrix::ProxyRow::ProxyRow()
	{
		m_ = 0;
		data_ = nullptr;
	}

	Matrix::ProxyRow::ProxyRow(size_t m, int32_t def)
	{
		m_ = m;

		if (m == 0)
		{
			data_ = nullptr;
			return;
		}

		data_ = new(std::nothrow) int32_t[m];

		if (data_ == nullptr)
		{
			m_ = 0;
			return;
		}

		for (size_t i = 0; i < m; i++)
			data_[i] = def;
	}

	Matrix::ProxyRow::ProxyRow(const Matrix::ProxyRow &pr)
	{
		m_ = pr.m_;

		if (m_ == 0)
		{
			data_ = nullptr;
			return;
		}

		data_ = new(std::nothrow) int32_t[m_];

		if (data_ == nullptr)
		{
			m_ = 0;
			return;
		}

		for (size_t i = 0; i < m_; i++)
			data_[i] = pr.data_[i];
	}

	size_t Matrix::ProxyRow::GetM() const
	{
		return m_;
	}

	const Matrix::ProxyRow &Matrix::ProxyRow::operator=(const Matrix::ProxyRow &pr)
	{
		if (data_)
			delete[] data_;

		m_ = pr.m_;

		if (m_ == 0)
		{
			data_ = nullptr;
			return *this;
		}

		data_ = new(std::nothrow) int32_t[m_];

		if (data_ == nullptr)
		{
			m_ = 0;
			return *this;
		}

		for (size_t i = 0; i < m_; i++)
			data_[i] = pr.data_[i];

		return *this;
	}

	int32_t &Matrix::ProxyRow::operator[](size_t j)
	{
		if (j >= m_)
			throw std::out_of_range("Matrix::ProxyRow []: index j out of range");
		return data_[j];
	}

	Matrix::ProxyRow::~ProxyRow()
	{
		if (data_)
			delete[] data_;
	}
}

//Matrix
namespace matrix
{
	Matrix::Matrix()
	{
		n_= 0;
		m_ = 0;
		rows_ = nullptr;
	}

    Matrix::Matrix(size_t n, size_t m, int32_t def)
    {
	    if (n > MaxSize || m > MaxSize || n * m > MaxSize || n == 0 || m == 0)
	    {
			n_ = 0;
		    m_ = 0;
		    rows_ = nullptr;
		    return;
	    }

	    n_ = n;
	    m_ = m;

	    rows_ = new(std::nothrow) ProxyRow[n];

	    if (rows_ == nullptr)
	    {
		    n_ = 0;
		    m_ = 0;
		    return;
	    }

	    for (size_t i = 0; i < n; i++)
	    {
		    rows_[i] = ProxyRow(m, def);
		    if (rows_[i].GetM() == 0)
		    {
			    n_ = 0;
			    m_ = 0;
			    delete[] rows_;
			    rows_ = nullptr;
			    break;
		    }
	    }
    }

	Matrix::Matrix(const Matrix & mt)
	{
		n_ = mt.n_;
		m_ = mt.m_;

		if (mt.n_ == 0 || mt.m_ == 0)
		{
			n_ = 0;
			m_ = 0;
			rows_ = nullptr;
			return;
		}

		rows_ = new(std::nothrow) ProxyRow[n_];

		if (rows_ == nullptr)
		{
			n_ = 0;
			m_ = 0;
			return;
		}

		for (size_t i = 0; i < n_; i++)
		{
			rows_[i] = mt.rows_[i];
			if (rows_[i].GetM() == 0)
			{
				n_ = 0;
				m_ = 0;
				delete[] rows_;
				rows_ = nullptr;
				break;
			}
		}
	}

	size_t Matrix::GetRowsCount() const
	{
		return n_;
	}

	size_t Matrix::GetColumnsCount() const
	{
		return m_;
	}

	const Matrix& Matrix::operator=(const Matrix& mt)
	{
		if (rows_)
			delete[] rows_;

		n_ = mt.n_;
		m_ = mt.m_;

		if (mt.n_ == 0 || mt.m_ == 0)
		{
			n_ = 0;
			m_ = 0;
			rows_ = nullptr;
			return *this;
		}

		rows_ = new(std::nothrow) ProxyRow[n_];

		if (rows_ == nullptr)
		{
			n_ = 0;
			m_ = 0;
			return *this;
		}

		for (size_t i = 0; i < n_; i++)
		{
			rows_[i] = mt.rows_[i];
			if (rows_[i].GetM() == 0)
			{
				n_ = 0;
				m_ = 0;
				delete[] rows_;
				rows_ = nullptr;
				break;
			}
		}
		return *this;
	}

	Matrix::ProxyRow& Matrix::operator[](size_t i) const
	{
		if (i >= n_)
			throw std::out_of_range("Matrix []: index i out of range");
		return rows_[i];
	}

	const Matrix& Matrix::operator*=(int32_t num)
	{
		for (size_t i = 0; i < n_; i++)
			for (size_t j = 0; j < m_; j++)
				rows_[i][j] *= num;
		return *this;
	}

	Matrix Matrix::operator+(const Matrix& mt) const
	{
		if (mt.n_ != n_ || mt.m_ != m_)
			throw std::length_error("Matrix +: sizes of matrices are not equal");

		Matrix ans(n_, m_);

		if (ans.n_ != 0)
		{
			for (size_t i = 0; i < n_; i++)
				for (size_t j = 0; j < m_; j++)
					ans.rows_[i][j] = rows_[i][j] + mt.rows_[i][j];
		}
		return ans;
	}

	bool Matrix::operator==(const Matrix& mt) const
	{
		if (mt.n_ != n_ || mt.m_ != m_)
			return false;
		for (size_t i = 0; i < n_; i++)
			for (size_t j = 0; j < m_; j++)
				if (rows_[i][j] != mt.rows_[i][j])
					return false;
		return true;
	}

	bool Matrix::operator!=(const Matrix& mt) const
	{
		if (mt.n_ != n_ || mt.m_ != m_)
			return true;
		for (size_t i = 0; i < n_; i++)
			for (size_t j = 0; j < m_; j++)
				if (rows_[i][j] != mt.rows_[i][j])
					return true;
		return false;
	}

    Matrix::~Matrix()
    {
		if (rows_)
            delete[] rows_;
    }


	std::ostream& operator << (std::ostream &os, const Matrix &mt)
	{
		for (size_t i = 0; i < mt.n_; i++)
		{
			for (size_t j = 0; j < mt.m_; j++)
				os << mt.rows_[i][j] << " ";
			os << std::endl;
		}
		return os;
	}
}