#ifndef sublevel_entrance_class_hpp
#define sublevel_entrance_class_hpp

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "vec2_class.hpp"

enum sublevel_entrance_type
{
	sle_start_of_level,
	
	sle_from_warp_block,
	
	// The number of sublevel entrance types is automatically updated by the
	// compiler via the le_count enum id
	sle_count,
	
} __attribute__((aligned(4)));


class sublevel_entrance
{
public:		// variables
	
	sublevel_entrance_type type;
	
	vec2_f24p8 in_sublevel_pos;
	
} __attribute__((aligned(4)));


extern vector<string> sle_name_vec;
extern const string sle_unknown_str;

inline const string& get_sle_name
	( sublevel_entrance_type the_sublevel_entrance_type )
{
	if ( the_sublevel_entrance_type < 0 
		|| the_sublevel_entrance_type >= sle_count )
	{
		cout << "Unknown sublevel_entrance_type!\n";
		return sle_name_vec.at(sle_start_of_level);
	}
	return sle_name_vec.at(the_sublevel_entrance_type);
}

inline const string& get_sle_name_debug
	( sublevel_entrance_type the_sublevel_entrance_type )
{
	if ( the_sublevel_entrance_type < 0 
		|| the_sublevel_entrance_type >= sle_count )
	{
		return sle_unknown_str;
	}
	return sle_name_vec.at(the_sublevel_entrance_type);
}


#endif		// sublevel_entrance_class_hpp
