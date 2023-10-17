#pragma once
#include <RE/A/ActorState.h>
#include <RE/F/FormTypes.h>
#include <RE/N/NiPoint3.h>
#include <string>
#include <unordered_set>

[[nodiscard]] static inline bool CanDisplay(const RE::TESBoundObject& a_object)
{
	switch (a_object.GetFormType()) {
	case RE::FormType::Scroll:
	case RE::FormType::Armor:
	case RE::FormType::Book:
	case RE::FormType::Ingredient:
	case RE::FormType::Misc:
	case RE::FormType::Weapon:
	case RE::FormType::Ammo:
	case RE::FormType::KeyMaster:
	case RE::FormType::AlchemyItem:
	case RE::FormType::Note:
	case RE::FormType::SoulGem:
		break;
	case RE::FormType::Light:
		{
			auto& light = static_cast<const RE::TESObjectLIGH&>(a_object);
			if (!light.CanBeCarried()) {
				return false;
			}
		}
		break;
	default:
		return false;
	}

	if (!a_object.GetPlayable()) {
		return false;
	}

	auto name = a_object.GetName();
	if (!name || name[0] == '\0') {
		return false;
	}

	return true;
}

bool static inline HasKeyword(RE::BGSKeywordForm* o, RE::FormID formID)
{
	bool result = false;

	if (o->keywords) {
		for (std::uint32_t idx = 0; idx < o->numKeywords; ++idx) {
			if (o->keywords[idx] && o->keywords[idx]->formID == formID) {
				result = true;
				break;
			}
		}
	}
	return result;
}

// 人物属性
struct PlayerInfo
{
	std::string name = "";         // 玩家名
	std::string ArrowDamage = "";  //弓箭伤害
	std::string DamageRight = "";  //右手伤害
	std::string DamageLeft = "";   //左手伤害
	std::string DamageStr = "";    //HUD显示内容
	//std::string DamageResist = ""; // 护甲值
	float kDamageResist = 0.0f;   // 护甲值
	float kPoisonResist = 0.0f;   // 毒抗
	float kResistFire = 0.0f;     // 火炕
	float kResistFrost = 0.0f;    // 冰抗
	float kResistShock = 0.0f;    // 雷抗
	float kResistMagic = 0.0f;    // 法抗
	float kResistDisease = 0.0f;  // 抗疾病
	RE::NiPoint3 Angle;           // 视角
	RE::NiPoint3 Position;        // 坐标

	float kOneHandedModifier = 0.0f;
	float kTwoHandedModifier = 0.0f;
	float kMarksmanModifier = 0.0f;
	float kSmithingModifier = 0.0f;
	float kSmithingPowerModifier = 0.0f;
	float kSmithingSkillAdvance = 0.0f;
	float kAlchemyModifier = 0.0f;
	float kAlchemySkillAdvance = 0.0f;
	float kAlchemyPowerModifier = 0.0f;
	float kEnchantingModifier = 0.0f;
	float kEnchantingPowerModifier = 0.0f;
	float kEnchantingSkillAdvance = 0.0f;

	float kHealth = 0.0f;
	float kMagicka = 0.0f;
	float kStamina = 0.0f;

	float kHealthBase = 0.0f;
	float kMagickaBase = 0.0f;
	float kStaminaBase = 0.0f;

	float gold = 0.0f;
	float equippedWeight = 0.0f;
	float carryWeight = 0.0f;

	std::string location = "";
	RE::FormID locationId = 0;
	std::string parentLocation = "";
	RE::FormID parentLocationId = 0;
	
};

// 装备防具信息

struct ArmorInfo
{
	//bool isShow = false;
	bool isExist = false;  // 是否装备
	bool isMainSlotAlert = false;
	bool isSpeacilSlotAlert = true;
	std::string treeId = "";  //HUD显示内容

	std::string name = "";           // 装备名称
	std::string formID = "";         // 物品ID
	std::string formTypeName = "";   // 物品类型
	std::string equipSlotName = "";  // 装备插槽部位
	std::string armorTypeName = "";  // 装备类型
	int goldValue = 0;               // 装备价格
	int value = 0;                   // 属性值
	float armorRating = 0;           // 装备等级
	float weight = 0;                // 重量
};

extern ArmorInfo wornArmos[32];

// 装备武器、弹药信息

struct WeaponInfo
{
	bool isExist = false;     // 是否装备
	std::string treeId = "";  //HUD显示内容

