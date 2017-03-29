#pragma once

/// 파일 코드 페이지 경고 무시
#pragma warning(disable:4819) 

#pragma warning(disable:4091) 

#pragma warning(disable:4996) 

// 이름 잘리는 경고
#pragma warning (disable:4503) 

// 미사용 지역 함수 제거
#pragma warning (disable:4505) 

#include "Macro.h" 

#include "Util.h"

#define WIN32_LEAN_AND_MEAN

#ifndef WINVER				
#define WINVER 0x0601 // 사용 가능한 최소 OS 버전을 정의해주세요.
#endif //WINVER
         
#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0601 // 사용 가능한 최소 OS 버전을 정의해주세요.
#endif //_WIN32_WINNT

#pragma warning ( push )
#pragma warning ( disable:4005 ) 
#include <boost/asio.hpp>
#pragma warning ( pop )

///#include <windows.h> 보다 위에 선언되어야함. Winsock 충돌을 막기 위해~
#ifndef _WINSOCKAPI_ 
#define _WINSOCKAPI_
#endif //_WINSOCKAPI_

/// _WINSOCKAPI_ 매크로 선언하고 나서 윈도우 헤더 포함~
#include <WindowsX.h> 

#include "str.h"

/// 로그 기능
#include "LogHelper.h" 
