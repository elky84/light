#include "light.h"
#include "FileUtil.h"
#include "DirectoryUtil.h"

#include <sys/types.h> 
#include <sys/stat.h> 

#define BOOST_FILESYSTEM_DEPRECATED 
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace light
{

std::wstring get_file_name(const std::wstring& name)
{
	std::wstring Result = name;
	std::wstring::size_type at = Result.find_last_of(L'\\') + 1;
	Result.erase(Result.begin(), Result.begin() + at);
	return Result;
}

std::wstring get_file_ext(const std::wstring& name)
{
	std::wstring Result = name;
	std::wstring::size_type at = Result.find_last_of(L'.');
	if(std::wstring::npos != at)
		Result.erase(Result.begin(), Result.begin() + at + 1);
	return Result;
}

std::wstring get_module_name()
{
	WCHAR szBuffer[MAX_PATH];
	::GetModuleFileName(NULL, szBuffer, MAX_PATH);
	return get_file_name(szBuffer);
}

std::wstring get_file_name_without_ext(const std::wstring& name, const std::wstring& delimeters)
{
	std::wstring Result = name;
	std::wstring::size_type at = Result.find_last_of(L'.');
	if(std::wstring::npos != at)
		Result.erase(Result.begin() + at, Result.end());

	at = Result.find_last_of(delimeters);
	if(std::wstring::npos != at)
		Result.erase(Result.begin(), Result.begin() + at + 1);

	return Result;
}

std::wstring get_module_name_without_ext()
{
	std::wstring name = get_module_name();
	return get_file_name_without_ext(name);
}

bool FileUtil::remove(const std::wstring& name)
{
	return boost::filesystem::remove(name);
}

bool FileUtil::move(const std::wstring& src, const std::wstring& dest)
{
	if( false == copy(src, dest) )
	{
		return false;
	}

	remove(src);
	return true;
}

bool FileUtil::rename(const std::wstring& src, const std::wstring& dest)
{
	boost::system::error_code ec;
	boost::filesystem::rename(src, dest, ec);
	return !ec;
}

bool FileUtil::copy(const std::wstring& src, const std::wstring& dest)
{
	boost::system::error_code ec;
	boost::filesystem::copy_file(src, dest);
	return !ec;
}

bool FileUtil::close(FILE*& fp)
{
	if(fp == NULL)
		return false;

	fclose(fp);
	fp = NULL;
	return true;
}

bool FileUtil::is_exist(const std::wstring& name) 
{
	boost::filesystem::path full_path( boost::filesystem::initial_path<boost::filesystem::path>() ); 
	full_path = boost::filesystem::system_complete( boost::filesystem::path( name.c_str() ) );

	return boost::filesystem::exists( full_path );
}

FILE* FileUtil::open(const std::wstring& name, const WCHAR* mode)
{
	FILE* fp = NULL;
	_wfopen_s(&fp, name.c_str(), mode);
	if(fp == NULL)
	{
		LOG_INFO(L"[fp == NULL].");
	}
	return fp;
}


bool FileUtil::create_time(const std::wstring& src, time_t& time)
{
	struct _stat Status;
	if( false == stat(src, Status))
	{
		return false;
	}
	time = Status.st_ctime;
	return true;
}

bool FileUtil::modify_time(const std::wstring& src, time_t& time)
{
	struct _stat Status;
	if( false == stat(src, Status))
	{
		return false;
	}
	time = Status.st_mtime;
	return true;
}

bool FileUtil::access_time(const std::wstring& src, time_t& time)
{
	struct _stat Status;
	if( false == stat(src, Status))
	{
		return false;
	}
	time = Status.st_atime;
	return true;
}

bool FileUtil::stat(const std::wstring& src, struct _stat& buf)
{
	if ( _tstat(src.c_str(), &buf) != 0 ) 
	{
		switch (errno) 
		{
		case ENOENT:
			LOG_ERROR(L"%s not found.", src.c_str());
			break;
		case EINVAL:
			LOG_ERROR(L"Invalid parameter to _stat.");
			break;
		default:
			LOG_ERROR(L"Unexpected error in _stat.");
			break;
		}
		return false;
	}
	return true;
}

void FileUtil::search(const std::wstring& filter, LIST_FILENAME& list)
{
    search(list, Directory::get_current_directory(), filter);
}

void FileUtil::search(LIST_FILENAME& list, const std::wstring& Path, const std::wstring& filter)
{
    boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());
    full_path = boost::filesystem::system_complete(boost::filesystem::path(Path.c_str()));

    if (false == boost::filesystem::exists(full_path))
    {
        LOG_ERROR(L"Not found %S", full_path.file_string().c_str());
        return;
    }

    std::list<std::wstring> listDirectory;
    if (false == boost::filesystem::is_directory(full_path))
    {
        LOG_ERROR(L"is not directory. [%S]", full_path.file_string().c_str());
        return;
    }

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr)
    {
        std::wstring name = STRING_TO_WSTRING(dir_itr->path().filename().file_string());
        try
        {
            if (boost::filesystem::is_directory(dir_itr->status()))
            {
                listDirectory.push_back(name);
            }
            else if (boost::filesystem::is_regular_file(dir_itr->status()))
            {
                if ((get_file_name_without_ext(name) == get_file_name_without_ext(filter) || get_file_name_without_ext(filter) == L"*") &&
                    (get_file_ext(name) == get_file_ext(filter) || get_file_ext(filter) == L"*"))
                {
                    list.push_back(STRING_TO_WSTRING(full_path.file_string()) + L"\\" + name);
                }
            }
            else
            {
                LOG_ERROR(L"other object. [%s]", name.c_str());
            }
        }
        catch (const std::exception &ex)
        {
            LOG_ERROR(L"exception raised. [%s] [%s]", name.c_str(), ex.what());
        }
    }

    for each(auto& val in listDirectory)
    {
        search(list, STRING_TO_WSTRING(full_path.file_string()) + L"\\" + val, filter);
    }
}

} //namespace light