	std::string name = "";            // 武器名称
	std::string formID = "";          // 物品ID
	std::string formTypeName = "";    // 物品类型
	std::string weaponTypeName = "";  // 武器类型
	RE::FormType formType = RE::FormType::None;
	int goldValue = 0;   // 装备价格
	int value = 0;       // 属性值
	int critDamage = 0;  // 暴击伤害
	int damage = 0;      // 武器伤害
	float weight = 0;    // 武器重量

	// 法术
	std::string castingTypeName = "";  // 法术类型
	std::string spellTypeName = "";    // 法术类型
	float cost = 0;                    // 法术消耗
	float time = 0;                    // 施法时间
	bool isTwoHand = false;            // 是否占用双手
};

struct InventoryInfo
{
	RE::TESBoundObject* ptr = nullptr;
	RE::InventoryEntryData* invPtr = nullptr;
	RE::ExtraDataList* invExtraPtr = nullptr;
	std::string name = "";
	std::string formIdStr = "";
	RE::FormID formId = 0;
	int count = 0;
	float weight = 0;
	bool isWorn = 0;
};

struct ActorInfo
{
	RE::Actor* ptr = nullptr;

	std::string formIdStr = "";
	RE::FormID formId = 0;
	std::string name = "";         // 玩家名
	std::string ArrowDamage = "";  //弓箭伤害
	std::string DamageRight = "";  //右手伤害
	std::string DamageLeft = "";   //左手伤害
	std::string DamageStr = "";    //HUD显示内容
	//std::string DamageResist = ""; // 护甲值
	float kDamageResist = 0.0f;   // 护甲值
	float kPoisonResist = 0.0f;   // 毒抗
	float kResistFire = 0.0f;     // 火炕
	float kResistFrost = 0.0f;    // 冰抗
	float kResistShock = 0.0f;    // 雷抗
	float kResistMagic = 0.0f;    // 法抗
	float kResistDisease = 0.0f;  // 抗疾病
	//RE::NiPoint3 Angle;           // 视角
	//RE::NiPoint3 Position;        // 坐标

	//float kOneHandedModifier = 0.0f;
	//float kTwoHandedModifier = 0.0f;
	//float kMarksmanModifier = 0.0f;
	//float kSmithingModifier = 0.0f;
	//float kSmithingPowerModifier = 0.0f;
	//float kSmithingSkillAdvance = 0.0f;
	//float kAlchemyModifier = 0.0f;
	//float kAlchemySkillAdvance = 0.0f;
	//float kAlchemyPowerModifier = 0.0f;
	//float kEnchantingModifier = 0.0f;
	//float kEnchantingPowerModifier = 0.0f;
	//float kEnchantingSkillAdvance = 0.0f;

	float kHealth = 0.0f;
	float kMagicka = 0.0f;
	float kStamina = 0.0f;
	float distance = 0.0f;
	int direction = 0;

	int isSentient = 0;
	int level = 0.0f;
	bool isDead = false;
	bool isTeammate = false;
	bool idHostile = false;
	bool isInCombat = false;

	float kHealthBase = 0.0f;
	float kMagickaBase = 0.0f;
	float kStaminaBase = 0.0f;
	time_t updateTime = 0;

	RE::ACTOR_LIFE_STATE lifeState = RE::ACTOR_LIFE_STATE::kAlive;

	int inventoryCount = 0;

	InventoryInfo Inventorys[100];
};

extern bool active;
extern bool activeItems;

extern PlayerInfo playerInfo;

extern int show_npc_window_dis_meter;
extern bool show_npc_window_dis;
extern bool show_npc_window_direction;
extern bool show_npc_window_dead_hidden;
extern bool show_enemy_window;
extern bool show_inv_window;
extern bool show_npc_window;

extern WeaponInfo leftWeaponInfo;
extern WeaponInfo rightWeaponInfo;
extern WeaponInfo ammoInfo;

extern int refresh_time_data;
extern int refresh_time_auto;
extern bool startflag;

//extern bool isRefreshActorInfo;
void __cdecl RefreshGameInfo(void*);
void __cdecl RefreshActorInfo(void*);
void __cdecl RefreshItemInfo(void*);

ActorInfo* getTeammateData();
ActorInfo* getNpcData();
ActorInfo* getHorseData();
ActorInfo* getEnemy2Data();
int getTeammateCount();
int getNpcCount();
int getHorseCount();
int getEnemyCount();

