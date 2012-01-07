#include "TGARead.h"
#include <stdio.h>

//------------------------------------------------------------------------------------------

namespace TGARead
{

	TGA::TGA( )
	{
		data = ( unsigned char* )0;
		width = 0;
		height = 0;
		byteCount = 0;
	}

	TGA::~TGA( )
	{
		delete[ ] data;
		data = 0;
	}

	void TGA::destroy( )
	{
		delete[ ] data;
		data = 0;
	}

	//--------------------------------------------------------------------------------------

	bool loadTGA( const char* path, TGA &tga )
	{
		FILE* file;

		unsigned char type[ 4 ];
		unsigned char info[ 6 ];

		file = fopen( path, "rb" );

		if( !file )
			return false;

		//Read in ID Length, Color Map Type, and Image Type
		fread( &type, sizeof( char ), 3, file );
		fseek( file, 12, SEEK_SET );
		// Read in Width, Height, Pixel Depth, and Image Descriptor (2,2,1,1)
		fread( &info, sizeof( char ), 6, file );

		if( type[ 1 ] != 0 || ( type[ 2 ] != 2 && type[ 2 ] != 3 ) )
		{
			fclose( file );
			return false;
		}

		tga.width = info[ 0 ] + info[ 1 ] * 256;
		tga.height = info[ 2 ] + info[ 3 ] * 256;
		tga.byteCount = info[ 4 ] / 8;

		if( tga.byteCount != 3 && tga.byteCount != 4 )
		{
			fclose( file );
			return false;
		}

		long imageSize = tga.width * tga.height * tga.width * tga.byteCount;

		tga.data = new unsigned char[ imageSize ];

		fread( tga.data, sizeof( unsigned char ), imageSize, file );

		fclose( file );

		return true;
	}
};