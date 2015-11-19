#ifndef level_converter_class_hpp
#define level_converter_class_hpp

#include "tiled_sublevel_class.hpp"

class level_converter
{
public:		// variables
	static constexpr u32 max_num_sublevels = 8;
	
	string output_dirname, output_basename;
	
	bool file_was_opened;
	
	string sublevel_file_names_str_raw;
	vector<string> sublevel_file_names_vec;
	
	vector<tiled_sublevel> tiled_sublevel_vec;
	
	
public:		// functions
	level_converter( const string& input_file_name,
		const string& s_output_prefix );
	
	inline const string output_prefix() const
	{
		return output_dirname + "/" + output_basename;
	}
	
	void generate_level_header_file();
	void generate_level_cpp_file();
	
};


#endif		// level_converter_class_hpp
