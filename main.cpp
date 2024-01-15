//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul XI - 11_01_umbra.cpp |
// ====================================
// 
//	Program ce deseneaza o casa si care suprinde efectele de umbrire folosindu-se tehnicile MODERN OpenGL; 
//	Elemente de NOUTATE (modelul de iluminare):
//	 - pentru a genera umbra unei surse este utilizata o matrice 4x4;
//	 - in shaderul de varfuri este inclusa si matricea umbrei;
//	 - in shaderul de fragment umbra este colorata separat;
//	 - sursa de lumina este punctuala(varianta de sursa directionala este comentata);
// 
//  
// 
//	Biblioteci
#include <windows.h>        //	Utilizarea functiilor de sistem Windows (crearea de ferestre, manipularea fisierelor si directoarelor);
#include <stdlib.h>         //  Biblioteci necesare pentru citirea shaderelor;
#include <stdio.h>
#include <math.h>			//	Biblioteca pentru calcule matematice;
#include <GL/glew.h>        //  Definește prototipurile functiilor OpenGL si constantele necesare pentru programarea OpenGL moderna; 
#include <GL/freeglut.h>    //	Include functii pentru: 
							//	- gestionarea ferestrelor si evenimentelor de tastatura si mouse, 
							//  - desenarea de primitive grafice precum dreptunghiuri, cercuri sau linii, 
							//  - crearea de meniuri si submeniuri;
#include "loadShaders.h"	//	Fisierul care face legatura intre program si shadere;
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <random>
#include <iostream>




//  Identificatorii obiectelor de tip OpenGL; 
GLuint
VaoId_ground,
VboId_ground,
EboId_ground,
VaoId_sphere,
VboId_sphere,
EboId_sphere,
VaoId_cylinder,
VboId_cylinder,
EboId_cylinder,
VaoId_rock,
VboId_rock,
EboId_rock,
ColorBufferId,
ProgramId,
ProgramIdcon,
myMatrixLocation,
matrUmbraLocation,
viewLocation,
projLocation,
matrRotlLocation,
lightColorLocation,
lightPosLocation,
viewPosLocation,
codColLocation;

int codCol;
float PI = 3.141592;

// matrice utilizate
glm::mat4 myMatrix, matrRot;

// elemente pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = PI / 8, beta = 0.0f, dist = 400.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0, Vy = 0.0, Vz = 1.0;
glm::mat4 view;

int const NR_PARR = 10, NR_MERID = 20;

float const U_MIN = -PI, U_MAX = PI, V_MIN = 0, V_MAX = -2 * PI;
float const U_MIN_sphere = -PI / 2, U_MAX_sphere = PI / 2, V_MIN_sphere = 0, V_MAX_sphere = 2 * PI;

float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;
float step_u_sphere = (U_MAX_sphere - U_MIN_sphere) / NR_PARR, step_v_sphere = (V_MAX_sphere - V_MIN_sphere) / NR_MERID;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -800.f, xwmax = 800, ywmin = -600, ywmax = 600, znear = 0.1, zfar = 1, fov = 45;
glm::mat4 projection;

// sursa de lumina
float xL = 10.f, yL = 300.f, zL = 600.f;

int index, index_aux;

float radius_sphere = 25.0f;
float radius_cylinder = 10.0f;

float a = 25.0f, b = 45.0f;

float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f;

// matricea umbrei
float matrUmbra[4][4];

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		Vx -= 0.1;
		break;
	case 'r':
		Vx += 0.1;
		break;
	case '+':
		dist += 5;
		break;
	case '-':
		dist -= 5;
		break;
	case 'w':
		Refx -= 20;
		break;
	case 's':
		Refx += 20;
		break;
	case 'a':
		Refy -= 20;
		break;
	case 'd':
		Refy += 20;
		break;



	}
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		beta -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.01;
		break;
	case GLUT_KEY_UP:
		alpha += 0.01;
		break;
	case GLUT_KEY_DOWN:
		alpha -= 0.01;
		break;
	}
}



