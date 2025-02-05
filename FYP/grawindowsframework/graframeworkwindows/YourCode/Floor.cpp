#include "Floor.h"
#include "Scene.h"
using namespace std;
//#include "MyScene.h"
#define SIZE 50.0f
Floor::Floor(int mapWidth, int mapHeight, char buffer[][100], MapGenerator* mapGenerator) // past 2D array. need past array points
{
	generatorMap = mapGenerator;
	widthUnit = mapGenerator->GetMapWidth();
	heightUnit = mapGenerator->GetMapHeight();
	lightColour = 0.0f;
	scale = 10;
	wallHeight = 100.0f;
	glEnable(GL_TEXTURE_2D);
	LoadTexture();
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_NORMALIZE); //Automatically normalize normals
}

Floor::~Floor(void)
{
	glDisable(GL_TEXTURE_2D);
}


void Floor::DrawSingleFloor(int x, int z)
{

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(x * SIZE, -9.9f, z *SIZE);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f((x + 1) * SIZE, -9.9f, z * SIZE);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f((x + 1) * SIZE, -9.9f, (z + 1) *SIZE);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(x* SIZE, -9.9f, (z + 1)* SIZE);

}
void Floor::DrawSingleWall(int x, int z, float wallHeight, float y) // firstly, drawing normal floor, then drawing unit wall at the center of block, then extend the wall to their neighbours. 
{


	//Unite center wall
	glColor3f(1.0f, 1.0f, 1.0f);//white
	DrawUnitWall(x, z, wallHeight, y, 1, 1);
	//bottom : No bottom of wall is needed, as floor cover all bottom of room

	//Extend wall:
	if ((x - 1 >= 0) && (generatorMap->GetBufferChar(x - 1, z) == 'b' || generatorMap->GetBufferChar(x - 1, z) == 'c' || generatorMap->GetBufferChar(x - 1, z) == 'd'))
	{
		DrawUnitWall(x, z, wallHeight, y, 0, 1);
	}
	if ((x + 1 < widthUnit) && (generatorMap->GetBufferChar(x + 1, z) == 'b' || generatorMap->GetBufferChar(x + 1, z) == 'c' || generatorMap->GetBufferChar(x + 1, z) == 'd'))
	{
		DrawUnitWall(x, z, wallHeight, y, 2, 1);
	}
	if ((z + 1 < heightUnit) && (generatorMap->GetBufferChar(x, z + 1) == 'b' || generatorMap->GetBufferChar(x, z + 1) == 'c' || generatorMap->GetBufferChar(x, z + 1) == 'd'))
	{
		DrawUnitWall(x, z, wallHeight, y, 1, 2);
	}
	if ((z - 1 >= 0) && (generatorMap->GetBufferChar(x, z - 1) == 'b' || generatorMap->GetBufferChar(x, z - 1) == 'c' || generatorMap->GetBufferChar(x, z - 1) == 'd'))
	{
		DrawUnitWall(x, z, wallHeight, y, 1, 0);
	}
}
void Floor::DrawSingleWindow(int x, int z, float wallHeight, bool rowDirection)
{

	if (!rowDirection) //Draw verticle wall
	{
		//back
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE, -10.0f, z *SIZE + SIZE / 3);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f((x + 1) * SIZE, -10.0f, z * SIZE + SIZE / 3);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f((x + 1) * SIZE, wallHeight, z *SIZE + SIZE / 3);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x* SIZE, wallHeight, z * SIZE + SIZE / 3);
		//front
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE, -10.0f, z * SIZE + 2 * SIZE / 3);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f((x + 1) * SIZE, -10.0f, z * SIZE + 2 * SIZE / 3);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f((x + 1) * SIZE, wallHeight, z *SIZE + 2 * SIZE / 3);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x* SIZE, wallHeight, z * SIZE + 2 * SIZE / 3);
		//left
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE, -10.0f, z * SIZE + SIZE / 3);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(x * SIZE, -10.0f, z * SIZE + 2 * SIZE / 3);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(x * SIZE, wallHeight, z *SIZE + 2 * SIZE / 3);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x * SIZE, wallHeight, z * SIZE + SIZE / 3);
		//right
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f((x + 1) * SIZE, -10.0f, z * SIZE + SIZE / 3);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f((x + 1) * SIZE, -10.0f, z * SIZE + 2 * SIZE / 3);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f((x + 1) * SIZE, wallHeight, z *SIZE + 2 * SIZE / 3);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f((x + 1)* SIZE, wallHeight, z * SIZE + SIZE / 3);

	}
	else //Draw horizon wall
	{
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE + SIZE / 3, -10.0f, z *SIZE);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, -10.0f, z * SIZE);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, wallHeight, z *SIZE);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x* SIZE + SIZE / 3, wallHeight, z * SIZE);
		//front
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE + SIZE / 3, -10.0f, (z + 1)* SIZE);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, -10.0f, (z + 1) * SIZE);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, wallHeight, (z + 1)*SIZE);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x* SIZE + SIZE / 3, wallHeight, (z + 1)* SIZE);
		//left
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE + SIZE / 3, -10.0f, z * SIZE);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(x * SIZE + SIZE / 3, -10.0f, (z + 1) * SIZE);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(x * SIZE + SIZE / 3, wallHeight, (z + 1)*SIZE);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x * SIZE + SIZE / 3, wallHeight, z * SIZE);
		//right
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, -10.0f, z * SIZE);
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, -10.0f, (z + 1)* SIZE);
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, wallHeight, (z + 1) *SIZE);
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(x * SIZE + 2 * SIZE / 3, wallHeight, z * SIZE);
	}
	//bottom
	glVertex3f(x * SIZE, -10.0f, z *SIZE);
	glVertex3f((x + 1) * SIZE, -10.0f, z * SIZE);
	glVertex3f((x + 1) * SIZE, -10.0f, (z + 1) *SIZE);
	glVertex3f(x* SIZE, -10.0f, (z + 1)* SIZE);

}
void Floor::DrawRoof(int x, int z)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(0, wallHeight, 0);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(x * SIZE, wallHeight, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(x * SIZE, wallHeight, z *SIZE);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(0, wallHeight, z * SIZE);
}

