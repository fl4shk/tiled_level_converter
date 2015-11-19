#ifndef sprite_type_stuff_hpp
#define sprite_type_stuff_hpp


#include "misc_types.hpp"
#include "vec2_class.hpp"

#include <vector>
#include <string>
using namespace std;


// Adding, removing, or changing sprite types
enum sprite_type
{
	// The Default Sprite (also used for when there is no sprite in the
	// slot).
	st_default,
	
	// The Player
	st_player,
	
	// Powerup Sprites
	st_waffle,
	
	st_muffin,
	st_fire_muffin,
	st_ice_muffin,
	st_chocolate_muffin,
	
	// Block-like Sprites
	
	// st_warp_block_# is used for warping around different parts of either
	// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
	// sublevel_entrance's during the whole level conversion process.
	st_warp_block_0, st_warp_block_1, st_warp_block_2, st_warp_block_3,
	st_warp_block_4, st_warp_block_5, st_warp_block_6, st_warp_block_7,
	st_warp_block_8, st_warp_block_9, st_warp_block_10, st_warp_block_11,
	st_warp_block_12, st_warp_block_13, st_warp_block_14, st_warp_block_15,
	st_warp_block_16, st_warp_block_17, st_warp_block_18, st_warp_block_19,
	st_warp_block_20, st_warp_block_21, st_warp_block_22, st_warp_block_23,
	st_warp_block_24, st_warp_block_25, st_warp_block_26, st_warp_block_27,
	st_warp_block_28, st_warp_block_29, st_warp_block_30, st_warp_block_31,
	
	// st_count is the amount of sprite types.  It is automatically updated
	// by the compiler.
	st_count,
	
} __attribute__((aligned(4)));

static constexpr sprite_type lowest_warp_id_st = st_warp_block_0,
	highest_warp_id_st = st_warp_block_31;



extern vector<string> st_name_vec;
extern const string st_unknown_str;

inline const string& get_st_name( sprite_type the_sprite_type )
{
	if ( the_sprite_type < 0 || the_sprite_type >= st_count )
	{
		return st_name_vec.at(st_default);
	}
	return st_name_vec.at(the_sprite_type);
}

inline const string& get_st_name_debug( sprite_type the_sprite_type )
{
	if ( the_sprite_type < 0 || the_sprite_type >= st_count )
	{
		return st_unknown_str;
	}
	return st_name_vec.at(the_sprite_type);
}

#endif		// sprite_type_stuff_hpp
