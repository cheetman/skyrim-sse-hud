//#pragma once
#include <cstdint>
#include <PCH.h>
#include "utils/utils.h"


struct EnemyInfo
{
	std::uint32_t formId = 0;
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
	int isSentient = 0;
	int level = 0.0f;
	float kHealthBase = 0.0f;
	float kMagickaBase = 0.0f;
	float kStaminaBase = 0.0f;
	time_t updateTime = 0;
	
} ;




template <typename T1, typename T2>
inline T2* dyna_cast(T1* base)
{
	auto asForm = static_cast<T1*>(base);
	auto ret = (asForm)->As<T2>();
	return ret;
}
#define DYNAMIC_CAST(base, srcType, targetType) (dyna_cast<srcType, targetType>(base))

void hookInstall();

EnemyInfo* getEnemyData();


//bool Hook_EnemyHealthLookupReferenceByHandle_impl(const RE::RefHandle& refHandle, RE::NiPointer<RE::TESObjectREFR>& refrOut);
