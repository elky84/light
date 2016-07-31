#include "stdafx.h"
#include "BoostSerializer.h"

struct sample_data  
{  
	int id;  
	std::string name;  

private:  
	friend class boost::serialization::access;  
	template <typename AR> void serialize(AR & ar, const unsigned int ver)  
	{  
		UNREFERENCED_PARAMETER(ver);
        light::serialize_member(ar, id, name);
	}  
};

TEST(Serializer, Test)
{  
	sample_data src, dst;  
    light::STREAM_BUFFER_TYPE buffer;

	src.id = 1001;  
	src.name = "Hohoho";  
	light::serialize(src, buffer);  

    light::deserialize(dst, &buffer.front(), buffer.size());
	ASSERT_EQ(src.id, dst.id);
	ASSERT_EQ(src.name, dst.name);
}  