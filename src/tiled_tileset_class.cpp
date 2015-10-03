#include "tiled_tileset_class.hpp"

tiled_tileset::tiled_tileset( xml_node<>* node )
{
	stringstream sstm;
	
	for ( auto attr=node->first_attribute();
		attr;
		attr=attr->next_attribute() )
	{
		sstm = stringstream();
		
		sstm << attr->value();
		
		if ( attr->name() == string("firstgid") )
		{
			sstm >> firstgid;
			if ( firstgid > 0 )
			{
				--firstgid;
			}
		}
		else if ( attr->name() == string("name") )
		{
			//sstm >> name;
			name = attr->value();
		}
		else if ( attr->name() == string("tilewidth") )
		{
			sstm >> tile_size_2d.x;
		}
		else if ( attr->name() == string("tileheight") )
		{
			sstm >> tile_size_2d.y;
		}
		else if ( attr->name() == string("tilecount") )
		{
			sstm >> tilecount;
		}
		
	}
	
	//cout << endl;
	
}
