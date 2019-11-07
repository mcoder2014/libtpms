#include "marchbox.h"

#include <iostream>
#include <fstream>
#include <iostream>


#include <QDebug>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// init the edgeTable of marching box algorithm
int MarchBox::edgeTable[256]={
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };

// init the triangle table of marching box algorithm
int MarchBox::triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};


///
/// \brief MarchBox::setRange
/// \param physical_max
/// \param physical_min
/// \param sampleSize  对一个逻辑单位进行多大的采样率
/// \param density  多长的物理单位对应一个逻辑单位
///
void MarchBox::setRange(Eigen::Vector3d physical_max, Eigen::Vector3d physical_min, int sampleSize, int density)
{
    this->m_boundingbox_logical.setEmpty();
    this->m_boundingbox_physical.setEmpty();

    // Set new Range
    this->m_boundingbox_physical.extend(physical_max);
    this->m_boundingbox_physical.extend(physical_min);

    if(sampleSize <=0)
        sampleSize = 4;
    if(density <=0)
        density = 4;


    Eigen::Vector3d logical_max = physical_max / (1.0 * density);
    Eigen::Vector3d logical_min = physical_min / (1.0 * density);
    this->m_boundingbox_logical.extend(logical_max);
    this->m_boundingbox_logical.extend(logical_min);

    // Update sample size
    m_ncellsX = static_cast<int>((logical_max[0] - logical_min[0]) * sampleSize);
    m_ncellsY = static_cast<int>((logical_max[1] - logical_min[1]) * sampleSize);
    m_ncellsZ = static_cast<int>((logical_max[2] - logical_min[2]) * sampleSize);

    this->m_sampleSize = sampleSize;
    this->m_density = density;

}

MarchBox::MarchBox()
{
    // To avoid bugs, the bounding must larger than 1
    this->setRange(Eigen::Vector3d(10.0,10.0,10.0),
                   Eigen::Vector3d(0.0,0.0,0.0),
                   8,
                   5);
}

