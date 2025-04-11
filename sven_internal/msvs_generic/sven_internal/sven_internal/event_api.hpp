/**
 * Copyright - xWhitey, 2023.
 * event_api.hpp - A simple event api.
 */

#ifdef EVENT_API_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in event_api.hpp
#else //EVENT_API_HPP_RECURSE_GUARD

#define EVENT_API_HPP_RECURSE_GUARD

#ifndef EVENT_API_HPP_GUARD
#define EVENT_API_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif //_MSC_VER

typedef enum struct EEventType : signed char {
	kNotForUse = SCHAR_MIN,
	kUpdateEvent,
	kTickEvent, //UNUSED FOR NOW
	kRenderEvent,
	kMouseMoveEvent,
	kMouseBtnDownEvent,
	kMouseBtnUpEvent,
	kKeyEvent,
	kMoveEvent,
	kPreUpdateEvent,
	kPreMoveEvent,
	kPacketEvent,
	kPrePostRunCmdEvent,
	kPostPostRunCmdEvent,
	kWeaponChangedEvent,
	kCommandIssuedEvent,
	kPostCommandIssuedEvent,
	kNetworkUserCommandSubmitEvent,
	kStudioDrawPlayerEvent,
	kPostStudioDrawPlayerEvent
} EEventType;

typedef struct ISimpleEvent {
	ISimpleEvent(_In_ EEventType _EventType);

	EEventType GetType();

	EEventType GetType() const;

	int GetTypeAsInt();

	int GetTypeAsInt() const;

	mutable bool m_bHasBeenFired;
	EEventType m_eEventType;
protected:
	virtual ~ISimpleEvent() = default;
} ISimpleEvent;

using ISimpleEvent = struct ISimpleEvent;

typedef struct ICancellableEvent : ISimpleEvent {
	ICancellableEvent(_In_ EEventType _EventType);

	mutable bool m_bIsCancelled = false;

	void CancelEvent();

	void CancelEvent() const;
} ICancellableEvent;

using ICancellableEvent = struct ICancellableEvent;

typedef struct CAbstractEventListener {
	virtual ~CAbstractEventListener() = default;
	virtual void OnEvent(const ISimpleEvent*) = 0;
} CAbstractEventListener;

using CAbstractEventListener = struct CAbstractEventListener;

typedef struct CEventManager {
	CEventManager();
	~CEventManager();

	bool IsThisVeryElementUnique(CAbstractEventListener* _Listener) const;

	void RegisterListener(CAbstractEventListener* _Listener);

	void UnregisterListener(CAbstractEventListener* _Listener);

	void ResortAllListeners(CAbstractEventListener* _ToIgnore);

	void Post(_In_ const ISimpleEvent* _Event);

	void ProcessStalledEventsUnconditionally();

	void Shutdown();

	CLinkedList<CAbstractEventListener*>* m_pListeners;
	bool m_bAcceptingFurtherEventFires;
} CEventManager;

using CEventManager = struct CEventManager;

typedef struct CUpdateEvent : ISimpleEvent {
#ifndef LIGHTNING_TO_MINT_MC
	CUpdateEvent(_In_ usercmd_s* _Cmd, _In_ float _Frametime, _In_ int _Active) : ISimpleEvent(EEventType::kUpdateEvent) {
		m_pCmd = _Cmd;
		m_flFrametime = _Frametime;
		m_iActive = _Active;
	}

	usercmd_s* m_pCmd;
	float m_flFrametime;
	int m_iActive;
#endif //LIGHTNING_TO_MINT_MC
} CUpdateEvent;

using CUpdateEvent = struct CUpdateEvent;

typedef struct CTickEvent : ISimpleEvent {
	CTickEvent() : ISimpleEvent(EEventType::kTickEvent) {}
} CTickEvent;

using CTickEvent = struct CTickEvent;

typedef struct CRenderEvent : ISimpleEvent {
	CRenderEvent() : ISimpleEvent(EEventType::kRenderEvent) {}
} CRenderEvent;

using CRenderEvent = struct CRenderEvent;

typedef struct CMouseMoveEvent : ISimpleEvent {
	CMouseMoveEvent(_In_ int _X, _In_ int _Y) : ISimpleEvent(EEventType::kMouseMoveEvent), m_iX(_X), m_iY(_Y) {}
	
	int m_iX, m_iY;
} CMouseMoveEvent;

