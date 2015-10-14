#ifndef tiled_sublevel_class_hpp
#define tiled_sublevel_class_hpp

#include "tiled_map_header_class.hpp"
#include "tiled_tileset_class.hpp"
#include "tiled_layer_class.hpp"
#include "tiled_object_class_stuff.hpp"

#include "sublevel_class.hpp"
#include "utility_funcs.hpp"

class tiled_sublevel
{
public:		// variables
	string output_dirname, output_basename;
	
	bool file_was_opened;
	
	string sublevel_str;
	
	xml_document<char> doc;
	
	// The header of the tilemap.
	tiled_map_header map_header;
	
	// A dynamic array of Tiled tilesets; it is likely that there will be
	// many tilesets in a sublevel.
	vector<tiled_tileset> tileset_vec;
	
	// A dynamic array of Tiled layers.
	vector<tiled_layer> layer_vec;
	
	// A dynamic array of Tiled objectgroups.
	vector<tiled_objectgroup> objectgroup_vec;
	
	
	// An std::map that maps object gid's to sprite_type's
	map< u32, sprite_type > gid_st_map;
	
	// An std::map that maps object gid's to tilesets 
	map< u32, tiled_tileset* > gid_tileset_ptr_map;
	
	// An std::map that maps st_warp_block ids to sublevel_entrance ids
	map< u32, u32 > warp_id_sle_id_map;
	
	
	// A 2D dynamic array of pointers to Tiled objects.  Only ONE Tiled
	// object is permitted per initial block coordinate.
	vector< vector<tiled_object*> > object_ptr_vec;
	
	
	sublevel* the_sublevel_ptr;
	
public:		// functions
	tiled_sublevel( const string& sublevel_file_name, 
		const string& s_output_prefix, sublevel& the_sublevel );
	
	
	void generate_the_sublevel();
	
	void debug_print();
	void debug_print_2();
	
	void debug_print_gid_st_map();
	
	void debug_print_gid_tileset_ptr_map();
	
	
protected:		// functions
	void build_gid_maps_and_correct_objects();
	
	void build_object_ptr_vec();
	void connect_warp_block_to_sublevel_entrance( tiled_object* obj_ptr,
		sprite_init_param_group& to_push );
	//void build_warp_block_extra_params( tiled_object* obj_ptr, 
	//	sprite_init_param_group& to_push );
	
	//void generate_the_sprite_ipg_vec_part_1();
	//void generate_the_sprite_ipg_vec_part_2();
	
	void write_uncompressed_block_data_to_file();
	void read_compressed_block_data_from_file();
	
	void generate_sublevel_header();
	void generate_sublevel_cpp_file();
	
	
	
	inline const string output_prefix() const
	{
		return output_dirname + "/" + output_basename;
	}
	
	
	
};


#endif		// tiled_sublevel_class_hpp
