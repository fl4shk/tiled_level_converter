#include "sublevel_class.hpp"

const vec2_u32 sublevel::max_size_2d = { 512, 32 };
const u32 sublevel::max_num_blocks = max_size_2d.x * max_size_2d.y;

//sublevel::sublevel()
//{
//	init(max_size_2d);
//}


sublevel::sublevel( const vec2_u32& s_size_2d )
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

void sublevel::init( const vec2_u32& s_size_2d )
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


