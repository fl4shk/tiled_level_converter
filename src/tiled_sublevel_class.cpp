#include "tiled_sublevel_class.hpp"



tiled_sublevel::tiled_sublevel( const string& s_sublevel_file_name,
	const string& s_output_prefix ) 
	: sublevel_file_name(s_sublevel_file_name)
{
	output_dirname.clear();
	output_basename.clear();
	for ( const char& the_char : s_output_prefix )
	{
		output_dirname += the_char;
		output_basename += the_char;
	}
	
	output_dirname = dirname( const_cast<char*>
		( output_dirname.c_str() ) );
	output_basename = basename( const_cast<char*>
		( output_basename.c_str() ) );
	
	fstream sublevel_file( s_sublevel_file_name, ios_base::in );
	
	if ( !sublevel_file.is_open() )
	{
		file_was_opened = false;
		return;
	}
	
	file_was_opened = true;
	
	// Read the whole file into sublevel_str
	getline( sublevel_file, sublevel_str_raw, '\0' );
	
	sublevel_file.close();
	
	
	sublevel_str.clear();
	for ( const char& the_char : sublevel_str_raw )
	{
		sublevel_str += the_char;
	}
	
	common_initialization_stuff();
	
	the_sublevel_was_generated = false;
}

tiled_sublevel& tiled_sublevel::operator = ( const tiled_sublevel& to_copy )
{
	sublevel_file_name = to_copy.sublevel_file_name;
	output_dirname = to_copy.output_dirname;
	output_basename = to_copy.output_basename;
	file_was_opened = to_copy.file_was_opened;
	
	
	sublevel_str_raw.clear();
	sublevel_str.clear();
	for ( const char& the_char : to_copy.sublevel_str_raw )
	{
		sublevel_str_raw += the_char;
		sublevel_str += the_char;
	}
	
	common_initialization_stuff();
	
	if ( to_copy.the_sublevel_was_generated )
	{
		generate_the_sublevel();
	}
	
	return *this;
}

void tiled_sublevel::common_initialization_stuff()
{
	doc.parse<0>( const_cast<char*>( sublevel_str.c_str() ) );
	
	map_header = tiled_map_header( doc.first_node() );
	
	for ( auto node=doc.first_node()->first_node();
		node;
		node=node->next_sibling() )
	{
		
		if ( node->name() == string("tileset") )
		{
			tileset_vec.push_back( tiled_tileset(node) );
		}
		else if ( node->name() == string("layer") )
		{
			layer_vec.push_back( tiled_layer(node) );
		}
		else if ( node->name() == string("objectgroup") )
		{
			objectgroup_vec.push_back( tiled_objectgroup(node) );
		}
	}
	
	build_gid_maps_and_correct_objects();
	
	build_object_ptr_vec();
	
	for ( u32 i=0; i<highest_warp_id_st - lowest_warp_id_st + 1; ++i )
	{
		the_sublevel.sublevel_entrance_vec.push_back
			(sublevel_entrance());
	}
	
	//cout << the_sublevel.sublevel_entrance_vec.size() << endl;
	
}


