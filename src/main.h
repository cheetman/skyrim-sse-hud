#pragma once
#include "event\BSTMenuEvent.h"
#include "event\BSTScriptEvent.h"
#include "event\BSTCrosshairRefEvent.h"

//#include "sse-imgui\imgui_wrapped.h"
//#include "sse-imgui\sse-imgui.h"
#include "utils\GeneralUtil.h"
#include "utils\NameUtil.h"
#include "utils\PlayerDataProvider.h"
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "hook\hudhook.h"
#include "hook\d3d11hook.h"

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

class OnInputEventDispatch
{
public:
	static void Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<uintptr_t> caller{ REL::ID(67315) };
		_DispatchInputEvent = trampoline.write_call<5>(caller.address() + 0x7B, DispatchInputEvent);
	}

private:
	static void DispatchInputEvent(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent** a_evns);
	static inline REL::Relocation<decltype(DispatchInputEvent)> _DispatchInputEvent;
};

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


