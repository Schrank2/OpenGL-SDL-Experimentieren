#ifndef WORLD_H // only defines if variable has not been defined yet
#define WORLD_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "defs.h"

// declare game so game.cpp can use it
struct Voxel {
	bool exists = false;
	RGBA_int color = RGBA_int(0,0,0,0);
	Voxel(bool exists, RGBA_int color) : exists(exists), color(color) {}
};

struct Chunk{
	bool generated = false;
	int x, y, z;
	vector<Voxel> VoxelStorage;
	vector<Voxel> CreateVoxelStorage() {
		RGBA_int RGBA = RGBA_int(0, 0, 0, 0);
		VoxelStorage.resize(4096, Voxel(false, RGBA));
		return VoxelStorage;
	}
	void generate() {
		int ix, iy, iz, index, worldZ;
		ix = iy = iz = index = 0;
		RGBA_int StoneColor = RGBA_int(100, 100, 100, 255);
		RGBA_int DirtColor = RGBA_int(150, 120, 100, 255);
		RGBA_int GrassColor = RGBA_int(120, 150, 100, 255);
		for(;ix < 16; ix++)
			for(;iy < 16; iy++)
				for (; iz < 16; iz++) {
					index = 16 * ix + 16 * iy + iz;
					worldZ = 16 * z + iz;
					if (worldZ < 5) VoxelStorage[index] = Voxel(true, StoneColor);
					if (worldZ < 7) VoxelStorage[index] = Voxel(true, DirtColor);
					if (worldZ < 8) VoxelStorage[index] = Voxel(true, GrassColor);
				}
	}
	Chunk(int x, int y, int z) : x(x), y(y), z(z), VoxelStorage(CreateVoxelStorage())  {}
};

class WORLD {
public:
	vector<Chunk> VoxelMap;
	vector<Point> Points;
	vector<Line> Lines;
	vector<SpaceTriangle> Triangles;
	void init(int* ScreenWidth, int* ScreenHeight);
	void tick();
	float TickStrength = 0.0f;
	bool DebugMenuShown = false;
	int ScreenWidth;
	int ScreenHeight;
	float ScreenWidthF;
	float ScreenHeightF;
	vector<ModelObject> ModelObjectQueue;
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;



#endif // WORLD_H