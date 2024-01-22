#include "common.hpp"
#include "script.h"

#include "../../inc/MinHook.h"

#define CALL(name) try { g_HookHandler->Invoke<void, rage::scrNativeCallContext*>(name, ctx); } catch(std::exception& e) { PRINT_ERROR(e.what()); }
#define NHOOK(name, ptr, callback) g_HookHandler->Hook<void(rage::scrNativeCallContext*)>(name, GetNativeHandler(ptr), [](rage::scrNativeCallContext* ctx)callback)

class CHookHandler {
	std::unordered_map<std::string, void*> Original;
public:
	template<typename RT, typename... ArgsTypes, typename... Args>
	RT Invoke(const std::string& funcName, Args&&... args) {
		if (Original[funcName] == nullptr) {
			throw std::invalid_argument(std::format("Hooking| {}'s OriginalFunc Not Found!", funcName));
		}
		return reinterpret_cast<RT(__fastcall*)(ArgsTypes ...)>(Original[funcName])(std::forward<Args>(args)...);
	}

	int Hook(const std::string& funcName, void* Address, void* Detour) {
		MH_STATUS Ret = MH_CreateHook(Address, Detour, reinterpret_cast<void**>(&Original[funcName]));
		if (Ret == MH_OK) {
			PRINT_INFO("Hooking| Name:", funcName, " Trampoline:", Original[funcName], " Detour : ", Detour, " CreateHook Success");
			return MH_EnableHook(Address);
		}
		PRINT_INFO("Hooking| CreateHook Failed At:", Address, " Orig:", Original[funcName], " Error:", MH_StatusToString(Ret));
		return -1;
	}
	template<typename T>
	int Hook(const std::string& funcName, void* Address, T* Detour) {
		return Hook(funcName, Address, reinterpret_cast<void*>(Detour));
	}
};

inline std::unique_ptr<CHookHandler> g_HookHandler;