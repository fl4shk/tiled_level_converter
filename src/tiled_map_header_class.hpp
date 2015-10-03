#ifndef tiled_map_header_class_hpp
#define tiled_map_header_class_hpp

#include "misc_includes.hpp"


class tiled_map_header
{
public:		// variables
	
	vec2_u32 size_2d, tile_size_2d;
	
	u32 nextobjectid;
	
public:		// functions
	inline tiled_map_header()
	{
	}
	
	tiled_map_header( xml_node<>* map_node );
	
	//tiled_map_header& operator = ( const tiled_map_header& to_copy );
	
	inline void debug_print()
	{
		cout << size_2d.x << " " << size_2d.y << " "
			<< tile_size_2d.x << " " << tile_size_2d.y << " "
			<< nextobjectid << endl;
	}
	
};



#endif		// tiled_map_header_class_hpp