///
/// \brief MarchBox::marching_cubes
/// \param implicit_surface
///
void MarchBox::marching_cubes(ImplicitSurface &implicit_surface)
{
    // Get the parameters from the class member variable

    // clear
    this->m_vertices.clear();
    this->m_faces.clear();
    this->m_index_map.clear();

    int ncellsX = m_ncellsX;
    int ncellsY = m_ncellsY;
    int ncellsZ = m_ncellsZ;

    Eigen::Vector3d logical_min = m_boundingbox_logical.min();
    Eigen::Vector3d logical_max = m_boundingbox_logical.max();
    Eigen::Vector3d physical_min = m_boundingbox_physical.min();
    Eigen::Vector3d physical_max = m_boundingbox_physical.max();

    // Calculate the step of each box
    Eigen::Vector3d logical_step = logical_max-logical_min;
    logical_step[0] = logical_step[0] / (ncellsX-1);
    logical_step[1] = logical_step[1] / (ncellsY-1);
    logical_step[2] = logical_step[2] / (ncellsZ-1);

    Eigen::Vector3d physical_step = physical_max - physical_min;
    physical_step[0] = physical_step[0] / (ncellsX - 1);
    physical_step[1] = physical_step[1] / (ncellsY - 1);
    physical_step[2] = physical_step[2] / (ncellsZ - 1);

    qDebug() << "\nImplicit Mesh type: " << implicit_surface.m_type
             << "\nnx: " << ncellsX
             << "\nny: " << ncellsY
             << "\nnz: " << ncellsZ;

    // To record the value of [ncellsX x ncellsY x ncellsZ] sample points
    std::vector<std::vector<std::vector<int>>> IS_value(
                ncellsX,std::vector<std::vector<int>>(
                    ncellsY, std::vector<int>(
                        ncellsZ, 0)));

    std::vector<std::vector<std::vector<glm::vec3>>> sample_points(
            ncellsX, std::vector<std::vector<glm::vec3>>(
                    ncellsY, std::vector<glm::vec3>(
                        ncellsZ, glm::vec3(0.0))));

    int count_sum_of_1 = 0; // use for debug
    int count_sum_of_0 = 0; // use for debug

    // init the record values and the sample_points of the marching cubes
    for(int iter_x = 0; iter_x < ncellsX; iter_x++)
    {
        double logical_x = logical_min[0] + logical_step[0] * iter_x;
        double physical_x = physical_min[0] + physical_step[0] * iter_x;
        for (int iter_y = 0; iter_y < ncellsY; iter_y++)
        {
            double logical_y = logical_min[1] + logical_step[1] * iter_y;
            double physical_y = physical_min[1] + physical_step[1] * iter_y;

            for (int iter_z = 0; iter_z < ncellsZ; iter_z++)
            {
                double logical_z = logical_min[2] + logical_step[2] * iter_z;
                double physical_z = physical_min[2] + physical_step[2] * iter_z;

                sample_points[iter_x][iter_y][iter_z] = glm::vec3(physical_x, physical_y, physical_z);
                // Record the IS_value
                double value = implicit_surface.f(logical_x, logical_y, logical_z);
                if(value > 0)
                {
                    IS_value[iter_x][iter_y][iter_z] = 1;
                    count_sum_of_1 ++;
                }
                else
                {
                    IS_value[iter_x][iter_y][iter_z] = 0;
                    count_sum_of_0 ++;
                }
            }
        }
    }

    std::cout << "Count the num of points at the out of Implicit surface : " << count_sum_of_1
              << "\nCount the num of points at the inner of Implicit surface :" << count_sum_of_0
              << std::endl;

    // Approximation of the all marching cubes
    for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)
    {
        for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
        {
            for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
            {
                // Get the eight point of the cube vertex,
                // to March the condition in 256 probailities

                //                 v4_______e4_____________v5
                //                  /|                    /|
                //                 / |                   / |
                //              e7/  |                e5/  |
                //               /___|______e6_________/   |
                //            v7|    |                 |v6 |e9
                //              |    |                 |   |
                //              |    |e8               |e10|
                //           e11|    |                 |   |
                //              |    |_________________|___|
                //              |   / v0      e0       |   /v1
                //              |  /                   |  /
                //              | /e3                  | /e1
                //              |/_____________________|/
                //              v3         e2          v2

                // The Cube with V0(iter_x, iter_y, iter_z)

                int cube_index = 0;
                float isolevel = 0.5;
                // v0 v1 v2 v3 v4 v5 v6 v7
                if(IS_value[iter_x    ][iter_y    ][iter_z    ] < isolevel) cube_index |= 1;
                if(IS_value[iter_x + 1][iter_y    ][iter_z    ] < isolevel) cube_index |= 2;
                if(IS_value[iter_x + 1][iter_y    ][iter_z + 1] < isolevel) cube_index |= 4;
                if(IS_value[iter_x    ][iter_y    ][iter_z + 1] < isolevel) cube_index |= 8;
                if(IS_value[iter_x    ][iter_y + 1][iter_z    ] < isolevel) cube_index |= 16;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z    ] < isolevel) cube_index |= 32;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 64;
                if(IS_value[iter_x    ][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 128;

                // Get the approximation triangle table
                int *edgeIndexList = triTable[cube_index];

                // -1 means the end of the triangle.
                while (*edgeIndexList != -1 )
                {
                    // Get the next approximation triangle
                    int e_index[3];
                    e_index[0] = *edgeIndexList;
                    e_index[1] = *(edgeIndexList + 1);
                    e_index[2] = *(edgeIndexList + 2);

                    edgeIndexList = edgeIndexList+3;      // ptr move 3 pos

                    glm::ivec3 face_index;
                    // Calculate the 3 points coordinates of the face
                    for (int e_i=0; e_i < 3; e_i ++)
                    {
                        int index = getVertexIdx(iter_x, iter_y, iter_z, e_index[e_i], sample_points);
                        face_index[e_i] = index;
                    }   // for (int e_i=0; e_i < 3; e_i ++)

                    this->m_faces.push_back(face_index);

                }   // while (*edgelist != -1 )

            }   //  for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
        }   // for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
    }   // for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)

}

