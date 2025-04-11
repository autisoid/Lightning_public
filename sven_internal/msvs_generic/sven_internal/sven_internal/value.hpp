/**
 * Copyright - xWhitey, 2023.
 * value.hpp - Our setting-system.
 */

#ifdef VALUE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in value.hpp
#else //VALUE_HPP_RECURSE_GUARD

#define VALUE_HPP_RECURSE_GUARD

#ifndef VALUE_HPP_GUARD
#define VALUE_HPP_GUARD
#pragma once

#ifdef __cplusplus

struct CVirtualConVar;

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VEC

typedef enum : signed char {
	kValueFlagNone = 0,
	kValueFlagDontSave = 1 << 0,
	kValueFlagDontRender = 1 << 1
} EValueFlag;

typedef enum : signed char {
	kValueTypeNone,
	kValueTypeBool,
	kValueTypeFloat,
	kValueTypeDouble,
	kValueTypeInteger,
	kValueTypeList,
	kValueTypeText,
	kValueTypeColour,
	kValueTypeExtra,
} EValueType;

typedef struct CValue {
private:
	typedef void(*_tOnceChangedCallback)(void* _UserData, void* _Value, void* _PreviousValue);
public:
	CValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ EValueType _ValueType);

	void AddFlag(_In_ EValueFlag _Flag);

	void TakeFlag(_In_ EValueFlag _Flag);

	bool HasFlag(_In_ EValueFlag _Flag);

	CVirtualConVar* GetConVar();

	void RegisterOnceValueChangedCallback(_In_ _tOnceChangedCallback _Callback, _In_opt_ void* _UserData);

	const char* m_pszName;
	char* m_pszConvarName;
	CVirtualConVar* m_pConVar;
	_tOnceChangedCallback m_pfnOnceValueChangedCB;
	void* m_pOnceValueChangedCBUserData;
	signed char m_eFlags;
	unsigned char m_eType;
protected:
	virtual ~CValue() = default;
} CValue;

using CValue = struct CValue;

typedef struct CBoolValue : CValue {
	CBoolValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ bool _DefaultValue);

	bool& Get();

	void Set(_In_ const bool& _Value);

	bool m_bDefaultValue;
	bool m_bCurrentValue;
} CBoolValue;

using CBoolValue = struct CBoolValue;

typedef struct CFloatValue : CValue {
	CFloatValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ float _Min, _In_ float _Max, _In_ float _DefaultValue);

	float& Get();

	void Set(_In_ const float& _Value);

	float m_fDefaultValue;
	float m_fCurrentValue;
	float m_fMinValue;
	float m_fMaxValue;
} CFloatValue;

using CFloatValue = struct CFloatValue;

typedef struct CIntegerValue : CValue {
	CIntegerValue(_In_ const char* _Name, _In_z_ char* _ConvarName, _In_ int _Min, _In_ int _Max, _In_ int _DefaultValue);

	int& Get();

	void Set(_In_ const int& _Value);

	int m_iDefaultValue;
	int m_iCurrentValue;
	int m_iMinValue;
	int m_iMaxValue;
} CIntegerValue;

using CIntegerValue = struct CIntegerValue;

typedef struct CListValue : CValue {
	CListValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ const char* _Modes, _In_ int _DefaultMode);

	int Get();

	void SetConVarValue(_In_ const int& _Value);

	const char* GetModeByIndex(_In_ const int& _Index);

	const char* m_pszModes;
	int m_nCurrentIdx;
	CVector<const char*>* m_rgpszModes;
} CListValue;

using CListValue = struct CListValue;

typedef struct CTextValue : CValue {
	CTextValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_z_ const char* _DefaultValue);

	const char* Get();

	void Set(_In_z_ char* _Text);

	char* m_pszText;
} CTextValue;

using CTextValue = struct CTextValue;

typedef struct CColourValue : CValue {
private:
	typedef void(*_tOnChangedCallback)(void*, float[4]);
public:
	CColourValue(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_opt_ _tOnChangedCallback _pfnCB, _In_opt_ void* _CBUserData, _In_ float _Red, _In_ float _Green, _In_ float _Blue, _In_opt_ float _Alpha = 1.f);

	float* Get();

	float m_aflRGBA[4];
	_tOnChangedCallback m_pfnOnceChangedCB;
	void* m_pCallbackUserData;
} CColourValue;

using CColourValue = struct CColourValue;

typedef struct CExtraValues : CValue {
private:
	typedef void(*_tWindowRenderCallback)(void*);
public:
	explicit CExtraValues(_In_z_ const char* _Name, _In_z_ char* _ConvarName, _In_ _tWindowRenderCallback _pfnCB, _In_opt_ void* _CBUserData);

	bool Get();
	void Set(_In_ const bool& _Value);

	bool m_bActive;
	_tWindowRenderCallback m_pfnWindowRenderCB;
	void* m_pCallbackUserData;
} CExtraValues;

using CExtraValues = struct CExtraValues;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VEC

#else //__cplusplus
#error C++ compiler required to compile value.hpp.
#endif //__cplusplus

#endif //VALUE_HPP_GUARD

#undef VALUE_HPP_RECURSE_GUARD
#endif //VALUE_HPP_RECURSE_GUARD