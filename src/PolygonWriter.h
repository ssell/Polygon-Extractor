/**
 * \file PolygonWriter.h
 * \author ssell
 *
 * Demo of how to use the data provided by the extraction methods
 */

#ifndef __WDTD_POLYGON_WRITER_H__
#define __WDTH_POLYGON_WRITER_H__

//------------------------------------------------------------------------------------------

#include "PolygonExtraction.h"

//------------------------------------------------------------------------------------------

void recordPolygon( std::vector< PolyExtract::Polygon > p_Polygons, const char* p_Path );

//------------------------------------------------------------------------------------------

#endif __WDTD_POLYGON_WRITER_H__