void tiled_sublevel::generate_the_sublevel()
{
	// General error detection and response (if necessary).
	if ( layer_vec.size() != 1 )
	{
		cout << "Error!  EXACTLY ONE Tiled tile layer is required!\n";
		cout << layer_vec.size() << endl;
		exit(1);
	}
	
	if ( layer_vec[0].size_2d.x > sublevel::max_size_2d.x )
	{
		cout << "Error!  Levels are not permitted to have a width of "
			<< "more than " << sublevel::max_size_2d.x << "blocks!  "
			<< "Exiting....\n";
		exit(1);
	}
	
	if ( layer_vec[0].size_2d.y != sublevel::max_size_2d.y )
	{
		cout << "Error!  Levels are required to have a height of EXACTLY " 
			<< sublevel::max_size_2d.y << " blocks!  "
			<< "Exiting....\n";
		exit(1);
	}
	
	the_sublevel.real_size_2d = layer_vec[0].size_2d;
	
	// Generate the sublevel's block data (both compressed and
	// uncompressed)
	u32 next_persistent_block_data_index = 1;
	
	for ( u32 j=0; j<layer_vec[0].tile_ids.size(); ++j )
	{
		vector<u32>& layer_col = layer_vec[0].tile_ids[j];
		vector<block>& bd_col = the_sublevel
			.uncompressed_block_data_vec[j];
		
		
		for ( u32 i=0; i<layer_col.size(); ++i )
		{
			if ( layer_col[i] < 0 )
			{
				bd_col[i] = block();
			}
			else if ( layer_col[i] > bt_count )
			{
				bd_col[i].type = layer_col[i];
				bd_col[i].persistent_data_index = 0;
			}
			else
			{
				bd_col[i].type = layer_col[i];
				
				if ( block_stuff::bt_has_persistent_data
					( static_cast<block_type>(layer_col[i]) ) )
				{
					if ( next_persistent_block_data_index
						>= block_stuff::persistent_block_data_array_size )
					{
						cout << "Error!  Too many blocks have persistent "
							<< "data!  Exiting....\n";
						
						exit(1);
					}
					
					bd_col[i].persistent_data_index 
						= next_persistent_block_data_index;
					
					++next_persistent_block_data_index;
				}
			}
		}
	}
	
	
	bool found_st_player_tiled_object = false;
	
	// Generate the sublevel's vector of sprite_init_param_group's
	for ( u32 j=0; j<object_ptr_vec.size(); ++j )
	{
		for ( u32 i=0; i<object_ptr_vec[j].size(); ++i )
		{
			tiled_object* obj_ptr = object_ptr_vec[j][i];
			if ( obj_ptr != NULL )
			{
				if ( obj_ptr->the_sprite_type == st_player )
				{
					if (found_st_player_tiled_object)
					{
						cout << "Warning!  More than one st_player "
							<< "tiled_object per sublevel is not "
							<< "permitted!\n";
						obj_ptr->reject();
						continue;
					}
					
					found_st_player_tiled_object = true;
					generate_start_of_level_sle_using_st_player(obj_ptr);
					continue;
				}
				
				sprite_init_param_group to_push;
				to_push.type = obj_ptr->the_sprite_type;
				to_push.initial_block_grid_x_coord 
					= obj_ptr->real_block_grid_pos.x;
				to_push.initial_block_grid_y_coord
					= obj_ptr->real_block_grid_pos.y;
				to_push.facing_right = ( obj_ptr->hflip ) ? false : true;
				
				to_push.extra_param_0 = to_push.extra_param_1 
					= to_push.extra_param_2 = to_push.extra_param_3 = 0;
				
				the_sublevel.sprite_ipg_vec.push_back(to_push);
				
				// If dealing with an st_warp_block, then add its
				// tiled_object pointer and an index to the array of 
				// sprite_init_param_groups to 
				// warp_block_object_ptr_and_sprite_ipg_index_vec.
				if ( obj_ptr->the_sprite_type >= lowest_warp_id_st 
					&& obj_ptr->the_sprite_type <= highest_warp_id_st )
				{
					//generate_sublevel_entrance_using_warp_block_old
					//	( obj_ptr, to_push );
					//generate_sublevel_entrance_using_warp_block( obj_ptr, 
					//	to_push );
					//cout << to_push.type - lowest_warp_id_st << endl;
					//warp_block_object_ptr_vec.push_back(obj_ptr);
					
					warp_block_object_ptr_and_sprite_ipg_index_vec
						.push_back( pair< tiled_object*, u32 >( obj_ptr, 
						the_sublevel.sprite_ipg_vec.size() - 1  ) );
				}
				
			}
		}
	}
	
	
	//cout << "\n\n";
	//
	//for ( u32 i=0; i<warp_block_object_ptr_and_sprite_ipg_ptr_vec.size();
	//	++i )
	//{
	//	cout <<
	//	warp_block_object_ptr_and_sprite_ipg_ptr_vec[i].second->type <<
	//	endl;
	//}
	
	////connect_warp_blocks_to_sublevel_entrances_old();
	//generate_warp_id_bdest_map_and_warp_id_sdest_map();
	generate_warp_id_bdest_and_sdest_stuff();
	generate_warp_block_sublevel_entrances();
	
	
	write_uncompressed_block_data_to_file();
	
	// Run gbalzss
	const string gbalzss_command = "gbalzss e "
		+ get_uncompressed_block_data_file_name( output_dirname,
			output_basename ) + " "
		+ get_compressed_block_data_file_name( output_dirname,
			output_basename );
	
	cout << "Running this command:  " << gbalzss_command << endl;
	if ( system(gbalzss_command.c_str()) != 0 )
	{
		cout << "There was an error running gbalzss!  Exiting....\n";
		exit(1);
	}
	
	read_compressed_block_data_from_file();
	
	// Remove the temporary files
	const string clean_up_command = "rm " 
		+ get_uncompressed_block_data_file_name( output_dirname, 
			output_basename ) + " "
		+ get_compressed_block_data_file_name( output_dirname,
			output_basename );
	
	cout << "Running this command:  " << clean_up_command << endl;
	system(clean_up_command.c_str());
	
	the_sublevel_was_generated = true;
}


