#include "light.h"
#include "DirectoryUtil.h"

#include <io.h>
#include <vector>
#include <ShellAPI.h>

namespace light
{

std::wstring get_path_without_file_name(const std::wstring& path, const std::wstring& delimiter)
{
	std::wstring result = path;
	std::wstring::size_type at = result.find_last_of(delimiter);
	if(std::wstring::npos != at)
	{
		result.erase(result.begin() + at + delimiter.size(), result.end());
		return result;
	}
	else
	{
		return L"";
	}
}

std::wstring get_file_name_without_path(const std::wstring& path, const std::wstring& delimiter)
{
	std::wstring result = path;
	std::wstring::size_type at = result.find_last_of(delimiter);
	if(std::wstring::npos != at)
	{
		result.erase(result.begin(), result.begin() + at + delimiter.size());
	}
	return result;
}

std::wstring get_module_path()
{
	return get_path_without_file_name( get_module_file_name() );
}

std::wstring get_module_file_name()
{
    WCHAR str[MAX_PATH] = { 0, };
    ::GetModuleFileName(NULL, str, MAX_PATH);
	return Directory::path_canonicalize(str).c_str();
}


std::wstring Directory::get_current_directory()
{
    WCHAR str[MAX_PATH] = {0, };
	::GetCurrentDirectory(MAX_PATH, str);
	return str;
}

bool Directory::set_current_directory(const std::wstring& directory)
{
	return ::SetCurrentDirectory(path_canonicalize(directory).c_str()) == TRUE ? true : false;
}

std::wstring Directory::path_canonicalize(const std::wstring& path)
{
    WCHAR result[MAX_PATH] = { 0, };
    if (path.find_first_of(L":") == std::wstring::npos)
    {
        std::wstring current_directory = get_current_directory() + L"\\" + path;
        BOOL ret = ::PathCanonicalize(result, current_directory.c_str());
        if (FALSE == ret)
        {
            LOG_ERROR(L"%d [%d] [%s]", __LINE__, GetLastError(), path.c_str());
            return L"";
        }
    }
    else
    {
        BOOL ret = ::PathCanonicalize(result, path.c_str());
        if (FALSE == ret)
        {
            LOG_ERROR(L"%d [%d] [%s]", __LINE__, GetLastError(), path.c_str());
            return L"";
        }
    }
    
    return result;
}

bool Directory::create_directory(const std::wstring& path)
{
	std::wstring absolute_path = path_canonicalize(path);
	str::VECTOR_STRING strings = str::split(absolute_path, L"\\");
	std::wstring current_path;
	for each(auto check_path in strings)
	{
		if ( current_path.empty() )
		{
			current_path = check_path;
		}
		else
		{
			current_path = current_path + L"\\" + check_path;
		}

		if( FALSE == ::CreateDirectory(current_path.c_str(), NULL) )
		{
			if(GetLastError() != ERROR_ALREADY_EXISTS )
			{
				LOG_ERROR(L"%d [%d] Path[%s]", __LINE__, GetLastError(), check_path.c_str());
				return false;
			}
		}
	}
	return true;
}

bool Directory::is_exist(const std::wstring& file_name) 
{
	intptr_t hFile;
	_tfinddatai64_t c_file;
	bool bResult = false;
	if((hFile = _tfindfirsti64(file_name.c_str(), &c_file)) != -1L)
	{
		if(!(c_file.attrib & _A_SUBDIR))  //디렉토리가 아니면 파일임.
		{
			bResult = true; 
		}
	}

	_findclose(hFile);
	return bResult;
}

bool Directory::is_exist_directory(const std::wstring& path)
{
	_tfinddatai64_t c_file;
	bool bResult = false;
	intptr_t hFile = _tfindfirsti64(const_cast<WCHAR*>(path.c_str()), &c_file);
	if (c_file.attrib & _A_SUBDIR ) //폴더라면
		bResult = true; //존재한다고 판단.

	_findclose(hFile);
	return bResult;
}

bool Directory::delete_directory(const std::wstring& path)
{
	SHFILEOPSTRUCT shFileOp = {0, }; 
	shFileOp.fFlags = FOF_NO_UI; //묻지않고 삭제
	shFileOp.wFunc = FO_DELETE;

	WCHAR ch = 0; // 끝에 NULL을 하나 더 붙여주기 위함
	std::wstring strAbsolutePath = path_canonicalize(path) + ch;
	shFileOp.pFrom = strAbsolutePath.c_str(); //shFileOp.pFrom 에 삭제할 폴더이름을 넣어줍니다. 여러개 폴더를 한번에 삭제할수도 있는데 널문자로 구분합니다. 마지막 폴더이름은 널문자2개로 끝나야 합니다.
	return SHFileOperation(&shFileOp) == S_OK ? true : false;
}

} //namespace light