int getPlayerInvCount();
int getPlayerInvBOOKCount();
int getPlayerInvWEAPCount();
int getPlayerInvARMOCount();
int getPlayerInvAMMOCount();
int getPlayerInvALCHCount();
int getPlayerInvFOODCount();
int getPlayerInvINGRCount();
int getPlayerGoldCount();

InventoryInfo* getPlayerInvData();
InventoryInfo* getPlayerInvARMOData();
InventoryInfo* getPlayerInvBOOKData();
InventoryInfo* getPlayerInvWEAPData();
InventoryInfo* getPlayerInvAMMOData();
InventoryInfo* getPlayerInvALCHData();
InventoryInfo* getPlayerInvFOODData();
InventoryInfo* getPlayerInvINGRData();
InventoryInfo* getPlayerInvData(int);

struct Actor2Info
{
	int npcCount = 0;
	int enemyCount = 0;
	int teammateCount = 0;
	int horseCount = 0;
	ActorInfo npcInfo[100];
	ActorInfo enemyInfo[100];
	ActorInfo teammateInfo[100];
	ActorInfo horseInfo[100];
};

struct PlayerInventoryInfo
{
	int inventoryARMOCount = 0;
	int inventoryWEAPCount = 0;
	int inventoryBOOKCount = 0;
	int inventoryAMMOCount = 0;
	int inventoryALCHCount = 0;
	int inventoryFOODCount = 0;
	int inventoryINGRCount = 0;
	int inventoryCount = 0;
	int gold = 0;
	InventoryInfo inventorysARMO[300];
	InventoryInfo inventorysWEAP[300];
	InventoryInfo inventorysBOOK[300];
	InventoryInfo inventorysAMMO[100];
	InventoryInfo inventorysALCH[300];
	InventoryInfo inventorysFOOD[300];
	InventoryInfo inventorysINGR[300];
	InventoryInfo inventorys[300];
};

// 临时
extern bool isGameLoading;

extern bool show_items_window;
extern bool show_items_window_settings;
extern bool show_items_window_formid;
extern bool show_items_window_refid;
extern bool show_items_window_file;
extern bool show_items_window_3D;
extern bool show_items_window_direction;
extern bool show_items_window_ignore;
extern int show_items_window_array_max_length;

// 艺术馆
extern bool show_items_window_gallery;

struct ItemInvInfo
{
	std::string name = "";
	int count;
	RE::TESBoundObject* ptr;
	RE::InventoryEntryData* ptr2;
	bool isCrime;
	bool isEnchanted;
};

struct ItemInfo
{
	int gold = 0;
	float weight = 0;
	RE::FormID formId = 0;
	std::string formIdStr = "";
	RE::FormID baseFormId = 0;
	std::string baseFormIdStr = "";
	std::string formTypeStr = "";
	std::string filename = "";

	std::string name = "";
	bool isCrime = false;
	RE::TESObjectREFR* ptr = nullptr;
	RE::LOCK_LEVEL lockLevel;
	bool isDeleted = false;
	bool isHarvested = false;
	bool isEnchanted = false;

	float distance = 0.0f;
	int direction = 0;
	bool isAuto = false;
};

struct ItemInfoBOOK : public ItemInfo
{
	bool isRead = false;
	bool isSpell = false;
	bool isSkill = false;
};

struct ItemInfoCONT : public ItemInfo
{
	int invCount = 0;
	ItemInvInfo invs[200];
};

struct ExcludeForm
{
	RE::FormID formId = 0;
	std::string name = "";
	std::string formTypeStr = "";

	bool operator<(const ExcludeForm& other) const
	{
		return formId < other.formId;
	}
};

struct WeatherForm
{
	RE::FormID formId = 0;
	std::string editorId = "";

	bool operator<(const WeatherForm& other) const
	{
		return formId < other.formId;
	}
};




struct IncludeForm
{
	RE::FormID formId = 0;
	std::string name = "";
	std::string formTypeStr = "";

	bool operator<(const IncludeForm& other) const
	{
		return formId < other.formId;
	}
};

struct GalleryForm
{
	RE::FormID formId = 0;
	std::string name = "";
	std::string modName = "";
	//std::string formIdStr = "";

	bool operator<(const GalleryForm& other) const
	{
		return formId < other.formId;
	}
};

