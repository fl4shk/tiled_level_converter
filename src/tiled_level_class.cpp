#include "tiled_level_class.hpp"



tiled_level::tiled_level( const string& level_file_name,
	const string& s_output_prefix, level& the_level )
{
	output_dirname = dirname(strdup(s_output_prefix.c_str()));
	output_basename = basename(strdup(s_output_prefix.c_str()));
	
	the_level_ptr = &the_level;
	
	fstream level_file( level_file_name, ios_base::in );
	
	if ( !level_file.is_open() )
	{
		file_was_opened = false;
		return;
	}
	
	file_was_opened = true;
	
	
	// Read the whole file into level_str
	getline( level_file, level_str, '\0' );
	
	level_file.close();
	
	doc.parse<0>( const_cast<char*>( level_str.c_str() ) );
	
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
	
}


void tiled_level::generate_the_level()
{
	// General error detection and response (if necessary).
	if ( layer_vec.size() != 1 )
	{
		cout << "Error!  EXACTLY ONE Tiled layer is required!\n";
		exit(1);
	}
	
	if ( layer_vec[0].size_2d.x > level::max_size_2d.x )
	{
		cout << "Error!  Levels are not permitted to have a width of "
			<< "more than " << level::max_size_2d.x << "blocks!  "
			<< "Exiting....\n";
		exit(1);
	}
	
	if ( layer_vec[0].size_2d.y != level::max_size_2d.y )
	{
		cout << "Error!  Levels are required to have a height of EXACTLY " 
			<< level::max_size_2d.y << " blocks!  "
			<< "Exiting....\n";
		exit(1);
	}
	
	
	the_level_ptr->real_size_2d = layer_vec[0].size_2d;
	
	// Generate the level's block data (both compressed and uncompressed)
	u32 next_persistent_block_data_index = 1;
	
	for ( u32 j=0; j<layer_vec[0].tile_ids.size(); ++j )
	{
		vector<u32>& layer_col = layer_vec[0].tile_ids[j];
		vector<block>& bd_col = the_level_ptr
			->uncompressed_block_data_vec[j];
		
		
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
	
	// Generate the level's vector of sprite_init_param_group's
	for ( u32 j=0; j<object_ptr_vec.size(); ++j )
	{
		for ( u32 i=0; i<object_ptr_vec[j].size(); ++i )
		{
			tiled_object* obj_ptr = object_ptr_vec[j][i];
			if ( obj_ptr != NULL )
			{
				sprite_init_param_group to_push;
				to_push.type = obj_ptr->the_sprite_type;
				to_push.initial_block_grid_x_coord 
					= obj_ptr->real_block_grid_pos.x;
				to_push.initial_block_grid_y_coord
					= obj_ptr->real_block_grid_pos.y;
				to_push.facing_right = ( obj_ptr->hflip ) ? false : true;
				
				the_level_ptr->sprite_ipg_vec.push_back(to_push);
			}
		}
	}
	
	generate_level_header();
	generate_level_cpp_file();
	
	// Remove the temporary files
	const string clean_up_command = "rm " 
		+ get_uncompressed_block_data_file_name( output_dirname, 
			output_basename ) + " "
		+ get_compressed_block_data_file_name( output_dirname,
			output_basename );
	
	cout << "Running this command:  " << clean_up_command << endl;
	system(clean_up_command.c_str());
	
	cout << "Parent is exiting (2)!\n";
	
}


void tiled_level::write_uncompressed_block_data_to_file()
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
		j<the_level_ptr->uncompressed_block_data_vec.size(); 
		++j )
	{
		vector<block>& bd_col = the_level_ptr
			->uncompressed_block_data_vec[j];
		
		for ( u32 i=0; i<bd_col.size(); ++i )
		{
			output_file.write( reinterpret_cast<char*>(&bd_col[i]), 
				sizeof(bd_col[i]) );
		}
	}
	
	output_file.close();
}

void tiled_level::read_compressed_block_data_from_file()
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
		
		the_level_ptr->compressed_block_data_vec.push_back(the_u32);
	}
	
}

void tiled_level::generate_level_header()
{
	fstream header_file( output_prefix() + string(".hpp"), ios_base::out );
	
	header_file << "#ifndef " << output_basename << "_hpp\n"
		<< "#define " << output_basename << "_hpp\n\n\n"
		<< "#include \"../game_engine_stuff/level_class.hpp\"\n";
	
	header_file << "\n\n";
	
	header_file << "extern const level< "
		<< the_level_ptr->compressed_block_data_vec.size() << ", "
		<< the_level_ptr->real_size_2d.x << ", " 
		<< the_level_ptr->real_size_2d.y << ", "
		<< the_level_ptr->sprite_ipg_vec.size() << ", "
		<< the_level_ptr->level_entrance_vec.size() << " > "
		<< output_basename << ";\n\n";
	
	header_file << "\n";
	
	header_file << "#endif\t\t// " << output_basename << "_hpp";
	
}