void MarchBox::marching_cubes_closed(ImplicitSurface &implicit_surface)
{
    // Get the parameters from the class member variable

    // clear
    this->m_vertices.clear();
    this->m_faces.clear();
    this->m_index_map.clear();

    int ncellsX = m_ncellsX;
    int ncellsY = m_ncellsY;
    int ncellsZ = m_ncellsZ;

    // 反向操作
    int additions = 2;

    Eigen::Vector3d logical_min = m_boundingbox_logical.min();
    Eigen::Vector3d logical_max = m_boundingbox_logical.max();
    Eigen::Vector3d physical_min = m_boundingbox_physical.min();
    Eigen::Vector3d physical_max = m_boundingbox_physical.max();

    // Calculate the step of each box
    Eigen::Vector3d logical_step = logical_max-logical_min;
    logical_step[0] = logical_step[0] / (ncellsX-1);
    logical_step[1] = logical_step[1] / (ncellsY-1);
    logical_step[2] = logical_step[2] / (ncellsZ-1);

    Eigen::Vector3d physical_step = physical_max - physical_min;
    physical_step[0] = physical_step[0] / (ncellsX - 1 - additions);
    physical_step[1] = physical_step[1] / (ncellsY - 1 - additions);
    physical_step[2] = physical_step[2] / (ncellsZ - 1 - additions);

    qDebug() << "\nImplicit Mesh type: " << implicit_surface.m_type
             << "\nnx: " << ncellsX
             << "\nny: " << ncellsY
             << "\nnz: " << ncellsZ;

    // To record the value of [ncellsX x ncellsY x ncellsZ] sample points
    std::vector<std::vector<std::vector<int>>> IS_value(
                ncellsX,std::vector<std::vector<int>>(
                    ncellsY, std::vector<int>(
                        ncellsZ, 0)));

    std::vector<std::vector<std::vector<glm::vec3>>> sample_points(
            ncellsX, std::vector<std::vector<glm::vec3>>(
                    ncellsY, std::vector<glm::vec3>(
                        ncellsZ, glm::vec3(0.0))));
    // Logical sample points
    int half_addition = additions/2;

    // Physical sample points position
    for(int iter_x = 0; iter_x < ncellsX; iter_x++)
    {
        double physical_x = physical_min[0] + physical_step[0] * (iter_x-half_addition);
        for (int iter_y = 0; iter_y < ncellsY; iter_y++)
        {
            double physical_y = physical_min[1] + physical_step[1] * (iter_y-half_addition);
            for (int iter_z = 0; iter_z < ncellsZ; iter_z++)
            {
               double physical_z = physical_min[2] + physical_step[2] * (iter_z-half_addition);
                    sample_points[iter_x][iter_y][iter_z] = glm::vec3(
                                physical_x, physical_y, physical_z);
            }
        }
    }


    for(int iter_x = 0; iter_x < ncellsX; iter_x++)
    {
        double logical_x = logical_min[0] + logical_step[0] * iter_x;
        double physical_x = physical_min[0] + physical_step[0] * (iter_x-half_addition);
        for (int iter_y = 0; iter_y < ncellsY; iter_y++)
        {
            double logical_y = logical_min[1] + logical_step[1] * iter_y;
            double physical_y = physical_min[1] + physical_step[1] * (iter_y-half_addition);
            for (int iter_z = 0; iter_z < ncellsZ; iter_z++)
            {
                double physical_z = physical_min[2] + physical_step[2] * (iter_z-half_addition);
                double logical_z = logical_min[2] + logical_step[2] * iter_z;
                sample_points[iter_x][iter_y][iter_z] = glm::vec3(physical_x,physical_y,physical_z);
                if(m_boundingbox_logical.contains(Eigen::Vector3d(physical_x,physical_y,physical_z )))
                {
                    // Record the IS_value
                    double value = implicit_surface.f(logical_x, logical_y, logical_z);
                    if(value < 0)
                    {
                        IS_value[iter_x][iter_y][iter_z] = 1;
                    }
                    else
                    {
                        IS_value[iter_x][iter_y][iter_z] = 0;
                    }
                }
                else
                {
                    IS_value[iter_x][iter_y][iter_z] = 0;
                }
            }
        }
    }


    // Approximation of the all marching cubes
    for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)
    {
        for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
        {
            for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
            {
                int cube_index = 0;
                float isolevel = 0.5;
                // v0 v1 v2 v3 v4 v5 v6 v7
                if(IS_value[iter_x    ][iter_y    ][iter_z    ] < isolevel) cube_index |= 1;
                if(IS_value[iter_x + 1][iter_y    ][iter_z    ] < isolevel) cube_index |= 2;
                if(IS_value[iter_x + 1][iter_y    ][iter_z + 1] < isolevel) cube_index |= 4;
                if(IS_value[iter_x    ][iter_y    ][iter_z + 1] < isolevel) cube_index |= 8;
                if(IS_value[iter_x    ][iter_y + 1][iter_z    ] < isolevel) cube_index |= 16;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z    ] < isolevel) cube_index |= 32;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 64;
                if(IS_value[iter_x    ][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 128;

                // Get the approximation triangle table
                int *edgeIndexList = triTable[cube_index];

                // -1 means the end of the triangle.
                while (*edgeIndexList != -1 )
                {
                    // Get the next approximation triangle
                    int e_index[3];
                    e_index[0] = *edgeIndexList;
                    e_index[1] = *(edgeIndexList + 1);
                    e_index[2] = *(edgeIndexList + 2);
                    edgeIndexList = edgeIndexList+3;      // ptr move 3 pos

                    glm::ivec3 face_index;
                    // Calculate the 3 points coordinates of the face
                    for (int e_i=0; e_i < 3; e_i ++)
                    {
                        int index = getVertexIdx(iter_x, iter_y, iter_z, e_index[e_i], sample_points);
                        face_index[e_i] = index;
                    }   // for (int e_i=0; e_i < 3; e_i ++)

                    this->m_faces.push_back(face_index);
                }   // while (*edgelist != -1 )
            }   //  for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
        }   // for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
    }   // for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)
}

