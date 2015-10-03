#ifndef tiled_object_class_stuff_hpp
#define tiled_object_class_stuff_hpp


#include "misc_includes.hpp"



// object's are intended to be treated as the Tiled analog of my own
// sprite_init_param_group, at least for Tiled objects that represent
// sprites in my game.
class tiled_object
{
public:		// variables
	
	// gid_raw doesn't tak... wait, what was this comment supposed to say?
	// I don't remember.  Horray for multi-tasking!
	u32 id, gid_raw, gid;
	
	bool hflip, vflip;
	
	vec2_u32 block_grid_pos;
	vec2<double> size_2d;
	
	
	// Tiled itself stores object positions based on the BOTTOM LEFT
	// corner, but my GBA game uses the TOP LEFT corner for positions.
	vec2_u32 real_block_grid_pos;
	
	// Tiled objects can have various sizes, but my GBA game doesn't
	// support such things.
	vec2_u32 real_size_2d;
	
	sprite_type the_sprite_type;
	
public:		// functions
	
	inline void debug_print()
	{
		cout << id << " " << gid_raw << ";  " 
			<< gid << " " << hflip << " " << vflip << ";  "
			<< block_grid_pos.x << " " << block_grid_pos.y << " "
			<< size_2d.x << " " << size_2d.y << ";  "
			<< real_block_grid_pos.x << " " << real_block_grid_pos.y << " "
			<< real_size_2d.x << " " << real_size_2d.y << ";  "
			<< get_st_name_debug(the_sprite_type) << endl;
		
		
	}
	
};


class tiled_objectgroup
{
public:		// variables
	
	string name;
	
	vector<tiled_object> object_vec;
	
	
public:		// functions
	
	inline tiled_objectgroup()
	{
	}
	
	tiled_objectgroup( xml_node<>* node );
	
	inline void debug_print()
	{
		cout << name << endl;
		
		for ( auto iter=object_vec.begin();
			iter!=object_vec.end();
			++iter )
		{
			iter->debug_print();
		}
		
		cout << endl;
	}
	
	
};



#endif		// tiled_object_class_stuff_hpp
