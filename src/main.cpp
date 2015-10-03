#include "misc_includes.hpp"
#include "utility_funcs.hpp"


#include "tiled_level_class.hpp"




int main( int argc, char** argv )
{
	if ( argc != 3 )
	{
		cout << "Usage:  " << argv[0] << " input_file_name "
			<< "output_prefix\n";
		return 0;
	}
	
	string input_file_name(argv[1]), output_prefix(argv[2]);
	
	level the_level;
	
	tiled_level the_tiled_level( input_file_name, output_prefix, 
		the_level );
	
	if ( !the_tiled_level.file_was_opened )
	{
		cout << "The TMX file couldn't be opened!  Exiting...\n";
		
		return 1;
	}
	
	
	the_tiled_level.generate_the_level();
	
	
	return 0;
	
}
