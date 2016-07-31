#pragma once

namespace TypeDefinition
{

class TendencyType
{
public:
	enum Type
	{
		Aggressive = 0, // 공격성
		Initiative = 1, // 적극성
		Thoughtful = 2, // 배려심
		lightly = 3, // 현명함
		Godliness = 4, // 신앙심
		Talent = 5, // 재능
		Ambition = 6, // 야망
	};
};

static const std::wstring TendencyType_STR[] = {
		L"공격성", 
		L"적극성", 
		L"배려심", 
		L"현명함", 
		L"신앙심", 
		L"재능", 
		L"야망", 
};

} // TypeDefinition

