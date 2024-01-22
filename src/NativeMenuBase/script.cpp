// Licensed under the MIT License.

#include "script.h"
#include "common.hpp"
#include "UI/Menu.hpp"
#include "UI/UIUtil.h"
#include "Submenus/Examples.hpp"

#include "eventhandler.hpp"
#include "hookhandler.hpp"
#include "util.hpp"
#include "globals.h"
#include "console.h"

#include "..\..\inc\MinHook.h"

#include "rage.hpp"

#include "scanner.hpp"

/*
#########################################################################
# RDR2 Native UI Menu Base
# Made by TuffyTown (Halen84)
# Menu base that is used to make similar menus to how they are in-game
# =======================================================================
# Credits: Alexander Blade (ScriptHook SDK), GitHub Contributors
# GitHub Link: https://github.com/Halen84/RDR2-Native-Menu-Base
# License: MIT
#########################################################################
*/


/*
HELP & INFO:

- The UI will look and function the best on 1920x1080 displays
- The "Submenus" folder is where you can put your functions for options. Its up to you how you want to organize everything.
- ^^^ You can also create the options in other files as long as you call it in InitializeMenu()
- To change keybinds, see UI/Menu.cpp in CheckInput()
- UI/Options/ contains all option types
- UI/Submenu.hpp is the Submenu class
- UI/Menu.cpp handles keypresses, navigation, some drawing, and other things
- UI/Menu.hpp is the CNativeMenu class
- UI/Drawing.cpp does the texture and text drawing for everything
*/
bool bInitialized = false;

bool crafting_boost = true;
struct craftprompt {
	bool agn, stow, craft;
	Prompt prompt;

	void restore() {
		agn = false;
		stow = false;
		craft = false;
		prompt = 0;
	}
}crafting_prompt;
std::uint64_t skipping_frame;

bool craft_triple = true;

rage::scrThread** m_CurrentScriptThread;
using _getnativehandler = rage::scrNativeHandler(*)(rage::scrNativeHash hash);
_getnativehandler GetNativeHandler;

bool IsScript(Hash hash) {
	return m_CurrentScriptThread && *m_CurrentScriptThread && (*m_CurrentScriptThread)->m_scriptHash == hash;
}

