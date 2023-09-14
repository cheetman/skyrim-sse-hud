#pragma once
#include <string>
#include <RE/F/FormTypes.h>
#include <RE/N/NiPoint3.h>
#include <RE/A/ActorState.h>


extern bool active;
extern bool activeItems;

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

	
	std::string location = "";  // 位置
};


extern PlayerInfo playerInfo;

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
} ;

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

	InventoryInfo Inventorys[60];
};



extern int show_npc_window_dis_meter;
extern bool show_npc_window_dis;
extern bool show_npc_window_dead_hidden;
extern bool show_enemy_window;
extern bool show_inv_window;
extern bool show_npc_window;

extern WeaponInfo leftWeaponInfo;
extern WeaponInfo rightWeaponInfo;
extern WeaponInfo ammoInfo;

extern int refresh_time_data;
extern bool startflag;

//extern bool isRefreshActorInfo;
void __cdecl RefreshGameInfo(void*);
void __cdecl RefreshActorInfo(void*);
void __cdecl RefreshItemInfo(void*);



ActorInfo* getTeammateData();
ActorInfo* getNpcData();
ActorInfo* getEnemy2Data();
int getTeammateCount();
int getNpcCount();
int getEnemyCount();


int getPlayerInvCount();
int getPlayerInvBOOKCount();
int getPlayerInvWEAPCount();
int getPlayerInvARMOCount();
int getPlayerInvAMMOCount();
int getPlayerInvALCHCount();
int getPlayerInvFOODCount();
int getPlayerGoldCount();

InventoryInfo* getPlayerInvData();
InventoryInfo* getPlayerInvARMOData();
InventoryInfo* getPlayerInvBOOKData();
InventoryInfo* getPlayerInvWEAPData();
InventoryInfo* getPlayerInvAMMOData();
InventoryInfo* getPlayerInvALCHData();
InventoryInfo* getPlayerInvFOODData();
InventoryInfo* getPlayerInvData(int);

struct Actor2Info
{
	int npcCount = 0;
	int enemyCount = 0;
	int teammateCount = 0;
	ActorInfo npcInfo[60];
	ActorInfo enemyInfo[60];
	ActorInfo teammateInfo[50];
};

struct PlayerInventoryInfo
{
	int inventoryARMOCount = 0;
	int inventoryWEAPCount = 0;
	int inventoryBOOKCount = 0;
	int inventoryAMMOCount = 0;
	int inventoryALCHCount = 0;
	int inventoryFOODCount = 0;
	int inventoryCount = 0;
	int gold = 0;
	InventoryInfo inventorysARMO[300];
	InventoryInfo inventorysWEAP[300];
	InventoryInfo inventorysBOOK[300];
	InventoryInfo inventorysAMMO[100];
	InventoryInfo inventorysALCH[300];
	InventoryInfo inventorysFOOD[300];
	InventoryInfo inventorys[300];
};

// 临时

	
extern bool isGameLoading;



extern bool show_items_window;

struct ItemInvInfo
{
	RE::InventoryEntryData* ptr;
	int count;
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
	
	std::string name = "";
	bool isCrime = false;
	RE::TESObjectREFR* ptr = nullptr;
	RE::LOCK_LEVEL lockLevel;
	bool isDeleted = false;
	bool isHarvested = false;

	
	int invCount = 0;
	ItemInvInfo invs[200];
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
	ItemInfo itemInfo[1000];
	ItemInfo itemInfoWEAP[1000];
	ItemInfo itemInfoARMO[1000];
	ItemInfo itemInfoAMMO[1000];
	ItemInfo itemInfoBOOK[1000];
	ItemInfo itemInfoALCH[1000];
	ItemInfo itemInfoINGR[1000];
	ItemInfo itemInfoMISC[1000];
	ItemInfo itemInfoCONT[1000];
	ItemInfo itemInfoFLOR[1000];
	ItemInfo itemInfoFOOD[1000];
	
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
ItemInfo* getItems();
ItemInfo* getItemsWEAP();
ItemInfo* getItemsARMO();
ItemInfo* getItemsAMMO();
ItemInfo* getItemsBOOK();
ItemInfo* getItemsINGR();
ItemInfo* getItemsALCH();
ItemInfo* getItemsMISC();
ItemInfo* getItemsCONT();
ItemInfo* getItemsFLOR();
ItemInfo* getItemsFOOD();
