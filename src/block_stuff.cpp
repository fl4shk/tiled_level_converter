#include "block_stuff.hpp"

namespace block_stuff
{
	vector< pair< string, bool > > bt_name_and_has_persistent_data_vec
	= {
		// A graphics-less block that sprites are free to move through.
		{ "bt_air", false },
		
		// A block that looks like a wooden log.  It is indestructable,
		// which is quite unlike real life wooden logs.
		{ "bt_log", false },
		
		// A wooden block without anything inside.  It is destroyed (that
		// is, replaced with an air block) when strongly hit.
		{ "bt_wood", false },
		
		// A block with eyes without anything inside.  It becomes an empty
		// block upon being strongly hit.
		{ "bt_eyes", false },
		
		// A block with an exclamation point on it.  The purpose of these
		// blocks is yet to be chosen, but I'm thinking that some kind of
		// event will happen when one is strongly hit from below.
		{ "bt_exclam", true },
		
		// An empty block that is indestructible.  
		{ "bt_dud", false },
		
		// Wooden blocks with contents
		{ "bt_wood_with_bt_dud", true },
		{ "bt_wood_with_st_waffle", true },
		{ "bt_wood_with_st_muffin", true },
		{ "bt_wood_with_st_fire_muffin", true },
		{ "bt_wood_with_st_ice_muffin", true },
		{ "bt_wood_with_st_chocolate_muffin", true },
		
		// Eyes blocks with contents
		{ "bt_eyes_with_st_waffle", true },
		{ "bt_eyes_with_st_muffin", true },
		{ "bt_eyes_with_st_fire_muffin", true },
		{ "bt_eyes_with_st_ice_muffin", true },
		{ "bt_eyes_with_st_chocolate_muffin", true },
		
		
		// Grass/Dirt blocks
		{ "bt_grass_single", false }, 
		
		{ "bt_grass_top_left", false },
		{ "bt_grass_top_middle", false },
		{ "bt_grass_top_right", false },
		
		{ "bt_dirt_middle_left", false },
		{ "bt_dirt_middle_middle", false },
		{ "bt_dirt_middle_right", false },
		
		{ "bt_dirt_bottom_left", false },
		{ "bt_dirt_bottom_middle", false },
		{ "bt_dirt_bottom_right", false },
		
		{ "bt_grass_slope_45_degrees", false },
		{ "bt_grass_slope_135_degrees", false },
		
		{ "bt_grass_in_slope_left", false },
		{ "bt_grass_in_slope_right", false },
		
	};
	
	const string bt_unknown_str = string("bt_unknown");
}

