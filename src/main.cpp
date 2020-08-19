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

//////////////////////////////////////////////////////////////

//#include <stdio.h>
#include <vector>
#include <queue>
//using namespace std;
#define MAX 10005 //maximo numero de vértices
#define Node pair< int , int > //definimos el nodo como un par( first , second ) donde first es el vertice adyacente y second el peso de la arista
#define INF 1<<30 //definimos un valor grande que represente la distancia infinita inicial, basta conque sea superior al maximo valor del peso en alguna de las aristas

//La cola de prioridad de C++ por default es un max-Heap (elemento de mayor valor en el tope)
//por ello es necesario realizar nuestro comparador para que sea un min-Heap
struct cmp {
    bool operator() ( const Node &a , const Node &b ) {
        return a.second > b.second;
    }
};
vector< Node > ady[ MAX ]; //lista de adyacencia
int distancia[ MAX ];      //distancia[ u ] distancia de vértice inicial a vértice con ID = u
bool visitado[ MAX ];      //para vértices visitados
priority_queue< Node , vector<Node> , cmp > Q; //priority queue propia del c++, usamos el comparador definido para que el de menor valor este en el tope
int V;                     //numero de vertices
int previo[ MAX ];         //para la impresion de caminos

//función de inicialización
void init1(){
    for( int i = 0 ; i <= V ; ++i ){
        distancia[ i ] = INF;  //inicializamos todas las distancias con valor infinito
        visitado[ i ] = false; //inicializamos todos los vértices como no visitados
        previo[ i ] = -1;      //inicializamos el previo del vertice i con -1
    }
}

//Paso de relajacion
void relajacion( int actual , int adyacente , int peso ){
    //Si la distancia del origen al vertice actual + peso de su arista es menor a la distancia del origen al vertice adyacente
    if( distancia[ actual ] + peso < distancia[ adyacente ] ){
        distancia[ adyacente ] = distancia[ actual ] + peso;  //relajamos el vertice actualizando la distancia
        previo[ adyacente ] = actual;                         //a su vez actualizamos el vertice previo
        Q.push( Node( adyacente , distancia[ adyacente ] ) ); //agregamos adyacente a la cola de prioridad
    }
}

//Impresion del camino mas corto desde el vertice inicial y final ingresados
void print( int destino ){
    if( previo[ destino ] != -1 )    //si aun poseo un vertice previo
        print( previo[ destino ] );  //recursivamente sigo explorando
    cout<<destino<<" ";//printf("%d " , destino ); //terminada la recursion imprimo los vertices recorridos
}

void dijkstra( int inicial ){
    init1(); //inicializamos nuestros arreglos
    Q.push( Node( inicial , 0 ) ); //Insertamos el vértice inicial en la Cola de Prioridad
    distancia[ inicial ] = 0;      //Este paso es importante, inicializamos la distancia del inicial como 0
    int actual , adyacente , peso;
    while( !Q.empty() ){                   //Mientras cola no este vacia
        actual = Q.top().first;            //Obtengo de la cola el nodo con menor peso, en un comienzo será el inicial
        Q.pop();                           //Sacamos el elemento de la cola
        if( visitado[ actual ] ) continue; //Si el vértice actual ya fue visitado entonces sigo sacando elementos de la cola
        visitado[ actual ] = true;         //Marco como visitado el vértice actual

        for(unsigned int i = 0 ; i < ady[ actual ].size() ; ++i ){ //reviso sus adyacentes del vertice actual
            adyacente = ady[ actual ][ i ].first;   //id del vertice adyacente
            peso = ady[ actual ][ i ].second;        //peso de la arista que une actual con adyacente ( actual , adyacente )
            if( !visitado[ adyacente ] ){        //si el vertice adyacente no fue visitado
                relajacion( actual , adyacente , peso ); //realizamos el paso de relajacion
            }
        }
    }

/*
    printf( "Distancias mas cortas iniciando en vertice %d\n" , inicial );
    for( int i = 1 ; i <= V ; ++i ){
        printf("Vertice %d , distancia mas corta = %d\n" , i , distancia[ i ] );
    }

    puts("\n**************Impresion de camino mas corto**************");
    printf("Ingrese vertice destino: ");
    int destino;
    scanf("%d" , &destino );
    print( destino );
    printf("\n");*/

    cout<< "Distancias mas cortas iniciando en vertice "<< inicial <<endl;
        for( int i = 1 ; i <= V ; ++i ){
        	cout<<"Vertice "<<i<<", distancia mas corta = "<<distancia[ i ]<<endl;

        }

        cout<<"\n**************Impresion de camino mas corto**************"<<endl;
        cout<<"Ingrese vertice destino: "<<endl;
        int destino;
        cin>>destino;
        print( destino );
        cout<<endl;
}

///////////////////////////////////////////////////////


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




	int inicial;//nuevo-katriel
	V=CT->getNumTriangles();//nuevo-katriel

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


	 	 cout<<":::::::::::::::::MATRIZ ADYACENCIA:::::::::::::::::::::"<<endl;

	 	 cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
	 	 ;
	 		 for (unsigned int i = 0; i < CT->getNumTriangles(); i++){
	 	 	        for (unsigned int j = 0; j < CT->getNumTriangles(); j++)
	 	 	        {
	 	 	            cout<<matrizTriangle[i][j]<<" ";

	 	 	            //17 triangulos // x aristas//nuevo-katriel
	 	 	          if(matrizTriangle[i][j]==1){
	 	 	            ady[ i+1 ].push_back( Node( j+1 , 1 ) ); //consideremos grafo dirigido
	 	 	            ady[ j+1 ].push_back( Node( i+1 , 1 ) ); //grafo no dirigido

	 	 	            }

	 	 	        }
	 	 	        cout<<endl;
	 	 	    }

	 		//imprime al final//nuevo-katriel
	 	/*	printf("Ingrese el vertice inicial: ");
	 		    scanf("%d" , &inicial );
	 		    dijkstra( inicial );*/

	 		//imprime en ejecucion
	 	cout<<"Ingrese el vertice inicial: "<<endl;
	 	cin>>inicial;
	 	 dijkstra( inicial );;


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


    init(window);

    matrizAdyacencia();

    while (!glfwWindowShouldClose(window)) {

        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
