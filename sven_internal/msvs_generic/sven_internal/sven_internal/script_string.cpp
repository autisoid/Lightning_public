#include "StdAfx.h"
#include "script_string.hpp"

/*#include <unordered_map>  // std::unordered_map
typedef std::unordered_map<CString, int> map_t;

typedef struct CStringFactory final : asIStringFactory {
	CStringFactory() {}
	~CStringFactory() override
	{
		if (m_StringCache.size() != 0) {
			printf("CStringFactory: m_StringCache.size() isn't 0!\n");
		}
	}

	int  GetRawStringData(const void* _Str, char* _Data, asUINT* _Length) const override
	{
		if (_Str == 0)
			return asERROR;

		if (_Length)
			*_Length = static_cast<const CString*>(_Str)->Length();

		if (_Data)
			memcpy(_Data, static_cast<const CString*>(_Str)->CStr(), static_cast<const CString*>(_Str)->Length());

		return asSUCCESS;
	}

	const void* GetStringConstant(const char* _Data, asUINT _Length) override
	{
		// The string factory might be modified from multiple 
		// threads, so it is necessary to use a mutex.
		asAcquireExclusiveLock();

		CString str(_Data);
		map_t::iterator it = m_StringCache.find(str);
		if (it != m_StringCache.end())
			it->second++;
		else
			it = m_StringCache.insert(map_t::value_type(str, 1)).first;

		asReleaseExclusiveLock();

		return &it->first;
	}

	int  ReleaseStringConstant(const void* _Str) override
	{
		if (_Str == nullptr)
			return asERROR;

		int ret = asSUCCESS;

		// The string factory might be modified from multiple 
		// threads, so it is necessary to use a mutex.
		asAcquireExclusiveLock();

		if (const map_t::iterator it = m_StringCache.find(*static_cast<const CString*>(_Str)); it == m_StringCache.end())
			ret = asERROR;
		else
		{
			it->second--;
			if (it->second == 0)
				m_StringCache.erase(it);
		}

		asReleaseExclusiveLock();

		return ret;
	}

	map_t m_StringCache;
} CStringFactory;

using CStringFactory = struct CStringFactory;

static CStringFactory* g_pStringFactory = nullptr;

CStringFactory* GetStringFactorySingleton()
{
	if (g_pStringFactory == nullptr)
	{
		// Make sure no other thread is creating the string factory at the same time
		asAcquireExclusiveLock();
		if (g_pStringFactory == nullptr)
		{
			// The following instance will be destroyed by the global 
			// CStringFactoryCleaner instance upon application shutdown
			g_pStringFactory = Q_new(CStringFactory)();
		}
		asReleaseExclusiveLock();
	}
	return g_pStringFactory;
}

typedef struct CStringFactoryCleaner
{
	~CStringFactoryCleaner()
	{
		if (g_pStringFactory)
		{
			// Only delete the string factory if the stringCache is empty
			// If it is not empty, it means that someone might still attempt
			// to release string constants, so if we delete the string factory
			// the application might crash. Not deleting the cache would
			// lead to a memory leak, but since this is only happens when the
			// application is shutting down anyway, it is not important.
			if (g_pStringFactory->m_StringCache.empty())
			{
				Q_delete(g_pStringFactory);
				g_pStringFactory = nullptr;
			}
		}
	}
} CStringFactoryCleaner;

using CStringFactoryCleaner = struct CStringFactoryCleaner;

static CStringFactoryCleaner g_Cleaner;*/

void ConstructString(CString* _ThisInstance) {
	new (_ThisInstance) CString();
}

void CopyConstructString(const CString& _Other, CString* _ThisInstance) {
	new (_ThisInstance) CString(_Other);
}

void DestructString(CString* _ThisInstance) {
	_ThisInstance->~CString();
}

int StringPrintf(const CString& _Buffer, const CString* _Format, ...) {
	va_list va;
	va_start(va, _Format);
	const int iResult = _vsnprintf_s(_Buffer.Data(), _Buffer.Length() + 1, _Buffer.Length(), _Format->CStr(), va);
	va_end(va);

	return iResult;
}

void RegisterStringType(asIScriptEngine* _Engine)
{
	int r = 0;

	//(void)g_Cleaner; //shut up resharper and the compiler

	// Register the string type
#if AS_CAN_USE_CPP11
	// With C++11 it is possible to use asGetTypeTraits to automatically determine the correct flags to use
	r = _Engine->RegisterObjectType("string", sizeof(CString), asOBJ_VALUE | asGetTypeTraits<CString>());
#else
	r = _Engine->RegisterObjectType("string", sizeof(CString), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
#endif

	//r = _Engine->RegisterStringFactory("string", GetStringFactorySingleton());
	r = _Engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructString), asCALL_CDECL_OBJLAST);
	r = _Engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const string &in)", asFUNCTION(CopyConstructString), asCALL_CDECL_OBJLAST);
	r = _Engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructString), asCALL_CDECL_OBJLAST);
	r = _Engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asMETHODPR(CBasicString, operator =, (const CBasicString&), CBasicString&), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asMETHODPR(CBasicString, operator +=, (const CBasicString&), CBasicString&), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "bool opEquals(const string &in) const", asMETHODPR(CBasicString, operator ==, (const CBasicString&) const, bool), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asMETHODPR(CBasicString, Compare, (const CBasicString&) const, int), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asMETHODPR(CBasicString, operator +, (const CBasicString&), CBasicString), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "size_t Length() const", asMETHODPR(CBasicString, Length, () const, size_t), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "bool IsEmpty() const", asMETHODPR(CBasicString, IsEmpty, () const, bool), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "string &ToLowercase()", asMETHODPR(CBasicString, ToLowercase, (), CBasicString&), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "string &ToUppercase()", asMETHODPR(CBasicString, ToUppercase, (), CBasicString&), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "int Compare(const string &in) const", asMETHODPR(CBasicString, Compare, (const CBasicString&) const, int), asCALL_THISCALL);
	r = _Engine->RegisterObjectMethod("string", "int ICompare(const string &in) const", asMETHODPR(CBasicString, ICompare, (const CBasicString&) const, int), asCALL_THISCALL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
	r = _Engine->RegisterGlobalFunction("int sprintf(string& out _Buffer, const string& in _Format, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in, ?&in)", asFUNCTION(StringPrintf), asCALL_CDECL);
}