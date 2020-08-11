//============================================================================
// Name        : Program 1, Drawing one Point
// Editado: Grupo de CGV
// Description : Avance del trabajo grupal, configuraciones
//============================================================================
//guias  Lab 02 Primitives,lab4.0
// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "CornerTable.h"
#include <fstream>
#include "DefinitionTypes.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

using namespace std;

GLuint m_VAO;
GLuint m_VBO;

int dimVertices;
int numberOfVertices;




CornerType numberTriangles;
CornerType numberVertices ;

CornerType* triangleList ;//almacena la pos del primer elemento

double* vertexList;//almacena la psocion del primer elemento

CornerTable *CT;
void readMeshFiles();
void init(double currentTime);
void display(double currentTime);

void readMeshFiles(){
 //ifstream fin("./meshes/mesh2.mesh");
 ifstream fin;


 fin.open("src/meshes/prueba.mesh",ios::in);


 //First Line OFF
 string name;
 //Second Line
 int npoint,ntriangle, var3;
 // npoint +1 Line edges that form triangle
 int var0;

 if (fin.fail()){

	 cout<<"error del archivo"<<endl;
	 exit(1);
 }else{

 // while(!fin.eof()){
		 fin>>name;
		 fin>>npoint>>ntriangle>>var3;
		 /* Llenamos la lista de puntos */
		 vertexList= new double[3*npoint];

		 for (int i = 0; i < npoint; i++){
			fin>>vertexList[3*i]>>vertexList[3*i+1]>>vertexList[3*i+2];
			//cout<<vertexList[3*i]<<" "<<vertexList[3*i+1]<<" "<<vertexList[3*i+2] <<'\n';
		 }

		 numberTriangles= ntriangle;
		 numberVertices = npoint;
		 /* Fill the list with all the points */
		 triangleList= new CornerType[3*ntriangle];

		 for (int i = 0; i <ntriangle; i++){
			fin>>var0>>triangleList[i*3]>>triangleList[i*3+1]>>triangleList[i*3+2];
			//cout<<var0<<" "<<triangleList[i*3]<<" "<<triangleList[i*3+1]<<" "<<triangleList[i*3+2]<<'\n';
		 }

  }
 fin.close();




 int numberCoordinatesByVertex = 3;
 CornerTable *ct =new CornerTable(triangleList, vertexList, numberTriangles, numberVertices, numberCoordinatesByVertex );
 //cout<<"# triangulos: "<<ct->getNumTriangles()<<endl;
 //cout<<"# verstices: "<<ct->getNumberVertices()<<endl;
 CT = ct;



}

void init(double currentTime){
	// Create vertices
	numberOfVertices = 3; // NUM VERTICES
 	dimVertices = numberOfVertices * 3; // NUM VERTICES* 3, TOTAL DE ELEMENTOS



/*	GLfloat m_Vertices [] ={
 			-1.0f, -1.0f, 0.0f,
 			1.0f, -1.0f, 0.0f,
 			0.0f, 1.0f, 0.0f
 		};
*/
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData( // LLEVA LOS VERTICES A LA MEMORIA DE TRABAJO
			GL_ARRAY_BUFFER,
			10*3*sizeof(double),
			(void*)CT->getAttributes(),	//m_Vertices,
			GL_STATIC_DRAW
	);


}

void display(double currentTime) {




	// Clear the screen
	// Dark blue background
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		glClear( GL_COLOR_BUFFER_BIT);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glVertexAttribPointer(
			0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_DOUBLE,           // type
			GL_FALSE,           // normalized?
			0,                  // stride , VA A TOMAR SALTO O TOMA DE UNO EN UNO,
			(void*) 0            // array buffer offset
	);
	//glEnableVertexAttribArray(0);
	// Draw the triangle !
	//glBindVertexArray(m_VAO);
	glPointSize(20.0f);
	glDrawArrays( GL_LINE_LOOP, 0, 10); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
}




int main( void ){

	// Initialise GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(600, 600, "Lab 01.2 - Draw triangle", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	readMeshFiles();
	init(glfwGetTime());


	 //	cout<<"# triangulos: "<<CT->getNumTriangles()<<endl;
	 	//cout<<"# vertices: "<<CT->getNumberVertices()<<endl;

	cout<<"printTriangleList: "<<endl;
	double* a=CT->getAttributes() ;//guardamos el primer lugar del vector

	 for (int i = 0; i < 14; i++){

		cout<<*a++<<endl;
		 }


	// Check if the window was closed
	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup VBO
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

