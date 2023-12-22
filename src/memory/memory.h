#pragma once
#include <RE/A/ActorState.h>
#include <RE/F/FormTypes.h>
#include <RE/N/NiPoint3.h>
#include <string>
#include <unordered_set>
#include <memory/quest.h>




extern bool active;
extern bool activeItems;



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

extern bool isRCMenuMove;

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
	ItemInfo itemInfoANHD[3000];  // 皮毛
	ItemInfo itemInfoANPA[3000];  // 组织
	ItemInfo itemInfoTOOL[3000];  // 工具
	ItemInfoCONT itemInfoACHR[3000];


	
	int questCount = 0;
	std::vector<QuestInfo> quests;
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
int getQuestCount();

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
std::vector<QuestInfo>& getQuests();

Item2Info* getItem2Info();

extern int screenWidth;
extern int screenHeight;

static const RE::FormID VendorItemOreIngot = 0x000914EC;
static const RE::FormID VendorItemGem = 0x000914ED;
static const RE::FormID VendorItemAnimalHide = 0x000914EA;
static const RE::FormID VendorItemAnimalPart = 0x000914EB;
static const RE::FormID VendorItemTool = 0x000914EE;

static const RE::FormID LocTypePlayerHouse = 0x000FC1A3;

extern int show_items_window_auto_dis_skyrim;
extern int show_items_window_auto_dis_local;
 extern int show_inv_window_height;


void clearItemInfoCache();

extern bool isCrimeIgnore;

extern bool isShowQuest;
