#pragma once

#include <iostream>


namespace hemp
{
	class Serializer;
	class Deserializer;


	enum class Error
	{
		NoError,
		CorruptedArchive,
		NonSerializable
	};


	template<typename X, typename Y>
	struct is_same
	{
		static constexpr bool value = false;
	};


	template<typename X>
	struct is_same<X, X>
	{
		static constexpr bool value = true;
	};


	template<typename Type>
	struct is_serializable
	{
	private:
		struct FakeType { };
	public:
		static constexpr bool value = is_same<Error, decltype(is_serializable<Type>::check((Type *) nullptr))>::value;

		static FakeType check(...);

		static Serializer param;

		template<typename U>
		static auto check(U *u) -> decltype(u->serialize(param));
	};


	template<typename Type>
	struct is_deserializable
	{
	private:
		struct FakeType { };
	public:
		static constexpr bool value = is_same<Error, decltype(is_deserializable<Type>::check((Type *) nullptr))>::value;

		static FakeType check(...);

		static Deserializer param;

		template<typename U>
		static auto check(U *u) -> decltype(u->deserialize(param));
	};


	class Serializer
	{
		static constexpr char Separator = ' ';
	public:
		Serializer();

		explicit Serializer(std::ostream & out);

		template<typename Type>
		Error save(Type & object, typename std::enable_if<is_serializable<Type>::value>::type * = nullptr);

		template<typename Type>
		Error save(Type & object, typename std::enable_if<!is_serializable<Type>::value>::type * = nullptr);

		Error operator()();

		template<typename Type>
		Error operator()(Type arg);

		template<typename Type, typename... Types>
		Error operator()(Type arg, Types... args);

	private:
		Error process(bool arg);

		Error process(uint64_t arg);

		template<typename Type>
		Error process(Type arg);

	private:
		std::ostream *out_;
	};


	class Deserializer
	{
	public:
		Deserializer();

		explicit Deserializer(std::istream & in_);

		template<typename Type>
		Error load(Type & object, typename std::enable_if<is_deserializable<Type>::value>::type * = nullptr);

		template<typename Type>
		Error load(Type & object, typename std::enable_if<!is_deserializable<Type>::value>::type * = nullptr);

		Error operator()();

		template<typename Type>
		Error operator()(Type & arg);

		template<typename Type, typename... Types>
		Error operator()(Type & arg, Types&... args);

	private:
		Error process(bool & arg);

		Error process(uint64_t & arg);

		template<typename Type>
		Error process(Type & arg);

	private:
		std::istream *in_;
	};
}
#include "serializer.tpp"