void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] =
	{
		// coordonate                   // culori			// normale
		// varfuri "ground"
	   -1500.0f,  -1500.0f, 0.0f, 1.0f,  0.68f, 0.47f, 0.2f,  0.0f, 0.0f, 1.0f,
		1500.0f,  -1500.0f, 0.0f, 1.0f,  0.68f, 0.47f, 0.2f,  0.0f, 0.0f, 1.0f,
		1500.0f,  1500.0f,  0.0f, 1.0f,  0.68f, 0.47f, 0.2f,  0.0f, 0.0f, 1.0f,
	   -1500.0f,  1500.0f,  0.0f, 1.0f,  0.68f, 0.47f, 0.2f,  0.0f, 0.0f, 1.0f,
	   // varfurile cubului
	   -50.0f,  -50.0f, 0.0f, 1.0f,   1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   50.0f,  -50.0f,  0.0f, 1.0f,  1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   50.0f,  50.0f,  0.0f, 1.0f,   1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   -50.0f,  50.0f,  0.0f, 1.0f,    1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,

	   -50.0f,  -50.0f, 150.0f, 1.0f,  1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   50.0f,  -50.0f,  150.0f, 1.0f, 1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   50.0f,  50.0f,  150.0f, 1.0f,  1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,
	   -50.0f,  50.0f, 150.0f, 1.0f,   1.0f, 0.5f, 0.2f,  0.0f, 0.0f, 1.0f,

	   // varfuri con
		-40.0f, -69.28f, 0.0f, 1.0f,   0.1f, 1.0f, 0.2f, -40.0f, -69.28f, 80.0f,
		40.0f, -69.28f, 0.0f, 1.0f,    0.1f, 1.0f, 0.2f, 40.0f, -69.28f, 80.0f,
		80.0f, 0.0f, 0.0f, 1.0f,       0.1f, 1.0f, 0.2f, 80.0f, 0.0f, 80.0f,
		40.0f, 69.28f, 0.0f, 1.0f,     0.1f, 1.0f, 0.2f, 40.0f, 69.28f, 80.0f,
	   -40.0f, 69.28f, 0.0f, 1.0f,     0.1f, 1.0f, 0.2f, -40.0f, 69.28f, 80.0f,
	   -80.0f, 0.0f,  0.0f, 1.0f,      0.1f, 1.0f, 0.2f, -80.0f, 0.0f, 80.0f,
		 0.0f, 0.0f, 100.0f, 1.0f,     0.3f, 1.0f, 0.2f, 0.0f, 0.0f, 1.0f,
	};

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
		// fetele "ground"
		 1, 2, 0,   2, 0, 3,
		 // fetele cubului
		 5, 6, 4,   6, 4, 7,
		 6, 7, 10, 10, 7, 11,
		 11, 7, 8,   8, 7, 4,
		 8, 4, 9,   9, 4, 5,
		 5, 6, 9,   9, 6, 10,
		 9, 10, 8,  8, 10, 11,
		 // fetele conului
		 12, 13, 18,
		 13, 14, 18,
		 14, 15, 18,
		 15, 16, 18,
		 16, 17, 18,
		 17, 12, 18
	};

	glGenVertexArrays(1, &VaoId_ground);
	glGenBuffers(1, &VboId_ground);
	glGenBuffers(1, &EboId_ground);
	glBindVertexArray(VaoId_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VboId_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	// atributul 1 = culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	// atributul 2 = normale
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void CreateVBO_rock(void)
{
	// varfurile 
	GLfloat Vertices[] =
	{
		// coordinates              // colors         // normals
		10.0f,  10.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,
		20.0f,   0.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,
		40.0f,  10.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,
		40.0f,  30.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,
		20.0f,  40.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,
		10.0f,  20.0f,  0.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, -1.0f,

		10.0f,  10.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
		20.0f,   0.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
		40.0f,  10.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
		40.0f,  30.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
		20.0f,  40.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
		10.0f,  20.0f,  10.0f, 1.0f,   0.71f, 0.75f, 0.81f,   0.0f, 0.0f, 1.0f,
	};

	// indicii pentru varfuri
	GLubyte Indices[] =
	{
		0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5,
		0, 6, 1, 6, 1, 7,
		7, 1, 2, 7, 2, 8,
		8, 2, 9, 9, 2, 3,
		9, 3, 10, 10, 3, 4,
		10, 4, 5, 10, 5, 11,
		11, 5, 0, 11, 6, 0,
		6, 7, 8, 6, 8, 9, 6, 9, 10, 6, 10, 11

	};

	glGenVertexArrays(1, &VaoId_rock);
	glGenBuffers(1, &VboId_rock);
	glGenBuffers(1, &EboId_rock);
	glBindVertexArray(VaoId_rock);

	glBindBuffer(GL_ARRAY_BUFFER, VboId_rock);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId_rock);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	// atributul 1 = culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	// atributul 2 = normale
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void CreateVBO_sphere(void) /// acum e spheroid
{
	// SFERA
	// Matricele pentru varfuri, culori, indici
	glm::vec4 Vertices1[(NR_PARR + 1) * NR_MERID];
	glm::vec3 Colors1[(NR_PARR + 1) * NR_MERID];
	GLushort Indices1[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = U_MIN_sphere + parr * step_u_sphere; // valori pentru u si v
			float v = V_MIN_sphere + merid * step_v_sphere;
			//float x_vf = radius_sphere * cosf(u) * cosf(v); // coordonatele varfului corespunzator lui (u,v)
			//float y_vf = radius_sphere * cosf(u) * sinf(v);
			//float z_vf = 70 + radius_sphere * sinf(u);

			float x_vf = a * sinf(v) * cosf(u); // coordonatele varfului corespunzator lui (u,v)
			float y_vf = a * sinf(v) * sinf(u);
			float z_vf = b * cosf(v) + 80;

			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			index = merid * (NR_PARR + 1) + parr;
			Vertices1[index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			Colors1[index] = glm::vec3(0.0, 1.0, 0.1);
			Indices1[index] = index;

			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (NR_MERID)+merid;
			Indices1[(NR_PARR + 1) * NR_MERID + index_aux] = index;

			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index; // varful v considerat
				int index2 = index + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}
				Indices1[AUX + 4 * index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices1[AUX + 4 * index + 1] = index2;
				Indices1[AUX + 4 * index + 2] = index3;
				Indices1[AUX + 4 * index + 3] = index4;
			}
		}
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId_sphere);
	glBindVertexArray(VaoId_sphere);
	glGenBuffers(1, &VboId_sphere); // atribute
	glGenBuffers(1, &EboId_sphere); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId_sphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1) + sizeof(Colors1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices1), Vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices1), sizeof(Colors1), Colors1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId_sphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices1)));
}

