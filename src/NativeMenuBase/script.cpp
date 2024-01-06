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

bool killing_spree;
bool perfect_pelt = true;
bool debug = true;
bool perfect_stealth;

bool loot_multiply = true;
int loot_multiplier = 3;
std::vector<const char*>items = { "HERB_", "FAT", "_FEATHER"};

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

bool unlock_all;

void InitializeMenu()
{
	{
		g_EventHandler->Subscribe(eEventType::EVENT_PED_CREATED, "KILLINGSPREE", &killing_spree, [](int idx) {

			EVENT_PED_CREATED_DATA EventData{};

			if (SCRIPTS::GET_EVENT_DATA(0, idx, (Any*)&EventData, 1)) {
				if (EventData.Created != PLAYER::PLAYER_PED_ID() && ENTITY::DOES_ENTITY_EXIST(EventData.Created) && ENTITY::IS_AN_ENTITY(EventData.Created) && PED::IS_PED_HUMAN(EventData.Created) && !ENTITY::IS_ENTITY_DEAD(EventData.Created)) {
					if(!ENTITY::IS_ENTITY_A_MISSION_ENTITY(EventData.Created))PED::SET_PED_CONFIG_FLAG(EventData.Created, ePedScriptConfigFlags::PCF_DontInfluenceWantedLevel, true);
				}
			}
			});
		g_EventHandler->Subscribe(eEventType::EVENT_TICK, "DEBUG", &debug, [](int id) {
			static int* HonorPtr = (int*)getGlobalPtr(36616);
			static bool once;
			if (killing_spree) {
				for (int i = 0; i < 23; i++) {
					MISC::SET_BIT(HonorPtr, i);
				}
				once = true;
			}
			else if(once){
				for (int i = 0; i < 23; i++) {
					MISC::CLEAR_BIT(HonorPtr, i);
				}
				once = false;
			}
			});

		g_EventHandler->Subscribe(eEventType::EVENT_ENTITY_DAMAGED, "PERFECTSTEALTH", &perfect_stealth, [](int idx) {
			EVENT_ENTITY_DAMAGED_DATA EventData{};
			if (SCRIPTS::GET_EVENT_DATA(0, idx, (Any*)&EventData, 9)) {
				if (EventData.Attacker == PLAYER::PLAYER_PED_ID()) {
					DECORATOR::DECOR_SET_BOOL(EventData.Victim, "bIgnoreDamageChecking", true);
				}
			}
			});

		g_EventHandler->Subscribe(eEventType::EVENT_ENTITY_DESTROYED, "PERFECT_PELT",&perfect_pelt, [](int scriptindex) {
			
			EVENT_ENTITY_DESTROYED_DATA EventData{};

			//PRINT_INFO("EVENT_ENTITY_DESTROYED called! ");

			if (SCRIPTS::GET_EVENT_DATA(0, scriptindex, (Any*)&EventData, 9)) {
				if (ENTITY::GET_IS_ANIMAL(EventData.Victim)) {
					//PRINT_INFO("", ENTITY::IS_AN_ENTITY(EventData.Attacker), "  ", EventData.Attacker, "   ", PLAYER::PLAYER_PED_ID(), "   ", EventData.Victim, "   ", ENTITY::IS_ENTITY_DEAD(EventData.Victim) ? PED::GET_PED_SOURCE_OF_DEATH(EventData.Victim) : 0);

					if (PED::GET_PED_SOURCE_OF_DEATH(EventData.Victim) == PLAYER::PLAYER_PED_ID()) {
						PED::_SET_PED_QUALITY(EventData.Victim, ePedQuality::PQ_HIGH);
						PED::_SET_PLAYER_CURRENT_ANIMAL_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 10000.0f, 2, 3);
						PED::_UPDATE_ANIMAL_DAMAGE_MODIFIER(PLAYER::PLAYER_ID());
						PED::_SET_PED_DAMAGE_CLEANLINESS(EventData.Victim, ePedDamageCleanliness::PED_DAMAGE_CLEANLINESS_PERFECT);
						PRINT_INFO("Modified Animal Quality");
					}
				}
			}
		});
		g_EventHandler->Subscribe(eEventType::EVENT_SHOT_FIRED_BULLET_IMPACT, "PERFECT_PELT",&perfect_pelt, [](int id) {
			
			EVENT_SHOT_FIRED_BULLET_IMPACT_DATA EventData{};

			if (SCRIPTS::GET_EVENT_DATA(0, id, (Any*)&EventData, 1)) {
				if (ENTITY::GET_IS_ANIMAL(EventData.Hit)) {

					if (PED::GET_PED_SOURCE_OF_DEATH(EventData.Hit) == PLAYER::PLAYER_PED_ID()) {

						PED::_SET_PED_DAMAGE_CLEANLINESS(EventData.Hit, ePedDamageCleanliness::PED_DAMAGE_CLEANLINESS_PERFECT);
						PRINT_INFO("Modified Animal Quality");
					}
					//TASK::TASK_ANIMAL_WRITHE(EventData.Animal, 0, 0);
					//PRINT_INFO("Karma Horse Agitator");
				}
			}
		});
	
		g_EventHandler->Subscribe(eEventType::EVENT_INVENTORY_ITEM_PICKED_UP, "DEBUG",&debug,  [](int idx) {
			
			
			struct EVENT_INVENTORY_ITEM_PICKED_UP_DATA {
				alignas(8) Hash InventoryItemHash;
				alignas(8) Hash PickupEntityModelHash;
				alignas(8) bool isItemWasUsed;
				alignas(8) bool isBought;
				alignas(8) Entity PickupEntityId;
			}EventData{};
			static_assert(sizeof(EVENT_INVENTORY_ITEM_PICKED_UP_DATA) == 8 * 5);

			if (SCRIPTS::GET_EVENT_DATA(0, idx, (Any*)&EventData, 5)) {
				PRINT_INFO("Pickup| InvHash:", EventData.InventoryItemHash, " ItemEntityModel:", EventData.PickupEntityModelHash, " EntityID:", EventData.PickupEntityId, " IsBought:", EventData.isBought);
			}
		});
		g_EventHandler->Subscribe(eEventType::EVENT_TICK, "DEBUG", &debug, [](int id) {
			if (!SCRIPTS::HAS_SCRIPT_LOADED("satchel")) return;

			static Hash Item;
			if (_G(g_satchelData._bVisable) && Item != _G(g_satchelData.CurrItem)) {
				Item = _G(g_satchelData.CurrItem);
				PRINT_INFO("Inventory| LookingAt:", Item);
			}
		});
	}
	
	PRINT_INFO("Populating Menu...");
	g_Menu->AddSubmenu("Jim's menu", "Messing Around", Submenu_EntryMenu, 8, [](Submenu* sub)
		{
			//sub->AddVectorOption("Regular Option", "Regular Option Example", [] {
			//	UIUtil::PrintSubtitle("~COLOR_BLUE~Regular~s~ option function executed");
			//});
			sub->AddRegularOption("Tie Nearby Enemies", "Determined By RelationShip", [](Option*) {
				Ped WorldPeds[1024];
				int PedCount = worldGetAllPeds(WorldPeds, 1024);
				for (int i = 0; i < PedCount; i++) {
					if (!ENTITY::IS_ENTITY_DEAD(WorldPeds[i]) && PED::IS_PED_HUMAN(WorldPeds[i]) && WorldPeds[i] != PLAYER::PLAYER_PED_ID()) {
						if (GetRelation(WorldPeds[i], PLAYER::PLAYER_PED_ID()) > eRelationType::ACQUAINTANCE_TYPE_PED_IGNORE) {
							TASK::TASK_KNOCKED_OUT_AND_HOGTIED(WorldPeds[i], 0.01f, false);
							BlockHonor(WorldPeds[i]);
						}
					}
				}
				UIUtil::PrintSubtitle("~COLOR_BLUE~Tie Nearby Enemies~s~ option executed");
				});

			sub->AddRegularOption("Tie Nearby Non-Friendly", "Determined By RelationShip", [](Option*) {
				Ped WorldPeds[1024];
				int PedCount = worldGetAllPeds(WorldPeds, 1024);
				for (int i = 0; i < PedCount; i++) {
					if (!ENTITY::IS_ENTITY_DEAD(WorldPeds[i]) && PED::IS_PED_HUMAN(WorldPeds[i]) && WorldPeds[i] != PLAYER::PLAYER_PED_ID()) {
						if (GetRelation(WorldPeds[i], PLAYER::PLAYER_PED_ID()) >= eRelationType::ACQUAINTANCE_TYPE_PED_IGNORE) {
							TASK::TASK_KNOCKED_OUT_AND_HOGTIED(WorldPeds[i], 0.01f, false);
							BlockHonor(WorldPeds[i]);
						}
					}
				}

				UIUtil::PrintSubtitle("~COLOR_BLUE~Tie Nearby Non-Friendly~s~ option executed");
				});

			sub->AddRegularOption("Tie Nearby Peds", "Tie all nearby npcs", [](Option*) {
				Ped WorldPeds[1024];
				int PedCount = worldGetAllPeds(WorldPeds, 1024);
				for (int i = 0; i < PedCount; i++) {
					if (!ENTITY::IS_ENTITY_DEAD(WorldPeds[i]) && PED::IS_PED_HUMAN(WorldPeds[i]) && WorldPeds[i] != PLAYER::PLAYER_PED_ID()) {
						TASK::TASK_KNOCKED_OUT_AND_HOGTIED(WorldPeds[i], 0.01f, false);
						BlockHonor(WorldPeds[i]);
					}
				}

				UIUtil::PrintSubtitle("~COLOR_BLUE~Tie Nearby Peds~s~ option executed");
				});

			sub->AddRegularOption("Unarm Nearby Peds", "Make them drop their weapon", [](Option*) {
				Ped WorldPeds[1024];
				int PedCount = worldGetAllPeds(WorldPeds, 1024);
				for (int i = 0; i < PedCount; i++) {
					if (!ENTITY::IS_ENTITY_DEAD(WorldPeds[i]) && PED::IS_PED_HUMAN(WorldPeds[i]) && WorldPeds[i] != PLAYER::PLAYER_PED_ID()) {
						WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(WorldPeds[i], WEAPON::GET_BEST_PED_WEAPON(WorldPeds[i], false, false), 0, 0, 0, 0);
					}
				}
				});

			sub->AddBoolOption("Killing Spree", "Disable Wanted and Honor", &killing_spree, [](Option*) {
				Ped WorldPeds[1024];
				int PedCount = worldGetAllPeds(WorldPeds, 1024);
				for (int i = 0; i < PedCount; i++) {
					if (!ENTITY::IS_ENTITY_DEAD(WorldPeds[i]) && PED::IS_PED_HUMAN(WorldPeds[i]) && WorldPeds[i] != PLAYER::PLAYER_PED_ID()) {
						if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(WorldPeds[i])) PED::SET_PED_CONFIG_FLAG(WorldPeds[i], ePedScriptConfigFlags::PCF_DontInfluenceWantedLevel, true);
					}
				}
				});

			sub->AddRegularOption("Spawn Pickup", "Spawn Chest With Pickup", [](Option*) {
				Hash pickup_hash = GetInputHash();
				if (pickup_hash == -1) return;
				int number = GetInputNum();
				if (number == -1) return;
				SpawnPickup(pickup_hash, number);
				});

			sub->AddRegularOption("Spawn Pickup Item", "Spawn Chest With Specified Item Hash", [](Option*) {
				Hash Item = GetInputHash();
				if (Item == -1) return;
				int number = GetInputNum();
				if (number == -1) return;
				SpawnPickupItem(Item, number);
			});
			
			sub->AddBoolOption("Perfect Stealth", "", &perfect_stealth);

			sub->AddBoolOption("Loot Multiply", "", &loot_multiply, [](Option* exp) {
				if (!loot_multiply) {
					loot_multiplier = 1;
					return;
				}

				loot_multiplier = GetInputNum();
				if (loot_multiplier == -1) loot_multiplier = 1;
				});

			sub->AddBoolOption("Fast Craft", "", &crafting_boost);
			
			sub->AddBoolOption("Unlock All DLC", "", &unlock_all);

			sub->AddBoolOption("Debug", "", &debug, [](Option*) {
				UNLOCK::UNLOCK_SET_UNLOCKED(joaat("SP_WEAPON_DUALWIELD"), true);
				UNLOCK::UNLOCK_SET_VISIBLE(joaat("SP_WEAPON_DUALWIELD"), true);
				WEAPON::_SET_ALLOW_DUAL_WIELD(PLAYER::PLAYER_PED_ID(), true);
				
				PLAYER::_SET_DEADEYE_ABILITY_LOCKED(PLAYER::GET_PLAYER_INDEX(), 1, false);
				PLAYER::_SET_DEADEYE_ABILITY_LOCKED(PLAYER::GET_PLAYER_INDEX(), 2, false);
				PLAYER::_SET_DEADEYE_ABILITY_LOCKED(PLAYER::GET_PLAYER_INDEX(), 3, false);
				PLAYER::_SET_DEADEYE_ABILITY_LOCKED(PLAYER::GET_PLAYER_INDEX(), 4, false);
				PLAYER::_SET_DEADEYE_ABILITY_LOCKED(PLAYER::GET_PLAYER_INDEX(), 5, false);
				PLAYER::_SET_DEADEYE_ABILITY_LEVEL(PLAYER::GET_PLAYER_INDEX(), 5);

				*getGlobalPtr(g_savedGlobals.deadeyeLevel) = 5;
				})->Execute();

			// This submenu is created below (Submenu_Examples). This just creates an option for the page
			// When adding a new submenu option, make sure to actually create the submenu
			//sub->AddSubmenuOption("Submenu Example", "Submenu Option Example", Submenu_Examples);
	});

	PRINT_INFO("Adding Hooks...");
	{
		uintptr_t exe_base = (uintptr_t)GetModuleHandle(NULL);
		PRINT_INFO("RDR2.exe ", exe_base);

		//Stealth
		{
			NHOOK("GET_PLAYER_CURRENT_STEALTH_NOISE", exe_base + 0xFA46A8, {
			
				if (perfect_stealth) {
					ctx->set_return_value<float>(0.5f);
					return;
				}
				CALL("GET_PLAYER_CURRENT_STEALTH_NOISE");
				});
			NHOOK("IS_TARGET_PED_IN_PERCEPTION_AREA", exe_base + 0xFA7E04,  {
			
				if (perfect_stealth) {
					if (ctx->get_arg<Ped>(1) == PLAYER::PLAYER_PED_ID()) {
						ctx->set_return_value<bool>(false);
						return;
					}
				}
				CALL("IS_TARGET_PED_IN_PERCEPTION_AREA");
				});
			NHOOK("HAS_ENTITY_CLEAR_LOS_TO_ENTITY", exe_base + 0xF7B224,  {
			
				if (perfect_stealth) {
					ctx->set_return_value<bool>(false);
					return;
				}
				CALL("HAS_ENTITY_CLEAR_LOS_TO_ENTITY");
				});
			NHOOK("HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT", exe_base + 0xF7B25C, {
				if (perfect_stealth) {
					ctx->set_return_value<bool>(false);
					return;
				}
				CALL("HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT");
			});
			NHOOK("IS_PED_GOING_INTO_COVER", exe_base + 0xFA68A0,  {
			
				if (perfect_stealth) {
					ctx->set_return_value<bool>(true);
					return;
				}
				CALL("IS_PED_GOING_INTO_COVER");
				});
			NHOOK("_EVENT_GET_RECENT_EVENT", exe_base + 0xF79C04,  {
				CALL("_EVENT_GET_RECENT_EVENT");
				if (perfect_stealth) {
					Hash Event = *ctx->get_return_value<Hash>();
					switch (Event) {
						case eEventType::EVENT_ACQUAINTANCE_PED_DEAD:
						case eEventType::EVENT_PED_SEEN_DEAD_PED:
						case eEventType::EVENT_SHOCKING_DEAD_BODY:
						case eEventType::EVENT_DEAD_PED_FOUND:
						case eEventType::EVENT_SHOCKING_PROPERTY_DAMAGE:
						case eEventType::EVENT_SHOCKING_WINDOW_SMASHED:
						case eEventType::EVENT_SHOCKING_SEEN_PED_ROBBED:
						case eEventType::EVENT_SHOCKING_SEEN_PED_INTIMIDATED:
						case eEventType::EVENT_SHOCKING_SEEN_CAR_STOLEN:
						case eEventType::EVENT_SHOCKING_MOUNT_STOLEN:
						case eEventType::EVENT_SHOCKING_ENTITY_HOGTIED:
							ctx->set_return_value<Hash>(0);
					}
				}
				});
			NHOOK("_IS_WEAPON_SILENT", exe_base + 0x10CE154, {
			
				if (perfect_stealth) {
					ctx->set_return_value<bool>(true);
					return;
				}
				CALL("_IS_WEAPON_SILENT");
			});
			NHOOK("GET_PED_CROUCH_MOVEMENT", exe_base + 0xFA3A70, {
				if (perfect_stealth) {
					ctx->set_return_value<bool>(true);
					return;
				}
				CALL("GET_PED_CROUCH_MOVEMENT");
			});
			NHOOK("_IS_TRACKED_PED_VISIBILITY_PERCENTAGE_NOT_LESS_THAN", exe_base + 0xFA7E78, {
				if (perfect_stealth) {
					ctx->set_return_value<bool>(false);
					return;
				}
				CALL("_IS_TRACKED_PED_VISIBILITY_PERCENTAGE_NOT_LESS_THAN");
				});
			NHOOK("GET_PED_BLACKBOARD_SCRIPT_FLOAT", exe_base + 0xFA37A4, {
				if (perfect_stealth) {
					if (MISC::GET_HASH_KEY("stealth") == MISC::GET_HASH_KEY(ctx->get_arg<const char*>(1)));
					else if (MISC::GET_HASH_KEY("Stealth") == MISC::GET_HASH_KEY(ctx->get_arg<const char*>(1)));
					else {
						CALL("GET_PED_BLACKBOARD_SCRIPT_FLOAT");
						return;
					}
					ctx->set_return_value<float>(1.0f);
				}
			});
		} //Stealth // Stealth
		
		
		NHOOK("IS_DLC_PRESENT", exe_base + 0xF50EF8, {
			CALL("IS_DLC_PRESENT");
			if(unlock_all) ctx->set_return_value<bool>(true);
			return;
			});
	
		NHOOK("_INVENTORY_ADD_ITEM_WITH_GUID", exe_base + 0x10325CC,  {
			if (loot_multiply) {
				if (SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("short_update")) {
					const std::string& Item = ItemsHashToString(ctx->get_arg<Hash>(3));
					for (auto idx : items) {
						if (Item.contains(idx)) {
							ctx->set_arg<int>(5, ctx->get_arg<int>(5) * loot_multiplier);
							break;
						}
					}
				}
			}
			if (debug) {
				PRINT_INFO_SCRIPT("Added Item: ", ItemsHashToString(ctx->get_arg<Hash>(3)).c_str(), " Amount: ", ctx->get_arg<int>(5), " ToSlot: ", SlotIdHashToString(ctx->get_arg<Hash>(4)));
			}
			CALL("_INVENTORY_ADD_ITEM_WITH_GUID");
			});
		NHOOK("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID", exe_base + 0x1032FA0,  {
			if (debug) {
				PRINT_INFO_SCRIPT("Removed Item: ", ItemsHashToString(ctx->get_arg<Hash>(1)).c_str(), " Amount: ", ctx->get_arg<int>(2));
			}
			CALL("_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID");
			});

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
	//printf("case %lu:\n    return \"SLOTID_SATCHEL\";\ncase %lu:\n    return \"SLOTID_ACTIVE_HORSE\";\ncase %lu:\n    return \"SLOTID_UPGRADE\" %lu", MISC::GET_HASH_KEY("SLOTID_SATCHEL"), MISC::GET_HASH_KEY("SLOTID_ACTIVE_HORSE"), MISC::GET_HASH_KEY("SLOTID_UPGRADE"), MISC::GET_HASH_KEY("SLOTID_WARDROBE"));

	// Initialize this submenu
	//g_ExamplesSubmenu->Init();
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