void tiled_sublevel::write_uncompressed_block_data_to_file()
{
	string output_file_name = get_uncompressed_block_data_file_name
		( output_dirname, output_basename );
	//cout << output_file_name << endl;
	
	fstream output_file( output_file_name, ios_base::out 
		| ios_base::binary );
	
	if ( !output_file.is_open() )
	{
		cout << "There was an error opening \"" << output_file_name 
			<< "\"!\n";
		
		exit(1);
	}
	
	for ( u32 j=0; 
		j<the_sublevel.uncompressed_block_data_vec.size(); 
		++j )
	{
		vector<block>& bd_col = the_sublevel.uncompressed_block_data_vec[j];
		
		for ( u32 i=0; i<bd_col.size(); ++i )
		{
			output_file.write( reinterpret_cast<char*>(&bd_col[i]), 
				sizeof(bd_col[i]) );
		}
	}
	
	output_file.close();
}

void tiled_sublevel::read_compressed_block_data_from_file()
{
	string input_file_name = get_compressed_block_data_file_name
		( output_dirname, output_basename );
	
	fstream input_file( input_file_name, ios_base::in | ios_base::binary );
	
	if ( !input_file.is_open() )
	{
		cout << "There was an error opening \"" << input_file_name
			<< "\"!\n";
		
		exit(1);
	}
	
	vector<u8> compressed_block_data_as_u8s;
	
	while ( !input_file.eof() )
	{
		int get_result = input_file.get();
		
		if ( get_result == -1 )
		{
			break;
		}
		
		u8 byte = get_result;
		compressed_block_data_as_u8s.push_back(byte);
	}
	
	input_file.close();
	
	if ( compressed_block_data_as_u8s.size() % 4 != 0 )
	{
		cout << "Error:  compressed_block_data_as_u8s.size() isn't "
			<< "divisible by 4!\n";
		exit(1);
	}
	
	// Construct the std::vector<u32> of compressed block data
	for ( u32 i=0; i<compressed_block_data_as_u8s.size(); i+=4 )
	{
		u32 the_u32 = ( compressed_block_data_as_u8s.at( i + 3 ) << 24 )
			| ( compressed_block_data_as_u8s.at( i + 2 ) << 16 )
			| ( compressed_block_data_as_u8s.at( i + 1 ) << 8 )
			| compressed_block_data_as_u8s.at( i + 0 );
		
		the_sublevel.compressed_block_data_vec.push_back(the_u32);
	}
	
}

void tiled_sublevel::generate_sublevel_header_old()
{
	fstream header_file( output_prefix() + string(".hpp"), ios_base::out );
	
	header_file << "#ifndef " << output_basename << "_hpp\n"
		<< "#define " << output_basename << "_hpp\n\n\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "sublevel_class.hpp\"\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "sprite_level_data_stuff.hpp\"\n";
	
	header_file << "\n\n";
	
	header_file << "extern const sublevel< "
		<< the_sublevel.compressed_block_data_vec.size() << ", "
		<< the_sublevel.real_size_2d.x << ", " 
		<< the_sublevel.real_size_2d.y << ", "
		<< the_sublevel.sprite_ipg_vec.size() << ", "
		<< the_sublevel.sublevel_entrance_vec.size() << " > "
		<< output_basename << ";\n\n";
	
	header_file << "\n";
	
	header_file << "#endif\t\t// " << output_basename << "_hpp";
	
}

