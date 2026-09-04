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
	vector<SpaceTriangle> VoxelModel;
	Pos A = Pos(0.0f, 0.0f, 0.0f);
	Pos B = Pos(1.0f, 0.0f, 0.0f);
	Pos C = Pos(1.0f, 1.0f, 0.0f);
	Pos D = Pos(0.0f, 1.0f, 0.0f);
	Pos E = Pos(0.0f, 0.0f, 1.0f);
	Pos F = Pos(1.0f, 0.0f, 1.0f);
	Pos G = Pos(1.0f, 1.0f, 1.0f);
	Pos H = Pos(0.0f, 1.0f, 1.0f); 
	Pos Center = Pos(0.5f, 0.5f, 0.5f);
	RGBA_int VoxelColor = RGBA_int(255, 0, 0, 255);
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


	int worldSize = 3;
	int index = 0;
	vector<vector<Voxel>> VoxelMap;
	VoxelMap.resize(worldSize, vector<Voxel>(worldSize*worldSize, Voxel(false,RGBA_int(0,0,0,0))));
	RGBA_int RED = RGBA_int(255, 0, 0, 255);
	if (false) {
		for (int x = 0; x < worldSize; x++) {
			for (int y = 0; y < worldSize; y++) {
				for (int z = 0; z < worldSize; z++) {
					index = y * worldSize + z;
					if (z % 2 == 0) VoxelMap[x][index] = Voxel(true, RED);
				}
			}
		}
	}

	vector<vector<Pos>> VoxelTriangles;
	for(int x = 0; x < worldSize; x++) {
		for(int y = 0; y < worldSize; y++) {
			for(int z = 0; z < worldSize; z++) {
				Voxel* CurrentVoxel = &VoxelMap[x][y * worldSize + z];
				if(CurrentVoxel->exists == true) {
					ModelObjectQueue.push_back(ModelObject(&VoxelModel, Pos(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)), CurrentVoxel->color));
				}
			}
		}
	}
	for(int i = 0; i < ModelObjectQueue.size(); i++) {
		simple.TranslateModelObject(&ModelObjectQueue[i], &Triangles);
	}


	// add Points to World
	Points.push_back(Point('O', Pos( 0.0f, 0.0f, 0.0f ), black));
	Points.push_back(Point('A', Pos(2.0f, 2.0f, 2.0f ), bright_red));
	Points.push_back(Point('B', Pos(2.0f, 0.0f, 0.0f ), bright_green));
	Points.push_back(Point('C', Pos(0.0f, 0.0f, 2.0f ), bright_blue));
	Points.push_back(Point('D', Pos(4.0f, 4.0f, -3.0f ), bright_red));
	Points.push_back(Point('E', Pos(0.0f, 0.05f, 0.0f ), bright_green));
	Points.push_back(Point('X', Pos(1.0f, 0.0f, 0.0f ), dark_blue));
	Points.push_back(Point('Y', Pos(0.0f, 1.0f, 0.0f ), dark_red));
	Points.push_back(Point('Z', Pos(0.0f, 0.0f, 1.0f ), dark_green));

	// add Triangles to World
	Triangles.push_back(SpaceTriangle(Points[1].position, Points[2].position, Points[3].position, bright_red));
	Triangles.push_back(SpaceTriangle(Points[0].position, Points[2].position, Points[3].position, bright_blue));
	Triangles.push_back(SpaceTriangle(Points[0].position, Points[1].position, Points[3].position, bright_green));
	Triangles.push_back(SpaceTriangle(Points[0].position, Points[1].position, Points[2].position, bright_orange));

	// add Lines to World
	//Lines.push_back(Line(Points[0], Points[6], 'x', dark_blue));
	//Lines.push_back(Line(Points[0], Points[7], 'y', dark_red));
	//Lines.push_back(Line(Points[0], Points[8], 'z', dark_green));
}

void WORLD::tick() {
	if (debug == true) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
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