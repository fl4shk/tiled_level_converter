#include "level_class.hpp"

const vec2_u32 level::max_size_2d = { 512, 32 };
const u32 level::max_num_blocks = max_size_2d.x * max_size_2d.y;

level::level()
{
	init(max_size_2d);
}


level::level( const vec2_u32& s_size_2d )
	: size_2d(s_size_2d)
{
	for ( u32 j=0; j<size_2d.y; ++j )
	{
		uncompressed_block_data_vec.push_back( vector<block>() );
		
		for ( u32 i=0; i<size_2d.x; ++i )
		{
			uncompressed_block_data_vec[j].push_back( block() );
		}
	}
}

void level::init( const vec2_u32& s_size_2d )
{
	size_2d = s_size_2d;
	
	for ( u32 j=0; j<size_2d.y; ++j )
	{
		uncompressed_block_data_vec.push_back( vector<block>() );
		
		for ( u32 i=0; i<size_2d.x; ++i )
		{
			uncompressed_block_data_vec[j].push_back( block() );
		}
	}
}