void tiled_sublevel::generate_sublevel_cpp_file_old()
{
	fstream cpp_file( output_prefix() + string(".thumb.cpp"), 
		ios_base::out );
	
	cpp_file << "#include \"" << output_basename << ".hpp\"\n\n\n";
	
	cpp_file << "const sublevel< "
		<< the_sublevel.compressed_block_data_vec.size() << ", "
		<< the_sublevel.real_size_2d.x << ", " 
		<< the_sublevel.real_size_2d.y << ", "
		<< the_sublevel.sprite_ipg_vec.size() << ", "
		<< the_sublevel.sublevel_entrance_vec.size () << " > "
		<< output_basename << "\n"
		<< "= {\n";
	
	
	// Build the array of compressed block data
	cpp_file << "\t{\n";
	
	u32 counter = 0;
	
	//for ( auto iter=the_sublevel.compressed_block_data_vec.begin();
	//	iter!=the_sublevel.compressed_block_data_vec.end();
	//	++iter )
	for ( u32& the_u32 : the_sublevel.compressed_block_data_vec )
	{
		////cpp_file << hex << "\t\t0x" << *iter << dec << ",\n";
		//cpp_file << hex << "\t\t0x" << the_u32 << dec << ",\n";
		if ( counter % 4 == 0 )
		{
			cpp_file << "\t\t";
		}
		
		//cpp_file << hex << "0x" << *iter << dec;
		cpp_file << hex << "0x" << the_u32 << dec;
		
		if ( counter % 4 == 3 )
		{
			cpp_file << ",\n";
		}
		else
		{
			cpp_file << ", ";
		}
		
		++counter;
	}
	
	cpp_file << "\n\t},\n\t\n";
	
	
	// Build the array of sprite_init_param_group's
	cpp_file << "\t{\n";
	
	//for ( auto iter=the_sublevel.sprite_ipg_vec.begin();
	//	iter!=the_sublevel.sprite_ipg_vec.end();
	//	++iter )
	for ( sprite_init_param_group& sprite_ipg 
		: the_sublevel.sprite_ipg_vec )
	{
		cpp_file << "\t\t{ ";
		
		//if ( iter->type < st_name_vec.size() )
		//{
		//	cpp_file << get_st_name(iter->type);
		//}
		if ( sprite_ipg.type < st_name_vec.size() )
		{
			cpp_file << get_st_name(sprite_ipg.type);
		}
		else
		{
			// Print out a number if the_sprite_type doesn't have a
			// corresponding string in st_name_vec.
			//cpp_file << iter->type;
			cpp_file << sprite_ipg.type;
		}
		
		//cpp_file << ", " << iter->initial_block_grid_x_coord << ", "
		//	<< iter->initial_block_grid_y_coord << ", " 
		//	<< ( iter->facing_right ? "true" : "false" );
		cpp_file << ", " << sprite_ipg.initial_block_grid_x_coord << ", "
			<< sprite_ipg.initial_block_grid_y_coord << ", " 
			<< ( sprite_ipg.facing_right ? "true" : "false" );
		
		
		
		//// This had to be changed to make the Sherwin's Adventure code
		//// compile with the OUTDATED version of arm-none-eabi- prefixed GCC
		//// and Binutils that are included in devkitARM
		////// Don't clutter the sprite_init_param_group's that have only
		////// zeroes as extra parameters.
		////if ( !( iter->extra_param_0 == 0 && iter->extra_param_1 == 0
		////	&& iter->extra_param_2 == 0 && iter->extra_param_3 == 0 ) )
		////{
		////	cpp_file << ", " << iter->extra_param_0 << ", " 
		////		<< iter->extra_param_1 << ", " 
		////		<< iter->extra_param_2 << ", " 
		////		<< iter->extra_param_3;
		////}
		//
		//cpp_file << ", " << iter->extra_param_0 << ", " 
		//	<< iter->extra_param_1 << ", " 
		//	<< iter->extra_param_2 << ", " 
		//	<< iter->extra_param_3;
		
		// This had to be changed to make the Sherwin's Adventure code
		// compile with the OUTDATED version of arm-none-eabi- prefixed GCC
		// and Binutils that are included in devkitARM
		//// Don't clutter the sprite_init_param_group's that have only
		//// zeroes as extra parameters.
		//if ( !( sprite_ipg.extra_param_0 == 0 && sprite_ipg.extra_param_1 == 0
		//	&& sprite_ipg.extra_param_2 == 0 && sprite_ipg.extra_param_3 == 0 ) )
		//{
		//	cpp_file << ", " << sprite_ipg.extra_param_0 << ", " 
		//		<< sprite_ipg.extra_param_1 << ", " 
		//		<< sprite_ipg.extra_param_2 << ", " 
		//		<< sprite_ipg.extra_param_3;
		//}
		
		cpp_file << ", " << sprite_ipg.extra_param_0 << ", " 
			<< sprite_ipg.extra_param_1 << ", " 
			<< sprite_ipg.extra_param_2 << ", " 
			<< sprite_ipg.extra_param_3;
		
		cpp_file << " },\n";
	}
	
	cpp_file << "\t},\n\t\n";
	
	// Build the array of sublevel_entrance's
	cpp_file << "\t{\n";
	
	for ( sublevel_entrance& the_sle : the_sublevel.sublevel_entrance_vec )
	{
		//cpp_file << "\t\t{ " << get_sle_name_debug(the_sle.type)
		//	<< hex << ", { 0x" << the_sle.in_sublevel_pos.x.data << ", 0x"
		//	<< the_sle.in_sublevel_pos.y.data << " } },\n" << dec;
		cpp_file << "\t\t{ " << get_sle_name_debug(the_sle.type)
			<< hex << ", vec2_f24p8( {0x" << the_sle.in_sublevel_pos.x.data
			<< "}, {0x" << the_sle.in_sublevel_pos.y.data << "} ) },\n" 
			<< dec;
	}
	
	cpp_file << "\t},\n";
	
	
	cpp_file << "};\n\n\n";
}

