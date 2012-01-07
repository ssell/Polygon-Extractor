#ifndef __EFA_TGA_READ_H__
#define __EFA_TGA_READ_H__

//------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------

namespace TGARead
{
	struct TGA
	{
		TGA( );
		~TGA( );

		void destroy( );

		int width, height;
		unsigned char byteCount;
		unsigned char* data;
	};

	bool loadTGA( const char* path, TGA &tga );
}

//------------------------------------------------------------------------------------------

#endif // __EFA_TGA_READ_H__