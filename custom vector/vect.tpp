#pragma once

#include <iterator>
#include <memory>
#include <initializer_list>

#include "vect.hpp"


namespace hemp
{
	template<typename Type>
	Type* Allocator<Type>::allocate(size_t count)
	{
		return new Type[count];
	}

	template<typename Type>
	void Allocator<Type>::deallocate(Type* ptr, size_t)
	{
		delete[] ptr;
	}


	template<typename Type>
	RAIterator<Type>::RAIterator() : _ptr(nullptr) {}

	template<typename Type>
	RAIterator<Type>::RAIterator(Type * rhs) : _ptr(rhs) {}

	template<typename Type>
	RAIterator<Type>::RAIterator(const RAIterator<Type> &rhs) : _ptr(rhs._ptr) {}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator=(Type* rhs)
	{
		_ptr = rhs;
		return *this;
	}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator=(const RAIterator<Type> &rhs)
	{
		_ptr = rhs._ptr;
		return *this;
	}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator+=(typename RAIterator<Type>::difference_type rhs)
	{
		_ptr += rhs;
		return *this;
	}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator-=(typename RAIterator<Type>::difference_type rhs)
	{
		_ptr -= rhs;
		return *this;
	}

	template<typename Type>
	Type& RAIterator<Type>::operator*() const
	{
		return *_ptr;
	}

	template<typename Type>
	Type* RAIterator<Type>::operator->() const
	{
		return _ptr;
	}

	template<typename Type>
	Type& RAIterator<Type>::operator[](typename RAIterator<Type>::difference_type rhs) const
	{
		return _ptr[rhs];
	}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename Type>
	RAIterator<Type>& RAIterator<Type>::operator--()
	{
		--_ptr;
		return *this;
	}

	template<typename Type>
	RAIterator<Type> RAIterator<Type>::operator++(int) const
	{
		RAIterator<Type> tmp(*this);
		++_ptr;
		return tmp;
	}

	template<typename Type>
	RAIterator<Type> RAIterator<Type>::operator--(int) const
	{
		RAIterator<Type> tmp(*this);
		--_ptr;
		return tmp;
	}

	template<typename Type>
	RAIterator<Type> RAIterator<Type>::operator+(const RAIterator<Type>& rhs)
	{
		return RAIterator<Type>(_ptr+rhs.ptr);
	}

	template<typename Type>
	typename RAIterator<Type>::difference_type RAIterator<Type>::operator-(const RAIterator<Type>& rhs) const
	{
		return _ptr-rhs.ptr;
	}

	template<typename Type>
	RAIterator<Type> RAIterator<Type>::operator+(typename RAIterator<Type>::difference_type rhs) const
	{
		return RAIterator<Type>(_ptr+rhs);
	}

	template<typename Type>
	RAIterator<Type> RAIterator<Type>::operator-(typename RAIterator<Type>::difference_type rhs) const
	{
		return RAIterator<Type>(_ptr-rhs);
	}

	template<typename T>
	RAIterator<T> operator+(typename RAIterator<T>::difference_type lhs, const RAIterator<T>& rhs)
	{
		return RAIterator<T>(lhs+rhs._ptr);
	}

	template<typename T>
	RAIterator<T> operator-(typename RAIterator<T>::difference_type lhs, const RAIterator<T>& rhs)
	{
		return RAIterator<T>(lhs-rhs._ptr);
	}

	template<typename Type>
	bool RAIterator<Type>::operator==(const RAIterator<Type>& rhs) const
	{
		return _ptr == rhs._ptr;
	}

	template<typename Type>
	bool RAIterator<Type>::operator!=(const RAIterator<Type>& rhs) const
	{
		return _ptr != rhs._ptr;
	}

	template<typename Type>
	bool RAIterator<Type>::operator>(const RAIterator<Type>& rhs) const
	{
		return _ptr > rhs._ptr;
	}

	template<typename Type>
	bool RAIterator<Type>::operator<(const RAIterator<Type>& rhs) const
	{
		return _ptr < rhs._ptr;
	}

	template<typename Type>
	bool RAIterator<Type>::operator>=(const RAIterator<Type>& rhs) const
	{
		return _ptr >= rhs._ptr;
	}

	template<typename Type>
	bool RAIterator<Type>::operator<=(const RAIterator<Type>& rhs) const
	{
		return _ptr <= rhs._ptr;
	}


	template<typename Type>
	Vector<Type>::Vector()
	{
		size_ = 0;
		capacity_ = 4;
		data_ = allocator_.allocate(capacity_);
	}

	template<typename Type>
	Vector<Type>::Vector(size_t size)
	{
		if (size < 4)
		{
			size_ = 0;
			capacity_ = 4;
		}
		else
		{
			size_ = 0;
			capacity_ = 1;
			while (size > 0)
			{
				size >>= 1;
				capacity_ <<= 1;
			}
		}
		data_ = allocator_.allocate(capacity_);
	}