void tiled_sublevel::build_gid_maps_and_correct_objects()
{
	// Build the maps
	//for ( auto iter=tileset_vec.begin();
	//	iter!=tileset_vec.end();
	//	++iter )
	for ( tiled_tileset& the_tileset : tileset_vec )
	{
		//for ( u32 i=iter->firstgid; i<iter->firstgid+iter->tilecount; ++i )
		//{
		//	gid_st_map[i] = (sprite_type)( i - iter->firstgid );
		//	gid_tileset_ptr_map[i] = &(*iter);
		//}
		for ( u32 i=the_tileset.firstgid; 
			i<the_tileset.firstgid+the_tileset.tilecount; 
			++i )
		{
			gid_st_map[i] = (sprite_type)( i - the_tileset.firstgid );
			gid_tileset_ptr_map[i] = &the_tileset;
		}
	}
	
	//debug_print_gid_st_map();
	//cout << endl;
	//debug_print_gid_tileset_ptr_map();
	//cout << endl;
	
	
	// Correct the object positions and sizes, and also set the_sprite_type
	// of each object to the correct value.
	//for ( auto og_iter=objectgroup_vec.begin();
	//	og_iter!=objectgroup_vec.end();
	//	++og_iter )
	for ( tiled_objectgroup& the_objectgroup : objectgroup_vec )
	{
		//for ( auto obj_iter=og_iter->object_vec.begin();
		//	obj_iter!=og_iter->object_vec.end();
		//	++obj_iter )
		for ( tiled_object& the_object : the_objectgroup.object_vec )
		{
			//tiled_tileset* tileset = gid_tileset_ptr_map.at(obj_iter->gid);
			tiled_tileset* tileset = gid_tileset_ptr_map.at
				(the_object.gid);
			
			
			// Tiled objects can have various sizes, but my GBA game
			// doesn't support such things.
			//obj_iter->real_size_2d = tileset->tile_size_2d;
			the_object.real_size_2d = tileset->tile_size_2d;
			
			
			//obj_iter->real_block_grid_pos.x = obj_iter->block_grid_pos.x;
			the_object.real_block_grid_pos.x = the_object.block_grid_pos.x;
			
			// Tiled itself stores object positions based on the BOTTOM
			// LEFT corner, but my GBA game uses the TOP LEFT corner for
			// positions.
			//obj_iter->real_block_grid_pos.y = obj_iter->block_grid_pos.y
			//	 - ( obj_iter->real_size_2d.y >> 4 );
			the_object.real_block_grid_pos.y = the_object.block_grid_pos.y
				 - ( the_object.real_size_2d.y >> 4 );
			
			
			// Set the_sprite_type
			//obj_iter->the_sprite_type = gid_st_map.at(obj_iter->gid);
			the_object.the_sprite_type = gid_st_map.at(the_object.gid);
			
		}
	}
	
}


