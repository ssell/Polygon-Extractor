#include "PolygonExtraction.h"

#include <cstdarg>
#include <sstream>

#define MIN(x,y) ( x < y ? x : y )
#define MAX(x,y) ( x > y ? x : y )

//------------------------------------------------------------------------------------------

namespace PolyExtract
{
	//--------------------------------------------------------------------------------------

	bool Extractor::loadTGA( std::string path )
	{
		return TGARead::loadTGA( path.c_str( ), m_TGA );
	}

	//--------------------------------------------------------------------------------------

	bool Extractor::execute( )
	{
		if( m_TGA.data == NULL )
			return false;

		extractFromFourBytes( );

		return setParents( );
	}

	//--------------------------------------------------------------------------------------

	void Extractor::extractFromFourBytes( )
	{		
		unsigned char limit = 254;
		unsigned int  origin = 1;

		//------------------------------------------------
		//------------------------------------------------

		// get OPTIONAL user customization of parameters
		//getParameters( limit );

		//------------------------------------------------
		//------------------------------------------------

		// TGA data is in a 1-dimension array, but we will treat it as two-dimensional as it is
		// easier that way to visualize the data that we are working with.

		unsigned pos = 0;
		unsigned x, y, value;

		bool alpha;

		// This while loop retrieves and stores all corners into the map

		while( pos < ( ( m_TGA.width + 2 ) * ( m_TGA.height + 2 ) ) * 4 )
		{
			value = 0;

			alpha = ( int )m_TGA.data[ pos + 3 ] <= ( int )limit;

			x = ( pos / 4 ) % m_TGA.width;
			y = ( pos / 4 ) / m_TGA.width;

			if( x == 0 )
				std::cout << std::endl;

			//--------------------------------------------
			// Do above and below checks

			if( y == 0 ) // Check bottom row
			{
				value += alpha == true ? 0 : 8; // cell below
											//pos + row width * byte count + 3 to get to alpha byte
				value += alpha != ( m_TGA.data[ pos + ( m_TGA.width * 4 ) + 3 ] <= limit ) ? 2 : 0; // cell above
			}
			else if( y != m_TGA.height - 1 ) // Check the middle rows
			{
				value += alpha != ( m_TGA.data[ pos + ( m_TGA.width * 4 ) + 3 ] <= limit ) ? 2 : 0; // cell above
				value += alpha != ( m_TGA.data[ pos - ( m_TGA.width * 4 ) + 3 ] <= limit ) ? 8 : 0; // cell below
			}
			else // top row
			{
				value += alpha == true ? 0 : 2; // cell above
				value += alpha != ( m_TGA.data[ pos - ( m_TGA.width * 4 ) + 3 ] <= limit ) ? 8 : 0; // cell below
			}

			//--------------------------------------------
			// Do left and right checks

			//If in left-most cell
			if( x == 0 )
				value += alpha == true ? 0 : 16;
			else //cell in the middle
				value += alpha != ( m_TGA.data[ pos - 1 ] <= limit ) ? 16 : 0; //cell to the left

			//If in right-most cell
			if( x == m_TGA.width - 1 )
				value += alpha == true ? 0 : 4;
			else
				value += alpha != ( m_TGA.data[ pos + 7 ] <= limit ) ? 4 : 0; //cell to the right

			//--------------------------------------------
			// Set the coordinates

			switch( value )
			{
			case 6:
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, value, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, value, alpha );
				break;
			case 12:
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y ) ] = Point( x + 1, y, value, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >(  x + 1, y ) ] = Point( x + 1, y, value, alpha );
				break;
			case 14: //break into a 6 and 12
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y ) ] = Point( x + 1, y, 12, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >(  x + 1, y ) ] = Point( x + 1, y, 12, alpha );
				break;
			case 18:
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, value, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, value, alpha );
				break;
			case 22: //break into a 6 and 18
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );
				break;
			case 24:
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, value, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, value, alpha );
				break;
			case 26: // break into a 18 and 24
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );

				break;
			case 28: //break into a 12 and 24
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y ) ] = Point( x + 1, y, 12, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >(  x + 1, y ) ] = Point( x + 1, y, 12, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );
				break;
			case 30: //break into a 6, 12, 18, and 24
				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x + 1, y + 1 ) ] = Point( x + 1, y + 1, 6, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x + 1, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x + 1, y ) ] = Point( x + 1, y, 12, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >(  x + 1, y ) ] = Point( x + 1, y, 12, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y + 1 ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y + 1 ) ] = Point( x, y + 1, 18, alpha );

				if( m_MainMap.find( std::pair< unsigned, unsigned >( x, y ) ) != m_MainMap.end( ) )
					m_AltMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );
				else
					m_MainMap[ std::pair< unsigned, unsigned >( x, y ) ] = Point( x, y, 24, alpha );
				break;
			}

			pos += 4;
		} // end corner retrieval while loop

		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------

		// Connect the dots!

		// The starting Point. lower_bound gives us the Point closest (or equal to) the origin 0,0
		std::map< std::pair< unsigned, unsigned >, Point >::iterator activePoint = m_MainMap.lower_bound( std::pair< unsigned, unsigned >( 0, 0 ) );

		unsigned iX, iY, aX, aY;
		int ret;

		while( activePoint != m_MainMap.end( ) && m_MainMap.size( ) > 0 )
		{
			Polygon tempPoly;

			// There is a Polygon
			iX = ( *activePoint ).second.x;
			iY = ( *activePoint ).second.y;

			aX = iX;
			aY = iY;

			if( aX > 200 || aY > 200 )
			{
				std::cout << "err" << std::endl;
			}

			do
			{
				ret = getNextPoint( ( *activePoint ).second, aX, aY );

				if( !ret )
				{
					std::cout << "failed[" << aX << "," << aY << "]" << std::endl;
					break; //next Point not found; error
				}
				
				std::cout << aX << "," << aY << std::endl;

				if( ret == 1 && ( ( *activePoint ).second.x != iX || ( *activePoint ).second.y != iY ) )
				{
					m_MainMap.erase( activePoint );
				}

				tempPoly.points.push_back( SimplePoint( ( long )aX, ( long )aY ) );
				

				activePoint = m_MainMap.find( std::pair< unsigned, unsigned >( aX, aY ) );

			} while( aX != iX || aY != iY ); // while we have not traversed entire Polygon

			m_MainMap.erase( activePoint );

			std::cout << "\n\n";

			polygonList.push_back( tempPoly );
			activePoint = m_MainMap.lower_bound( std::pair< unsigned, unsigned >( 0, 0 ) );
		}

		//------------------------------------------------
		//------------------------------------------------
	}

	//--------------------------------------------------------------------------------------

	bool Extractor::getNextPoint( Point point, unsigned &x, unsigned &y )
	{
		int xTraverse = 0;
		int yTraverse = 0;

		// Use the information from Point (corner type, and if it was an alpha pixel) to determine what direction to
		// travel to find the next Point. Remember, we travel in a counter-clockwise fashion
		if( point.fromAlpha )
		{
			switch( point.cornerType )
			{
			case 6:
				yTraverse = -1;
				break;
			case 12:
				xTraverse = -1;
				break;
			case 18:
				xTraverse = 1;
				break;
			case 24:
				yTraverse = 1;
				break;
			}
		}
		else
		{
			switch( point.cornerType )
			{
			case 6:
				xTraverse = -1;
				break;
			case 12:
				yTraverse = 1;
				break;
			case 18:
				yTraverse = -1;
				break;
			case 24:
				xTraverse = 1;
				break;

			}
		}

		//----------------------------------------------------------------------------------

		return traverse( xTraverse, yTraverse, x, y );
	}

	//--------------------------------------------------------------------------------------

	bool Extractor::traverse( int xDir, int yDir, unsigned &xCurr, unsigned &yCurr )
	{
		// Possibly make some check for proper corner using direction and corner provided
		// by current point from findPoint. Compare between the two maps

		xCurr += xDir;
		yCurr += yDir;

		// unsigned, so if < 0, will roll over to unsigned int max
		if( xCurr > m_TGA.width || yCurr > m_TGA.height )
			return false;

		if( m_MainMap.find( std::pair< unsigned, unsigned >( xCurr, yCurr ) ) != m_MainMap.end( ) )
			return true;
		else
		{
			std::map< std::pair< unsigned, unsigned >, Point >::iterator iter = m_AltMap.find( std::pair< unsigned, unsigned >( xCurr, yCurr ) );

			if( iter != m_AltMap.end( ) )
			{
				// Remove from the alt map and place into the main map
				m_MainMap[ std::pair< unsigned, unsigned >( xCurr, yCurr ) ] = ( *iter ).second;
				m_AltMap.erase( iter );

				return true;
			}

			return traverse( xDir, yDir, xCurr, yCurr );
		}
	}

	//--------------------------------------------------------------------------------------

	bool Extractor::setParents( )
	{
		if( polygonList.empty( ) )
			return false;
		else if( polygonList.size( ) == 1 )
			return true;

		std::vector< unsigned > parents;

		//--------------------------------------------
		// Find the immediate parents of all polygons

		for( int i = 0; i < polygonList.size( ); i++ )
		{
			parents.clear( );

			//----------------------------------------
			// Compile list of all polygons that polygonList[ j ] is located within.
			// We need to check only a single point due to the nature of the data-set

			for( int j = 0; j < polygonList.size( ); j++ )
			{
				if( j == i )
					continue;

				if( inPolygon( polygonList[ i ].points[ 0 ], j ) )
					parents.push_back( j );
			}

			//----------------------------------------
			// Compare areas of all prospective parents.
			// Parent with the smallest area is the most immediate.

			if( !parents.empty( ) )
			{
				// Remove -1 from parentIndex and set first parent in list as active parent
				polygonList[ i ].parentIndex = parents[ 0 ];

				// parent[ 0 ] is already active parent, so start checking at parent[ 1 ]
				for( int j = 1; j < parents.size( ); j++ )
					// if parent[ i ] area is smaller than the current parents area, then set it as the new parent
					if( area( polygonList[ parents[ j ] ] ) < area( polygonList[ polygonList[ i ].parentIndex ] ) )
						polygonList[ i ].parentIndex = j;
			}
		}

		//--------------------------------------------
		// If a parent of a polygon has itself a parent,
		// then the current polygon will have no parent.
		// This is because its parent is a hole, and therefor
		// it will be treated as a new, independent polygon.

		int *realParent = new int[ polygonList.size( ) ];
		int ancestorCount, parent;

		// Dont want to modify ancestry lines until all paths are traced, so store
		// the 'real' parents into the realParent array
		for( int i = 0; i < polygonList.size( ); i++ )
		{
			// Trace parent lists to -1. If number of ancestors is even, then it has no parents.
			ancestorCount = 0;
			parent = polygonList[ i ].parentIndex;

			/***
				ALPHA IN GREEN HAS WRONG PARENT
				****/
			while( parent != -1 )
			{
				parent = polygonList[ parent ].parentIndex;
				ancestorCount++;
			}

			realParent[ i ] = ancestorCount % 2 != 0 ? polygonList[ i ].parentIndex : -1;
		}

		// Sent parentIndex to the 'real' parent
		for( int i = 0; i < polygonList.size( ); i++ )
			polygonList[ i ].parentIndex = realParent[ i ];

		std::cout << "\n\n";
		for( int i = 0; i < polygonList.size( ); i++ )
			std::cout << polygonList[ i ].parentIndex << std::endl;

		return true;
	}

	//--------------------------------------------------------------------------------------

	bool Extractor::inPolygon( SimplePoint p0, unsigned index )
	{
		// Point in Polygon - Solution 1 (2D) from:
		// http://paulbourke.net/geometry/insidepoly/

		unsigned counter = 0;

		double xIntersect;

		SimplePoint p1;
		SimplePoint p2;

		Polygon poly = polygonList[ index ];

		p1 = poly.points[ 0 ];

		for( unsigned i = 1; i <= poly.points.size( ); i++ )
		{
			p2 = poly.points[ i % poly.points.size( ) ];

			if( p0.y > MIN( p1.y, p2.y ) )
			{
				if( p0.y <= MAX( p1.y, p2.y ) )
				{
					if( p0.x <= MAX( p1.x, p2.x ) )
					{
						if( p1.y != p2.y )
						{
							xIntersect = ( p0.y - p1.y ) * ( p2.x - p1.x ) / ( p2.y - p1.y ) + p1.x;

							if( p1.x == p2.x || p0.x <= xIntersect )
								counter++;
						}
					}
				}
			}

			p1 = p2;
		}

		if( counter % 2 == 0 )
			return false;
		else
			return true;
	}

	//--------------------------------------------------------------------------------------

	float Extractor::area( Polygon poly )
	{
		/**
		 * http://www.mathopenref.com/coordpolygonarea.html
		 *
		 *	 |  (X1*Y2-Y1*X2)+(X2*Y3-Y2*X3)+...+(Xn*Y1-Yn*X1)  |
		 *   |  ---------------------------------------------  |
		 *   |                        2                        |
		 *
		 * note the |x| abs
		 */

		float sum = 0;
		unsigned i;

		for( i = 0; i < poly.points.size( ) - 1; i++ )
			sum += ( poly.points[ i ].x * poly.points[ i + 1 ].y - poly.points[ i ].y * poly.points[ i + 1 ].x );

		sum += ( poly.points[ i ].x * poly.points[ 0 ].y - poly.points[ i ].y * poly.points[ 0 ].x );

		if( sum < 0 )
			return ( sum * -1.f ) / 2.f;
		else
			return sum / 2.f;
	}
};