void CreateVBO_cylinder(void)
{
	// CILINDRU
	glm::vec4 Vertices2[(NR_PARR + 1) * NR_MERID];
	glm::vec3 Colors2[(NR_PARR + 1) * NR_MERID];
	GLushort Indices2[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;
			float x_vf = radius_cylinder * cosf(v);
			float y_vf = radius_cylinder * sinf(v);
			float z_vf = radius_cylinder * 4.0f * sinf(u) + 25.0f;

			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			index = merid * (NR_PARR + 1) + parr;
			Vertices2[index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			Colors2[index] = glm::vec3(0.5, 0.2, 0.1);
			Indices2[index] = index;

			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (NR_MERID)+merid;
			Indices2[(NR_PARR + 1) * NR_MERID + index_aux] = index;

			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index; // varful v considerat
				int index2 = index + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}
				Indices2[AUX + 4 * index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices2[AUX + 4 * index + 1] = index2;
				Indices2[AUX + 4 * index + 2] = index3;
				Indices2[AUX + 4 * index + 3] = index4;
			}
		}
	};

	glGenVertexArrays(1, &VaoId_cylinder);
	glBindVertexArray(VaoId_cylinder);
	glGenBuffers(1, &VboId_cylinder); // atribute
	glGenBuffers(1, &EboId_cylinder); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId_cylinder);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2) + sizeof(Colors2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices2), Vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices2), sizeof(Colors2), Colors2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId_cylinder);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices2)));
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId_ground);
	glDeleteBuffers(1, &EboId_ground);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId_ground);
	glDeleteBuffers(1, &VboId_sphere);
	glDeleteBuffers(1, &EboId_sphere);

	glDeleteBuffers(1, &VboId_cylinder);
	glDeleteBuffers(1, &EboId_cylinder);
	glDeleteVertexArrays(1, &VaoId_cylinder);

	glDeleteBuffers(1, &VboId_rock);
	glDeleteBuffers(1, &EboId_rock);
	glDeleteVertexArrays(1, &VaoId_rock);

}

void CreateShaders(void)
{
	ProgramId = LoadShaders("example.vert", "example.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	myMatrix = glm::mat4(1.0f);
	matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));
	glClearColor(0.68f, 0.91f, 0.92f, 0.0f);
	CreateVBO();
	CreateVBO_sphere();
	CreateVBO_cylinder();
	CreateVBO_rock();
	CreateShaders();
	// locatii pentru shader-e
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	lightColorLocation = glGetUniformLocation(ProgramId, "lightColor");
	lightPosLocation = glGetUniformLocation(ProgramId, "lightPos");
	viewPosLocation = glGetUniformLocation(ProgramId, "viewPos");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
}

