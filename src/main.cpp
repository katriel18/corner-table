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
//#include <vector>
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
void matrizAdyacencia();

//CODIGO
void readMeshFiles(){


 ifstream fin;

 fin.open("src/meshes/mesh3.mesh",ios::in);
 //fin.open("src/meshes/prueba03.mesh",ios::in);

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

			 double a=0.0,b=0.0,c=0.0;

			 fin>>a>>b>>c;

			 //cout<<"puntos "<<i+1<<": "<<endl;
			 //cout<<a<<" "<<b<<" "<<c<<'\n';

			 vertexList[3*i]=a/2;
						vertexList[3*i+1]=b/2;
								   vertexList[3*i+2]=c/2;

			//fin>>vertexList[3*i]>>vertexList[3*i+1]>>vertexList[3*i+2];

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
void matrizAdyacencia(){


	cout<<"Num Triangles"<<CT->getNumTriangles()<<endl;
	 /*	 for(int i=0;i<ct->getNumTriangles()*3;i++){
	 		 cout<<"TRIANGLE: "<<ct->cornerTriangle(i)<<" Vert: "<<ct->cornerToVertexIndex(i);
	 		 cout<<" CORNER: "<<i<<" Previus: "<<ct->cornerPrevious(i);
	 		 cout<<" Next: "<<ct->cornerNext(i);
	 		cout<<" Opuesto: "<<ct->cornerOpposite(i)<<endl;
	 			 cout<<"T IZQ: "<<ct->cornerTriangle(ct->cornerRight(i));
	 			cout<<" T DER: "<<ct->cornerTriangle(ct->cornerLeft(i))<<endl;
	 			cout<<endl;
	 	 }
	 	 cout<<"_________________________"<<endl;
	 	 */
	CT->printTriangleList();

	 	int matrizTriangle[CT->getNumTriangles()][CT->getNumTriangles()];
	 	 for (unsigned int m = 0; m < CT->getNumTriangles(); m++){
	 	 	        for (unsigned int j = 0; j < CT->getNumTriangles(); j++)
	 	 	        {
	 	 	        	matrizTriangle[m][j] =  0;
	 	 	        }
	 	 	   }



	 for(unsigned int i=0;i<CT->getNumTriangles()*3;i++){
	 		CornerType  triangleRigth =CT->cornerTriangle(CT->cornerRight(i));
	 		if( triangleRigth <=CT->getNumTriangles()){
	 			cout<<"T "<<CT->cornerTriangle(i)<< "Tizq"<<triangleRigth<<endl;
	 			matrizTriangle[CT->cornerTriangle(i)][triangleRigth]=1;
	 			//matrizTriangle[triangleRigth][ct->cornerTriangle(i)]=1;
	 		}
	 	}


	 	 cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;


	 	 cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
	 		 for (unsigned int i = 0; i < CT->getNumTriangles(); i++){
	 	 	        for (unsigned int j = 0; j < CT->getNumTriangles(); j++)
	 	 	        {
	 	 	            cout<<matrizTriangle[i][j]<<" ";

	 	 	        }
	 	 	        cout<<endl;
	 	 	    }



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


    GLuint m_EBO;
    glGenBuffers(1, &m_EBO);
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
   glPointSize(2.0f);
   glDrawElements(GL_TRIANGLES, CT->getNumTriangles()*3, GL_UNSIGNED_INT, 0);

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

    GLFWwindow* window = glfwCreateWindow(1200,600, "Corner Table", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    readMeshFiles();
    matrizAdyacencia();

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
