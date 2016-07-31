#pragma once

#include "light.h"
#include "light-curl.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Dependencies\\gmock-1.7.0\\gtest\\msvc\\gtest\\debug\\gtestd.lib")
#pragma comment(lib, "..\\..\\Dependencies\\gmock-1.7.0\\msvc\\2015\\debug\\gmock.lib")
#else //_DEBUG
#pragma comment(lib, "..\\..\\Dependencies\\gmock-1.7.0\\gtest\\msvc\\gtest\\release\\gtest.lib")
#pragma comment(lib, "..\\..\\Dependencies\\gmock-1.7.0\\msvc\\2015\\release\\gmock.lib")
#endif //_DEBUG
