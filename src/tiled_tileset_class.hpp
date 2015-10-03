#ifndef tiled_tileset_class_hpp
#define tiled_tileset_class_hpp

#include "misc_includes.hpp"


class tiled_tileset
{
public:		// variables
	u32 firstgid;
	string name;
	
	vec2_u32 tile_size_2d;
	u32 tilecount;
	
public:		// functions
	inline tiled_tileset()
	{
	}
	
	tiled_tileset( xml_node<>* node );
	
	inline u32 lastgid() const
	{
		return firstgid + tilecount - 1;
	}
	
	inline void debug_print()
	{
		cout << firstgid << " " << name << " " 
			<< tile_size_2d.x << " " << tile_size_2d.y << " "
			<< tilecount << endl;
	}
	
};


#endif		// tiled_tileset_class_hpp