void tiled_sublevel::build_object_ptr_vec()
{
	
	// Allocate the vertical stuff.
	object_ptr_vec.resize(map_header.size_2d.y);
	
	// Allocate the horizontal stuff.
	for ( u32 j=0; j<map_header.size_2d.y; ++j )
	{
		object_ptr_vec[j].resize(map_header.size_2d.x);
	}
	
	// object_ptr_vec contains only NULL pointers by default.
	
	
	// Build the 2D dynamic array of pointers to objects
	//for ( auto og_iter=objectgroup_vec.begin();
	//	og_iter!=objectgroup_vec.end();
	//	++og_iter )
	for ( tiled_objectgroup& the_objectgroup : objectgroup_vec )
	{
		//for ( auto obj_iter=og_iter->object_vec.begin();
		//	obj_iter!=og_iter->object_vec.end();
		//	++obj_iter )
		for ( tiled_object& the_object : the_objectgroup.object_vec )
		{
			//vec2_u32& rbg_pos = obj_iter->real_block_grid_pos;
			vec2_u32& rbg_pos = the_object.real_block_grid_pos;
			
			// Only one Tiled object is permitted per block grid coordinate
			if ( object_ptr_vec[rbg_pos.y][rbg_pos.x] == NULL )
			{
				//object_ptr_vec[rbg_pos.y][rbg_pos.x] = &(*obj_iter);
				object_ptr_vec[rbg_pos.y][rbg_pos.x] = &the_object;
			}
			else
			{
				cout << "Warning!  More than one Tiled object per block "
					<< "grid coordinate is not permitted!\n";
				////cout << "The following Tiled object has been rejected:  ";
				////obj_iter->debug_print_short();
				//obj_iter->reject();
				//cout << "The following Tiled object has been rejected:  ";
				//the_object.debug_print_short();
				the_object.reject();
				cout << endl;
			}
			
		}
	}
	
	
}

void tiled_sublevel::generate_sublevel_entrance_using_warp_block_old
	( tiled_object* obj_ptr, sprite_init_param_group& to_push )
{
	static constexpr u32 num_pixels_per_block_row_or_col = 16;
	
	if ( warp_id_sle_id_map.count(to_push.type - lowest_warp_id_st) )
	{
		cout << "Error!  More than one st_warp_block with the same"
			<< " number in a sublevel is not permitted!\n";
		obj_ptr->reject_and_exit();
	}
	
	bool found_bdest = false;
	//for ( auto prop_iter=obj_ptr->property_vec.begin();
	//	prop_iter!=obj_ptr->property_vec.end();
	//	++prop_iter )
	for ( tiled_object_property& the_property : obj_ptr->property_vec )
	{
		// Destination warp block
		//if ( prop_iter->name == string("bdest") )
		if ( the_property.name == string("bdest") )
		{
			found_bdest = true;
			
			stringstream value_sstm;
			//value_sstm << prop_iter->value;
			value_sstm << the_property.value;
			
			u32 bdest_value;
			value_sstm >> bdest_value;
			
			the_sublevel.sublevel_entrance_vec[to_push.type 
				- lowest_warp_id_st] = { sle_from_warp_block, vec2_f24p8
				( ( num_pixels_per_block_row_or_col 
				* obj_ptr->real_block_grid_pos.x ) 
				<< fixed24p8::shift,
				( num_pixels_per_block_row_or_col
				* obj_ptr->real_block_grid_pos.y ) 
				<< fixed24p8::shift ) };
			
			//warp_id_sle_id_map[bdest_value] 
			//	= the_sublevel.sublevel_entrance_vec.size() - 1;
			//warp_id_sle_id_map[to_push.type - lowest_warp_id_st]
			//	= the_sublevel.sublevel_entrance_vec.size() - 1;
			warp_id_sle_id_map[to_push.type - lowest_warp_id_st]
				= bdest_value;
			
			//to_push.extra_param_0 = bdest_value;
			//to_push.extra_param_0 
			//	= the_sublevel.sublevel_entrance_vec.size() - 1;
			
			// Only process a single property
			break;
		}
	}
	
	if (!found_bdest)
	{
		cout << "Error!  st_warp_block's are REQUIRED to have a "
			<< "destination st_warp_block, specified by a property called "
			<< "\"bdest\" in Tiled.\n";
		obj_ptr->reject_and_exit();
	}
}

