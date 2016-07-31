#pragma once

namespace light
{

template <typename T>
class ReloadCommon
	: public T
{
public: // for common method

	void clear()
	{
		T::clear();
		Temp.clear();
	}

	void deploy()
	{
		T::clear();
		T::operator =(Temp);
		Temp.clear();
	}

protected:
	T Temp;
};

template <typename T, typename value_type = T::value_type, typename iterator_type = T::iterator>
class ReloadMultiIndexContainer
	: public ReloadCommon<T>
{
public: // for temp
	std::pair<iterator_type, bool> insert(const value_type& val)
	{
		return Temp.insert(val);
	}

	void deploy()
	{
		T::clear();
		for each(const value_type& val in Temp)
		{
			T::insert(val);
		}
		Temp.clear();
	}
};

template <typename T, typename key_type = T::key_type, typename mapped_type = T::mapped_type, typename iter_type = T::iterator, typename value_type = T::value_type>
class ReloadMappedContainer
	: public ReloadCommon<T>
{
public: // for temp
	std::pair<iter_type, bool> insert(const key_type& key, const mapped_type& val)
	{
		return Temp.insert(value_type(key, val));
	}

	std::pair<iter_type, bool> insert(const value_type& val)
	{
		return Temp.insert(val);
	}

	mapped_type& temp(const key_type& key)
	{
		return Temp[key];
	}

public: // for active

	iter_type find(const key_type& key)
	{
		return T::find(key);
	}

	bool find(const key_type& key, mapped_type& val)
	{
		T::iterator it = T::find(key);
		if( it == this->end() )
			return false;

		val = it->second;
		return true;
	}

	bool find(const key_type& key, mapped_type*& pVal)
	{
		T::iterator it = T::find(key);
		if( it == this->end() )
			return false;

		pVal = &(it->second);
		return true;
	}
};


template <typename T, typename value_type = T::value_type >
class ReloadSetContainer
	: public ReloadCommon<T>
{
public:  // for temp
	void insert(value_type& val)
	{
		Temp.insert(val);
	}

	void insert(const value_type& val)
	{
		Temp.insert(val);
	}

public: // for active
	bool find(const value_type& val)
	{
		T::iterator it = T::find(val);
		if( it == this->end() )
			return false;

		return true;
	}
};

template <typename T, typename value_type = T::value_type >
class ReloadContainer
	: public ReloadCommon<T>
{
public:  // for temp
	void insert(value_type& val)
	{
		Temp.insert(val);
	}

	void push(value_type& val)
	{
		Temp.push(val);
	}

	void push_back(value_type& val)
	{
		Temp.push_back(val);
	}

public: // for active

	bool find(value_type& val)
	{
		T::iterator it = T::find(val);
		if( it == this->end() )
			return false;

		val = *it;
		return true;
	}

	bool at(const size_t n, value_type& val)
	{
		if( size() <= n )
			return false;

		val = operator[](n);
		return true;
	}
};

} // namespace light
