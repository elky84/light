#include "stdafx.h"

#include "DirectoryUtil.h"

int _tmain(int argc, _TCHAR* argv[])
{
	light::Directory::set_current_directory(light::get_module_path()); // 바이너리 생성 경로를 현재 디렉토리로 지정하자.

	::testing::InitGoogleMock(&argc, argv);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
