#pragma once

#pragma warning ( push )  
#pragma warning ( disable:4244 )  
#pragma warning ( disable:4310 )  
#pragma warning ( disable:4702 )  

#include <boost/archive/binary_oarchive.hpp>  
#include <boost/archive/binary_iarchive.hpp>  
#include <boost/serialization/array.hpp>  
#include <boost/serialization/vector.hpp>  
#include <boost/serialization/list.hpp>  
#include <boost/serialization/string.hpp>  
#include <boost/iostreams/stream.hpp>  
#include <boost/iostreams/device/array.hpp>  
#include <boost/iostreams/device/back_inserter.hpp>  
#include <string>  
#include <vector>  

namespace light
{

typedef std::vector<char> STREAM_BUFFER_TYPE;  
typedef boost::iostreams::back_insert_device<STREAM_BUFFER_TYPE > BOOST_STREAM_DEVICE;  
typedef boost::iostreams::basic_array_source<char> BOOST_ARRAY_DEVICE;  
typedef boost::iostreams::stream<BOOST_STREAM_DEVICE> OUT_STREAM_TYPE;  
typedef boost::iostreams::stream_buffer<BOOST_ARRAY_DEVICE> IN_STREAM_TYPE;  

template <typename T> inline void serialize(T const& obj/* _in_ */, STREAM_BUFFER_TYPE& buffer_/* _out_ */)  
{  
	OUT_STREAM_TYPE out_s_(buffer_);  
	boost::archive::binary_oarchive out_ar_(out_s_);  
	out_ar_ << obj;  
	out_s_.flush();  
}  
template <typename T> inline bool deserialize(T& obj/* _out_ */, char const* pRawData/* _in_ */, size_t dataSize/* _in_ */)  
{  
	try  
	{  
		IN_STREAM_TYPE in_s_(pRawData, dataSize);  
		boost::archive::binary_iarchive in_ar_(in_s_);  
		in_ar_ >> obj;  
	}  
	catch (boost::archive::archive_exception const&)  
	{  
		return false;  
	}  

	return true;  
}  

template <typename AR, typename T>
inline void serialize_member(AR& ar, T& d1) { ar & d1; }
template <typename AR, typename T, typename T2>
inline void serialize_member(AR& ar, T& d1, T2& d2) { ar & d1; ar & d2; }
template <typename AR, typename T, typename T2, typename T3>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3) { ar & d1; ar & d2; ar & d3; }
template <typename AR, typename T, typename T2, typename T3, typename T4>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4) { ar & d1; ar & d2; ar & d3; ar & d4; }
template <typename AR, typename T, typename T2, typename T3, typename T4, typename T5>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4, T5& d5) { ar & d1; ar & d2; ar & d3; ar & d4; ar & d5; }
template <typename AR, typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4, T5& d5, T6& d6) { ar & d1; ar & d2; ar & d3; ar & d4; ar & d5; ar & d6; }
template <typename AR, typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4, T5& d5, T6& d6, T7& d7) { ar & d1; ar & d2; ar & d3; ar & d4; ar & d5; ar & d6; ar & d7; }
template <typename AR, typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4, T5& d5, T6& d6, T7& d7, T8& d8) { ar & d1; ar & d2; ar & d3; ar & d4; ar & d5; ar & d6; ar & d7; ar & d8; }
template <typename AR, typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
inline void serialize_member(AR& ar, T& d1, T2& d2, T3& d3, T4& d4, T5& d5, T6& d6, T7& d7, T8& d8, T9& d9) { ar & d1; ar & d2; ar & d3; ar & d4; ar & d5; ar & d6; ar & d7; ar & d8; ar & d9 }

} // namespace light

#pragma warning ( pop )  