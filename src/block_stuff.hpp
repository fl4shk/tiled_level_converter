#ifndef block_stuff_hpp
#define block_stuff_hpp

#include "misc_types.hpp"

#include <vector>
#include <string>
#include <utility>
using namespace std;

enum block_type
{
	// A graphics-less block that sprites are free to move through.
	bt_air,
	
	// A block that looks like a wooden log.  It is indestructible, which
	// is quite unlike real life wooden logs.
	bt_log,
	
	// A wooden block without anything inside.  It is destroyed (that is,
	// replaced with an air block) when strongly hit.
	bt_wood,
	
	// A block with eyes without anything inside.  It becomes an empty
	// block upon being strongly hit.
	bt_eyes,
	
	// A block with an exclamation point on it.  The purpose of these
	// blocks is yet to be chosen, but I'm thinking that some kind of event
	// will happen when one is strongly hit.
	bt_exclam,
	
	// An empty block that is indestructible.  
	bt_dud,
	
	// Wooden blocks with contents
	bt_wood_with_bt_dud,
	bt_wood_with_st_waffle,
	bt_wood_with_st_muffin,
	bt_wood_with_st_fire_muffin,
	bt_wood_with_st_ice_muffin,
	bt_wood_with_st_chocolate_muffin,
	
	// Eyes blocks with contents
	//bt_eyes_with_bt_dud		// This is the same as bt_eyes.
	bt_eyes_with_st_waffle,
	bt_eyes_with_st_muffin,
	bt_eyes_with_st_fire_muffin,
	bt_eyes_with_st_ice_muffin,
	bt_eyes_with_st_chocolate_muffin,
	
	
	// Grass/Dirt blocks
	bt_grass_single,
	
	bt_grass_top_left,
	bt_grass_top_middle,
	bt_grass_top_right,
	
	bt_dirt_middle_left,
	bt_dirt_middle_middle,
	bt_dirt_middle_right,
	
	bt_dirt_bottom_left,
	bt_dirt_bottom_middle,
	bt_dirt_bottom_right,
	
	bt_grass_slope_135_degrees,
	bt_grass_slope_45_degrees,
	
	bt_grass_in_slope_left,
	bt_grass_in_slope_right,
	
	
	// bt_count is the amount of block types.  It is automatically updated
	// by the compiler.
	bt_count,
	
} __attribute__((aligned(4)));



// A class for unique block data.
class block
{
public:		// variables
	
	u16 type;
	u16 persistent_data_index;
	
public:		// functions
	
	inline block_type get_block_type() const
	{
		return (block_type)type;
	}
	
} __attribute__((aligned(4)));


namespace block_stuff
{
	extern vector< pair< string, bool > > 
		bt_name_and_has_persistent_data_vec;
	
	extern const string bt_unknown_str;
	
	constexpr u32 persistent_block_data_array_size = 2048;
	
	inline const string& get_bt_name( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return bt_name_and_has_persistent_data_vec.at(bt_air).first;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type).first;
	}
	
	inline const string& get_bt_name_debug( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return bt_unknown_str;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type)
			.first;
	}
	 
	inline bool bt_has_persistent_data( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return false;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type)
			.second;
	}
}


#endif		// block_stuff_hpp
