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
bool crafting_boost = true;
struct craftprompt {
	bool agn, stow, craft;
	Prompt prompt;

	void restore() {
		agn = false;
		stow = false;
		craft = false;
		prompt = -1;
	}
}crafting_prompt;

bool debug;
bool bInitialized;

void InitializeMenu()
{
	PRINT_INFO("Populating Menu...");
	g_Menu->AddSubmenu("Auto Crafting", "", Submenu_EntryMenu, 8, [](Submenu* sub)
		{
			sub->AddBoolOption("Fast Craft", "", &crafting_boost);
	});

	PRINT_INFO("Adding Hooks...");
	{
		uintptr_t exe_base = (uintptr_t)GetModuleHandle(NULL);
		PRINT_INFO("RDR2.exe ", exe_base);
		//Auto Crafting
		NHOOK("_UI_PROMPT_SET_TEXT", exe_base + 0x1061B48, {
			if (crafting_boost) {
				if (SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("player_camp")) {
					//if (debug) PRINT_INFO_SCRIPT("PromptId:", ctx->get_arg<Prompt>(0)," PromptText:", ctx->get_arg<const char*>(1));
					if (std::string(ctx->get_arg<const char*>(1)) == "CAMP_REC_COOK_AGN") {
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
				}
			}
			CALL("_UI_PROMPT_SET_TEXT");
			});
		NHOOK("_UI_PROMPT_REGISTER_END", exe_base + 0x1061794, {
			if (crafting_boost) {
				if (SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("player_camp")) {
					if (ctx->get_arg<Prompt>(0) == crafting_prompt.prompt) {
						if (crafting_prompt.agn || crafting_prompt.stow || crafting_prompt.craft) {
							HUD::_UI_PROMPT_SET_HOLD_AUTO_FILL_MODE(ctx->get_arg<Prompt>(0), 3500, 1000);
							if (debug) PRINT_INFO_SCRIPT("Modified Prompt ", ctx->get_arg<Prompt>(0));
						}
					}
				}
			}
			CALL("_UI_PROMPT_REGISTER_END");
			});
		NHOOK("_UI_PROMPT_IS_JUST_PRESSED", exe_base + 0x10616A0, {
			if (crafting_boost) {
				if (SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("player_camp")) {
					//if(debug) PRINT_INFO_SCRIPT("crafting_prompt.prompt:", crafting_prompt.prompt, " ctx->get_arg<Prompt>(0):", ctx->get_arg<Prompt>(0));
					if (crafting_prompt.prompt == ctx->get_arg<Prompt>(0)) {
						//PRINT_INFO("", crafting_prompt.agn, " ", crafting_prompt.craft, " ", crafting_prompt.stow, " ", HUD::_UI_PROMPT_HAS_MASH_MODE(crafting_prompt.prompt));
						if (HUD::_UI_PROMPT_HAS_HOLD_MODE_COMPLETED(ctx->get_arg<Prompt>(0))) {
							ctx->set_return_value<bool>(true);
							return;
						}
					}
				}
			}
			CALL("_UI_PROMPT_IS_JUST_PRESSED");
			});
	}

	bInitialized = true;
}


int main()
{
	#if ALLOCATE_CONSOLE
	AllocateConsole("Debug");
	#endif
	
	if(!g_Menu) g_Menu = std::make_unique<CNativeMenu>();
	if (!g_EventHandler) g_EventHandler = std::make_unique<CEventHandler>();
	if (!g_HookHandler) g_HookHandler = std::make_unique<CHookHandler>();

	if(!bInitialized) InitializeMenu(); // Make sure to call InitializeMenu() before calling any other CNativeMenu (g_Menu) function
	
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
	main();
}
