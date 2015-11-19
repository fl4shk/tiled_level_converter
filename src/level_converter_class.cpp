#include "level_converter_class.hpp"


level_converter::level_converter ( const string& input_file_name,
	const string& s_output_prefix )
{
	output_dirname = dirname(strdup(s_output_prefix.c_str()));
	output_basename = basename(strdup(s_output_prefix.c_str()));
	
	fstream sublevel_file_names_file( input_file_name, ios_base::in );
	
	if ( !sublevel_file_names_file.is_open() )
	{
		file_was_opened = false;
		return;
	}
	
	file_was_opened = true;
	
	// Read the whole file into sublevel_file_names_str_raw
	getline( sublevel_file_names_file, sublevel_file_names_str_raw, '\0' );
	
	sublevel_file_names_file.close();
	
	//cout << sublevel_file_names_str_raw;
	
	stringstream sublevel_file_names_sstm(sublevel_file_names_str_raw);
	string line;
	
	while ( getline( sublevel_file_names_sstm, line, '\n' ) )
	{
		if ( line.empty() )
		{
			//cout << "I found an empty line!\n";
			continue;
		}
		
		sublevel_file_names_vec.push_back(line);
	}
	
	if ( sublevel_file_names_vec.size() > max_num_sublevels )
	{
		cout << "Error!  A maximum of " << max_num_sublevels 
			<< " sublevels per level is permitted!  Exiting....\n";
		
		exit(1);
	}
	else if ( sublevel_file_names_vec.size() == 0 )
	{
		cout << "Error!  At least one sublevel is needed to be able to "
			<< "convert!  Exiting....\n";
		
		exit(1);
	}
	
	
	for ( u32 i=0; i<sublevel_file_names_vec.size(); ++i )
	{
		string& sublevel_file_name = sublevel_file_names_vec.at(i);
		
		stringstream sublevel_num_sstm;
		string sublevel_num_str;
		
		sublevel_num_sstm << i;
		sublevel_num_sstm >> sublevel_num_str;
		
		tiled_sublevel to_push( sublevel_file_name, output_dirname,
			string("sublevel_") + sublevel_num_str );
		
		if ( !to_push.file_was_opened )
		{
			cout << "Error!  The following sublevel file could not be "
				<< "opened:  " << sublevel_file_name << ".  Exiting....\n";
			
			exit(1);
		}
		//to_push.mostly_generate_the_sublevel();
		//cout << to_push.layer_vec.size() << " sdf\n";
		
		tiled_sublevel_vec.push_back(to_push);
	}
	
	
	for ( tiled_sublevel& the_tiled_sublevel : tiled_sublevel_vec )
	{
		//the_tiled_sublevel.generate_sublevel_entrance_vec();
		//cout << the_tiled_sublevel.get_sublevel_str_raw() << endl;
		cout << the_tiled_sublevel.get_sublevel_file_name() << endl;
		the_tiled_sublevel.generate_the_sublevel();
		cout << endl;
	}
	
	generate_level_header_file();
	generate_level_cpp_file();
}

void level_converter::generate_level_header_file()
{
	fstream header_file( output_prefix() + string(".hpp"), ios_base::out );
	
	header_file << "#ifndef " << output_basename << "_hpp\n"
		<< "#define " << output_basename << "_hpp\n\n\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "level_class.hpp\"\n"
		<< "#include \"../game_engine_stuff/level_stuff/"
		<< "sprite_level_data_stuff.hpp\"\n";
	
	header_file << "\n\n";
	
	for ( tiled_sublevel& the_tiled_sublevel : tiled_sublevel_vec )
	{
		header_file << "// " << the_tiled_sublevel.sublevel_file_name 
			<< "\n";
		
		header_file << "extern const sublevel< "
			<< the_tiled_sublevel.the_sublevel.compressed_block_data_vec
			.size() << ", " 
			<< the_tiled_sublevel.the_sublevel.real_size_2d.x << ", " 
			<< the_tiled_sublevel.the_sublevel.real_size_2d.y << ", "
			<< the_tiled_sublevel.the_sublevel.sprite_ipg_vec.size() << ", "
			<< the_tiled_sublevel.the_sublevel.sublevel_entrance_vec.size() 
			<< " > " << output_basename << "_" 
			<< the_tiled_sublevel.output_basename << ";\n\n";
		
	}
	
	
	header_file << "\nextern const level " << output_basename << ";\n";
	
	header_file << "\n\n";
	
	header_file << "#endif\t\t// " << output_basename << "_hpp";
	
	
}

