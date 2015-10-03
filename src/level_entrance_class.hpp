#ifndef level_entrance_class_hpp
#define level_entrance_class_hpp

#include "vec2_class.hpp"

enum level_entrance_type
{
	le_start_of_level,
	
	
	// The number of level entrance types is automatically updated by the
	// compiler via the le_count enum id
	le_count,
	
} __attribute__((aligned(4)));


class level_entrance
{
public:		// variables
	
	level_entrance_type type;
	
	vec2_f24p8 in_level_pos;
	
} __attribute__((aligned(4)));



#endif		// level_entrance_class_hpp
