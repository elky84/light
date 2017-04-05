#pragma once

#include "light.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Dependencies\\googletest\\googletest\\msvc\\gtest\\debug\\gtestd.lib")
#pragma comment(lib, "..\\..\\Dependencies\\googletest\\googlemock\\msvc\\2017\\debug\\gmock.lib")
#else //_DEBUG
#pragma comment(lib, "..\\..\\Dependencies\\googletest\\googletest\\msvc\\gtest\\release\\gtest.lib")
#pragma comment(lib, "..\\..\\Dependencies\\googletest\\googlemock\\msvc\\2017\\release\\gmock.lib")
#endif //_DEBUG
