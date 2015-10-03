#ifndef vec2_class_hpp
#define vec2_class_hpp

#include "misc_types.hpp"
#include "fixed24p8_class.hpp"


template< typename type >
class vec2
{
public:		// variables
	type x, y;
	
public:		// functions
	inline vec2()
		: x( type() ), y( type() )
	{
	}
	
	inline vec2( vec2<type>& to_copy )
		: x(to_copy.x), y(to_copy.y)
	{
	}
	
	
	inline vec2( type& s_x, type& s_y )
		: x(s_x), y(s_y)
	{
	}
	
	
	constexpr inline vec2( const vec2<type>& to_copy )
		: x(to_copy.x), y(to_copy.y)
	{
	}
	
	constexpr inline vec2( const type& s_x, const type& s_y )
		: x(s_x), y(s_y)
	{
	}
	
	
	inline vec2<type>& operator = ( const vec2<type>& to_copy )
	{
		x = to_copy.x;
		y = to_copy.y;
		
		return *this;
	}
	
	
	inline vec2<type> operator + ( const vec2<type>& to_add ) const
	{
		return vec2<type> ( x + to_add.x, y + to_add.y );
	}
	inline vec2<type> operator - ( const vec2<type>& to_sub ) const
	{
		return vec2<type> ( x - to_sub.x, y - to_sub.y );
	}
	
	inline vec2<type> operator * ( const type& scale ) const
	{
		return vec2<type> ( x * scale, y * scale );
	}
	
	
	
	inline vec2<type>& operator += ( const vec2<type>& to_add )
	{
		x += to_add.x;
		y += to_add.y;
		
		return *this;
	}
	inline vec2<type>& operator -= ( const vec2<type>& to_sub )
	{
		x -= to_sub.x;
		y -= to_sub.y;
		
		return *this;
	}
	
	inline vec2<type>& operator *= ( const type& scale )
	{
		x *= scale;
		y *= scale;
		
		return *this;
	}
	
	
	// Comparison Operator Overloads
	inline bool operator == ( const vec2<type>& to_cmp ) const
	{
		return ( ( x == to_cmp.x ) && ( y == to_cmp.y ) );
	}
	
	inline bool operator != ( const vec2<type>& to_cmp ) const
	{
		return ( ( x != to_cmp.x ) || ( y != to_cmp.y ) );
	}
	
	
	
} __attribute__((aligned(4)));

template< typename type >
inline vec2<type> operator * ( const type& scale, 
	const vec2<type>& vec2_to_mul )
{
	return vec2_to_mul * scale;
}


typedef vec2<u16> vec2_u16; typedef vec2<s16> vec2_s16;
typedef vec2<u32> vec2_u32; typedef vec2<s32> vec2_s32;
typedef vec2<u64> vec2_u64; typedef vec2<s64> vec2_s64;


template<>
class vec2<fixed24p8>
{
public:		// variables
	fixed24p8 x, y;
	
public:		// functions
	
	inline vec2()
		: x( fixed24p8() ), y( fixed24p8() )
	{
	}
	
	inline vec2( vec2<fixed24p8>& to_copy )
		: x(to_copy.x), y(to_copy.y)
	{
	}
	
	
	inline vec2( fixed24p8& s_x, fixed24p8& s_y )
		: x(s_x), y(s_y)
	{
	}
	
	
	constexpr inline vec2( const vec2<fixed24p8>& to_copy )
		: x(to_copy.x), y(to_copy.y)
	{
	}
	
	constexpr inline vec2( const fixed24p8& s_x, const fixed24p8& s_y )
		: x(s_x), y(s_y)
	{
	}
	
	
	
} __attribute__((aligned(4)));



typedef vec2<fixed24p8> vec2_f24p8;



#endif		// vec2_class_hpp
