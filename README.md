# Lightning
Lightning (a.k.a. "allah client") (a.k.a. sven_internal between some people) - a Minecraft styled internal DLC for Sven Co-op, adding a lot of new features.

Written in C++. Authors: xWhitey, oxiKKK (Timestop mode in AirStuck).

Special thanks to: ReHLDS for exploits, Sw1ft747 for the idea and for some modules, a1++ for some exploits, sh0tx, guwi, desu.

Currently supported version of the game: 5.26 (lazily and probably crash-pronely supporting 5.25, Half-Life build 8684 (steam_legacy), 9920)

Featured by https://github.com/Noxturnix/sven_internal (originally located at https://github.com/Sw1ft747/sven_internal). Was meant to be an addition to that mod, became standalone in the end.

# Why release it?
There's a bozo in the game developer team, named Sniper, who's a bastard.

Now the game is going back to 5.25, when we had Sw1ft747's `sven_internal` :)

# How can I protect my server from haxxors using this DLC?
Use [ReHLDS_Sven](https://github.com/autisoid/ReHLDS_Sven). It might be unstable in the current moment, though I'm still working on it hardly. (Hi Sniper!)

# Features
- ClickGUI (**Right Shift** key by default)
- ESP (Wallhack)
- Bobbing (View bobbing like in Half-Life 2!)
- FrameSkipper (boost your fps!)
- HUD (display information about enabled modules + watermark)
- Various visual improvements (color your in-game HUD, custom crosshair!)
- **AntiSlowhack** (prevent nasty servers from executing commands on your client! Doesn't fully protect you from unauthorized command execution though)
- Ragebot (**NOT IN THIS VERSION**)
- Backtrack (cannot be used without **Ragebot**)
- Forwardtrack (cannot be used without **Ragebot**)
- Doubletap (useless)
- DiscordRPC
- AirStuck
- ConsoleSpammer (spam the console of the server you're playing on if it has https://github.com/wootguy/anticheat installed!)
- Disabler (**NOT IN THIS VERSION**, disables https://github.com/wootguy/anticheat entirely for you)
- FakeLatency (simulate high ping!)
- FastCrowbar
- InstantRespawn (upon turning on, if Survival Mode is inactive you suicide and get instantly respawned!)
- Kicker (**NOT IN THIS VERSION**, kicks all the players from the server you're playing on)
- SelfSink (useless in 5.26, automatically sinks you into the ground)
- ServerCrasher (**NOT IN THIS VERSION**, stall/crash the server you're playing on!)
- SlideWalk (**NOT IN THIS VERSION**, desync your lower body part for other players!)
- Stresser (**NOT IN THIS VERSION**, stress-test the server you're playing on! Expect heavy lags though :D)
- TimeMachine (LTFX2 speedhack, exclusive)
- ApplicationSpeed (Speedhack like in Cheat Engine)
- AutoFreezeOnChat (Automatically turns on AirStuck when you open up the chat)
- AutoTertiaryAttackGlitch
- MessageStealer (steal the messages other players send and re-send them with your name!)
- UserCmdRateLimiter (**NOT IN THIS VERSION**)
- AntiAim
- AutoJumpbug
- AutoJump
- FakeLag
- Speed (Speedhack)
- Strafe (Vectorial Autostrafer from Sw1ft747's `sven_internal`)

# How to install
In the beginning, you **_have_** to build the DLC on your own. I won't provide releases here at least for now. (#antiskid)

There are several ways to load up the DLC into the game. You can either use [SvenMod](https://github.com/autisoid/svenmod) and load this DLC as a SvenMod plugin, or use [MetaHookSv](https://github.com/hzqst/MetaHookSv) to load this DLC as a MetaHook plugin.

Since the DLC is fully standalone (i.e. it doesn't use any frameworks like SvenMod/MetaHook), you can just inject it like a normal DLL via DLL Injectors (e.g. Process Hacker)

The last way to load up the DLC is by using [svenmod_starloader](https://github.com/autisoid/svenmod_starloader) if you don't want to modify any of your game files.

# Console Variables/Commands
Some of the stuff in the DLC is configured via in-game console - use commands with `sc_lightning_` prefix (e.g. `sc_lightning_crosshair` commands family).

Each feature has a console command, which can be bound to a key (e.g. `bind capslock sc_lightning_speed`) but you also can bind features to keys via the ClickGUI by pressing MMB (middle mouse button) on a feature and then pressing the key you wish.

# Files of the DLC
The DLC uses `lightning.tech` folder located in the game's root folder. You have to take it from this repository and copy it over there, because if you don't do so the game will crash when you open up the ClickGUI (TODO: Fix maybe?)

This very folder is used to store all the configurable stuff, except convars. All modules and all their values are automatically saved each time you change something.

To make every convar you modify save, create a file named `lightning.cfg` in `svencoop` folder (it's located in the game's root folder too) and insert the convars you want there. This config will be executed each time the DLC is loaded up.

# Is this VAC safe?
Entirely. At least used to be. I have been playing with this DLC for more than a year on my main account and crashing servers, rage hacking for all the time being and no VAC bans so far. Though, after I release this, some nasty people might add detections for this very DLC, because now they might know its vulnerabilities.
