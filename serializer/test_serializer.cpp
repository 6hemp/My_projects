#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>

#include "serializer.hpp"


struct DataEmpty
{
	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer();
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer();
	}

	bool operator==(const DataEmpty &) const
	{
		return true;
	}
};


struct DataBool1
{
	bool a;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a);
	}

	bool operator==(const DataBool1 & other) const
	{
		return a == other.a;
	}
};


struct DataBool4
{
	bool a;
	bool b;
	bool c;
	bool d;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d);
	}

	bool operator==(const DataBool4 & other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d;
	}
};


struct DataInt1
{
	uint64_t a;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a);
	}

	bool operator==(const DataInt1 & other) const
	{
		return a == other.a;
	}
};


struct DataInt4
{
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d);
	}

	bool operator==(const DataInt4 & other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d;
	}
};


struct Data1
{
	bool a;
	bool b;
	bool c;
	uint64_t d;
	uint64_t e;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d, e);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d, e);
	}

	bool operator==(const Data1 & other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d && e == other.e;
	}
};


struct Data2
{
	bool a;
	bool b;
	uint64_t c;
	uint64_t d;
	uint64_t e;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d, e);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d, e);
	}

	bool operator==(const Data2 & other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d && e == other.e;
	}
};


struct DataIncType
{
	bool a;
	bool b;
	uint64_t c;
	uint64_t d;
	std::string s;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d, s);
	}

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d, s);
	}

	bool operator==(const DataIncType & other) const
	{
		return a == other.a && b == other.b && c == other.c && d == other.d && s == other.s;
	}
};


struct DataNonDes
{
	bool a;
	bool b;
	uint64_t c;
	uint64_t d;

	template <typename Serializer>
	hemp::Error serialize(Serializer& serializer)
	{
		return serializer(a, b, c, d);
	}
};


struct DataNonSer
{
	bool a;
	bool b;
	uint64_t c;
	uint64_t d;

	template <typename Deserializer>
	hemp::Error deserialize(Deserializer& deserializer)
	{
		return deserializer(a, b, c, d);
	}
};


struct DataNonDesSer
{
	bool a;
	bool b;
	uint64_t c;
	uint64_t d;
};


class TestSerializer : public ::testing::Test
{
public:
	std::stringstream stream;
	hemp::Serializer serializer;
	hemp::Deserializer deserializer;

	void reset()
	{
		stream.str(std::string());
		serializer = hemp::Serializer(stream);
		deserializer = hemp::Deserializer(stream);
	}
protected:
	virtual void SetUp()
	{
		reset();
	}

	virtual void TearDown() { }
};


TEST_F(TestSerializer, test_serializer_empty)
{
	DataEmpty x;
	DataEmpty y;

	ASSERT_EQ(serializer.save(x), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y), hemp::Error::NoError);
	ASSERT_EQ(x, y);
}


TEST_F(TestSerializer, test_serializer_bool)
{
	DataBool1 x1 {true};
	DataBool1 y1 {false};
	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::NoError);
	ASSERT_EQ(x1, y1);
	reset();

	DataBool4 x4 {true, false, true, true};
	DataBool4 y4 {false, false, true, false};
	ASSERT_EQ(serializer.save(x4), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y4), hemp::Error::NoError);
	ASSERT_EQ(x4, y4);
	reset();

	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y4), hemp::Error::CorruptedArchive);
	reset();

	ASSERT_EQ(serializer.save(x4), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::CorruptedArchive);
	reset();

	DataInt1 xi1 {1};
	ASSERT_EQ(serializer.save(xi1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::CorruptedArchive);
}


TEST_F(TestSerializer, test_serializer_int)
{
	DataInt1 x1 {5};
	DataInt1 y1 {9};
	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::NoError);
	ASSERT_EQ(x1, y1);
	reset();

	DataInt4 x4 {1, 2, 3, 5};
	DataInt4 y4 {9, 6, 3, 0};
	ASSERT_EQ(serializer.save(x4), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y4), hemp::Error::NoError);
	ASSERT_EQ(x4, y4);
	reset();

	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y4), hemp::Error::CorruptedArchive);
	reset();

	ASSERT_EQ(serializer.save(x4), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::CorruptedArchive);
	reset();

	DataBool1 xb1 {true};
	ASSERT_EQ(serializer.save(xb1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::CorruptedArchive);
}


TEST_F(TestSerializer, test_serializer_common)
{
	Data1 x1 {true, true, false, 5, 10};
	Data1 y1 {false, true, true, 6, 11};
	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::NoError);
	ASSERT_EQ(x1, y1);
	reset();

	Data2 x2 {false, false, 1, 3, 5};
	Data2 y2 {true, true, 0,1, 0};
	ASSERT_EQ(serializer.save(x2), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y2), hemp::Error::NoError);
	ASSERT_EQ(x2, y2);
	reset();

	ASSERT_EQ(serializer.save(x1), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y2), hemp::Error::CorruptedArchive);
	reset();

	ASSERT_EQ(serializer.save(x2), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y1), hemp::Error::CorruptedArchive);
}


TEST_F(TestSerializer, test_serializer_non_serializable)
{
	DataIncType x1 {true, false, 100, 1000, "false"};
	ASSERT_EQ(serializer.save(x1), hemp::Error::NonSerializable);
	reset();

	DataNonDes x2 {true, false, 100, 1000};
	DataNonDes y2 {false, false, 150, 15};
	ASSERT_EQ(serializer.save(x2), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y2), hemp::Error::NonSerializable);
	reset();

	DataNonSer x3 {true, true, 9, 10};
	DataNonSer y3 {true, false, 42, 1337};
	ASSERT_EQ(serializer.save(x3), hemp::Error::NonSerializable);
	reset();

	ASSERT_EQ(serializer.save(x2), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y3), hemp::Error::NoError);
	ASSERT_EQ(x2.a, y3.a);
	ASSERT_EQ(x2.b, y3.b);
	ASSERT_EQ(x2.c, y3.c);
	ASSERT_EQ(x2.d, y3.d);
	reset();


	DataNonDesSer x4 {true, true, 300, 420};
	DataNonDesSer y4 {false, false, 666, 777};
	ASSERT_EQ(serializer.save(x4), hemp::Error::NonSerializable);

	reset();
	ASSERT_EQ(serializer.save(x2), hemp::Error::NoError);
	ASSERT_EQ(deserializer.load(y4), hemp::Error::NonSerializable);
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}