void Floor::Draw()
{

	//glBindTexture should at the outside of glBegin and glEnd;
	if (!Scene::GetGameStart()) //detect the game state, if game start then begin drawing
	{

		widthUnit = generatorMap->GetMapWidth();
		heightUnit = generatorMap->GetMapHeight();

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_CULL_FACE);
		glTranslatef(-300.0f, -20.0f, -300.0f);


		for (int z = 0; z < heightUnit; z++)
		{
			for (int x = 0; x < widthUnit; x++)
			{

				switch (generatorMap->GetBufferChar(x, z)) // Each object should apply different transform action, using switch case here
				{
				case 'a':
				{
					DrawRoofAndFloor(x, z);
					break;
				}
				case 'b':
				{
					DrawRoofAndFloor(x, z);
					glBindTexture(GL_TEXTURE_2D, wallTexId);
					glBegin(GL_QUADS);
					DrawSingleWall(x, z, wallHeight, -10.0f);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);

					break;
				}
				case 'c':
				{
					DrawRoofAndFloor(x, z);
					glBindTexture(GL_TEXTURE_2D, wallTexId);
					glBegin(GL_QUADS);
					DrawSingleWall(x, z, wallHeight, wallHeight / 2);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);

					glBindTexture(GL_TEXTURE_2D, windowTexId);
					GLfloat lightColor0[] = { lightColour, lightColour, lightColour, 1.0f }; //Color (0.5, 0.5, 0.5)
					GLfloat lightPos0[] = { x * SIZE + SIZE / 2, wallHeight / 2, z * SIZE + SIZE / 2, 1.0f }; //Position
					glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
					glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
					glBegin(GL_QUADS);
					if ((generatorMap->GetBufferChar(x, z - 1) == 'b' || 
						generatorMap->GetBufferChar(x, z - 1) == 'c' || 
						generatorMap->GetBufferChar(x, z - 1) == 'd') &&
						(generatorMap->GetBufferChar(x, z + 1) == 'b' ||
						generatorMap->GetBufferChar(x, z + 1) == 'c' ||
						generatorMap->GetBufferChar(x, z + 1) == 'd'))
					{
						DrawSingleWindow(x, z, wallHeight / 2 + 0.5, true);
					}
					else
					{

						DrawSingleWindow(x, z, wallHeight / 2 + 0.5, false);
					}
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);
					break;
				}
				case 'd':
				{
					DrawRoofAndFloor(x, z);
					glBindTexture(GL_TEXTURE_2D, wallTexId);
					glBegin(GL_QUADS);
					DrawSingleWall(x, z, wallHeight, wallHeight / 2);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);

					glBindTexture(GL_TEXTURE_2D, doorTexId);
					glBegin(GL_QUADS);
					if ((generatorMap->GetBufferChar(x, z - 1) == 'b' ||
						generatorMap->GetBufferChar(x, z - 1) == 'c' ||
						generatorMap->GetBufferChar(x, z - 1) == 'd') &&
						(generatorMap->GetBufferChar(x, z + 1) == 'b' ||
						generatorMap->GetBufferChar(x, z + 1) == 'c' ||
						generatorMap->GetBufferChar(x, z + 1) == 'd'))
					{
						DrawSingleWindow(x, z, wallHeight / 2 + 0.5, true);
					}
					else
					{
						DrawSingleWindow(x, z, wallHeight / 2 + 0.5, false);
					}
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);
					break;

				}
				case 'e':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef((x - 2)*SIZE, -8.0f, z *SIZE + SIZE / 2);
					glScalef(35.0f, 35.0f, 35.0f);
					sofa.RenderModel();
					glPopMatrix();
					break;
				}
				case 'f':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(3.0f, 3.0f, 3.0f);
					bed.RenderModel();
					glPopMatrix();
					break;

				}
				case 'g':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(10.0f, 10.0f, 10.0f);
					chair.RenderModel();

					glPopMatrix();
					break;
				}
				case 'h':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, 0.0f, z*SIZE + SIZE / 2);
					glScalef(15.0f, 15.0f, 15.0f);
					coffeeTable.RenderModel();
					glPopMatrix();
					break;
				}
				case 'i':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(10.0f, 10.0f, 10.0f);
					toilet.RenderModel();
					glPopMatrix();
					break;
				}
				case 'j':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(5.0f, 5.0f, 5.0f);
					refrigerator.RenderModel();
					glPopMatrix();
					break;
				}
				case 'k':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(8.0f, 8.0f, 8.0f);
					television.RenderModel();
					glPopMatrix();
					break;
				}
				case 'l':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(5.0f, 5.0f, 5.0f);
					wardrobe.RenderModel();
					glPopMatrix();
					break;
				}
				case 'm':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(7.0f, 7.0f, 7.0f);
					bookCase.RenderModel();
					glPopMatrix();
					break;
				}
				case 'n':
				{
					DrawRoofAndFloor(x, z);
					glPushMatrix();
					glTranslatef(x*SIZE + SIZE / 2, -8.0f, z*SIZE + SIZE / 2);
					glScalef(6.0f, 6.0f, 6.0f);
					washingMachine.RenderModel();
					glPopMatrix();
					break;
				}
				default: //Drawing all need to judge the units around it, so that '0' used to fill the blank and make sure stack overflow won't happen
					break;
				}
			}
		}

		glPopMatrix();
		glEnable(GL_CULL_FACE);
		glPopAttrib();
	}

}
void Floor::Update(const double& deltatime) //Change light intensity based on time change
{

	if (lightChange)
	{
		lightColour += 0.1f * deltatime;
	}
	else
	{
		lightColour -= 0.1f * deltatime;
	}
	if (lightColour >= 1.0f)
	{
		lightChange = false;
	}
	if (lightColour <= 0.0f)
	{
		lightChange = true;
	}
}
void Floor::DrawUnitWall(int x, int z, float wallHeight, float y, int xSize, int ySize)
{
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, y, z *SIZE + ySize * SIZE / 3);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, y, z *SIZE + (1 + ySize) *  SIZE / 3);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, wallHeight, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, wallHeight, z *SIZE + ySize * SIZE / 3);
	//front
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, y, z *SIZE + ySize * SIZE / 3);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(x * SIZE + (1 + xSize) * SIZE / 3, y, z *SIZE + ySize * SIZE / 3);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(x * SIZE + (1 + xSize) *SIZE / 3, wallHeight, z *SIZE + ySize * SIZE / 3);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(x * SIZE + xSize *SIZE / 3, wallHeight, z *SIZE + ySize * SIZE / 3);
	//left
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(x * SIZE + (1 + xSize) *SIZE / 3, y, z *SIZE + ySize * SIZE / 3);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(x * SIZE + (1 + xSize) *SIZE / 3, y, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(x * SIZE + (1 + xSize) *SIZE / 3, wallHeight, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(x * SIZE + (1 + xSize) * SIZE / 3, wallHeight, z *SIZE + ySize * SIZE / 3);
	//right
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, y, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(x * SIZE + (1 + xSize) * SIZE / 3, y, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(x * SIZE + (1 + xSize) *SIZE / 3, wallHeight, z *SIZE + (1 + ySize) *   SIZE / 3);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(x * SIZE + xSize * SIZE / 3, wallHeight, z *SIZE + (1 + ySize) *   SIZE / 3);
}

void Floor::DrawRoofAndFloor(int x, int z)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, floorTexId);
	glBegin(GL_QUADS);
	DrawSingleFloor(x, z);
	glEnd();

	glTranslatef(0.0f, wallHeight + 9.9f, 0.0);
	glBindTexture(GL_TEXTURE_2D, ceilingTexId);
	glBegin(GL_QUADS);
	DrawSingleFloor(x, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void Floor::LoadTexture()
{
	coffeeTable.LoadOBJ("./Resources/models/CoffeeTable.obj");
	chair.LoadOBJ("./Resources/models/chair.obj");
	bed.LoadOBJ("./Resources/models/gtaBed.obj");
	sofa.LoadOBJ("./Resources/models/Sofa.obj");
	toilet.LoadOBJ("./Resources/models/Toilet.obj");
	refrigerator.LoadOBJ("./Resources/models/Refrigerator.obj");
	television.LoadOBJ("./Resources/models/television.obj");
	wardrobe.LoadOBJ("./Resources/models/Wardrobe.obj");
	bookCase.LoadOBJ("./Resources/models/Chest.obj");
	washingMachine.LoadOBJ("./Resources/models/KitchenTable.obj");

	wallTexId = Scene::GetTexture("./Resources/textures/wallPaper.bmp");
	windowTexId = Scene::GetTexture("./Resources/textures/window.bmp");
	floorTexId = Scene::GetTexture("./Resources/textures/floor.bmp");
	ceilingTexId = Scene::GetTexture("./Resources/textures/ceiling.bmp");
	doorTexId = Scene::GetTexture("./Resources/textures/RoomDoor.bmp");
}