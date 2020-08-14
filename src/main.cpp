//============================================================================
// Name        : Corner Table
// Guias 	   : Lab03 , Lab04.2
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

GLuint renderingProgram;

GLfloat* m_Vertices;

using namespace std;

//NUEVAS LIBRERIAS
#include <vector>
#include "CornerTable.h"
#include <fstream>
#include "DefinitionTypes.h"
//NUEVAS VARIABLES
CornerType numberTriangles;
CornerType numberVertices ;
CornerType* triangleList ;//almacena la pos del primer elemento del array
double* vertexList;//almacena la posicion del primer elemento del array
CornerTable *CT;

//DECALARACION DE METODOS
void readMeshFiles();
void init (GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
//CODIGO
void readMeshFiles(){

 ifstream fin;

 //ifstream fin("./meshes/mesh2.mesh");
 fin.open("src/meshes/prueba02.mesh",ios::in);

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
 CornerTable* ct =new CornerTable(triangleList, vertexList, numberTriangles, numberVertices, numberCoordinatesByVertex );

 CT = ct;

}


void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");
    // Create Vertex Array Object
	GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint m_VBO;
    glGenBuffers(1, &m_VBO);
	/*m_Vertices = new GLfloat[20] {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};*/
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			3*CT->getNumberVertices()*sizeof(double),//20 * sizeof(GLfloat),	// tamanho do buffer
			(void*)CT->getAttributes(),	//m_Vertices,			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização

    // Create an element array
    GLuint m_EBO;
    glGenBuffers(1, &m_EBO);
   /* GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			CT->getNumTriangles()*3*sizeof(CornerType),//sizeof(elements),
			(void*)CT->getTriangleList(),//elements,
			GL_STATIC_DRAW);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(renderingProgram, "iPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(
			posAttrib,
			3,//2,
			GL_DOUBLE,//GL_FLOAT,
			GL_FALSE,
			0,//5 * sizeof(GLfloat),
			(void*) 0 //0
			);
/*
	GLint colAttrib = glGetAttribLocation(renderingProgram, "iColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(
			colAttrib,
			3,
			GL_FLOAT,
			GL_FALSE,
			5 * sizeof(GLfloat),
			(void*) (2 * sizeof(GLfloat)));*/
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,GL_TRIANGLES
   glPointSize(30.0f);
   glDrawElements(GL_POINTS, CT->getNumTriangles()*3, GL_UNSIGNED_INT, 0);

}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(600,600, "Corner Table", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    readMeshFiles();

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
