#pragma once
#include <PCH.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <stdlib.h>

namespace StringUtil
{
	static std::string cutString(std::string a_value)
	{
		std::string text;
		if (a_value.find(".") != std::string::npos) {
			auto s = a_value.substr(a_value.find(".") + 1, 2);
			if (count(s.begin(), s.end(), '0') == 2) {
				text = a_value.substr(0, a_value.find("."));
			} else {
				text = a_value.substr(0, a_value.find(".") + 3);
			}
		} else {
			text = a_value;
		}
		return text;
	}

	static std::string getStringValueFromFloat(float a_x) { return cutString(fmt::format(FMT_STRING("{:.2f}"), a_x)); }

	static float calculateValue(float a_rm, float a_r) { return (a_rm * a_r) / 100; }

	template <typename T>
	static std::string intToHex(T a_i)
	{
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << a_i;

		return stream.str();
	}

	template <typename T>
	static std::string delimitTwoValues(float a_v1, float a_v2, T v_d)
	{
		return fmt::format(FMT_STRING("{}{}{}"), getStringValueFromFloat(a_v1), v_d, getStringValueFromFloat(a_v2));
	}

	
	static std::string Utf8ToGbk(const char* src_str)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
		wchar_t* wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char* szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		std::string strTemp(szGBK);
		if (wszGBK)
			delete[] wszGBK;
		if (szGBK)
			delete[] szGBK;
		return strTemp;
	}

}


namespace ValueUtil
{
	static float calculateArmorDamageRes(float a_armor_rating, int32_t a_pieces_worn)
	{
		return (float(a_armor_rating * 0.12) + float(3 * a_pieces_worn));
	}

	static std::string getValueWithCapIfNeeded(float a_res, float a_cap, std::string a_ending)
	{
		auto value = StringUtil::getStringValueFromFloat(a_res);

		if (a_res > a_cap) {
			value = fmt::format(FMT_STRING("{}{} ({})"),
				StringUtil::getStringValueFromFloat(a_cap),
				a_ending,
				StringUtil::getStringValueFromFloat(a_res));
		}
		return value;
	}

	static std::string getValueWithPermAV(float a_av, float a_perm_av)
	{
		if (a_av != a_perm_av) {
			return StringUtil::delimitTwoValues(a_av, a_perm_av, "/"sv);
		}
		return StringUtil::getStringValueFromFloat(a_av);
	}
}

namespace VectorUtil
{
	static std::vector<uint16_t> getIntersect(std::vector<uint16_t>& a_vec_first, std::vector<uint16_t>& a_vec_second)
	{
		std::vector<uint16_t> intersectionVector;

		std::sort(a_vec_first.begin(), a_vec_first.end());
		std::sort(a_vec_second.begin(), a_vec_second.end());

		std::set_intersection(a_vec_first.begin(),
			a_vec_first.end(),
			a_vec_second.begin(),
			a_vec_second.end(),
			back_inserter(intersectionVector));
		return intersectionVector;
	}

	template <typename T>
	static std::string getDelimitedString(std::vector<T>& a_vec)
	{
		auto deliString =
			a_vec.empty() ?
                "" :
                std::accumulate(std::begin(a_vec), std::end(a_vec), std::string{}, [](std::string r, int p) {
					return std::move(r) + std::to_string(p) + ",";
				});

		if (!deliString.empty()) {
			deliString.pop_back();
		}
		return deliString;
	}
}

