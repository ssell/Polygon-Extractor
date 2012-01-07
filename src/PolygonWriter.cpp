#include "PolygonWriter.h"

#include <fstream>

//------------------------------------------------------------------------------------------

/**

	Going to output our polygon(s) to a file (.mdl)

	The format of the file will look like this:

		<Number of Polygons> 
		<First Polygon>
		<Second Polygon>
		...

	With '#' representing a polygon with no ancestor
	proceeded by '%' representing any holes in that polygon.


	Example:

		3
		#
		0,0				// Polygon with no holes or ancestors
		0,1
		1,1
		#
		10,10			// Polygon with one hole and no ancestors
		100,10
		100,100
		10,100
		%
		12,12			// Polygon with one ancestor
		22,12
		22,22
		12,22
**/
void recordPolygon( std::vector< PolyExtract::Polygon > p_Polygons, const char* p_Path )
{
	PolyExtract::Polygon activePoly;

	std::ofstream out;
	out.open( p_Path );

	if( out.is_open( ) )
	{
		out << p_Polygons.size( ) << "\n";

		while( !p_Polygons.empty( ) )
		{
			activePoly = p_Polygons.at( 0 );
			p_Polygons.erase( p_Polygons.begin( ) );

			if( activePoly.parentIndex == -1 )
				out << "#\n";
			else
				out << "%\n";

			for( int i = 0; i < activePoly.points.size( ); i++ )
				out << activePoly.points.at( i ).x << "," << activePoly.points.at( i ).y << "\n";
		}
	}

	out.close( );
}

//------------------------------------------------------------------------------------------

