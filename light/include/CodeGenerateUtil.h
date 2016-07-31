#pragma once

namespace light
{

class CodeGenrateUtil
{
protected:
	static std::wstring ToParameterType(const std::wstring& Type)
	{
		std::wstring str = Type;
		if( str == L"WCHAR" )
		{
			return str + L"*";
		}
		else
		{
			return str + L"&";
		}
	}

	static std::wstring ToVector(const std::wstring& Type)
	{
		return L"std::vector<" + Type + L">";
	}

	static std::wstring ToVectorReference(const std::wstring& Type)
	{
		return ToVector(Type) + L"&";
	}

protected:
	static void Include(std::wstringstream& stream, const std::wstring& File)
	{
		stream << L"#include " << WrapQuot(File) << std::endl; 
	}

protected:
	static void NameSpaceBegin(std::wstringstream& stream, const std::wstring& Name)
	{
		stream << L"namespace " << Name << std::endl;
		stream << L"{" << std::endl;
	}

	static void NameSpaceEnd(std::wstringstream& stream, const std::wstring& Name)
	{
		stream << L"} " << L"// " << Name << std::endl;
		stream << std::endl;
	}

protected: /// 클래스 선언용

	static void ClassBegin(std::wstringstream& stream, const std::wstring& ClassName, const std::wstring& ParentClass)
	{
		stream << L"class " << ClassName << L" : public " << ParentClass << std::endl;
		stream << L"{" << std::endl;
	}

	static void ClassBegin(std::wstringstream& stream, const std::wstring& ClassName)
	{
		stream << L"class " << ClassName << std::endl;
		stream << L"{" << std::endl;
	}

	static void ClassEnd(std::wstringstream& stream)
	{
		stream << L"};" << std::endl;
	}

protected: /// 생성자 관련 메소드
	static void ConstructorBegin(std::wstringstream& stream, const std::wstring& ClassName)
	{
		stream << std::tab << ClassName << L"()" << std::endl;
		stream << std::tab << L"{" << std::endl;
	}

	static void ConstructorEnd(std::wstringstream& stream)
	{
		stream << std::tab << L"}" << std::endl;
		stream << std::endl;
	}

protected: /// 간편한 유틸 메소드 지원
	/// vector resize method
	template <typename T>
	static void VectorResize(std::wstringstream& stream, const std::wstring& Variable, const T& Size)
	{
		stream << std::tab << std::tab << Variable << L".resize(" << Size << L");" << std::endl;
		stream << std::endl;
	}

	/// Member memset method
	static void Memset(std::wstringstream& stream, const std::wstring& Variable)
	{
		stream << std::tab << std::tab << L"memset(&" << Variable << L", 0, sizeof(" << Variable<< L"));" << std::endl;
		stream << std::endl;
	}

protected: // Class internal fuction
	/// get constant method
	static void ConstGetMethod(std::wstringstream& stream, const std::wstring& Type, const std::wstring& MethodName, const std::wstring& MemberVariable)
	{
		stream << std::tab << Type << L" " << MethodName << L"() const" << std::endl;
		stream << std::tab << L"{" << std::endl;
		stream << std::tab << std::tab << L"return " << MemberVariable << L";" << std::endl;
		stream << std::tab << L"}" << std::endl;
		stream << std::endl;
	}

	/// get method
	static void GetMethod(std::wstringstream& stream, const std::wstring& Type, const std::wstring& MethodName, const std::wstring& MemberVariable)
	{
		stream << std::tab << Type << L" " << MethodName << L"()" << std::endl;
		stream << std::tab << L"{" << std::endl;
		stream << std::tab << std::tab << L"return " << MemberVariable << L";" << std::endl;
		stream << std::tab << L"}" << std::endl;
		stream << std::endl;
	}

	/// set method
	static void SetMethod(std::wstringstream& stream, const std::wstring& Type, const std::wstring& MethodName, const std::wstring& MemberVariable)
	{
		stream << std::tab << L"void " << MethodName << L"(const " << TypeToReference(Type) << L" _" << MethodName << L")" << std::endl;
		stream << std::tab << L"{" << std::endl;
		stream << std::tab << std::tab << MemberVariable << L" = _" << MethodName << L";" << std::endl;
		stream << std::tab << L"}" << std::endl;
		stream << std::endl;
	}

	/// size method
	static void SizeMethod(std::wstringstream& stream, const std::wstring& MethodName, const std::wstring& MemberVariable)
	{
		stream << std::tab << L"size_t " << MethodName << L"Size()" << std::endl;
		stream << std::tab << L"{" << std::endl;
		stream << std::tab << std::tab << L"return _countof(" << MemberVariable << L");" << std::endl;
		stream << std::tab << L"}" << std::endl;
		stream << std::endl;
	}

protected: /// 메소드 코드 작성용 

	/// 파라미터 2개용
	static void MethodBegin(std::wstringstream& stream, const std::wstring& returnType, const std::wstring& Method, const std::wstring& ParamType1, const std::wstring& Param1, const std::wstring& ParamType2, const std::wstring& Param2)
	{
		stream << std::tab << returnType << L" " << Method << L"(" << ParamType1 << L" " <<  Param1 << L", " << ParamType2 << L" " <<  Param2 << L")" << std::endl;
		stream << std::tab << L"{" << std::endl;
	}