using CMouseMoveEvent = struct CMouseMoveEvent;

typedef struct CMouseBtnDownEvent : ICancellableEvent {
	CMouseBtnDownEvent(_In_ int _Button) : ICancellableEvent(EEventType::kMouseBtnDownEvent), m_iButton(_Button) {}

	int m_iButton;
} CMouseBtnDownEvent;

using CMouseBtnDownEvent = struct CMouseBtnDownEvent;

typedef struct CMouseBtnUpEvent : ICancellableEvent {
	CMouseBtnUpEvent(_In_ int _Button) : ICancellableEvent(EEventType::kMouseBtnUpEvent), m_iButton(_Button) {}
	
	int m_iButton;
} CMouseBtnUpEvent;

using CMouseBtnUpEvent = struct CMouseBtnUpEvent;

typedef enum struct EKeyAction : unsigned char {
	kDown = 0x0,
	kUp
} EKeyAction;

typedef struct CKeyEvent : ICancellableEvent {
	CKeyEvent(_In_ EKeyAction _Action, _In_ int _Button, _In_ int _Scancode) : ICancellableEvent(EEventType::kKeyEvent), m_eAction(_Action), m_iButton(_Button), m_iScancode(_Scancode) {}
	
	EKeyAction m_eAction;
	int m_iButton;
	int m_iScancode;
} CKeyEvent;

using CKeyEvent = struct CKeyEvent;

typedef struct CMoveEvent : ISimpleEvent {
#ifndef LIGHTNING_TO_MINT_MC
	CMoveEvent(_In_ usercmd_s* _Cmd, _In_ playermove_s* _PlayerMove) : ISimpleEvent(EEventType::kMoveEvent) {
		m_pCmd = _Cmd;
		m_pPlayerMove = _PlayerMove;
	}

	usercmd_s* m_pCmd;
	playermove_s* m_pPlayerMove;
#endif //LIGHTNING_TO_MINT_MC
} CMoveEvent;

using CMoveEvent = struct CMoveEvent;

typedef struct CPreUpdateEvent : ICancellableEvent {
#ifndef LIGHTNING_TO_MINT_MC
	CPreUpdateEvent(_In_ usercmd_s* _Cmd, _In_ float _Frametime) : ICancellableEvent(EEventType::kPreUpdateEvent) {
		m_pCmd = _Cmd;
		m_flFrametime = _Frametime;
	}

	usercmd_s* m_pCmd;
	float m_flFrametime;
#endif //LIGHTNING_TO_MINT_MC
} CPreUpdateEvent;

using CPreUpdateEvent = struct CPreUpdateEvent;

typedef struct CPreMoveEvent : ICancellableEvent {
	CPreMoveEvent() : ICancellableEvent(EEventType::kPreMoveEvent) {}
} CPreMoveEvent;

using CPreMoveEvent = struct CPreMoveEvent;

typedef struct CPacketEvent : ICancellableEvent {
	CPacketEvent(_In_ netchan_s* _Chan, _In_ int _DataSize, _In_ char* _Data) : ICancellableEvent(EEventType::kPacketEvent) {
		m_pChan = _Chan;
		m_iDataSize = _DataSize;
		m_pcData = _Data;
	}

	netchan_s* m_pChan;
	int m_iDataSize;
	char* m_pcData;
} CPacketEvent;

using CPacketEvent = struct CPacketEvent;

typedef struct CPrePostRunCmdEvent : ICancellableEvent {
	CPrePostRunCmdEvent(_In_ local_state_s* _From, _In_ local_state_s* _To, _In_ usercmd_s* _Cmd, _In_ int _Runfuncs, _In_ double _Time, _In_ unsigned int _RandomSeed) : ICancellableEvent(EEventType::kPrePostRunCmdEvent) {
		m_pFrom = _From;
		m_pTo = _To;
		m_pCmd = _Cmd;
		m_iRunfuncs = _Runfuncs;
		m_dblTime = _Time;
		m_uiRandomSeed = _RandomSeed;
	}

	local_state_s* m_pFrom, *m_pTo;
	usercmd_s* m_pCmd;
	int m_iRunfuncs;
	double m_dblTime;
	unsigned int m_uiRandomSeed;
} CPrePostRunCmdEvent;