namespace QuestUtil
{
	static std::vector<uint16_t> getStagesIfComplete(RE::TESQuest* a_quest)
	{
		std::vector<uint16_t> finStages;

		//all favour quests finish at 25 execpt rift = 20
		auto hex = StringUtil::intToHex(a_quest->GetFormID());
		logger::debug("Questname ({}), formid {}, prio {}"sv, a_quest->GetName(), hex, a_quest->data.priority);

		//todo make util function if for checking stages
		logger::trace("executed Stages for quest {}"sv, hex);
		auto exec = a_quest->executedStages;
		for (auto it = exec->begin(); it != exec->end(); ++it) {
			auto i = *it;
			auto qstdata = i.data;
			logger::trace("index {}, flag {}"sv, 1);
			if (qstdata.flags.underlying() == 1) {
				finStages.push_back(qstdata.index);
			}
		}

		logger::trace("waiting Stages for quest {}"sv, hex);
		auto waiting = a_quest->waitingStages;
		for (auto it = waiting->begin(); it != waiting->end(); ++it) {
			auto i = *it;
			auto qstdata = i->data;
			logger::trace("index {}, flag {}"sv, qstdata.index, qstdata.flags.get());
			if (qstdata.flags.underlying() == 1) {
				finStages.push_back(qstdata.index);
			}
		}
		return finStages;
	}

	static bool isOneQuestStageComplete(RE::TESQuest* a_quest, std::vector<uint16_t> a_stages)
	{
		auto finStages = getStagesIfComplete(a_quest);
		auto intersect = VectorUtil::getIntersect(finStages, a_stages);
		auto hex = StringUtil::intToHex(a_quest->GetFormID());

		logger::debug("For Quest {}, got Stages (Values)/size completed ({})/{}, given ({})/{}, intersect ({})/{}"sv,
			hex,
			VectorUtil::getDelimitedString(finStages),
			finStages.size(),
			VectorUtil::getDelimitedString(a_stages),
			a_stages.size(),
			VectorUtil::getDelimitedString(intersect),
			intersect.size());

		return intersect.size() > 0 ? true : false;
	}

	static bool isQuestStageComplete(RE::TESQuest* a_quest, uint16_t a_stage)
	{
		return isOneQuestStageComplete(a_quest, std::vector{ a_stage });
	}

	static bool isQuestStageComplete(RE::FormID a_form_id, uint16_t a_stage)
	{
		auto qst = RE::TESForm::LookupByID(a_form_id)->As<RE::TESQuest>();
		return isQuestStageComplete(qst, a_stage);
	}

	static bool isOneQuestStageComplete(RE::FormID a_form_id, std::vector<uint16_t> a_stages)
	{
		auto qst = RE::TESForm::LookupByID(a_form_id)->As<RE::TESQuest>();
		return isOneQuestStageComplete(qst, a_stages);
	}

	static uint16_t getAs(int a_i) { return static_cast<uint16_t>(a_i); }
}


namespace Detours
{
	static_assert(sizeof(uintptr_t) == sizeof(void*));

	const static uint32_t DISASM_MAX_INSTRUCTIONS = 50;  // Maximum number of instructions to decode at once

	const static uint32_t OPT_MASK = 0xFFF;             // Mask for all options
	const static uint32_t OPT_NONE = 0x000;             // No options
	const static uint32_t OPT_BREAK_ON_FAIL = 0x001;    // Throw INT3 on failure
	const static uint32_t OPT_DO_NOT_PAD_NOPS = 0x002;  // Don't pad replaced instructions with NOPs

	struct JumpTrampolineHeader
	{
		uint32_t Magic;   // Used to verify the header
		uint32_t Random;  // Variable to change the code/data hash

		uintptr_t CodeOffset;    // Offset, in code, that was hooked:	"target"
		uintptr_t DetourOffset;  // User function that is called:		"destination"

		uintptr_t InstructionLength;  // Length of the instructions that were replaced
		uintptr_t InstructionOffset;  // Where the backed-up instructions are

		uintptr_t TrampolineLength;  // Length of the trampoline
		uintptr_t TrampolineOffset;  // Code offset where 'jmp (q/d)word ptr <user function>' occurs

		// Anything after this struct is null data or pure code (instructions/trampoline)
	};

	void SetGlobalOptions(uint32_t Options);
	uint32_t GetGlobalOptions();

	uint64_t DetourAlignAddress(uint64_t Address, uint8_t Align);

