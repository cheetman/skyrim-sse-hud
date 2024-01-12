#pragma once
#include <memory/memory.h>

extern bool show_inv_window;



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
	bool isWorn = false;
	bool isEnchanted = false;
	std::uint16_t uniqueID = 0;
	std::string debugName = "";
	bool isAlchemyEnt = false;
	bool isSmithingEnt = false;
	bool isEnchantingEnt = false;
	float AlchemyValue = 0;
	float SmithingValue = 0;
	float EnchantingValue = 0;
	int slotMask = 0;
	std::string slotMaskStr = "";
	bool isFavor = false;
	bool isBest = false;
};


struct FavorEntDetailInfo 
{
	RE::FormID formId = 0;
	float value = 0;
};
struct FavorInfo : InventoryInfo
{
	//RE::FormID formId = 0;
	bool isMap = false;
	std::vector<FavorEntDetailInfo> entDetail;
	std::vector<FavorEntDetailInfo> entDetail2;
};

extern std::vector<FavorInfo> favorAlchemy;
extern std::vector<FavorInfo> favorSmithing;

bool compareForInventory(const InventoryInfo& info1, const InventoryInfo& info2);

struct PlayerInventoryInfo
{
	int inventoryARMOCount = 0;
	int inventoryWEAPCount = 0;
	int inventoryBOOKCount = 0;
	int inventoryAMMOCount = 0;
	int inventoryALCHCount = 0;
	int inventoryFOODCount = 0;
	int inventoryINGRCount = 0;
	int inventoryCount = 0;
	int gold = 0;
	InventoryInfo inventorysARMO[300];
	InventoryInfo inventorysWEAP[300];
	InventoryInfo inventorysBOOK[300];
	InventoryInfo inventorysAMMO[100];
	InventoryInfo inventorysALCH[300];
	InventoryInfo inventorysFOOD[300];
	InventoryInfo inventorysINGR[300];
	InventoryInfo inventorys[300];

	std::vector<InventoryInfo> inventorysAlchemy;
	std::vector<InventoryInfo> inventorysSmithing;
	std::vector<InventoryInfo> inventorysEnchanting;
	std::vector<InventoryInfo> inventorysAlchemyPotion;
	std::vector<InventoryInfo> inventorysSmithingPotion;
	std::vector<InventoryInfo> inventorysEnchantingPotion;
	std::vector<InventoryInfo> inventorysAlchemyFavor;
	std::vector<InventoryInfo> inventorysSmithingFavor;
	std::vector<InventoryInfo> inventorysEnchantingFavor;
	int inventorysAlchemyCount = 0;
	int inventorysEnchantingCount = 0;
	int inventorysSmithingCount = 0;
	int inventorysAlchemyPotionCount = 0;
	int inventorysEnchantingPotionCount = 0;
	int inventorysSmithingPotionCount = 0;
	int inventorysAlchemyFavorCount = 0;
	int inventorysEnchantingFavorCount = 0;
	int inventorysSmithingFavorCount = 0;

	
};


int getPlayerInvCount();
int getPlayerInvBOOKCount();
int getPlayerInvWEAPCount();
int getPlayerInvARMOCount();
int getPlayerInvAMMOCount();
int getPlayerInvALCHCount();
int getPlayerInvFOODCount();
int getPlayerInvINGRCount();
int getPlayerGoldCount();
int getPlayerInvAlchemyCount();
int getPlayerInvSmithingCount();
int getPlayerInvAlchemyPotionCount();
int getPlayerInvSmithingPotionCount();
int getPlayerInvEnchantingPotionCount();
int getPlayerInvAlchemyFavorCount();
int getPlayerInvSmithingFavorCount();
int getPlayerInvEnchantingFavorCount();
InventoryInfo* getPlayerInvData();
InventoryInfo* getPlayerInvARMOData();
InventoryInfo* getPlayerInvBOOKData();
InventoryInfo* getPlayerInvWEAPData();
InventoryInfo* getPlayerInvAMMOData();
InventoryInfo* getPlayerInvALCHData();
InventoryInfo* getPlayerInvFOODData();
InventoryInfo* getPlayerInvINGRData();
InventoryInfo* getPlayerInvData(int);
std::vector<InventoryInfo>& getPlayerInvAlchemy();
std::vector<InventoryInfo>& getPlayerInvSmithing();
std::vector<InventoryInfo>& getPlayerInvAlchemyPotion();
std::vector<InventoryInfo>& getPlayerInvSmithingPotion();
std::vector<InventoryInfo>& getPlayerInvEnchantingPotion();
std::vector<InventoryInfo>& getPlayerInvAlchemyFavor();
std::vector<InventoryInfo>& getPlayerInvSmithingFavor();
std::vector<InventoryInfo>& getPlayerInvEnchantingFavor();

void RefreshPlayerInvInfo();

bool isFavor(const InventoryInfo& inv, const std::vector<FavorInfo>& favors);
