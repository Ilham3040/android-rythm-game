/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

/* next task 

- implement circular buffer

*/

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "circularbuffer.h"

const int screenWidth = 1080;
const int screenHeight = 1920;
const Color semiBlack = {0, 0, 0, 128};
const Color transparent = {0, 0, 0, 0};
const int tilesWidth = screenWidth / 4;
Color multipleColor[] = {RED,YELLOW,GREEN,BLUE};
const float tilesWidthFloat = (float)tilesWidth;




int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT |  FLAG_WINDOW_HIGHDPI);

	InitWindow(1080, 1920, "Hello Raylib");

	Rectangle hitBox[4];
	Rectangle hitButtons[4];
	for (int i = 0; i < 4; i++)
	{
		hitBox[i] = (Rectangle){0+i*tilesWidth,1440,tilesWidth,168};
		hitButtons[i] = (Rectangle) {0+i*tilesWidth,1608,tilesWidthFloat,308};
	}
	

	float yPosInitial = -160;

	float yPos[BUFFER_SIZE] = {yPosInitial};
	int column[BUFFER_SIZE] = {0,1,2,3,1,4,2,0,1,3};
	int active[BUFFER_SIZE] = {0};
	float spawnTime[BUFFER_SIZE] = {};
	float hitTime[BUFFER_SIZE] = {15.2, 16.5, 18.1, 20.3, 21.7, 23.4, 25.0, 26.8, 28.2, 29.9};

	CircularBufferFloat lanes[4];

	for (int i = 0; i < 4; i++) {
		lanes[i] = initCircularBufferFloat();
	}

	for (size_t i = 0; i < 10; i++) {
		int targetLane = column[i];
		if (targetLane >= 4) continue; 
		pushtoCircularBufferFloat(&lanes[targetLane], hitTime[i]);
	}
	


	float ar = 2;
	float preempt = 1.2;
	float approachTiming = (preempt - (0.15 * (ar - 5)));
	float speed;


	int render = 0;
	int done = 0;


	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		spawnTime[i] = hitTime[i] - approachTiming;
	}
	
	
	float timer = 0;

	while (!WindowShouldClose())
	{
		
		timer = (float) GetTime() * 1000;
		speed = (1444 - yPosInitial) / approachTiming;
		

		BeginDrawing();


		ClearBackground(PURPLE);

		if (render < BUFFER_SIZE)
		{
			if(!active[render] && GetTime() >= spawnTime[render]){
				active[render] = 1;
				yPos[render] = -160;
				render++;
			}

			if(yPos[done] == 1920) {
				done++;
			}

			for (int i = done; i < render; i++)
			{
				DrawRectangleRec((Rectangle){4 + (float)column[i] * tilesWidthFloat, yPos[i], tilesWidthFloat, 160}, multipleColor[column[i]]);
				yPos[i] += speed * GetFrameTime();
			}
		}
		
		

		for (int i = 0; i < 4; i++)
		{
			DrawRectangleRec(hitBox[i],transparent);
			DrawRectangleLinesEx(hitBox[i],2.0f,WHITE);
			DrawRectangleRec(hitButtons[i],PINK);
			DrawRectangleLinesEx(hitButtons[i],2.0f,BLACK);

		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePos = GetMousePosition();
		
			for (int i = 0; i < 4; i++)
			{
				if (CheckCollisionPointRec(mousePos, hitButtons[i])) {
					float checktiming = peekCircularBufferFloat(&lanes[i]) * 1000.0f;
					float diff = fabsf(timer - checktiming); // Correct float absolute value

					printf("Target: %.2fms | Current: %.2fms | Diff: %.2fms\n", checktiming, timer, diff);

					if (diff < 200.0f) {
						printf("Perfect!\n");
						popfromCircularBufferFloat(&lanes[i]);
					}
				}
			}

		}

		
	

				
		EndDrawing();
	}


	CloseWindow();
	return 0;
}