///
/// \brief MarchBox::marching_cubes
/// \param implicit_surface
/// \param depth
/// 将极小曲面模型生成有厚度的体积
///
void MarchBox::marching_cubes(ImplicitSurface &implicit_surface, float depth)
{

}

#ifdef USING_SURFACEMESH
void MarchBox::marching_cubes(ImplicitSurface &implicit_surface,
                              SurfaceMesh::SurfaceMeshModel &surface_mesh,
                              int sampleSize,
                              int density)
{
    // Update the two bounding box in the march_box
    surface_mesh.updateBoundingBox();
    Eigen::AlignedBox3d mesh_bbox =  surface_mesh.bbox();

    std::cout << "Mesh boundary:\n"
              << "MIN: X:\t" << mesh_bbox.min()[0] << "\tMAX: X:\t" << mesh_bbox.max()[0]
              << "\nMIN: Y:\t" << mesh_bbox.min()[1] << "\tMAX: Y:\t" << mesh_bbox.max()[1]
              << "\nMIN: Z:\t" << mesh_bbox.min()[2] << "\tMAX: Z:\t" << mesh_bbox.max()[2]
             << std::endl;

    if(sampleSize <= 0)
        sampleSize = m_sampleSize;
    if(density <= 0)
        density = m_density;
    this->setRange(mesh_bbox.max(), mesh_bbox.min(), sampleSize, density);

    // Create Octree
    Octree octree(&surface_mesh);

    // clear
    this->m_vertices.clear();
    this->m_faces.clear();
    this->m_index_map.clear();

    // Get parameters
    int ncellsX = m_ncellsX;
    int ncellsY = m_ncellsY;
    int ncellsZ = m_ncellsZ;

    Eigen::Vector3d logical_min = m_boundingbox_logical.min();
    Eigen::Vector3d logical_max = m_boundingbox_logical.max();
    Eigen::Vector3d physical_min = m_boundingbox_physical.min();
    Eigen::Vector3d physical_max = m_boundingbox_physical.max();

    // Calculate the step of each box
    Eigen::Vector3d logical_step = logical_max-logical_min;
    logical_step[0] = logical_step[0] / (ncellsX-1);
    logical_step[1] = logical_step[1] / (ncellsY-1);
    logical_step[2] = logical_step[2] / (ncellsZ-1);

    Eigen::Vector3d physical_step = physical_max - physical_min;
    physical_step[0] = physical_step[0] / (ncellsX - 1);
    physical_step[1] = physical_step[1] / (ncellsY - 1);
    physical_step[2] = physical_step[2] / (ncellsZ - 1);

    qDebug() << "\nImplicit Mesh type: " << implicit_surface.m_type
             << "\nnum of X: " << ncellsX
             << "\nnum of Y: " << ncellsY
             << "\nnum of Z: " << ncellsZ;

    // To record the value of [ncellsX x ncellsY x ncellsZ] sample points
    std::vector<std::vector<std::vector<int>>> IS_value(
                ncellsX,std::vector<std::vector<int>>(
                    ncellsY, std::vector<int>(
                        ncellsZ, 0)));

    std::vector<std::vector<std::vector<glm::vec3>>> sample_points(
            ncellsX, std::vector<std::vector<glm::vec3>>(
                    ncellsY, std::vector<glm::vec3>(
                        ncellsZ, glm::vec3(0.0))));

    int count_sum_of_1 = 0; // use for debug
    int count_sum_of_0 = 0; // use for debug
    Eigen::Vector3d up_direction(0,0,1);

    // init the record values and the sample_points of the marching cubes
    for(int iter_x = 0; iter_x < ncellsX; iter_x++)
    {
        double logical_x = logical_min[0] + logical_step[0] * iter_x;
        double physical_x = physical_min[0] + physical_step[0] * iter_x;
        for (int iter_y = 0; iter_y < ncellsY; iter_y++)
        {
            double logical_y = logical_min[1] + logical_step[1] * iter_y;
            double physical_y = physical_min[1] + physical_step[1] * iter_y;

            for (int iter_z = 0; iter_z < ncellsZ; iter_z++)
            {
                double logical_z = logical_min[2] + logical_step[2] * iter_z;
                double physical_z = physical_min[2] + physical_step[2] * iter_z;

                sample_points[iter_x][iter_y][iter_z] = glm::vec3(physical_x, physical_y, physical_z);
                // Record the IS_value
                Ray ray(Eigen::Vector3d(physical_x, physical_y, physical_z),
                        up_direction);
                QSet<int> intersects = octree.intersectRay(ray, 0.01, false);
                HitResult res;
                int intersect_count = 0;
                // Fast, not robust tests
                for( int i: intersects)
                {
                    octree.intersectionTestAccelerated(SurfaceMesh::Model::Face(i), ray, res);

                    // find the nearest intersection point
                    if(res.hit)
                        intersect_count++;
                }

                // 奇数个交点说明顶点在网格内部
                if(intersect_count %2 ==1)
                {
                    double value = implicit_surface.f(logical_x, logical_y, logical_z);
                    if(value > 0)
                    {
                        IS_value[iter_x][iter_y][iter_z] = 1;
                        count_sum_of_1 ++;
                    }
                    else
                    {
                        IS_value[iter_x][iter_y][iter_z] = 0;
                        count_sum_of_0 ++;
                    }
                }
                else
                {
                    // 顶点在网格外边直接为 0
                    IS_value[iter_x][iter_y][iter_z] = 0;
                    count_sum_of_0 ++;
                }

            }
        }
    }

    std::cout << "Count the num of points at the out of Implicit surface : " << count_sum_of_1
              << "\nCount the num of points at the inner of Implicit surface :" << count_sum_of_0
              << std::endl;

    // Approximation of the all marching cubes
    for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)
    {
        for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
        {
            for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
            {
                // Get the eight point of the cube vertex,
                // to March the condition in 256 probailities

                int cube_index = 0;
                float isolevel = 0.5;
                // v0 v1 v2 v3 v4 v5 v6 v7
                if(IS_value[iter_x    ][iter_y    ][iter_z    ] < isolevel) cube_index |= 1;
                if(IS_value[iter_x + 1][iter_y    ][iter_z    ] < isolevel) cube_index |= 2;
                if(IS_value[iter_x + 1][iter_y    ][iter_z + 1] < isolevel) cube_index |= 4;
                if(IS_value[iter_x    ][iter_y    ][iter_z + 1] < isolevel) cube_index |= 8;
                if(IS_value[iter_x    ][iter_y + 1][iter_z    ] < isolevel) cube_index |= 16;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z    ] < isolevel) cube_index |= 32;
                if(IS_value[iter_x + 1][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 64;
                if(IS_value[iter_x    ][iter_y + 1][iter_z + 1] < isolevel) cube_index |= 128;

                // Get the approximation triangle table
                int *edgeIndexList = triTable[cube_index];

                // -1 means the end of the triangle.
                while (*edgeIndexList != -1 )
                {
                    // Get the next approximation triangle
                    int e_index[3];
                    e_index[0] = *edgeIndexList;
                    e_index[1] = *(edgeIndexList + 1);
                    e_index[2] = *(edgeIndexList + 2);

                    edgeIndexList = edgeIndexList+3;      // ptr move 3 pos

                    glm::ivec3 face_index;
                    // Calculate the 3 points coordinates of the face
                    for (int e_i=0; e_i < 3; e_i ++)
                    {
                        int index = getVertexIdx(iter_x, iter_y, iter_z, e_index[e_i], sample_points);
                        face_index[e_i] = index;
                    }   // for (int e_i=0; e_i < 3; e_i ++)

                    this->m_faces.push_back(face_index);

                }   // while (*edgelist != -1 )

            }   //  for (int iter_z = 0; iter_z < ncellsZ-1; iter_z++)
        }   // for (int iter_y = 0; iter_y < ncellsY-1; iter_y++)
    }   // for(int iter_x = 0; iter_x < ncellsX-1; iter_x++)

}
#endif


