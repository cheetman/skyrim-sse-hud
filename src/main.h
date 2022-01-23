#pragma once
#include "event\BSTMenuEvent.h"
#include "event\BSTScriptEvent.h"
#include "sse-imgui\imgui_wrapped.h"
#include "sse-imgui\sse-imgui.h"
#include "utils\GeneralUtil.h"
#include "utils\NameUtil.h"
#include "utils\PlayerDataProvider.h"
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

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
	std::string name = ""; // 玩家名
	std::string ArrowDamage = ""; //弓箭伤害
	std::string DamageRight = ""; //右手伤害
	std::string DamageLeft = ""; //左手伤害
	std::string DamageStr = "";  //HUD显示内容
	//std::string DamageResist = ""; // 护甲值
	float kDamageResist = 0.0f; // 护甲值
	float kPoisonResist = 0.0f; // 毒抗
	float kResistFire = 0.0f;    // 火炕
	float kResistFrost = 0.0f;  // 冰抗
	float kResistShock = 0.0f;   // 雷抗
	float kResistMagic = 0.0f;  // 法抗
	float kResistDisease = 0.0f;  // 抗疾病
	RE::NiPoint3 Angle;           // 视角
	RE::NiPoint3 Position;        // 坐标

} playerInfo;


// 装备防具信息

struct ArmorInfo
{
	//bool isShow = false;
	bool isExist = false;  // 是否装备
	bool isMainSlotAlert = false; 
	bool isSpeacilSlotAlert = true;
	std::string treeId = ""; //HUD显示内容

	std::string name = ""; // 装备名称
	std::string formID = "";  // 物品ID
	std::string formTypeName = "";  // 物品类型
	std::string equipSlotName = ""; // 装备插槽部位
	std::string armorTypeName = ""; // 装备类型
	int goldValue = 0;	// 装备价格
	int value = 0;  // 属性值
	float armorRating = 0; // 装备等级
	float weight = 0; // 重量
};
ArmorInfo wornArmos[32];


// 装备武器、弹药信息

struct WeaponInfo
{
	bool isExist = false;     // 是否装备
	std::string treeId = "";  //HUD显示内容

	std::string name = "";  // 武器名称
	std::string formID = "";  // 物品ID
	std::string formTypeName = "";  // 物品类型
	std::string weaponTypeName = "";  // 武器类型
	RE::FormType formType = RE::FormType::None; 
	int goldValue = 0; 	// 装备价格
	int value = 0;      // 属性值
	int critDamage = 0; // 暴击伤害
	int damage = 0; // 武器伤害
	float weight = 0; // 武器重量

	// 法术
	std::string castingTypeName = ""; // 法术类型
	std::string spellTypeName = "";    // 法术类型
	float cost = 0;  // 法术消耗
	float time = 0;  // 施法时间
	bool isTwoHand = false; // 是否占用双手
} 
leftWeaponInfo, rightWeaponInfo, ammoInfo;






static bool startflag = false;
static bool auto_remove_ammo = true;

// 文件路径
std::filesystem::path settings;
std::string fontFilePath;

// 默认配置
static bool show_player_info_window = true;
static bool show_player_armor_window = true;
static bool show_player_weapon_window = true;
static bool show_player_debug_window = false;
static bool no_titlebar = false;
static bool no_resize = false;
static bool no_collapse = false;
static bool no_background = false;
static bool auto_resize = false;
static int imgui_style_index = 2;

bool save_settings();
bool load_settings();
