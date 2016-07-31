#include "light.h"
#include "EnvironmentManager.h"

#include "ProcessUtil.h"
#include "FileUtil.h"
#include "DirectoryUtil.h"
#include "Seh.h"

#include "LeakTracking.h"
#include "MemoryUtil.hpp"

#include "CommandLineArguments.h"

namespace light
{

EnvironmentManager::EnvironmentManager(const std::wstring& program_name, const std::wstring& arg)
: m_program_name(program_name)
, m_active(FALSE)
{
	Directory::set_current_directory(get_module_path()); // 바이너리 생성 경로를 현재 디렉토리로 지정하자.
	LogHelper::initialize();

	std::locale::global(std::locale("kor")); //유니코드 한글 출력을 위한 것!

	CustomExceptionHandler::Install();

	Seh::Install();

	srand((unsigned int)time(NULL));

#ifdef _DEBUG
#else // _DEBUG
    LFH::ApplyAllHeap();
#endif // _DEBUG

    CommandLineArguments::parse(arg);
}

EnvironmentManager::~EnvironmentManager()
{
    LOG_INFO(L"complete");
    LogHelper::release();
}

void EnvironmentManager::setup(EnvironmentInterface* environment)
{
	m_environment.reset(environment);
	m_environment->initialize();	
}

void EnvironmentManager::on_complete()
{
    LOG_INFO(L"success");

    m_active.exchange(true);
	m_environment->on_complete();
}

void EnvironmentManager::stop()
{
    LOG_INFO(L"called");
    m_active.exchange(false);
}

} //namespace light
