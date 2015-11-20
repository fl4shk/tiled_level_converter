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
	string sublevel_file_name, output_dirname, output_basename;
	
	bool file_was_opened;
	
	string sublevel_str_raw, sublevel_str;
	
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
	
	// An std::map that maps st_door ids to sublevel_entrance ids.
	// This is deprecated.
	map< u32, u32 > warp_id_sle_id_map;
	
	
	// An std::map that maps this tiled_sublevel's st_door id's to
	// their bdest (block destination) values
	map< u32, u32 > warp_id_bdest_map;
	
	// An std::map that maps this tiled_sublevel's st_door id's to
	// their sdest (sublevel destination) values, using a value of -1 if
	// the st_door is linked to another st_door in its own
	// sublevel.
	map< u32, s64 > warp_id_sdest_map;
	
	// A 2D dynamic array of pointers to Tiled objects.  Only ONE Tiled
	// object is permitted per initial block coordinate.
	vector< vector<tiled_object*> > object_ptr_vec;
	
	
	// A vector of pointers to tiled_object's that correspond to instances
	// of st_door sprites.
	vector< pair< tiled_object*, u32 > >
		door_object_ptr_and_sprite_ipg_index_vec;
	
	
	sublevel the_sublevel;
	
	bool the_sublevel_was_generated;
	
	
public:		// functions
	tiled_sublevel( const string& s_sublevel_file_name, 
		const string& s_output_prefix );
	
	inline tiled_sublevel( const string& s_sublevel_file_name, 
		const string& s_output_dirname, const string& s_output_basename )
		: tiled_sublevel( s_sublevel_file_name, s_output_dirname + "/" 
			+ s_output_basename )
	{
	}
	
	inline tiled_sublevel( const tiled_sublevel& to_copy )
	{
		*this = to_copy;
		//cout << layer_vec.size() << endl;
	}
	
	tiled_sublevel& operator = ( const tiled_sublevel& to_copy );
	
	inline const string& get_sublevel_file_name() const
	{
		return sublevel_file_name;
	}
	inline const string& get_sublevel_str_raw() const
	{
		return sublevel_str_raw;
	}
	
	void common_initialization_stuff();
	
	void generate_the_sublevel();
	void generate_sublevel_header_old();
	void generate_sublevel_cpp_file_old();
	
	
	void debug_print();
	void debug_print_2();
	
	void debug_print_gid_st_map();
	
	void debug_print_gid_tileset_ptr_map();
	
	inline const string output_prefix() const
	{
		return output_dirname + "/" + output_basename;
	}
	
protected:		// functions
	
	void build_gid_maps_and_correct_objects();
	
	void build_object_ptr_vec();
	//void connect_door_to_sublevel_entrance( tiled_object* obj_ptr,
	//	sprite_init_param_group& to_push );
	void generate_sublevel_entrance_using_door_old
		( tiled_object* obj_ptr, sprite_init_param_group& to_push );
	void connect_doors_to_sublevel_entrances_old();
	
	//void generate_warp_id_bdest_map_and_warp_id_sdest_map();
	void generate_warp_id_bdest_and_sdest_stuff();
	void generate_door_sublevel_entrances();
	
	void generate_start_of_level_sle_using_st_player
		( tiled_object* obj_ptr );
	
	//void build_door_extra_params( tiled_object* obj_ptr, 
	//	sprite_init_param_group& to_push );
	
	//void generate_the_sprite_ipg_vec_part_1();
	//void generate_the_sprite_ipg_vec_part_2();
	
	void write_uncompressed_block_data_to_file();
	void read_compressed_block_data_from_file();
	
	
	
	
	
};


#endif		// tiled_sublevel_class_hpp
