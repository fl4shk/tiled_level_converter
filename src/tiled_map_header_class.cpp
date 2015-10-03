#include "tiled_map_header_class.hpp"


tiled_map_header::tiled_map_header( xml_node<>* map_node )
{
	stringstream sstm;
	
	
	for ( auto attr=map_node->first_attribute();
		attr;
		attr=attr->next_attribute() )
	{
		sstm = stringstream();
		
		sstm << attr->value();
		
		if ( attr->name() == string("orientation") )
		{
			if ( attr->value() != string("orthogonal") )
			{
				cout << "Error!  The tilemap doesn't have orthogonal "
					<< "orientation!  Exiting....\n";
				exit(1);
			}
		}
		else if ( attr->name() == string("renderorder") )
		{
			if ( attr->value() != string("right-down") )
			{
				cout << "Error!  The tilemap doesn't have a renderorder "
					<< " of right-down!  Exiting....\n";
				exit(1);
			}
		}
		else if ( attr->name() == string("width") )
		{
			sstm >> size_2d.x;
		}
		else if ( attr->name() == string("height") )
		{
			sstm >> size_2d.y;
		}
		else if ( attr->name() == string("tilewidth") )
		{
			sstm >> tile_size_2d.x;
		}
		else if ( attr->name() == string("tileheight") )
		{
			sstm >> tile_size_2d.y;
		}
		else if ( attr->name() == string("nextobjectid") )
		{
			sstm >> nextobjectid;
		}
		
	}
	
}


//tiled_map_header& tiled_map_header::operator = 
//	( const tiled_map_header& to_copy )
//{
//	orientation_is_orthogonal = to_copy.orientation_is_orthogonal;
//	renderorder_is_right_down = to_copy.renderorder_is_right_down;
//	
//	size_2d.x = to_copy.size_2d.x;
//	size_2d.y = to_copy.size_2d.y;
//	
//	tile_size_2d.x = to_copy.tile_size_2d.x;
//	tile_size_2d.y = to_copy.tile_size_2d.y;
//	
//	nextobjectid = to_copy.nextobjectid;
//	
//	return *this;
//}




