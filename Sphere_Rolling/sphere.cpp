#include "Angel-yjc.h"
#include "iostream"
#include "fstream"

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;

//Global Variables
std::string filename; //input filename
void file_in1(void); //function to read input file
const int radius=1; //radius of sphere
mat4 accmat = Angel::identity(); //accmat is initially set to identity
bool begin = false; //controls keyboard 'b'
void addMenu(void); //menu function
void mouse(int button, int state, int x, int y); //mouse function
bool roll=false; //controls mouse right click
point3 calculate_mat(point3 a, point3 b); //function that calculates ab/|ab|
point3 cross_product(point3 y_axis, point3 mat); //cross product to determine the coordinates of rotation
point3 calculate_mat1(point3 f, point3 t); 
point3 y_axis(0.0,1.0,0.0); //y axis
int current = 0; //current point pointer
int next = 1; //next point pointer
point3 rot[3];
float distance(point3 p1, point3 p2); //returns the distance b/w point p1 and p2
bool test(); //function that tests if current position > end of line segments
point3 po[3]={ //the 3 points that determine the endpoints of the path taken by sphere
point3(-4.0,1.0,4.0),
point3(3.0,1.0,-4.0),
point3(-3.0,1.0,-3.0)
};
point3 current_point = po[current]; //current point
point3 mat[3];
point3 mat1[3];

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint program;       /* shader program object id */
GLuint sphere_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;  /* vertex buffer object id for floor */
GLuint axis_buffer;

// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 13.0;

GLfloat angle = 0.0; // rotation angle in degrees
vec4 init_eye(7.0,3.0,-10.0, 1.0); // initial viewer position
vec4 eye = init_eye;               // current viewer position

int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'

int sphereFlag = 0;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'

int sphere_NumVertices;

point3 sphere_points[400]; // positions for all vertices of sphere
color3 sphere_colors[400]; // colors for all vertices of sphere


const int floor_NumVertices = 6; //(1 face)*(2 triangles/face)*(3 vertices/triangle)
point3 floor_points[floor_NumVertices]={ // positions for all vertices of floor
point3(5.0,0.0,8.0),
point3(5.0,0.0,-4.0),
point3(-5.0,0.0,-4.0),
point3(5.0,0.0,8.0),
point3(-5.0,0.0,8.0),
point3(-5.0,0.0,-4.0)
};
color3 floor_colors[floor_NumVertices]={ // colors for all vertices
color3(0.0,1.0,0.0),
color3(0.0,1.0,0.0),
color3(0.0,1.0,0.0),
color3(0.0,1.0,0.0),
color3(0.0,1.0,0.0),
color3(0.0,1.0,0.0)
};

point3 axis_points[9]={ // positions for all vertices of x,y,z axis
point3(0, 0, 0),
point3(10, 0, 0),
point3(20, 0, 0),
point3(0, 0, 0),
point3(0, 10, 0),
point3(0, 20, 0),
point3(0, 0, 0),
point3(0, 0, 10),
point3(0, 0, 20)
};
color3 axis_colors[9]={ // colors for all vertices of x,y,z axis
color3(1.0, 0.0, 0.0),
color3(1.0, 0.0, 0.0),
color3(1.0, 0.0, 0.0),
color3(1.0, 0.0, 1.0),
color3(1.0, 0.0, 1.0),
color3(1.0, 0.0, 1.0),
color3(0.0, 0.0, 1.0),
color3(0.0, 0.0, 1.0),
color3(0.0, 0.0, 1.0)
};

//----------------------------------------------------------------------------
point3 calculate_mat(point3 f, point3 t)
{
	point3 temp;
 	float te;
	temp.x=t.x-f.x;
	temp.y=t.y-f.y;
	temp.z=t.z-f.z;
	te=sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
	temp.x=temp.x/te;
        temp.y=temp.y/te;
	temp.z=temp.z/te;
	return temp;
}
point3 calculate_mat1(point3 f, point3 t)
{       
        point3 temp1;
        
        temp1.x=t.x-f.x;
        temp1.y=t.y-f.y;
        temp1.z=t.z-f.z;
	return temp1;
}

