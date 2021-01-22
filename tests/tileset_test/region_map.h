
//{{BLOCK(region_map)

//======================================================================
//
//	region_map, 128x120@8, 
//	+ palette 256 entries, not compressed
//	+ 233 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 16x15 
//	Total size: 512 + 14912 + 480 = 15904
//
//	Time-stamp: 2021-01-20, 14:18:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_REGION_MAP_H
#define GRIT_REGION_MAP_H

#define region_mapTilesLen 14912
extern const unsigned int region_mapTiles[3728];

#define region_mapMapLen 480
extern const unsigned short region_mapMap[240];

#define region_mapPalLen 512
extern const unsigned short region_mapPal[256];

#endif // GRIT_REGION_MAP_H

//}}BLOCK(region_map)
