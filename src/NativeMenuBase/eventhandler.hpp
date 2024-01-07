#include "common.hpp"
#include "script.h"

struct EventHandler {
	std::function<void(int scriptindex)> callback;
	bool* enabled;
};

class CEventHandler {
	std::unordered_map<eEventType, std::vector<EventHandler>> m_Registry;

public:
	void Subscribe(eEventType eventtype, const std::string& name, bool* enableswitch, std::function<void(int scriptindex)> handler) {
		m_Registry[eventtype].push_back({ handler, enableswitch });
	}
	//void UnSubscribe(eEventType eventtype, const std::string& name) {
	//	auto& Handlers = m_Registry[eventtype];
	//	for (auto it = Handlers.begin(); it != Handlers.end(); it++) {
	//		if (it->name == name) Handlers.erase(it);
	//	}
	//}

	void Tick() {
		int EventNum = SCRIPTS::GET_NUMBER_OF_EVENTS(0);
		for (int i = 0; i < EventNum; i++) {
			Hash Event = SCRIPTS::GET_EVENT_AT_INDEX(0, i);
			for (auto&& Handler : m_Registry[(eEventType)Event]) {
				if (*Handler.enabled)Handler.callback(i);
			}

			switch (Event) {
			case eEventType::EVENT_VEHICLE_CREATED:
			case eEventType::EVENT_VEHICLE_DESTROYED:
			case eEventType::EVENT_PED_CREATED:
			case eEventType::EVENT_PED_DESTROYED:
			case eEventType::EVENT_SHOCKING_ITEM_STOLEN:
				break;
			default:
				PRINT_INFO("EventLogger| EVENT:", Event, " NAME:", EventHashToString(Event));
			}
		}

		for (auto&& Handler : m_Registry[eEventType::EVENT_TICK]) {
			if (*Handler.enabled) Handler.callback(0);
		}
	}
};

inline std::unique_ptr<CEventHandler> g_EventHandler;