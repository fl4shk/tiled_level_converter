#include "tiled_object_class_stuff.hpp"


tiled_objectgroup::tiled_objectgroup( xml_node<>* node )
{
	// find the name attribute
	for ( auto attr=node->first_attribute();
		attr;
		attr=attr->next_attribute() )
	{
		if ( attr->name() == string("name") )
		{
			name = attr->value();
			
			// I don't know why I don't include a break statement after
			// finding the name attribute.
		}
	}
	
	// Loop through all the internal nodes within the current objectgroup
	// for the purpose of building object_vec.
	for ( auto object_node=node->first_node();
		object_node;
		object_node=object_node->next_sibling() )
	{
		tiled_object to_push;
		
		// Build to_push using the attributes of object_node.
		for ( auto attr=object_node->first_attribute();
			attr;
			attr=attr->next_attribute() )
		{
			stringstream attr_sstm;
			
			attr_sstm << attr->value();
			
			if ( attr->name() == string("id") )
			{
				attr_sstm >> to_push.id;
			}
			else if ( attr->name() == string("gid") )
			{
				attr_sstm >> to_push.gid_raw;
				
				// Tiled stores hflip and vflip within an object's gid
				// parameter.
				to_push.gid = to_push.gid_raw & 0x3fffffff;
				to_push.vflip = to_push.gid_raw & 0x40000000;
				to_push.hflip = to_push.gid_raw & 0x80000000;
				
				// subtract the gid by 1
				if ( to_push.gid > 0 )
				{
					--to_push.gid;
				}
			}
			else if ( attr->name() == string("x") )
			{
				u32 temp;
				attr_sstm >> temp;
				
				to_push.block_grid_pos.x = temp >> 4;
			}
			else if ( attr->name() == string("y") )
			{
				u32 temp;
				attr_sstm >> temp;
				
				to_push.block_grid_pos.y = temp >> 4;
			}
			else if ( attr->name() == string("width") )
			{
				attr_sstm >> to_push.size_2d.x;
			}
			else if ( attr->name() == string("height") )
			{
				attr_sstm >> to_push.size_2d.y;
			}
			
		}
		
		
		object_vec.push_back(to_push);
	}
	
	
}

