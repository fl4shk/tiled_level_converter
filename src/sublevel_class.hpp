#ifndef sublevel_class_hpp
#define sublevel_class_hpp

#include "array_2d_helper_class.hpp"
#include "sprite_level_data_stuff.hpp"
#include "block_stuff.hpp"

#include "sublevel_entrance_class.hpp"


#include <vector>
using namespace std;


//// Here is a reference class for sublevels as they appear in the GBA-side
////code.
//template< u32 compressed_block_data_size, u32 xsize, u32 ysize, 
//	u32 sprite_ipg_arr_size, u32 sublevel_entrance_arr_size >
//class sublevel
//{
//public:		// variables
//	const u32 compressed_block_data[compressed_block_data_size];
//	const sprite_init_param_group sprite_ipg_arr[sprite_ipg_arr_size];
//	const sublevel_entrance sublevel_entrance_arr[sublevel_entrance_arr_size];
//	
//} __attribute__((aligned(4)));


class sublevel
{
public:		// constants
	static const vec2_u32 max_size_2d;
	static const u32 max_num_blocks;
	
public:		// variables
	
	// size_2d is generally set to max_size_2d
	vec2_u32 size_2d;
	
	// real_size_2d is set to the size_2d of the primary Tiled layer.
	vec2_u32 real_size_2d;
	
	
	// The std::vector of std::vector<block>'s used as a dynamically
	// allocated 2D array of UNCOMPRESSED block data.  However, this vector
	// of vectors is initialized to the SAME SIZE (both for the outer
	// vector and the inner vectors) every time the program is run!  This
	// is so that, IN THE GBA-SIDE CODE, the COMPRESSED array of block data
	// can be DECOMPRESSED DIRECTLY to the statically allocated 2D array of
	// block data.
	vector< vector<block> > uncompressed_block_data_vec;
	
	// On the other hand, compressed_block_data_vec can have a variable
	// size.
	vector<u32> compressed_block_data_vec;
	
	// sprite_ipg_vec can also have a variable size.
	vector<sprite_init_param_group> sprite_ipg_vec;
	
	// Additionally, sublevel_entrance_vec can have a variable size.
	vector<sublevel_entrance> sublevel_entrance_vec;
	
public:		// functions
	inline sublevel() : sublevel(max_size_2d)
	{
	}
	
	sublevel( const vec2_u32& s_size_2d );
	
	void init( const vec2_u32& s_size_2d );
	
	
};



#endif		// sublevel_class_hpp
