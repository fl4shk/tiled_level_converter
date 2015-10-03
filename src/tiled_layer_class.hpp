#ifndef tiled_layer_class_hpp
#define tiled_layer_class_hpp

#include "misc_includes.hpp"

// tiled_layer's are intended to be used 
class tiled_layer
{
public:		// variables
	string name;
	
	vec2_u32 size_2d;
	
	string data_str;
	
	vector< vector<u32> > tile_ids;
	
public:		// functions
	
	inline tiled_layer()
	{
	}
	
	tiled_layer( xml_node<>* node );
	
	inline void debug_print()
	{
		cout << name << " " << size_2d.x << " " << size_2d.y << endl;
		
		for ( auto iter=tile_ids.begin(); iter!=tile_ids.end(); ++iter )
		{
			for ( auto iter2=iter->begin(); iter2!=iter->end(); ++iter2 )
			{
				if ( *iter2 != 0 )
				{
					cout << *iter2 << " ";
				}
				else
				{
					cout << "  ";
				}
			}
			cout << endl;
		}
	}
	
	inline void debug_print_2()
	{
		cout << name << " " << size_2d.x << " " << size_2d.y << endl;
		
		for ( auto iter=tile_ids.begin(); iter!=tile_ids.end(); ++iter )
		{
			for ( auto iter2=iter->begin(); iter2!=iter->end(); ++iter2 )
			{
				cout << *iter2 << " ";
			}
			cout << endl;
		}
	}
	
	
	
};


#endif		// tiled_layer_class_hpp