void InitializeMenu()
{
	g_Menu->AddSubmenu("AutoCraft", "", Submenu_EntryMenu, 8, [](Submenu* sub)
		{
			sub->AddBoolOption("AutoCraft", "", &crafting_boost);
			sub->AddBoolOption("3x Craft", "", &craft_triple);
	});

	PRINT_INFO("Adding Hooks...");
	{

		//Auto Crafting
		//NHOOK("_UI_PROMPT_SET_TEXT", exe_base + 0x1061B48, {
		//	if (!IsScript(954940763)) goto quit;
		//	
		//	if (crafting_boost) {
		//		PRINT_INFO_SCRIPT("Prompt:", ctx->get_arg<Prompt>(0), " Text:", ctx->get_arg<const char*>(1));
		//		if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_COOK_AGN") {
		//			crafting_prompt.restore();
		//			crafting_prompt.agn = true;
		//			crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
		//		}
		//		else if (std::string(ctx->get_arg<const char*>(1)) == "STOW_ITEM") {
		//			crafting_prompt.restore();
		//			crafting_prompt.stow = true;
		//			crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
		//		}
		//		else if (std::string(ctx->get_arg<const char*>(1)) == "CRAFT_FASTER") {
		//			crafting_prompt.restore();
		//			crafting_prompt.craft = true;
		//			crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
		//		}
		//		else if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_MAKE_AGN") {
		//			crafting_prompt.restore();
		//			crafting_prompt.agn = true;
		//			crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
		//		}
		//	}
		//	
		//
		//	quit:
		//	CALL("_UI_PROMPT_SET_TEXT");
		//	});
		//NHOOK("_UI_PROMPT_REGISTER_END", exe_base + 0x1061794, {
		//	if (!IsScript(954940763)) goto quit;
		//	
		//	if (crafting_boost) {
		//		//PRINT_INFO_SCRIPT("Logging Prompt:", crafting_prompt.prompt, "   ", ctx->get_arg<Prompt>(0));
		//		if (crafting_prompt.prompt != 0 && ctx->get_arg<Prompt>(0) == crafting_prompt.prompt) {
		//			if (crafting_prompt.stow || crafting_prompt.agn || crafting_prompt.craft) {
		//				HUD::_UI_PROMPT_SET_HOLD_AUTO_FILL_MODE(ctx->get_arg<Prompt>(0), 3800, 1000);
		//			}
		//		}
		//	}
		//
		//	quit:
		//	CALL("_UI_PROMPT_REGISTER_END");
		//	});
		//NHOOK("_UI_PROMPT_IS_JUST_PRESSED", exe_base + 0x10616A0, {
		//	if (!IsScript(954940763)) goto quit;
		//	
		//	if (crafting_boost) {
		//		//PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
		//		if (crafting_prompt.prompt != 0 && crafting_prompt.prompt == ctx->get_arg<Prompt>(0)) {
		//			PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
		//			PRINT_INFO("", crafting_prompt.agn, " ", crafting_prompt.stow);
		//			if (crafting_prompt.agn || crafting_prompt.stow || crafting_prompt.craft) {
		//				if (HUD::_UI_PROMPT_HAS_HOLD_MODE_COMPLETED(ctx->get_arg<Prompt>(0))) {
		//					ctx->set_return_value<bool>(true);
		//					return;
		//				}
		//			}
		//		}
		//	}
		//
		//	quit:
		//	CALL("_UI_PROMPT_IS_JUST_PRESSED");
		//	});
		//NHOOK("_UI_PROMPT_IS_PRESSED", exe_base + 0x10616E8, {
		//	if (!IsScript(954940763)) goto quit;
		//
		//	if (crafting_boost) {
		//		//PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
		//		if (crafting_prompt.prompt != 0 && crafting_prompt.prompt == ctx->get_arg<Prompt>(0)) {
		//			PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
		//			PRINT_INFO("", crafting_prompt.agn, " ", crafting_prompt.stow);
		//			if (crafting_prompt.agn || crafting_prompt.stow || crafting_prompt.craft) {
		//				if (HUD::_UI_PROMPT_HAS_HOLD_MODE_COMPLETED(ctx->get_arg<Prompt>(0))) {
		//					ctx->set_return_value<bool>(true);
		//					return;
		//				}
		//			}
		//		}
		//	}
		//
		//	quit:
		//	CALL("_UI_PROMPT_IS_PRESSED");
		//	});
		//
		////Triple Crafting
		//NHOOK("_INVENTORY_ADD_ITEM_WITH_GUID", exe_base + 0x10325CC, {
		//	if (!IsScript(954940763)) goto quit;
		//	
		//	if (craft_triple) {
		//		ctx->set_arg<int>(5, ctx->get_arg<int>(5) * 3);
		//	}
		//	
		//	quit:
		//	PRINT_INFO_SCRIPT("Added Item: ", ItemsHashToString(ctx->get_arg<Hash>(3)), " Amount: ", ctx->get_arg<int>(5), " ToSlot: ", SlotIdHashToString(ctx->get_arg<Hash>(4)));
		//	CALL("_INVENTORY_ADD_ITEM_WITH_GUID");
		//	});
		//NHOOK("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID", exe_base + 0x1032FA0, {
		//	if (!IsScript(954940763)) goto quit;
		//	
		//	if (craft_triple) {
		//		ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		//	}
		//
		//	quit:
		//	PRINT_INFO_SCRIPT("Removed Item: ", ItemsHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		//	CALL("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID");
		//	});
		//NHOOK("_REMOVE_AMMO_FROM_PED_BY_TYPE", exe_base + 0x10CE4FC, {
		//	if (!IsScript(954940763)) goto quit;
		//
		//	if (craft_triple) {
		//		ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		//	}
		//
		//	quit:
		//	PRINT_INFO_SCRIPT("Removed Ammo: ", AmmoHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		//	CALL("_REMOVE_AMMO_FROM_PED_BY_TYPE");
		//	});
		//NHOOK("_ADD_AMMO_TO_PED_BY_TYPE", exe_base + 0x10CA944, {
		//	if (!IsScript(954940763)) goto quit;
		//
		//	if (craft_triple) {
		//		ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		//	}
		//
		//	quit:
		//	PRINT_INFO_SCRIPT("Added Ammo: ", AmmoHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		//	CALL("_ADD_AMMO_TO_PED_BY_TYPE");
		//	});
		
		NHOOK("_ENABLE_HUD_CONTEXT_THIS_FRAME", 0xC9CAEAEEC1256E54, {
			if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

			if (crafting_boost) {
				if (ctx->get_arg<Hash>(0) == 0xE3FEFB3D) skipping_frame = MISC::GET_FRAME_COUNT();
			}

			quit:
			CALL("_ENABLE_HUD_CONTEXT_THIS_FRAME");
			});
		NHOOK("_UI_PROMPT_SET_TEXT", 0x5DD02A8318420DD7, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (crafting_boost) {
			PRINT_INFO_SCRIPT("Prompt:", ctx->get_arg<Prompt>(0), " Text:", ctx->get_arg<const char*>(1));
			if (skipping_frame == MISC::GET_FRAME_COUNT());
			else if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_COOK_AGN") {
				crafting_prompt.restore();
				crafting_prompt.agn = true;
				crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
			}
			else if (std::string(ctx->get_arg<const char*>(1)) == "STOW_ITEM") {
				crafting_prompt.restore();
				crafting_prompt.stow = true;
				crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
			}
			else if (std::string(ctx->get_arg<const char*>(1)) == "CRAFT_FASTER") {
				crafting_prompt.restore();
				crafting_prompt.craft = true;
				crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
			}
			else if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_MAKE_AGN") {
				crafting_prompt.restore();
				crafting_prompt.agn = true;
				crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
			}
			else if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_MAKE") {
				crafting_prompt.restore();
				crafting_prompt.craft = true;
				crafting_prompt.prompt = ctx->get_arg<Prompt>(0);
			}
		}


		quit:
		CALL("_UI_PROMPT_SET_TEXT");
		});
		NHOOK("_UI_PROMPT_REGISTER_END", 0xF7AA2696A22AD8B9, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (crafting_boost) {
			//PRINT_INFO_SCRIPT("Logging Prompt:", crafting_prompt.prompt, "   ", ctx->get_arg<Prompt>(0));
			if (crafting_prompt.prompt != 0 && ctx->get_arg<Prompt>(0) == crafting_prompt.prompt) {
				if (crafting_prompt.stow || crafting_prompt.agn || crafting_prompt.craft) {
					HUD::_UI_PROMPT_SET_HOLD_AUTO_FILL_MODE(ctx->get_arg<Prompt>(0), 3800, 1000);
				}
			}
		}

		quit:
		CALL("_UI_PROMPT_REGISTER_END");
		});
		NHOOK("_UI_PROMPT_IS_JUST_PRESSED", 0x2787CC611D3FACC5, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (crafting_boost) {
			//PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
			if (crafting_prompt.prompt != 0 && crafting_prompt.prompt == ctx->get_arg<Prompt>(0)) {
				PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
				PRINT_INFO("", crafting_prompt.agn, " ", crafting_prompt.stow);
				if (crafting_prompt.agn || crafting_prompt.stow || crafting_prompt.craft) {
					if (HUD::_UI_PROMPT_HAS_HOLD_MODE_COMPLETED(ctx->get_arg<Prompt>(0))) {
						ctx->set_return_value<bool>(true);
						return;
					}
				}
			}
		}

		quit:
		CALL("_UI_PROMPT_IS_JUST_PRESSED");
		});
		NHOOK("_UI_PROMPT_IS_PRESSED", 0x21E60E230086697F, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (crafting_boost) {
			//PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
			if (crafting_prompt.prompt != 0 && crafting_prompt.prompt == ctx->get_arg<Prompt>(0)) {
				PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
				PRINT_INFO("", crafting_prompt.agn, " ", crafting_prompt.stow);
				if (crafting_prompt.agn || crafting_prompt.stow || crafting_prompt.craft) {
					if (HUD::_UI_PROMPT_HAS_HOLD_MODE_COMPLETED(ctx->get_arg<Prompt>(0))) {
						ctx->set_return_value<bool>(true);
						return;
					}
				}
			}
		}

		quit:
		CALL("_UI_PROMPT_IS_PRESSED");
		});

		//Triple Crafting
		NHOOK("_INVENTORY_ADD_ITEM_WITH_GUID", 0xCB5D11F9508A928D, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (craft_triple) {
			ctx->set_arg<int>(5, ctx->get_arg<int>(5) * 3);
		}

		quit:
		PRINT_INFO_SCRIPT("Added Item: ", ItemsHashToString(ctx->get_arg<Hash>(3)), " Amount: ", ctx->get_arg<int>(5), " ToSlot: ", SlotIdHashToString(ctx->get_arg<Hash>(4)));
		CALL("_INVENTORY_ADD_ITEM_WITH_GUID");
		});
		NHOOK("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID", 0xB4158C8C9A3B5DCE, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (craft_triple) {
			ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		}

		quit:
		PRINT_INFO_SCRIPT("Removed Item: ", ItemsHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		CALL("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID");
		});
		NHOOK("_REMOVE_AMMO_FROM_PED_BY_TYPE", 0xB6CFEC32E3742779, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (craft_triple) {
			ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		}

		quit:
		PRINT_INFO_SCRIPT("Removed Ammo: ", AmmoHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		CALL("_REMOVE_AMMO_FROM_PED_BY_TYPE");
		});
		NHOOK("_ADD_AMMO_TO_PED_BY_TYPE", 0x106A811C6D3035F3, {
		if (!IsScript(954940763) && !IsScript(2790161247)) goto quit;

		if (craft_triple) {
			ctx->set_arg<int>(2, ctx->get_arg<int>(2) * 3);
		}

		quit:
		PRINT_INFO_SCRIPT("Added Ammo: ", AmmoHashToString(ctx->get_arg<Hash>(1)), " Amount: ", ctx->get_arg<int>(2));
		CALL("_ADD_AMMO_TO_PED_BY_TYPE");
		});
	}

	bInitialized = true;
}