void level_converter::generate_level_cpp_file()
{
	fstream cpp_file( output_prefix() + string(".thumb.cpp"), 
		ios_base::out );
	
	cpp_file << "#include \"" << output_basename << ".hpp\"\n\n\n";
	
	//for ( tiled_sublevel& the_tiled_sublevel : tiled_sublevel_vec )
	for ( u32 i=0; i<tiled_sublevel_vec.size(); ++i )
	{
		tiled_sublevel& the_tiled_sublevel = tiled_sublevel_vec[i];
		
		cpp_file << "// " << the_tiled_sublevel.sublevel_file_name << "\n";
		
		cpp_file << "const sublevel< "
			<< the_tiled_sublevel.the_sublevel.compressed_block_data_vec
			.size() << ", "
			<< the_tiled_sublevel.the_sublevel.real_size_2d.x << ", " 
			<< the_tiled_sublevel.the_sublevel.real_size_2d.y << ", "
			<< the_tiled_sublevel.the_sublevel.sprite_ipg_vec.size() << ", "
			<< the_tiled_sublevel.the_sublevel.sublevel_entrance_vec.size()
			<< " > " << output_basename << "_" 
			<< the_tiled_sublevel.output_basename << "\n" << "= {\n";
		
		
		// Build the array of compressed block data
		cpp_file << "\t{\n";
		
		u32 counter = 0;
		
		for ( u32& the_u32 
			: the_tiled_sublevel.the_sublevel.compressed_block_data_vec )
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
		
		if ( counter % 4 != 0 )
		{
			cpp_file << "\n";
		}
		cpp_file << "\t},\n\t\n";
		
		
		// Build the array of sprite_init_param_group's
		cpp_file << "\t{\n";
		
		for ( sprite_init_param_group& sprite_ipg 
			: the_tiled_sublevel.the_sublevel.sprite_ipg_vec )
		{
			cpp_file << "\t\t{ ";
			
			if ( sprite_ipg.type < st_name_vec.size() )
			{
				cpp_file << get_st_name(sprite_ipg.type);
			}
			else
			{
				// Print out a number if the_sprite_type doesn't have a
				// corresponding string in st_name_vec.
				cpp_file << sprite_ipg.type;
			}
			
			cpp_file << ", " << sprite_ipg.initial_block_grid_x_coord 
				<< ", " << sprite_ipg.initial_block_grid_y_coord << ", " 
				<< ( sprite_ipg.facing_right ? "true" : "false" );
			
			
			// This had to be changed to make the Sherwin's Adventure code
			// compile with the OUTDATED version of arm-none-eabi- prefixed
			// GCC and Binutils that are included in devkitARM
			//// Don't clutter the sprite_init_param_group's that have only
			//// zeroes as extra parameters.
			//if ( !( sprite_ipg.extra_param_0 == 0 
			//	&& sprite_ipg.extra_param_1 == 0
			//	&& sprite_ipg.extra_param_2 == 0 
			//	&& sprite_ipg.extra_param_3 == 0 ) )
			//{
			//	cpp_file << ", " << sprite_ipg.extra_param_0 << ", " 
			//		<< sprite_ipg.extra_param_1 << ", " 
			//		<< sprite_ipg.extra_param_2 << ", " 
			//		<< sprite_ipg.extra_param_3;
			//}
			
			//cpp_file << ", " << sprite_ipg.extra_param_0 << ", " 
			//	<< sprite_ipg.extra_param_1 << ", " 
			//	<< sprite_ipg.extra_param_2 << ", " 
			//	<< sprite_ipg.extra_param_3;
			
			if ( sprite_ipg.type >= lowest_warp_id_st 
				&& sprite_ipg.type <= highest_warp_id_st )
			{
				cpp_file << ", " << sprite_ipg.extra_param_0 << ", ";
				
				//if ( sprite_ipg
				//	<< sprite_ipg.extra_param_1 << ", ";
				if ( sprite_ipg.extra_param_1 == (u32)-1 )
				{
					//cout << "Found a sprite_init_param_group with -1 for "
					//	<< "its extra_param_1!\n";
					cpp_file << i;
				}
				else
				{
					cpp_file << sprite_ipg.extra_param_1;
				}
				
				
				cpp_file << ", " << sprite_ipg.extra_param_2 << ", " 
					<< sprite_ipg.extra_param_3;
			}
			else
			{
				cpp_file << ", " << sprite_ipg.extra_param_0 << ", " 
					<< sprite_ipg.extra_param_1 << ", " 
					<< sprite_ipg.extra_param_2 << ", " 
					<< sprite_ipg.extra_param_3;
			}
			
			cpp_file << " },\n";
		}
		
		cpp_file << "\t},\n\t\n";
		
		// Build the array of sublevel_entrance's
		cpp_file << "\t{\n";
		
		for ( sublevel_entrance& the_sle 
			: the_tiled_sublevel.the_sublevel.sublevel_entrance_vec )
		{
			cpp_file << "\t\t{ " << get_sle_name_debug(the_sle.type)
				<< hex << ", vec2_f24p8( {0x" 
				<< the_sle.in_sublevel_pos.x.data << "}, {0x" 
				<< the_sle.in_sublevel_pos.y.data << "} ) },\n" << dec;
		}
		
		cpp_file << "\t},\n";
		
		
		cpp_file << "};\n\n\n";
	}
	
	
	cpp_file << "const level " << output_basename << " = level\n\t"
		<< "( sublevel_pointer(" << output_basename << "_" 
		<< tiled_sublevel_vec[0].output_basename << ")";
	
	if ( tiled_sublevel_vec.size() == 1 )
	{
		cpp_file << " );\n";
	}
	else
	{
		cpp_file << ",\n";
		
		//for ( tiled_sublevel& the_tiled_sublevel : tiled_sublevel_vec )
		for ( size_t i=1; i<tiled_sublevel_vec.size(); ++i )
		{
			cpp_file << "\tsublevel_pointer(" << output_basename << "_" 
				<< tiled_sublevel_vec[i].output_basename << ")";
			
			if ( i == tiled_sublevel_vec.size() - 1 )
			{
				cpp_file << " );";
			}
			else
			{
				cpp_file << ",";
			}
			
			cpp_file << "\n";
		}
	}
	
	cpp_file << "\n\n\n";
	
	
}

