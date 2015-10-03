#include "tiled_layer_class.hpp"

tiled_layer::tiled_layer( xml_node<>* node )
{
	stringstream attr_sstm;
	
	
	// First, build the "header" of the layer
	for ( auto attr=node->first_attribute();
		attr;
		attr=attr->next_attribute() )
	{
		attr_sstm = stringstream();
		
		attr_sstm << attr->value();
		
		if ( attr->name() == string("name") )
		{
			name = attr->value();
		}
		else if ( attr->name() == string("width") )
		{
			attr_sstm >> size_2d.x;
		}
		else if ( attr->name() == string("height") )
		{
			attr_sstm >> size_2d.y;
		}
	}
	
	
	// Next, build data_str and, more importantly, tile_ids.
	auto internal_node = node->first_node();
	
	if ( internal_node->name() != string("data") )
	{
		cout << "Error!  A layer has been found without a data section!\n";
		exit(1);
	}
	
	
	if ( !( internal_node->first_attribute()->name() == string("encoding") 
		&& internal_node->first_attribute()->value() == string("csv") ) )
	{
		cout << "Error!  A layer has been found without csv encoding!\n";
		exit(1);
	}
	
	data_str = internal_node->value();
	data_str = data_str.substr(1);
	
	stringstream data_str_sstm(data_str);
	
	string line;
	
	// Here's the conversion from the CSV format to a 2D dynamic array of
	// tile IDs
	while ( getline( data_str_sstm, line, '\n' ) )
	{
		stringstream line_sstm(line);
		string id_str;
		
		// This is a vector for ONE ROW of tile IDs
		vector<u32> id_line_vec;
		
		
		while ( getline( line_sstm, id_str, ',' ) )
		{
			stringstream id_sstm;
			u32 to_push;
			
			// Convert from the string type to the u32 type
			id_sstm << id_str;
			id_sstm >> to_push;
			
			if ( to_push > 0 )
			{
				--to_push;
			}
			
			id_line_vec.push_back(to_push);
		}
		
		tile_ids.push_back(id_line_vec);
		
	}
	
	
	
}


