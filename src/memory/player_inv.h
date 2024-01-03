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
	bool isWorn = 0;
};
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
InventoryInfo* getPlayerInvData();
InventoryInfo* getPlayerInvARMOData();
InventoryInfo* getPlayerInvBOOKData();
InventoryInfo* getPlayerInvWEAPData();
InventoryInfo* getPlayerInvAMMOData();
InventoryInfo* getPlayerInvALCHData();
InventoryInfo* getPlayerInvFOODData();
InventoryInfo* getPlayerInvINGRData();
InventoryInfo* getPlayerInvData(int);

void RefreshPlayerInvInfo();
