string UTIL_VectorToString(const CVector& in _Vector) {
    return formatFloat(_Vector[0]) + " " + formatFloat(_Vector[1]) + " " + formatFloat(_Vector[2]);
}

void MyBoolValueChangeCB(any@ _UserData, bool& out _Value, const bool& in _PreviousValue) {
	CBoolValue@ pTestBool = null;
	_UserData.retrieve(@_UserData);

	g_Console.Print("MyBoolValueChangeCB: changed from " + (_PreviousValue ? "true" : "false") + " to " + (_Value ? "true" : "false") + "\n");
}

class CTestModule {
    CBaseScriptModule@ m_pModule = null;
	CBoolValue@ m_pTestBool = null;

    CTestModule() {
        @m_pModule = CBaseScriptModule("Test", "Render", 0, true);
		@m_pTestBool = CBoolValue("This is a test bool value", "sc_test_bool_value", false);
        m_pModule.SetOnEnable(OnEnableCB(OnEnable));
        m_pModule.SetOnDisable(OnDisableCB(OnDisable));
        m_pModule.SetOnEvent(OnEventCB(OnEvent));
		m_pTestBool.RegisterOnceValueChangedCallback(MyBoolValueChangeCB, any(@m_pTestBool));
		m_pModule.AddValue(m_pTestBool);
    }
    
    void OnEnable() {
        g_EventManager.RegisterListener(@m_pModule);
    }
    
    void OnDisable() {
        g_EventManager.UnregisterListener(@m_pModule);
    }
    
    void OnEvent(ISimpleEvent@ _Event) {
        if (_Event.GetType() == kUpdateEvent) {
            const CUpdateEvent@ pEvent = cast<const CUpdateEvent@>(_Event);
            if (pEvent is null) return;
            usercmd_s@ pCmd = pEvent.m_pCmd;
            //pCmd.buttons |= IN_ATTACK;
            g_dblGameSpeed = 0.1 * 1000.0;
            string szSkyName = "";
            szSkyName.resize(32);
            movevars_s@ pMoveVars = get_g_MoveVars();
            int idx;
            for (idx = 0; idx < 32; idx++) {
                char ch = pMoveVars.skyName[idx];
                if (ch == 0) break;
                szSkyName[idx] = ch;
            }
            szSkyName.resize(idx);
            g_EngineFuncs.Con_Printf(szSkyName + "\n");
        }
    }
}

void ExampleCommand() {
    g_EngineFuncs.Con_Printf("Hello, World!\n");
}

void PluginInit() {
    CTestModule@ pModule = CTestModule();
    g_ModuleManager.RegisterModule(pModule.m_pModule);
    g_EngineFuncs.Cmd_AddCommand("sc_lightning_test", EngineCommandCB(ExampleCommand));
}