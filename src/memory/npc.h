#pragma once
#include <memory/memory.h>
#include <imgui/imgui.h>
#include <memory/player_inv.h>

struct ExcludeForm;


struct ActorInfo
{
	RE::Actor* ptr = nullptr;

	std::string formIdStr = "";
	RE::FormID formId = 0;
	RE::FormID baseFormId = 0;
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
	int level = 0;
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



extern int show_npc_window_dis_meter;
extern bool show_npc_window_dis;
extern bool show_npc_window_direction;
extern bool show_npc_window_dead_hidden;
extern bool show_enemy_window;
extern bool show_npc_window;
extern bool show_npc_window_process;
extern bool show_npc_window_process_combat;
extern bool show_npc_window_ignore;

extern ImVec4 colorProgressNpc1;
extern ImVec4 colorProgressNpc2;
extern ImVec4 colorProgressNpc3;

extern std::unordered_set<RE::FormID> excludeNpcFormIds;
extern std::vector<ExcludeForm> excludeNpcForms;


ActorInfo* getTeammateData();
ActorInfo* getNpcData();
ActorInfo* getHorseData();
ActorInfo* getEnemy2Data();
int getTeammateCount();
int getNpcCount();
int getHorseCount();
int getEnemyCount();



void __cdecl RefreshActorInfo(void*);
