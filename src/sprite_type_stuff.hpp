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
	
	
	// st_count is the amount of sprite types.  It is automatically updated
	// by the compiler.
	st_count,
	
} __attribute__((aligned(4)));




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
