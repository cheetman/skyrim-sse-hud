#pragma once
#include <string>
#include <RE/F/FormTypes.h>
#include <RE/N/NiPoint3.h>



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


extern WeaponInfo leftWeaponInfo;
extern WeaponInfo rightWeaponInfo;
extern WeaponInfo ammoInfo;

extern int refresh_time_data;
extern bool startflag;


void __cdecl RefreshGameInfo(void*);