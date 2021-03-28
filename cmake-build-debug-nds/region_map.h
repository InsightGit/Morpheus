
//{{BLOCK(region_map)

//======================================================================
//
//	region_map, 128x120@8, 
//	+ palette 256 entries, not compressed
//	+ 240 tiles not compressed
//	Total size: 512 + 15360 = 15872
//
//	Time-stamp: 2021-03-27, 18:37:08
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_REGION_MAP_H
#define GRIT_REGION_MAP_H

#define region_mapTilesLen 15360
extern const unsigned int region_mapTiles[3840];

#define region_mapPalLen 512
extern const unsigned short region_mapPal[256];

#define region_mapMapLen 2048
extern const unsigned short region_mapMap[1024];

#endif // GRIT_REGION_MAP_H

//}}BLOCK(region_map)
