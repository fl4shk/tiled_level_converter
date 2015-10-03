#ifndef tiled_level_class_hpp
#define tiled_level_class_hpp

#include "tiled_map_header_class.hpp"
#include "tiled_tileset_class.hpp"
#include "tiled_layer_class.hpp"
#include "tiled_object_class_stuff.hpp"

#include "level_class.hpp"
#include "utility_funcs.hpp"

class tiled_level
{
public:		// variables
	string output_dirname, output_basename;
	
	bool file_was_opened;
	
	string level_str;
	
	xml_document<char> doc;
	
	// The header of the tilemap.
	tiled_map_header map_header;
	
	// A dynamic array of Tiled tilesets; it is likely that there will be
	// many tilesets in a level.
	vector<tiled_tileset> tileset_vec;
	
	// A dynamic array of Tiled layers.
	vector<tiled_layer> layer_vec;
	
	// A dynamic array of Tiled objectgroups.
	vector<tiled_objectgroup> objectgroup_vec;
	
	
	// An std::map that maps object gid's to sprite_type's
	map< u32, sprite_type > gid_st_map;
	
	// An std::map that maps object gid's to tilesets 
	map< u32, tiled_tileset* > gid_tileset_ptr_map;
	
	
	// A 2D dynamic array of pointers to Tiled objects.  Only ONE Tiled
	// object is permitted per initial block coordinate.
	vector< vector<tiled_object*> > object_ptr_vec;
	
	
	level* the_level_ptr;
	
public:		// functions
	tiled_level( const string& level_file_name, 
		const string& s_output_prefix, level& the_level );
	
	
	void generate_the_level();
	
	void debug_print();
	void debug_print_2();
	
	void debug_print_gid_st_map();
	
	void debug_print_gid_tileset_ptr_map();
	
	
protected:		// functions
	void build_gid_maps_and_correct_objects();
	
	void build_object_ptr_vec();
	
	void write_uncompressed_block_data_to_file();
	void read_compressed_block_data_from_file();
	
	void generate_level_header();
	void generate_level_cpp_file();
	
	
	
	inline const string output_prefix() const
	{
		return output_dirname + "/" + output_basename;
	}
	
	
	
};


#endif		// tiled_level_class_hpp