void tiled_sublevel::connect_warp_blocks_to_sublevel_entrances_old()
{
	for ( sprite_init_param_group& sprite_ipg 
		: the_sublevel.sprite_ipg_vec )
	{
		if ( sprite_ipg.type >= lowest_warp_id_st 
			&& sprite_ipg.type <= highest_warp_id_st )
		{
			cout << ( sprite_ipg.type - lowest_warp_id_st ) << " " 
				<< warp_id_sle_id_map[sprite_ipg.type - lowest_warp_id_st]
				<< endl;
			sprite_ipg.extra_param_0 = warp_id_sle_id_map[sprite_ipg.type 
				- lowest_warp_id_st];
		}
	}
}

//void tiled_sublevel::generate_warp_id_bdest_map_and_warp_id_sdest_map()
void tiled_sublevel::generate_warp_id_bdest_and_sdest_stuff()
{
	// Continue here.
	
	
	//for ( tiled_object* obj_ptr : warp_block_object_ptr_vec )
	for ( pair< tiled_object*, u32 >& the_pair
		: warp_block_object_ptr_and_sprite_ipg_index_vec )
	{
		tiled_object* obj_ptr = the_pair.first;
		sprite_init_param_group& sprite_ipg 
			= the_sublevel.sprite_ipg_vec[the_pair.second];
		
		if ( warp_id_bdest_map.count(obj_ptr->the_sprite_type 
			- lowest_warp_id_st) )
		{
			cout << "Error!  More than one st_warp_block with the same"
				<< " number in a sublevel is not permitted!\n";
			obj_ptr->reject_and_exit();
		}
		
		bool found_bdest = false, found_sdest = false;
		
		//cout << obj_ptr->property_vec.size() << endl;
		for ( tiled_object_property& the_property : obj_ptr->property_vec )
		{
			//cout << the_property.name << " " << the_property.value 
			//	<< endl;
			
			// Destination warp block
			if ( the_property.name == string("bdest") )
			{
				found_bdest = true;
				
				stringstream value_sstm;
				//value_sstm << prop_iter->value;
				value_sstm << the_property.value;
				
				u32 bdest_value;
				value_sstm >> bdest_value;
				
				warp_id_bdest_map[obj_ptr->the_sprite_type 
					- lowest_warp_id_st] = bdest_value;
				
				sprite_ipg.extra_param_0 = bdest_value;
				
				// Only process bdest and sdest values
				if (found_sdest)
				{
					break;
				}
			}
			
			// Destination sublevel
			else if ( the_property.name == string("sdest") )
			{
				found_sdest = true;
				
				stringstream value_sstm;
				//value_sstm << prop_iter->value;
				value_sstm << the_property.value;
				
				u32 sdest_value;
				value_sstm >> sdest_value;
				
				warp_id_sdest_map[obj_ptr->the_sprite_type
					- lowest_warp_id_st] = sdest_value;
				
				sprite_ipg.extra_param_1 = sdest_value;
				
				// Only process bdest and sdest values
				if (found_bdest)
				{
					break;
				}
			}
			
		}
		
		if (!found_bdest)
		{
			cout << "Error!  st_warp_block's are REQUIRED to have a "
				<< "destination st_warp_block, specified by a property "
				<< " called \"bdest\" in Tiled.\n";
			obj_ptr->reject_and_exit();
		}
		
		// If there isn't an sdest property, then use a value of -1, which
		// will be used by the level_converter_class to mean that the same
		// sublevel should be considered the destination sublevel for this
		// st_warp_block. 
		if (!found_sdest)
		{
			warp_id_sdest_map[obj_ptr->the_sprite_type - lowest_warp_id_st] 
				= -1;
			sprite_ipg.extra_param_1 = -1;
			
			//cout << "Found an st_warp_block without an sdest property!\n";
			//cout << "Here's a debug_print().\n";
			//obj_ptr->debug_print();
		}
	}
	
	
}