void tiled_level::generate_level_cpp_file()
{
	fstream cpp_file( output_prefix() + string(".thumb.cpp"), 
		ios_base::out );
	
	cpp_file << "#include \"" << output_basename << ".hpp\"\n\n\n";
	
	cpp_file << "const level< "
		<< the_level_ptr->compressed_block_data_vec.size() << ", "
		<< the_level_ptr->real_size_2d.x << ", " 
		<< the_level_ptr->real_size_2d.y << ", "
		<< the_level_ptr->sprite_ipg_vec.size() << ", "
		<< the_level_ptr->level_entrance_vec.size () << " > "
		<< output_basename << "\n"
		<< "= {\n";
	
	
	// Build the array of compressed block data
	cpp_file << "\t{\n";
	
	u32 counter = 0;
	for ( auto iter=the_level_ptr->compressed_block_data_vec.begin();
		iter!=the_level_ptr->compressed_block_data_vec.end();
		++iter )
	{
		//cpp_file << hex << "\t\t0x" << *iter << dec << ",\n";
		if ( counter % 4 == 0 )
		{
			cpp_file << "\t\t";
		}
		
		cpp_file << hex << "0x" << *iter << dec;
		
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
	
	cpp_file << "\t},\n\t\n";
	
	
	// Build the array of sprite_init_param_group's
	cpp_file << "\t{\n";
	
	for ( auto iter=the_level_ptr->sprite_ipg_vec.begin();
		iter!=the_level_ptr->sprite_ipg_vec.end();
		++iter )
	{
		cpp_file << "\t\t{ ";
		
		if ( iter->type < st_name_vec.size() )
		{
			cpp_file << get_st_name(iter->type);
		}
		else
		{
			// Print out a number if the_sprite_type doesn't have a
			// corresponding string in st_name_vec.
			cpp_file << iter->type;
		}
		
		cpp_file << ", " << iter->initial_block_grid_x_coord << ", "
			<< iter->initial_block_grid_y_coord << ", " 
			<< ( iter->facing_right ? "true" : "false" );
		
		
		cpp_file << " },\n";
	}
	
	cpp_file << "\t},\n";
	
	// Build hte array of 
	
	
	cpp_file << "};\n\n\n";
}

void tiled_level::build_gid_maps_and_correct_objects()
{
	// Build the maps
	for ( auto iter=tileset_vec.begin();
		iter!=tileset_vec.end();
		++iter )
	{
		for ( u32 i=iter->firstgid; i<iter->firstgid+iter->tilecount; ++i )
		{
			gid_st_map[i] = (sprite_type)( i - iter->firstgid );
			gid_tileset_ptr_map[i] = &(*iter);
		}
	}
	
	//debug_print_gid_st_map();
	//cout << endl;
	//debug_print_gid_tileset_ptr_map();
	//cout << endl;
	
	
	// Correct the object positions and sizes, and also set the_sprite_type
	// of each object to the correct value.
	for ( auto og_iter=objectgroup_vec.begin();
		og_iter!=objectgroup_vec.end();
		++og_iter )
	{
		for ( auto obj_iter=og_iter->object_vec.begin();
			obj_iter!=og_iter->object_vec.end();
			++obj_iter )
		{
			tiled_tileset* tileset = gid_tileset_ptr_map.at(obj_iter->gid);
			
			
			// Tiled objects can have various sizes, but my GBA game
			// doesn't support such things.
			obj_iter->real_size_2d = tileset->tile_size_2d;
			
			
			obj_iter->real_block_grid_pos.x = obj_iter->block_grid_pos.x;
			
			// Tiled itself stores object positions based on the BOTTOM
			// LEFT corner, but my GBA game uses the TOP LEFT corner for
			// positions.
			obj_iter->real_block_grid_pos.y = obj_iter->block_grid_pos.y
				 - ( obj_iter->real_size_2d.y >> 4 );
			
			
			// Set the_sprite_type
			obj_iter->the_sprite_type = gid_st_map.at(obj_iter->gid);
			
		}
	}
	
}


void tiled_level::build_object_ptr_vec()
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
	for ( auto og_iter=objectgroup_vec.begin();
		og_iter!=objectgroup_vec.end();
		++og_iter )
	{
		for ( auto obj_iter=og_iter->object_vec.begin();
			obj_iter!=og_iter->object_vec.end();
			++obj_iter )
		{
			vec2_u32& rbg_pos = obj_iter->real_block_grid_pos;
			
			// Only one Tiled object is permitted per block grid coordinate
			if ( object_ptr_vec[rbg_pos.y][rbg_pos.x] == NULL )
			{
				object_ptr_vec[rbg_pos.y][rbg_pos.x] = &(*obj_iter);
			}
			else
			{
				cout << "Warning!  More than one Tiled object per block "
					<< "grid coordinate is not permitted!\n";
				cout << "The following Tiled object has been rejected:  ";
				obj_iter->debug_print();
				cout << endl;
			}
			
		}
	}
	
	
}


void tiled_level::debug_print()
{
	
	map_header.debug_print();
	
	cout << endl;
	
	for ( auto iter=tileset_vec.begin(); iter!=tileset_vec.end(); ++iter )
	{
		iter->debug_print();
	}
	
	cout << endl;
	
	for ( auto iter=layer_vec.begin(); iter!=layer_vec.end(); ++iter )
	{
		iter->debug_print();
	}
	
	cout << endl;
	
	for ( auto iter=objectgroup_vec.begin();
		iter!=objectgroup_vec.end();
		++iter )
	{
		iter->debug_print();
	}
}

void tiled_level::debug_print_gid_st_map()
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

void tiled_level::debug_print_gid_tileset_ptr_map()
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



