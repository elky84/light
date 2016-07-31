#include "light.h"
#include "BoostRandom.h"

namespace light
{

int BoostRandom::rand_int(const int minimum, const int maximum)
{
	static SeedInt randInstance;
	return randInstance.rand(minimum, maximum);
}

double BoostRandom::rand_double(const double minimum, const double maximum)
{
	static SeedDouble randInstance;
	return randInstance.rand(minimum, maximum);
}

} // namespace light
