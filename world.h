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
	vector<Voxel> VoxelStorage;
	vector<Voxel> CreateVoxelStorage() {
		RGBA_int RGBA = RGBA_int(0, 0, 0, 0);
		vector<Voxel> VoxelStorageInit;
		VoxelStorageInit.resize(16*16*16*16, Voxel(false, RGBA));
		cout << "VoxelStorage Initialisation Size: " << VoxelStorageInit.size() << endl;
		return VoxelStorageInit;
	}
	void generate(int x, int y, int z) {
		generated = true;
		//cout << "generating Chunk" << endl;
		if (VoxelStorage.empty()) {
			//cout << "Pregeneration Size: " << VoxelStorage.size();
			VoxelStorage = CreateVoxelStorage();
			//cout << " Postgeneration Size: " << VoxelStorage.size() << endl;
		}
		int index, worldY;
		index = worldY = 0;
		RGBA_int StoneColor = RGBA_int(100, 100, 100, 255);
		RGBA_int DirtColor = RGBA_int(255, 0, 0, 255);
		RGBA_int GrassColor = RGBA_int(0, 0, 255, 255);
		for(int ix = 0;ix < 16; ix++)
			for(int iy = 0;iy < 16; iy++)
				for (int iz = 0; iz < 16; iz++) {
					index = (16 * 16 * ix) + (16 * iy) + iz;
					worldY = (16 * y) + iy;
					if (worldY < 5) VoxelStorage[index] = Voxel(true, StoneColor);
					if (worldY < 7) VoxelStorage[index] = Voxel(true, DirtColor);
					if (worldY < 8) VoxelStorage[index] = Voxel(true, GrassColor);
				}
	}
	Chunk() : VoxelStorage(CreateVoxelStorage())  {}
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
	int VoxelMapSize = 16;
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;



#endif // WORLD_H