	bool DetourCopyMemory(uintptr_t Target, uintptr_t Memory, size_t Length);
	bool DetourFlushCache(uintptr_t Target, size_t Length);
	uintptr_t IATHook(uintptr_t Module, const char* ImportModule, const char* API, uintptr_t Detour);
	uintptr_t IATDelayedHook(uintptr_t Module, const char* ImportModule, const char* API, uintptr_t Detour);

#ifdef _M_IX86
	enum class X86Option
	{
		USE_JUMP,      // jmp <address>;
		USE_CALL,      // call <address>;
		USE_PUSH_RET,  // push <address>; retn;
	};

	namespace X86
	{
		// Redirects a single static function to another
		uintptr_t DetourFunction(uintptr_t Target, uintptr_t Detour, X86Option Options = X86Option::USE_JUMP);

		// Redirects a class member function (__thiscall) to another
		template <typename T>
		uintptr_t DetourFunctionClass(uintptr_t Target, T Detour, X86Option Options = X86Option::USE_JUMP)
		{
			return DetourFunction(Target, *(uintptr_t*)&Detour, Options);
		}

		// Removes a detoured function (Static or class member)
		bool DetourRemove(uintptr_t Trampoline);

		// Redirects an index in a virtual table
		uintptr_t DetourVTable(uintptr_t Target, uintptr_t Detour, uint32_t TableIndex);

		// Redirects a class member virtual function (__thiscall) to another
		template <typename T>
		uintptr_t DetourClassVTable(uintptr_t Target, T Detour, uint32_t TableIndex)
		{
			return DetourVTable(Target, *(uintptr_t*)&Detour, TableIndex);
		}

		// Removes a detoured virtual table index
		bool VTableRemove(uintptr_t Target, uintptr_t Function, uint32_t TableIndex);

		void DetourWriteStub(JumpTrampolineHeader* Header);
		bool DetourWriteJump(JumpTrampolineHeader* Header);
		bool DetourWriteCall(JumpTrampolineHeader* Header);
		bool DetourWritePushRet(JumpTrampolineHeader* Header);

		uint32_t DetourGetHookLength(X86Option Options);
	}
#endif  // _M_IX86

#ifdef _M_AMD64
	enum class X64Option
	{
		USE_PUSH_RET,    // push <low 32 address>; [rsp+4h] = <hi 32 addr>; retn;
		USE_RAX_JUMP,    // mov rax, <address>; jmp rax;
		USE_REL32_JUMP,  // jmp <address within +/- 2GB>
		USE_REL32_CALL,  // call <address within +/- 2GB>
	};

	namespace X64
	{
		// Redirects a single static function to another
		uintptr_t DetourFunction(uintptr_t Target, uintptr_t Detour, X64Option Options = X64Option::USE_REL32_JUMP);

		// Redirects a class member function (__thiscall) to another
		template <typename T>
		uintptr_t DetourFunctionClass(uintptr_t Target, T Detour, X64Option Options = X64Option::USE_REL32_JUMP)
		{
			return DetourFunction(Target, *(uintptr_t*)&Detour, Options);
		}

		// Removes a detoured function (Static or class member)
		bool DetourRemove(uintptr_t Trampoline);

		// Redirects an index in a virtual table
		uintptr_t DetourVTable(uintptr_t Target, uintptr_t Detour, uint32_t TableIndex);

		// Redirects a class member virtual function (__thiscall) to another
		template <typename T>
		uintptr_t DetourClassVTable(uintptr_t Target, T Detour, uint32_t TableIndex)
		{
			return DetourVTable(Target, *(uintptr_t*)&Detour, TableIndex);
		}

		// Removes a detoured virtual table index
		bool VTableRemove(uintptr_t Target, uintptr_t Function, uint32_t TableIndex);

		void DetourWriteStub(JumpTrampolineHeader* Header);
		bool DetourWritePushRet(JumpTrampolineHeader* Header);
		bool DetourWriteRaxJump(JumpTrampolineHeader* Header);
		bool DetourWriteRel32Jump(JumpTrampolineHeader* Header);
		bool DetourWriteRel32Call(JumpTrampolineHeader* Header);

		uint32_t DetourGetHookLength(X64Option Options);
	}
#endif  // _M_AMD64
}