using CPrePostRunCmdEvent = struct CPrePostRunCmdEvent;

typedef struct CPostPostRunCmdEvent : ISimpleEvent {
	CPostPostRunCmdEvent(_In_ local_state_s* _From, _In_ local_state_s* _To, _In_ usercmd_s* _Cmd, _In_ int _Runfuncs, _In_ double _Time, _In_ unsigned int _RandomSeed) : ISimpleEvent(EEventType::kPostPostRunCmdEvent) {
		m_pFrom = _From;
		m_pTo = _To;
		m_pCmd = _Cmd;
		m_iRunfuncs = _Runfuncs;
		m_dblTime = _Time;
		m_uiRandomSeed = _RandomSeed;
	}

	local_state_s* m_pFrom, *m_pTo;
	usercmd_s* m_pCmd;
	int m_iRunfuncs;
	double m_dblTime;
	unsigned int m_uiRandomSeed;
} CPostPostRunCmdEvent;

using CPostPostRunCmdEvent = struct CPostPostRunCmdEvent;

typedef struct CWeaponChangedEvent : ISimpleEvent {
	CWeaponChangedEvent(_In_ int _State, _In_ int _WeaponId) : ISimpleEvent(EEventType::kWeaponChangedEvent), m_iState(_State), m_iWeaponId(_WeaponId) {}

	int m_iState, m_iWeaponId;
} CWeaponChangedEvent;

using CWeaponChangedEvent = struct CWeaponChangedEvent;

typedef struct CCommandIssuedEvent : ICancellableEvent {
	CCommandIssuedEvent(_In_z_ const char* _Text) : ICancellableEvent(EEventType::kCommandIssuedEvent), m_pszText(_Text) {}

	const char* m_pszText;
} CCommandIssuedEvent;

using CCommandIssuedEvent = struct CCommandIssuedEvent;

typedef struct CPostCommandIssuedEvent : ISimpleEvent {
	CPostCommandIssuedEvent(_In_z_ const char* _Text) : ISimpleEvent(EEventType::kPostCommandIssuedEvent), m_pszText(_Text) {}

	const char* m_pszText;
} CPostCommandIssuedEvent;

using CPostCommandIssuedEvent = struct CPostCommandIssuedEvent;

typedef struct CNetworkUserCommandSubmitEvent : ICancellableEvent {
	CNetworkUserCommandSubmitEvent(_In_ usercmd_s* _To, _In_ usercmd_s* _From) : ICancellableEvent(EEventType::kNetworkUserCommandSubmitEvent) {
		m_pTo = _To;
		m_pFrom = _From;
	}

	usercmd_s* m_pTo;
	usercmd_s* m_pFrom;
} CNetworkUserCommandSubmitEvent;

using CNetworkUserCommandSubmitEvent = struct CNetworkUserCommandSubmitEvent;

typedef struct CStudioDrawPlayerEvent : ISimpleEvent {
	CStudioDrawPlayerEvent(_In_ int _Flags, _In_ entity_state_t* _State) : ISimpleEvent(EEventType::kStudioDrawPlayerEvent) {
		m_iFlags = _Flags;
		m_pState = _State;
	}

	int m_iFlags;
	entity_state_t* m_pState;
} CStudioDrawPlayerEvent;

using CStudioDrawPlayerEvent = struct CStudioDrawPlayerEvent;

typedef struct CPostStudioDrawPlayerEvent : ISimpleEvent {
	CPostStudioDrawPlayerEvent(_In_ int _Flags, _In_ entity_state_t* _State) : ISimpleEvent(EEventType::kPostStudioDrawPlayerEvent) {
		m_iFlags = _Flags;
		m_pState = _State;
	}

	int m_iFlags;
	entity_state_t* m_pState;
} CPostStudioDrawPlayerEvent;

using CPostStudioDrawPlayerEvent = struct CPostStudioDrawPlayerEvent;

#ifdef _MSC_VER
#pragma pack(pop)
#endif //_MSC_VER

#else //!__cplusplus
#error C++ compiler required to compile event_api.hpp
#endif //__cplusplus

#endif //EVENT_API_HPP_GUARD

#undef EVENT_API_HPP_RECURSE_GUARD
#endif //EVENT_API_HPP_RECURSE_GUARD