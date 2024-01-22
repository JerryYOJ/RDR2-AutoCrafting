#include "script.h"
#include "common.hpp"
#include "../../inc/enums.h"

#define PRINT_INFO_SCRIPT(...) PRINT("[From ", ScriptHashToString(SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME()).c_str(), "][INFO]: ", __VA_ARGS__);

bool IsHash(const std::string& str) {
	for (auto it = str.begin(); it != str.end(); it++) {
		if (std::isdigit(*it)) continue;
		else if (*it == '-' && it == str.begin()) continue;
		return false;
	}
	return true;
}

Hash joaat(const char* text)
{
	return IsHash(text) ? std::stoll(text) : MISC::GET_HASH_KEY(text);
}

std::string getClipboardText() {
	std::string ret;
	if (::OpenClipboard(NULL)) {
		HGLOBAL hGlobal = ::GetClipboardData(CF_UNICODETEXT);
		if (hGlobal != NULL) {
			LPWSTR lpszData = (LPWSTR)::GlobalLock(hGlobal);
			if (lpszData != nullptr) {
				int size = ::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, nullptr, 0, nullptr, nullptr);
				if (size > 0) {
					ret.resize(size + 1);
					::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, &ret[0], size, nullptr, nullptr);
				}
				::GlobalUnlock(hGlobal);
			}
		}
		::CloseClipboard();
	}
	return ret;
}

int RINT(int p1, int p2)
{
	return MISC::GET_RANDOM_INT_IN_RANGE(p1, p2);
}

int CreateBlipForPed(Hash style, Hash sprite, const char* blipName, Ped ped)
{
	int pedBlip = MAP::BLIP_ADD_FOR_ENTITY(style, ped);
	MAP::SET_BLIP_SPRITE(pedBlip, sprite, true);
	MAP::_SET_BLIP_NAME(pedBlip, blipName);
	MAP::SET_GPS_MULTI_ROUTE_RENDER(true);
	return pedBlip;
}

int CreateBlipForCoords(Hash style, Hash sprite, const char* blipName, Vector3 coords)
{
	int coordBlip = MAP::BLIP_ADD_FOR_COORDS(style, coords.x, coords.y, coords.z);
	MAP::SET_BLIP_SPRITE(coordBlip, sprite, true);
	MAP::_SET_BLIP_NAME(coordBlip, blipName);
	MAP::SET_GPS_MULTI_ROUTE_RENDER(true);
	return coordBlip;
}

std::string Vector3ToString(const Vector3& v)
{
	std::stringstream ss;
	ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return ss.str();
}

Vehicle getVehicle(Ped ped)
{
	return PED::GET_VEHICLE_PED_IS_USING(ped);
}

bool inVehicle(Ped ped)
{
	return PED::IS_PED_IN_ANY_VEHICLE(ped, true);
}

bool VehicleIsEmpty(Vehicle vehicle)
{
	bool empty{ true };
	int totalSeats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(vehicle));
	for (int i = 0; i < totalSeats; i++)
	{
		int seatPed = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, i);
		if (seatPed != 0 && !ENTITY::IS_ENTITY_DEAD(seatPed))
		{
			empty = false;
			break;
		}
	}

	return empty;
}

Vector3 GetCoords(Ped ped)
{
	return ENTITY::GET_ENTITY_COORDS(ped, true, true);
}

int CreatePed(Hash model, float posx, float posy, float posz, float heading, int outfitVariation)
{
	int ped{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	ped = PED::CREATE_PED(model, posx, posy, posz, heading, false, false, false, false);
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);
	PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, outfitVariation, false);
	PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	return ped;
}

int CreatePed(Hash model, Vector3 pos, float heading, int outfitVariation)
{
	int ped{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	ped = PED::CREATE_PED(model, pos, heading, false, false, false, false);
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);
	PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, outfitVariation, false);
	PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	return ped;
}

int CreatePedOnMount(Ped mount, Hash model, int seatIndex, int outfitVariation)
{
	int ped{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	ped = PED::CREATE_PED_ON_MOUNT(mount, model, seatIndex, false, false, false, false);
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);
	PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, outfitVariation, false);
	PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	return ped;
}

int CreateVehicle(Hash model, float posx, float posy, float posz, float heading)
{
	int vehicle{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	vehicle = VEHICLE::CREATE_VEHICLE(model, posx, posy, posz, heading, false, false, false, false);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle, true);
	return vehicle;
}

int CreateVehicle(Hash model, Vector3 pos, float heading)
{
	int vehicle{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	vehicle = VEHICLE::CREATE_VEHICLE(model, pos, heading, false, false, false, false);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle, true);
	return vehicle;
}

int CreatePedInVehicle(Vehicle vehicle, Hash model, int seatIndex, int outfitVariation)
{
	int ped{};
	STREAMING::REQUEST_MODEL(model, true);
	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(0);
	}
	ped = PED::CREATE_PED_INSIDE_VEHICLE(vehicle, model, seatIndex, false, false, false);
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);
	PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, outfitVariation, false);
	PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	return ped;
}

void GiveSaddleToHorse(Ped horse)
{
	PED::_APPLY_SHOP_ITEM_TO_PED(horse, joaat("HORSE_EQUIPMENT_CHARRO_01_IMPROVED_NEW_SADDLE_000"), true, false, false);
}

void UpdatePedOutfit(Ped ped, int outfitVariation)
{
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		PED::_EQUIP_META_PED_OUTFIT_PRESET(ped, outfitVariation, true);
		PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	}
}

int GetRandomOutfitPreset(Ped ped)
{
	int preset{};
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		preset = RINT(0, PED::GET_NUM_META_PED_OUTFITS(ped));
	}
	else
	{
		preset = 0;
	}
	return preset;
}

void SetRandomOutfitPreset(Ped ped)
{
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
		PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);
	}
}

void RequestTextureDict(const char* textureDict)
{
	if (!TXD::DOES_STREAMED_TEXTURE_DICT_EXIST(textureDict))
	{
		return;
	}
	else
	{
		TXD::REQUEST_STREAMED_TEXTURE_DICT(textureDict, true);
		while (!TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(textureDict)) { WAIT(0); }
	}
}

void RequestTextureHash(Hash textureHash)
{
	if (!TXD::DOES_STREAMED_TXD_EXIST(textureHash))
	{
		return;
	}
	else
	{
		TXD::REQUEST_STREAMED_TXD(textureHash, true);
		while (!TXD::HAS_STREAMED_TXD_LOADED(textureHash)) { WAIT(0); }
	}
}

void ReleaseTextureDict(const char* textureDict)
{
	if (!TXD::DOES_STREAMED_TEXTURE_DICT_EXIST(textureDict))
	{
		return;
	}
	else
	{
		TXD::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(textureDict);
	}
}

void ReleaseTextureHash(Hash textureHash)
{
	if (!TXD::DOES_STREAMED_TXD_EXIST(textureHash))
	{
		return;
	}
	else
	{
		TXD::SET_STREAMED_TXD_AS_NO_LONGER_NEEDED(textureHash);
	}
}

void DrawTextToScreen(const char* text, float x, float y, float scale, int r, int g, int b, int a)
{
	UIDEBUG::_BG_SET_TEXT_SCALE(scale, scale);
	UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(text))), x, y);
}

void DrawTextToScreen(int value, float x, float y, float scale, int r, int g, int b, int a)
{
	UIDEBUG::_BG_SET_TEXT_SCALE(scale, scale);
	UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(std::to_string(value).c_str()))), x, y);
}

void DrawTextToScreen(std::string text, float x, float y, float scale, int r, int g, int b, int a)
{
	UIDEBUG::_BG_SET_TEXT_SCALE(scale, scale);
	UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(text.c_str()))), x, y);
}

void DrawTextToScreen(Vector3 value, float x, float y, float scale, int r, int g, int b, int a)
{
	UIDEBUG::_BG_SET_TEXT_SCALE(scale, scale);
	UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);
	UIDEBUG::_BG_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", static_cast<char*>(_strdup(Vector3ToString(value).c_str()))), x, y);
}

void PlayFrontendAudio(const char* audioName, const char* audioRef)
{
	AUDIO::PLAY_SOUND_FRONTEND(audioName, audioRef, true, 0);
}

void StopFrontendAudio(const char* audioName, const char* audioRef)
{
	AUDIO::_STOP_SOUND_WITH_NAME(audioName, audioRef);
}



void kill_scripts(std::vector<Hash> scripts)
{
	SCRIPTS::SCRIPT_THREAD_ITERATOR_RESET();
	int ThreadID = SCRIPTS::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
	while (ThreadID > 0)
	{
		if (find(scripts.begin(), scripts.end(), SCRIPTS::_GET_HASH_OF_THREAD(ThreadID)) != scripts.end())
		{
			SCRIPTS::TERMINATE_THREAD(ThreadID);
		}
		ThreadID = SCRIPTS::SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
	}
}
//example priority PP_High
Prompt promptHandler(const char* promptText, Hash controlAction, int promptType, int priority)
{
	Prompt prompt{};
	prompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
	HUD::_UI_PROMPT_SET_CONTROL_ACTION(prompt, controlAction);
	HUD::_UI_PROMPT_SET_TEXT(prompt, MISC::VAR_STRING(10, "LITERAL_STRING", promptText));
	HUD::_UI_PROMPT_SET_STANDARD_MODE(prompt, promptType);
	HUD::_UI_PROMPT_SET_PRIORITY(prompt, priority);
	HUD::_UI_PROMPT_REGISTER_END(prompt);
	HUD::_UI_PROMPT_SET_ENABLED(prompt, false);
	HUD::_UI_PROMPT_SET_VISIBLE(prompt, false);
	return prompt;
}

void DisplayObjective(const char* objective)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(objective);
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}

void DisplayMissionName(const char* name, int duration)
{
	FeedData data{};
	FeedInfo info{};
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", name);
	data.duration = duration;
	UIFEED::_UI_FEED_POST_MISSION_NAME((Any*)&data, (Any*)&info, true);
}

void DisplayHelpText(const char* text, int duration)
{
	FeedData data{};
	FeedInfo info{};
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", text);
	data.duration = duration;
	UIFEED::_UI_FEED_POST_HELP_TEXT((Any*)&data, (Any*)&info, true);
}

void DisplayLeftToast(const char* title, const char* subtitle, const char* textureDictionary, const char* textureName, int duration)
{
	FeedData data{};
	FeedInfo info{};
	data.duration = duration;
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", title);
	info.subtitle = MISC::VAR_STRING(10, "LITERAL_STRING", subtitle);
	info.texture_dictionary_hash = MISC::GET_HASH_KEY(textureDictionary);
	info.texture_name_hash = MISC::GET_HASH_KEY(textureName);
	UIFEED::_UI_FEED_POST_SAMPLE_TOAST((Any*)&data, (Any*)&info, true, true);
}

void DisplayOneTextShard(const char* title, int duration)
{
	FeedData data{};
	FeedInfo info{};
	data.duration = duration;
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", title);
	UIFEED::_UI_FEED_POST_ONE_TEXT_SHARD((Any*)&data, (Any*)&info, true);
}

void DisplayTwoTextShard(const char* title, const char* subtitle, int duration)
{
	FeedData data{};
	FeedInfo info{};
	data.duration = duration;
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", title);
	info.subtitle = MISC::VAR_STRING(10, "LITERAL_STRING", subtitle);
	UIFEED::_UI_FEED_POST_TWO_TEXT_SHARD((Any*)&data, (Any*)&info, true, true);
}

void DisplayThreeTextShard(const char* title, const char* subtitle, const char* secondary_subtitle, int duration)
{
	FeedData data{};
	FeedInfo info{};
	data.duration = duration;
	info.title = MISC::VAR_STRING(10, "LITERAL_STRING", title);
	info.subtitle = MISC::VAR_STRING(10, "LITERAL_STRING", subtitle);
	info.secondary_subtitle = MISC::VAR_STRING(10, "LITERAL_STRING", secondary_subtitle);
	UIFEED::_UI_FEED_POST_THREE_TEXT_SHARD((Any*)&data, (Any*)&info, true, true, true);
}

void DisplayRightToast(const char* title, const char* imageDictionary, Hash imageHash, int bounceAmount, Hash color, const char* soundSet, const char* soundToPlay, int p8, bool toggle)
{
	SampleToastRightStruct1 data{};
	SampleToastRightStruct2 info{};

	data.SoundSet = soundSet;
	data.SoundToPlay = soundToPlay;
	data.f_3 = p8;
	data.Duration = 450;
	info.Title = title;
	info.ImageDictionary = imageDictionary;
	info.ImageHash = imageHash;
	info.BounceAmount = bounceAmount;
	info.Color = color;
	info.f_6 = 0;
	UIFEED::_UI_FEED_POST_SAMPLE_TOAST_RIGHT((Any*)&data, (Any*)&info, toggle);
}

Ped getClosestEnemy(float distance)
{
	Ped worldPeds[1024];
	int worldPedCount = worldGetAllPeds(worldPeds, 1024);
	std::vector<std::pair<int, float>> pv{};
	pv.clear();
	int p{};
	float f{};
	for (int i = 0; i < worldPedCount; i++)
	{
		if (!ENTITY::IS_ENTITY_DEAD(worldPeds[i]))
		{
			if (PED::GET_CURRENT_TARGET_FOR_PED(worldPeds[i]) == PLAYER::PLAYER_PED_ID())
			{
				f = MISC::GET_DISTANCE_BETWEEN_COORDS(ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true).z, ENTITY::GET_ENTITY_COORDS(worldPeds[i], true, true).x, ENTITY::GET_ENTITY_COORDS(worldPeds[i], true, true).y, ENTITY::GET_ENTITY_COORDS(worldPeds[i], true, true).z, true);
				if (f <= distance)
				{
					pv.push_back({ worldPeds[i], f });
				}
			}
		}
	}
	if (!pv.empty())
	{
		auto min_distance = std::min_element(pv.begin(), pv.end(),
			[](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});
		p = min_distance->first;
	}
	pv.erase(std::remove_if(pv.begin(), pv.end(),
		[](const std::pair<int, float>& v) {
			return ENTITY::IS_ENTITY_DEAD(v.first) ||
				PED::GET_CURRENT_TARGET_FOR_PED(v.first) != PLAYER::PLAYER_PED_ID();
		}), pv.end());
	return p;
}

void addPedToPlayerGroup(Ped ped)
{
	PED::SET_PED_AS_GROUP_MEMBER(ped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID()));
}
void createGroup(int& outGroup, int formation, float formationSeperation)
{
	outGroup = PED::CREATE_GROUP(0);
	PED::SET_GROUP_FORMATION(outGroup, formation);
	PED::SET_GROUP_FORMATION_SPACING(outGroup, formationSeperation, formationSeperation, formationSeperation);
}

/*TuffyTown (Halen84) Add Item To Inventory Functions https://www.rdr2mods.com/forums/topic/2139-addingremoving-items-to-inventory-using-natives/ */
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
sGuid CreateNewGUID()
{
	sGuid guid{};
	return guid;
}

sGuid GetPlayerInventoryItemGUID(Hash item, sGuid guid, Hash slotId)
{
	sGuid outGuid{};
	INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(1, (Any*)&guid, item, slotId, (Any*)&outGuid);
	return outGuid;

}

sGuid GetPlayerInventoryGUID()
{
	return GetPlayerInventoryItemGUID(MISC::GET_HASH_KEY("CHARACTER"), CreateNewGUID(), MISC::GET_HASH_KEY("SLOTID_NONE"));
}

Hash GetItemGroup(Hash item)
{
	sItemInfo info{};

	if (!ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(item, 0))
	{
		return 0;
	}
	if (!ITEMDATABASE::ITEMDATABASE_FILLOUT_ITEM_INFO(item, (Any*)&info))
	{
		return 0;
	}
	return info.f_2;
}

sSlotInfo GetItemSlotInfo(Hash item)
{
	sSlotInfo slotInfo{};

	slotInfo.guid = GetPlayerInventoryGUID();
	slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_SATCHEL");

	Hash group = GetItemGroup(item);
	switch (group)
	{
	case 0xC2286F01: // CLOTHING
		if (!INVENTORY::_INVENTORY_FITS_SLOT_ID(item, MISC::GET_HASH_KEY("SLOTID_WARDROBE")))
		{
			slotInfo.guid = GetPlayerInventoryItemGUID(MISC::GET_HASH_KEY("WARDROBE"), slotInfo.guid, MISC::GET_HASH_KEY("SLOTID_WARDROBE"));
			slotInfo.slotId = INVENTORY::_GET_DEFAULT_ITEM_SLOT_INFO(item, MISC::GET_HASH_KEY("WARDROBE"));
		}
		else
		{
			slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_WARDROBE");
		}
		break;
	case 0x95A6F147: // HORSE
		slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_ACTIVE_HORSE");
		break;
	case 0x80FB92CD: // UPGRADE
		if (INVENTORY::_INVENTORY_FITS_SLOT_ID(item, MISC::GET_HASH_KEY("SLOTID_UPGRADE")))
		{
			slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_UPGRADE");
		}
	default:
		if (INVENTORY::_INVENTORY_FITS_SLOT_ID(item, MISC::GET_HASH_KEY("SLOTID_SATCHEL")))
		{
			slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_SATCHEL");
		}
		else if (INVENTORY::_INVENTORY_FITS_SLOT_ID(item, MISC::GET_HASH_KEY("SLOTID_WARDROBE")))
		{
			slotInfo.slotId = MISC::GET_HASH_KEY("SLOTID_WARDROBE");
		}
		else
		{
			slotInfo.slotId = INVENTORY::_GET_DEFAULT_ITEM_SLOT_INFO(item, MISC::GET_HASH_KEY("CHARACTER"));
		}
		break;
	}
	return slotInfo;
}

bool AddItemWithGUID(Hash item, sGuid& guid, sSlotInfo& slotInfo, int quantity, Hash addReason)
{
	if (!INVENTORY::_INVENTORY_IS_GUID_VALID((Any*)&slotInfo.guid))
	{
		return false;
	}
	if (!INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(1, (Any*)&guid, (Any*)&slotInfo.guid, item, slotInfo.slotId, quantity, addReason))
	{
		return false;
	}
	return true;
}
bool AddItemToInventory(Hash item, int quantity)
{
	sSlotInfo slotInfo = GetItemSlotInfo(item);
	sGuid guid = GetPlayerInventoryItemGUID(item, slotInfo.guid, slotInfo.slotId);
	return AddItemWithGUID(item, guid, slotInfo, quantity, MISC::GET_HASH_KEY("ADD_REASON_DEFAULT"));

}

bool RemoveItemFromInventory(int inventoryid, Hash item, int quanity, Hash removeReason)
{
	return INVENTORY::_INVENTORY_REMOVE_INVENTORY_ITEM_WITH_ITEMID(inventoryid, item, quanity, removeReason);
}
/*back to none inventory functions*/

int GetMount(Ped ped)
{
	return PED::GET_MOUNT(ped);
}

Hash GetWeapon(Ped ped)
{
	Hash weaponHash{};
	WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, true, WEAPON_ATTACH_POINT_HAND_PRIMARY, true);
	return weaponHash;
}

Hash GetBestWeapon(Ped ped)
{
	return WEAPON::GET_BEST_PED_WEAPON(ped, true, true);
}

int GetAmmo(int ped)
{
	return WEAPON::GET_AMMO_IN_PED_WEAPON(ped, GetWeapon(ped));
}

int GetMaxClipAmmo(int ped)
{
	return WEAPON::GET_MAX_AMMO_IN_CLIP(ped, GetWeapon(ped), true);
}

int GetClipAmmo(int ped)
{
	int clipAmmo{};
	WEAPON::GET_AMMO_IN_CLIP(ped, &clipAmmo, GetWeapon(ped));
	return clipAmmo;
}

Object CreateObject(Hash model, Vector3 pos)
{
	Object obj{};
	if (STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model, true);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			WAIT(0);
		}
		obj = OBJECT::CREATE_OBJECT(model, pos, true, true, true, false, true);
		OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(obj, true);
	}
	return obj;
}

int GetPedTarget(Ped ped)
{
	return PED::GET_CURRENT_TARGET_FOR_PED(ped);
}

Volume CreateVolumeBox(float posx, float posy, float posz, float size)
{
	return VOLUME::CREATE_VOLUME_BOX(posx, posy, posz, 0.f, 0.f, 0.f, size, size, size);
}

Volume CreateVolumeSphere(float posx, float posy, float posz, float size)
{
	return VOLUME::CREATE_VOLUME_SPHERE(posx, posy, posz, 0.f, 0.f, 0.f, size, size, size);
}

void DismountPed(Ped ped)
{
	PED::_REMOVE_PED_FROM_MOUNT(ped, true, true);
}
// see enum eEnterExitVehicleFlags
void DismountAnyVehicle(Ped ped, int flags)
{
	TASK::TASK_LEAVE_ANY_VEHICLE(ped, 0, flags);
}

void AddWeaponToPed(Ped ped, Hash weaponHash, int ammoCount, int attachPoint, bool inHand)
{
	WEAPON::GIVE_WEAPON_TO_PED(ped, weaponHash, ammoCount, inHand, !inHand, attachPoint, false, 0.5f, 1.f, joaat("ADD_REASON_DEFAULT"), false, 0.f, false);
}

void RemoveAllWeapons(Ped ped)
{
	WEAPON::REMOVE_ALL_PED_WEAPONS(ped, true, true);
}

void RemoveCurrentWeapon(Ped ped)
{
	WEAPON::REMOVE_WEAPON_FROM_PED(ped, GetWeapon(ped), true, REMOVE_REASON_DEFAULT);
}

Hash GetWeaponFromGroup(Hash group)
{
	return WEAPON::_0xF8204EF17410BF43(group, 0.5f, 1.f, 0);
}

Hash GetWeaponFromGroups(std::vector<Hash> groups)
{
	std::vector<Hash> weapons{};
	for (int i = 0; i < groups.size(); i++)
	{
		weapons.push_back(WEAPON::_0xF8204EF17410BF43(groups[i], 0.5f, 1.f, 0));
	}
	return weapons[RINT(0, weapons.size())];
}

int GetIntStatValue(Hash BaseId, Hash PermutationId)
{
	int StatInt{};
	StatId Stat{};
	Stat.BaseId = BaseId;
	Stat.PermutationId = PermutationId;
	STATS::STAT_ID_GET_INT((Any*)&Stat, &StatInt);
	return StatInt;
}

void SetIntStatValue(Hash BaseId, Hash PermutationId, int value)
{
	StatId Stat{};
	Stat.BaseId = BaseId;
	Stat.PermutationId = PermutationId;
	STATS::STAT_ID_SET_INT((Any*)&Stat, value, true);
}

bool BlockHonor(Ped Target) {
	PED::SET_PED_CONFIG_FLAG(Target, ePedScriptConfigFlags::PCF_DisableHonorModifiers, true);

	if (ENTITY::IS_ENTITY_A_PED(Target)) {
		DECORATOR::DECOR_SET_INT(Target, "honor_block", 1);
		DECORATOR::DECOR_SET_INT(Target, "honor_override", 0);
		return true;
	}
	return false;
}

int GetRelation(Ped ped1, Ped ped2) {
	if (PED::IS_PED_IN_GROUP(ped1) && PED::IS_PED_IN_GROUP(ped2)) {
		return PED::GET_RELATIONSHIP_BETWEEN_GROUPS(PED::GET_PED_RELATIONSHIP_GROUP_HASH(ped1), PED::GET_PED_RELATIONSHIP_GROUP_HASH(ped2));
	}
	else {
		return PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped1, ped2);
	}
}

void SpawnPickup(Hash s_pickup_object, int num) {
	PRINT_INFO("Spawning| Pickup:", s_pickup_object, " Num:", num);
	
	Hash Chest = -1587197023;
	{
		if (!STREAMING::IS_MODEL_VALID(Chest)) {
			PRINT_INFO("Chest Hash Invalid!");
			return;
		}
		if (!STREAMING::IS_MODEL_VALID(s_pickup_object)) {
			PRINT_INFO("s_pickup Hash Invalid! ", s_pickup_object);
			return;
		}
	}

	{
		if (!STREAMING::HAS_MODEL_LOADED(Chest)) {
			STREAMING::REQUEST_MODEL(Chest, false);
			while (!STREAMING::HAS_MODEL_LOADED(Chest)) WAIT(0);
		}
		if (!STREAMING::HAS_MODEL_LOADED(s_pickup_object)) {
			STREAMING::REQUEST_MODEL(s_pickup_object, false);
			while (!STREAMING::HAS_MODEL_LOADED(s_pickup_object)) WAIT(0);
		}
	}


	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());

	Vector3 position = Vector3(coords.x + forward.x * 1.6, coords.y + forward.y * 1.6, coords.z + forward.z * 1.6);

	Object Chestobj = OBJECT::CREATE_OBJECT(Chest, position.x, position.y, position.z, true, true, false, false, true);
	//Object pickup_object = OBJECT::CREATE_OBJECT(s_pickup_object, position.x, position.y, position.z, true, true, false, false, true);

	std::vector<Object> pickup_objects;
	for (int i = 1; i <= num; i++) {
		pickup_objects.push_back(OBJECT::CREATE_OBJECT(s_pickup_object, position.x, position.y, position.z, true, true, false, false, true));
	}

	while (true) {
		WAIT(0);
		if (!ENTITY::DOES_ENTITY_EXIST(Chestobj)) continue;
		auto exist_check = [](std::vector<Object>& pickup_object) {
			for (auto object : pickup_object) {
				if (!ENTITY::DOES_ENTITY_EXIST(object)) {
					return false;
				}
			}
			return true;
			};
		if (!exist_check(pickup_objects)) continue;
		break;
	}

	OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(Chestobj, false);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Chestobj, true, false);

	for (auto pickup_object : pickup_objects) {
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pickup_object, true, false);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(pickup_object, Chestobj, 0, -0.045f, 0.03f, 0.30f, 0, 0, 0, false, false, false, false, 2, true, false, false);
		GRAPHICS::SET_PICKUP_LIGHT(pickup_object, true);
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(s_pickup_object);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Chest);

	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&Chestobj);
	for (auto pickup_object : pickup_objects) ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&pickup_object);
}

void SpawnPickupItem(Hash Item, int num) {
	PRINT_INFO("Spawning| Item:", Item, " Num:", num);

	if (!ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(Item, 0)) {
		PRINT_WARN("Item Hash Invalid!");
		return;
	}
	
	Hash Chest = 0xA1654BA1;
	Hash Carrier = 0xD0285DDD;
	if (!STREAMING::HAS_MODEL_LOADED(Carrier)) {
		STREAMING::REQUEST_MODEL(Carrier, false);
		while (!STREAMING::HAS_MODEL_LOADED(Carrier)) WAIT(0);
	}
	if (!STREAMING::HAS_MODEL_LOADED(Chest)) {
		STREAMING::REQUEST_MODEL(Chest, false);
		while (!STREAMING::HAS_MODEL_LOADED(Chest)) WAIT(0);
	}

	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::PLAYER_PED_ID());
	Vector3 position = Vector3(coords.x + forward.x * 1.6, coords.y + forward.y * 1.6, coords.z + forward.z * 1.6);

	Object ChestObj = CreateObject(Chest, position);
	std::vector<Object> pickups;
	for (int i = 1; i <= num; i++) pickups.push_back(CreateObject(Carrier, position));

	while (true) {
		WAIT(0);
		if (!ENTITY::DOES_ENTITY_EXIST(ChestObj)) continue;
		auto exist_check = [](std::vector<Object>& pickups) {
			for (auto object : pickups) {
				if (!ENTITY::DOES_ENTITY_EXIST(object)) {
					return false;
				}
			}
			return true;
			};
		if (!exist_check(pickups)) continue;
		break;
	}

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ChestObj, true, false);
	for (auto pickup_object : pickups) {
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pickup_object, true, false);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(pickup_object, ChestObj, 0, -0.045f, 0.03f, 0.30f, 0, 0, 0, false, false, false, false, 2, true, false, false);
		GRAPHICS::SET_PICKUP_LIGHT(pickup_object, true);
		TASK::_MAKE_OBJECT_CARRIABLE(pickup_object);
		TASK::__SET_OBJECT_PICKUPED_ITEM(pickup_object, Item);
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Carrier);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Chest);

	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ChestObj);
	for (auto pickup_object : pickups) ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&pickup_object);
}

Hash GetInputHash() {
	std::string defaultStr = "";
start:
	MISC::DISPLAY_ONSCREEN_KEYBOARD(0, "STABLE_RENAME_MOUNT_PROMPT", "", defaultStr.c_str(), "", "", "", 50);
	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
		if (GetAsyncKeyState(VK_LCONTROL) && IsKeyJustUp(0x56)) {
			MISC::CANCEL_ONSCREEN_KEYBOARD();
			defaultStr = getClipboardText();
			WAIT(0);
			goto start;
		}
		WAIT(0);
	}
	if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return -1;
	Hash ret = joaat(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	WAIT(0);
	return ret;
}

int GetInputNum() {
	MISC::DISPLAY_ONSCREEN_KEYBOARD(3, MISC::VAR_STRING(10, "LITERAL_STRING", "Enter Number"), "", "1", "", "", "", 50);
	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
		WAIT(0);
	}
	if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return -1;
	int ret = std::stoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	WAIT(0);
	return ret;
}

