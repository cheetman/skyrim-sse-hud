#pragma once
#include <unordered_set>
struct IncludeForm;
struct ExcludeForm;

extern std::unordered_set<RE::FormID> autoContFormIds;
extern std::vector<IncludeForm> autoContForms;
extern std::unordered_set<int> excludeLocationFormIds;
extern std::vector<ExcludeForm> excludeLocationForms;
//extern std::unordered_set<RE::TESObjectREFR*> deleteREFRs;

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