point3 cross_product(point3 y_axis, point3 ma)
{
	point3 ret;
	ret.x = y_axis.y * ma.z - y_axis.z * ma.y;
	ret.y = y_axis.z * ma.x - y_axis.x * ma.z;
	ret.z = y_axis.x * ma.y - y_axis.y * ma.x;
	return ret;
}
int Index = 0; // YJC: This must be a global variable since quad() is called
//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{   

    for (int k=0;k<3;k++)
    {
	if(k==2)
	{
		mat[k]=calculate_mat(po[k],po[0]);
		mat1[k]=calculate_mat(po[k],po[0]);
	}
	else
	{
		mat[k]=calculate_mat(po[k],po[k+1]);
		mat1[k]=calculate_mat(po[k],po[k+1]);
	}
    }
    for (int j=0;j<3;j++)
    {
	rot[j]=cross_product(y_axis,mat[j]);
    }

    glGenBuffers(1, &sphere_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)* sphere_NumVertices + sizeof(color3)* sphere_NumVertices,
		 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)* sphere_NumVertices, sphere_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)* sphere_NumVertices, sizeof(color3)* sphere_NumVertices,
                    sphere_colors);

 // Create and initialize a vertex buffer object for floor, to be used in display()
    glGenBuffers(1, &floor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_colors),
		 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(floor_colors),
                    floor_colors);

    glGenBuffers(1, &axis_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, axis_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_points) + sizeof(axis_colors),
	         NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(axis_points), axis_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(axis_points), sizeof(axis_colors),
	                axis_colors);
 // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader42.glsl", "fshader42.glsl");
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
    glLineWidth(2.0);
}
//----------------------------------------------------------------------------
// drawObj(buffer, num_vertices):
//   draw the object that is associated with the vertex buffer object "buffer"
//   and has "num_vertices" vertices.
//
void drawObj(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 
      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}
//----------------------------------------------------------------------------
void display( void )
{
  GLuint  model_view;  // model-view matrix uniform shader variable location
  GLuint  projection;  // projection matrix uniform shader variable location

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram(program); // Use the shader program

    model_view = glGetUniformLocation(program, "model_view" );
    projection = glGetUniformLocation(program, "projection" );
    glClearColor(0.529, 0.807, 0.92, 0.0);
/*---  Set up and pass on Projection matrix to the shader ---*/
    mat4  p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

/*---  Set up and pass on Model-View matrix to the shader ---*/
    // eye is a global variable of vec4 set to init_eye and updated by keyboard()
    vec4    at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 1.0, 0.0, 0.0);

    mat4  mv = LookAt(eye, at, up);
    //me-accmat = Rotate(angle,rot[current].x,rot[current].y,rot[current].z) * accmat;
    //me-mat4 mv1=mv*Translate(current_point.x,current_point.y,current_point.z)*accmat;
    mat4 mv1=mv*Translate(current_point.x,current_point.y,current_point.z)*Rotate(angle,rot[current].x,rot[current].y,rot[current].z)*accmat;
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1); // GL_TRUE: matrix is row-major
    if (sphereFlag == 1) // Filled cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              //Wireframe cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(sphere_buffer, sphere_NumVertices);  // draw the cube

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if (floorFlag == 1) // Filled floor
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              // Wireframe floor
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(floor_buffer, floor_NumVertices); // draw the floor
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(axis_buffer, 9); // draw the axis
     

    glutSwapBuffers();
}
//---------------------------------------------------------------------------

