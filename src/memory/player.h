#pragma once
#include <memory/memory.h>
#include <imgui/imgui.h>

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

	std::int32_t gold = 0;
	float equippedWeight = 0.0f;
	float carryWeight = 0.0f;

	std::string location = "";
	RE::FormID locationId = 0;
	std::string parentLocation = "";
	RE::FormID parentLocationId = 0;

	float xp = 0.0f;
	float levelThreshold = 0.0f;

	RE::TESObjectCELL* cellPtr = nullptr;
	std::string cellName = "";

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

struct EffectInfo
{
	float elapsedSeconds;
	float duration;
	float magnitude;
	std::string name;
	std::string spellName;
	RE::FormID effectId;
	RE::FormID spellId;
	std::string text;
	std::string text2;
};

struct Effects2Info
{
	int count = 0;
	std::vector<EffectInfo> list;
};

extern WeaponInfo leftWeaponInfo;
extern WeaponInfo rightWeaponInfo;
extern WeaponInfo ammoInfo;
extern WeaponInfo powerInfo;

extern ArmorInfo wornArmos[32];
extern PlayerInfo playerInfo;

extern bool show_player_base_info_window;
extern bool show_player_base_info_window_max_fix;
extern bool show_player_armor_window;
extern bool show_player_weapon_window;
extern bool show_player_mod_window;
extern bool show_player_info_window;
extern bool show_player_gold_window;
extern bool show_player_carryweight_window;
extern bool show_player_xp_window;
extern bool show_player_effects_window;
extern bool show_player_effects_ignore_permanent;
extern bool show_player_effects_negative;
extern bool show_player_effects_listignore;
extern bool show_player_effects_ignore_spell;
extern bool show_player_effects_process;

extern ImVec4 colorProgressEffect1;
//extern ImVec4 colorProgressEffect2;
extern ImVec4 colorProgressEffect3;

	/// <summary>
/// 刷新玩家信息
/// </summary>
void refreshPlayerInfo();

std::vector<EffectInfo>& getEffects();
int getEffectsCount();

struct ExcludeFormEffect
{
	RE::FormID effectId = 0;
	RE::FormID spellId = 0;
	std::string effectName = "";
	std::string spellName = "";

};

struct ExcludeFormEffectIds
{
	RE::FormID effectId = 0;
	RE::FormID spellId = 0;

	bool operator==(const ExcludeFormEffectIds& other) const
	{
		return effectId == other.effectId && spellId == other.spellId;
	}

};
struct ExcludeFormEffectIdsHash
{
	std::size_t operator()(const ExcludeFormEffectIds& s) const
	{
		// 这里可以自定义哈希函数，比如将两个 id 合并成一个哈希值
		return std::hash<RE::FormID>()(s.effectId) ^ std::hash<RE::FormID>()(s.spellId);
	}
};

extern std::unordered_set<ExcludeFormEffectIds, ExcludeFormEffectIdsHash> excludeEffectFormIds;
extern std::vector<ExcludeFormEffect> excludeEffectForms;