void createSphere(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat * rotateMat;

	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindVertexArray(VaoId_sphere);
	codCol = 5;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	// shadow
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindVertexArray(VaoId_sphere);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}
}

void createCylinder(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat * rotateMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindVertexArray(VaoId_cylinder);
	codCol = 6;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	//shadow
	myMatrix = myMatrix * scaleMat * rotateMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glBindVertexArray(VaoId_cylinder);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

}

void createCone(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
}

void createParallelipiped(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {

	glBindVertexArray(VaoId_ground);
	codCol = 0;
	glUniform1i(codColLocation, codCol);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	myMatrix = glm::translate(transform, glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));

	// glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));


	// shadow
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));
}

void createWindow(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	// window frame
	createParallelipiped(translate_x + 50.0f, translate_y - 30.0f, translate_z + 100.0f, 0.05f, 0.03f, 0.2f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.0f, translate_y - 10.0f, translate_z + 100.0f, 0.05f, 0.03f, 0.2f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 49.9f, translate_y - 20.0f, translate_z + 100.0f, 0.05f, 0.015f, 0.2f, angle, rotate_x, rotate_y, rotate_z);

	createParallelipiped(translate_x + 50.0f, translate_y - 20.0f, translate_z + 100.0f, 0.05f, 0.17f, 0.02f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 49.9f, translate_y - 20.0f, translate_z + 100.0f + 0.27f / 4 * 100.0f, 0.05f, 0.17f, 0.01f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 49.9f, translate_y - 20.0f, translate_z + 100.0f + 0.27f / 2 * 100.0f, 0.05f, 0.17f, 0.01f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 49.9f, translate_y - 20.0f, translate_z + 100.0f + 3 * 0.27f / 4 * 100.0f, 0.05f, 0.17f, 0.01f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.0f, translate_y - 20.0f, translate_z + 100.0f + 0.27f * 100.0f, 0.05f, 0.17f, 0.02f, angle, rotate_x, rotate_y, rotate_z);

	// window glass
	createParallelipiped(translate_x + 49.8f, translate_y - 20.0f, translate_z + 100.0f, 0.05f, 0.22f, 0.2f, angle, rotate_x, rotate_y, rotate_z);
}

void createDoor(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createParallelipiped(translate_x + 50.0f, translate_y - 30.0f, translate_z + 80.0f, 0.05f, 0.03f, 0.33f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.0f, translate_y - 10.0f, translate_z + 80.0f, 0.05f, 0.03f, 0.33f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.0f, translate_y - 20.0f, translate_z + 100.0f + 0.265f * 100.0f, 0.05f, 0.17f, 0.02f, angle, rotate_x, rotate_y, rotate_z);

	createParallelipiped(translate_x + 49.5f, translate_y - 20.0f, translate_z + 80.0f, 0.05f, 0.2f, 0.33f, angle, rotate_x, rotate_y, rotate_z);

	createParallelipiped(translate_x + 49.6f, translate_y - 15.0f, translate_z + 100.0f, 0.05f, 0.015f, 0.05f, angle, rotate_x, rotate_y, rotate_z);
}

void createBuilding(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	// 100.0f L
	glBindVertexArray(VaoId_ground);
	codCol = 0;
	glUniform1i(codColLocation, codCol);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	myMatrix = glm::translate(transform, glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));

	myMatrix = myMatrix * scaleMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));

	// shadow
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));
}

void createChimney(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	// con
	// cilindru
	createCylinder(translate_x, translate_y, translate_z, 0.5f, 0.5f, 0.1f, angle, rotate_x, rotate_y, rotate_z);
}

void createRoof(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createParallelipiped(translate_x - 25.0f, translate_y - 25.0f, translate_z + 150.2f, 0.11f, 0.11f, 0.01f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 25.0f, translate_y - 5.0f, translate_z + 151.2f, 0.11f, 0.11f, 0.01f, angle, rotate_x, rotate_y, rotate_z);

	// createParallelipiped(translate_x + 20.0f, translate_y - 25.0f, translate_z + 150.2f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
}

