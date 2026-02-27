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
	int column[BUFFER_SIZE] = {0,1,2,3,1,3,2,0,1,3};
	int active[BUFFER_SIZE] = {0};
	float spawnTime[BUFFER_SIZE] = {};
	float hitTime[BUFFER_SIZE] = {
		3.0f, 4.3f, 5.9f, 8.1f, 9.5f,
		11.2f, 12.8f, 14.6f, 16.0f, 17.7f
	};

	CircularBufferFloat lanes[4];

	for (int i = 0; i < 4; i++) {
		lanes[i] = initCircularBufferFloat();
	}

	for (size_t i = 0; i < 10; i++) {
		int targetLane = column[i];
		if (targetLane >= 4) continue; 
		pushtoCircularBufferFloat(&lanes[targetLane], hitTime[i]);
	}
	


	float scrollSpeed = 5;
	float preTime = 1200 - (scrollSpeed - 5) * 140;
	float speed = (1444 - yPosInitial) / preTime;


	int render = 0;
	int done = 0;

	float hitTimer = 0.0f;
	const char* hitText = "";
	Color hitIndicator = transparent;
	int hitCombo = 0;
	int score = 0;


	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		spawnTime[i] = hitTime[i] * 1000 - preTime;
	}
	
	
	float timer = 0;

	while (!WindowShouldClose())
	{
		timer = (float) GetTime() * 1000;
		BeginDrawing();
		ClearBackground(PURPLE);

		if (render < BUFFER_SIZE)
		{
			if(!active[render] && timer >= spawnTime[render]){
				active[render] = 1;
				yPos[render] = -160;
				render++;
			}
		}

		for (int i = done; i < render; i++)
		{
			DrawRectangleRec((Rectangle){4 + (float)column[i] * tilesWidthFloat, yPos[i], tilesWidthFloat, 160}, multipleColor[column[i]]);
			yPos[i] += speed * GetFrameTime() * 1000;
		}
		
		for (int i = 0; i < 4; i++)
		{
			DrawRectangleRec(hitBox[i],transparent);
			DrawRectangleLinesEx(hitBox[i],2.0f,WHITE);
			DrawRectangleRec(hitButtons[i],PINK);
			DrawRectangleLinesEx(hitButtons[i],2.0f,BLACK);

		}

		for (size_t i = 0; i < 4; i++)
		{	
			if (peekCircularBufferFloat(&lanes[i]) == 0) continue;
			
			float checktiming = peekCircularBufferFloat(&lanes[i]) * 1000.0f;
			float diff = timer - checktiming;
			if (diff > 180.0f) {
				printf("Miss!\n");
				hitText = "Miss!";
    			hitTimer = 1.0f;
				hitIndicator = RED;
				hitCombo = 0;
				popfromCircularBufferFloat(&lanes[i]);
				done++;
			}
		}
		

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePos = GetMousePosition();
			for (int i = 0; i < 4; i++)
			{
				float checktiming = peekCircularBufferFloat(&lanes[i]) * 1000.0f;
				float diff = fabsf(timer - checktiming);
				if (CheckCollisionPointRec(mousePos, hitButtons[i])) {

					// printf("Target: %.2fms | Current: %.2fms | Diff: %.2fms\n", checktiming, timer, diff);

					if (diff < 60.0f) {
						printf("Perfect!\n");
						hitText = "Perfect!";
						hitTimer = 1.0f;
						hitIndicator = GOLD;
						hitCombo++;
						score += hitCombo * 300;
						popfromCircularBufferFloat(&lanes[i]);
						done++;
					} else if (diff < 120.0f) {
						printf("Great!\n");
						hitText = "Great!";
						hitTimer = 1.0f;
						hitIndicator = BLUE;
						hitCombo++;
						score += hitCombo * 200;
						popfromCircularBufferFloat(&lanes[i]);
						done++;
					} else if (diff < 180.0f) {
						printf("Good!\n");
						hitText = "Good!";
						hitTimer = 1.0f;
						hitIndicator = GREEN;
						hitCombo++;
						score += hitCombo * 100;
						popfromCircularBufferFloat(&lanes[i]);
						done++;
					}
				}
			}
		}

		if (hitTimer > 0) {
			int textWidth = MeasureText(hitText, 32);
			int centeredX = 540 - (textWidth / 2); 
			DrawText(hitText, centeredX, 1300, 32, hitIndicator);
			hitTimer -= GetFrameTime();
		}

		DrawText(TextFormat("%i", score), 100, 1300, 24, WHITE);
		DrawText(TextFormat("%i", hitCombo), 980 - MeasureText(TextFormat("%i", hitCombo),24), 1300, 24, WHITE);
	
		EndDrawing();
	}


	CloseWindow();
	return 0;
}
