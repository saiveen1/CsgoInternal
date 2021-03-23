
#include "utils.h"


HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;

void Utils::AttachConsole()
{
    _old_out = GetStdHandle(STD_OUTPUT_HANDLE);
    _old_err = GetStdHandle(STD_ERROR_HANDLE);
    _old_in = GetStdHandle(STD_INPUT_HANDLE);

    ::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

    _out = GetStdHandle(STD_OUTPUT_HANDLE);
    _err = GetStdHandle(STD_ERROR_HANDLE);
    _in = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(_out,
        ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

    SetConsoleMode(_in,
        ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
        ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
}

/*
    * @brief Detach console
    *
    * Detach and destroy the attached console
    */
void Utils::DetachConsole()
{
    if (_out && _err && _in) {
        FreeConsole();

        if (_old_out)
            SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
        if (_old_err)
            SetStdHandle(STD_ERROR_HANDLE, _old_err);
        if (_old_in)
            SetStdHandle(STD_INPUT_HANDLE, _old_in);
    }
}

    /*
    * @brief Print to console
    *
    * Replacement to printf that works with the newly created console
    */
bool Utils::ConsolePrint(const char* fmt, ...)
{
	if (!_out)
		return false;

	char buf[1024];
	va_list va;

	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}

LPWSTR Utils::C2W(LPCSTR szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}
//将宽字节wchar_t*转化为单字节char*  
LPSTR Utils::W2C(LPCWSTR szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}

FLOAT Utils::GetDistance(Vec2 src, Vec2 dst)
{
	return sqrtf((src.x - dst.x) * (src.x - dst.x) + (src.y - dst.y) * (src.y - dst.y));
}

PIMAGE_NT_HEADERS Utils::GetNtHeaders(LPVOID pBuffer)
{
	return (PIMAGE_NT_HEADERS)((UINT_PTR)pBuffer + *((UINT_PTR*)((UINT_PTR)(pBuffer)+0x3c)));
}

PIMAGE_SECTION_HEADER Utils::GetSectionHeader(LPVOID pModule)
{
	auto pNtHeaders = GetNtHeaders(pModule);
	auto pFileHeader = (PIMAGE_FILE_HEADER)((UINT_PTR)pNtHeaders + 4);
	auto pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((UINT_PTR)pFileHeader + 0x14);
	auto pSectionHeader = (PIMAGE_SECTION_HEADER)((UINT_PTR)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);
	return pSectionHeader;
}

BYTE* Utils::PatternScan(LPVOID pModule, LPCSTR pattern)
{
	//特征码??转换为-1
	static auto pattern2Vec = [](LPCSTR pattern)
	{
		auto temp = (LPSTR)pattern;
		auto patternVec = std::vector<UINT>{};
		while (*temp)
		{
			if (*temp == '?')
			{
				temp += 2;
				patternVec.push_back(-1);
			}
			//直到找到数字返回16进制, 第二个参数被置为数字后的下一个字符, 此处为0x20
			else
				patternVec.push_back(strtoul(temp, &temp, 16));
			temp++;
		}
		return patternVec;
	};

	auto patternBytes = pattern2Vec((CHAR*)pattern);
	auto ptSize = patternBytes.size();
	auto ptData = patternBytes.data();

	auto numOfSections = GetNtHeaders(pModule)->FileHeader.NumberOfSections;
	auto pSectionHeader = GetSectionHeader(pModule);
	for (auto i = 0; i < numOfSections; i++, pSectionHeader++)
	{
		auto virtualAddr = pSectionHeader->VirtualAddress;
		auto virtualSize = pSectionHeader->Misc.VirtualSize;
		for (auto j = virtualAddr; j < virtualSize; j++)
		{
			BOOL bFound = TRUE;
			for (auto k = 0ul; k < ptSize; k++)
				if (((BYTE*)pModule)[j + k] != ptData[k])
					if (ptData[k] != -1)
					{
						bFound = FALSE;
						break;
					}
			if (bFound)
				return (BYTE*)((UINT_PTR)pModule + j);
		}
	}
	return NULL;
}