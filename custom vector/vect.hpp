#pragma once

#include <iterator>
#include <memory>
#include <initializer_list>


namespace hemp
{
	template<typename Type>
	class Allocator
	{
	public:

		Type * allocate(size_t count);

		void deallocate(Type *ptr, size_t count);
	};


	template<typename Type>
	class RAIterator : public std::iterator<std::random_access_iterator_tag, Type>
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
	public:

		RAIterator();

		RAIterator(Type *rhs);

		RAIterator(const RAIterator<Type> & rhs);

		RAIterator<Type> & operator=(Type *rhs);

		RAIterator<Type> & operator=(const RAIterator<Type> & rhs);

		RAIterator<Type> & operator+=(difference_type rhs);

		RAIterator<Type> & operator-=(difference_type rhs);

		Type & operator*() const;

		Type *operator->() const;

		Type & operator[](difference_type rhs) const;

		RAIterator<Type> & operator++();

		RAIterator<Type> & operator--();

		RAIterator<Type> operator++(int) const;

		RAIterator<Type> operator--(int) const;

		RAIterator<Type> operator+(const RAIterator<Type> & rhs);

		difference_type operator-(const RAIterator<Type> & rhs) const;

		RAIterator<Type> operator+(difference_type rhs) const;

		RAIterator<Type> operator-(difference_type rhs) const;

		template<typename T>
		friend RAIterator<T> operator+(difference_type lhs, const RAIterator<T> & rhs);

		template<typename T>
		friend RAIterator<T> operator-(difference_type lhs, const RAIterator<T> & rhs);

		bool operator==(const RAIterator<Type> & rhs) const;

		bool operator!=(const RAIterator<Type> & rhs) const;

		bool operator>(const RAIterator<Type> & rhs) const;

		bool operator<(const RAIterator<Type> & rhs) const;

		bool operator>=(const RAIterator<Type> & rhs) const;

		bool operator<=(const RAIterator<Type> & rhs) const;

	private:
		Type *_ptr;
	};


	template<typename Type>
	class Vector
	{
	public:

		Vector();

		explicit Vector(size_t size);

		Vector(size_t size, const Type & defaultValue);

		Vector(std::initializer_list <Type> init);

		Vector(const Vector<Type> & other);

		Vector<Type> & operator=(const Vector<Type> & other);

		Type & operator[](size_t ind);

		void push_back(Type && elem);

		void pop_back();

		template<typename ... Args>
		void emplace_back(Args && ... args);

		bool empty() const;

		size_t size() const;

		void clear();

		RAIterator<Type> begin();

		RAIterator<Type> rbegin();

		RAIterator<Type> end();

		RAIterator<Type> rend();

		void resize(size_t count);

		void reserve(size_t size);

		size_t capacity() const;

		~Vector();

	private:
		Type *data_;
		size_t size_;
		size_t capacity_;
		Allocator<Type> allocator_;
	};
}

#include "vect.tpp"