void CreateBlock(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createParallelipiped(translate_x, translate_y, translate_z, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);

	// beams
	createParallelipiped(translate_x + 50.0f, translate_y, translate_z + 150.0f, 0.05f, 1.0f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.0f, translate_y, translate_z + 150.0f, 0.05f, 1.0f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x, translate_y + 50.0f - 0.05 * 50.0f, translate_z + 150.0f, 1.0f - 0.025f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x, translate_y - 50.0f + 0.05 * 50.0f, translate_z + 150.0f, 1.0f - 0.025f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);

	createParallelipiped(translate_x + 50.0f, translate_y, translate_z, 0.05f, 1.0f + 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.0f, translate_y, translate_z, 0.05f, 1.0f + 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x, translate_y + 50.0f, translate_z, 1.0f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x, translate_y - 50.0f, translate_z, 1.0f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);


	// windows
	createWindow(translate_x, translate_y, translate_z + 15.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
	createWindow(translate_x, translate_y + 40.0f, translate_z + 15.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
	createWindow(translate_x, translate_y, translate_z - 25.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
	createWindow(translate_x, translate_y + 40.0f, translate_z - 25.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
	createWindow(translate_x, translate_y, translate_z - 75.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);


	// door
	createDoor(translate_x, translate_y + 40.0f, translate_z - 75.0f, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);


	// wall
	createBuilding(translate_x, translate_y, translate_z, 1.01f, 1.01f, 1.0f / 2.5f, angle, rotate_x, rotate_y, rotate_z);


	// bricks
	// front
	createParallelipiped(translate_x + 50.2f, translate_y + 40.0f, translate_z + 60.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.2f, translate_y + 30.0f, translate_z + 67.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.2f, translate_y, translate_z + 50.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.2f, translate_y - 46.0f, translate_z + 80.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 50.2f, translate_y + 46.0f, translate_z + 130.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);

	// back
	createParallelipiped(translate_x - 50.2f, translate_y + 40.0f, translate_z + 60.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.2f, translate_y + 30.0f, translate_z + 67.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.2f, translate_y, translate_z + 50.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.2f, translate_y - 46.0f, translate_z + 80.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 50.2f, translate_y + 46.0f, translate_z + 130.0f, 0.05f, 0.1f, 0.03f, angle, rotate_x, rotate_y, rotate_z);

	// right
	createParallelipiped(translate_x, translate_y + 50.2f, translate_z + 82.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 22.0f, translate_y + 50.2f, translate_z + 90.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 12.0f, translate_y + 50.2f, translate_z + 120.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 40.0f, translate_y + 50.2f, translate_z + 10.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);

	// left
	createParallelipiped(translate_x, translate_y - 50.2f, translate_z + 82.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 22.0f, translate_y - 50.2f, translate_z + 130.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 2.0f, translate_y - 50.2f, translate_z + 20.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x - 45.0f, translate_y - 50.2f, translate_z + 70.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);
	createParallelipiped(translate_x + 35.0f, translate_y - 50.2f, translate_z + 110.0f, 0.1f, 0.05f, 0.03f, angle, rotate_x, rotate_y, rotate_z);

	// panel
	createParallelipiped(translate_x + 55.0f, translate_y - 50.0f, translate_z + 90.0f, 0.25f, 0.01f, 0.35f, angle, rotate_x, rotate_y, rotate_z);

	// roof
	createRoof(translate_x, translate_y, translate_z, 1.0f, 1.0f, 1.0f, angle, rotate_x, rotate_y, rotate_z);
}

void CreateTree(float x, float y, float z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createSphere(x, y, z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);
	createCylinder(x, y, z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);
}

void CreateGard(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createParallelipiped(translate_x, translate_y, translate_z + 17.0f, 0.018f, 1.85f, 0.02f, angle, rotate_x, rotate_y, rotate_z);

	for (int index = 85; index >= -87; index -= 12)
		createParallelipiped(translate_x, translate_y - index, translate_z, 0.02f, 0.032f, 0.16f, angle, rotate_x, rotate_y, rotate_z);

}

void createRock(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	glBindVertexArray(VaoId_rock);
	codCol = 0;
	glUniform1i(codColLocation, codCol);

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat * rotateMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_BYTE, (void*)(0));

	// shadow
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_BYTE, (void*)(0));
}

void CreateBumpyTerrain(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createRock(translate_x + 80.0f, translate_y - 85.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.35f, 0.25f, angle + PI / 2, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 50.0f, translate_y - 100.0f, translate_z + 0.02 * 150.0f, 0.25f, 0.25f, 0.25f, angle + PI / 2, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 30.0f, translate_y - 80.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.35f, 0.15f, angle + PI / 3, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 75.0f, translate_y - 30.0f, translate_z + 0.02 * 150.0f, 0.15f, 0.25f, 0.25f, angle + PI / 4, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 65.0f, translate_y + 40.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.35f, 0.35f, angle + PI / 5, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 20.0f, translate_y - 80.0f, translate_z + 0.02 * 150.0f, 0.05f, 0.05f, 0.25f, angle + PI / 4, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 50.0f, translate_y - 90.0f, translate_z + 0.02 * 150.0f, 0.15f, 0.1f, 0.15f, angle + PI / 6, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 60.0f, translate_y - 60.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.35f, 0.3f, angle + PI / 2, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 30.0f, translate_y + 60.0f, translate_z + 0.02 * 150.0f, 0.25f, 0.2f, 0.35f, angle + 1.0f, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x - 15.0f, translate_y + 75.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.3f, 0.2f, angle + PI / 4, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x, translate_y + 95.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.1f, 0.15f, angle + PI / 4, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 20.0f, translate_y + 50.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.35f, 0.25f, angle + PI / 4, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 43.0f, translate_y + 68.0f, translate_z + 0.02 * 150.0f, 0.05f, 0.1f, 0.15f, angle + PI / 8, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 60.0f, translate_y + 55.0f, translate_z + 0.02 * 150.0f, 0.35f, 0.2f, 0.35f, angle + PI / 2, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
	createRock(translate_x + 80.0f, translate_y + 80.0f, translate_z + 0.02 * 150.0f, 0.15f, 0.3f, 0.25f, angle + PI / 3, rotate_x + 0.0f, rotate_y + 0.0f, rotate_z + 1.0f);
}

void CreateResidence(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z) {
	createParallelipiped(translate_x + 20.0f, translate_y, translate_z, 2.1f, 2.1f, 0.02f, angle, rotate_x, rotate_y, rotate_z); // baza sub block

	CreateGard(translate_x + 120.0f, translate_y, translate_z - 5.0f, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z); // gard

	CreateBlock(translate_x, translate_y, translate_z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);

	translate_x < 0 ? translate_x *= -1 : translate_x;
	translate_y < 0 ? translate_y *= -1 : translate_y;

	CreateTree(translate_x, translate_y - 75.0f, translate_z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);
	CreateTree(translate_x, translate_y + 75.0f, translate_z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);

	CreateBumpyTerrain(translate_x, translate_y, translate_z, scale_x, scale_y, scale_z, angle, rotate_x, rotate_y, rotate_z);
}

void createRoadBand(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z)
{
	glBindVertexArray(VaoId_ground);
	codCol = 3;
	glUniform1i(codColLocation, codCol);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	myMatrix = glm::translate(transform, glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));

	// glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));


	// shadow
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));
}

