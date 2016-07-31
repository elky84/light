#pragma once

namespace light
{

class PureVirtualFunction
{
public:
	class Base 
	{ 
	public:
		Base() 
		{
			initialize();
		} 

		void initialize() 
		{ 
			call(); 
		} 

	private:
		virtual void call() = 0;
	};

	class Child : public Base
	{ 
	public:
		Child() 
		{
		}

	private:
		virtual void call() 
		{
		}
	}; 
};

class ConstructorException
{
public:
	ConstructorException()
	{
		int* p = NULL;
		*p = 5;
	}

	~ConstructorException()
	{
	}
};

class DestructorException
{
public:
	DestructorException()
	{
	}

	~DestructorException()
	{
		int* p = NULL;
		*p = 5;
	}
};

const int DATA_SIZE = 1000000;

class Object
{
public:
	Object()
		: m_key(0)
	{

	}

	virtual ~Object()
	{

	}

	int Key()
	{
		return m_key;
	}

private:
	int m_key;
};

class Child : public Object
{
public:
	Child()
		: m_integer(0)
	{
		memset(m_data, 0, sizeof(m_data));
	}

	void set(int n)
	{
		m_integer = n;
	}

private:
	int m_data[DATA_SIZE];

	int m_integer;
};


class RaiseExceptions
{
public:
	static void PureVirtualFunctionCall();

	static void CRTError();

	static void StackOverflow();

	static void NullPtrAccess();

	static void DivideZero();

	static void InvalidCastingFunctionCall();

	static void DuplicatedDelete();

	static void NullPtrAssign();

	static void StackOverwrite();

	static void DestructExceptionRaise();

	static void ConstructorWithDestructExceptionRaise();

	static void UnlimitRecursiveFunctionCall();

	static void HeabufferOverrun();
};

} // namespace light