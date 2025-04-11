/**
 * Copyright - xWhitey, 2024.
 * StdAfx.h - our precompiled header
 *
 * Lightning a.k.a. lightning.tech (Sven Co-op) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef STDAFX_H_RECURSE_GUARD
#error Recursive header files inclusion detected in StdAfx.h
#else //STDAFX_H_RECURSE_GUARD

#define STDAFX_H_RECURSE_GUARD

#ifndef STDAFX_H_GUARD
#define STDAFX_H_GUARD
#pragma once

#ifdef __cplusplus

#include "common.hpp"
#include "globals.hpp"
#include "timer.hpp"
#include "linked_list.hpp"
#include "file_mgr.hpp"
#include "config_mgr.hpp"
#include "angelscript_scripting_api.hpp"
#include "movement_recorder.hpp"
#include "virtual_console.hpp"
#include "event_api.hpp"
#include "notifications.hpp"
#include "player_sp.hpp"
#include "value.hpp"
#include "module.hpp"
#include "module_manager.hpp"
#include "entity_descriptions.hpp"
#include "entity_list.hpp"
#include "hooks.hpp"
#include "cheat.hpp"
#include "globals.hpp"
#include "patterns.hpp"
#include "utils.hpp"
#include "dummyfile.hpp"

#include "string.hpp"

#include "angelscript/angelscript.h"

#else //!__cplusplus
#error C++ compiler required to compile StdAfx.h
#endif //__cplusplus

#endif //STDAFX_H_GUARD

#undef STDAFX_H_RECURSE_GUARD
#endif //STDAFX_H_RECURSE_GUARD

#pragma hdrstop