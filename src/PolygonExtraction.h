#ifndef __WDTD_POLY_EXTRACT_H__
#define __WDTD_POLY_EXTRACT_H__

//------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

#include "TGARead.h"

//------------------------------------------------------------------------------------------

namespace PolyExtract
{
	struct Point
	{
		float x, y;
		unsigned cornerType;

		bool fromAlpha;

		Point( ){ x = 0; y = 0; cornerType = 0; }
		Point( float pX, float pY ){ x = pX; y = pY; cornerType = 0; fromAlpha = false; }
		Point( float pX, float pY, unsigned corner, bool alpha ){ x = pX; y = pY; cornerType = corner; fromAlpha = alpha; }
	};

	struct SimplePoint
	{
		long x, y;

		SimplePoint( ){ }
		SimplePoint( long px, long py ){ x = px; y = py; }
	};

	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------

	struct Polygon
	{
		std::vector< SimplePoint > points;
		int parentIndex;

		Polygon( ) : parentIndex( -1 ) { }
	};

	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------

	class Extractor
	{
	public:

		Extractor( ){ }
		~Extractor( ){ }

		bool loadTGA( std::string path );
		bool execute( );

		std::vector< Polygon > getPolygonList( ){ return polygonList; }

	protected:

		void extractFromFourBytes( );
		bool setParents( );

		bool traverse( int xDir, int yDir, unsigned &xCurr, unsigned &yCurr );
		bool getNextPoint( Point point, unsigned &x, unsigned &y );
		bool inPolygon( SimplePoint p0, unsigned index );

		float area( Polygon poly );

	private:

		TGARead::TGA m_TGA;

		std::vector< Polygon > polygonList;

		std::map< std::pair< unsigned, unsigned >, Point > m_MainMap;
		std::map< std::pair< unsigned, unsigned >, Point > m_AltMap;

	};
};

//------------------------------------------------------------------------------------------

#endif // __WDTH_POLY_EXTRACT_H__