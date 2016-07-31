#pragma once

namespace light
{

template <typename T>
inline T bit_sum(const T &dest, const T &src)
{
	return dest | src;
}

template <typename T>
inline T bit_xor(const T &dest, const T &src)
{
	return dest ^ src;
}

template <typename T>
inline T bit_and(const T &Value, const T &n)
{
	return Value & n;
}

template <typename T>
inline bool bit_compare(const T &dest, const T &src)
{
	if( (dest & src) == src)
	{
		return true;
	}
	return false;
}

template <typename T>
inline bool bit_state(const T &Value, const unsigned short &n)
{
	if(Value & (1 << n))
	{
		return true;
	}
	return false;
}

template <typename T>
inline void bit_on(T &Value, const unsigned short &n)
{
	Value |= (1 << n);
}

template <typename T>
inline void bit_off(T &Value, const unsigned short &n)
{
	Value &= ~(1 << n);
}

} //namespace light
