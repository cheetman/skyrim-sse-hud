#pragma once
#include <RE/A/ActorState.h>
#include <RE/F/FormTypes.h>
#include <RE/N/NiPoint3.h>
#include <string>
#include <unordered_set>




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

	//RE::FormID weather = 0;
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



extern bool active;
extern bool activeItems;

extern PlayerInfo playerInfo;


extern WeaponInfo leftWeaponInfo;
extern WeaponInfo rightWeaponInfo;
extern WeaponInfo ammoInfo;

extern int refresh_time_data;
extern bool startflag;

void __cdecl RefreshGameInfo(void*);
void __cdecl RefreshItemInfo(void*);




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
	std::string ownerName = "";
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
	RE::TESWeather::WeatherDataFlag flag = RE::TESWeather::WeatherDataFlag::kNone;

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
	int itemCountANHD = 0;
	int itemCountANPA = 0;
	int itemCountTOOL = 0;

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
	ItemInfo itemInfoANHD[3000]; // 皮毛
	ItemInfo itemInfoANPA[3000]; // 组织
	ItemInfo itemInfoTOOL[3000]; // 工具

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
int getItemCountANHD();
int getItemCountANPA();
int getItemCountTOOL();

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
ItemInfo* getItemsANHD();
ItemInfo* getItemsANPA();
ItemInfo* getItemsTOOL();

extern std::unordered_set<int> excludeFormIds;
extern std::vector<ExcludeForm> excludeForms;

extern std::unordered_set<RE::TESObjectREFR*> trackPtrs;
extern std::unordered_set<RE::Actor*> trackActorPtrs;

//extern std::unordered_set<int> galleryFormIds;
//extern std::vector<GalleryForm> galleryFormData;
//extern std::vector<GalleryModForm> galleryFormModData;
//extern int galleryModTotalCount;
//extern int galleryModCount;
//extern int galleryItemTotalCount;
//extern int galleryItemCount;

extern std::vector<WeatherForm> weatherForms;
extern RE::FormID currentWeather;

extern int screenWidth;
extern int screenHeight;


static const RE::FormID VendorItemOreIngot = 0x000914EC;
static const RE::FormID VendorItemGem = 0x000914ED;
static const RE::FormID VendorItemAnimalHide = 0x000914EA;
static const RE::FormID VendorItemAnimalPart = 0x000914EB;
static const RE::FormID VendorItemTool = 0x000914EE;


extern bool show_player_base_info_window;
extern bool show_player_armor_window;
extern bool show_player_weapon_window;
extern bool show_player_mod_window;
extern bool show_player_info_window;

extern int show_items_window_auto_dis_skyrim;
extern int show_items_window_auto_dis_local;

extern int show_inv_window_height;

void initData();

void clearItemInfoCache();
