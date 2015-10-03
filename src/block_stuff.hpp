#ifndef block_stuff_hpp
#define block_stuff_hpp

#include "misc_types.hpp"

#include <vector>
#include <string>
#include <utility>
using namespace std;

enum block_type
{
	bt_air,
	bt_log,
	bt_wood,
	bt_qmark,
	bt_exclam,
	bt_dud,
	
	
	// bt_count is the amount of block types.  It is automatically updated
	// by the compiler.
	bt_count,
	
} __attribute__((aligned(4)));



// A class for unique block data.
class block
{
public:		// variables
	
	u16 type;
	u16 persistent_data_index;
	
public:		// functions
	
	inline block_type get_block_type() const
	{
		return (block_type)type;
	}
	
} __attribute__((aligned(4)));


namespace block_stuff
{
	extern vector< pair< string, bool > > 
		bt_name_and_has_persistent_data_vec;
	
	extern const string bt_unknown_str;
	
	constexpr u32 persistent_block_data_array_size = 2048;
	
	inline const string& get_bt_name( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return bt_name_and_has_persistent_data_vec.at(bt_air).first;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type).first;
	}
	
	inline const string& get_bt_name_debug( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return bt_unknown_str;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type)
			.first;
	}
	 
	inline bool bt_has_persistent_data( block_type the_block_type )
	{
		if ( the_block_type < 0 || the_block_type >= bt_count )
		{
			return false;
		}
		return bt_name_and_has_persistent_data_vec.at(the_block_type)
			.second;
	}
}


#endif		// block_stuff_hpp
