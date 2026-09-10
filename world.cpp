#include <iostream>
#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "SimpleRenderer.h"
#include "world.h"
#include <vector>

vector<Point> Points;
vector<Line> Lines;
vector<SpaceTriangle> Triangles;
vector<SpaceTriangle> VoxelModel;
int worldSize = 20;
// bright RGB colors
RGBA_int bright_red(255, 0, 0, 255);
RGBA_int bright_blue(100, 100, 200, 255);
RGBA_int bright_green(0, 255, 0, 255);
RGBA_int bright_orange(255, 165, 0, 255);
// dark RGB colors
RGBA_int dark_red(128, 0, 0, 255);
RGBA_int dark_green(0, 128, 0, 255);
RGBA_int dark_blue(0, 0, 128, 255);
RGBA_int black(75, 75, 75, 255);

void WORLD::init(int* ScreenWidth, int* ScreenHeight) {
	if (debug == true) { cout << "[DEBUG] function world.init() from world.cpp" << endl; }
	world.ScreenWidth = *ScreenWidth;
	world.ScreenHeight = *ScreenHeight;
	world.ScreenWidthF = static_cast<float>(world.ScreenWidth);
	world.ScreenHeightF = static_cast<float>(world.ScreenHeight);
	Pos A = Pos(0.0f, 0.0f, 0.0f);
	Pos B = Pos(1.0f, 0.0f, 0.0f);
	Pos C = Pos(1.0f, 1.0f, 0.0f);
	Pos D = Pos(0.0f, 1.0f, 0.0f);
	Pos E = Pos(0.0f, 0.0f, 1.0f);
	Pos F = Pos(1.0f, 0.0f, 1.0f);
	Pos G = Pos(1.0f, 1.0f, 1.0f);
	Pos H = Pos(0.0f, 1.0f, 1.0f); 
	Pos Center = Pos(0.5f, 0.5f, 0.5f);
	RGBA_int VoxelColor = RGBA_int(0, 0, 0, 255);
	VoxelModel.push_back(SpaceTriangle(A, B, C, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(A, C, D, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(E, F, G, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(E, G, H, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(A, B, F, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(A, F, E, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(B, C, G, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(B, G, F, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(C, D, H, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(C, H, G, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(D, A, E, VoxelColor));
	VoxelModel.push_back(SpaceTriangle(D, E, H, VoxelColor));

	VoxelMap.resize(4096, Chunk());
}

void WORLD::tick() {
	if (debug) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
	simple.VoxelsToTrianglesTime = SDL_GetTicks();
	Triangles.clear();
	ModelObjectQueue.clear();
	// generating nearby chunks
	int GridCameraX = static_cast<int>(simple.Camera.pos.x);
	int GridCameraY = static_cast<int>(simple.Camera.pos.y);
	int GridCameraZ = static_cast<int>(simple.Camera.pos.z);
	int ChunkGridCameraX = static_cast<int>(floor(simple.Camera.pos.x / 16));
	int ChunkGridCameraY = static_cast<int>(floor(simple.Camera.pos.y / 16));
	int ChunkGridCameraZ = static_cast<int>(floor(simple.Camera.pos.z / 16));
	int GridCameraIndex = (GridCameraX * 16) + (GridCameraY * 16) + (GridCameraZ * 16);
	int RenderDistanceX = 1;
	int RenderDistanceY = 1;
	int RenderDistanceZ = 1;
	int CurrentChunkIndex = 0;
	for (int x = -RenderDistanceX; x <= RenderDistanceX; x++) {
		for (int y = -RenderDistanceY; y <= RenderDistanceY; y++) {
			for (int z = -RenderDistanceZ; z <= RenderDistanceZ; z++) {
				CurrentChunkIndex = (x + ChunkGridCameraX) * 16 + (y + ChunkGridCameraY) * 16 + (z + ChunkGridCameraZ);
				Chunk* C = &(VoxelMap[CurrentChunkIndex]);
				if (C->generated == false) {
					C->generate(x,y,z);
				}
				if (debug) cout << "[DEBUG] Rendering Chunk at x: " << x + ChunkGridCameraX << " y: " << y + ChunkGridCameraY << " z: " << z + ChunkGridCameraZ << endl;
				for (int X = 0; X < 16; X++) {
					for (int Y = 0; Y < 16; Y++) {
						for (int Z = 0; Z < 16; Z++) {
							int index = (X * 16) + (Y * 16) + Z;
							Voxel* CurrentVoxel = &(C->VoxelStorage[index]);
							if(CurrentVoxel->exists) {
								Pos VoxelPosition = Pos(X + x, Y + y, Z + z);
								ModelObjectQueue.push_back(ModelObject(&VoxelModel, VoxelPosition, CurrentVoxel->color));
							}
						}
					}
				}
			}
		}
	}



	for (int i = 0; i < ModelObjectQueue.size(); i++) {
		simple.TranslateModelObject(&(ModelObjectQueue[i]), &Triangles);
	}
	simple.VoxelsToTrianglesTime = SDL_GetTicks() - simple.VoxelsToTrianglesTime;
	// Update Camera Rotation
	simple.CameraYaw += 90.0f * (mainMouse.movex / world.ScreenWidthF) * mainMouse.sens;
	simple.CameraPitch += 90.0f * (mainMouse.movey / world.ScreenHeightF) * mainMouse.sens;
	mainMouse.movex = 0.0f;
	mainMouse.movey = 0.0f;
	
	// Keep Rotation within 0.0f to 360.0f
	if (simple.CameraYaw > 360.0f) simple.CameraYaw -= 360.0f;
	if (simple.CameraYaw < 0.0f) simple.CameraYaw += 360.0f;
	if (simple.CameraPitch > 360.0f) simple.CameraPitch -= 360.0f;
	if (simple.CameraPitch < 0.0f) simple.CameraPitch += 360.0f;
	if (simple.CameraYaw > 1000.0f or simple.CameraYaw <= -1000.0f) simple.CameraYaw = 0.0f;
	if (simple.CameraPitch > 1000.0f or simple.CameraPitch <= -1000.0f) simple.CameraPitch = 0.0f;
	// Update Movement
	float Yaw = simple.CameraYaw * (3.14 / 180.0f);
	float Pitch = simple.CameraPitch * (3.14 / 180.0f);
	float x = cos(Yaw) - sin(Yaw);
	float z = cos(Yaw) + sin(Yaw);
	// W
	if (mainInput[0].active == true) {
		simple.Camera.velocity.z += 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.x += 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.z += 10.0f * world.TickStrength;
	}
	// A
	if (mainInput[1].active == true) {
		simple.Camera.velocity.x -= 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.z += 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.x -= 10.0f * world.TickStrength;
	}
	// S
	if (mainInput[2].active == true) {
		simple.Camera.velocity.z -= 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.x -= 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.z -= 10.0f * world.TickStrength;
	}
	// D
	if (mainInput[3].active == true) {
		simple.Camera.velocity.x += 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.z -= 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.x += 10.0f * world.TickStrength;
	}

	if (mainInput[4].active == true) simple.Camera.velocity.y += 10.0f * world.TickStrength;
	if (mainInput[5].active == true) simple.Camera.velocity.y -= 10.0f * world.TickStrength;
	// Update Physics, later for all physics objects in the world
	simple.Camera.pos.x += simple.Camera.velocity.x * TickStrength;
	simple.Camera.pos.y += simple.Camera.velocity.y * TickStrength;
	simple.Camera.pos.z += simple.Camera.velocity.z * TickStrength;
	simple.Camera.velocity.x *= 0.9f;
	simple.Camera.velocity.y *= 0.9f;
	simple.Camera.velocity.z *= 0.9f;
}

WORLD world;