inline constexpr std::string ItemsHashToString(Hash Item) {
	switch (Item) {
	case CONSUMABLE_AGED_PIRATE_RUM:
		return "CONSUMABLE_AGED_PIRATE_RUM";
		break;
	case CONSUMABLE_APPLE:
		return "CONSUMABLE_APPLE";
		break;
	case CONSUMABLE_APRICOTS_CAN:
		return "CONSUMABLE_APRICOTS_CAN";
		break;
	case CONSUMABLE_BAKED_BEANS_CAN:
		return "CONSUMABLE_BAKED_BEANS_CAN";
		break;
	case CONSUMABLE_BISCUIT_BOX:
		return "CONSUMABLE_BISCUIT_BOX";
		break;
	case CONSUMABLE_BRANDY:
		return "CONSUMABLE_BRANDY";
		break;
	case CONSUMABLE_BREAD_ROLL:
		return "CONSUMABLE_BREAD_ROLL";
		break;
	case CONSUMABLE_CANDY_BAG:
		return "CONSUMABLE_CANDY_BAG";
		break;
	case CONSUMABLE_CARROT:
		return "CONSUMABLE_CARROT";
		break;
	case CONSUMABLE_CHEESE_WEDGE:
		return "CONSUMABLE_CHEESE_WEDGE";
		break;
	case CONSUMABLE_CHEWING_TOBACCO:
		return "CONSUMABLE_CHEWING_TOBACCO";
		break;
	case CONSUMABLE_CHOCOLATE_BAR:
		return "CONSUMABLE_CHOCOLATE_BAR";
		break;
	case CONSUMABLE_CIGAR:
		return "CONSUMABLE_CIGAR";
		break;
	case CONSUMABLE_CIGARETTE_BOX:
		return "CONSUMABLE_CIGARETTE_BOX";
		break;
	case CONSUMABLE_COCAINE_CHEWING_GUM:
		return "CONSUMABLE_COCAINE_CHEWING_GUM";
		break;
	case CONSUMABLE_COFFEE_GNDS_REG:
		return "CONSUMABLE_COFFEE_GNDS_REG";
		break;
	case CONSUMABLE_CORN:
		return "CONSUMABLE_CORN";
		break;
	case CONSUMABLE_CORNEDBEEF_CAN:
		return "CONSUMABLE_CORNEDBEEF_CAN";
		break;
	case CONSUMABLE_COVER_SCENT:
		return "CONSUMABLE_COVER_SCENT";
		break;
	case CONSUMABLE_CRACKERS:
		return "CONSUMABLE_CRACKERS";
		break;
	case CONSUMABLE_GIN:
		return "CONSUMABLE_GIN";
		break;
	case CONSUMABLE_GINSENG_ELIXIER:
		return "CONSUMABLE_GINSENG_ELIXIER";
		break;
	case CONSUMABLE_HAIR_GREASE:
		return "CONSUMABLE_HAIR_GREASE";
		break;
	case CONSUMABLE_HAIR_TONIC:
		return "CONSUMABLE_HAIR_TONIC";
		break;
	case CONSUMABLE_HERBIVORE_BAIT:
		return "CONSUMABLE_HERBIVORE_BAIT";
		break;
	case CONSUMABLE_HERB_ALASKAN_GINSENG:
		return "CONSUMABLE_HERB_ALASKAN_GINSENG";
		break;
	case CONSUMABLE_HERB_AMERICAN_GINSENG:
		return "CONSUMABLE_HERB_AMERICAN_GINSENG";
		break;
	case CONSUMABLE_HERB_BAY_BOLETE:
		return "CONSUMABLE_HERB_BAY_BOLETE";
		break;
	case CONSUMABLE_HERB_BLACK_BERRY:
		return "CONSUMABLE_HERB_BLACK_BERRY";
		break;
	case CONSUMABLE_HERB_BLACK_CURRANT:
		return "CONSUMABLE_HERB_BLACK_CURRANT";
		break;
	case CONSUMABLE_HERB_BURDOCK_ROOT:
		return "CONSUMABLE_HERB_BURDOCK_ROOT";
		break;
	case CONSUMABLE_HERB_CHANTERELLES:
		return "CONSUMABLE_HERB_CHANTERELLES";
		break;
	case CONSUMABLE_HERB_COMMON_BULRUSH:
		return "CONSUMABLE_HERB_COMMON_BULRUSH";
		break;
	case CONSUMABLE_HERB_CREEPING_THYME:
		return "CONSUMABLE_HERB_CREEPING_THYME";
		break;
	case CONSUMABLE_HERB_CURRANT:
		return "CONSUMABLE_HERB_CURRANT";
		break;
	case CONSUMABLE_HERB_DESERT_SAGE:
		return "CONSUMABLE_HERB_DESERT_SAGE";
		break;
	case CONSUMABLE_HERB_ENGLISH_MACE:
		return "CONSUMABLE_HERB_ENGLISH_MACE";
		break;
	case CONSUMABLE_HERB_EVERGREEN_HUCKLEBERRY:
		return "CONSUMABLE_HERB_EVERGREEN_HUCKLEBERRY";
		break;
	case CONSUMABLE_HERB_GINSENG:
		return "CONSUMABLE_HERB_GINSENG";
		break;
	case CONSUMABLE_HERB_GOLDEN_CURRANT:
		return "CONSUMABLE_HERB_GOLDEN_CURRANT";
		break;
	case CONSUMABLE_HERB_HUMMINGBIRD_SAGE:
		return "CONSUMABLE_HERB_HUMMINGBIRD_SAGE";
		break;
	case CONSUMABLE_HERB_INDIAN_TOBACCO:
		return "CONSUMABLE_HERB_INDIAN_TOBACCO";
		break;
	case CONSUMABLE_HERB_MILKWEED:
		return "CONSUMABLE_HERB_MILKWEED";
		break;
	case CONSUMABLE_HERB_OLEANDER_SAGE:
		return "CONSUMABLE_HERB_OLEANDER_SAGE";
		break;
	case CONSUMABLE_HERB_OREGANO:
		return "CONSUMABLE_HERB_OREGANO";
		break;
	case CONSUMABLE_HERB_PARASOL_MUSHROOM:
		return "CONSUMABLE_HERB_PARASOL_MUSHROOM";
		break;
	case CONSUMABLE_HERB_PRAIRIE_POPPY:
		return "CONSUMABLE_HERB_PRAIRIE_POPPY";
		break;
	case CONSUMABLE_HERB_RAMS_HEAD:
		return "CONSUMABLE_HERB_RAMS_HEAD";
		break;
	case CONSUMABLE_HERB_RED_RASPBERRY:
		return "CONSUMABLE_HERB_RED_RASPBERRY";
		break;
	case CONSUMABLE_HERB_RED_SAGE:
		return "CONSUMABLE_HERB_RED_SAGE";
		break;
	case CONSUMABLE_HERB_SAGE:
		return "CONSUMABLE_HERB_SAGE";
		break;
	case CONSUMABLE_HERB_VANILLA_FLOWER:
		return "CONSUMABLE_HERB_VANILLA_FLOWER";
		break;
	case CONSUMABLE_HERB_VIOLET_SNOWDROP:
		return "CONSUMABLE_HERB_VIOLET_SNOWDROP";
		break;
	case CONSUMABLE_HERB_WILD_CARROTS:
		return "CONSUMABLE_HERB_WILD_CARROTS";
		break;
	case CONSUMABLE_HERB_WILD_FEVERFEW:
		return "CONSUMABLE_HERB_WILD_FEVERFEW";
		break;
	case CONSUMABLE_HERB_WILD_MINT:
		return "CONSUMABLE_HERB_WILD_MINT";
		break;
	case CONSUMABLE_HERB_WINTERGREEN_BERRY:
		return "CONSUMABLE_HERB_WINTERGREEN_BERRY";
		break;
	case CONSUMABLE_HERB_YARROW:
		return "CONSUMABLE_HERB_YARROW";
		break;
	case CONSUMABLE_HORSE_MEDICINE:
		return "CONSUMABLE_HORSE_MEDICINE";
		break;
	case CONSUMABLE_HORSE_REVIVER:
		return "CONSUMABLE_HORSE_REVIVER";
		break;
	case CONSUMABLE_HORSE_STIMULANT:
		return "CONSUMABLE_HORSE_STIMULANT";
		break;
	case CONSUMABLE_JERKY:
		return "CONSUMABLE_JERKY";
		break;
	case CONSUMABLE_JERKY_VENISON:
		return "CONSUMABLE_JERKY_VENISON";
		break;
	case CONSUMABLE_KIDNEYBEANS_CAN:
		return "CONSUMABLE_KIDNEYBEANS_CAN";
		break;
	case CONSUMABLE_MEDICINE:
		return "CONSUMABLE_MEDICINE";
		break;
	case CONSUMABLE_MOONSHINE:
		return "CONSUMABLE_MOONSHINE";
		break;
	case CONSUMABLE_OAT_CAKES:
		return "CONSUMABLE_OAT_CAKES";
		break;
	case CONSUMABLE_OFFAL:
		return "CONSUMABLE_OFFAL";
		break;
	case CONSUMABLE_PEACH:
		return "CONSUMABLE_PEACH";
		break;
	case CONSUMABLE_PEACHES_CAN:
		return "CONSUMABLE_PEACHES_CAN";
		break;
	case CONSUMABLE_PEAR:
		return "CONSUMABLE_PEAR";
		break;
	case CONSUMABLE_PEAS_CAN:
		return "CONSUMABLE_PEAS_CAN";
		break;
	case CONSUMABLE_PINEAPPLES_CAN:
		return "CONSUMABLE_PINEAPPLES_CAN";
		break;
	case CONSUMABLE_POTENT_HORSE_MEDICINE:
		return "CONSUMABLE_POTENT_HORSE_MEDICINE";
		break;
	case CONSUMABLE_POTENT_HORSE_STIMULANT:
		return "CONSUMABLE_POTENT_HORSE_STIMULANT";
		break;
	case CONSUMABLE_POTENT_MEDICINE:
		return "CONSUMABLE_POTENT_MEDICINE";
		break;
	case CONSUMABLE_POTENT_RESTORATIVE:
		return "CONSUMABLE_POTENT_RESTORATIVE";
		break;
	case CONSUMABLE_POTENT_SNAKE_OIL:
		return "CONSUMABLE_POTENT_SNAKE_OIL";
		break;
	case CONSUMABLE_POTENT_TONIC:
		return "CONSUMABLE_POTENT_TONIC";
		break;
	case CONSUMABLE_PREDATOR_BAIT:
		return "CONSUMABLE_PREDATOR_BAIT";
		break;
	case CONSUMABLE_RESTORATIVE:
		return "CONSUMABLE_RESTORATIVE";
		break;
	case CONSUMABLE_RUM:
		return "CONSUMABLE_RUM";
		break;
	case CONSUMABLE_SALMON_CAN:
		return "CONSUMABLE_SALMON_CAN";
		break;
	case CONSUMABLE_SNAKE_OIL:
		return "CONSUMABLE_SNAKE_OIL";
		break;
	case CONSUMABLE_SPECIAL_HORSE_MEDICINE:
		return "CONSUMABLE_SPECIAL_HORSE_MEDICINE";
		break;
	case CONSUMABLE_SPECIAL_SNAKE_OIL_CRAFTED:
		return "CONSUMABLE_SPECIAL_SNAKE_OIL_CRAFTED";
		break;
	case CONSUMABLE_SPECIAL_TONIC_CRAFTED:
		return "CONSUMABLE_SPECIAL_TONIC_CRAFTED";
		break;
	case CONSUMABLE_STRAWBERRIES_CAN:
		return "CONSUMABLE_STRAWBERRIES_CAN";
		break;
	case CONSUMABLE_SWEET_CORN_CAN:
		return "CONSUMABLE_SWEET_CORN_CAN";
		break;
	case CONSUMABLE_TONIC:
		return "CONSUMABLE_TONIC";
		break;
	case CONSUMABLE_VALERIAN_ROOT:
		return "CONSUMABLE_VALERIAN_ROOT";
		break;
	case CONSUMABLE_WHISKEY:
		return "CONSUMABLE_WHISKEY";
		break;
	case PROVISION_ALLIGATOR_LEGENDARY_SKIN_PRISTINE:
		return "PROVISION_ALLIGATOR_LEGENDARY_SKIN_PRISTINE";
		break;
	case PROVISION_ALLIGATOR_LEGENDARY_TOOTH:
		return "PROVISION_ALLIGATOR_LEGENDARY_TOOTH";
		break;
	case PROVISION_ALLIGATOR_SKIN:
		return "PROVISION_ALLIGATOR_SKIN";
		break;
	case PROVISION_ALLIGATOR_SKIN_POOR:
		return "PROVISION_ALLIGATOR_SKIN_POOR";
		break;
	case PROVISION_ALLIGATOR_SKIN_PRISTINE:
		return "PROVISION_ALLIGATOR_SKIN_PRISTINE";
		break;
	case PROVISION_ALLIGATOR_TOOTH:
		return "PROVISION_ALLIGATOR_TOOTH";
		break;
	case PROVISION_ANIMAL_CARCASS_ALLIGATOR_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ALLIGATOR_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ALLIGATOR_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ALLIGATOR_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ALLIGATOR_POOR:
		return "PROVISION_ANIMAL_CARCASS_ALLIGATOR_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_ARMADILLO_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ARMADILLO_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ARMADILLO_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ARMADILLO_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ARMADILLO_POOR:
		return "PROVISION_ANIMAL_CARCASS_ARMADILLO_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BADGER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BADGER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BADGER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BADGER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BADGER_POOR:
		return "PROVISION_ANIMAL_CARCASS_BADGER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_BAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BEAVER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BEAVER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BEAVER_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BEAVER_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BEAVER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BEAVER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BEAVER_POOR:
		return "PROVISION_ANIMAL_CARCASS_BEAVER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BIGHORN_RAM_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BIGHORN_RAM_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_POOR:
		return "PROVISION_ANIMAL_CARCASS_BIGHORN_SHEEP_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BLUEJAY_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BLUEJAY_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BLUEJAY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BLUEJAY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BLUEJAY_POOR:
		return "PROVISION_ANIMAL_CARCASS_BLUEJAY_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BOBCAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BOBCAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BOBCAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BOBCAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BOBCAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_BOBCAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BUCK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BUCK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BUCK_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BUCK_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BUCK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BUCK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BUCK_POOR:
		return "PROVISION_ANIMAL_CARCASS_BUCK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_BULLFROG_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_BULLFROG_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_BULLFROG_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_BULLFROG_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_BULLFROG_POOR:
		return "PROVISION_ANIMAL_CARCASS_BULLFROG_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CALICONDOR_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CALICONDOR_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CALICONDOR_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CALICONDOR_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CALICONDOR_POOR:
		return "PROVISION_ANIMAL_CARCASS_CALICONDOR_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CARDINAL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CARDINAL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CARDINAL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CARDINAL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CARDINAL_POOR:
		return "PROVISION_ANIMAL_CARCASS_CARDINAL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CEDARWAXWING_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CEDARWAXWING_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CEDARWAXWING_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CEDARWAXWING_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CEDARWAXWING_POOR:
		return "PROVISION_ANIMAL_CARCASS_CEDARWAXWING_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CHICKEN_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CHICKEN_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CHICKEN_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CHICKEN_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CHICKEN_POOR:
		return "PROVISION_ANIMAL_CARCASS_CHICKEN_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CHIPMUNK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CHIPMUNK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CHIPMUNK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CHIPMUNK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CHIPMUNK_POOR:
		return "PROVISION_ANIMAL_CARCASS_CHIPMUNK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CORMORANT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CORMORANT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CORMORANT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CORMORANT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CORMORANT_POOR:
		return "PROVISION_ANIMAL_CARCASS_CORMORANT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_COUGAR_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_COUGAR_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_COUGAR_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_COUGAR_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_COUGAR_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_COUGAR_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_COUGAR_POOR:
		return "PROVISION_ANIMAL_CARCASS_COUGAR_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_COYOTE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_COYOTE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_COYOTE_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_COYOTE_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_COYOTE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_COYOTE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_COYOTE_POOR:
		return "PROVISION_ANIMAL_CARCASS_COYOTE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAB_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CRAB_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAB_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CRAB_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAB_POOR:
		return "PROVISION_ANIMAL_CARCASS_CRAB_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CRANE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CRANE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CRANE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CRANE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CRANE_POOR:
		return "PROVISION_ANIMAL_CARCASS_CRANE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAWFISH_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CRAWFISH_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAWFISH_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CRAWFISH_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CRAWFISH_POOR:
		return "PROVISION_ANIMAL_CARCASS_CRAWFISH_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CRLNAPARKET_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CRLNAPARKET_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CRLNAPARKET_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CRLNAPARKET_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CRLNAPARKET_POOR:
		return "PROVISION_ANIMAL_CARCASS_CRLNAPARKET_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_CROW_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_CROW_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_CROW_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_CROW_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_CROW_POOR:
		return "PROVISION_ANIMAL_CARCASS_CROW_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_DEER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_DEER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_DEER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_DEER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_DEER_POOR:
		return "PROVISION_ANIMAL_CARCASS_DEER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_DUCK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_DUCK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_DUCK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_DUCK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_DUCK_POOR:
		return "PROVISION_ANIMAL_CARCASS_DUCK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_EAGLE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_EAGLE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_EAGLE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_EAGLE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_EAGLE_POOR:
		return "PROVISION_ANIMAL_CARCASS_EAGLE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_EGRET_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_EGRET_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_EGRET_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_EGRET_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_EGRET_POOR:
		return "PROVISION_ANIMAL_CARCASS_EGRET_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_FOX_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_FOX_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_FOX_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_FOX_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_FOX_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_FOX_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_FOX_POOR:
		return "PROVISION_ANIMAL_CARCASS_FOX_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_GILA_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_GILA_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_GILA_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_GILA_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_GILA_POOR:
		return "PROVISION_ANIMAL_CARCASS_GILA_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_GOAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_GOAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_GOAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_GOAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_GOAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_GOAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_GOOSE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_GOOSE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_GOOSE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_GOOSE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_GOOSE_POOR:
		return "PROVISION_ANIMAL_CARCASS_GOOSE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_HAWK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_HAWK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_HAWK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_HAWK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_HAWK_POOR:
		return "PROVISION_ANIMAL_CARCASS_HAWK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_HERON_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_HERON_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_HERON_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_HERON_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_HERON_POOR:
		return "PROVISION_ANIMAL_CARCASS_HERON_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_IGUANA_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_IGUANA_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_IGUANA_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_IGUANA_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_IGUANA_POOR:
		return "PROVISION_ANIMAL_CARCASS_IGUANA_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_LOON_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_LOON_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_LOON_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_LOON_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_LOON_POOR:
		return "PROVISION_ANIMAL_CARCASS_LOON_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_MUSKRAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_MUSKRAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_MUSKRAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_MUSKRAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_MUSKRAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_MUSKRAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_OPOSSUM_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_OPOSSUM_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_OPOSSUM_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_OPOSSUM_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_OPOSSUM_POOR:
		return "PROVISION_ANIMAL_CARCASS_OPOSSUM_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_ORIOLE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ORIOLE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ORIOLE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ORIOLE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ORIOLE_POOR:
		return "PROVISION_ANIMAL_CARCASS_ORIOLE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_OWL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_OWL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_OWL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_OWL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_OWL_POOR:
		return "PROVISION_ANIMAL_CARCASS_OWL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PANTHER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PANTHER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PANTHER_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PANTHER_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PANTHER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PANTHER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PANTHER_POOR:
		return "PROVISION_ANIMAL_CARCASS_PANTHER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PARROT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PARROT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PARROT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PARROT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PARROT_POOR:
		return "PROVISION_ANIMAL_CARCASS_PARROT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PELICAN_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PELICAN_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PELICAN_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PELICAN_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PELICAN_POOR:
		return "PROVISION_ANIMAL_CARCASS_PELICAN_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PHEASANT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PHEASANT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PHEASANT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PHEASANT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PHEASANT_POOR:
		return "PROVISION_ANIMAL_CARCASS_PHEASANT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PIGEON_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PIGEON_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PIGEON_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PIGEON_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PIGEON_POOR:
		return "PROVISION_ANIMAL_CARCASS_PIGEON_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PIG_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PIG_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PIG_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PIG_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PIG_POOR:
		return "PROVISION_ANIMAL_CARCASS_PIG_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_POOR:
		return "PROVISION_ANIMAL_CARCASS_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PRARECHICK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PRARECHICK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PRARECHICK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PRARECHICK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PRARECHICK_POOR:
		return "PROVISION_ANIMAL_CARCASS_PRARECHICK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_PRONGHORN_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_PRONGHORN_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_PRONGHORN_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PRONGHORN_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PRONGHORN_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_PRONGHORN_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_PRONGHORN_POOR:
		return "PROVISION_ANIMAL_CARCASS_PRONGHORN_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_QUAIL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_QUAIL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_QUAIL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_QUAIL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_QUAIL_POOR:
		return "PROVISION_ANIMAL_CARCASS_QUAIL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_RABBIT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_RABBIT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_RABBIT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_RABBIT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_RABBIT_POOR:
		return "PROVISION_ANIMAL_CARCASS_RABBIT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_RACOON_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_RACOON_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_RACOON_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_RACOON_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_RACOON_POOR:
		return "PROVISION_ANIMAL_CARCASS_RACOON_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_RAM_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_RAM_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_RAM_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_RAM_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_RAM_POOR:
		return "PROVISION_ANIMAL_CARCASS_RAM_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_RAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_RAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_RAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_RAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_RAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_RAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_RAVEN_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_RAVEN_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_RAVEN_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_RAVEN_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_RAVEN_POOR:
		return "PROVISION_ANIMAL_CARCASS_RAVEN_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_REDFTBOOBY_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_REDFTBOOBY_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_REDFTBOOBY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_REDFTBOOBY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_REDFTBOOBY_POOR:
		return "PROVISION_ANIMAL_CARCASS_REDFTBOOBY_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_ROBIN_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ROBIN_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ROBIN_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ROBIN_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ROBIN_POOR:
		return "PROVISION_ANIMAL_CARCASS_ROBIN_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_ROOSTER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ROOSTER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ROOSTER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ROOSTER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ROOSTER_POOR:
		return "PROVISION_ANIMAL_CARCASS_ROOSTER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_POOR:
		return "PROVISION_ANIMAL_CARCASS_ROSESPOONBIL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SEAGULL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SEAGULL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SEAGULL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SEAGULL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SEAGULL_POOR:
		return "PROVISION_ANIMAL_CARCASS_SEAGULL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SHEEP_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SHEEP_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SHEEP_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SHEEP_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SHEEP_POOR:
		return "PROVISION_ANIMAL_CARCASS_SHEEP_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SKUNK_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SKUNK_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SKUNK_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SKUNK_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SKUNK_POOR:
		return "PROVISION_ANIMAL_CARCASS_SKUNK_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BLRAT_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BOA_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BOA_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BOA_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BOA_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_BOA_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_BOA_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_COPPER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_FERDLNCE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_WATER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_WATER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_WATER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_WATER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SNAKE_WATER_POOR:
		return "PROVISION_ANIMAL_CARCASS_SNAKE_WATER_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SONGBIRD_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SONGBIRD_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SONGBIRD_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SONGBIRD_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SONGBIRD_POOR:
		return "PROVISION_ANIMAL_CARCASS_SONGBIRD_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SPARROW_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SPARROW_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SPARROW_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SPARROW_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SPARROW_POOR:
		return "PROVISION_ANIMAL_CARCASS_SPARROW_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_SQUIRREL_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_SQUIRREL_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_SQUIRREL_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_SQUIRREL_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_SQUIRREL_POOR:
		return "PROVISION_ANIMAL_CARCASS_SQUIRREL_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_TOAD_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_TOAD_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_TOAD_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_TOAD_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_TOAD_POOR:
		return "PROVISION_ANIMAL_CARCASS_TOAD_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_TURKEY_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_TURKEY_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_TURKEY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_TURKEY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_TURKEY_POOR:
		return "PROVISION_ANIMAL_CARCASS_TURKEY_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_POOR:
		return "PROVISION_ANIMAL_CARCASS_TURTLE_SNAP_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_VULTURE_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_VULTURE_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_VULTURE_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_VULTURE_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_VULTURE_POOR:
		return "PROVISION_ANIMAL_CARCASS_VULTURE_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_WOLF_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_WOLF_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_WOLF_LEGENDARY_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_WOLF_LEGENDARY_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_WOLF_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_WOLF_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_WOLF_POOR:
		return "PROVISION_ANIMAL_CARCASS_WOLF_POOR";
		break;
	case PROVISION_ANIMAL_CARCASS_WOODPECKER_HIGH_QUALITY:
		return "PROVISION_ANIMAL_CARCASS_WOODPECKER_HIGH_QUALITY";
		break;
	case PROVISION_ANIMAL_CARCASS_WOODPECKER_PERFECT:
		return "PROVISION_ANIMAL_CARCASS_WOODPECKER_PERFECT";
		break;
	case PROVISION_ANIMAL_CARCASS_WOODPECKER_POOR:
		return "PROVISION_ANIMAL_CARCASS_WOODPECKER_POOR";
		break;
	case PROVISION_ANIMAL_FAT:
		return "PROVISION_ANIMAL_FAT";
		break;
	case PROVISION_ANIMAL_SCENT_GLAND:
		return "PROVISION_ANIMAL_SCENT_GLAND";
		break;
	case PROVISION_ARMADILLO_SKIN:
		return "PROVISION_ARMADILLO_SKIN";
		break;
	case PROVISION_ARMADILLO_SKIN_POOR:
		return "PROVISION_ARMADILLO_SKIN_POOR";
		break;
	case PROVISION_ARMADILLO_SKIN_PRISTINE:
		return "PROVISION_ARMADILLO_SKIN_PRISTINE";
		break;
	case PROVISION_ASTEROID_CHUNK:
		return "PROVISION_ASTEROID_CHUNK";
		break;
	case PROVISION_BADGER_CLAW:
		return "PROVISION_BADGER_CLAW";
		break;
	case PROVISION_BADGER_PELT:
		return "PROVISION_BADGER_PELT";
		break;
	case PROVISION_BADGER_PELT_POOR:
		return "PROVISION_BADGER_PELT_POOR";
		break;
	case PROVISION_BADGER_PELT_PRISTINE:
		return "PROVISION_BADGER_PELT_PRISTINE";
		break;
	case PROVISION_BAT_WING:
		return "PROVISION_BAT_WING";
		break;
	case PROVISION_BEAR_CLAW:
		return "PROVISION_BEAR_CLAW";
		break;
	case PROVISION_BEAR_FUR:
		return "PROVISION_BEAR_FUR";
		break;
	case PROVISION_BEAR_FUR_POOR:
		return "PROVISION_BEAR_FUR_POOR";
		break;
	case PROVISION_BEAR_FUR_PRISTINE:
		return "PROVISION_BEAR_FUR_PRISTINE";
		break;
	case PROVISION_BEAR_LEGENDARY_CLAW:
		return "PROVISION_BEAR_LEGENDARY_CLAW";
		break;
	case PROVISION_BEAR_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_BEAR_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_BEAUS_GIFT_1:
		return "PROVISION_BEAUS_GIFT_1";
		break;
	case PROVISION_BEAUS_GIFT_2:
		return "PROVISION_BEAUS_GIFT_2";
		break;
	case PROVISION_BEAVER_FUR:
		return "PROVISION_BEAVER_FUR";
		break;
	case PROVISION_BEAVER_FUR_POOR:
		return "PROVISION_BEAVER_FUR_POOR";
		break;
	case PROVISION_BEAVER_FUR_PRISTINE:
		return "PROVISION_BEAVER_FUR_PRISTINE";
		break;
	case PROVISION_BEAVER_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_BEAVER_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_BIGHORN_HORN:
		return "PROVISION_BIGHORN_HORN";
		break;
	case PROVISION_BIG_GAME_MEAT:
		return "PROVISION_BIG_GAME_MEAT";
		break;
	case PROVISION_BIRD_FEATHER_FLIGHT:
		return "PROVISION_BIRD_FEATHER_FLIGHT";
		break;
	case PROVISION_BLACK_BEAR_CLAW:
		return "PROVISION_BLACK_BEAR_CLAW";
		break;
	case PROVISION_BLACK_BEAR_FUR:
		return "PROVISION_BLACK_BEAR_FUR";
		break;
	case PROVISION_BLACK_BEAR_FUR_POOR:
		return "PROVISION_BLACK_BEAR_FUR_POOR";
		break;
	case PROVISION_BLACK_BEAR_FUR_PRISTINE:
		return "PROVISION_BLACK_BEAR_FUR_PRISTINE";
		break;
	case PROVISION_BLUEGILL_DESC:
		return "PROVISION_BLUEGILL_DESC";
		break;
	case PROVISION_BLUEJAY_FEATHER:
		return "PROVISION_BLUEJAY_FEATHER";
		break;
	case PROVISION_BOAR_LEGENDARY_SKIN_PRISTINE:
		return "PROVISION_BOAR_LEGENDARY_SKIN_PRISTINE";
		break;
	case PROVISION_BOAR_RARE_TUSK:
		return "PROVISION_BOAR_RARE_TUSK";
		break;
	case PROVISION_BOAR_SKIN:
		return "PROVISION_BOAR_SKIN";
		break;
	case PROVISION_BOAR_SKIN_POOR:
		return "PROVISION_BOAR_SKIN_POOR";
		break;
	case PROVISION_BOAR_SKIN_PRISTINE:
		return "PROVISION_BOAR_SKIN_PRISTINE";
		break;
	case PROVISION_BOAR_TUSK:
		return "PROVISION_BOAR_TUSK";
		break;
	case PROVISION_BOOBY_FEATHER:
		return "PROVISION_BOOBY_FEATHER";
		break;
	case PROVISION_BRACELET_GOLD:
		return "PROVISION_BRACELET_GOLD";
		break;
	case PROVISION_BRACELET_PLATINUM:
		return "PROVISION_BRACELET_PLATINUM";
		break;
	case PROVISION_BRACELET_SILVER:
		return "PROVISION_BRACELET_SILVER";
		break;
	case PROVISION_BUCKLE_GOLD:
		return "PROVISION_BUCKLE_GOLD";
		break;
	case PROVISION_BUCKLE_PLATINUM:
		return "PROVISION_BUCKLE_PLATINUM";
		break;
	case PROVISION_BUCKLE_SILVER:
		return "PROVISION_BUCKLE_SILVER";
		break;
	case PROVISION_BUCK_ANTLERS:
		return "PROVISION_BUCK_ANTLERS";
		break;
	case PROVISION_BUCK_FUR:
		return "PROVISION_BUCK_FUR";
		break;
	case PROVISION_BUCK_FUR_POOR:
		return "PROVISION_BUCK_FUR_POOR";
		break;
	case PROVISION_BUCK_FUR_PRISTINE:
		return "PROVISION_BUCK_FUR_PRISTINE";
		break;
	case PROVISION_BUCK_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_BUCK_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_BUFFALO_FUR:
		return "PROVISION_BUFFALO_FUR";
		break;
	case PROVISION_BUFFALO_FUR_POOR:
		return "PROVISION_BUFFALO_FUR_POOR";
		break;
	case PROVISION_BUFFALO_FUR_PRISTINE:
		return "PROVISION_BUFFALO_FUR_PRISTINE";
		break;
	case PROVISION_BUFFALO_HORN:
		return "PROVISION_BUFFALO_HORN";
		break;
	case PROVISION_BULHDCATFSH_DESC:
		return "PROVISION_BULHDCATFSH_DESC";
		break;
	case PROVISION_BULL_HIDE:
		return "PROVISION_BULL_HIDE";
		break;
	case PROVISION_BULL_HIDE_POOR:
		return "PROVISION_BULL_HIDE_POOR";
		break;
	case PROVISION_BULL_HIDE_PRISTINE:
		return "PROVISION_BULL_HIDE_PRISTINE";
		break;
	case PROVISION_BULL_HORNS:
		return "PROVISION_BULL_HORNS";
		break;
	case PROVISION_CARDINAL_FEATHER:
		return "PROVISION_CARDINAL_FEATHER";
		break;
	case PROVISION_CATHERINES_NECKLACE:
		return "PROVISION_CATHERINES_NECKLACE";
		break;
	case PROVISION_CC_VINTAGE_HANDCUFFS:
		return "PROVISION_CC_VINTAGE_HANDCUFFS";
		break;
	case PROVISION_CEDARWAXWING_FEATHER:
		return "PROVISION_CEDARWAXWING_FEATHER";
		break;
	case PROVISION_CHICKEN_FEATHER:
		return "PROVISION_CHICKEN_FEATHER";
		break;
	case PROVISION_CHNCATFSH_DESC:
		return "PROVISION_CHNCATFSH_DESC";
		break;
	case PROVISION_CHNPKRL_DESC:
		return "PROVISION_CHNPKRL_DESC";
		break;
	case PROVISION_COMP_DUTCH_PIPE:
		return "PROVISION_COMP_DUTCH_PIPE";
		break;
	case PROVISION_CONDOR_FEATHER:
		return "PROVISION_CONDOR_FEATHER";
		break;
	case PROVISION_CORMORANT_FEATHER:
		return "PROVISION_CORMORANT_FEATHER";
		break;
	case PROVISION_COUGAR_FANG:
		return "PROVISION_COUGAR_FANG";
		break;
	case PROVISION_COUGAR_FUR:
		return "PROVISION_COUGAR_FUR";
		break;
	case PROVISION_COUGAR_FUR_POOR:
		return "PROVISION_COUGAR_FUR_POOR";
		break;
	case PROVISION_COUGAR_FUR_PRISTINE:
		return "PROVISION_COUGAR_FUR_PRISTINE";
		break;
	case PROVISION_COUGAR_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_COUGAR_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_COW_HIDE:
		return "PROVISION_COW_HIDE";
		break;
	case PROVISION_COW_HIDE_POOR:
		return "PROVISION_COW_HIDE_POOR";
		break;
	case PROVISION_COW_HIDE_PRISTINE:
		return "PROVISION_COW_HIDE_PRISTINE";
		break;
	case PROVISION_COYOTE_FUR:
		return "PROVISION_COYOTE_FUR";
		break;
	case PROVISION_COYOTE_FUR_POOR:
		return "PROVISION_COYOTE_FUR_POOR";
		break;
	case PROVISION_COYOTE_FUR_PRISTINE:
		return "PROVISION_COYOTE_FUR_PRISTINE";
		break;
	case PROVISION_COYOTE_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_COYOTE_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_CROW_FEATHER:
		return "PROVISION_CROW_FEATHER";
		break;
	case PROVISION_CRUSTACEAN_MEAT:
		return "PROVISION_CRUSTACEAN_MEAT";
		break;
	case PROVISION_DB_SKULL_STATUE:
		return "PROVISION_DB_SKULL_STATUE";
		break;
	case PROVISION_DEER_HIDE:
		return "PROVISION_DEER_HIDE";
		break;
	case PROVISION_DEER_HIDE_POOR:
		return "PROVISION_DEER_HIDE_POOR";
		break;
	case PROVISION_DEER_HIDE_PRISTINE:
		return "PROVISION_DEER_HIDE_PRISTINE";
		break;
	case PROVISION_DISCO_AMMOLITE:
		return "PROVISION_DISCO_AMMOLITE";
		break;
	case PROVISION_DISCO_ANCIENT_NECKLACE:
		return "PROVISION_DISCO_ANCIENT_NECKLACE";
		break;
	case PROVISION_DISCO_ARROWHEAD_02:
		return "PROVISION_DISCO_ARROWHEAD_02";
		break;
	case PROVISION_DISCO_FERTILITY_STATUE:
		return "PROVISION_DISCO_FERTILITY_STATUE";
		break;
	case PROVISION_DISCO_FERTILITY_STATUE_FEMALE:
		return "PROVISION_DISCO_FERTILITY_STATUE_FEMALE";
		break;
	case PROVISION_DISCO_FLUORITE:
		return "PROVISION_DISCO_FLUORITE";
		break;
	case PROVISION_DISCO_SHRUNKEN_HEAD:
		return "PROVISION_DISCO_SHRUNKEN_HEAD";
		break;
	case PROVISION_DISCO_VIKING_COMB:
		return "PROVISION_DISCO_VIKING_COMB";
		break;
	case PROVISION_DUCK_FEATHER:
		return "PROVISION_DUCK_FEATHER";
		break;
	case PROVISION_EAGLE_FEATHER:
		return "PROVISION_EAGLE_FEATHER";
		break;
	case PROVISION_EAGLE_TALON:
		return "PROVISION_EAGLE_TALON";
		break;
	case PROVISION_EARRING_GOLD:
		return "PROVISION_EARRING_GOLD";
		break;
	case PROVISION_EARRING_PLATINUM:
		return "PROVISION_EARRING_PLATINUM";
		break;
	case PROVISION_EARRING_SILVER:
		return "PROVISION_EARRING_SILVER";
		break;
	case PROVISION_EGRET_FEATHER:
		return "PROVISION_EGRET_FEATHER";
		break;
	case PROVISION_EGRET_PLUME_LITTLE:
		return "PROVISION_EGRET_PLUME_LITTLE";
		break;
	case PROVISION_EGRET_PLUME_REDDISH:
		return "PROVISION_EGRET_PLUME_REDDISH";
		break;
	case PROVISION_EGRET_PLUME_SNOWY:
		return "PROVISION_EGRET_PLUME_SNOWY";
		break;
	case PROVISION_ELK_ANTLERS:
		return "PROVISION_ELK_ANTLERS";
		break;
	case PROVISION_ELK_FUR:
		return "PROVISION_ELK_FUR";
		break;
	case PROVISION_ELK_FUR_POOR:
		return "PROVISION_ELK_FUR_POOR";
		break;
	case PROVISION_ELK_FUR_PRISTINE:
		return "PROVISION_ELK_FUR_PRISTINE";
		break;
	case PROVISION_ELK_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_ELK_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_EXOTIC_BIRD_MEAT:
		return "PROVISION_EXOTIC_BIRD_MEAT";
		break;
	case PROVISION_FISH_BLUEGILL:
		return "PROVISION_FISH_BLUEGILL";
		break;
	case PROVISION_FISH_BLUEGILL_LEGENDARY:
		return "PROVISION_FISH_BLUEGILL_LEGENDARY";
		break;
	case PROVISION_FISH_BLUEGILL_POOR:
		return "PROVISION_FISH_BLUEGILL_POOR";
		break;
	case PROVISION_FISH_BULLHEAD_CATFISH:
		return "PROVISION_FISH_BULLHEAD_CATFISH";
		break;
	case PROVISION_FISH_BULLHEAD_CATFISH_LEGENDARY:
		return "PROVISION_FISH_BULLHEAD_CATFISH_LEGENDARY";
		break;
	case PROVISION_FISH_BULLHEAD_CATFISH_POOR:
		return "PROVISION_FISH_BULLHEAD_CATFISH_POOR";
		break;
	case PROVISION_FISH_CHAIN_PICKEREL:
		return "PROVISION_FISH_CHAIN_PICKEREL";
		break;
	case PROVISION_FISH_CHAIN_PICKEREL_LEGENDARY:
		return "PROVISION_FISH_CHAIN_PICKEREL_LEGENDARY";
		break;
	case PROVISION_FISH_CHAIN_PICKEREL_POOR:
		return "PROVISION_FISH_CHAIN_PICKEREL_POOR";
		break;
	case PROVISION_FISH_CHANNEL_CATFISH:
		return "PROVISION_FISH_CHANNEL_CATFISH";
		break;
	case PROVISION_FISH_CHANNEL_CATFISH_LEGENDARY:
		return "PROVISION_FISH_CHANNEL_CATFISH_LEGENDARY";
		break;
	case PROVISION_FISH_CHANNEL_CATFISH_POOR:
		return "PROVISION_FISH_CHANNEL_CATFISH_POOR";
		break;
	case PROVISION_FISH_LAKE_STURGEON:
		return "PROVISION_FISH_LAKE_STURGEON";
		break;
	case PROVISION_FISH_LAKE_STURGEON_LEGENDARY:
		return "PROVISION_FISH_LAKE_STURGEON_LEGENDARY";
		break;
	case PROVISION_FISH_LAKE_STURGEON_POOR:
		return "PROVISION_FISH_LAKE_STURGEON_POOR";
		break;
	case PROVISION_FISH_LARGEMOUTH_BASS:
		return "PROVISION_FISH_LARGEMOUTH_BASS";
		break;
	case PROVISION_FISH_LARGEMOUTH_BASS_LEGENDARY:
		return "PROVISION_FISH_LARGEMOUTH_BASS_LEGENDARY";
		break;
	case PROVISION_FISH_LARGEMOUTH_BASS_POOR:
		return "PROVISION_FISH_LARGEMOUTH_BASS_POOR";
		break;
	case PROVISION_FISH_LONGNOSE_GAR:
		return "PROVISION_FISH_LONGNOSE_GAR";
		break;
	case PROVISION_FISH_LONGNOSE_GAR_LEGENDARY:
		return "PROVISION_FISH_LONGNOSE_GAR_LEGENDARY";
		break;
	case PROVISION_FISH_LONGNOSE_GAR_POOR:
		return "PROVISION_FISH_LONGNOSE_GAR_POOR";
		break;
	case PROVISION_FISH_MUSKIE:
		return "PROVISION_FISH_MUSKIE";
		break;
	case PROVISION_FISH_MUSKIE_LEGENDARY:
		return "PROVISION_FISH_MUSKIE_LEGENDARY";
		break;
	case PROVISION_FISH_MUSKIE_POOR:
		return "PROVISION_FISH_MUSKIE_POOR";
		break;
	case PROVISION_FISH_NORTHERN_PIKE:
		return "PROVISION_FISH_NORTHERN_PIKE";
		break;
	case PROVISION_FISH_NORTHERN_PIKE_LEGENDARY:
		return "PROVISION_FISH_NORTHERN_PIKE_LEGENDARY";
		break;
	case PROVISION_FISH_NORTHERN_PIKE_POOR:
		return "PROVISION_FISH_NORTHERN_PIKE_POOR";
		break;
	case PROVISION_FISH_PERCH:
		return "PROVISION_FISH_PERCH";
		break;
	case PROVISION_FISH_PERCH_LEGENDARY:
		return "PROVISION_FISH_PERCH_LEGENDARY";
		break;
	case PROVISION_FISH_PERCH_POOR:
		return "PROVISION_FISH_PERCH_POOR";
		break;
	case PROVISION_FISH_REDFIN_PICKEREL:
		return "PROVISION_FISH_REDFIN_PICKEREL";
		break;
	case PROVISION_FISH_REDFIN_PICKEREL_LEGENDARY:
		return "PROVISION_FISH_REDFIN_PICKEREL_LEGENDARY";
		break;
	case PROVISION_FISH_REDFIN_PICKEREL_POOR:
		return "PROVISION_FISH_REDFIN_PICKEREL_POOR";
		break;
	case PROVISION_FISH_ROCK_BASS:
		return "PROVISION_FISH_ROCK_BASS";
		break;
	case PROVISION_FISH_ROCK_BASS_LEGENDARY:
		return "PROVISION_FISH_ROCK_BASS_LEGENDARY";
		break;
	case PROVISION_FISH_ROCK_BASS_POOR:
		return "PROVISION_FISH_ROCK_BASS_POOR";
		break;
	case PROVISION_FISH_SMALLMOUTH_BASS:
		return "PROVISION_FISH_SMALLMOUTH_BASS";
		break;
	case PROVISION_FISH_SMALLMOUTH_BASS_LEGENDARY:
		return "PROVISION_FISH_SMALLMOUTH_BASS_LEGENDARY";
		break;
	case PROVISION_FISH_SMALLMOUTH_BASS_POOR:
		return "PROVISION_FISH_SMALLMOUTH_BASS_POOR";
		break;
	case PROVISION_FISH_SOCKEYE_SALMON:
		return "PROVISION_FISH_SOCKEYE_SALMON";
		break;
	case PROVISION_FISH_SOCKEYE_SALMON_LEGENDARY:
		return "PROVISION_FISH_SOCKEYE_SALMON_LEGENDARY";
		break;
	case PROVISION_FISH_SOCKEYE_SALMON_POOR:
		return "PROVISION_FISH_SOCKEYE_SALMON_POOR";
		break;
	case PROVISION_FISH_STEELHEAD_TROUT:
		return "PROVISION_FISH_STEELHEAD_TROUT";
		break;
	case PROVISION_FISH_STEELHEAD_TROUT_LEGENDARY:
		return "PROVISION_FISH_STEELHEAD_TROUT_LEGENDARY";
		break;
	case PROVISION_FISH_STEELHEAD_TROUT_POOR:
		return "PROVISION_FISH_STEELHEAD_TROUT_POOR";
		break;
	case PROVISION_FISH_STHDTROUT_DESC:
		return "PROVISION_FISH_STHDTROUT_DESC";
		break;
	case PROVISION_FLAKEY_FISH_MEAT:
		return "PROVISION_FLAKEY_FISH_MEAT";
		break;
	case PROVISION_FOX_FUR:
		return "PROVISION_FOX_FUR";
		break;
	case PROVISION_FOX_FUR_POOR:
		return "PROVISION_FOX_FUR_POOR";
		break;
	case PROVISION_FOX_FUR_PRISTINE:
		return "PROVISION_FOX_FUR_PRISTINE";
		break;
	case PROVISION_FOX_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_FOX_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_FROG_SKIN:
		return "PROVISION_FROG_SKIN";
		break;
	case PROVISION_FROG_SKIN_POOR:
		return "PROVISION_FROG_SKIN_POOR";
		break;
	case PROVISION_FROG_SKIN_PRISTINE:
		return "PROVISION_FROG_SKIN_PRISTINE";
		break;
	case PROVISION_GAMEY_BIRD_MEAT:
		return "PROVISION_GAMEY_BIRD_MEAT";
		break;
	case PROVISION_GAME_MEAT:
		return "PROVISION_GAME_MEAT";
		break;
	case PROVISION_GATOR_EGG:
		return "PROVISION_GATOR_EGG";
		break;
	case PROVISION_GILA_SKIN:
		return "PROVISION_GILA_SKIN";
		break;
	case PROVISION_GILA_SKIN_POOR:
		return "PROVISION_GILA_SKIN_POOR";
		break;
	case PROVISION_GILA_SKIN_PRISTINE:
		return "PROVISION_GILA_SKIN_PRISTINE";
		break;
	case PROVISION_GOAT_HAIR:
		return "PROVISION_GOAT_HAIR";
		break;
	case PROVISION_GOAT_HAIR_POOR:
		return "PROVISION_GOAT_HAIR_POOR";
		break;
	case PROVISION_GOAT_HAIR_PRISTINE:
		return "PROVISION_GOAT_HAIR_PRISTINE";
		break;
	case PROVISION_GOLDBAR_LARGE:
		return "PROVISION_GOLDBAR_LARGE";
		break;
	case PROVISION_GOLDBAR_SMALL:
		return "PROVISION_GOLDBAR_SMALL";
		break;
	case PROVISION_GOLDRING:
		return "PROVISION_GOLDRING";
		break;
	case PROVISION_GOLDTOOTH:
		return "PROVISION_GOLDTOOTH";
		break;
	case PROVISION_GOLD_NUGGET:
		return "PROVISION_GOLD_NUGGET";
		break;
	case PROVISION_GOOSE_FEATHER:
		return "PROVISION_GOOSE_FEATHER";
		break;
	case PROVISION_GRISTLY_MUTTON:
		return "PROVISION_GRISTLY_MUTTON";
		break;
	case PROVISION_GRITTY_FISH_MEAT:
		return "PROVISION_GRITTY_FISH_MEAT";
		break;
	case PROVISION_HAWK_FEATHER:
		return "PROVISION_HAWK_FEATHER";
		break;
	case PROVISION_HERON_FEATHER:
		return "PROVISION_HERON_FEATHER";
		break;
	case PROVISION_HERPTILE_MEAT:
		return "PROVISION_HERPTILE_MEAT";
		break;
	case PROVISION_IGUANA_SCALE:
		return "PROVISION_IGUANA_SCALE";
		break;
	case PROVISION_IGUANA_SKIN:
		return "PROVISION_IGUANA_SKIN";
		break;
	case PROVISION_IGUANA_SKIN_POOR:
		return "PROVISION_IGUANA_SKIN_POOR";
		break;
	case PROVISION_IGUANA_SKIN_PRISTINE:
		return "PROVISION_IGUANA_SKIN_PRISTINE";
		break;
	case PROVISION_ITEMS_USED:
		return "PROVISION_ITEMS_USED";
		break;
	case PROVISION_JAVELINA_SKIN:
		return "PROVISION_JAVELINA_SKIN";
		break;
	case PROVISION_JAVELINA_SKIN_POOR:
		return "PROVISION_JAVELINA_SKIN_POOR";
		break;
	case PROVISION_JAVELINA_SKIN_PRISTINE:
		return "PROVISION_JAVELINA_SKIN_PRISTINE";
		break;
	case PROVISION_JEWELRYBAG_LG:
		return "PROVISION_JEWELRYBAG_LG";
		break;
	case PROVISION_JEWELRYBAG_SM:
		return "PROVISION_JEWELRYBAG_SM";
		break;
	case PROVISION_JEWELRY_BOX_LRG_FNCY_01:
		return "PROVISION_JEWELRY_BOX_LRG_FNCY_01";
		break;
	case PROVISION_LEGBLUEGILL_DESC:
		return "PROVISION_LEGBLUEGILL_DESC";
		break;
	case PROVISION_LEGBULHDCATFSH_DESC:
		return "PROVISION_LEGBULHDCATFSH_DESC";
		break;
	case PROVISION_LEGCHNCATFSH_DESC:
		return "PROVISION_LEGCHNCATFSH_DESC";
		break;
	case PROVISION_LEGCHNPKRL_DESC:
		return "PROVISION_LEGCHNPKRL_DESC";
		break;
	case PROVISION_LEGLKSTURG_DESC:
		return "PROVISION_LEGLKSTURG_DESC";
		break;
	case PROVISION_LEGLNGNOSEGAR_DESC:
		return "PROVISION_LEGLNGNOSEGAR_DESC";
		break;
	case PROVISION_LEGLRGMTHBASS_DESC:
		return "PROVISION_LEGLRGMTHBASS_DESC";
		break;
	case PROVISION_LEGMUSKIE_DESC:
		return "PROVISION_LEGMUSKIE_DESC";
		break;
	case PROVISION_LEGNPIKE_DESC:
		return "PROVISION_LEGNPIKE_DESC";
		break;
	case PROVISION_LEGPERCH_DESC:
		return "PROVISION_LEGPERCH_DESC";
		break;
	case PROVISION_LEGRDFNPCKREL_DESC:
		return "PROVISION_LEGRDFNPCKREL_DESC";
		break;
	case PROVISION_LEGROCKBASS_DESC:
		return "PROVISION_LEGROCKBASS_DESC";
		break;
	case PROVISION_LEGSCKEYESAL_DESC:
		return "PROVISION_LEGSCKEYESAL_DESC";
		break;
	case PROVISION_LEGSMLMTHBASS_DESC:
		return "PROVISION_LEGSMLMTHBASS_DESC";
		break;
	case PROVISION_LEGSTHDTROUT_DESC:
		return "PROVISION_LEGSTHDTROUT_DESC";
		break;
	case PROVISION_LKSTURG_DESC:
		return "PROVISION_LKSTURG_DESC";
		break;
	case PROVISION_LNGNOSEGAR_DESC:
		return "PROVISION_LNGNOSEGAR_DESC";
		break;
	case PROVISION_LOANSHARK_SKINS:
		return "PROVISION_LOANSHARK_SKINS";
		break;
	case PROVISION_LOON_FEATHER:
		return "PROVISION_LOON_FEATHER";
		break;
	case PROVISION_LRGMTHBASS_DESC:
		return "PROVISION_LRGMTHBASS_DESC";
		break;
	case PROVISION_MARYS_FOUNTAIN_PEN:
		return "PROVISION_MARYS_FOUNTAIN_PEN";
		break;
	case PROVISION_MATURE_VENISON:
		return "PROVISION_MATURE_VENISON";
		break;
	case PROVISION_MOLLYS_POCKET_MIRROR:
		return "PROVISION_MOLLYS_POCKET_MIRROR";
		break;
	case PROVISION_MOOSE_ANTLER:
		return "PROVISION_MOOSE_ANTLER";
		break;
	case PROVISION_MOOSE_FUR:
		return "PROVISION_MOOSE_FUR";
		break;
	case PROVISION_MOOSE_FUR_POOR:
		return "PROVISION_MOOSE_FUR_POOR";
		break;
	case PROVISION_MOOSE_FUR_PRISTINE:
		return "PROVISION_MOOSE_FUR_PRISTINE";
		break;
	case PROVISION_MOOSE_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_MOOSE_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_MUSKIE_DESC:
		return "PROVISION_MUSKIE_DESC";
		break;
	case PROVISION_MUSKRAT_FUR:
		return "PROVISION_MUSKRAT_FUR";
		break;
	case PROVISION_MUSKRAT_FUR_POOR:
		return "PROVISION_MUSKRAT_FUR_POOR";
		break;
	case PROVISION_MUSKRAT_FUR_PRISTINE:
		return "PROVISION_MUSKRAT_FUR_PRISTINE";
		break;
	case PROVISION_NATIVE_AMERICAN_RING:
		return "PROVISION_NATIVE_AMERICAN_RING";
		break;
	case PROVISION_NECKLACE_GOLD:
		return "PROVISION_NECKLACE_GOLD";
		break;
	case PROVISION_NECKLACE_PLATINUM:
		return "PROVISION_NECKLACE_PLATINUM";
		break;
	case PROVISION_NECKLACE_SILVER:
		return "PROVISION_NECKLACE_SILVER";
		break;
	case PROVISION_NPIKE_DESC:
		return "PROVISION_NPIKE_DESC";
		break;
	case PROVISION_OPOSSUM_FUR:
		return "PROVISION_OPOSSUM_FUR";
		break;
	case PROVISION_OPOSSUM_FUR_POOR:
		return "PROVISION_OPOSSUM_FUR_POOR";
		break;
	case PROVISION_OPOSSUM_FUR_PRISTINE:
		return "PROVISION_OPOSSUM_FUR_PRISTINE";
		break;
	case PROVISION_ORIOLE_FEATHER:
		return "PROVISION_ORIOLE_FEATHER";
		break;
	case PROVISION_OWL_FEATHER:
		return "PROVISION_OWL_FEATHER";
		break;
	case PROVISION_OXEN_HIDE:
		return "PROVISION_OXEN_HIDE";
		break;
	case PROVISION_OXEN_HIDE_POOR:
		return "PROVISION_OXEN_HIDE_POOR";
		break;
	case PROVISION_OXEN_HIDE_PRISTINE:
		return "PROVISION_OXEN_HIDE_PRISTINE";
		break;
	case PROVISION_OXEN_HORN:
		return "PROVISION_OXEN_HORN";
		break;
	case PROVISION_PANTHER_FUR:
		return "PROVISION_PANTHER_FUR";
		break;
	case PROVISION_PANTHER_FUR_POOR:
		return "PROVISION_PANTHER_FUR_POOR";
		break;
	case PROVISION_PANTHER_FUR_PRISTINE:
		return "PROVISION_PANTHER_FUR_PRISTINE";
		break;
	case PROVISION_PANTHER_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_PANTHER_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_PARAKEET_FEATHER:
		return "PROVISION_PARAKEET_FEATHER";
		break;
	case PROVISION_PARROT_FEATHER:
		return "PROVISION_PARROT_FEATHER";
		break;
	case PROVISION_PEARSONS_NAVAL_COMPASS:
		return "PROVISION_PEARSONS_NAVAL_COMPASS";
		break;
	case PROVISION_PELICAN_FEATHER:
		return "PROVISION_PELICAN_FEATHER";
		break;
	case PROVISION_PEN:
		return "PROVISION_PEN";
		break;
	case PROVISION_PENELOPES_BRACELET:
		return "PROVISION_PENELOPES_BRACELET";
		break;
	case PROVISION_PERCH_DESC:
		return "PROVISION_PERCH_DESC";
		break;
	case PROVISION_PHEASANT_FEATHER:
		return "PROVISION_PHEASANT_FEATHER";
		break;
	case PROVISION_PIGEON_FEATHER:
		return "PROVISION_PIGEON_FEATHER";
		break;
	case PROVISION_PIG_SKIN:
		return "PROVISION_PIG_SKIN";
		break;
	case PROVISION_PIG_SKIN_POOR:
		return "PROVISION_PIG_SKIN_POOR";
		break;
	case PROVISION_PIG_SKIN_PRISTINE:
		return "PROVISION_PIG_SKIN_PRISTINE";
		break;
	case PROVISION_PLUMP_BIRD_MEAT:
		return "PROVISION_PLUMP_BIRD_MEAT";
		break;
	case PROVISION_POCKET_WATCH_GOLD:
		return "PROVISION_POCKET_WATCH_GOLD";
		break;
	case PROVISION_POCKET_WATCH_PLATINUM:
		return "PROVISION_POCKET_WATCH_PLATINUM";
		break;
	case PROVISION_POCKET_WATCH_REUTLINGE:
		return "PROVISION_POCKET_WATCH_REUTLINGE";
		break;
	case PROVISION_POCKET_WATCH_SILVER:
		return "PROVISION_POCKET_WATCH_SILVER";
		break;
	case PROVISION_PRIME_BEEF:
		return "PROVISION_PRIME_BEEF";
		break;
	case PROVISION_PRONGHORN_FUR:
		return "PROVISION_PRONGHORN_FUR";
		break;
	case PROVISION_PRONGHORN_FUR_POOR:
		return "PROVISION_PRONGHORN_FUR_POOR";
		break;
	case PROVISION_PRONGHORN_FUR_PRISTINE:
		return "PROVISION_PRONGHORN_FUR_PRISTINE";
		break;
	case PROVISION_PRONGHORN_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_PRONGHORN_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_QUAIL_FEATHER:
		return "PROVISION_QUAIL_FEATHER";
		break;
	case PROVISION_RABBIT_PELT:
		return "PROVISION_RABBIT_PELT";
		break;
	case PROVISION_RABBIT_PELT_POOR:
		return "PROVISION_RABBIT_PELT_POOR";
		break;
	case PROVISION_RABBIT_PELT_PRISTINE:
		return "PROVISION_RABBIT_PELT_PRISTINE";
		break;
	case PROVISION_RACCOON_FUR:
		return "PROVISION_RACCOON_FUR";
		break;
	case PROVISION_RACCOON_FUR_POOR:
		return "PROVISION_RACCOON_FUR_POOR";
		break;
	case PROVISION_RACCOON_FUR_PRISTINE:
		return "PROVISION_RACCOON_FUR_PRISTINE";
		break;
	case PROVISION_RAM_HIDE:
		return "PROVISION_RAM_HIDE";
		break;
	case PROVISION_RAM_HIDE_POOR:
		return "PROVISION_RAM_HIDE_POOR";
		break;
	case PROVISION_RAM_HIDE_PRISTINE:
		return "PROVISION_RAM_HIDE_PRISTINE";
		break;
	case PROVISION_RAM_LEGENDARY_HIDE_PRISTINE:
		return "PROVISION_RAM_LEGENDARY_HIDE_PRISTINE";
		break;
	case PROVISION_RAT_FUR:
		return "PROVISION_RAT_FUR";
		break;
	case PROVISION_RAT_FUR_POOR:
		return "PROVISION_RAT_FUR_POOR";
		break;
	case PROVISION_RAT_FUR_PRISTINE:
		return "PROVISION_RAT_FUR_PRISTINE";
		break;
	case PROVISION_RAVEN_FEATHER:
		return "PROVISION_RAVEN_FEATHER";
		break;
	case PROVISION_RCKITTY_EMERALD:
		return "PROVISION_RCKITTY_EMERALD";
		break;
	case PROVISION_RCM_POCKET_WATCH:
		return "PROVISION_RCM_POCKET_WATCH";
		break;
	case PROVISION_RC_QUARTZ_CHUNK:
		return "PROVISION_RC_QUARTZ_CHUNK";
		break;
	case PROVISION_RC_ROCK_STATUE:
		return "PROVISION_RC_ROCK_STATUE";
		break;
	case PROVISION_RC_SLVCATCHER_WATCH:
		return "PROVISION_RC_SLVCATCHER_WATCH";
		break;
	case PROVISION_RDFNPCKREL_DESC:
		return "PROVISION_RDFNPCKREL_DESC";
		break;
	case PROVISION_REINFORCED_BANDOLIER_BEAR_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_BEAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_BOAR_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_BOAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_BUFFALO_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_BUFFALO_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_COUGAR_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_COUGAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_DEER_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_DEER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_GATOR_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_GATOR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_PANTHER_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_PANTHER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_RABBIT_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_RABBIT_CHALLENGE";
		break;
	case PROVISION_REINFORCED_BANDOLIER_SNAKE_CHALLENGE:
		return "PROVISION_REINFORCED_BANDOLIER_SNAKE_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_BEAR_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_BEAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_BOAR_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_BOAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_BUFFALO_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_BUFFALO_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_COUGAR_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_COUGAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_DEER_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_DEER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_GATOR_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_GATOR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_PANTHER_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_PANTHER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_RABBIT_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_RABBIT_CHALLENGE";
		break;
	case PROVISION_REINFORCED_GUNBELT_SNAKE_CHALLENGE:
		return "PROVISION_REINFORCED_GUNBELT_SNAKE_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_BEAR_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_BEAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_BOAR_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_BOAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_BUFFALO_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_BUFFALO_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_COUGAR_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_COUGAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_DEER_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_DEER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_GATOR_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_GATOR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_PANTHER_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_PANTHER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_RABBIT_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_RABBIT_CHALLENGE";
		break;
	case PROVISION_REINFORCED_HOLSTER_SNAKE_CHALLENGE:
		return "PROVISION_REINFORCED_HOLSTER_SNAKE_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_BEAR_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_BEAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_BOAR_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_BOAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_BUFFALO_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_BUFFALO_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_COUGAR_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_COUGAR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_DEER_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_DEER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_GATOR_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_GATOR_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_PANTHER_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_PANTHER_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_RABBIT_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_RABBIT_CHALLENGE";
		break;
	case PROVISION_REINFORCED_OFFHAND_SNAKE_CHALLENGE:
		return "PROVISION_REINFORCED_OFFHAND_SNAKE_CHALLENGE";
		break;
	case PROVISION_RING_PLATINUM:
		return "PROVISION_RING_PLATINUM";
		break;
	case PROVISION_RING_SILVER:
		return "PROVISION_RING_SILVER";
		break;
	case PROVISION_ROBIN_FEATHER:
		return "PROVISION_ROBIN_FEATHER";
		break;
	case PROVISION_ROCKBASS_DESC:
		return "PROVISION_ROCKBASS_DESC";
		break;
	case PROVISION_ROOSTER_FEATHER:
		return "PROVISION_ROOSTER_FEATHER";
		break;
	case PROVISION_ROTTEN_MEAT:
		return "PROVISION_ROTTEN_MEAT";
		break;
	case PROVISION_RO_FLOWER_ACUNAS_STAR:
		return "PROVISION_RO_FLOWER_ACUNAS_STAR";
		break;
	case PROVISION_RO_FLOWER_CIGAR:
		return "PROVISION_RO_FLOWER_CIGAR";
		break;
	case PROVISION_RO_FLOWER_CLAMSHELL:
		return "PROVISION_RO_FLOWER_CLAMSHELL";
		break;
	case PROVISION_RO_FLOWER_DRAGONS:
		return "PROVISION_RO_FLOWER_DRAGONS";
		break;
	case PROVISION_RO_FLOWER_GHOST:
		return "PROVISION_RO_FLOWER_GHOST";
		break;
	case PROVISION_RO_FLOWER_LADY_OF_NIGHT:
		return "PROVISION_RO_FLOWER_LADY_OF_NIGHT";
		break;
	case PROVISION_RO_FLOWER_LADY_SLIPPER:
		return "PROVISION_RO_FLOWER_LADY_SLIPPER";
		break;
	case PROVISION_RO_FLOWER_MOCCASIN:
		return "PROVISION_RO_FLOWER_MOCCASIN";
		break;
	case PROVISION_RO_FLOWER_NIGHT_SCENTED:
		return "PROVISION_RO_FLOWER_NIGHT_SCENTED";
		break;
	case PROVISION_RO_FLOWER_QUEENS:
		return "PROVISION_RO_FLOWER_QUEENS";
		break;
	case PROVISION_RO_FLOWER_RAT_TAIL:
		return "PROVISION_RO_FLOWER_RAT_TAIL";
		break;
	case PROVISION_RO_FLOWER_SPARROWS:
		return "PROVISION_RO_FLOWER_SPARROWS";
		break;
	case PROVISION_RO_FLOWER_SPIDER:
		return "PROVISION_RO_FLOWER_SPIDER";
		break;
	case PROVISION_SADIES_HARMONICA:
		return "PROVISION_SADIES_HARMONICA";
		break;
	case PROVISION_SCKEYESAL_DESC:
		return "PROVISION_SCKEYESAL_DESC";
		break;
	case PROVISION_SCRAP_METAL:
		return "PROVISION_SCRAP_METAL";
		break;
	case PROVISION_SEAGULL_FEATHER:
		return "PROVISION_SEAGULL_FEATHER";
		break;
	case PROVISION_SHEEP_WOOL:
		return "PROVISION_SHEEP_WOOL";
		break;
	case PROVISION_SHEEP_WOOL_POOR:
		return "PROVISION_SHEEP_WOOL_POOR";
		break;
	case PROVISION_SHEEP_WOOL_PRISTINE:
		return "PROVISION_SHEEP_WOOL_PRISTINE";
		break;
	case PROVISION_SKUNK_FUR:
		return "PROVISION_SKUNK_FUR";
		break;
	case PROVISION_SKUNK_FUR_POOR:
		return "PROVISION_SKUNK_FUR_POOR";
		break;
	case PROVISION_SKUNK_FUR_PRISTINE:
		return "PROVISION_SKUNK_FUR_PRISTINE";
		break;
	case PROVISION_SMLMTHBASS_DESC:
		return "PROVISION_SMLMTHBASS_DESC";
		break;
	case PROVISION_SNAKE_SKIN:
		return "PROVISION_SNAKE_SKIN";
		break;
	case PROVISION_SNAKE_SKIN_POOR:
		return "PROVISION_SNAKE_SKIN_POOR";
		break;
	case PROVISION_SNAKE_SKIN_PRISTINE:
		return "PROVISION_SNAKE_SKIN_PRISTINE";
		break;
	case PROVISION_SONGBIRD_FEATHER:
		return "PROVISION_SONGBIRD_FEATHER";
		break;
	case PROVISION_SPARROW_FEATHER:
		return "PROVISION_SPARROW_FEATHER";
		break;
	case PROVISION_SPOONBILL_FEATHER:
		return "PROVISION_SPOONBILL_FEATHER";
		break;
	case PROVISION_SQUIRREL_PELT:
		return "PROVISION_SQUIRREL_PELT";
		break;
	case PROVISION_SQUIRREL_PELT_POOR:
		return "PROVISION_SQUIRREL_PELT_POOR";
		break;
	case PROVISION_SQUIRREL_PELT_PRISTINE:
		return "PROVISION_SQUIRREL_PELT_PRISTINE";
		break;
	case PROVISION_SQUIRREL_TAIL:
		return "PROVISION_SQUIRREL_TAIL";
		break;
	case PROVISION_STRINGY_MEAT:
		return "PROVISION_STRINGY_MEAT";
		break;
	case PROVISION_SUCCULENT_FISH_MEAT:
		return "PROVISION_SUCCULENT_FISH_MEAT";
		break;
	case PROVISION_TAKANTA_BUFFALO_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_TAKANTA_BUFFALO_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_TALISMAN_ALLIGATOR_TOOTH:
		return "PROVISION_TALISMAN_ALLIGATOR_TOOTH";
		break;
	case PROVISION_TALISMAN_ALLIGATOR_TOOTH_DESC:
		return "PROVISION_TALISMAN_ALLIGATOR_TOOTH_DESC";
		break;
	case PROVISION_TALISMAN_BEAR_CLAW:
		return "PROVISION_TALISMAN_BEAR_CLAW";
		break;
	case PROVISION_TALISMAN_BEAR_CLAW_DESC:
		return "PROVISION_TALISMAN_BEAR_CLAW_DESC";
		break;
	case PROVISION_TALISMAN_BOAR_TUSK:
		return "PROVISION_TALISMAN_BOAR_TUSK";
		break;
	case PROVISION_TALISMAN_BUFFALO_HORN:
		return "PROVISION_TALISMAN_BUFFALO_HORN";
		break;
	case PROVISION_TALISMAN_BUFFALO_HORN_DESC:
		return "PROVISION_TALISMAN_BUFFALO_HORN_DESC";
		break;
	case PROVISION_TALISMAN_EAGLE_TALON:
		return "PROVISION_TALISMAN_EAGLE_TALON";
		break;
	case PROVISION_TALISMAN_EAGLE_TALON_DESC:
		return "PROVISION_TALISMAN_EAGLE_TALON_DESC";
		break;
	case PROVISION_TALISMAN_RAVEN_CLAW:
		return "PROVISION_TALISMAN_RAVEN_CLAW";
		break;
	case PROVISION_TALISMAN_RAVEN_CLAW_DESC:
		return "PROVISION_TALISMAN_RAVEN_CLAW_DESC";
		break;
	case PROVISION_TENDER_PORK:
		return "PROVISION_TENDER_PORK";
		break;
	case PROVISION_TH_ANTIQUE_BRASS_COMPASS:
		return "PROVISION_TH_ANTIQUE_BRASS_COMPASS";
		break;
	case PROVISION_TRINKET_BEAVER_TOOTH:
		return "PROVISION_TRINKET_BEAVER_TOOTH";
		break;
	case PROVISION_TRINKET_BEAVER_TOOTH_DESC:
		return "PROVISION_TRINKET_BEAVER_TOOTH_DESC";
		break;
	case PROVISION_TRINKET_BISON_HORN:
		return "PROVISION_TRINKET_BISON_HORN";
		break;
	case PROVISION_TRINKET_BISON_HORN_DESC:
		return "PROVISION_TRINKET_BISON_HORN_DESC";
		break;
	case PROVISION_TRINKET_BUCK_ANTLER:
		return "PROVISION_TRINKET_BUCK_ANTLER";
		break;
	case PROVISION_TRINKET_BUCK_ANTLER_DESC:
		return "PROVISION_TRINKET_BUCK_ANTLER_DESC";
		break;
	case PROVISION_TRINKET_COUGAR_FANG:
		return "PROVISION_TRINKET_COUGAR_FANG";
		break;
	case PROVISION_TRINKET_COUGAR_FANG_DESC:
		return "PROVISION_TRINKET_COUGAR_FANG_DESC";
		break;
	case PROVISION_TRINKET_COYOTE_FANG:
		return "PROVISION_TRINKET_COYOTE_FANG";
		break;
	case PROVISION_TRINKET_COYOTE_FANG_DESC:
		return "PROVISION_TRINKET_COYOTE_FANG_DESC";
		break;
	case PROVISION_TRINKET_ELK_ANTLER:
		return "PROVISION_TRINKET_ELK_ANTLER";
		break;
	case PROVISION_TRINKET_FOX_CLAW:
		return "PROVISION_TRINKET_FOX_CLAW";
		break;
	case PROVISION_TRINKET_FOX_CLAW_DESC:
		return "PROVISION_TRINKET_FOX_CLAW_DESC";
		break;
	case PROVISION_TRINKET_IGUANA_SCALE:
		return "PROVISION_TRINKET_IGUANA_SCALE";
		break;
	case PROVISION_TRINKET_IGUANA_SCALE_DESC:
		return "PROVISION_TRINKET_IGUANA_SCALE_DESC";
		break;
	case PROVISION_TRINKET_LION_PAW:
		return "PROVISION_TRINKET_LION_PAW";
		break;
	case PROVISION_TRINKET_LION_PAW_DESC:
		return "PROVISION_TRINKET_LION_PAW_DESC";
		break;
	case PROVISION_TRINKET_MOOSE_ANTLER:
		return "PROVISION_TRINKET_MOOSE_ANTLER";
		break;
	case PROVISION_TRINKET_MOOSE_ANTLER_DESC:
		return "PROVISION_TRINKET_MOOSE_ANTLER_DESC";
		break;
	case PROVISION_TRINKET_OWL_FEATHER:
		return "PROVISION_TRINKET_OWL_FEATHER";
		break;
	case PROVISION_TRINKET_OWL_FEATHER_DESC:
		return "PROVISION_TRINKET_OWL_FEATHER_DESC";
		break;
	case PROVISION_TRINKET_PANTHER_EYE:
		return "PROVISION_TRINKET_PANTHER_EYE";
		break;
	case PROVISION_TRINKET_PANTHER_EYE_DESC:
		return "PROVISION_TRINKET_PANTHER_EYE_DESC";
		break;
	case PROVISION_TRINKET_PRONGHORN_ANTLER:
		return "PROVISION_TRINKET_PRONGHORN_ANTLER";
		break;
	case PROVISION_TRINKET_PRONGHORN_ANTLER_DESC:
		return "PROVISION_TRINKET_PRONGHORN_ANTLER_DESC";
		break;
	case PROVISION_TRINKET_RAM_HORN:
		return "PROVISION_TRINKET_RAM_HORN";
		break;
	case PROVISION_TRINKET_RAM_HORN_DESC:
		return "PROVISION_TRINKET_RAM_HORN_DESC";
		break;
	case PROVISION_TRINKET_WOLF_HEART:
		return "PROVISION_TRINKET_WOLF_HEART";
		break;
	case PROVISION_TRINKET_WOLF_HEART_DESC:
		return "PROVISION_TRINKET_WOLF_HEART_DESC";
		break;
	case PROVISION_TURKEY_FEATHER:
		return "PROVISION_TURKEY_FEATHER";
		break;
	case PROVISION_VULTURE_FEATHER:
		return "PROVISION_VULTURE_FEATHER";
		break;
	case PROVISION_WATCH:
		return "PROVISION_WATCH";
		break;
	case PROVISION_WHITE_BUFFALO_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_WHITE_BUFFALO_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_WHOOPING_CRANE_FEATHER:
		return "PROVISION_WHOOPING_CRANE_FEATHER";
		break;
	case PROVISION_WOLF_FUR:
		return "PROVISION_WOLF_FUR";
		break;
	case PROVISION_WOLF_FUR_POOR:
		return "PROVISION_WOLF_FUR_POOR";
		break;
	case PROVISION_WOLF_FUR_PRISTINE:
		return "PROVISION_WOLF_FUR_PRISTINE";
		break;
	case PROVISION_WOLF_HEART:
		return "PROVISION_WOLF_HEART";
		break;
	case PROVISION_WOLF_LEGENDARY_FUR_PRISTINE:
		return "PROVISION_WOLF_LEGENDARY_FUR_PRISTINE";
		break;
	case PROVISION_WOODPECKER_FEATHER:
		return "PROVISION_WOODPECKER_FEATHER";
		break;
	default:
		return std::to_string(Item);
		break;
	}
}
inline constexpr const char* EventHashToString(Hash EventHash)
{
	switch (EventHash)
	{
	case -1:
		return "EVENT_INVALID";
	case 2314218394:
		return "EVENT_ACQUAINTANCE_PED_DISLIKE";
	case 435938875:
		return "EVENT_ACQUAINTANCE_PED_HATE";
	case 2516364232:
		return "EVENT_ACQUAINTANCE_PED_LIKE";
	case 3887971237:
		return "EVENT_ACQUAINTANCE_PED_RESPECT";
	case 510893831:
		return "EVENT_ACQUAINTANCE_PED_WANTED";
	case 1734436639:
		return "EVENT_ACQUAINTANCE_PED_DISGUISE";
	case 4098067509:
		return "EVENT_ACQUAINTANCE_PED_DEAD";
	case 2050604246:
		return "EVENT_ACQUAINTANCE_PED_THIEF";
	case 2221147004:
		return "EVENT_INTERACTION";
	case 1225420150:
		return "EVENT_INTERACTION_ACTION";
	case 3743820235:
		return "EVENT_REACTION_ANALYSIS_ACTION";
	case 1874382407:
		return "EVENT_AMBIENT_THREAT_LEVEL_MAXED";
	case 1210910842:
		return "EVENT_SHOCKING_PED_PLANTING_EXPLOSIVE";
	case 1073463643:
		return "EVENT_SHOCKING_PLANTED_EXPLOSIVE";
	case 4261207436:
		return "EVENT_ANIMAL_DETECTED_PREY";
	case 1545444922:
		return "EVENT_ANIMAL_DETECTED_PREDATOR";
	case 1428655956:
		return "EVENT_ANIMAL_DETECTED_THREAT";
	case 571505700:
		return "EVENT_ANIMAL_DETECTED_TRAIN";
	case 2372107364:
		return "EVENT_ANIMAL_RESPONDED_TO_THREAT";
	case 1379175797:
		return "EVENT_ANIMAL_TAMING_CALLOUT";
	case 2129777492:
		return "EVENT_ANIMAL_PROVOKED";
	case 2165787623:
		return "EVENT_CALM_HORSE";
	case 1312307149:
		return "EVENT_PLAYER_IN_CLOSE_PROXIMITY_TO_HORSE";
	case 4016019196:
		return "EVENT_CALL_FOR_BACKUP";
	case 2333485942:
		return "EVENT_CALL_FOR_COVER";
	case 2012933482:
		return "EVENT_CAR_UNDRIVEABLE";
	case 182250203:
		return "EVENT_CLIMB_LADDER_ON_ROUTE";
	case 1586716140:
		return "EVENT_CLIMB_NAVMESH_ON_ROUTE";
	case 413931470:
		return "EVENT_COMBAT_TAUNT";
	case 205245793:
		return "EVENT_COMMUNICATE_EVENT";
	case 1548353157:
		return "EVENT_COP_CAR_BEING_STOLEN";
	case 1924269094:
		return "EVENT_CRIME_CONFIRMED";
	case 3464903843:
		return "EVENT_CRIME_REPORTED";
	case 3316418807:
		return "EVENT_DAMAGE";
	case 1589923363:
		return "EVENT_DEAD_PED_FOUND";
	case 2934931347:
		return "EVENT_DEATH";
	case 3705929954:
		return "EVENT_DRAFT_ANIMAL_DETACHED_FROM_VEHICLE";
	case 443288134:
		return "EVENT_DRAGGED_OUT_CAR";
	case 3998521455:
		return "EVENT_DUMMY_CONVERSION";
	case 1113682948:
		return "EVENT_ENTITY_DISARMED";
	case 1794914733:
		return "EVENT_ENTITY_HOGTIED";
	case 1973084963:
		return "EVENT_EXPLOSION";
	case 4153757512:
		return "EVENT_EXPLOSION_HEARD";
	case 1384795140:
		return "EVENT_FIRE_NEARBY";
	case 2045969979:
		return "EVENT_FLUSH_TASKS";
	case 3858460595:
		return "EVENT_FOOT_STEP_HEARD";
	case 1570376850:
		return "EVENT_GET_OUT_OF_WATER";
	case 1949380209:
		return "EVENT_GIVE_PED_TASK";
	case 3855809865:
		return "EVENT_CLEAR_PED_TASKS";
	case 157877922:
		return "EVENT_GUN_AIMED_AT";
	case 1165534493:
		return "EVENT_HEADSHOT_BLOCKED_BY_HAT";
	case 4026492398:
		return "EVENT_HELP_AMBIENT_FRIEND";
	case 353377915:
		return "EVENT_HOGTIED_ENTITY_PICKED_UP";
	case 590795301:
		return "EVENT_INJURED_CRY_FOR_HELP";
	case 4027190659:
		return "EVENT_INJURED_RIDER";
	case 3830731575:
		return "EVENT_INJURED_DRIVER";
	case 343470035:
		return "EVENT_INJURED_OWNER";
	case 2796099506:
		return "EVENT_CRIME_CRY_FOR_HELP";
	case 871686291:
		return "EVENT_IN_AIR";
	case 1538190288:
		return "EVENT_IN_WATER";
	case 1937487669:
		return "EVENT_INCAPACITATED";
	case 2864273100:
		return "EVENT_KNOCKEDOUT";
	case 2723875039:
		return "EVENT_LASSO_DETACHED";
	case 3697581151:
		return "EVENT_LASSO_HIT";
	case 579261718:
		return "EVENT_BOLAS_HIT";
	case 4274177915:
		return "EVENT_LEADER_ENTERED_CAR_AS_DRIVER";
	case 2603205546:
		return "EVENT_LEADER_ENTERED_COVER";
	case 4225875267:
		return "EVENT_LEADER_EXITED_CAR_AS_DRIVER";
	case 1234058372:
		return "EVENT_LEADER_HOLSTERED_WEAPON";
	case 1189677571:
		return "EVENT_LEADER_LEFT_COVER";
	case 3820501548:
		return "EVENT_LEADER_UNHOLSTERED_WEAPON";
	case 1907495613:
		return "EVENT_MELEE_ACTION";
	case 1034611035:
		return "EVENT_MOUNTED_COLLISION";
	case 519123279:
		return "EVENT_MUST_LEAVE_BOAT";
	case 1016266288:
		return "EVENT_NEW_TASK";
	case 304848651:
		return "EVENT_NONE";
	case 3376514843:
		return "EVENT_OBJECT_COLLISION";
	case 151661781:
		return "EVENT_ON_FIRE";
	case 4054181205:
		return "EVENT_HITCHING_POST";
	case 2643381442:
		return "EVENT_HITCH_ANIMAL";
	case 2172523647:
		return "EVENT_CATCH_ITEM";
	case 1908962476:
		return "EVENT_OPEN_DOOR";
	case 769834622:
		return "EVENT_LOCKED_DOOR";
	case 936374126:
		return "EVENT_PEER_WINDOW";
	case 1006186021:
		return "EVENT_SHOVE_PED";
	case 3392169427:
		return "EVENT_VEHICLE_WAITING_ON_PED_TO_MOVE_AWAY";
	case 2391584737:
		return "EVENT_PED_COLLISION_WITH_PED";
	case 2873133018:
		return "EVENT_PED_COLLISION_WITH_PLAYER";
	case 3479862777:
		return "EVENT_PED_ENTERED_MY_VEHICLE";
	case 2402057053:
		return "EVENT_PED_JACKING_MY_VEHICLE";
	case 276199831:
		return "EVENT_PED_ON_VEHICLE_ROOF";
	case 1197193638:
		return "EVENT_PED_TO_CHASE";
	case 3006223815:
		return "EVENT_PED_TO_FLEE";
	case 2756498035:
		return "EVENT_PERSCHAR_PED_SPAWNED";
	case 3607700738:
		return "EVENT_PICKUP_CARRIABLE";
	case 1082572570:
		return "EVENT_PLACE_CARRIABLE_ONTO_PARENT";
	case 3053114403:
		return "EVENT_CARRIABLE_VEHICLE_STOW_START";
	case 867155253:
		return "EVENT_CARRIABLE_VEHICLE_STOW_COMPLETE";
	case 1811873798:
		return "EVENT_PLAYER_ANTAGONIZED_PED";
	case 2478244655:
		return "EVENT_PLAYER_ESCALATED_PED";
	case 313219550:
		return "EVENT_PLAYER_GREETED_PED";
	case 1387172233:
		return "EVENT_PLAYER_PROMPT_TRIGGERED";
	case 4116875920:
		return "EVENT_PLAYER_COLLECTED_AMBIENT_PICKUP";
	case 2982542425:
		return "EVENT_PLAYER_STRIPPED_WEAPON";
	case 318280644:
		return "EVENT_PLAYER_COLLISION_WITH_PED";
	case 3925796549:
		return "EVENT_PLAYER_HAT_EQUIPPED";
	case 3008136040:
		return "EVENT_PLAYER_HAT_KNOCKED_OFF";
	case 498393689:
		return "EVENT_PLAYER_HAT_REMOVED_AT_SHOP";
	case 2030961287:
		return "EVENT_PED_HAT_KNOCKED_OFF";
	case 4210375313:
		return "EVENT_PLAYER_LOCK_ON_TARGET";
	case 1176209503:
		return "EVENT_PLAYER_LOOK_FOCUS";
	case 2612580022:
		return "EVENT_PLAYER_MOUNT_WILD_HORSE";
	case 2282511196:
		return "EVENT_PLAYER_APPROACHED";
	case 1353184080:
		return "EVENT_PLAYER_ON_ROOFTOP";
	case 415022413:
		return "EVENT_PLAYER_SIM_UPDATE";
	case 832287042:
		return "EVENT_PLAYER_ROBBED_PED";
	case 3347664377:
		return "EVENT_POTENTIAL_BE_WALKED_INTO";
	case 650393230:
		return "EVENT_POTENTIAL_BLAST";
	case 3815660657:
		return "EVENT_POTENTIAL_GET_RUN_OVER";
	case 3323575926:
		return "EVENT_POTENTIAL_WALK_INTO_FIRE";
	case 1994340821:
		return "EVENT_POTENTIAL_WALK_INTO_OBJECT";
	case 2085861296:
		return "EVENT_POTENTIAL_WALK_INTO_VEHICLE";
	case 3999651271:
		return "EVENT_PROVIDING_COVER";
	case 721285987:
		return "EVENT_PULLED_FROM_MOUNT";
	case 1551265137:
		return "EVENT_RADIO_TARGET_POSITION";
	case 347157807:
		return "EVENT_RAN_OVER_PED";
	case 3270863451:
		return "EVENT_REACTION_COMBAT_VICTORY";
	case 3916669313:
		return "EVENT_REACTION_INVESTIGATE_DEAD_PED";
	case 671637744:
		return "EVENT_REACTION_INVESTIGATE_THREAT";
	case 3409095023:
		return "EVENT_RESPONDED_TO_THREAT";
	case 1074204023:
		return "EVENT_INCOMING_THREAT";
	case 2204691489:
		return "EVENT_REVIVED";
	case 256981913:
		return "EVENT_SCRIPT_COMMAND";
	case 1347843421:
		return "EVENT_SHOCKING_ANIMAL_CHARGING";
	case 3966140907:
		return "EVENT_SHOCKING_AUDIBLE_REACTION";
	case 1498498500:
		return "EVENT_SHOCKING_BEAT_ALARMING";
	case 3296483171:
		return "EVENT_SHOCKING_BEAT_ALARMING_NEW";
	case 4114844507:
		return "EVENT_SHOCKING_BEAT_AMUSING";
	case 697410695:
		return "EVENT_SHOCKING_BEAT_AMUSING_NEW";
	case 513747494:
		return "EVENT_SHOCKING_BEAT_DANGEROUS";
	case 3079676810:
		return "EVENT_SHOCKING_BEAT_DANGEROUS_NEW";
	case 1551210646:
		return "EVENT_SHOCKING_BEAT_DANGEROUS_GO_INSIDE";
	case 4106704054:
		return "EVENT_SHOCKING_BEAT_DANGEROUS_GO_INSIDE_NEW";
	case 156880836:
		return "EVENT_SHOCKING_DISTURBANCE";
	case 67779789:
		return "EVENT_SHOCKING_BEAT_DISTURBING";
	case 3067184348:
		return "EVENT_SHOCKING_BEAT_DISTURBING_NEW";
	case 161068336:
		return "EVENT_SHOCKING_BEAT_INSIGNIFICANT_NEW";
	case 4271617117:
		return "EVENT_SHOCKING_BEAT_INTERESTING";
	case 3164568967:
		return "EVENT_SHOCKING_BEAT_INTERESTING_NEW";
	case 2389732209:
		return "EVENT_SHOCKING_BEAT_SURPRISING";
	case 792302887:
		return "EVENT_SHOCKING_BEAT_SURPRISING_NEW";
	case 495656844:
		return "EVENT_SHOCKING_BEAT_TERRIFYING";
	case 2186499706:
		return "EVENT_SHOCKING_BEAT_TERRIFYING_NEW";
	case 3892467083:
		return "EVENT_SHOCKING_CAR_ALARM";
	case 846531670:
		return "EVENT_SHOCKING_PURSUIT";
	case 2035401594:
		return "EVENT_SHOCKING_CAR_CRASH";
	case 2955065230:
		return "EVENT_SHOCKING_BICYCLE_CRASH";
	case 2303040158:
		return "EVENT_SHOCKING_BREACH_ATTEMPT";
	case 2910203524:
		return "EVENT_SHOCKING_CAR_PILE_UP";
	case 4112473936:
		return "EVENT_SHOCKING_CAR_ON_CAR";
	case 1190727272:
		return "EVENT_SHOCKING_CRIME_SCENE";
	case 3762293212:
		return "EVENT_SHOCKING_DANGEROUS_ANIMAL";
	case 869302489:
		return "EVENT_SHOCKING_DEAD_BODY";
	case 3645031540:
		return "EVENT_SHOCKING_GUARD_DOG_BARKING";
	case 872115874:
		return "EVENT_SHOCKING_DOOR_BARGED_OPEN";
	case 3588251446:
		return "EVENT_SHOCKING_DOOR_BROKEN";
	case 1863491027:
		return "EVENT_SHOCKING_DRIVING_ON_PAVEMENT";
	case 3889570878:
		return "EVENT_SHOCKING_BICYCLE_ON_PAVEMENT";
	case 888085158:
		return "EVENT_SHOCKING_ENGINE_REVVED";
	case 921656702:
		return "EVENT_SHOCKING_ENTERED_INTERIOR";
	case 3101189355:
		return "EVENT_LASSO_WHIZZED_BY";
	case 2599852072:
		return "EVENT_SHOCKING_ENTITY_LASSOED";
	case 2693035047:
		return "EVENT_SHOCKING_ENTITY_HOGTIED";
	case 1925708505:
		return "EVENT_SHOCKING_EQUIPPED_MASK";
	case 217247011:
		return "EVENT_SHOCKING_EXPLOSION";
	case 956330317:
		return "EVENT_SHOCKING_FIRE";
	case 3475623917:
		return "EVENT_SHOCKING_FISHING_CAST";
	case 1233529491:
		return "EVENT_SHOCKING_FLEEING_PED";
	case 1271374678:
		return "EVENT_SHOCKING_FLEEING_VEHICLE";
	case 3707305529:
		return "EVENT_SHOT_FIRED";
	case 1671336020:
		return "EVENT_SHOCKING_HELICOPTER_OVERHEAD";
	case 501274433:
		return "EVENT_SHOCKING_PED_ENTERED_COVER";
	case 2099000865:
		return "EVENT_SHOCKING_PED_FALLING";
	case 4105322671:
		return "EVENT_SHOCKING_PED_JUMP";
	case 578971986:
		return "EVENT_SHOCKING_PLAYER_LAND";
	case 1444585293:
		return "EVENT_SHOCKING_PLAYER_CLIMB";
	case 1664579810:
		return "EVENT_SHOCKING_PED_KNOCKED_INTO_BY_PLAYER";
	case 2868493672:
		return "EVENT_SHOCKING_PED_KNOCKED_OUT";
	case 1760763491:
		return "EVENT_SHOCKING_PED_LEAPT_TO_TRANSPORT";
	case 151625665:
		return "EVENT_SHOCKING_HIJACKING";
	case 1442804547:
		return "EVENT_SHOCKING_HORN_SOUNDED";
	case 204402922:
		return "EVENT_SHOCKING_HORSE_REARED";
	case 3927008926:
		return "EVENT_SHOCKING_HORSE_FLEE_FROM_PLAYER_PROMPT";
	case 900141154:
		return "EVENT_SHOCKING_THROWN_FROM_HORSE";
	case 394521334:
		return "EVENT_SHOCKING_IN_DANGEROUS_VEHICLE";
	case 1590389061:
		return "EVENT_SHOCKING_INJURED_PED";
	case 1288064257:
		return "EVENT_SHOCKING_LAW_ARRESTING_CRIMINAL";
	case 2543923744:
		return "EVENT_SHOCKING_LOCK_PICKED";
	case 85441810:
		return "EVENT_SHOCKING_MAD_DRIVER";
	case 1503814123:
		return "EVENT_SHOCKING_MAD_DRIVER_EXTREME";
	case 85576031:
		return "EVENT_SHOCKING_MAD_DRIVER_BICYCLE";
	case 1943790124:
		return "EVENT_SHOCKING_MELEE_FIGHT";
	case 3176358168:
		return "EVENT_SHOCKING_ITEM_STOLEN";
	case 1201762715:
		return "EVENT_SHOCKING_MOUNT_STOLEN";
	case 1577882066:
		return "EVENT_SHOCKING_MUGGING";
	case 531874976:
		return "EVENT_SHOCKING_MULTIKILL";
	case 2958607324:
		return "EVENT_SHOCKING_NON_VIOLENT_WEAPON_AIMED_AT";
	case 2690001464:
		return "EVENT_SHOCKING_PAIN_VOCALIZATION";
	case 2984191997:
		return "EVENT_SHOCKING_PED_RUN_OVER";
	case 1591069782:
		return "EVENT_SHOCKING_PED_SCREAMED";
	case 506397713:
		return "EVENT_SHOCKING_PED_SHOT";
	case 3714444609:
		return "EVENT_SHOCKING_PED_TIED_BY_BOLAS";
	case 650790298:
		return "EVENT_SHOCKING_PLANE_FLY_BY";
	case 3703042945:
		return "EVENT_SHOCKING_PLAYER_ARRIVAL";
	case 3225901919:
		return "EVENT_SHOCKING_PLAYER_GRIEFING";
	case 3613894491:
		return "EVENT_SHOCKING_PLAYER_POURING_MOONSHINE";
	case 2084141474:
		return "EVENT_SHOCKING_PLAYER_RESTING";
	case 267902573:
		return "EVENT_SHOCKING_PLAYER_RUNNING";
	case 2398580622:
		return "EVENT_SHOCKING_PLAYER_CROUCHING";
	case 1128007058:
		return "EVENT_SHOCKING_PLAYER_CROUCHING_IN_FOLIAGE_WITH_WEAPON";
	case 3843984096:
		return "EVENT_SHOCKING_PLAYER_WITHIN_LOCKDOWN_REGION";
	case 3328036318:
		return "EVENT_SHOCKING_POTENTIAL_BLAST";
	case 2599117891:
		return "EVENT_SHOCKING_PROPERTY_DAMAGE";
	case 3068337182:
		return "EVENT_SHOCKING_RANSACK";
	case 2343965043:
		return "EVENT_SHOCKING_REACTION_TEST_LOW";
	case 2081523811:
		return "EVENT_SHOCKING_REACTION_TEST_MEDIUM";
	case 1364393977:
		return "EVENT_SHOCKING_REACTION_TEST_HIGH";
	case 2631644142:
		return "EVENT_SHOCKING_RUNNING_STAMPEDE";
	case 73464052:
		return "EVENT_SHOCKING_SEEN_CAR_STOLEN";
	case 2937513934:
		return "EVENT_SHOCKING_SEEN_CONFRONTATION";
	case 3413021340:
		return "EVENT_SHOCKING_SEEN_GANG_FIGHT";
	case 1931615605:
		return "EVENT_SHOCKING_SEEN_INSULT";
	case 4123400204:
		return "EVENT_SHOCKING_SEEN_MELEE_ACTION";
	case 2262056044:
		return "EVENT_SHOCKING_BAR_BRAWL_ACTION";
	case 748896394:
		return "EVENT_SHOCKING_INTERESTING_MELEE_FIGHT";
	case 2119319276:
		return "EVENT_SHOCKING_ABANDONED_VEHICLE";
	case 301509654:
		return "EVENT_SHOCKING_SEEN_NICE_CAR";
	case 2267583573:
		return "EVENT_SHOCKING_SEEN_PED_ROBBED";
	case 1722245163:
		return "EVENT_SHOCKING_SEEN_PED_KILLED";
	case 1750359646:
		return "EVENT_SHOCKING_SEEN_PED_LOOTED";
	case 3424472423:
		return "EVENT_SHOCKING_SEEN_PED_INTIMIDATED";
	case 1621729820:
		return "EVENT_SHOCKING_SEEN_VEHICLE_TOWED";
	case 2833992153:
		return "EVENT_SHOCKING_SEEN_WEAPON_THREAT";
	case 2837534541:
		return "EVENT_SHOCKING_SEEN_WEIRD_PED";
	case 2330409356:
		return "EVENT_SHOCKING_SEEN_WEIRD_PED_APPROACHING";
	case 2267396732:
		return "EVENT_SHOCKING_SUBMERGED_VEHICLE";
	case 2044016570:
		return "EVENT_SHOCKING_WINDOW_SMASHED";
	case 220865698:
		return "EVENT_SHOCKING_SHOP_ROBBED";
	case 3490755027:
		return "EVENT_SHOCKING_SIREN";
	case 1311036869:
		return "EVENT_SHOCKING_SPEECH";
	case 3163257534:
		return "EVENT_SHOCKING_STRANGE_OUTFIT";
	case 2283977241:
		return "EVENT_SHOCKING_STUDIO_BOMB";
	case 4020348010:
		return "EVENT_SHOCKING_TRAIN_WHISTLE";
	case 3562400426:
		return "EVENT_SHOCKING_TOXIC_MOONSHINE";
	case 3670641147:
		return "EVENT_SHOCKING_TOXIC_MOONSHINE_CLOUD";
	case 1770386025:
		return "EVENT_SHOCKING_TRESPASSING";
	case 149265999:
		return "EVENT_SHOCKING_UNCONSCIOUS_PED";
	case 3727617538:
		return "EVENT_SHOCKING_VISIBLE_REACTION";
	case 1992843916:
		return "EVENT_SHOCKING_VISIBLE_WEAPON";
	case 510388268:
		return "EVENT_SHOCKING_VISIBLE_LIT_EXPLOSIVE";
	case 2450480860:
		return "EVENT_SHOCKING_WEAPON_DRAWN";
	case 412087229:
		return "EVENT_SHOCKING_WITNESS_CALL_FOR_LAW";
	case 3639619628:
		return "EVENT_SHOCKING_WITNESS_REPORTING";
	case 2187104608:
		return "EVENT_SHOCKING_PED_DROPPED";
	case 2258339172:
		return "EVENT_SHOCKING_BAIT_DROPPED";
	case 2507051957:
		return "EVENT_SHOCKING_PLAYER_DUEL";
	case 3882883212:
		return "EVENT_CRIME_WITNESSED";
	case 3283655451:
		return "EVENT_POTENTIAL_CRIME";
	case 3426569632:
		return "EVENT_POTENTIAL_THREAT_APPROACHING";
	case 918533970:
		return "EVENT_ARMED_PED_APPROACHING";
	case 882658719:
		return "EVENT_RIDER_DISMOUNTED";
	case 3296293821:
		return "EVENT_SHOT_FIRED_BASE";
	case 2787876538:
		return "EVENT_SHOT_FIRED_BULLET_IMPACT";
	case 3192877889:
		return "EVENT_SHOT_FIRED_WHIZZED_BY";
	case 4072518867:
		return "EVENT_FRIENDLY_AIMED_AT";
	case 1198436399:
		return "EVENT_SHOUT_BLOCKING_LOS";
	case 3838553161:
		return "EVENT_SHOUT_TARGET_POSITION";
	case 3543100534:
		return "EVENT_STATIC_COUNT_REACHED_MAX";
	case 3634723742:
		return "EVENT_STUCK_IN_AIR";
	case 379908161:
		return "EVENT_SUSPICIOUS_ACTIVITY";
	case 2242258303:
		return "EVENT_SWITCH_2_NM_TASK";
	case 1753103034:
		return "EVENT_UNIDENTIFIED_PED";
	case 1065635568:
		return "EVENT_VEHICLE_COLLISION";
	case 4209553450:
		return "EVENT_VEHICLE_DAMAGE_WEAPON";
	case 2472471454:
		return "EVENT_VEHICLE_ON_FIRE";
	case 3240895598:
		return "EVENT_WHISTLING_HEARD";
	case 770328215:
		return "EVENT_DISTURBANCE";
	case 402722103:
		return "EVENT_ENTITY_DAMAGED";
	case 3981701542:
		return "EVENT_ENTITY_BROKEN";
	case 2145012826:
		return "EVENT_ENTITY_DESTROYED";
	case 735942751:
		return "EVENT_PED_CREATED";
	case 1626561060:
		return "EVENT_PED_DESTROYED";
	case 2431945707:
		return "EVENT_VEHICLE_CREATED";
	case 3063620295:
		return "EVENT_VEHICLE_DESTROYED";
	case 2164747579:
		return "EVENT_WITHIN_GUN_COMBAT_AREA";
	case 3334225333:
		return "EVENT_WITHIN_LAW_RESPONSE_AREA";
	case 282773725:
		return "EVENT_WON_APPROACH_ELECTION";
	case 3138439328:
		return "EVENT_PLAYER_UNABLE_TO_ENTER_VEHICLE";
	case 2034746601:
		return "EVENT_SCENARIO_FORCE_ACTION";
	case 3476761921:
		return "EVENT_STAT_VALUE_CHANGED";
	case 178452260:
		return "EVENT_PED_SEEN_DEAD_PED";
	case 3313338020:
		return "EVENT_PLAYER_DEATH";
	case 2403068798:
		return "EVENT_SHOT_FIRED_WHIZZED_BY_ENTITY";
	case 3972935010:
		return "EVENT_PED_RAN_OVER_SCRIPT";
	case 4154416011:
		return "EVENT_ENTITY_EXPLOSION";
	case 295876924:
		return "EVENT_CUT_FREE";
	case 4234705153:
		return "EVENT_TRANSITION_TO_HOGTIED";
	case 1266167444:
		return "EVENT_HOGTIED";
	case 526946626:
		return "EVENT_GET_UP";
	case 2783242999:
		return "EVENT_LOOT";
	case 1376140891:
		return "EVENT_LOOT_COMPLETE";
	case 3549877221:
		return "EVENT_IMPENDING_SAMPLE_PROMPT";
	case 3260846708:
		return "EVENT_HELP_TEXT_REQUEST";
	case 2785559390:
		return "EVENT_LOOT_VALIDATION_FAIL";
	case 1640116056:
		return "EVENT_LOOT_PLANT_START";
	case 3326694975:
		return "EVENT_MOUNT_REACTION";
	case 3832735580:
		return "EVENT_SADDLE_TRANSFER";
	case 1208357138:
		return "EVENT_CARRIABLE_UPDATE_CARRY_STATE";
	case 1081092949:
		return "EVENT_INVENTORY_ITEM_PICKED_UP";
	case 1505348054:
		return "EVENT_INVENTORY_ITEM_REMOVED";
	case 1417095237:
		return "EVENT_BUCKED_OFF";
	case 1638298852:
		return "EVENT_MOUNT_OVERSPURRED";
	case 71122427:
		return "EVENT_START_CONVERSATION";
	case 1652530845:
		return "EVENT_STOP_CONVERSATION";
	case 3725666035:
		return "EVENT_MISS_INTENDED_TARGET";
	case 3048848052:
		return "EVENT_PED_ANIMAL_INTERACTION";
	case 2309687491:
		return "EVENT_CALM_PED";
	case 1327216456:
		return "EVENT_PED_WHISTLE";
	case 1473676746:
		return "EVENT_PLAYER_DEBUG_TELEPORTED";
	case 3791764536:
		return "EVENT_HORSE_STARTED_BREAKING";
	case 218595333:
		return "EVENT_HORSE_BROKEN";
	case 3399414835:
		return "EVENT_PICKUP_SPAWNED";
	case 2725760494:
		return "EVENT_BEING_LOOTED";
	case 2358004211:
		return "EVENT_DEBUG_SETUP_CUTSCENE_WORLD_STATE";
	case 2921239211:
		return "EVENT_WAIT_FOR_INTERACTION";
	case 3677514192:
		return "EVENT_CHALLENGE_REWARD";
	case 2203022922:
		return "EVENT_CALCULATE_LOOT";
	case 2564195088:
		return "EVENT_OBJECT_INTERACTION";
	case 1352063587:
		return "EVENT_CONTAINER_INTERACTION";
	case 2099179610:
		return "EVENT_ITEM_PROMPT_INFO_REQUEST";
	case 3712605669:
		return "EVENT_CARRIABLE_PROMPT_INFO_REQUEST";
	case 1553659161:
		return "EVENT_REVIVE_ENTITY";
	case 1784289253:
		return "EVENT_TRIGGERED_ANIMAL_WRITHE";
	case 1655597605:
		return "EVENT_PLAYER_HORSE_AGITATED_BY_ANIMAL";
	case 1272433714:
		return "EVENT_OWNED_ENTITY_INTERACTION";
	case 1351025667:
		return "EVENT_CHALLENGE_GOAL_COMPLETE";
	case 1669410864:
		return "EVENT_CHALLENGE_GOAL_UPDATE";
	case 3164210461:
		return "EVENT_DAILY_CHALLENGE_STREAK_COMPLETED";
	case 2812820736:
		return "EVENT_NETWORK_PLAYER_JOIN_SESSION";
	case 1697477512:
		return "EVENT_NETWORK_PLAYER_LEFT_SESSION";
	case 2293864779:
		return "EVENT_NETWORK_PLAYER_JOIN_SCRIPT";
	case 3857469464:
		return "EVENT_NETWORK_PLAYER_LEFT_SCRIPT";
	case 3437210871:
		return "EVENT_NETWORK_SESSION_MERGE_START";
	case 2175165818:
		return "EVENT_NETWORK_SESSION_MERGE_END";
	case 1434205464:
		return "EVENT_NETWORK_PLAYER_SPAWN";
	case 3840822853:
		return "EVENT_NETWORK_PLAYER_COLLECTED_PICKUP";
	case 1274067014:
		return "EVENT_NETWORK_PLAYER_COLLECTED_PORTABLE_PICKUP";
	case 3451042364:
		return "EVENT_NETWORK_PLAYER_DROPPED_PORTABLE_PICKUP";
	case 1121131740:
		return "EVENT_NETWORK_EXTENDED_INVITE";
	case 3787126902:
		return "EVENT_NETWORK_SCRIPT_EVENT";
	case 1793200955:
		return "EVENT_NETWORK_PED_DISARMED";
	case 1342634267:
		return "EVENT_NETWORK_PED_HAT_SHOT_OFF";
	case 1626145032:
		return "EVENT_NETWORK_PLAYER_MISSED_SHOT";
	case 1355399116:
		return "EVENT_NETWORK_PLAYER_SIGNED_OFFLINE";
	case 2921665944:
		return "EVENT_NETWORK_PLAYER_SIGNED_ONLINE";
	case 163683216:
		return "EVENT_NETWORK_SIGN_IN_STATE_CHANGED";
	case 239947537:
		return "EVENT_NETWORK_SIGN_IN_START_NEW_GAME";
	case 2606436897:
		return "EVENT_NETWORK_NETWORK_ROS_CHANGED";
	case 1976253964:
		return "EVENT_NETWORK_NETWORK_BAIL";
	case 3768299828:
		return "EVENT_NETWORK_BAIL_DECISION_PENDING";
	case 701022886:
		return "EVENT_NETWORK_BAIL_DECISION_MADE";
	case 3827233718:
		return "EVENT_NETWORK_HOST_MIGRATION";
	case 995882143:
		return "EVENT_NETWORK_IS_VOLUME_EMPTY_RESULT";
	case 557673123:
		return "EVENT_NETWORK_CHEAT_TRIGGERED";
	case 2979396540:
		return "EVENT_NETWORK_DAMAGE_ENTITY";
	case 676208328:
		return "EVENT_NETWORK_INCAPACITATED_ENTITY";
	case 4183952112:
		return "EVENT_NETWORK_KNOCKEDOUT_ENTITY";
	case 3123256501:
		return "EVENT_NETWORK_REVIVED_ENTITY";
	case 2143094135:
		return "EVENT_NETWORK_PLAYER_ARREST";
	case 1660856426:
		return "EVENT_NETWORK_TIMED_EXPLOSION";
	case 1373658008:
		return "EVENT_NETWORK_PRIMARY_CREW_CHANGED";
	case 2979514117:
		return "EVENT_NETWORK_CREW_JOINED";
	case 1194448728:
		return "EVENT_NETWORK_CREW_LEFT";
	case 1028782110:
		return "EVENT_NETWORK_CREW_INVITE_RECEIVED";
	case 1234888675:
		return "EVENT_NETWORK_CREW_CREATION";
	case 2114586158:
		return "EVENT_NETWORK_CREW_DISBANDED";
	case 1068922597:
		return "EVENT_VOICE_SESSION_STARTED";
	case 4063576971:
		return "EVENT_VOICE_SESSION_ENDED";
	case 295704064:
		return "EVENT_VOICE_CONNECTION_REQUESTED";
	case 980298223:
		return "EVENT_VOICE_CONNECTION_RESPONSE";
	case 2389900255:
		return "EVENT_VOICE_CONNECTION_TERMINATED";
	case 750867124:
		return "EVENT_TEXT_MESSAGE_RECEIVED";
	case 904763044:
		return "EVENT_CLOUD_FILE_RESPONSE";
	case 1385704366:
		return "EVENT_NETWORK_PICKUP_RESPAWNED";
	case 1415355908:
		return "EVENT_NETWORK_PRESENCE_STAT_UPDATE";
	case 1814485447:
		return "EVENT_NETWORK_PED_LEFT_BEHIND";
	case 2108920557:
		return "EVENT_NETWORK_INBOX_MSGS_RCVD";
	case 2809338689:
		return "EVENT_NETWORK_ATTEMPT_HOST_MIGRATION";
	case 545528824:
		return "EVENT_NETWORK_INCREMENT_STAT";
	case 1658389497:
		return "EVENT_NETWORK_SESSION_EVENT";
	case 753021595:
		return "EVENT_NETWORK_CREW_KICKED";
	case 3569695057:
		return "EVENT_NETWORK_ROCKSTAR_INVITE_RECEIVED";
	case 543140406:
		return "EVENT_NETWORK_ROCKSTAR_INVITE_REMOVED";
	case 2198990111:
		return "EVENT_NETWORK_PLATFORM_INVITE_ACCEPTED";
	case 904577075:
		return "EVENT_NETWORK_INVITE_RESULT";
	case 809652668:
		return "EVENT_NETWORK_INVITE_RESPONSE";
	case 516249386:
		return "EVENT_NETWORK_JOIN_REQUEST_TIMED_OUT";
	case 1860341470:
		return "EVENT_NETWORK_INVITE_UNAVAILABLE";
	case 1827342969:
		return "EVENT_NETWORK_CASH_TRANSACTION_LOG";
	case 2986598902:
		return "EVENT_NETWORK_CREW_RANK_CHANGE";
	case 1832265142:
		return "EVENT_NETWORK_VEHICLE_UNDRIVABLE";
	case 1890598297:
		return "EVENT_NETWORK_PRESENCE_TRIGGER";
	case 3292326396:
		return "EVENT_NETWORK_PRESENCE_EMAIL";
	case 3955709671:
		return "EVENT_NETWORK_EMAIL_RECEIVED";
	case 2969267014:
		return "EVENT_NETWORK_SPECTATE_LOCAL";
	case 3610083314:
		return "EVENT_NETWORK_CLOUD_EVENT";
	case 1731288223:
		return "EVENT_NETWORK_CASHINVENTORY_TRANSACTION";
	case 446963576:
		return "EVENT_NETWORK_CASHINVENTORY_DELETE_CHAR";
	case 2794710382:
		return "EVENT_NETWORK_PERMISSION_CHECK_RESULT";
	case 587071841:
		return "EVENT_NETWORK_APP_LAUNCHED";
	case 1027163239:
		return "EVENT_NETWORK_ONLINE_PERMISSIONS_UPDATED";
	case 2462027470:
		return "EVENT_NETWORK_SYSTEM_SERVICE_EVENT";
	case 1629782592:
		return "EVENT_NETWORK_REQUEST_DELAY";
	case 586277309:
		return "EVENT_NETWORK_SOCIAL_CLUB_ACCOUNT_LINKED";
	case 3414176060:
		return "EVENT_NETWORK_SCADMIN_PLAYER_UPDATED";
	case 3652658002:
		return "EVENT_NETWORK_SCADMIN_RECEIVED_CASH";
	case 2194753722:
		return "EVENT_NETWORK_CREW_INVITE_REQUEST_RECEIVED";
	case 4197450690:
		return "EVENT_NETWORK_LASSO_ATTACH";
	case 2177299314:
		return "EVENT_NETWORK_LASSO_DETACH";
	case 3229233863:
		return "EVENT_NETWORK_HOGTIE_BEGIN";
	case 3375466525:
		return "EVENT_NETWORK_HOGTIE_END";
	case 2823345285:
		return "EVENT_NETWORK_DRAG_PED";
	case 1727082765:
		return "EVENT_NETWORK_DROP_PED";
	case 3646221521:
		return "EVENT_NETWORK_GANG";
	case 678947301:
		return "EVENT_NETWORK_GANG_WAYPOINT_CHANGED";
	case 2013393302:
		return "EVENT_NETWORK_BULLET_IMPACTED_MULTIPLE_PEDS";
	case 3409919219:
		return "EVENT_NETWORK_VEHICLE_LOOTED";
	case 1741908893:
		return "EVENT_NETWORK_AWARD_CLAIMED";
	case 141007368:
		return "EVENT_NETWORK_LOOT_CLAIMED";
	case 3168749364:
		return "EVENT_NETWORK_MINIGAME_REQUEST_COMPLETE";
	case 1694142010:
		return "EVENT_NETWORK_BOUNTY_REQUEST_COMPLETE";
	case 212329117:
		return "EVENT_NETWORK_FRIENDS_LIST_UPDATED";
	case 3578561221:
		return "EVENT_NETWORK_FRIEND_STATUS_UPDATED";
	case 3660904792:
		return "EVENT_NETWORK_SC_FEED_POST_NOTIFICATION";
	case 453501714:
		return "EVENT_NETWORK_HUB_UPDATE";
	case 1559647390:
		return "EVENT_NETWORK_PICKUP_COLLECTION_FAILED";
	case 1725992066:
		return "EVENT_NETWORK_DEBUG_TOGGLE_MP";
	case 2258845462:
		return "EVENT_NETWORK_PROJECTILE_ATTACHED";
	case 2058130545:
		return "EVENT_NETWORK_PROJECTILE_NO_DAMAGE_IMPACT";
	case 4063032011:
		return "EVENT_NETWORK_POSSE_CREATED";
	case 1268264445:
		return "EVENT_NETWORK_POSSE_JOINED";
	case 3986895308:
		return "EVENT_NETWORK_POSSE_LEFT";
	case 3873613459:
		return "EVENT_NETWORK_POSSE_DISBANDED";
	case 2058084749:
		return "EVENT_NETWORK_POSSE_KICKED";
	case 2545726460:
		return "EVENT_NETWORK_POSSE_DATA_OR_MEMBERSHIP_CHANGED";
	case 415576404:
		return "EVENT_NETWORK_POSSE_DATA_CHANGED";
	case 1830788491:
		return "EVENT_NETWORK_POSSE_MEMBER_JOINED";
	case 1047667690:
		return "EVENT_NETWORK_POSSE_MEMBER_LEFT";
	case 2602139233:
		return "EVENT_NETWORK_POSSE_MEMBER_DISBANDED";
	case 176872144:
		return "EVENT_NETWORK_POSSE_MEMBER_KICKED";
	case 2716508067:
		return "EVENT_NETWORK_POSSE_MEMBER_SET_ACTIVE";
	case 23105215:
		return "EVENT_NETWORK_POSSE_LEADER_SET_ACTIVE";
	case 237247060:
		return "EVENT_NETWORK_POSSE_PRESENCE_REQUEST_COMPLETE";
	case 2781829107:
		return "EVENT_NETWORK_POSSE_STATS_READ_COMPLETE";
	case 797969925:
		return "EVENT_NETWORK_POSSE_EX_INACTIVE_DISBANDED";
	case 2274960805:
		return "EVENT_NETWORK_POSSE_EX_ADMIN_DISBANDED";
	case 3147900654:
		return "EVENT_NETWORK_TOO_MANY_SCRIPTED_GAME_EVENTS";
	case 1588672286:
		return "EVENT_NETWORK_NOMINATED_GO_TO_NEXT_CONTENT_RESPONSE";
	case 1699948728:
		return "EVENT_NETWORK_NOMINATED_GET_UPCOMING_CONTENT_RESPONSE";
	case 4249958308:
		return "EVENT_SCENARIO_ADD_PED";
	case 3838043512:
		return "EVENT_SCENARIO_REMOVE_PED";
	case 3798825516:
		return "EVENT_SCENARIO_RELEASE_BUTTON";
	case 3451411458:
		return "EVENT_SCENARIO_DESTROY_PROP";
	case 3027649786:
		return "EVENT_UI_QUICK_ITEM_USED";
	case 3948754772:
		return "EVENT_UI_ITEM_INSPECT_ACTIONED";
	case 3066409991:
		return "EVENT_UNIT_TEST_SCENARIO_EXIT";
	case 432140815:
		return "EVENT_HEARD_DEAD_PED_COLLISION";
	case 3644710618:
		return "EVENT_RECOVER_AFTER_KNOCKOUT";
	case 3364812205:
		return "EVENT_NETWORK_CASHINVENTORY_NOTIFICATION";
	case 823440502:
		return "EVENT_PRE_MELEE_KILL";
	case 1378654347:
		return "EVENT_SHOCKING_REACTION_TEST_ACTIVE_LOOK";
	case 3139366874:
		return "EVENT_SEEN_TERRIFIED_PED";
	case 2263836110:
		return "EVENT_MOUNT_DAMAGED_BY_PLAYER";
	case 1501535693:
		return "EVENT_NEARBY_AMBIENT_THREAT";
	case 1150469382:
		return "EVENT_SHOCKING_PLAYER_REVIVED";
	case 3373494960:
		return "EVENT_ERRORS_UNKNOWN_ERROR";
	case 3302264373:
		return "EVENT_ERRORS_ARRAY_OVERFLOW";
	case 397004310:
		return "EVENT_ERRORS_INSTRUCTION_LIMIT";
	case 2004694700:
		return "EVENT_ERRORS_STACK_OVERFLOW";
	case 3285192533:
		return "EVENT_ERRORS_GLOBAL_BLOCK_INACCESSIBLE";
	case 1028822748:
		return "EVENT_ERRORS_GLOBAL_BLOCK_NOT_RESIDENT";
	default:
		break;
	}
}
inline constexpr std::string ScriptHashToString(Hash ScriptHash) {
	switch (ScriptHash) {
	case 3078434705:
		return "aberdeenpigfarm";
		break;
	case 167147412:
		return "abigail21_outro";
		break;
	case 3744696865:
		return "abigail2_1";
		break;
	case 3932881725:
		return "abigail2_1_intro";
		break;
	case 125359090:
		return "act_bankrobbery01";
		break;
	case 1802005408:
		return "act_cajav_homerob1";
		break;
	case 3364848464:
		return "act_calen_fff1";
		break;
	case 2342956856:
		return "act_camp_dominoes_light";
		break;
	case 1029767084:
		return "act_camp_fff_light";
		break;
	case 1058456162:
		return "act_camp_poker_light";
		break;
	case 4046860197:
		return "act_caunc_rustling";
		break;
	case 1184222144:
		return "act_caunc_rustling_invite";
		break;
	case 947886869:
		return "act_fishing01";
		break;
	case 3231361953:
		return "act_fishing03";
		break;
	case 3655556658:
		return "act_fishing06";
		break;
	case 3434269455:
		return "act_gen_dominoes";
		break;
	case 3017446535:
		return "act_gen_fff";
		break;
	case 2694866195:
		return "act_gen_fishing";
		break;
	case 748362637:
		return "act_gen_poker";
		break;
	case 1613439565:
		return "act_hunting_2";
		break;
	case 4154564492:
		return "adlerranch";
		break;
	case 1535019117:
		return "aggregate";
		break;
	case 3341224172:
		return "aguasdulces";
		break;
	case 1292457185:
		return "ambient_fishing_scenario";
		break;
	case 1443095796:
		return "ambient_load";
		break;
	case 3687905297:
		return "ambient_vignette_manager_loader";
		break;
	case 764968087:
		return "ambush_bnd_cliff1";
		break;
	case 522449008:
		return "ambush_bnd_ridge_ambush";
		break;
	case 3736031177:
		return "ambush_bnd_sniper_attack";
		break;
	case 432727541:
		return "ambush_exc_bridge_trap";
		break;
	case 397295622:
		return "ambush_exc_hide_cover";
		break;
	case 255451117:
		return "ambush_exc_lookout_attack";
		break;
	case 1494464635:
		return "ambush_exc_road_robbery";
		break;
	case 3112878453:
		return "ambush_exc_scm_prec";
		break;
	case 795152258:
		return "ambush_exc_wagon_bomb";
		break;
	case 3370101636:
		return "ambush_exc_wagon_turret";
		break;
	case 1131188991:
		return "ambush_gen_night_rob";
		break;
	case 3226924558:
		return "ambush_inb_bridge_ambush";
		break;
	case 403219433:
		return "ambush_inb_forest";
		break;
	case 1403359481:
		return "ambush_inb_forest_attack";
		break;
	case 1778626581:
		return "ambush_inb_harass";
		break;
	case 4000214781:
		return "ambush_inb_road_attack";
		break;
	case 982693049:
		return "ambush_odr_bridge_ambush";
		break;
	case 3223174354:
		return "ambush_odr_bridge_prevent";
		break;
	case 3678196734:
		return "ambush_odr_bridge_trap";
		break;
	case 3511044107:
		return "ambush_odr_hso";
		break;
	case 3356457729:
		return "ambush_odr_lookout_attack";
		break;
	case 45730399:
		return "ambush_odr_ride_out";
		break;
	case 3969829215:
		return "ambush_odr_road_prec";
		break;
	case 1929116863:
		return "ambush_odr_road_robbery";
		break;
	case 2234272261:
		return "ambush_pnk_type1";
		break;
	case 455010899:
		return "ambush_rnc_type1";
		break;
	case 1416029372:
		return "ambush_sav_corner";
		break;
	case 487301906:
		return "ambush_sav_forest_attack";
		break;
	case 2719390627:
		return "ambush_sav_lookout_attack";
		break;
	case 543617978:
		return "ambush_sav_river_ambush";
		break;
	case 1118373112:
		return "ambush_sav_tree_line";
		break;
	case 2829288818:
		return "angryisolationist";
		break;
	case 7359335:
		return "annesburg";
		break;
	case 3550472498:
		return "armadillo";
		break;
	case 4116539434:
		return "armoire_corpse_scenario";
		break;
	case 2888859982:
		return "asset_viewer_main";
		break;
	case 1733869312:
		return "audiotest";
		break;
	case 4009522551:
		return "av_alligator_boar_leg";
		break;
	case 2167775339:
		return "av_alligator_boar_neck";
		break;
	case 784790724:
		return "av_alligator_eat_dead_bird";
		break;
	case 1210206205:
		return "av_amb_camp_robbery";
		break;
	case 3148731318:
		return "av_animal_attack";
		break;
	case 3254156248:
		return "av_animal_carry_ride";
		break;
	case 1631181696:
		return "av_animal_scavenger";
		break;
	case 1023803332:
		return "av_barber_shaving";
		break;
	case 2135685140:
		return "av_bearblack_intimidate_bearblack";
		break;
	case 4252254507:
		return "av_bear_intimidate_wolf";
		break;
	case 4147534969:
		return "av_bear_run_catch_fish";
		break;
	case 3193622855:
		return "av_bear_scratch_back";
		break;
	case 242295766:
		return "av_bear_vs_wolves";
		break;
	case 673938227:
		return "av_beaver_swim_with_branch";
		break;
	case 2020081960:
		return "av_big_cat_ambush_deer";
		break;
	case 997644620:
		return "av_big_cat_kill_raccoon";
		break;
	case 1370815766:
		return "av_birds_in_tree";
		break;
	case 1663751108:
		return "av_bird_fence_swarm";
		break;
	case 2241213123:
		return "av_bird_flee_swarm";
		break;
	case 1712090190:
		return "av_bird_land";
		break;
	case 1217219997:
		return "av_bird_land_swarm";
		break;
	case 1468617093:
		return "av_bird_on_animal";
		break;
	case 75316194:
		return "av_bird_swarm";
		break;
	case 3819097478:
		return "av_boat_placement";
		break;
	case 1845750001:
		return "av_body_toss";
		break;
	case 1200498298:
		return "av_bucks_fighting";
		break;
	case 763143251:
		return "av_buck_and_doe";
		break;
	case 4255077481:
		return "av_buffalo_vs_buffalo";
		break;
	case 3234217479:
		return "av_butcher_shared";
		break;
	case 3381008257:
		return "av_cat_carry_rat";
		break;
	case 1465940190:
		return "av_cat_catch_bird";
		break;
	case 1138708920:
		return "av_chamberpot_toss";
		break;
	case 1460265988:
		return "av_coyote_catch_squirrel";
		break;
	case 4243729100:
		return "av_cross_road";
		break;
	case 3742328270:
		return "av_crows_and_eagles_eat_deer";
		break;
	case 1103924536:
		return "av_crows_and_vultures_eat_deer";
		break;
	case 3846736611:
		return "av_dead_animal";
		break;
	case 605571445:
		return "av_dead_horse";
		break;
	case 744860727:
		return "av_deer_antlers_stuck";
		break;
	case 2702137963:
		return "av_deer_tree_rub";
		break;
	case 1937367038:
		return "av_discoverable_parakeet";
		break;
	case 1879622779:
		return "av_dogs_playing";
		break;
	case 2663973653:
		return "av_dog_chase_cat";
		break;
	case 2162306876:
		return "av_dog_chase_tail";
		break;
	case 2681837693:
		return "av_dog_in_wagon";
		break;
	case 3118813547:
		return "av_drop_rabbit_table";
		break;
	case 2132633290:
		return "av_eagle_catch_fish";
		break;
	case 3542187723:
		return "av_eagle_catch_rabbit";
		break;
	case 582697607:
		return "av_eagle_catch_seasnake";
		break;
	case 2173542684:
		return "av_eagle_dive_catch_fish";
		break;
	case 3114930321:
		return "av_female_rider_male_walker";
		break;
	case 3371108343:
		return "av_fishing_river";
		break;
	case 2247685615:
		return "av_fox_catch_rodent";
		break;
	case 1702932216:
		return "av_fox_hunt_in_snow";
		break;
	case 629615895:
		return "av_fox_sit";
		break;
	case 1338035925:
		return "av_get_directions";
		break;
	case 1938954171:
		return "av_hawk_catch_snake";
		break;
	case 2050391888:
		return "av_hobo_train_hop";
		break;
	case 347823191:
		return "av_light_smoke";
		break;
	case 3659192880:
		return "av_lumber_pass";
		break;
	case 4136168223:
		return "av_macfarlane_farmer";
		break;
	case 837976954:
		return "av_mangy_dog_and_vultures";
		break;
	case 1996608666:
		return "av_man_jump_fence";
		break;
	case 3454657602:
		return "av_man_sit_read";
		break;
	case 3331307705:
		return "av_man_stand_at_wagon";
		break;
	case 2814034818:
		return "av_man_with_lantern";
		break;
	case 722616744:
		return "av_moose_slipping";
		break;
	case 4268231248:
		return "av_owl_catch_rat";
		break;
	case 2502267667:
		return "av_pelican_dive";
		break;
	case 2398897773:
		return "av_pump_cart";
		break;
	case 190892333:
		return "av_rams_headbutt";
		break;
	case 1684107554:
		return "av_rats_eating_group";
		break;
	case 3165408665:
		return "av_rats_eat_deer";
		break;
	case 454247025:
		return "av_rider_gallop";
		break;
	case 1028536701:
		return "av_ridgeline_animal";
		break;
	case 3509776625:
		return "av_ridgeline_native";
		break;
	case 1200118423:
		return "av_ridgeline_rider";
		break;
	case 2678403307:
		return "av_roving_herd";
		break;
	case 1338376671:
		return "av_scavenger_on_animal";
		break;
	case 3711298658:
		return "av_seagull_catch_fish";
		break;
	case 3196004020:
		return "av_sheep_grazing";
		break;
	case 262825778:
		return "av_shopkeeper_restock";
		break;
	case 3668317727:
		return "av_slow_rider";
		break;
	case 1328835395:
		return "av_squirrel_chase_squirrel";
		break;
	case 1209198301:
		return "av_stage_coach";
		break;
	case 3453578228:
		return "av_starting_to_rain";
		break;
	case 3918775824:
		return "av_swimming_animal";
		break;
	case 3292848993:
		return "av_swim_wolf_chase_moose";
		break;
	case 136056037:
		return "av_traveller_gallop_past";
		break;
	case 2517106643:
		return "av_vultures_eat_carcass";
		break;
	case 1868548374:
		return "av_wagon_back_passenger";
		break;
	case 3821159041:
		return "av_wagon_barrel";
		break;
	case 3146914582:
		return "av_wagon_music";
		break;
	case 2861334199:
		return "av_wagon_sweeper";
		break;
	case 1786120739:
		return "av_walk_horse_carry_deer";
		break;
	case 3020123590:
		return "av_walk_with_dog";
		break;
	case 2554783300:
		return "av_walk_with_horse";
		break;
	case 2224636238:
		return "av_wall_vomit";
		break;
	case 2064933084:
		return "av_water_horse";
		break;
	case 2509616939:
		return "av_whisper_sit";
		break;
	case 2368578054:
		return "av_whore_give_money";
		break;
	case 2014287148:
		return "av_wild_animal";
		break;
	case 3828158414:
		return "av_wolf_ambush_deer";
		break;
	case 359594735:
		return "av_wolf_flee_wolves";
		break;
	case 1803657673:
		return "av_wolf_takedown_miss";
		break;
	case 2443953514:
		return "av_wolves_alarmed";
		break;
	case 2347410869:
		return "av_wolves_eat_deer";
		break;
	case 2559707453:
		return "av_wolves_howling";
		break;
	case 3185225800:
		return "av_wolves_playing";
		break;
	case 3113841655:
		return "bandana";
		break;
	case 1466202449:
		return "bankrob01_invite";
		break;
	case 1329086997:
		return "bankrobberies_simple";
		break;
	case 1327728489:
		return "barcustomer_interaction";
		break;
	case 2794869592:
		return "bathing";
		break;
	case 1280482653:
		return "bayounwa_population";
		break;
	case 1969584186:
		return "beat_animal_attack";
		break;
	case 329126232:
		return "beat_animal_mauling";
		break;
	case 4115254708:
		return "beat_approach";
		break;
	case 1004387056:
		return "beat_arms_deal";
		break;
	case 2294919706:
		return "beat_author";
		break;
	case 315261560:
		return "beat_bandito_breakout";
		break;
	case 178511838:
		return "beat_bandito_execution";
		break;
	case 3264420138:
		return "beat_bear_trap";
		break;
	case 3085556445:
		return "beat_boat_attack";
		break;
	case 2502378033:
		return "beat_booby_trap";
		break;
	case 1974510096:
		return "beat_bounty_transport";
		break;
	case 662555111:
		return "beat_brontes_town_encounter";
		break;
	case 862013261:
		return "beat_bronte_patrol";
		break;
	case 2247979219:
		return "beat_burning_bodies";
		break;
	case 892187994:
		return "beat_campfire_ambush";
		break;
	case 3906269132:
		return "beat_chain_gang";
		break;
	case 2983149979:
		return "beat_checkpoint";
		break;
	case 560112719:
		return "beat_coach_robbery";
		break;
	case 3561443506:
		return "beat_consequence";
		break;
	case 1562521445:
		return "beat_corpse_cart";
		break;
	case 2290576693:
		return "beat_crashed_wagon";
		break;
	case 2586052630:
		return "beat_dark_alley_ambush";
		break;
	case 2191211547:
		return "beat_dark_alley_bum";
		break;
	case 1054720730:
		return "beat_dark_alley_stabber";
		break;
	case 3244766616:
		return "beat_dead_bodies";
		break;
	case 1992589327:
		return "beat_dead_john";
		break;
	case 3115422024:
		return "beat_del_lobo_posse";
		break;
	case 12829553:
		return "beat_diagnostics";
		break;
	case 2551452146:
		return "beat_disabled_beggar";
		break;
	case 231435979:
		return "beat_domestic_dispute";
		break;
	case 3585358130:
		return "beat_drown_murder";
		break;
	case 252107220:
		return "beat_drunk_camp";
		break;
	case 24673819:
		return "beat_drunk_dueler";
		break;
	case 4248465843:
		return "beat_duel_boaster";
		break;
	case 3902566516:
		return "beat_duel_winner";
		break;
	case 1717252640:
		return "beat_escort";
		break;
	case 1509872429:
		return "beat_executions";
		break;
	case 3428989461:
		return "beat_fleeing_family";
		break;
	case 2042485992:
		return "beat_fleeing_trespasser";
		break;
	case 3406336048:
		return "beat_foot_robbery";
		break;
	case 2764578318:
		return "beat_friendly_outdoorsman";
		break;
	case 566846282:
		return "beat_frozen_to_death";
		break;
	case 154006177:
		return "beat_fussar_chase";
		break;
	case 1667147145:
		return "beat_gang_camp_reminder";
		break;
	case 2618471337:
		return "beat_gang_ped1_encounter";
		break;
	case 1828484894:
		return "beat_gold_panner";
		break;
	case 4160924839:
		return "beat_herbalist_camp";
		break;
	case 1347312315:
		return "beat_horse_race";
		break;
	case 646331739:
		return "beat_hostage_rescue";
		break;
	case 639879517:
		return "beat_hunter";
		break;
	case 2840851085:
		return "beat_inbred_kidnap";
		break;
	case 1764574876:
		return "beat_injured_rider";
		break;
	case 4124061023:
		return "beat_intimidation_tactics";
		break;
	case 1917598358:
		return "beat_kidnap_victim";
		break;
	case 867647608:
		return "beat_laramie_gang_rustling";
		break;
	case 1787538230:
		return "beat_layout_editor";
		break;
	case 2298348796:
		return "beat_lemoyne_town_encounter";
		break;
	case 686654155:
		return "beat_locked_safe";
		break;
	case 4258704699:
		return "beat_lone_prisoner";
		break;
	case 2442975550:
		return "beat_lost_dog";
		break;
	case 3518935792:
		return "beat_lost_drunk";
		break;
	case 801327752:
		return "beat_lost_friend";
		break;
	case 1329599478:
		return "beat_lost_man";
		break;
	case 3150595431:
		return "beat_moonshine_camp";
		break;
	case 774479363:
		return "beat_murder_campfire";
		break;
	case 3039508739:
		return "beat_naked_swimmer";
		break;
	case 277870671:
		return "beat_odriscoll_town_encounter";
		break;
	case 1926102125:
		return "beat_on_the_run";
		break;
	case 2625520647:
		return "beat_outlaw_looter";
		break;
	case 955392:
		return "beat_outlaw_transport";
		break;
	case 800697180:
		return "beat_parlor_ambush";
		break;
	case 2258481162:
		return "beat_peeping_tom";
		break;
	case 654378511:
		return "beat_people_in_need_snake_bite";
		break;
	case 2416529792:
		return "beat_pickpocket";
		break;
	case 3333549364:
		return "beat_piss_pot";
		break;
	case 911461481:
		return "beat_player_camp_attack";
		break;
	case 2880465632:
		return "beat_player_camp_stranger";
		break;
	case 3428826602:
		return "beat_poisoned";
		break;
	case 1682425533:
		return "beat_police_chase";
		break;
	case 2764519459:
		return "beat_posse_breakout";
		break;
	case 1961612300:
		return "beat_prison_wagon";
		break;
	case 452324593:
		return "beat_public_hanging";
		break;
	case 1905203226:
		return "beat_rally";
		break;
	case 2393041995:
		return "beat_rally_dispute";
		break;
	case 1172209857:
		return "beat_rally_setup";
		break;
	case 4071740637:
		return "beat_rat_infestation";
		break;
	case 3415418685:
		return "beat_rifle_practice";
		break;
	case 2828915864:
		return "beat_rowdy_drunks";
		break;
	case 1000355129:
		return "beat_savage_aftermath";
		break;
	case 3933433721:
		return "beat_savage_fight";
		break;
	case 1843462482:
		return "beat_savage_wagon";
		break;
	case 2074255092:
		return "beat_savage_warning";
		break;
	case 2986356480:
		return "beat_sharp_shooter";
		break;
	case 3979279681:
		return "beat_show_off";
		break;
	case 2064175054:
		return "beat_skipping_stones";
		break;
	case 2941187713:
		return "beat_slum_ambush";
		break;
	case 5315657:
		return "beat_spooked_horse";
		break;
	case 1746187532:
		return "beat_stalking_shadows";
		break;
	case 3164213996:
		return "beat_stranded_rider";
		break;
	case 2324016277:
		return "beat_street_fight";
		break;
	case 2446996003:
		return "beat_taunting";
		break;
	case 1752956204:
		return "beat_torch_procession";
		break;
	case 2037814832:
		return "beat_torturing_captive";
		break;
	case 648911107:
		return "beat_town_burial";
		break;
	case 3356808208:
		return "beat_town_confrontation";
		break;
	case 3800417909:
		return "beat_town_robbery";
		break;
	case 2159620550:
		return "beat_town_trouble";
		break;
	case 4252138516:
		return "beat_town_widow";
		break;
	case 2715407105:
		return "beat_traffic_attack";
		break;
	case 910719791:
		return "beat_train_holdup";
		break;
	case 2302456923:
		return "beat_trapped_woman";
		break;
	case 1665631644:
		return "beat_treasure_hunter";
		break;
	case 2585592143:
		return "beat_voice";
		break;
	case 2130358493:
		return "beat_wagon_threat";
		break;
	case 3089793123:
		return "beat_washed_ashore";
		break;
	case 137763809:
		return "beat_wealthy_couple";
		break;
	case 2528809788:
		return "beat_wilderness_hanging";
		break;
	case 2928648228:
		return "beat_wild_man";
		break;
	case 2504981505:
		return "beat_wild_man_cave";
		break;
	case 2954544042:
		return "beaverhollow";
		break;
	case 773227447:
		return "beechers2_2";
		break;
	case 2668080277:
		return "beechers2_2_intro";
		break;
	case 2972196419:
		return "beechers2_2_outro";
		break;
	case 935052306:
		return "beechersc";
		break;
	case 2586580314:
		return "beechershope";
		break;
	case 1703557170:
		return "beechers_cow";
		break;
	case 3846755197:
		return "benchmark";
		break;
	case 3762369910:
		return "benedictpoint";
		break;
	case 1940339958:
		return "bigvalleytrapper";
		break;
	case 2537961006:
		return "bigvalley_population";
		break;
	case 3984806861:
		return "binoculars";
		break;
	case 218357474:
		return "bjack_launch_sp";
		break;
	case 648454729:
		return "bjack_sp";
		break;
	case 973608053:
		return "blackbalsamrise";
		break;
	case 1053078005:
		return "blackwater";
		break;
	case 2621047162:
		return "bluegillmarsh_population";
		break;
	case 2708170015:
		return "bounty1";
		break;
	case 2718868166:
		return "bounty1_intro";
		break;
	case 2662207990:
		return "bounty1_outro";
		break;
	case 1629771901:
		return "bounty_poster";
		break;
	case 4133044184:
		return "braithwaitemanor";
		break;
	case 4006900035:
		return "braithwaites1";
		break;
	case 2878692864:
		return "braithwaites1_intro";
		break;
	case 2418397964:
		return "braithwaites1_outro";
		break;
	case 3775518126:
		return "braithwaites2";
		break;
	case 3326639181:
		return "braithwaites2_intro";
		break;
	case 545170718:
		return "braithwaites2_outro";
		break;
	case 194652874:
		return "braithwaites3";
		break;
	case 1358240305:
		return "braithwaites3_intro";
		break;
	case 1164440954:
		return "broom_scenario";
		break;
	case 446610303:
		return "butchercreek";
		break;
	case 2654962627:
		return "caligahall";
		break;
	case 4202550627:
		return "camera_item";
		break;
	case 778601866:
		return "camera_photomode";
		break;
	case 4097253396:
		return "campfire";
		break;
	case 3421025685:
		return "campfire_always";
		break;
	case 148267983:
		return "campfire_gang";
		break;
	case 3090917225:
		return "campfire_gang_es";
		break;
	case 4106601517:
		return "campfire_gang_launch";
		break;
	case 833544090:
		return "camp_beaverhollow";
		break;
	case 3505878049:
		return "camp_beechershope";
		break;
	case 2253485190:
		return "camp_clemenspoint";
		break;
	case 1442468227:
		return "camp_colter";
		break;
	case 2915668072:
		return "camp_fire_cooking";
		break;
	case 2256552121:
		return "camp_guarma";
		break;
	case 297498058:
		return "camp_horseshoeoverlook";
		break;
	case 188702764:
		return "camp_lakay";
		break;
	case 557522521:
		return "camp_pronghornranch";
		break;
	case 3508323330:
		return "camp_shadybelle";
		break;
	case 100690523:
		return "camp_tester";
		break;
	case 3851598696:
		return "caravan_absence_scene";
		break;
	case 2932636699:
		return "caravan_state_debug_ui";
		break;
	case 2897442688:
		return "carmodydell";
		break;
	case 3272420814:
		return "catfishjacksons";
		break;
	case 2292626395:
		return "cheat_ui";
		break;
	case 3453576172:
		return "chezporter";
		break;
	case 1199407794:
		return "chollasprings_population";
		break;
	case 3570273752:
		return "civilized_region_test";
		break;
	case 2921769406:
		return "civilwarbride";
		break;
	case 3499965072:
		return "clemenscove";
		break;
	case 3845433072:
		return "clemenspoint";
		break;
	case 3418033156:
		return "coachrobberies";
		break;
	case 862964864:
		return "coachrobberies_gang1";
		break;
	case 2086914330:
		return "coachrobberies_gang1_invite";
		break;
	case 3097089754:
		return "coachrobberies_gang2";
		break;
	case 4181682022:
		return "coachrobberies_gang2_invite";
		break;
	case 1708306765:
		return "coachrobberies_gang3";
		break;
	case 3874001010:
		return "coachrobberies_gang3_invite";
		break;
	case 1555341073:
		return "coachrobberies_gang4";
		break;
	case 475089045:
		return "coachrobberies_gang4_invite";
		break;
	case 2192150657:
		return "coachrobberies_intro";
		break;
	case 1774218047:
		return "coachrobberies_note";
		break;
	case 52395882:
		return "coffee_drinking";
		break;
	case 1427412083:
		return "collectible_posters";
		break;
	case 754430623:
		return "colter";
		break;
	case 3657406415:
		return "combat_test";
		break;
	case 2737580753:
		return "cornwall1";
		break;
	case 599912375:
		return "cornwall1_intro";
		break;
	case 927563817:
		return "cornwall1_outro";
		break;
	case 2747318607:
		return "cornwallkerosene";
		break;
	case 2120602169:
		return "crawdadwillies";
		break;
	case 4203710762:
		return "crayfish_search";
		break;
	case 1459883022:
		return "cropfarm";
		break;
	case 2314968282:
		return "cumberlandforest_population";
		break;
	case 3435659272:
		return "cv_amb_butcher_bleed_deer";
		break;
	case 991516100:
		return "cv_amb_butcher_feed_dog";
		break;
	case 3854232552:
		return "cv_amb_butcher_hang_carcass";
		break;
	case 1009037546:
		return "cv_amb_comp_donate";
		break;
	case 2804590563:
		return "cv_amb_comp_donate_look";
		break;
	case 2525339245:
		return "cv_amb_dog_jump_and_lick";
		break;
	case 3402176525:
		return "cv_amb_dog_jump_bed";
		break;
	case 297823695:
		return "cv_amb_dog_lick_table";
		break;
	case 288777091:
		return "cv_amb_dog_pet_sitting";
		break;
	case 2367658543:
		return "cv_amb_drop_rabbit_table";
		break;
	case 1604168785:
		return "cv_amb_drunk_campfire";
		break;
	case 3351435663:
		return "cv_amb_fire_starter";
		break;
	case 1962636996:
		return "cv_amb_jack_look_pot";
		break;
	case 676224050:
		return "cv_amb_jack_poke_fire";
		break;
	case 4142603290:
		return "cv_amb_light_smoke";
		break;
	case 782459325:
		return "cv_amb_sack_wagon";
		break;
	case 2895694559:
		return "cv_amb_sore_joints";
		break;
	case 2337774611:
		return "cv_amb_start_phonograph";
		break;
	case 1639480402:
		return "cv_amb_whisper_sit";
		break;
	case 1968168972:
		return "cv_amb_whisper_stand";
		break;
	case 3969345014:
		return "cv_cc_abi_01";
		break;
	case 2775734185:
		return "cv_cc_abi_02";
		break;
	case 3608394479:
		return "cv_cc_abi_03";
		break;
	case 2201424691:
		return "cv_cc_abi_04";
		break;
	case 3019502776:
		return "cv_cc_abi_05";
		break;
	case 1839622162:
		return "cv_cc_abi_06";
		break;
	case 1617677725:
		return "cv_cc_abi_07";
		break;
	case 1281467785:
		return "cv_cc_abi_08";
		break;
	case 1042188547:
		return "cv_cc_abi_09";
		break;
	case 1609420213:
		return "cv_cc_abi_10";
		break;
	case 2435133475:
		return "cv_cc_abi_11";
		break;
	case 1015678702:
		return "cv_cc_abi_12";
		break;
	case 1841850730:
		return "cv_cc_abi_13";
		break;
	case 956858343:
		return "cv_cc_abi_14";
		break;
	case 1256039317:
		return "cv_cc_abi_15";
		break;
	case 359872701:
		return "cv_cc_abi_16";
		break;
	case 120364080:
		return "cv_cc_abi_17";
		break;
	case 3811562551:
		return "cv_cc_abi_19";
		break;
	case 1607844033:
		return "cv_cc_abi_20";
		break;
	case 400470228:
		return "cv_cc_abi_21";
		break;
	case 1012562379:
		return "cv_cc_abi_23";
		break;
	case 1953615575:
		return "cv_cc_am_cln_shared";
		break;
	case 2511556720:
		return "cv_cc_am_cnf_01";
		break;
	case 1183134229:
		return "cv_cc_am_cnf_02";
		break;
	case 1699966897:
		return "cv_cc_am_cnf_03";
		break;
	case 3794790760:
		return "cv_cc_am_cnf_04";
		break;
	case 3502163590:
		return "cv_cc_am_cnf_05";
		break;
	case 3633984046:
		return "cv_cc_bll_04";
		break;
	case 333228210:
		return "cv_cc_bll_05";
		break;
	case 85658415:
		return "cv_cc_bll_06";
		break;
	case 3937556038:
		return "cv_cc_bll_08";
		break;
	case 619236018:
		return "cv_cc_bll_09";
		break;
	case 3107485351:
		return "cv_cc_bll_10";
		break;
	case 2338724611:
		return "cv_cc_bll_11";
		break;
	case 4085050159:
		return "cv_cc_bll_13";
		break;
	case 3317895100:
		return "cv_cc_bll_14";
		break;
	case 3561860305:
		return "cv_cc_bll_15";
		break;
	case 2445572892:
		return "cv_cc_chr_01";
		break;
	case 673687520:
		return "cv_cc_chr_02";
		break;
	case 910312469:
		return "cv_cc_chr_03";
		break;
	case 3632662686:
		return "cv_cc_chr_05";
		break;
	case 476169514:
		return "cv_cc_dna_01";
		break;
	case 2682306673:
		return "cv_cc_dnh_01";
		break;
	case 2981979178:
		return "cv_cc_dnh_02";
		break;
	case 3445005148:
		return "cv_cc_dnh_03";
		break;
	case 3690608803:
		return "cv_cc_dnh_04";
		break;
	case 3905704519:
		return "cv_cc_dnh_05";
		break;
	case 4209702532:
		return "cv_cc_dnh_06";
		break;
	case 3949156217:
		return "cv_cc_dnh_07";
		break;
	case 67864777:
		return "cv_cc_dnh_08";
		break;
	case 366029908:
		return "cv_cc_dnh_09";
		break;
	case 2561817836:
		return "cv_cc_dnh_10";
		break;
	case 3423511460:
		return "cv_cc_dnh_11";
		break;
	case 3184592681:
		return "cv_cc_dnh_12";
		break;
	case 2979162244:
		return "cv_cc_dnj_01";
		break;
	case 3316420736:
		return "cv_cc_dnj_02";
		break;
	case 3824012546:
		return "cv_cc_dnj_03";
		break;
	case 2505879521:
		return "cv_cc_dnj_04";
		break;
	case 2141357165:
		return "cv_cc_dnj_05";
		break;
	case 14157526:
		return "cv_cc_dnj_06";
		break;
	case 790029139:
		return "cv_cc_dnj_07";
		break;
	case 3044322057:
		return "cv_cc_dnk_01";
		break;
	case 1896424243:
		return "cv_cc_dnk_02";
		break;
	case 1861295875:
		return "cv_cc_dnk_03";
		break;
	case 1570929766:
		return "cv_cc_dnk_04";
		break;
	case 3470049789:
		return "cv_cc_dnl_01";
		break;
	case 2232921740:
		return "cv_cc_dnl_02";
		break;
	case 1011234636:
		return "cv_cc_dnm_01";
		break;
	case 2286800730:
		return "cv_cc_dnm_02";
		break;
	case 2500389072:
		return "cv_cc_dnm_03";
		break;
	case 1411500542:
		return "cv_cc_dnv_02";
		break;
	case 1818308951:
		return "cv_cc_dog_01";
		break;
	case 610935146:
		return "cv_cc_dog_04";
		break;
	case 931907501:
		return "cv_cc_dog_05";
		break;
	case 1356790355:
		return "cv_cc_dog_06";
		break;
	case 945211707:
		return "cv_cc_dog_08";
		break;
	case 1114387545:
		return "cv_cc_dtc_03";
		break;
	case 1357959522:
		return "cv_cc_dtc_04";
		break;
	case 1777763625:
		return "cv_cc_dtc_10";
		break;
	case 3306142554:
		return "cv_cc_dtc_14";
		break;
	case 3607322433:
		return "cv_cc_dtc_15";
		break;
	case 3627704599:
		return "cv_cc_dtc_17";
		break;
	case 3459009787:
		return "cv_cc_dtc_18";
		break;
	case 3152193640:
		return "cv_cc_dtc_19";
		break;
	case 1139651872:
		return "cv_cc_dtc_21";
		break;
	case 1093775324:
		return "cv_cc_dtc_22";
		break;
	case 721126256:
		return "cv_cc_dtc_23";
		break;
	case 1540842791:
		return "cv_cc_dtc_24";
		break;
	case 1432901705:
		return "cv_cc_dtc_25";
		break;
	case 2766075705:
		return "cv_cc_dtc_28";
		break;
	case 3197119131:
		return "cv_cc_dtc_29";
		break;
	case 3910303939:
		return "cv_cc_dtc_30";
		break;
	case 4283411773:
		return "cv_cc_dtc_31";
		break;
	case 72529719:
		return "cv_cc_dtc_32";
		break;
	case 319378596:
		return "cv_cc_dtc_33";
		break;
	case 673939176:
		return "cv_cc_dtc_34";
		break;
	case 1210355359:
		return "cv_cc_dtc_shared";
		break;
	case 2603891913:
		return "cv_cc_hsh_01";
		break;
	case 3081303474:
		return "cv_cc_hsh_03";
		break;
	case 418232386:
		return "cv_cc_hsh_04";
		break;
	case 939783790:
		return "cv_cc_hsh_06";
		break;
	case 634737169:
		return "cv_cc_hsh_07";
		break;
	case 3548360039:
		return "cv_cc_hsh_08";
		break;
	case 3249146300:
		return "cv_cc_hsh_09";
		break;
	case 691362241:
		return "cv_cc_hsh_11";
		break;
	case 3764406296:
		return "cv_cc_hsh_12";
		break;
	case 3331921034:
		return "cv_cc_hsh_13";
		break;
	case 2219741174:
		return "cv_cc_hsh_14";
		break;
	case 4069387379:
		return "cv_cc_hsh_15";
		break;
	case 2567518659:
		return "cv_cc_hsh_16";
		break;
	case 2907726417:
		return "cv_cc_hsh_17";
		break;
	case 4118966932:
		return "cv_cc_hsh_18";
		break;
	case 2325191904:
		return "cv_cc_hsh_19";
		break;
	case 4044616455:
		return "cv_cc_jav_01";
		break;
	case 259141571:
		return "cv_cc_jav_02";
		break;
	case 1375188153:
		return "cv_cc_jav_03";
		break;
	case 1149147591:
		return "cv_cc_jav_04";
		break;
	case 2093582940:
		return "cv_cc_jav_05";
		break;
	case 184821463:
		return "cv_cc_jav_07";
		break;
	case 4249160537:
		return "cv_cc_jav_08";
		break;
	case 931135438:
		return "cv_cc_jav_09";
		break;
	case 3153922594:
		return "cv_cc_jav_10";
		break;
	case 1752523540:
		return "cv_cc_jav_11";
		break;
	case 571942302:
		return "cv_cc_jck_01";
		break;
	case 4159721801:
		return "cv_cc_jck_02";
		break;
	case 208108087:
		return "cv_cc_jck_03";
		break;
	case 2446296321:
		return "cv_cc_jck_04";
		break;
	case 2791353891:
		return "cv_cc_jck_05";
		break;
	case 1004924358:
		return "cv_cc_jck_par_01";
		break;
	case 897164363:
		return "cv_cc_jhn_01";
		break;
	case 825138101:
		return "cv_cc_jhn_02";
		break;
	case 2717252930:
		return "cv_cc_jhn_03";
		break;
	case 2344866014:
		return "cv_cc_jhn_04";
		break;
	case 1756465850:
		return "cv_cc_jhn_05";
		break;
	case 3665096255:
		return "cv_cc_jhn_06";
		break;
	case 3365751440:
		return "cv_cc_jhn_07";
		break;
	case 3126308357:
		return "cv_cc_jhn_08";
		break;
	case 2760934007:
		return "cv_cc_jhn_09";
		break;
	case 904511855:
		return "cv_cc_jhn_10";
		break;
	case 719449307:
		return "cv_cc_kar_01";
		break;
	case 239842223:
		return "cv_cc_kar_02";
		break;
	case 333602:
		return "cv_cc_kar_03";
		break;
	case 1864856933:
		return "cv_cc_kar_04";
		break;
	case 1676238569:
		return "cv_cc_kar_05";
		break;
	case 1166352929:
		return "cv_cc_kar_06";
		break;
	case 936543932:
		return "cv_cc_kar_07";
		break;
	case 1209804627:
		return "cv_cc_kar_08";
		break;
	case 971639535:
		return "cv_cc_kar_09";
		break;
	case 4262530522:
		return "cv_cc_kar_10";
		break;
	case 3959581113:
		return "cv_cc_kar_11";
		break;
	case 423019557:
		return "cv_cc_kar_12";
		break;
	case 248360787:
		return "cv_cc_kar_13";
		break;
	case 408647090:
		return "cv_cc_kar_par_02";
		break;
	case 169957694:
		return "cv_cc_kar_par_03";
		break;
	case 3357463324:
		return "cv_cc_kie_01";
		break;
	case 3182804554:
		return "cv_cc_kie_04";
		break;
	case 2139439594:
		return "cv_cc_kie_05";
		break;
	case 1659406513:
		return "cv_cc_kie_07";
		break;
	case 1957571644:
		return "cv_cc_kie_08";
		break;
	case 2138522038:
		return "cv_cc_kie_09";
		break;
	case 2126689281:
		return "cv_cc_kie_10";
		break;
	case 2325597111:
		return "cv_cc_kie_11";
		break;
	case 2620288728:
		return "cv_cc_kie_12";
		break;
	case 637633148:
		return "cv_cc_kie_13";
		break;
	case 936322583:
		return "cv_cc_kie_14";
		break;
	case 1668709733:
		return "cv_cc_kie_15";
		break;
	case 1551340484:
		return "cv_cc_len_01";
		break;
	case 3523051206:
		return "cv_cc_len_02";
		break;
	case 3242188107:
		return "cv_cc_len_03";
		break;
	case 2773067103:
		return "cv_cc_len_04";
		break;
	case 2474836434:
		return "cv_cc_len_05";
		break;
	case 146730056:
		return "cv_cc_len_06";
		break;
	case 4145367285:
		return "cv_cc_len_07";
		break;
	case 2129123484:
		return "cv_cc_len_08";
		break;
	case 3009758924:
		return "cv_cc_lnsnm_01";
		break;
	case 4064041133:
		return "cv_cc_mch_02";
		break;
	case 2748398552:
		return "cv_cc_mch_03";
		break;
	case 3210146531:
		return "cv_cc_mch_05";
		break;
	case 2834712098:
		return "cv_cc_mch_06";
		break;
	case 1464476359:
		return "cv_cc_mch_07";
		break;
	case 2223668551:
		return "cv_cc_mch_08";
		break;
	case 495234601:
		return "cv_cc_mch_10";
		break;
	case 3954625162:
		return "cv_cc_mch_11";
		break;
	case 4196722534:
		return "cv_cc_mch_12";
		break;
	case 3359245201:
		return "cv_cc_mch_13";
		break;
	case 2825700343:
		return "cv_cc_mch_15";
		break;
	case 3299212393:
		return "cv_cc_mch_16";
		break;
	case 2464684270:
		return "cv_cc_mch_17";
		break;
	case 2702849362:
		return "cv_cc_mch_18";
		break;
	case 1866682785:
		return "cv_cc_mch_19";
		break;
	case 213866967:
		return "cv_cc_mch_20";
		break;
	case 4268539186:
		return "cv_cc_mch_21";
		break;
	case 2702312062:
		return "cv_cc_mch_22";
		break;
	case 3314535289:
		return "cv_cc_mch_24";
		break;
	case 3073978060:
		return "cv_cc_mch_25";
		break;
	case 1605959629:
		return "cv_cc_mch_26";
		break;
	case 1232786257:
		return "cv_cc_mch_27";
		break;
	case 2187904300:
		return "cv_cc_mch_28";
		break;
	case 1812043870:
		return "cv_cc_mch_29";
		break;
	case 2738259943:
		return "cv_cc_mch_30";
		break;
	case 2643754147:
		return "cv_cc_mch_31";
		break;
	case 3333508828:
		return "cv_cc_mch_32";
		break;
	case 2968396630:
		return "cv_cc_mch_33";
		break;
	case 4286140690:
		return "cv_cc_mll_01";
		break;
	case 837498357:
		return "cv_cc_mll_02";
		break;
	case 3725954635:
		return "cv_cc_mll_03";
		break;
	case 281146275:
		return "cv_cc_mll_04";
		break;
	case 1801169105:
		return "cv_cc_mll_05";
		break;
	case 1563561086:
		return "cv_cc_mll_06";
		break;
	case 1206772218:
		return "cv_cc_mll_07";
		break;
	case 966346065:
		return "cv_cc_mll_08";
		break;
	case 2978395430:
		return "cv_cc_mll_09";
		break;
	case 1070413932:
		return "cv_cc_mrb_01";
		break;
	case 360702942:
		return "cv_cc_mrb_02";
		break;
	case 121718625:
		return "cv_cc_mrb_03";
		break;
	case 4175637157:
		return "cv_cc_mrb_04";
		break;
	case 3941830342:
		return "cv_cc_mrb_05";
		break;
	case 3684724768:
		return "cv_cc_mrb_06";
		break;
	case 3463960015:
		return "cv_cc_mrb_07";
		break;
	case 3206362906:
		return "cv_cc_mrb_08";
		break;
	case 2984778928:
		return "cv_cc_mrb_09";
		break;
	case 2079174564:
		return "cv_cc_mrb_10";
		break;
	case 2352795714:
		return "cv_cc_mrb_11";
		break;
	case 230118201:
		return "cv_cc_mrb_12";
		break;
	case 17547124:
		return "cv_cc_prs_01";
		break;
	case 2714632442:
		return "cv_cc_prs_02";
		break;
	case 3551651009:
		return "cv_cc_prs_03";
		break;
	case 952282849:
		return "cv_cc_prs_04";
		break;
	case 671157598:
		return "cv_cc_prs_05";
		break;
	case 4237309853:
		return "cv_cc_prs_13";
		break;
	case 3181853132:
		return "cv_cc_prs_14";
		break;
	case 3491094185:
		return "cv_cc_prs_15";
		break;
	case 299131429:
		return "cv_cc_prs_16";
		break;
	case 616171504:
		return "cv_cc_prs_17";
		break;
	case 1382340565:
		return "cv_cc_prs_callover";
		break;
	case 3191968060:
		return "cv_cc_prs_par_01";
		break;
	case 3956783512:
		return "cv_cc_sad_01";
		break;
	case 2687246914:
		return "cv_cc_sad_02";
		break;
	case 3471474622:
		return "cv_cc_sad_03";
		break;
	case 2906606216:
		return "cv_cc_sen_01";
		break;
	case 3214110452:
		return "cv_cc_sen_02";
		break;
	case 3511849586:
		return "cv_cc_sen_03";
		break;
	case 2618337263:
		return "cv_cc_sen_04";
		break;
	case 2915486555:
		return "cv_cc_sen_05";
		break;
	case 1957288230:
		return "cv_cc_sen_06";
		break;
	case 2264169915:
		return "cv_cc_sen_07";
		break;
	case 1345654845:
		return "cv_cc_sen_08";
		break;
	case 1651946688:
		return "cv_cc_sen_09";
		break;
	case 2860301027:
		return "cv_cc_sen_10";
		break;
	case 2621513324:
		return "cv_cc_sen_11";
		break;
	case 117437416:
		return "cv_cc_sen_12";
		break;
	case 4173158243:
		return "cv_cc_sen_13";
		break;
	case 110949150:
		return "cv_cc_sen_14";
		break;
	case 4174796689:
		return "cv_cc_sen_15";
		break;
	case 3775997959:
		return "cv_cc_sen_16";
		break;
	case 3520399759:
		return "cv_cc_sen_17";
		break;
	case 2204657793:
		return "cv_cc_snk_par_01";
		break;
	case 2982856013:
		return "cv_cc_snk_par_02";
		break;
	case 2684953034:
		return "cv_cc_snk_par_03";
		break;
	case 3968160260:
		return "cv_cc_ssnunc_01";
		break;
	case 1825822573:
		return "cv_cc_ssn_01";
		break;
	case 2073588982:
		return "cv_cc_ssn_02";
		break;
	case 2417925634:
		return "cv_cc_ssn_03";
		break;
	case 2669427709:
		return "cv_cc_ssn_04";
		break;
	case 3599837926:
		return "cv_cc_ssn_05";
		break;
	case 4194824659:
		return "cv_cc_ssn_07";
		break;
	case 3343486039:
		return "cv_cc_ssn_08";
		break;
	case 809558353:
		return "cv_cc_ssn_10";
		break;
	case 594364330:
		return "cv_cc_ssn_11";
		break;
	case 347023918:
		return "cv_cc_ssn_12";
		break;
	case 4166971790:
		return "cv_cc_ssn_13";
		break;
	case 3927528707:
		return "cv_cc_ssn_14";
		break;
	case 3458309396:
		return "cv_cc_ssn_16";
		break;
	case 3913407378:
		return "cv_cc_str_02";
		break;
	case 3589453044:
		return "cv_cc_str_03";
		break;
	case 4233402237:
		return "cv_cc_swn_06";
		break;
	case 1166027219:
		return "cv_cc_swn_07";
		break;
	case 1455344720:
		return "cv_cc_swn_08";
		break;
	case 803602079:
		return "cv_cc_swn_09";
		break;
	case 334022653:
		return "cv_cc_swn_10";
		break;
	case 1254536632:
		return "cv_cc_swn_11";
		break;
	case 948244789:
		return "cv_cc_swn_12";
		break;
	case 3736821163:
		return "cv_cc_swn_13";
		break;
	case 3430201630:
		return "cv_cc_swn_14";
		break;
	case 61679494:
		return "cv_cc_swn_15";
		break;
	case 4042752539:
		return "cv_cc_swn_16";
		break;
	case 2543046493:
		return "cv_cc_swn_17";
		break;
	case 3153074197:
		return "cv_cc_swn_19";
		break;
	case 2512666370:
		return "cv_cc_swn_21";
		break;
	case 2215615385:
		return "cv_cc_swn_22";
		break;
	case 541202190:
		return "cv_cc_tll_01";
		break;
	case 2321935192:
		return "cv_cc_tll_02";
		break;
	case 2061454411:
		return "cv_cc_tll_03";
		break;
	case 1706631679:
		return "cv_cc_tll_04";
		break;
	case 1467057520:
		return "cv_cc_tll_05";
		break;
	case 3345147221:
		return "cv_cc_tll_06";
		break;
	case 431163153:
		return "cv_cc_trl_01";
		break;
	case 1979531152:
		return "cv_cc_trl_03";
		break;
	case 3341894198:
		return "cv_cc_unc_01";
		break;
	case 1232914127:
		return "cv_cc_unc_02";
		break;
	case 1539828581:
		return "cv_cc_unc_03";
		break;
	case 771559376:
		return "cv_cc_unc_04";
		break;
	case 1081291964:
		return "cv_cc_unc_05";
		break;
	case 2343750458:
		return "cv_cc_unc_06";
		break;
	case 2650959833:
		return "cv_cc_unc_07";
		break;
	case 1886721215:
		return "cv_cc_unc_08";
		break;
	case 2671606240:
		return "cv_cf_am_01";
		break;
	case 1616477186:
		return "cv_cf_shared";
		break;
	case 1413027356:
		return "cv_end_abi_01";
		break;
	case 2189947577:
		return "cv_end_abi_02";
		break;
	case 2773792850:
		return "cv_end_abi_04";
		break;
	case 191726730:
		return "cv_end_abi_05";
		break;
	case 971006319:
		return "cv_end_abi_06";
		break;
	case 807489009:
		return "cv_end_abi_07";
		break;
	case 1569638029:
		return "cv_end_abi_slp_shared";
		break;
	case 1546677440:
		return "cv_end_anc_01";
		break;
	case 1997815742:
		return "cv_end_anj_01";
		break;
	case 2630224673:
		return "cv_end_anj_02";
		break;
	case 2457335429:
		return "cv_end_anj_03";
		break;
	case 3129132698:
		return "cv_end_anj_04";
		break;
	case 782327625:
		return "cv_end_anu_01";
		break;
	case 452433266:
		return "cv_end_chr_01";
		break;
	case 4135439487:
		return "cv_end_chr_03";
		break;
	case 1070096159:
		return "cv_end_chr_05";
		break;
	case 519981781:
		return "cv_end_cnj_01";
		break;
	case 4129148014:
		return "cv_end_cnu_01";
		break;
	case 3838585291:
		return "cv_end_cnu_02";
		break;
	case 3532719445:
		return "cv_end_cnu_03";
		break;
	case 3208240807:
		return "cv_end_cnu_04";
		break;
	case 3440540252:
		return "cv_end_cnu_05";
		break;
	case 1477063087:
		return "cv_end_jck_01";
		break;
	case 2133786608:
		return "cv_end_jck_03";
		break;
	case 793468970:
		return "cv_end_jck_04";
		break;
	case 1098712205:
		return "cv_end_jck_05";
		break;
	case 379498193:
		return "cv_end_jck_06";
		break;
	case 3348674010:
		return "cv_end_unc_01";
		break;
	case 1846837975:
		return "cv_end_unc_02";
		break;
	case 3667319770:
		return "cv_end_unc_04";
		break;
	case 1212888901:
		return "cv_end_unc_06";
		break;
	case 1452856288:
		return "cv_end_unc_07";
		break;
	case 4127851545:
		return "cv_end_unj_01";
		break;
	case 2515036483:
		return "cv_itm_abi_01";
		break;
	case 1179451234:
		return "cv_itm_bll_01";
		break;
	case 960130360:
		return "cv_itm_chr_01";
		break;
	case 709480279:
		return "cv_itm_chr_02";
		break;
	case 3631655858:
		return "cv_itm_chr_03";
		break;
	case 1723656007:
		return "cv_itm_dtc_01";
		break;
	case 3186439233:
		return "cv_itm_hsh_01";
		break;
	case 3236529865:
		return "cv_itm_jav_01";
		break;
	case 2153276407:
		return "cv_itm_kar_01";
		break;
	case 1630371424:
		return "cv_itm_kie_01";
		break;
	case 3502396063:
		return "cv_itm_mrb_02";
		break;
	case 3552998505:
		return "cv_itm_prs_01";
		break;
	case 3072556408:
		return "cv_itm_sen_01";
		break;
	case 1555370054:
		return "cv_mus_shared";
		break;
	case 3942780093:
		return "cv_mus_swn_01";
		break;
	case 925172360:
		return "cv_sph_dtc_bvh_01";
		break;
	case 3118608484:
		return "cv_sph_dtc_clm_01";
		break;
	case 3028773793:
		return "cv_sph_dtc_sdb_01";
		break;
	case 381313112:
		return "cv_sph_prs_low_food_01";
		break;
	case 1721125537:
		return "cv_sph_sen_hso_01";
		break;
	case 3121773341:
		return "cv_sph_ssn_low_food_01";
		break;
	case 46168290:
		return "cv_wnt_dtc_downes01";
		break;
	case 2849918928:
		return "cv_wnt_dtc_hso_01";
		break;
	case 510020944:
		return "cv_wnt_shared";
		break;
	case 1426879920:
		return "cv_wnt_str_04";
		break;
	case 4037159382:
		return "cv_wnt_swn_18";
		break;
	case 3419272692:
		return "cv_wrk_bll_01";
		break;
	case 3597208370:
		return "cv_wrk_bll_02";
		break;
	case 3170768924:
		return "cv_wrk_hsh_01";
		break;
	case 611313386:
		return "cv_wrk_hsh_02";
		break;
	case 1675236371:
		return "cv_wrk_jav_01";
		break;
	case 729125684:
		return "cv_wrk_mch_01";
		break;
	case 1096269560:
		return "cv_wrk_mch_02";
		break;
	case 3595095123:
		return "cv_wrk_swn_01";
		break;
	case 2664033667:
		return "dairyfarm";
		break;
	case 1778316593:
		return "dakotarivertrapper";
		break;
	case 3478785691:
		return "deadrival";
		break;
	case 121538324:
		return "deadsettler";
		break;
	case 4131157756:
		return "debug_fast_travel";
		break;
	case 2434108300:
		return "debug_generic_widgets";
		break;
	case 1439595205:
		return "debug_launcher";
		break;
	case 199064273:
		return "debug_metapeditor";
		break;
	case 278878453:
		return "debug_metapeditor_custom_launch";
		break;
	case 548183821:
		return "debug_newspapers";
		break;
	case 2347113815:
		return "debug_update";
		break;
	case 89040947:
		return "def_comp_brain";
		break;
	case 3051458192:
		return "def_intro_script";
		break;
	case 229447731:
		return "dinolady";
		break;
	case 3854779999:
		return "dino_bones";
		break;
	case 4207953618:
		return "discoverable_alchemist_house";
		break;
	case 2942139109:
		return "discoverable_easel";
		break;
	case 946458418:
		return "discoverable_generic";
		break;
	case 4138692981:
		return "discoverable_generic_carriable";
		break;
	case 4127235308:
		return "discoverable_generic_corpse";
		break;
	case 1407685275:
		return "discoverable_generic_location";
		break;
	case 1983556422:
		return "discoverable_generic_ped";
		break;
	case 4153374402:
		return "discoverable_geyser";
		break;
	case 2772434035:
		return "discoverable_ghost_train";
		break;
	case 267992491:
		return "discoverable_hermit_woman";
		break;
	case 829888883:
		return "discoverable_indian_burial";
		break;
	case 1652746163:
		return "discoverable_lightning_trees";
		break;
	case 2986229387:
		return "discoverable_meteor_shower";
		break;
	case 1012821399:
		return "discoverable_tree_house";
		break;
	case 656938100:
		return "discoverable_tree_trunk_boat";
		break;
	case 920407580:
		return "disco_letter";
		break;
	case 3925461521:
		return "doc_book";
		break;
	case 3590745592:
		return "doc_coach_robbery_note";
		break;
	case 2006217472:
		return "doc_newspaper";
		break;
	case 130487986:
		return "doc_player_journal";
		break;
	case 718126496:
		return "dominoesintro_invite";
		break;
	case 3639010207:
		return "dominoes_launch_sp";
		break;
	case 639632500:
		return "dominoes_sp";
		break;
	case 2242155126:
		return "downsranch";
		break;
	case 2552561535:
		return "dreamanim";
		break;
	case 1654813563:
		return "dynamic_craft_scenario";
		break;
	case 1974284623:
		return "eating_scenario";
		break;
	case 3835989923:
		return "electric_lantern";
		break;
	case 669371062:
		return "emeraldranch";
		break;
	case 3533517186:
		return "es_activity_building";
		break;
	case 1966390672:
		return "es_clean_stalls";
		break;
	case 3457013082:
		return "es_clean_stalls_launch";
		break;
	case 420381058:
		return "event_area_appleseed_stg1";
		break;
	case 218491249:
		return "event_area_appleseed_stg2";
		break;
	case 4086216323:
		return "event_area_appleseed_stg3";
		break;
	case 3781366316:
		return "event_area_appleseed_stg4";
		break;
	case 1641452309:
		return "event_area_appleseed_stg5";
		break;
	case 1197386860:
		return "event_area_castorspond";
		break;
	case 900795163:
		return "event_area_centralunion_stg1";
		break;
	case 58173097:
		return "event_area_centralunion_stg2";
		break;
	case 289489468:
		return "event_area_centralunion_stg3";
		break;
	case 1125413177:
		return "event_area_ea_launcher";
		break;
	case 638006361:
		return "event_area_fortriggsholdingcamp";
		break;
	case 1441248259:
		return "fast_travel_prototype";
		break;
	case 31007510:
		return "feud1";
		break;
	case 3383434281:
		return "feud1_intro";
		break;
	case 608937410:
		return "feud1_outro";
		break;
	case 3162205847:
		return "fillet_launch_sp";
		break;
	case 504839342:
		return "fillet_sp";
		break;
	case 3749469340:
		return "finale1";
		break;
	case 878578639:
		return "finale1_intro";
		break;
	case 3928388080:
		return "finale2";
		break;
	case 3266579160:
		return "finale2_intro";
		break;
	case 2495498013:
		return "finale3";
		break;
	case 1751014822:
		return "finale3_outro";
		break;
	case 3659555827:
		return "fishing01_invite";
		break;
	case 3866869219:
		return "fishing02_invite";
		break;
	case 3367282631:
		return "fishing03_invite";
		break;
	case 3788542280:
		return "fishing_core";
		break;
	case 1137994799:
		return "fivefingerfilletintro_invite";
		break;
	case 2746890662:
		return "fivefingerfillet_02_invite";
		break;
	case 2631227160:
		return "flow_controller";
		break;
	case 70064092:
		return "focus_ped_cam";
		break;
	case 3437709948:
		return "focus_ped_cycle";
		break;
	case 2181091102:
		return "focus_ped_walk";
		break;
	case 2003889757:
		return "footlocker_scenario";
		break;
	case 1504058959:
		return "freqsv";
		break;
	case 90061644:
		return "frozenexplorers";
		break;
	case 3454034239:
		return "frozen_to_death_scenario";
		break;
	case 3748199224:
		return "fundraiser_inspect";
		break;
	case 2408342772:
		return "fussar1";
		break;
	case 3765211036:
		return "fussar1_intro";
		break;
	case 4031073179:
		return "fussar1_outro";
		break;
	case 2169391224:
		return "fussar2";
		break;
	case 4197265778:
		return "fussar2_intro";
		break;
	case 1949644879:
		return "fussar2_outro";
		break;
	case 1213668958:
		return "gang1";
		break;
	case 2142564350:
		return "gang1_intro";
		break;
	case 832253123:
		return "gang1_outro";
		break;
	case 1989016267:
		return "gang2";
		break;
	case 3563191418:
		return "gang2_intro";
		break;
	case 748775183:
		return "gang3";
		break;
	case 4249419994:
		return "gang3_intro";
		break;
	case 2990475521:
		return "gang_enc_bandito_shack";
		break;
	case 1221573498:
		return "gang_enc_band_shack_launch";
		break;
	case 3241213711:
		return "gang_enc_lemoyne_church";
		break;
	case 4055112334:
		return "gaptoothridge_population";
		break;
	case 2114735110:
		return "gator_eggs";
		break;
	case 946994853:
		return "generic_activity_invite";
		break;
	case 1342765494:
		return "generic_alcohol_item";
		break;
	case 2481841928:
		return "generic_companion_activity";
		break;
	case 531652048:
		return "generic_cutscene";
		break;
	case 195871429:
		return "generic_document_inspection";
		break;
	case 4159134554:
		return "generic_ghosttown";
		break;
	case 457323884:
		return "generic_item_writing_on_back";
		break;
	case 2938224314:
		return "generic_mission";
		break;
	case 2742100405:
		return "generic_multibite_item";
		break;
	case 2850982404:
		return "generic_puking_item";
		break;
	case 1710557173:
		return "generic_shop_ui_events";
		break;
	case 3191509588:
		return "generic_show_mc";
		break;
	case 2578109192:
		return "generic_single_use_item";
		break;
	case 1261903608:
		return "generic_smoking_item";
		break;
	case 717558736:
		return "generic_wagon_fence_core";
		break;
	case 1077801405:
		return "generic_weapon_item";
		break;
	case 2534746570:
		return "give_all_items";
		break;
	case 2276227229:
		return "grangershoggery";
		break;
	case 3777540429:
		return "grays1";
		break;
	case 3074032537:
		return "grays1_intro";
		break;
	case 3486129043:
		return "grays1_outro";
		break;
	case 4084094424:
		return "grays2";
		break;
	case 782334331:
		return "grays2_intro";
		break;
	case 329925958:
		return "grays2_outro";
		break;
	case 115178678:
		return "grays3";
		break;
	case 770938942:
		return "grays3_intro";
		break;
	case 2167968458:
		return "greatplains_population";
		break;
	case 3206824558:
		return "grizzlieseast_population";
		break;
	case 3435456548:
		return "grizzlieswest_population";
		break;
	case 2648452607:
		return "grizzlies_abandon";
		break;
	case 1726874869:
		return "grizzlies_population";
		break;
	case 27879759:
		return "guama1";
		break;
	case 3479110843:
		return "guama2";
		break;
	case 1306155628:
		return "guama2_intro";
		break;
	case 2787390022:
		return "guama3";
		break;
	case 2638935485:
		return "guama_population";
		break;
	case 1333023256:
		return "guarma3_intro";
		break;
	case 984790457:
		return "gunfight";
		break;
	case 1952679621:
		return "handcart_test";
		break;
	case 178206986:
		return "hanging_carcass";
		break;
	case 454916132:
		return "happyfamily";
		break;
	case 530799402:
		return "heartland_population";
		break;
	case 4167654357:
		return "hennigansstead_population";
		break;
	case 3863230897:
		return "herb";
		break;
	case 1073830104:
		return "herbal_plants";
		break;
	case 2056650000:
		return "herb_acunas_star_orchid";
		break;
	case 3362092179:
		return "herb_alaskan_ginseng";
		break;
	case 1660962977:
		return "herb_american_ginseng";
		break;
	case 4163557846:
		return "herb_bay_bolete";
		break;
	case 2807439780:
		return "herb_black_berry";
		break;
	case 561629573:
		return "herb_black_currant";
		break;
	case 1829679323:
		return "herb_burdock_root";
		break;
	case 918832610:
		return "herb_burdock_root_single";
		break;
	case 3676349502:
		return "herb_chanterelles";
		break;
	case 3099612658:
		return "herb_cigar_orchid";
		break;
	case 1077596967:
		return "herb_clamshell_orchid";
		break;
	case 653458791:
		return "herb_common_bulrush";
		break;
	case 437319607:
		return "herb_creeping_thyme";
		break;
	case 2775479221:
		return "herb_desert_sage";
		break;
	case 2149767448:
		return "herb_dragons_mouth_orchid";
		break;
	case 1014445254:
		return "herb_english_mace";
		break;
	case 2047656648:
		return "herb_english_mace_single";
		break;
	case 224814421:
		return "herb_evergreen_huckleberry";
		break;
	case 752824478:
		return "herb_ghost_orchid";
		break;
	case 1012187868:
		return "herb_golden_currant";
		break;
	case 3254260805:
		return "herb_hummingbird_sage";
		break;
	case 2384339950:
		return "herb_indian_tobacco";
		break;
	case 2368419866:
		return "herb_item";
		break;
	case 3475453727:
		return "herb_lady_of_night_orchid";
		break;
	case 1906968775:
		return "herb_lady_slipper_orchid";
		break;
	case 2482479703:
		return "herb_milkweed";
		break;
	case 635871552:
		return "herb_milkweed_single";
		break;
	case 4159532633:
		return "herb_moccasin_flower_orchid";
		break;
	case 920123680:
		return "herb_night_scented_orchid";
		break;
	case 1983971282:
		return "herb_oleander_sage";
		break;
	case 1644150238:
		return "herb_oleander_sage_single";
		break;
	case 1561519041:
		return "herb_oregano";
		break;
	case 3314470443:
		return "herb_parasol_mushroom";
		break;
	case 2556080170:
		return "herb_prairie_poppy";
		break;
	case 150470908:
		return "herb_queens_orchid";
		break;
	case 424771379:
		return "herb_rams_head";
		break;
	case 2629842039:
		return "herb_rat_tail_orchid";
		break;
	case 787316814:
		return "herb_red_raspberry";
		break;
	case 984616481:
		return "herb_red_sage";
		break;
	case 1587928307:
		return "herb_sparrows_egg_orchid";
		break;
	case 2556041874:
		return "herb_spider_orchid";
		break;
	case 1078285403:
		return "herb_vanilla_flower";
		break;
	case 4032070289:
		return "herb_violet_snowdrop";
		break;
	case 2567264765:
		return "herb_wild_carrots";
		break;
	case 1783324404:
		return "herb_wild_feverfew";
		break;
	case 1288848815:
		return "herb_wild_mint";
		break;
	case 918090738:
		return "herb_wintergreen_berry";
		break;
	case 170272501:
		return "herb_yarrow";
		break;
	case 3199924141:
		return "herb_yarrow_single";
		break;
	case 3849724361:
		return "hideout_beaverhollow";
		break;
	case 1096288512:
		return "hideout_fortmercer";
		break;
	case 3256444424:
		return "hideout_gaptoothbreach";
		break;
	case 3932065469:
		return "hideout_hangingdogranch";
		break;
	case 976745472:
		return "hideout_hangingdogranch_es";
		break;
	case 820132199:
		return "hideout_shadybelle";
		break;
	case 2608445087:
		return "hideout_sixpointcabin";
		break;
	case 586845900:
		return "hideout_smugglerdocks";
		break;
	case 2848021237:
		return "hideout_solomonsfolly";
		break;
	case 2315249294:
		return "hideout_thieveslanding";
		break;
	case 453426889:
		return "hideout_twinrocks";
		break;
	case 1895447864:
		return "homeinvasion";
		break;
	case 928883695:
		return "homerobjavier_invite";
		break;
	case 460889840:
		return "horseshoeoverlook";
		break;
	case 3408258110:
		return "hunting02_invite";
		break;
	case 969023612:
		return "hunting06_invite";
		break;
	case 1138230654:
		return "hunting1";
		break;
	case 419704111:
		return "hunting1_intro";
		break;
	case 3566196994:
		return "hunting_activity";
		break;
	case 3960666836:
		return "hunting_zone_bear_legendary";
		break;
	case 4161713517:
		return "hunting_zone_beaver";
		break;
	case 4166792014:
		return "hunting_zone_big_horn_ram";
		break;
	case 1323449975:
		return "hunting_zone_bison";
		break;
	case 92551598:
		return "hunting_zone_boar";
		break;
	case 908124921:
		return "hunting_zone_buck";
		break;
	case 981829559:
		return "hunting_zone_buffalo_tatanka";
		break;
	case 514018669:
		return "hunting_zone_bullgator";
		break;
	case 4216346813:
		return "hunting_zone_cougar";
		break;
	case 59286922:
		return "hunting_zone_coyote";
		break;
	case 4188202391:
		return "hunting_zone_elk";
		break;
	case 3420611850:
		return "hunting_zone_fox";
		break;
	case 4057205393:
		return "hunting_zone_moose";
		break;
	case 250030526:
		return "hunting_zone_panther_legendary";
		break;
	case 4038789676:
		return "hunting_zone_pronghorn";
		break;
	case 1787744145:
		return "hunting_zone_wolf";
		break;
	case 1455337684:
		return "ilo_playground";
		break;
	case 280106501:
		return "industry1";
		break;
	case 2448040189:
		return "industry1_intro";
		break;
	case 2710190035:
		return "industry3";
		break;
	case 455366650:
		return "industry3_intro";
		break;
	case 1732169874:
		return "industry3_outro";
		break;
	case 3353098761:
		return "init_all_sp";
		break;
	case 2542999499:
		return "init_global_block_mc";
		break;
	case 2000112562:
		return "init_global_block_mc_deathmatch";
		break;
	case 834753522:
		return "init_global_block_mc_race";
		break;
	case 3104004775:
		return "init_global_block_mp";
		break;
	case 801149685:
		return "init_global_block_profiler";
		break;
	case 3980004584:
		return "init_global_block_shared";
		break;
	case 296970374:
		return "init_global_block_softsave";
		break;
	case 2188962967:
		return "init_global_block_sp";
		break;
	case 468713052:
		return "init_global_block_transition";
		break;
	case 2222521289:
		return "init_global_block_ugc";
		break;
	case 2790161247:
		return "interactive_campfire";
		break;
	case 1695076417:
		return "itemdatabase_debug";
		break;
	case 2025540285:
		return "item_bait_herbivore";
		break;
	case 1063112956:
		return "item_bait_predator";
		break;
	case 3718293962:
		return "junction_test";
		break;
	case 92447735:
		return "junction_test_startup";
		break;
	case 406627834:
		return "lagras";
		break;
	case 2135513525:
		return "laramie1";
		break;
	case 652356154:
		return "laramie1_intro";
		break;
	case 616400346:
		return "larnedsod";
		break;
	case 309412207:
		return "law_arrest";
		break;
	case 69471720:
		return "law_guama";
		break;
	case 3289422089:
		return "law_office_asb";
		break;
	case 3432957896:
		return "law_office_blw";
		break;
	case 4221555076:
		return "law_office_rho";
		break;
	case 3181795131:
		return "law_office_std";
		break;
	case 492246723:
		return "law_office_stw";
		break;
	case 2354005987:
		return "law_office_twd";
		break;
	case 2149945203:
		return "law_office_val";
		break;
	case 1967903291:
		return "layout_editor";
		break;
	case 1217016285:
		return "leadin_editor";
		break;
	case 3752353550:
		return "loanshark_catfish";
		break;
	case 2853257306:
		return "loanshark_horsechase1";
		break;
	case 2454540158:
		return "loanshark_hunter";
		break;
	case 931426067:
		return "loanshark_miner1";
		break;
	case 1864457804:
		return "loanshark_miner2";
		break;
	case 3838990580:
		return "loanshark_sellhorse1";
		break;
	case 2545364328:
		return "loanshark_soldier";
		break;
	case 3418273321:
		return "loanshark_undertaker";
		break;
	case 3507427690:
		return "loanshark_womancry";
		break;
	case 1691162093:
		return "long_update";
		break;
	case 779244858:
		return "lonniesshack";
		break;
	case 2403681057:
		return "loonycult";
		break;
	case 2570562101:
		return "lovetriangle";
		break;
	case 771700241:
		return "luckyscabin";
		break;
	case 375084842:
		return "macfarlanesranch";
		break;
	case 4090184674:
		return "macleanshouse";
		break;
	case 669725655:
		return "main";
		break;
	case 1922710136:
		return "manzanitapost";
		break;
	case 2780866186:
		return "map_app_event_handler";
		break;
	case 4046699221:
		return "marston1";
		break;
	case 2622939373:
		return "marston1_intro";
		break;
	case 3807780442:
		return "marston2";
		break;
	case 2089602745:
		return "marston2_intro";
		break;
	case 1488447571:
		return "marston2_or_4_outro";
		break;
	case 4937988:
		return "marston4";
		break;
	case 2306733649:
		return "marston4_intro";
		break;
	case 3097341158:
		return "marston5x_outro";
		break;
	case 3048017679:
		return "marston5_1";
		break;
	case 647501420:
		return "marston5_1_intro";
		break;
	case 881495232:
		return "marston5_2";
		break;
	case 1791799617:
		return "marston5_2_intro";
		break;
	case 1113139293:
		return "marston5_3";
		break;
	case 556982993:
		return "marston5_3_intro";
		break;
	case 2748129289:
		return "marston6";
		break;
	case 4072011154:
		return "marston6_intro";
		break;
	case 1472890885:
		return "marston7";
		break;
	case 3770022409:
		return "marston7_intro";
		break;
	case 3398725015:
		return "marston8";
		break;
	case 1215279194:
		return "marston8_intro";
		break;
	case 1818678037:
		return "marston8_outro";
		break;
	case 2487608125:
		return "mary1";
		break;
	case 4174244693:
		return "mary1_intro";
		break;
	case 2013539002:
		return "mary3";
		break;
	case 3103538961:
		return "mary3_intro";
		break;
	case 72273844:
		return "medium_update";
		break;
	case 1833753578:
		return "metaped_clothing_gender_swap";
		break;
	case 1845967105:
		return "metaped_clothing_tool";
		break;
	case 1067632799:
		return "metaped_clothing_tool_outfit_testing";
		break;
	case 2616109996:
		return "metaped_clothing_tool_quick_menu";
		break;
	case 7930962:
		return "metaped_editor";
		break;
	case 1221273613:
		return "milking_cow";
		break;
	case 2619404053:
		return "milking_cow_launch";
		break;
	case 2913005851:
		return "millesaniclaim";
		break;
	case 2127340491:
		return "missinghusband";
		break;
	case 2655662309:
		return "mission_tester";
		break;
	case 2466493671:
		return "mob1";
		break;
	case 4064020004:
		return "mob1_intro";
		break;
	case 161135796:
		return "mob1_outro";
		break;
	case 2764167267:
		return "mob2";
		break;
	case 857750679:
		return "mob2_intro";
		break;
	case 3033430140:
		return "mob3";
		break;
	case 2625217866:
		return "mob3_intro";
		break;
	case 4290296818:
		return "mob3_outro";
		break;
	case 3129312234:
		return "mob4";
		break;
	case 467264317:
		return "mob4_intro";
		break;
	case 3254993527:
		return "mob4_outro";
		break;
	case 3427084137:
		return "mob5";
		break;
	case 828230586:
		return "mob5_intro";
		break;
	case 2132008884:
		return "money_pile";
		break;
	case 538796785:
		return "money_scenario";
		break;
	case 4060507972:
		return "mountainman";
		break;
	case 100456178:
		return "mudtown1";
		break;
	case 318789680:
		return "mudtown1_intro";
		break;
	case 4063817563:
		return "mudtown1_outro";
		break;
	case 3014898273:
		return "mudtown2";
		break;
	case 4210771957:
		return "mudtown2_intro";
		break;
	case 3666509838:
		return "mudtown3";
		break;
	case 1758784809:
		return "mudtown3b";
		break;
	case 2757035685:
		return "mudtown3b_intro";
		break;
	case 684435287:
		return "mudtown3_intro";
		break;
	case 3591351570:
		return "mudtown3_outro";
		break;
	case 407436170:
		return "mudtown4";
		break;
	case 3906075514:
		return "mudtown4_intro";
		break;
	case 1022641380:
		return "mudtown5";
		break;
	case 3983973408:
		return "mudtown5_intro";
		break;
	case 2202722803:
		return "music_timing_editor";
		break;
	case 3269768968:
		return "native1";
		break;
	case 1774798926:
		return "native1_intro";
		break;
	case 3619200677:
		return "native1_outro";
		break;
	case 2963477125:
		return "native2";
		break;
	case 3618529446:
		return "native2_intro";
		break;
	case 233199348:
		return "native2_outro";
		break;
	case 2377960633:
		return "native3";
		break;
	case 271745275:
		return "native3_intro";
		break;
	case 1920448170:
		return "native3_outro";
		break;
	case 426714858:
		return "native_son1";
		break;
	case 1826990118:
		return "native_son1_intro";
		break;
	case 123634377:
		return "native_son2";
		break;
	case 3613248680:
		return "native_son2_intro";
		break;
	case 2099867229:
		return "native_son3";
		break;
	case 913408301:
		return "native_son3_intro";
		break;
	case 617757784:
		return "native_son3_outro";
		break;
	case 1065218200:
		return "net_ambient_chest";
		break;
	case 2999748244:
		return "nm_test";
		break;
	case 2016724067:
		return "odriscolls1";
		break;
	case 380959364:
		return "odriscolls1_intro";
		break;
	case 2597400072:
		return "odriscolls1_outro";
		break;
	case 3568631138:
		return "odriscolls3";
		break;
	case 3183093413:
		return "odriscolls3_intro";
		break;
	case 3502559158:
		return "odriscolls3_outro";
		break;
	case 2806227584:
		return "odriscolls4";
		break;
	case 451627726:
		return "odriscolls4_intro";
		break;
	case 1094407797:
		return "odriscolls5";
		break;
	case 3550409244:
		return "odriscolls5_intro";
		break;
	case 2816853215:
		return "oldfortwallace";
		break;
	case 507209938:
		return "orangeplantation";
		break;
	case 3073929984:
		return "outfit_swapping";
		break;
	case 157270313:
		return "paintedsky";
		break;
	case 1392012650:
		return "pausemenu_multiplayer";
		break;
	case 29649618:
		return "pause_menu";
		break;
	case 27496334:
		return "pause_menu_replay_launcher";
		break;
	case 3587038532:
		return "ped_led_wnt_test";
		break;
	case 669480684:
		return "physics_perf_test";
		break;
	case 2445492111:
		return "placement_editor";
		break;
	case 3236211526:
		return "placement_editor_doors";
		break;
	case 3759879811:
		return "placement_editor_gang_camps";
		break;
	case 3313894640:
		return "placement_editor_peds";
		break;
	case 2521599794:
		return "placement_editor_pickups";
		break;
	case 2749202727:
		return "placement_editor_points";
		break;
	case 673923530:
		return "placement_editor_props";
		break;
	case 920252012:
		return "placement_editor_prop_sets";
		break;
	case 500965127:
		return "placement_editor_vehicles";
		break;
	case 1127240006:
		return "placement_editor_volumes";
		break;
	case 1170343158:
		return "placement_editor_weapons";
		break;
	case 954940763:
		return "player_camp";
		break;
	case 130495496:
		return "player_horse";
		break;
	case 1433015236:
		return "player_menu";
		break;
	case 2279012014:
		return "player_scenario_camera";
		break;
	case 1121120602:
		return "poker_launch_sp";
		break;
	case 19547436:
		return "poker_sp";
		break;
	case 529858287:
		return "pre_sean1_law";
		break;
	case 2124571506:
		return "prisoner";
		break;
	case 2319693464:
		return "prisoner_blw_1";
		break;
	case 2561981891:
		return "prisoner_rhd_1";
		break;
	case 3375112710:
		return "prisoner_std_1";
		break;
	case 1849780186:
		return "prisoner_str_1";
		break;
	case 2857292743:
		return "prisoner_twd_1";
		break;
	case 1118706885:
		return "prisoner_val_1";
		break;
	case 846372306:
		return "pronghornranch";
		break;
	case 1101101543:
		return "property_use_core";
		break;
	case 1026482805:
		return "prop_drop";
		break;
	case 4289511573:
		return "ransack_looting";
		break;
	case 724635720:
		return "ransack_moneybag";
		break;
	case 4245474032:
		return "rare_fish";
		break;
	case 2033046531:
		return "rathskellerfork";
		break;
	case 3978913990:
		return "rcm_abigail11";
		break;
	case 3757231929:
		return "rcm_abigail22";
		break;
	case 66783646:
		return "rcm_abigail31";
		break;
	case 641202673:
		return "rcm_beau_and_penelope10";
		break;
	case 2364327777:
		return "rcm_beau_and_penelope11";
		break;
	case 3140396004:
		return "rcm_beau_and_penelope12";
		break;
	case 1014441879:
		return "rcm_beau_and_penelope20";
		break;
	case 1230291282:
		return "rcm_beau_and_penelope21";
		break;
	case 3986645647:
		return "rcm_beechers11";
		break;
	case 2847070903:
		return "rcm_beechers12";
		break;
	case 1566126337:
		return "rcm_beechers21";
		break;
	case 843775610:
		return "rcm_bh_bandito_mine";
		break;
	case 1337333465:
		return "rcm_bh_bandito_shack";
		break;
	case 4115346489:
		return "rcm_bh_blackwater_hunt";
		break;
	case 925960418:
		return "rcm_bh_camp_return";
		break;
	case 1640155710:
		return "rcm_bh_laramie_sleeping";
		break;
	case 121868966:
		return "rcm_bh_sd_saloon";
		break;
	case 2568175421:
		return "rcm_bh_shack_escape";
		break;
	case 1557947979:
		return "rcm_bh_skinner_brother";
		break;
	case 761261961:
		return "rcm_bh_skinner_search";
		break;
	case 1973809918:
		return "rcm_bh_wife_and_lover";
		break;
	case 4149485406:
		return "rcm_bounty_duel1";
		break;
	case 982780208:
		return "rcm_bounty_exconfed1";
		break;
	case 3023134344:
		return "rcm_bounty_rancher1";
		break;
	case 2476019114:
		return "rcm_braithwaites01";
		break;
	case 3598369541:
		return "rcm_calderon1";
		break;
	case 2697975724:
		return "rcm_calderon2";
		break;
	case 3491391039:
		return "rcm_calderon21";
		break;
	case 3258731139:
		return "rcm_calderon22";
		break;
	case 2082765849:
		return "rcm_callaway1";
		break;
	case 203152483:
		return "rcm_callaway12";
		break;
	case 1315709097:
		return "rcm_callaway2";
		break;
	case 114561398:
		return "rcm_callaway3";
		break;
	case 945810450:
		return "rcm_chain_gang1";
		break;
	case 651512061:
		return "rcm_chain_gang2";
		break;
	case 3454375711:
		return "rcm_chain_gang3";
		break;
	case 4233393148:
		return "rcm_chain_gang4";
		break;
	case 2140764804:
		return "rcm_chain_gang5";
		break;
	case 1709917992:
		return "rcm_chain_gang6";
		break;
	case 355280301:
		return "rcm_chain_gang7";
		break;
	case 2011907525:
		return "rcm_coach_robbery1";
		break;
	case 1364152385:
		return "rcm_collect_cigarette_cards1";
		break;
	case 809929944:
		return "rcm_collect_dinosaur_bones11";
		break;
	case 1636396893:
		return "rcm_collect_dinosaur_bones12";
		break;
	case 746649559:
		return "rcm_collect_exotics1";
		break;
	case 1044323155:
		return "rcm_collect_exotics2";
		break;
	case 1291958488:
		return "rcm_collect_exotics3";
		break;
	case 2807164251:
		return "rcm_collect_exotics4";
		break;
	case 2483996373:
		return "rcm_collect_exotics5";
		break;
	case 3318557265:
		return "rcm_collect_exotics6";
		break;
	case 1897663855:
		return "rcm_collect_rare_fish1";
		break;
	case 2460700813:
		return "rcm_collect_rare_fish2";
		break;
	case 2345630921:
		return "rcm_collect_rock_faces1";
		break;
	case 2037372938:
		return "rcm_collect_rock_faces2";
		break;
	case 3139983768:
		return "rcm_collect_taxidermy1";
		break;
	case 3434216619:
		return "rcm_collect_taxidermy2";
		break;
	case 3386532618:
		return "rcm_crackpot1";
		break;
	case 3197652102:
		return "rcm_crackpot2";
		break;
	case 4118231619:
		return "rcm_crackpot3";
		break;
	case 2746393704:
		return "rcm_crawley1";
		break;
	case 3843371146:
		return "rcm_creole1";
		break;
	case 912897403:
		return "rcm_doctors_opinion1";
		break;
	case 4190018131:
		return "rcm_down1";
		break;
	case 2356535568:
		return "rcm_down1_2";
		break;
	case 1691906181:
		return "rcm_down2";
		break;
	case 1445057304:
		return "rcm_down3";
		break;
	case 717010902:
		return "rcm_dusters21";
		break;
	case 2841387987:
		return "rcm_dusters61";
		break;
	case 2806259619:
		return "rcm_dusters62";
		break;
	case 2160438133:
		return "rcm_dutch11";
		break;
	case 720861706:
		return "rcm_dutch21";
		break;
	case 3773627066:
		return "rcm_dutch31";
		break;
	case 2143701:
		return "rcm_edith_down21";
		break;
	case 3992785294:
		return "rcm_edith_down22";
		break;
	case 2732176723:
		return "rcm_evelyn_miller1";
		break;
	case 1226277324:
		return "rcm_evelyn_miller2";
		break;
	case 988210539:
		return "rcm_evelyn_miller3";
		break;
	case 1822247127:
		return "rcm_evelyn_miller4";
		break;
	case 1580444676:
		return "rcm_evelyn_miller5";
		break;
	case 987941121:
		return "rcm_exconfed11";
		break;
	case 1569455758:
		return "rcm_for_my_art1";
		break;
	case 667521802:
		return "rcm_for_my_art2";
		break;
	case 975124405:
		return "rcm_for_my_art3";
		break;
	case 2248593283:
		return "rcm_for_my_art4";
		break;
	case 822006242:
		return "rcm_fundraiser";
		break;
	case 2940841104:
		return "rcm_gang01";
		break;
	case 2101135479:
		return "rcm_gang02";
		break;
	case 1028569078:
		return "rcm_gunslinger1_1";
		break;
	case 731976859:
		return "rcm_gunslinger1_2";
		break;
	case 622867557:
		return "rcm_gunslinger2_1";
		break;
	case 2887976058:
		return "rcm_gunslinger3_1";
		break;
	case 2189379138:
		return "rcm_gunslinger5_1";
		break;
	case 3037788792:
		return "rcm_herbalist_camp";
		break;
	case 3436109897:
		return "rcm_here_kitty_kitty1";
		break;
	case 4040632409:
		return "rcm_here_kitty_kitty2";
		break;
	case 3804204074:
		return "rcm_here_kitty_kitty3";
		break;
	case 2892668801:
		return "rcm_here_kitty_kitty4";
		break;
	case 372667159:
		return "rcm_here_kitty_kitty5";
		break;
	case 3310275904:
		return "rcm_homerob00";
		break;
	case 473889567:
		return "rcm_homerob01";
		break;
	case 144473269:
		return "rcm_jack2";
		break;
	case 81817967:
		return "rcm_mary01";
		break;
	case 2610863853:
		return "rcm_mary02";
		break;
	case 1480986649:
		return "rcm_mary31";
		break;
	case 968849173:
		return "rcm_marybeth1";
		break;
	case 4282083264:
		return "rcm_mason1";
		break;
	case 3326965225:
		return "rcm_mason2";
		break;
	case 2726178379:
		return "rcm_mason3";
		break;
	case 3561525723:
		return "rcm_mason4";
		break;
	case 1175123294:
		return "rcm_mason5";
		break;
	case 2686337666:
		return "rcm_mob01";
		break;
	case 3597184790:
		return "rcm_mob02";
		break;
	case 682709252:
		return "rcm_monroe11";
		break;
	case 2797816506:
		return "rcm_mr_mayor1";
		break;
	case 2314408218:
		return "rcm_mr_mayor2";
		break;
	case 2619979143:
		return "rcm_mr_mayor3";
		break;
	case 1643274001:
		return "rcm_mudtown3_1";
		break;
	case 639100765:
		return "rcm_mudtown3_2";
		break;
	case 4099212248:
		return "rcm_mudtown3_3";
		break;
	case 2541590473:
		return "rcm_native1";
		break;
	case 147356253:
		return "rcm_native2";
		break;
	case 2591248818:
		return "rcm_oh_brother1";
		break;
	case 1453378054:
		return "rcm_oh_brother2";
		break;
	case 2253105507:
		return "rcm_oh_brother3";
		break;
	case 3732658078:
		return "rcm_pearson1";
		break;
	case 2852302978:
		return "rcm_pearson1_outro";
		break;
	case 1661413845:
		return "rcm_poisoned_well1";
		break;
	case 4236827858:
		return "rcm_poisoned_well2";
		break;
	case 3461382242:
		return "rcm_poisoned_well3";
		break;
	case 471342072:
		return "rcm_poisoned_well4";
		break;
	case 768458595:
		return "rcm_poisoned_well5";
		break;
	case 1734396:
		return "rcm_rains_fall1";
		break;
	case 3706664870:
		return "rcm_ride_the_lightning1";
		break;
	case 2868728771:
		return "rcm_ride_the_lightning2";
		break;
	case 3107975240:
		return "rcm_ride_the_lightning3";
		break;
	case 2272955582:
		return "rcm_ride_the_lightning4";
		break;
	case 2511612209:
		return "rcm_ride_the_lightning5";
		break;
	case 1677051317:
		return "rcm_ride_the_lightning6";
		break;
	case 1920361142:
		return "rcm_ride_the_lightning7";
		break;
	case 4223748696:
		return "rcm_sadie11";
		break;
	case 824114000:
		return "rcm_serial_killer1";
		break;
	case 1215305388:
		return "rcm_slave_catcher1";
		break;
	case 2983717242:
		return "rcm_slave_catcher2";
		break;
	case 3722488098:
		return "rcm_strauss11";
		break;
	case 3156174528:
		return "rcm_strauss21";
		break;
	case 795753704:
		return "rcm_strauss31";
		break;
	case 549232517:
		return "rcm_strauss32";
		break;
	case 2180080109:
		return "rcm_strauss33";
		break;
	case 3370202994:
		return "rcm_the_odd_fellows1";
		break;
	case 3063911151:
		return "rcm_the_odd_fellows2";
		break;
	case 2836262768:
		return "rcm_tilly1";
		break;
	case 604822537:
		return "rcm_treasure_hunter";
		break;
	case 1349696220:
		return "rcm_war_veteran1";
		break;
	case 1112940195:
		return "rcm_war_veteran2";
		break;
	case 1858631559:
		return "rcm_war_veteran3";
		break;
	case 1611880989:
		return "rcm_war_veteran4";
		break;
	case 3400524495:
		return "region_law_bloodhound";
		break;
	case 210972922:
		return "region_law_guama_fussar";
		break;
	case 851455356:
		return "region_law_patrol_creator";
		break;
	case 1710040540:
		return "region_law_rho_law_swarm";
		break;
	case 3654084408:
		return "region_law_roadblock_creator";
		break;
	case 455847360:
		return "region_law_town_mob";
		break;
	case 86120456:
		return "region_law_val_belltower_sniper";
		break;
	case 2720377168:
		return "region_law_w_eliz_pinks";
		break;
	case 3972271114:
		return "region_town_lockdown_generic";
		break;
	case 2132718013:
		return "register_core";
		break;
	case 1670888889:
		return "register_scenario";
		break;
	case 1637780761:
		return "respawn_dump_body";
		break;
	case 577559150:
		return "respawn_layout_editor";
		break;
	case 830251406:
		return "respawn_persistence";
		break;
	case 1854362007:
		return "respond_parcel";
		break;
	case 2221233657:
		return "reverend1";
		break;
	case 1330185712:
		return "reverend1_intro";
		break;
	case 911078780:
		return "reverend1_outro";
		break;
	case 3978543392:
		return "re_murder_camp_note";
		break;
	case 2046780049:
		return "rhodes";
		break;
	case 599196017:
		return "rhodes_donkeys";
		break;
	case 4262029783:
		return "ridgewoodfarm";
		break;
	case 3591969700:
		return "riggsstation";
		break;
	case 3027402228:
		return "rileyscharge";
		break;
	case 475263662:
		return "riobravo_population";
		break;
	case 1506199076:
		return "roanokeridge_population";
		break;
	case 1312956596:
		return "roanoketrapper";
		break;
	case 2994034707:
		return "rob_register";
		break;
	case 1158132544:
		return "rockyseven";
		break;
	case 2103522376:
		return "rock_carvings";
		break;
	case 3356297404:
		return "run_aa_tool";
		break;
	case 447490364:
		return "sadie2";
		break;
	case 4004447435:
		return "sadie2_intro";
		break;
	case 3974385065:
		return "sadie3";
		break;
	case 1849010924:
		return "sadie3_intro";
		break;
	case 1922488592:
		return "sadie4";
		break;
	case 4030952494:
		return "sadie4_intro";
		break;
	case 3094386765:
		return "sadie4_outro";
		break;
	case 2232253949:
		return "sadie5";
		break;
	case 1325428362:
		return "sadie5_intro";
		break;
	case 1187384473:
		return "sadie5_outro";
		break;
	case 749123173:
		return "saintdenis";
		break;
	case 2675382419:
		return "saint_denis1";
		break;
	case 677333795:
		return "saint_denis1_intro";
		break;
	case 1609893116:
		return "saloon1";
		break;
	case 1085366363:
		return "saloon1_intro";
		break;
	case 1560768681:
		return "saloon1_outro";
		break;
	case 559221705:
		return "saloon_conversations";
		break;
	case 4036079297:
		return "saloon_dining";
		break;
	case 4290909205:
		return "satchel";
		break;
	case 1349139153:
		return "satchel_ui_event_handler";
		break;
	case 2778538915:
		return "save_menu_ui_event_handler";
		break;
	case 766020954:
		return "scaleform_dynamic_items_creation_test";
		break;
	case 2594542616:
		return "scaleform_font_test";
		break;
	case 2641825981:
		return "scaleform_graphic_test";
		break;
	case 2233279923:
		return "scaleform_lobby_menu_demo";
		break;
	case 435917813:
		return "scaleform_minigame_test";
		break;
	case 3472105193:
		return "scaleform_option_stepper_test";
		break;
	case 807464192:
		return "scaleform_pause_menu_demo";
		break;
	case 2468527552:
		return "scaleform_ui_test";
		break;
	case 3077947252:
		return "scarletthorseshop";
		break;
	case 3285199170:
		return "scarlettmeadows_population";
		break;
	case 3737590761:
		return "scenario_point_finder";
		break;
	case 1263693094:
		return "scenario_vig_launcher";
		break;
	case 3502110290:
		return "sean1";
		break;
	case 2746360963:
		return "sean1_intro";
		break;
	case 1496445951:
		return "security_door_knock_scenario";
		break;
	case 506618227:
		return "security_door_peek_scenario";
		break;
	case 2552500790:
		return "serendipity";
		break;
	case 1726869400:
		return "serialkiller";
		break;
	case 125848256:
		return "sf_playground_script";
		break;
	case 1447751321:
		return "shack_beechersa1";
		break;
	case 3859455882:
		return "shack_beechersb1";
		break;
	case 257061693:
		return "shack_beechersc1";
		break;
	case 1095920653:
		return "shack_civilwarbride1";
		break;
	case 3417876607:
		return "shack_deadrival";
		break;
	case 1916201726:
		return "shack_deadsettler";
		break;
	case 3913997758:
		return "shack_frozenexplorers1";
		break;
	case 3764208019:
		return "shack_gunfight1";
		break;
	case 3951495140:
		return "shack_happyfamily1";
		break;
	case 663999405:
		return "shack_hungrybear1";
		break;
	case 4268802156:
		return "shack_item_loot_scenario";
		break;
	case 3064377635:
		return "shack_loonycult1";
		break;
	case 3932283479:
		return "shack_lovetriangle1";
		break;
	case 3577718626:
		return "shack_missinghusband1";
		break;
	case 3365197327:
		return "shack_missinghusband1_wagon";
		break;
	case 2286756346:
		return "shack_mountainman1";
		break;
	case 1262439980:
		return "shack_ontherun1";
		break;
	case 1111604511:
		return "shack_slavepen1";
		break;
	case 1583545946:
		return "shack_starvingchildren";
		break;
	case 1132796418:
		return "shack_undergroundrailroad";
		break;
	case 2415898582:
		return "shadybelle";
		break;
	case 822027777:
		return "shepherdsrise";
		break;
	case 2470657462:
		return "shocking_beats";
		break;
	case 4143175351:
		return "shooting_range";
		break;
	case 564662301:
		return "shoprobberies";
		break;
	case 1848756007:
		return "shoprobberies_dynamic";
		break;
	case 3056365129:
		return "shop_bait";
		break;
	case 1386513205:
		return "shop_barber";
		break;
	case 1816543000:
		return "shop_butcher";
		break;
	case 3683390053:
		return "shop_doctor";
		break;
	case 2433378966:
		return "shop_dynamic";
		break;
	case 1347332696:
		return "shop_fence";
		break;
	case 1649811752:
		return "shop_general";
		break;
	case 4132892594:
		return "shop_gunsmith";
		break;
	case 593152553:
		return "shop_horse_shop";
		break;
	case 1156535622:
		return "shop_horse_shop_sp";
		break;
	case 3946415572:
		return "shop_hotel";
		break;
	case 1055046396:
		return "shop_market";
		break;
	case 1453830393:
		return "shop_net_barber";
		break;
	case 2741305474:
		return "shop_newspaper_boy";
		break;
	case 2434501192:
		return "shop_pearson";
		break;
	case 3553967210:
		return "shop_photo_studio";
		break;
	case 4226059202:
		return "shop_post_office";
		break;
	case 1143796793:
		return "shop_tailor";
		break;
	case 1913125789:
		return "shop_train_station";
		break;
	case 4047262880:
		return "shop_trapper";
		break;
	case 2702957080:
		return "short_update";
		break;
	case 1904885261:
		return "show_lighting_adjustment";
		break;
	case 2513323630:
		return "show_movie_magic_lantern";
		break;
	case 3790809808:
		return "show_nbx_bigband";
		break;
	case 3927178800:
		return "show_nbx_bigband_02";
		break;
	case 2942732241:
		return "show_nbx_bulletcatch";
		break;
	case 2360104488:
		return "show_nbx_cancan";
		break;
	case 1786356849:
		return "show_nbx_cancan_02";
		break;
	case 955777573:
		return "show_nbx_escapeartist";
		break;
	case 3657309634:
		return "show_nbx_escapenoose";
		break;
	case 1940040968:
		return "show_nbx_firebreath_01";
		break;
	case 2218445324:
		return "show_nbx_firedance_01";
		break;
	case 2534698943:
		return "show_nbx_firedance_02";
		break;
	case 3579246828:
		return "show_nbx_flexfight";
		break;
	case 2435016617:
		return "show_nbx_oddfellows";
		break;
	case 3904747912:
		return "show_nbx_snakedance_01";
		break;
	case 4202781967:
		return "show_nbx_snakedance_02";
		break;
	case 440814870:
		return "show_nbx_strongwoman";
		break;
	case 1566700255:
		return "show_nbx_sworddance_01";
		break;
	case 3686409115:
		return "simple_crafting";
		break;
	case 665326394:
		return "singing";
		break;
	case 1319108221:
		return "sisikapenitentiary";
		break;
	case 3731218039:
		return "slavepen";
		break;
	case 4103182118:
		return "sleeping_scenario";
		break;
	case 2663352134:
		return "smoketest";
		break;
	case 1791992046:
		return "smoketest_budget";
		break;
	case 962686565:
		return "smoketest_second_screen";
		break;
	case 2981576005:
		return "smoketest_stability";
		break;
	case 2324248348:
		return "smoketest_wait";
		break;
	case 4012757236:
		return "smuggler2";
		break;
	case 3084123387:
		return "smuggler2_intro";
		break;
	case 3772237380:
		return "smuggler2_outro";
		break;
	case 670028154:
		return "spd_agnesdowd1";
		break;
	case 3714502181:
		return "spd_andershelgerson";
		break;
	case 2239597637:
		return "spd_armadillotowncrier";
		break;
	case 2170863364:
		return "spd_blandpreacher";
		break;
	case 233841855:
		return "spd_blindmancassidy1";
		break;
	case 309319785:
		return "spd_blindmancassidy_horse_buck";
		break;
	case 251496891:
		return "spd_bummicky1";
		break;
	case 2399406530:
		return "spd_bummicky2";
		break;
	case 3234065729:
		return "spd_bummicky3";
		break;
	case 2856435773:
		return "spd_bummicky4";
		break;
	case 1476860877:
		return "spd_bummicky5";
		break;
	case 3380281127:
		return "spd_bummicky6";
		break;
	case 3034404332:
		return "spd_bummicky7";
		break;
	case 1956326956:
		return "spd_chelonianmaster1";
		break;
	case 888720469:
		return "spd_civilwarcommando";
		break;
	case 4222156850:
		return "spd_crackpotrobot";
		break;
	case 1593824864:
		return "spd_dorotheawicklow1";
		break;
	case 1833333485:
		return "spd_dorotheawicklow2";
		break;
	case 864425171:
		return "spd_endlessstrainer1";
		break;
	case 3117978513:
		return "spd_eugenicsproponent";
		break;
	case 3694234520:
		return "spd_gavin1";
		break;
	case 225209869:
		return "spd_gavin2";
		break;
	case 1307760222:
		return "spd_giant";
		break;
	case 838171671:
		return "spd_giant_birds";
		break;
	case 776118277:
		return "spd_grizzledjon1";
		break;
	case 3420282319:
		return "spd_joebutler1";
		break;
	case 3650550082:
		return "spd_joebutler2";
		break;
	case 1739658616:
		return "spd_joebutler3";
		break;
	case 1969729765:
		return "spd_joebutler4";
		break;
	case 1505696224:
		return "spd_johnmadman";
		break;
	case 3730629629:
		return "spd_lillianpowell1";
		break;
	case 3549351521:
		return "spd_lillianpowell2";
		break;
	case 2330836264:
		return "spd_lillianpowell3";
		break;
	case 2032933285:
		return "spd_lillianpowell4";
		break;
	case 2905321749:
		return "spd_madscientist";
		break;
	case 3359692646:
		return "spd_mayorofstrawberry1";
		break;
	case 3730443122:
		return "spd_philosopher1";
		break;
	case 3399152738:
		return "spd_poorjoe1";
		break;
	case 1119049930:
		return "spd_sheriffoftumbleweed";
		break;
	case 2577571616:
		return "spd_soothsayer1";
		break;
	case 3860982649:
		return "spd_sunworshipper1";
		break;
	case 1331348664:
		return "spd_swampweirdo1";
		break;
	case 4125200097:
		return "spd_thomasdowne1";
		break;
	case 4164157703:
		return "spd_timothydonahue1";
		break;
	case 4015583057:
		return "spd_timothydonahue2";
		break;
	case 118155328:
		return "spd_tinyhermit1";
		break;
	case 3643765838:
		return "spd_vampire";
		break;
	case 1289699235:
		return "stage_coach";
		break;
	case 1104607124:
		return "startup";
		break;
	case 4291947009:
		return "startup_clip";
		break;
	case 1038823813:
		return "startup_mp";
		break;
	case 3008760941:
		return "startup_sp";
		break;
	case 3044421150:
		return "starvingchildren";
		break;
	case 2553435103:
		return "stew_eating";
		break;
	case 427683330:
		return "strawberry";
		break;
	case 3307427772:
		return "susan1";
		break;
	case 1306136784:
		return "susan1_intro";
		break;
	case 894179146:
		return "swansonstation";
		break;
	case 2801675202:
		return "talltreestrapper";
		break;
	case 2261938642:
		return "talltrees_population";
		break;
	case 678508515:
		return "taxidermy";
		break;
	case 3666190014:
		return "taxidermy_marital";
		break;
	case 173394207:
		return "test_startup";
		break;
	case 200318414:
		return "test_startup2";
		break;
	case 3465162524:
		return "theatre_ticket_taker";
		break;
	case 1846381745:
		return "thieveslanding";
		break;
	case 4118136820:
		return "tithe_scenario";
		break;
	case 3406766065:
		return "town_secrets_er_daughter";
		break;
	case 4139443687:
		return "town_secrets_sd_trelawny";
		break;
	case 2189300677:
		return "town_secrets_strawberry";
		break;
	case 2577036050:
		return "town_secrets_val_moira";
		break;
	case 3041226367:
		return "trainrobbery_ambient";
		break;
	case 865478871:
		return "train_annesburg";
		break;
	case 1527681014:
		return "train_fast_travel_core";
		break;
	case 1020047063:
		return "train_robbery1";
		break;
	case 1046165797:
		return "train_robbery1_intro";
		break;
	case 2685695333:
		return "train_robbery2";
		break;
	case 1017561833:
		return "train_robbery2_intro";
		break;
	case 1915427219:
		return "train_robbery3";
		break;
	case 1216390792:
		return "train_robbery3_intro";
		break;
	case 2206252094:
		return "train_robbery4";
		break;
	case 259307343:
		return "train_robbery4_intro";
		break;
	case 1015315623:
		return "train_robbery4_outro";
		break;
	case 1337352009:
		return "train_summon";
		break;
	case 2054187315:
		return "train_vfx_test";
		break;
	case 1765845133:
		return "treasure_hunter";
		break;
	case 3566390819:
		return "trelawny1";
		break;
	case 1863896574:
		return "trelawny1_intro";
		break;
	case 2770008149:
		return "tumbleweed";
		break;
	case 1682029110:
		return "ui_sticky_feed_priority_test";
		break;
	case 3694850185:
		return "ui_unit_tests";
		break;
	case 4237766670:
		return "undertaker";
		break;
	case 534575292:
		return "utopia1";
		break;
	case 3095579597:
		return "utopia1_intro";
		break;
	case 238015842:
		return "utopia2";
		break;
	case 1957400601:
		return "utopia2_intro";
		break;
	case 3715246897:
		return "utopia2_outro";
		break;
	case 459833523:
		return "valentine";
		break;
	case 1639472059:
		return "vanhornmansion";
		break;
	case 212549755:
		return "vanhorntradingpost";
		break;
	case 1630596328:
		return "vig_debug_ui";
		break;
	case 3981869187:
		return "vig_playlist_colter";
		break;
	case 635369197:
		return "vig_playlist_dtc1_post";
		break;
	case 3553836388:
		return "vig_playlist_dtc3_post";
		break;
	case 156012081:
		return "vig_playlist_fud1_post";
		break;
	case 2720529259:
		return "vig_playlist_gang3_post";
		break;
	case 4007234598:
		return "vig_playlist_hosea_focus";
		break;
	case 1512407123:
		return "vig_playlist_lowkey_hso";
		break;
	case 2727534338:
		return "vig_playlist_mud1_post";
		break;
	case 410549804:
		return "vig_playlist_mud5_post";
		break;
	case 1709413547:
		return "vig_playlist_music_pns";
		break;
	case 2271485372:
		return "vig_playlist_music_snknj";
		break;
	case 1423621071:
		return "vig_playlist_party_hso";
		break;
	case 601461788:
		return "vig_playlist_party_sdb";
		break;
	case 825903191:
		return "vig_playlist_sad1_post";
		break;
	case 3231837483:
		return "vig_playlist_strauss31_post";
		break;
	case 1978046393:
		return "virtualization_unit_test_support";
		break;
	case 896699223:
		return "walk_and_talk_test_script_widget";
		break;
	case 3897852604:
		return "wallacestation";
		break;
	case 2818729232:
		return "wanted_poster";
		break;
	case 1663398575:
		return "wapiti";
		break;
	case 2447121312:
		return "watsonscabin";
		break;
	case 2119362698:
		return "wilderness_chest";
		break;
	case 3815061423:
		return "window_peek_scenario";
		break;
	case 235626141:
		return "winter1";
		break;
	case 219008523:
		return "winter1_outro";
		break;
	case 1518204801:
		return "winter2";
		break;
	case 3998219937:
		return "winter2_intro";
		break;
	case 3860140116:
		return "winter2_outro";
		break;
	case 1695386784:
		return "winter4";
		break;
	case 2873669358:
		return "winter4_intro";
		break;
	case 2902716849:
		return "winter4_outro";
		break;
	case 374246894:
		return "xml_menus";
		break;
	default:
		return "invalid";
		break;
	}
}
inline constexpr std::string AmmoHashToString(Hash AmmoHash)
{
	switch (AmmoHash)
	{
	case 0x7DF4D025:
		return "AMMO_22";
		break;
	case 0x8E919F27:
		return "AMMO_22_TRANQUILIZER";
		break;
	case 0x38E6F55F:
		return "AMMO_ARROW";
		break;
	case 0x1F901FAE:
		return "AMMO_ARROW_CONFUSION";
		break;
	case 0xDC6FE2FE:
		return "AMMO_ARROW_DISORIENT";
		break;
	case 0xA3B9DB42:
		return "AMMO_ARROW_DRAIN";
		break;
	case 0xC1F57A79:
		return "AMMO_ARROW_DYNAMITE";
		break;
	case 0x11B25B49:
		return "AMMO_ARROW_FIRE";
		break;
	case 0x9238061F:
		return "AMMO_ARROW_IMPROVED";
		break;
	case 0x07865A92:
		return "AMMO_ARROW_POISON";
		break;
	case 0xAE6E2B0E:
		return "AMMO_ARROW_SMALL_GAME";
		break;
	case 0x62CEC038:
		return "AMMO_ARROW_TRACKING";
		break;
	case 0xF680010B:
		return "AMMO_ARROW_TRAIL";
		break;
	case 0xB6731F5A:
		return "AMMO_ARROW_WOUND";
		break;
	case 0x020C7A4A:
		return "AMMO_BOLAS";
		break;
	case 0x22E119A9:
		return "AMMO_BOLAS_HAWKMOTH";
		break;
	case 0x9AB3E5C1:
		return "AMMO_BOLAS_INTERTWINED";
		break;
	case 0x87BA17E6:
		return "AMMO_BOLAS_IRONSPIKED";
		break;
	case 0xC4564CBF:
		return "AMMO_BULLET_EXPLOSIVE";
		break;
	case 0xBF3C391A:
		return "AMMO_BULLET_EXPRESS";
		break;
	case 0xEDE7E77B:
		return "AMMO_BULLET_HIGH_VELOCITY";
		break;
	case 0xFB1A7424:
		return "AMMO_BULLET_NORMAL";
		break;
	case 0xA51266E6:
		return "AMMO_BULLET_SPLIT_POINT";
		break;
	case 0xB6976AA1:
		return "AMMO_CANNON";
		break;
	case 0x1C9D6E9D:
		return "AMMO_DYNAMITE";
		break;
	case 0xCAE5CECB:
		return "AMMO_DYNAMITE_NORMAL";
		break;
	case 0x321BA159:
		return "AMMO_DYNAMITE_VOLATILE";
		break;
	case 0x3FCF790D:
		return "AMMO_FIRE_BOTTLE_NORMAL";
		break;
	case 0x52E776C3:
		return "AMMO_FIRE_BOTTLE_VOLATILE";
		break;
	case 0x194631D6:
		return "AMMO_HATCHET";
		break;
	case 0xA9708E57:
		return "AMMO_HATCHET_ANCIENT";
		break;
	case 0xB925EC32:
		return "AMMO_HATCHET_CLEAVER";
		break;
	case 0x63A5047F:
		return "AMMO_HATCHET_DOUBLE_BIT";
		break;
	case 0xCABE0C0F:
		return "AMMO_HATCHET_DOUBLE_BIT_RUSTED";
		break;
	case 0x8507C1F7:
		return "AMMO_HATCHET_HEWING";
		break;
	case 0x1AA32EB0:
		return "AMMO_HATCHET_HUNTER";
		break;
	case 0xBEDC8EB6:
		return "AMMO_HATCHET_HUNTER_RUSTED";
		break;
	case 0xE501537B:
		return "AMMO_HATCHET_VIKING";
		break;
	case 0xEAD00129:
		return "AMMO_LASSO";
		break;
	case 0x5633F9D5:
		return "AMMO_MOLOTOV";
		break;
	case 0x886C55D7:
		return "AMMO_MOLOTOV_VOLATILE";
		break;
	case 0x631C84FC:
		return "AMMO_MOONSHINEJUG";
		break;
	case 0x656A2F3B:
		return "AMMO_MOONSHINEJUG_MP";
		break;
	case 0x743D4F54:
		return "AMMO_PISTOL";
		break;
	case 0x31E2AD5B:
		return "AMMO_PISTOL_EXPRESS";
		break;
	case 0x46A648C2:
		return "AMMO_PISTOL_EXPRESS_EXPLOSIVE";
		break;
	case 0xABD96830:
		return "AMMO_PISTOL_HIGH_VELOCITY";
		break;
	case 0x0E163B80:
		return "AMMO_PISTOL_SPLIT_POINT";
		break;
	case 0x39714C4F:
		return "AMMO_POISONBOTTLE";
		break;
	case 0xB0B80B9A:
		return "AMMO_REPEATER";
		break;
	case 0xDD871DC8:
		return "AMMO_REPEATER_EXPRESS";
		break;
	case 0x9C8B6796:
		return "AMMO_REPEATER_EXPRESS_EXPLOSIVE";
		break;
	case 0x0DCBE210:
		return "AMMO_REPEATER_HIGH_VELOCITY";
		break;
	case 0x44750C88:
		return "AMMO_REPEATER_SPLIT_POINT";
		break;
	case 0x64356159:
		return "AMMO_REVOLVER";
		break;
	case 0x4970588D:
		return "AMMO_REVOLVER_EXPRESS";
		break;
	case 0x04A8EFBB:
		return "AMMO_REVOLVER_EXPRESS_EXPLOSIVE";
		break;
	case 0x83C5E860:
		return "AMMO_REVOLVER_HIGH_VELOCITY";
		break;
	case 0x4A25B008:
		return "AMMO_REVOLVER_SPLIT_POINT";
		break;
	case 0x0D05319F:
		return "AMMO_RIFLE";
		break;
	case 0xB392591E:
		return "AMMO_RIFLE_ELEPHANT";
		break;
	case 0x62A11A4B:
		return "AMMO_RIFLE_EXPRESS";
		break;
	case 0x6D926443:
		return "AMMO_RIFLE_EXPRESS_EXPLOSIVE";
		break;
	case 0x6ECB67F9:
		return "AMMO_RIFLE_HIGH_VELOCITY";
		break;
	case 0x0BEFA5B2:
		return "AMMO_RIFLE_SPLIT_POINT";
		break;
	case 0x90083D3B:
		return "AMMO_SHOTGUN";
		break;
	case 0xBFCB2621:
		return "AMMO_SHOTGUN_BUCKSHOT_INCENDIARY";
		break;
	case 0x12C60041:
		return "AMMO_SHOTGUN_SLUG";
		break;
	case 0x2314B32A:
		return "AMMO_SHOTGUN_SLUG_EXPLOSIVE";
		break;
	case 0x9E4AD291:
		return "AMMO_THROWING_KNIVES";
		break;
	case 0x9117CF91:
		return "AMMO_THROWING_KNIVES_CONFUSE";
		break;
	case 0x59DCB686:
		return "AMMO_THROWING_KNIVES_DISORIENT";
		break;
	case 0x6D0020AB:
		return "AMMO_THROWING_KNIVES_DRAIN";
		break;
	case 0x48DC05F6:
		return "AMMO_THROWING_KNIVES_IMPROVED";
		break;
	case 0xF51D1AC7:
		return "AMMO_THROWING_KNIVES_JAVIER";
		break;
	case 0xD8F38BBD:
		return "AMMO_THROWING_KNIVES_NORMAL";
		break;
	case 0x7BA5E56E:
		return "AMMO_THROWING_KNIVES_POISON";
		break;
	case 0x4BC1020F:
		return "AMMO_THROWING_KNIVES_TRAIL";
		break;
	case 0x9143D131:
		return "AMMO_THROWING_KNIVES_WOUND";
		break;
	case 0xCE156C30:
		return "AMMO_THROWN_ITEM";
		break;
	case 0x49A985D7:
		return "AMMO_TOMAHAWK";
		break;
	case 0xF25D45BC:
		return "AMMO_TOMAHAWK_ANCIENT";
		break;
	case 0xABD7C401:
		return "AMMO_TOMAHAWK_HOMING";
		break;
	case 0xCE489834:
		return "AMMO_TOMAHAWK_IMPROVED";
		break;
	case 0xBA2D509B:
		return "AMMO_TURRET";
		break;
	}
}
inline constexpr std::string SlotIdHashToString(Hash SlotId) {
	switch (SlotId) {
	case 1084182731:
		return "SLOTID_SATCHEL";
	case 1782082944:
		return "SLOTID_ACTIVE_HORSE";
	case 2983264686:
		return "SLOTID_UPGRADE";
	case 1034665895:
		return "SLOTID_WARDROBE";
	default:
		return std::to_string(SlotId);
		break;
	}
}