struct GalleryModForm
{
	int compileIndex = 0;
	int smallFileCompileIndex = 0;
	std::string filename = "";
	std::string name = "";
	size_t totalItemCount = 0;
	size_t itemCount = 0;
	//std::string formIdStr = "";

	bool operator<(const GalleryModForm& other) const
	{
		return compileIndex < other.compileIndex;
	}
};

struct Item2Info
{
	int itemCount = 0;
	int itemCountWEAP = 0;
	int itemCountARMO = 0;
	int itemCountAMMO = 0;
	int itemCountBOOK = 0;
	int itemCountALCH = 0;
	int itemCountINGR = 0;
	int itemCountMISC = 0;
	int itemCountCONT = 0;
	int itemCountFLOR = 0;
	int itemCountFOOD = 0;
	int itemCountKEYM = 0;
	int itemCountTREE = 0;
	int itemCountSGEM = 0;
	int itemCountACTI = 0;
	int itemCountACHR = 0;
	int itemCountSTON = 0;
	int itemCountANVI = 0;

	ItemInfo itemInfo[3000];
	ItemInfo itemInfoWEAP[3000];
	ItemInfo itemInfoARMO[3000];
	ItemInfo itemInfoAMMO[3000];
	ItemInfoBOOK itemInfoBOOK[3000];
	ItemInfo itemInfoALCH[3000];
	ItemInfo itemInfoINGR[3000];
	ItemInfo itemInfoMISC[3000];
	ItemInfoCONT itemInfoCONT[3000];
	ItemInfo itemInfoFLOR[3000];
	ItemInfo itemInfoFOOD[3000];
	ItemInfo itemInfoKEYM[3000];
	ItemInfo itemInfoTREE[3000];
	ItemInfo itemInfoSGEM[3000];
	ItemInfo itemInfoACTI[3000];
	ItemInfo itemInfoSTON[3000];
	ItemInfo itemInfoANVI[3000];

	ItemInfoCONT itemInfoACHR[3000];
};

int getItemCount();
int getItemCountWEAP();
int getItemCountARMO();
int getItemCountAMMO();
int getItemCountBOOK();
int getItemCountINGR();
int getItemCountALCH();
int getItemCountMISC();
int getItemCountCONT();
int getItemCountFLOR();
int getItemCountFOOD();
int getItemCountKEYM();
int getItemCountTREE();
int getItemCountSGEM();
int getItemCountACTI();
int getItemCountACHR();
int getItemCountSTON();
int getItemCountANVI();

ItemInfo* getItems();
ItemInfo* getItemsWEAP();
ItemInfo* getItemsARMO();
ItemInfo* getItemsAMMO();
ItemInfoBOOK* getItemsBOOK();
ItemInfo* getItemsINGR();
ItemInfo* getItemsALCH();
ItemInfo* getItemsMISC();
ItemInfoCONT* getItemsCONT();
ItemInfo* getItemsFLOR();
ItemInfo* getItemsFOOD();
ItemInfo* getItemsKEYM();
ItemInfo* getItemsTREE();
ItemInfo* getItemsSGEM();
ItemInfo* getItemsACTI();
ItemInfo* getItemsSTON();
ItemInfo* getItemsANVI();

ItemInfoCONT* getItemsACHR();

extern std::unordered_set<int> excludeFormIds;
extern std::vector<ExcludeForm> excludeForms;

extern std::unordered_set<RE::TESObjectREFR*> trackPtrs;
extern std::unordered_set<RE::Actor*> trackActorPtrs;

extern std::unordered_set<int> galleryFormIds;
extern std::vector<GalleryForm> galleryFormData;
extern std::vector<GalleryModForm> galleryFormModData;
extern int galleryModTotalCount;
extern int galleryModCount;
extern int galleryItemTotalCount;
extern int galleryItemCount;

extern std::vector<WeatherForm> weatherForms;
extern RE::FormID currentWeather;

extern int screenWidth;
extern int screenHeight;

float calculateDistance(const RE::NiPoint3& p1, const RE::NiPoint3& p2);

static const RE::FormID VendorItemOreIngot = 0x000914EC;
static const RE::FormID VendorItemGem = 0x000914ED;

extern bool show_player_base_info_window;
extern bool show_player_armor_window;
extern bool show_player_weapon_window;
extern bool show_player_mod_window;
extern bool show_player_info_window;