void MarchBox::writeOBJ(const std::string &fileName)
{
    std::cout << "Writing OBJ file" << std::endl;
    // check if we actually have an ISO surface
    if(this->m_vertices.size () == 0 || this->m_faces.size() == 0) {
        std::cout << "No ISO surface generated. Skipping OBJ generation." << std::endl;
        return;
    }

    // open output file
    std::ofstream file(fileName);
    if(!file) {
        std::cout << "Error opening output file" << std::endl;
        return;
    }

    std::cout << "Generating OBJ mesh with " << m_vertices.size() << " vertices and "
      << m_faces.size() << " faces" << std::endl;

    // write vertices
    for(auto const & v : m_vertices) {
        file << "v " << v.x << ' ' << v.y << ' ' << v.z << '\n';
    }

    // write face indices
    for(auto const & f : m_faces) {
        file << "f " << (f.x + 1) << ' ' << (f.y + 1) << ' ' << (f.z + 1) << '\n';
    }

    file.close();
}


int MarchBox::getVertexIdx(int cx_id, int cy_id, int cz_id, int edge_idx, std::vector<std::vector<std::vector<glm::vec3> > > &sample_points)
{
    // 向公共边最小的那个方形看齐
    static std::unordered_map<int, int> edge_id_map_x{
        {3, 1},
        {7, 5},
        {8, 9},
        {11, 10}};
    static std::unordered_map<int, int> edge_id_map_y{
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}};
    static std::unordered_map<int, int> edge_id_map_z{
        {0, 2},
        {4, 6},
        {8, 11},
        {9, 10}};

    if(cx_id > 0 && edge_id_map_x.find(edge_idx) != edge_id_map_x.end())
    {
        edge_idx = edge_id_map_x[edge_idx];
        cx_id--;
    }
    if(cy_id > 0 && edge_id_map_y.find(edge_idx) != edge_id_map_y.end())
    {
        edge_idx = edge_id_map_y[edge_idx];
        cy_id--;
    }
    if(cz_id > 0 && edge_id_map_z.find(edge_idx) != edge_id_map_z.end())
    {
        edge_idx = edge_id_map_z[edge_idx];
        cz_id--;
    }

    int64_t key = cx_id * m_ncellsY * m_ncellsZ * 12
            + cy_id * m_ncellsZ * 12
            + cz_id * 12
            + edge_idx;

    if(m_index_map.find(key) != m_index_map.end())
    {
        return m_index_map[key];
    }
    else
    {
        // Calculate vertex
        glm::vec3 v0 = sample_points[cx_id    ][cy_id    ][cz_id    ];
        glm::vec3 v1 = sample_points[cx_id + 1][cy_id    ][cz_id    ];
        glm::vec3 v2 = sample_points[cx_id + 1][cy_id    ][cz_id + 1];
        glm::vec3 v3 = sample_points[cx_id    ][cy_id    ][cz_id + 1];
        glm::vec3 v4 = sample_points[cx_id    ][cy_id + 1][cz_id    ];
        glm::vec3 v5 = sample_points[cx_id + 1][cy_id + 1][cz_id    ];
        glm::vec3 v6 = sample_points[cx_id + 1][cy_id + 1][cz_id + 1];
        glm::vec3 v7 = sample_points[cx_id    ][cy_id + 1][cz_id + 1];

        glm::vec3 pos;
        switch(edge_idx)
        {
        case 0:
            pos = (v0 + v1) / glm::vec3(2);
            break;
        case 1:
            pos = (v1 + v2) / glm::vec3(2);
            break;
        case 2:
            pos = (v2 + v3) / glm::vec3(2);
            break;
        case 3:
            pos = (v0 + v3) / glm::vec3(2);
            break;
        case 4:
            pos = (v4 + v5) / glm::vec3(2);
            break;
        case 5:
            pos = (v5 + v6) / glm::vec3(2);
            break;
        case 6:
            pos = (v6 + v7) / glm::vec3(2);
            break;
        case 7:
            pos = (v4 + v7) / glm::vec3(2);
            break;
        case 8:
            pos = (v0 + v4) / glm::vec3(2);
            break;
        case 9:
            pos = (v1 + v5) / glm::vec3(2);
            break;
        case 10:
            pos = (v2 + v6) / glm::vec3(2);
            break;
        case 11:
            pos = (v3 + v7) / glm::vec3(2);
            break;
        }
        // Add into m_vertices
        this->m_vertices.push_back(pos);

        // add into map
        int index = this->m_vertices.size() - 1;
        m_index_map[key] = index;
        // return idx
        return index;
    }
}