	/// 파라미터 1개용
	static void MethodBegin(std::wstringstream& stream, const std::wstring& returnType, const std::wstring& Method, const std::wstring& ParamType, const std::wstring& Param)
	{
		stream << std::tab << returnType << L" " << Method << L"(" << ParamType << L" " <<  Param << L")" << std::endl;
		stream << std::tab << L"{" << std::endl;
	}

	/// 파라미터 없을 때
	static void MethodBegin(std::wstringstream& stream, const std::wstring& returnType, const std::wstring& Method)
	{
		stream << std::tab << returnType << L" " << Method << L"()" << std::endl;
		stream << std::tab << L"{" << std::endl;
	}

	static void MethodCode(std::wstringstream& stream, const std::wstring& strcode)
	{
		stream << L"\t\t" << strcode << std::endl;
	}

	static void MethodEnd(std::wstringstream& stream)
	{
		stream << L"\t}" << std::endl;
		stream << std::endl;
	}

	static void EndLine(std::wstringstream& stream)
	{
		stream << std::endl;
	}

protected: // 인스턴스 연결 관련
	static std::wstring LinkToInstance(const std::wstring& Instance, const std::wstring& variable)
	{
		return Instance + L"." + variable;
	}

	/// 파라미터 3개용
	static std::wstring MemberFunctionCall(const std::wstring& Instance, const std::wstring& Method, const std::wstring& Param1, const std::wstring& Param2, const std::wstring& Param3)
	{
		return LinkToInstance(Instance, FunctionCall(Method, Param1, Param2, Param3));
	}

	/// 파라미터 2개용
	static std::wstring MemberFunctionCall(const std::wstring& Instance, const std::wstring& Method, const std::wstring& Param1, const std::wstring& Param2)
	{
		return LinkToInstance(Instance, FunctionCall(Method, Param1, Param2));
	}

	/// 파라미터 1개용
	static std::wstring MemberFunctionCall(const std::wstring& Instance, const std::wstring& Method, const std::wstring& Param)
	{
		return LinkToInstance(Instance, FunctionCall(Method, Param));
	}

	/// 파라미터 없을 때
	static std::wstring MemberFunctionCall(const std::wstring& Instance, const std::wstring& Method)
	{
		return LinkToInstance(Instance, FunctionCall(Method));
	}

protected: // 함수 호출 관련

	/// 파라미터 3개용
	static std::wstring FunctionCall(const std::wstring& Method, const std::wstring& Param1, const std::wstring& Param2, const std::wstring& Param3)
	{
		return Method + L"(" + Param1 + L", " + Param2 + L", " + Param3 + L")";
	}

	/// 파라미터 2개용
	static std::wstring FunctionCall(const std::wstring& Method, const std::wstring& Param1, const std::wstring& Param2)
	{
		return Method + L"(" + Param1 + L", " + Param2 + L")";
	}

	/// 파라미터 1개용
	static std::wstring FunctionCall(const std::wstring& Method, const std::wstring& Param)
	{
		return Method + L"(" + Param + L")";
	}

	/// 파라미터 없을 때
	static std::wstring FunctionCall(const std::wstring& Method)
	{
		return Method + L"()";
	}

protected: // 생성 /  선언 관련 메소드
	/// Global Declare variable
	static void GlobalDeclare(std::wstringstream& stream, const std::wstring& Type, const std::wstring& Variable)
	{
		stream << L"__declspec(selectany) " << Type << L" " << Variable << L";" << std::endl;
	}

	/// Declare variable
	static void Declare(std::wstringstream& stream, const std::wstring& Type, const std::wstring& MemberVariable)
	{
		stream << std::tab << Type << L" " << MemberVariable << L";" << std::endl;
	}

	/// Declare variable
	static void Declare(std::wstringstream& stream, const std::wstring& Type, const std::wstring& MemberVariable, const DWORD Size)
	{
		stream << std::tab << Type << L" " << MemberVariable << L"[" << Size << L"];" << std::endl;
	}

	static std::wstring Instance(const std::wstring& Type, const std::wstring& Name)
	{
		return Type + L" " + Name + L";";
	}

protected: // 타입 관련 메소드

	/// MapType
	static std::wstring MapType(const std::wstring& KeyType, const std::wstring& ValueType)
	{
		return L"std::map<" + KeyType + L", " + ValueType + L">";
	}

protected: // 접근 지정자 관련 메소드
	/// Public
	static void Public(std::wstringstream& stream)
	{
		stream << L"public:" << std::endl;
	}

	/// Private
	static void Private(std::wstringstream& stream)
	{
		stream << L"private:" << std::endl;
	}

protected: // 접두 접미 관련 메소드
	static std::wstring WrapQuot(const std::wstring& str)
	{
		return WrapWCHAR(L"\"" + str + L"\"");
	}

	static std::wstring WrapWCHAR(const std::wstring& str)
	{
		return L"L" + str;
	}

	static std::wstring MemberPrefix(const std::wstring& Name)
	{
		return Prefix(Name, L"m_");
	}

	static std::wstring Prefix(const std::wstring& Name, const std::wstring& prefix)
	{
		return prefix + Name;
	}

	static std::wstring Postfix(const std::wstring& Name, const std::wstring& postfix)
	{
		return Name + postfix;
	}

	static std::wstring TypeToReference(const std::wstring& Type)
	{
		return Postfix(Type, L"&");
	}

	static std::wstring TypeToConstReference(const std::wstring& Type)
	{
		return Postfix(Prefix(Type, L"const "), L"&");
	}
};


} // namespace light