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


//CODIGO
void readMeshFiles(){

 ifstream fin;

 fin.open("src/meshes/mesh3.mesh",ios::in);
 //fin.open("src/meshes/prueba03.mesh",ios::in);

 int npoint,ntriangle, var3;
 // npoint +1 Line edges that form triangle
 int var0;

 string name;
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



			 vertexList[3*i]=a/2;
						vertexList[3*i+1]=b/2;
								   vertexList[3*i+2]=c/2;



		 }

		 numberTriangles= ntriangle;
		 numberVertices = npoint;

		 triangleList= new CornerType[3*ntriangle];

		 for (int i = 0; i <ntriangle; i++){
			fin>>var0>>triangleList[i*3]>>triangleList[i*3+1]>>triangleList[i*3+2];

		 }

  }
 fin.close();

 int numberCoordinatesByVertex = 3;
 CornerTable* ct =new CornerTable(triangleList, vertexList, numberTriangles, numberVertices, numberCoordinatesByVertex );

 CT = ct;
 	 cout<<"Num Triangles"<<ct->getNumTriangles()<<endl;

 	 ct->printTriangleList();

 	int matrizTriangle[ct->getNumTriangles()][ct->getNumTriangles()];
 	 for (int m = 0; m < ct->getNumTriangles(); m++){
 	 	        for (int j = 0; j < ct->getNumTriangles(); j++)
 	 	        {
 	 	        	matrizTriangle[m][j] =  0;
 	 	        }
 	 	   }



 for(int i=0;i<ct->getNumTriangles()*3;i++){
 		CornerType  triangleRigth =ct->cornerTriangle(ct->cornerRight(i));
 		if( triangleRigth <=ct->getNumTriangles()){
 			cout<<"T "<<ct->cornerTriangle(i)<< "Tizq"<<triangleRigth<<endl;
 			matrizTriangle[ct->cornerTriangle(i)][triangleRigth]=1;

 		}
 	}


 	 cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;


 	 cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
 		 for (int i = 0; i < ct->getNumTriangles(); i++){
 	 	        for (int j = 0; j < ct->getNumTriangles(); j++)
 	 	        {
 	 	            cout<<matrizTriangle[i][j]<<" ";

 	 	        }
 	 	        cout<<endl;
 	 	    }




}


void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);


    GLuint m_VBO;
    glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

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

}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);



    //GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,GL_TRIANGLES
   glPointSize(2.0f);
   //glDrawElements(GL_TRIANGLES, CT->getNumTriangles()*3, GL_UNSIGNED_INT, 0);
   glDrawElements(GL_LINE_STRIP, CT->getNumTriangles()*3, GL_UNSIGNED_INT, 0);
   glBegin(GL_LINES);
   	   glVertex2f(.5f,.5f);
   	   glVertex2f(.9f,9.f);
   glEnd();
}

int dijkstra (vvi& G, vvi& W, int n, int s, int t){
    vector <bool> vis(n, false); //nodos visitados, inicialmente ninguno
    priority_queue <ii> Q; //cola de prioridad
    Q.push({-0,s}); //introducimos el nodo origen (a distancia 0 de él mismo)

    while (not Q.empty()){
        ii arc = Q.top() //arco con menor peso desde F hasta G\F
        Q.pop(); //lo quitamos de la cola

        int v = arc.second; //vértice de Q a menor distancia del nodo origen
        int p = arc.first; //distancia entre el nodo origen y v

        if (v == t) return -p; //si es el nodo destino hemos acabado

        if (not vis[v]){ //si no lo hemos visitado
            vis[v] = true;
            for (int i = 0; i < G[v].size(); ++i){ //miramos sus vecinos
                int u = G[v][i];
                int w = W[v][i];

                Q.push({p-w, u}); // añadimos los vecinos con la distancia a s
            }
        }
    }
   return inf; // no hay caminos entre s y t
}

int main(void) {
	int a, b;
	            cout << "INGRESE EL PUNTO A"<<endl;
	            cout << "PUNTO A:" ;
	            cin >> a;
	            cout << "INGRESE EL PUNTO B"<<endl;
	            cout << "PUNTO B:" ;
	            cin >> b;
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(1200,800, "Corner Table -  Grupo de Jefferson Renzo Pumacarhua Alvarez", NULL, NULL);
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