float distance(point3 p1, point3 p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}
bool test()
	{	
	point3 from_point = po[current];
        point3 to_point = po[next];
	float d1 = distance(current_point, from_point);
	float d2 = distance(to_point, from_point);

	return d1 > d2;

}
//----------------------------------------------------------------------------
void idle (void)
{   if(begin){
    angle += 2.5;    //YJC: change this value to adjust the cube rotation speed.
    //if(angle>360)
    //{
      // angle -=360;
    //}
    float d;
    d = (angle * 2 * 3.14 * radius)/360;
    //current_point.x = current_point.x + (mat[current].x * d);
    //current_point.y = current_point.y + (mat[current].y * d);
    //current_point.z = current_point.z + (mat[current].z * d);
	current_point = po[current] + mat[current]*d;    
//current_rot = cross_product(y_axis,current_point);
    //angle += 0.006;    //YJC: change this value to adjust the cube rotation speed.

    if(test())
    {
	current=next;
	next=next+1;
	if(next==3)
	{
		next=0;
	}
	current_point=po[current];
	
	accmat = Rotate(angle,rot[current].x,rot[current].y,rot[current].z) * accmat;
	angle = 0.0;	
//current_rot=mat[current];
    }
    glutPostRedisplay();
}}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	case 'b': case'B': //start Rolling
	    begin = true;
            //glutIdleFunc(idle);
	    break;

        case 'X': eye[0] += 1.0; break;
	case 'x': eye[0] -= 1.0; break;
        case 'Y': eye[1] += 1.0; break;
	case 'y': eye[1] -= 1.0; break;
        case 'Z': eye[2] += 1.0; break;
	case 'z': eye[2] -= 1.0; break;

        case 'a': case 'A': // Toggle between animation and non-animation
	    animationFlag = 1 -  animationFlag;
            if (animationFlag == 1) glutIdleFunc(idle);
            else                    glutIdleFunc(NULL);
            break;
	   
        case 'c': case 'C': // Toggle between filled and wireframe cube
	    sphereFlag = 1 -  sphereFlag;   
            break;

        case 'f': case 'F': // Toggle between filled and wireframe floor
	    floorFlag = 1 -  floorFlag; 
            break;

	case ' ':  // reset to initial viewer/eye position
	    eye = init_eye;
	    break;
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}
//----------------------------------------------------------------------------

void file_in1(void)
{       
 	int count=0,points,col;
	float temp[3];
	std::ifstream input_file;
	input_file.open("/Users/suchethas/sphere.128");
	input_file >> col;
	//sphereData = new point3[col * 3];
	//sphere_color = new point3[col * 3];
	for (int i = 0; i < col; i++) {
		input_file >> points;
		for (int j = 0; j < points; j++) {
			for (int k = 0; k < 3; k++) {
				input_file >> temp[k];

			}
			point3 tmp( temp[0], temp[1], temp[2]);
			sphere_points[count++] = tmp;
			//std::cout << sphere_points[count - 1] << std::endl;
		}
	}
	for (int i = 0; i < col * 3; i++) 
	{
		sphere_colors[i] = color3(1.0, 0.84, 0.0);
	}
	sphere_NumVertices = col * 3;
}
//----------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && begin) {
		roll=!roll;
	}
	if (roll) {
		// Stop rolling
		glutIdleFunc(idle);
	}
	else {
		glutIdleFunc(NULL);
	}
}
void menu(int index)
{
	switch (index)
	{
	case (1) :
	{
		eye = init_eye;
		glutIdleFunc(idle);
		break;
	}
	case (2) :
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
	display();
}

void addMenu() 
{
	glutCreateMenu(menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddMenuEntry("Quit", 2);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}
//----------------------------------------------------------------------------
int main( int argc, char **argv )
{   
    
    //std::cout << "Enter the filename: sphere.128 / sphere.8"; sin >> filename;
    file_in1();
	
    glutInit(&argc, argv);
#ifdef __APPLE__ // Enable core profile of OpenGL 3.2 on macOS.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
    glutInitWindowSize(512, 512);
    glutCreateWindow("Sphere Rolling");
    addMenu();
#ifdef __APPLE__ // on macOS
    // Core profile requires to create a Vertex Array Object (VAO).
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
#else           // on Linux or Windows, we still need glew
    /* Call glewInit() and error checking */
    int err = glewInit();
    if (GLEW_OK != err)
    { 
        printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(err)); 
        exit(1);
    }
#endif

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);    
    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}
