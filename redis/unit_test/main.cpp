#include "stdafx.h"

#include "DirectoryUtil.h"
#include "ProcessUtil.h"

int _tmain(int argc, _TCHAR* argv[])
{
	light::Directory::set_current_directory(light::get_module_path()); // 바이너리 생성 경로를 현재 디렉토리로 지정하자.

    light::Process::execute(L"..\\redis\\redis-server.exe", L"", false);

	::testing::InitGoogleMock(&argc, argv);
	::testing::InitGoogleTest(&argc, argv);

	int ret = RUN_ALL_TESTS();
    light::Process::execute(L"taskkill /F /IM redis-server.exe");
    return ret;
}
