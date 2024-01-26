#pragma once
#include <unordered_set>
struct IncludeForm;
struct ExcludeForm;

extern std::unordered_set<RE::FormID> autoContFormIds;
extern std::vector<IncludeForm> autoContForms;
extern std::unordered_set<RE::FormID> excludeLocationFormIds;
extern std::vector<ExcludeForm> excludeLocationForms;


extern std::unordered_map<std::string, std::unordered_set<RE::FormID>> whiteListFormIds;
extern std::unordered_map<std::string, std::vector<ExcludeForm>> whiteListForms;
extern std::unordered_map<std::string, std::unordered_set<RE::FormID>> blackListFormIds;
extern std::unordered_map<std::string, std::vector<ExcludeForm>> blackListForms;

extern bool auto_whitelist_misc;
extern bool auto_whitelist_ammo;
extern bool auto_whitelist_food;
extern bool auto_whitelist_flor;
extern bool auto_whitelist_ingr;
extern bool auto_whitelist_alch;
extern bool auto_whitelist_sgem;
extern bool auto_whitelist_tree;
extern bool auto_whitelist_weap;
extern bool auto_whitelist_ston;
extern bool auto_whitelist_anvi;
extern bool auto_whitelist_anhd;
extern bool auto_whitelist_anpa;
extern bool auto_whitelist_armo;
extern bool auto_whitelist_tool;

//extern std::unordered_set<RE::FormID> whiteListFormIds_misc;
//extern std::unordered_set<RE::FormID> whiteListFormIds_ammo;
//extern std::unordered_set<RE::FormID> whiteListFormIds_food;
//extern std::unordered_set<RE::FormID> whiteListFormIds_flor;
//extern std::unordered_set<RE::FormID> whiteListFormIds_ingr;
//extern std::unordered_set<RE::FormID> whiteListFormIds_alch;
//extern std::unordered_set<RE::FormID> whiteListFormIds_sgem;
//extern std::unordered_set<RE::FormID> whiteListFormIds_tree;
//extern std::unordered_set<RE::FormID> whiteListFormIds_weap;
//extern std::unordered_set<RE::FormID> whiteListFormIds_ston;
//extern std::unordered_set<RE::FormID> whiteListFormIds_anvi;
//extern std::unordered_set<RE::FormID> whiteListFormIds_anhd;
//extern std::unordered_set<RE::FormID> whiteListFormIds_anpa;
//extern std::unordered_set<RE::FormID> whiteListFormIds_armo;
//
//extern std::vector<ExcludeForm> whiteListForms_misc;
//extern std::vector<ExcludeForm> whiteListForms_ammo;
//extern std::vector<ExcludeForm> whiteListForms_food;
//extern std::vector<ExcludeForm> whiteListForms_flor;
//extern std::vector<ExcludeForm> whiteListForms_ingr;
//extern std::vector<ExcludeForm> whiteListForms_alch;
//extern std::vector<ExcludeForm> whiteListForms_sgem;
//extern std::vector<ExcludeForm> whiteListForms_tree;
//extern std::vector<ExcludeForm> whiteListForms_weap;
//extern std::vector<ExcludeForm> whiteListForms_ston;
//extern std::vector<ExcludeForm> whiteListForms_anvi;
//extern std::vector<ExcludeForm> whiteListForms_anhd;
//extern std::vector<ExcludeForm> whiteListForms_anpa;
//extern std::vector<ExcludeForm> whiteListForms_tool;
//extern std::vector<ExcludeForm> whiteListForms_armo;

extern bool show_items_window_auto_ignore;
extern int show_items_window_auto_dis;
 
extern bool show_items_window_auto_keym;
extern bool show_items_window_auto_ammo;
extern bool show_items_window_auto_flor;
extern bool show_items_window_auto_food;
extern bool show_items_window_auto_ingr;
extern bool show_items_window_auto_alch;
extern bool show_items_window_auto_book;
extern bool show_items_window_auto_misc;
extern bool show_items_window_auto_tree;
extern bool show_items_window_auto_sgem;
extern bool show_items_window_auto_achr;
extern bool show_items_window_auto_cont;
extern bool show_items_window_auto_conttype;
extern bool show_items_window_auto_armo;
extern bool show_items_window_auto_weap;
extern bool show_items_window_auto_ston;
extern bool show_items_window_auto_anvi;
extern bool show_items_window_auto_anhd;
extern bool show_items_window_auto_anpa;
extern bool show_items_window_auto_tool;
extern bool show_items_window_auto_achr_ingr;
extern bool show_items_window_auto_achr_food;
extern bool show_items_window_auto_achr_alch;
extern bool show_items_window_auto_achr_sgem;
extern bool show_items_window_auto_achr_ammo;
extern bool show_items_window_auto_achr_misc;
extern bool show_items_window_auto_achr_gold;
extern bool show_items_window_auto_achr_scrl;
extern bool show_items_window_auto_achr_keym;
extern bool show_items_window_auto_achr_book;
extern bool show_items_window_auto_achr_weap;
extern bool show_items_window_auto_achr_armo;
extern bool show_items_window_auto_achr_ston;
extern bool show_items_window_auto_achr_anvi;
extern bool show_items_window_auto_achr_lock;
extern bool show_items_window_auto_achr_anhd;
extern bool show_items_window_auto_achr_anpa;
extern bool show_items_window_auto_achr_tool;
extern bool show_items_window_auto_cont_ingr;
extern bool show_items_window_auto_cont_food;
extern bool show_items_window_auto_cont_alch;
extern bool show_items_window_auto_cont_sgem;
extern bool show_items_window_auto_cont_ammo;
extern bool show_items_window_auto_cont_misc;
extern bool show_items_window_auto_cont_book;
extern bool show_items_window_auto_cont_gold;
extern bool show_items_window_auto_cont_scrl;
extern bool show_items_window_auto_cont_keym;
extern bool show_items_window_auto_cont_weap;
extern bool show_items_window_auto_cont_armo;
extern bool show_items_window_auto_cont_ston;
extern bool show_items_window_auto_cont_anvi;
extern bool show_items_window_auto_cont_lock;
extern bool show_items_window_auto_cont_anhd;
extern bool show_items_window_auto_cont_anpa;
extern bool show_items_window_auto_cont_tool;
extern bool show_items_window_auto_notification;

extern bool show_items_window_auto_weap_enchant;
extern bool show_items_window_auto_weap_price;
extern bool show_items_window_auto_weap_priceweight;
extern bool show_items_window_auto_armo_enchant;
extern bool show_items_window_auto_armo_price;
extern bool show_items_window_auto_armo_priceweight;
extern int show_items_window_auto_weap_price_value;
extern int show_items_window_auto_armo_price_value;
extern int show_items_window_auto_weap_priceweight_value;
extern int show_items_window_auto_armo_priceweight_value;

extern int refresh_time_auto;
extern int show_items_window_auto_every_max;
extern int show_items_window_auto_every;
extern bool show_items_window_auto_enable;


void addItem(RE::PlayerCharacter* contReff, RE::TESObjectREFR* reff, int count);
void addItemAuto(RE::PlayerCharacter* contReff, RE::TESObjectREFR* reff, int count);
void RemoveItemACHR(RE::PlayerCharacter* contReff, RE::TESObjectREFR* actor, RE::TESBoundObject* obj, int count, bool autoTake);
void RemoveItemCONT(RE::PlayerCharacter* contReff, RE::TESObjectREFR* actor, RE::TESBoundObject* obj, int count, bool autoTake);

void __cdecl TimerAutoPick(void*);