void createCurb(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z)
{
	glBindVertexArray(VaoId_ground);
	codCol = 4;
	glUniform1i(codColLocation, codCol);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	myMatrix = glm::translate(transform, glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));

	// glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));

	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));
}


void createRoad(float translate_x, float translate_y, float translate_z, float scale_x, float scale_y, float scale_z, float angle, float rotate_x, float rotate_y, float rotate_z, bool copac)
{
	//Drumul:
	glBindVertexArray(VaoId_ground);
	codCol = 2;
	glUniform1i(codColLocation, codCol);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	myMatrix = glm::translate(transform, glm::vec3(translate_x, translate_y, translate_z));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));

	// glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotate_x, rotate_y, rotate_z));

	myMatrix = myMatrix * scaleMat;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(6));

	
	float bandsOffset = 50.0f;
	float bands = scale_y;
	for (int i = 0; i < bands; i++) {
		createRoadBand(translate_x, translate_y - (i * bandsOffset), translate_z + 0.5, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	}

	for (int i = 1; i < bands; i++) {
		createRoadBand(translate_x, translate_y + (i * bandsOffset), translate_z + 0.5, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	}
	// Liniile:
	//createRoadBand(translate_x, translate_y, translate_z + 20.01, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	//createRoadBand(translate_x, translate_y + 50.0f, translate_z + 20.01, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	//createRoadBand(translate_x, translate_y + 100.0f, translate_z + 20.01, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	//createRoadBand(translate_x, translate_y + 150.0f, translate_z + 20.01, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);
	//createRoadBand(translate_x, translate_y + 200.0f, translate_z + 20.01, scale_x / 50.0f, scale_y / 50.0f, scale_z / 50.0f, angle, rotate_x, rotate_y, rotate_z);

	// Bordurile

	createCurb(translate_x + 40.0f, translate_y + 0.5f, translate_z, scale_x / 20.0f, scale_y, scale_z + 0.05f, angle, rotate_x, rotate_y, rotate_z);
	createCurb(translate_x - 40.0f, translate_y + 0.5f, translate_z, scale_x / 20.0f, scale_y, scale_z + 0.05f, angle, rotate_x, rotate_y, rotate_z);

	// Copacei

	if (copac == true) {
		for (int i = 0; i <= 5; i++) {
			CreateTree(translate_x + 50.0f, translate_y + i * bandsOffset, translate_z, 1 / 2.0f, 1 / 2.0f, 1 / 2.0f, angle, rotate_x, rotate_y, rotate_z);

		}
		for (int i = 1; i <= 5; i++) {
			CreateTree(translate_x + 50.0f, translate_y - i * bandsOffset, translate_z, 1 / 2.0f, 1 / 2.0f, 1 / 2.0f, angle, rotate_x, rotate_y, rotate_z);

		}
	}

}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-1400.0, 1400.0);
std::vector<std::pair<std::pair<float,float>, float>> randomNumbers(200);
std::uniform_real_distribution<float> dis2(0.5, 1.2);


