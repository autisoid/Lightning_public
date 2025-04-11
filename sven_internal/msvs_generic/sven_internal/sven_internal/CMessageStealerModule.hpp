/**
 * Copyright - xWhitey, 2024.
 * CMessageStealerModule.hpp - description
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef CMESSAGESTEALERMODULE_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in CMessageStealerModule.hpp
#else //CMESSAGESTEALERMODULE_HPP_RECURSE_GUARD

#define CMESSAGESTEALERMODULE_HPP_RECURSE_GUARD

#ifndef CMESSAGESTEALERMODULE_HPP_GUARD
#define CMESSAGESTEALERMODULE_HPP_GUARD
#pragma once

#ifdef __cplusplus

typedef struct CMessageStealerModule : CModule {
	CMessageStealerModule();

	void OnEnable() override;
	void OnDisable() override;

	void OnEvent(_In_ const ISimpleEvent*) override;

	CBoolValue* m_pConcatenateRandomCharsAtTheEnd;
	CBoolValue* m_pConcatenateRandomCharsAtTheBeginning;
	CIntegerValue* m_pRandomCharactersLengthMin;
	CIntegerValue* m_pRandomCharactersLengthMax;

	struct CTrustedRandom* m_pRandomDevice;
} CMessageStealerModule;

using CMessageStealerModule = struct CMessageStealerModule;


#else //!__cplusplus
#error C++ compiler required to compile CMessageStealerModule.hpp
#endif //__cplusplus

#endif //CMESSAGESTEALERMODULE_HPP_GUARD

#undef CMESSAGESTEALERMODULE_HPP_RECURSE_GUARD
#endif //CMESSAGESTEALERMODULE_HPP_RECURSE_GUARD