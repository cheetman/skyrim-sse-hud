#pragma once
#include "event\BSTMenuEvent.h"
#include "event\BSTScriptEvent.h"
#include "event\BSTCrosshairRefEvent.h"

#include "sse-imgui\imgui_wrapped.h"
#include "sse-imgui\sse-imgui.h"
#include "utils\GeneralUtil.h"
#include "utils\NameUtil.h"
#include "utils\PlayerDataProvider.h"
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "hook\hudhook.h"

//static float health = 0.0f;
//static float kMagicka = 0.0f;
//static float kStamina = 0.0f;
//static float armor = 0.0f;
//static float lightArmor = 0.0f;
//static float heavyArmor = 0.0f;
//static float kMeleeDamage = 0.0f;
//static float kUnarmedDamage = 0.0f;
//static float kCriticalChance = 0.0f;
//static float kDamageResist = 0.0f;
//static float lightArmor2 = 0.0f;
//static float heavyArmor2 = 0.0f;
//std::string playerRaceName = "";

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

} playerInfo;

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
ArmorInfo wornArmos[32];

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
} leftWeaponInfo, rightWeaponInfo, ammoInfo;

static bool startflag = false;
static bool auto_remove_ammo = false;

// 文件路径
std::filesystem::path settings;
std::string fontFilePath;

// 默认配置
static bool show_player_base_info_window = false;
static bool flag_process = false;
static bool flag_base_info_setting = true;

static bool show_player_info_window = true;
static bool show_player_mod_window = false;
static bool show_player_armor_window = true;
static bool show_player_weapon_window = true;
static bool show_player_debug_window = false;
static bool no_titlebar = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_background = false;
static bool auto_resize = true;
static int imgui_style_index = 2;

// 20220428 追加
static bool window_border = true;
static bool frame_border = true;
static bool bullet_text = false;

//static int refresh_time_show = 1000;
static int refresh_time_data = 300;
static float colorPlotHistogramX = 0.9f;
static float colorPlotHistogramY = 0.7f;
static float colorPlotHistogramZ = 0;
static float colorPlotHistogramW = 1;

static bool show_enemy_window = false;
static bool show_crosshair = false;


bool save_settings();
bool load_settings();

const char* actorValues[] = {
	"负重 [kCarryWeight]",
	"生命恢复 [kHealRate]",
	"魔法恢复 [kMagickaRate]",
	"耐力恢复 [KStaminaRate]",
	"行走速度 [kSpeedMult]",
	"武器基础伤害 [kMeleeDamage]",
	"空手基础伤害 [kUnarmedDamage]"
};
RE::ActorValue actorValuesIndex[] = {
	RE::ActorValue::kCarryWeight,
	RE::ActorValue::kHealRate,
	RE::ActorValue::kMagickaRate,
	RE::ActorValue::KStaminaRate,
	RE::ActorValue::kSpeedMult,
	RE::ActorValue::kMeleeDamage,
	RE::ActorValue::kUnarmedDamage,
};

const char* perActorValues[] = {
	"毒抗 [kPoisonResist]",
	"火抗 [kResistFire]", 
	"冰抗 [kResistFrost]", 
	"雷抗 [kResistShock]",
	"法抗 [kResistMagic]",
	"抗疾病 [kResistDisease]", 
	"远程基础伤害 [kMarksmanPowerModifier]",
	"法术基础伤害 [kDestructionPowerModifier]"
};
RE::ActorValue perActorValuesIndex[] = {
	RE::ActorValue::kPoisonResist,
	RE::ActorValue::kResistFire,
	RE::ActorValue::kResistFrost,
	RE::ActorValue::kResistShock,
	RE::ActorValue::kResistMagic,
	RE::ActorValue::kResistDisease,
	RE::ActorValue::kMarksmanPowerModifier,
	RE::ActorValue::kDestructionPowerModifier,
};

// 基础属性进度条
static float progress = 0;
static float progress2 = 0;
static float progress3 = 0;
static int stateMod_lastSelectIndex = -1;
static int stateMod_selectIndex = 0;
static float stateMod_nowValue = 0;
static float stateMod_newValue = 0;
static int statePerMod_lastSelectIndex = -1;
static int statePerMod_selectIndex = 0;
static float statePerMod_nowValue = 0;
static float statePerMod_newValue = 0;


