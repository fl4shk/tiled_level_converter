#include "misc_includes.hpp"
#include "utility_funcs.hpp"


#include "tiled_sublevel_class.hpp"




int main( int argc, char** argv )
{
	if ( argc != 3 )
	{
		cout << "Usage:  " << argv[0] << " input_file_name "
			<< "output_prefix\n";
		return 0;
	}
	
	string input_file_name(argv[1]), output_prefix(argv[2]);
	
	sublevel the_sublevel;
	
	tiled_sublevel the_tiled_sublevel( input_file_name, output_prefix, 
		the_sublevel );
	
	if ( !the_tiled_sublevel.file_was_opened )
	{
		cout << "The TMX file couldn't be opened!  Exiting...\n";
		
		return 1;
	}
	
	
	the_tiled_sublevel.generate_the_sublevel();
	
	
	return 0;
	
}
