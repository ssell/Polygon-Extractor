/**
	NOTE TO SELF

	Targa data starts at 0,0 [bottom left] and reads from the right and up

**/

#include "TGARead.h"
#include "PolygonExtraction.h"
#include "PolygonWriter.h"

unsigned MAX_X;
unsigned MAX_Y;

//------------------------------------------------------------------------------------------

void getParameters( unsigned char &limit )
{
	//------------------------------------------------
	// Alpha Sensitivity

	std::cout << "\nTarga image contains alpha channel.\nPixels with alpha <= " << ( int )limit << " will be removed. Do you wish to change this? (y/n)";

	char yesOrNo = ' ';

	while( yesOrNo != 'n' && yesOrNo != 'N' )
	{
		std::cout << "\n> ";
		std::cin >> yesOrNo;

		if( yesOrNo == 'y' || yesOrNo == 'Y' )
		{
			int get = 256;

			std::cout << "\nPlease set new limit ( 0 - 255 )";

			do
			{
				std::cout << "\n> ";
				std::cin >> get;
			} while( get > 255 );

			limit = ( unsigned char )get;
		}
	}
}

//------------------------------------------------------------------------------------------

int main( int argc, char* argv[ ] )
{
	PolyExtract::Extractor extract;
	
	bool success = false;

	//--------------------------------------------------------------------------------------
	// Read in the image data

	while( !success )
	{
		if( argc == 1 )
		{
			std::string imagePath;

			std::cout << "Please enter path to image:\n> ";
			std::cin >> imagePath;

			success = extract.loadTGA( imagePath );

			if( !success )
				std::cout << "\n!Failed to read in TGA data!\nPlease ensure path leads to an UNCOMPRESSED Targa (.tga) file.\n" << std::endl;
		}
		else
		{
			std::cout << "Using image path:\n> " << argv[ 1 ] << std::endl;
			success = extract.loadTGA( argv[ 1 ] );

			if( !success )
			{
				std::cout << "\n!Failed to read in TGA data!\nPlease ensure path leads to an UNCOMPRESSED Targa (.tga) file.\n" << std::endl;
				argc = 1;
			}
		}
	}

	//-----------------------------------------------------------------------------------------

	extract.execute( );

	std::cout << "\nRecording polygon list to: \"test_poly_write.txt\"" << std::endl;
	recordPolygon( extract.getPolygonList( ), "test_poly_write.txt" );

	//-----------------------------------------------------------------------------------------

	system( "pause()" );

	return 0;
}