void tiled_sublevel::generate_warp_block_sublevel_entrances()
{
	static constexpr u32 num_pixels_per_block_row_or_col = 16;
	
	//the_sublevel.sublevel_entrance_vec[to_push.type 
	//	- lowest_warp_id_st] = { sle_from_warp_block, vec2_f24p8
	//	( ( num_pixels_per_block_row_or_col 
	//	* obj_ptr->real_block_grid_pos.x ) 
	//	<< fixed24p8::shift,
	//	( num_pixels_per_block_row_or_col
	//	* obj_ptr->real_block_grid_pos.y ) 
	//	<< fixed24p8::shift ) };
	
	//for ( tiled_object* obj_ptr : warp_block_object_ptr_vec )
	for ( pair< tiled_object*, u32 >& the_pair
		: warp_block_object_ptr_and_sprite_ipg_index_vec )
	{
		tiled_object* obj_ptr = the_pair.first;
		the_sublevel.sublevel_entrance_vec[obj_ptr->the_sprite_type 
			- lowest_warp_id_st] = { sle_from_warp_block, vec2_f24p8
			( ( num_pixels_per_block_row_or_col 
			* obj_ptr->real_block_grid_pos.x ) << fixed24p8::shift,
			( num_pixels_per_block_row_or_col
			* obj_ptr->real_block_grid_pos.y ) << fixed24p8::shift ) };
	}
	
}

void tiled_sublevel::generate_start_of_level_sle_using_st_player
	( tiled_object* obj_ptr )
{
	static constexpr u32 num_pixels_per_block_row_or_col = 16;
	
	the_sublevel.sublevel_entrance_vec.push_back( (sublevel_entrance)
		{ sle_start_of_level, vec2_f24p8 
		( ( num_pixels_per_block_row_or_col 
		* obj_ptr->real_block_grid_pos.x ) << fixed24p8::shift,
		( num_pixels_per_block_row_or_col
		* obj_ptr->real_block_grid_pos.y ) << fixed24p8::shift )} );
	
}


void tiled_sublevel::debug_print()
{
	
	map_header.debug_print();
	
	cout << endl;
	
	//for ( auto iter=tileset_vec.begin(); iter!=tileset_vec.end(); ++iter )
	//{
	//	iter->debug_print();
	//}
	for ( tiled_tileset& the_tileset : tileset_vec )
	{
		the_tileset.debug_print();
	}
	
	cout << endl;
	
	//for ( auto iter=layer_vec.begin(); iter!=layer_vec.end(); ++iter )
	//{
	//	iter->debug_print();
	//}
	for ( tiled_layer& the_layer : layer_vec )
	{
		the_layer.debug_print();
	}
	
	cout << endl;
	
	//for ( auto iter=objectgroup_vec.begin();
	//	iter!=objectgroup_vec.end();
	//	++iter )
	//{
	//	iter->debug_print();
	//}
	
	for ( tiled_objectgroup& the_objectgroup : objectgroup_vec )
	{
		the_objectgroup.debug_print();
	}
}

void tiled_sublevel::debug_print_gid_st_map()
{
	for ( auto iter=tileset_vec.begin();
		iter!=tileset_vec.end();
		++iter )
	{
		for ( u32 i=iter->firstgid; 
			i<iter->firstgid+iter->tilecount; 
			++i )
		{
			cout << i << "," << gid_st_map.at(i) << ";  ";
		}
		cout << endl;
	}
	
}

void tiled_sublevel::debug_print_gid_tileset_ptr_map()
{
	for ( auto iter=tileset_vec.begin();
		iter!=tileset_vec.end();
		++iter )
	{
		for ( u32 i=iter->firstgid; 
			i<iter->firstgid+iter->tilecount; 
			++i )
		{
			cout << i << "," << gid_tileset_ptr_map.at(i)->name 
				<< ";  ";
		}
		cout << endl;
	}
	
}



