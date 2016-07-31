#pragma once

namespace light
{

class Interpolation
{
public:
	Interpolation(const float fMin, const float fMax, const float fDivide)
		: m_fMin(fMin)
		, m_fMax(fMax)
		, m_fDivide(fDivide)
		, m_fProgress(0.0f)
	{
	}

	virtual const float execute()
	{
		const float fProgress = Progress();
		const float fResult = Operate(fProgress / Divide());
		return (m_fMax * fResult) + (m_fMin * ( 1.0f - fResult));
	}

	virtual const float Operate(const float fValue) = 0;

	bool is_complete() const
	{
		return equals(m_fProgress, m_fDivide);
	}

protected:
	const float Progress()
	{
		return m_fProgress++; // 일부러 이렇게했음. 이전값을 반환하고 후증가 시키기 위함.
	}

	const float Divide() const
	{
		return m_fDivide;
	}

private:
	const float m_fMin;

	const float m_fMax;

	float m_fProgress;

	const float m_fDivide;
};

class LinearInterpolation : public Interpolation
{
public:
	LinearInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return fValue;
	}
};

class SmoothStepInterpolation : public Interpolation
{
public:
	SmoothStepInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return SmoothStep(fValue);
	}

	static float SmoothStep(float x)
	{
		return x * x * (3 - 2 * x);
	}
};

class SquaredInterpolation : public Interpolation
{
public:
	SquaredInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return 1 - (1 - fValue) * (1 - fValue);
	}
};

class InverseSquaredInterpolation : public Interpolation
{
public:
	InverseSquaredInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return fValue * fValue;
	}
};

class SinInterpolation : public Interpolation
{
public:
	SinInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return sin(fValue * 3.14f / 2);
	}
};

class CosInterpolation : public Interpolation
{
public:
	CosInterpolation(const float fMin, const float fMax, const float fDivide)
		: Interpolation(fMin, fMax, fDivide)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return 0.5f - cos(-fValue * 3.14f) * 0.5f;
	}
};

class SplineInterpolation : public Interpolation
{
public:
	SplineInterpolation(const float fMin, const float fMax, const float fDivide, const float fQ, const float fT)
		: Interpolation(fMin, fMax, fDivide)
		, Q(fQ)
		, T(fT)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return catmullrom(fValue, Q, 0, 1, T);
	}

	static float catmullrom(float t, float p0, float p1, float p2, float p3)
	{
		return 0.5f * ( 
			( 2 * p1) + 
			(-p0 + p2) * t + 
			(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t + 
			(p0 + 3 *p1 - 3 * p2 + p3) * t * t * t
			);
	}

private:
	const float Q;
	const float T;
};

class WeightedAverageInterpolation : public Interpolation
{
public:
	WeightedAverageInterpolation(const float fMin, const float fMax, const float fDivide, const float fWeight)
		: Interpolation(fMin, fMax, fDivide)
		, Weight(fWeight)
	{
	}

private:
	virtual const float Operate(const float fValue)
	{
		return ((fValue * (Divide() - 1)) + Weight) / Divide();
	}

private:
	const float Weight;
};

} //namespace light
