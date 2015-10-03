#ifndef utility_funcs_hpp
#define utility_funcs_hpp

#include "misc_includes.hpp"


inline const string get_uncompressed_block_data_file_name
	( const string& output_dirname, const string& output_basename )
{
	return output_dirname + "/" + output_basename 
		+ "_uncompressed_block_data.bin";
}

inline const string get_compressed_block_data_file_name
	( const string& output_dirname, const string& output_basename )
{
	return output_dirname + "/" + output_basename 
		+ "_compressed_block_data.bin";
}


#endif		// utility_funcs_hpp