void randomNumbersForForest() {	
	for (int i = 0; i < randomNumbers.size(); i++) {
		 
		float xtranslate = dis(gen);
		float ytranslate = dis(gen);
		float scale = dis2(gen);
		if (xtranslate >= -100.0f && xtranslate <= 550.0f && ytranslate >= -200.0f && ytranslate <= 950) {
			i--;
			continue;
		}
		randomNumbers[i].first.first = xtranslate;
		randomNumbers[i].first.second = ytranslate;
		randomNumbers[i].second = scale;
	}
}

void createForest() {
	for (int i = 0; i < randomNumbers.size(); i++) {
		CreateTree(randomNumbers[i].first.first, randomNumbers[i].first.second, 0.0f, randomNumbers[i].second, randomNumbers[i].second, randomNumbers[i].second, 0.0f, 0.0f, 0.0000001f, 0.0f);
	}
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glTranslatef(-cameraX, -cameraY, -cameraZ);

	//pozitia observatorului
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	// matrice de vizualizare + proiectie
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// matricea pentru umbra
	float D = -0.5f;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

	// Variabile uniforme pentru iluminare
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLocation, xL, yL, zL);
	glUniform3f(viewPosLocation, Obsx, Obsy, Obsz);

	

	glBindVertexArray(VaoId_ground);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	myMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	// blocuri

	

	CreateResidence(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(0.0f, 210.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(0.0f, 600.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(0.0f, 810.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);


	CreateResidence(-500.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 180.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(-500.0f, -210.0f, 0.0f, 1.0f, 1.0f, 1.0f, 180.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(-500.0f, -600.0f, 0.0f, 1.0f, 1.0f, 1.0f, 180.0f, 0.0f, 0.0f, 1.0f);
	CreateResidence(-500.0f, -810.0f, 0.0f, 1.0f, 1.0f, 1.0f, 180.0f, 0.0f, 0.0f, 1.0f);



	// drum

	createRoad(200.0f, 400.0f, 0.7f, 0.8f, 11.0f, 0.001f, 0.0f, 0.0f, 0.0f, 1.0f, 1);

	createRoad(300.0f, 400.0f, 0.7f, 0.8f, 11.0f, 0.001f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

	/*createRoad(167.0f, 105.0f, 0.7f, 0.9f, 4.2f, 0.001f, 0.0f, 0.0f, 0.0f, 1.0f, 1);

	createRoad(335.0f, 105.0f, 0.7f, 0.9f, 4.2f, 0.001f, 0.0f, 0.0f, 0.0f, 1.0f, 0);*/

	static bool isFirstRun = true;
	if (isFirstRun) {
		randomNumbersForForest();
		isFirstRun = false;
	}
	
	createForest();
	

	glutSwapBuffers();
	glFlush();
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(350, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("11_01_umbra - OpenGL <<nou>>");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}