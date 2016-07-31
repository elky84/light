#pragma once

#include "Str.h"

namespace light
{

std::wstring get_path_without_file_name(const std::wstring& path, const std::wstring& delimiter = L"\\");

std::wstring get_file_name_without_path(const std::wstring& path, const std::wstring& delimiter = L"\\");

std::wstring get_module_path();

std::wstring get_module_file_name();

class Directory
{
public:
	static std::wstring get_current_directory();

	static bool set_current_directory(const std::wstring& directory);

	static std::wstring path_canonicalize(const std::wstring& path);

	static bool create_directory(const std::wstring& path);

	static bool is_exist(const std::wstring& file_name);

	static bool is_exist_directory(const std::wstring& path);

	static bool delete_directory(const std::wstring& path);
};

} //namespace light