	template<typename Type>
	Vector<Type>::Vector(size_t size, const Type & defaultValue)
	{
		if (size < 4)
		{
			size_ = 0;
			capacity_ = 4;
		}
		else
		{
			size_ = size;
			capacity_ = 1;
			while (size > 0)
			{
				size >>= 1;
				capacity_ <<= 1;
			}
		}
		data_ = allocator_.allocate(capacity_);
		for (size_t i = 0; i < size_; ++i)
			data_[i] = defaultValue;
	}

	template<typename Type>
	Vector<Type>::Vector(std::initializer_list<Type> init)
	{
		if (init.size() < 4)
		{
			size_ = init.size();
			capacity_ = 4;
		}
		else
		{
			size_ = init.size();
			capacity_ = 1;
			while (size_ > 0)
			{
				size_ >>= 1;
				capacity_ <<= 1;
			}
			size_ = init.size();
		}
		data_ = allocator_.allocate(capacity_);
		auto current = init.begin();
		const auto end = init.end();
		for (size_t i = 0; current != end; ++i)
			data_[i] = *current++;
	}

	template<typename Type>
	Vector<Type>::Vector(const Vector<Type> & other)
	{
		size_ = other.size_;
		capacity_ = other.capacity_;
		data_ = allocator_.allocate(capacity_);
		for (size_t i = 0; i < size_; ++i)
			data_[i] = other.data_[i];
	}

	template<typename Type>
	Vector<Type> & Vector<Type>::operator=(const Vector<Type> & other)
	{
		if (&other == this)
			return *this;

		clear();
		size_ = other.size_;
		capacity_ = other.capacity_;
		data_ = allocator_.allocate(capacity_);
		for (size_t i = 0; i < size_; ++i)
			data_[i] = other.data_[i];

		return *this;
	}

	template<typename Type>
	Type & Vector<Type>::operator[](size_t ind)
	{
		return data_[ind];
	}

	template<typename Type>
	void Vector<Type>::push_back(Type && elem)
	{
		if (data_)
		{
			if (size_ == capacity_)
				resize(capacity_ + 1);
			data_[size_++] = elem;
		}
		else
		{
			size_ = 0;
			capacity_ = 4;
			data_ = allocator_.allocate(capacity_);
			data_[size_++] = elem;
		}
	}

	template<typename Type>
	void Vector<Type>::pop_back()
	{
		--size_;
		if (capacity_ >= 8 && size_ <= capacity_  / 4)
			resize(capacity_ / 2 - 1);
	}

	template<typename Type>
	template<typename ... Args>
	void Vector<Type>::emplace_back(Args && ... args)
	{
		if (data_)
		{
			if (size_ == capacity_)
				resize(capacity_ + 1);
			data_[size_++] = Type(args...);
		}
		else
		{
			size_ = 0;
			capacity_ = 4;
			data_ = allocator_.allocate(capacity_);
			data_[size_++] = Type(args...);
		}
	}

	template<typename Type>
	bool Vector<Type>::empty() const
	{
		return size_ == 0;
	}

	template<typename Type>
	size_t Vector<Type>::size() const
	{
		return size_;
	}

	template<typename Type>
	void Vector<Type>::clear()
	{
		if (data_)
			allocator_.deallocate(data_, capacity_);
		size_ = 0;
		capacity_ = 0;
		data_ = nullptr;
	}

	template<typename Type>
	RAIterator<Type> Vector<Type>::begin()
	{
		return RAIterator<Type>(data_);
	}

	template<typename Type>
	RAIterator<Type> Vector<Type>::rbegin()
	{
		return RAIterator<Type>(data_ + size_ - 1);
	}

	template<typename Type>
	RAIterator<Type> Vector<Type>::end()
	{
		return RAIterator<Type>(data_ + size_);
	}

	template<typename Type>
	RAIterator<Type> Vector<Type>::rend()
	{
		return RAIterator<Type>(data_ - 1);
	}

	template<typename Type>
	void Vector<Type>::resize(size_t count)
	{
		Vector<Type> tmp = *this;
		clear();
		if (count < tmp.size_)
			size_ = count;
		else
			size_ = tmp.size_;
		capacity_ = 1;
		while (count > 0)
		{
			count >>= 1;
			capacity_ <<= 1;
		}
		data_ = allocator_.allocate(capacity_);
		for (size_t i = 0; i < size_; ++i)
			data_[i] = tmp[i];
	}

	template<typename Type>
	void Vector<Type>::reserve(size_t size)
	{
		resize(capacity_ + size);
	}

	template<typename Type>
	size_t Vector<Type>::capacity() const
	{
		return capacity_;
	}

	template<typename Type>
	Vector<Type>::~Vector()
	{
		clear();
	}
}