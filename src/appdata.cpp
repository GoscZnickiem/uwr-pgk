#include "appdata.hpp"

AppData& AppData::data() {
	static AppData data;
	return data;
}
