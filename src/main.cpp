#include "misc_includes.hpp"
#include "utility_funcs.hpp"

#include "level_converter_class.hpp"



int main( int argc, char** argv )
{
	if ( argc != 3 )
	{
		cout << "Usage:  " << argv[0] << " input_file_name "
			<< "output_prefix\n";
		return 0;
	}
	
	string input_file_name(argv[1]), output_prefix(argv[2]);
	
	////sublevel the_sublevel;
	////
	////tiled_sublevel the_tiled_sublevel( input_file_name, output_prefix, 
	////	the_sublevel );
	//tiled_sublevel the_tiled_sublevel( input_file_name, output_prefix );
	//
	//if ( !the_tiled_sublevel.file_was_opened )
	//{
	//	cout << "The TMX file couldn't be opened!  Exiting...\n";
	//	
	//	return 1;
	//}
	//
	//the_tiled_sublevel.generate_the_sublevel();
	
	level_converter the_level_converter( input_file_name, output_prefix );
	
	if ( !the_level_converter.file_was_opened )
	{
		cout << "The file couldn't be opened!  Exiting....\n";
		
		return 1;
	}
	
	
	return 0;
	
}