void NativeMain()
{
	return;
}


#pragma warning(disable:28159)
void WaitAndDraw(unsigned ms)
{
	DWORD time = GetTickCount() + ms;
	bool waited = false;
	while (GetTickCount() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if (g_Menu) {
			g_Menu->Update();
		}
	}
}
#pragma warning(default:28159)

void ScriptMain()
{
#if ALLOCATE_CONSOLE
	AllocateConsole("Debug");
#endif

	scanner sc = scanner(nullptr);
	m_CurrentScriptThread = sc.scan("48 89 2D ? ? ? ? 48 89 2D ? ? ? ? 48 8B 04 F9").Add(3).Rip().As<rage::scrThread**>();
	GetNativeHandler = sc.scan("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().As<_getnativehandler>();
	
	if (!g_Menu) g_Menu = std::make_unique<CNativeMenu>();
	if (!g_EventHandler) g_EventHandler = std::make_unique<CEventHandler>();
	if (!g_HookHandler) g_HookHandler = std::make_unique<CHookHandler>();

	if (!bInitialized) InitializeMenu(); // Make sure to call InitializeMenu() before calling any other CNativeMenu (g_Menu) function

	g_Menu->GetSubmenu(Submenu_EntryMenu)->GetOption(g_Menu->GetSubmenu(Submenu_EntryMenu)->GetNumberOfOptions() - 1)->Execute();

	g_Menu->GoToSubmenu(Submenu_EntryMenu); // We only need to do this manually ONCE. It's automatic. See comment inside function.

	if (!UIUtil::GetScreenDimensions()) {
		PRINT_WARN("Failed to get Red Dead Redemption 2 game window dimensions. The UI may be sized incorrectly.");
	}

	while (true)
	{
		// This is required. Do not remove.
		// This makes the menu render everything.
		g_Menu->Update();
		g_EventHandler->Tick();

		WAIT(0);
	}
}
