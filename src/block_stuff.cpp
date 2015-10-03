#include "block_stuff.hpp"

namespace block_stuff
{
	vector< pair< string, bool > > bt_name_and_has_persistent_data_vec
	= {
		{ "bt_air", false },
		{ "bt_log", false },
		{ "bt_wood", false },
		{ "bt_qmark", true },
		{ "bt_exclam", true },
		{ "bt_dud", false },
	};
	
	const string bt_unknown_str = string("bt_unknown");
}

