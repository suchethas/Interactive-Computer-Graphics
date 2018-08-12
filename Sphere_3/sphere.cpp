#include "Angel-yjc.h"
#include "iostream"
#include "fstream"

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
//Global Variables
#define PI 3.14159265358979323846
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
mat4 trans_p_to_q(12,0,0,0,14,0,3,-1,0,0,12,0,0,0,0,12);
point3 current_point = po[current]; //current point
point3 mat[3];
point3 mat1[3];
point4 floor_light_points[6];
//vec3   floor_normals[6];
GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);
void set_point_light_floor(mat4 mv);
void set_spot_light_floor(mat4 mv);
void set_directional_light_floor(mat4 mv);
GLuint program;       /* shader program object id */
GLuint program_2;
GLuint sphere_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;  /* vertex buffer object id for floor */
GLuint axis_buffer;
GLuint shadow_buffer;
GLuint s_flat_buffer;
GLuint s_smooth_buffer;
GLuint floor_light_buffer;
// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 13.0;

GLfloat angle = 0.0; // rotation angle in degrees
vec4 init_eye(7.0,3.0,-10.0, 1.0); // initial viewer position
vec4 eye = init_eye;               // current viewer position
void lighting_menu(int index);
void shadow_menu(int index);
int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'
//int shadowFlag = 1; //1: solid shadow; 0: wireframe shadow.  Toggled by key 
int sphereFlag = 0;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'
void ini_set_menu_flag();
int sphere_NumVertices;
void compute_smooth_normal();
point3 sphere_points[3100]; // positions for all vertices of sphere
color3 sphere_colors[3100]; // colors for all vertices of sphere
point3 shadow_points[3100];
point4 shadow_points_4[3100];
point3 shadow_colors[3100];
point3 sphere_normal_flat[3100];
point3 sphere_normal_smooth[3100];
point3 floor_normals[6];
void compute_flat_normal();
int col;
void drawObj2(GLuint buffer, int num_vertices);
void my_quad( int a, int b, int c, int d );
void floor_light();
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
point4 f_l_vertices[4] = {
	point4( 5, 0,  8, 1.0),
	point4( 5, 0,  -4, 1.0),
	point4(  -5,  0,  -4, 1.0),
	point4(  -5,  0,  8, 1.0),
};
void set_spot_light_floor(mat4 mv);
int Index = 0;
int shadow =0;
int lighting = 0;
int flatshade=0;
int smoothshade=0;
int spotlight=0;
int pointsource=1;
void my_quad( int a, int b, int c, int d )
{
	// Initialize temporary vectors along the quad's edges to
	//   compute its face normal
	vec4 u = f_l_vertices[b] - f_l_vertices[a];
	vec4 v = f_l_vertices[d] - f_l_vertices[a];

	vec3 tmp_normal = normalize( cross(v, u) );
	floor_normals[Index] = tmp_normal; Index++;
	floor_normals[Index] = tmp_normal; Index++;
	floor_normals[Index] = tmp_normal; Index++;
	floor_normals[Index] = tmp_normal; Index++;
	floor_normals[Index] = tmp_normal; Index++;
	floor_normals[Index] = tmp_normal; Index++;
}

void floor_light()
{
	Index = 0;
	my_quad(1, 0, 3, 2);

}
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
void set_directional_light(mat4 mv);
void set_point_light(mat4 mv);
void set_spot_light(mat4 mv);
//----------------------------------------------------------------------------
void set_directional_light(mat4 mv)
{
	color4 global_a(1,1,1,1);
	color4 light_ambient( 0, 0, 0, 1.0 );
	color4 light_diffuse( 0.8, 0.8, 0.8, 1.0 );
	color4 light_specular( 0.2, 0.2, 0.2, 1.0 );
	color4 material_ambient( 0.2, 0.2, 0.2, 1.0 );
	color4 material_diffuse( 1.0, 0.84, 0, 1 );
	color4 material_specular( 1.0, 0.84, 0, 1 );
	float  material_shininess = 125;
	color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
	glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
		  1, ambient_product );
    	glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
		  1, diffuse_product );
    	glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
		  1, specular_product );
	glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
	glUniform1f(glGetUniformLocation(program, "point_flag"), 0.0);
	glUniform1f(glGetUniformLocation(program, "spot_flag"), 0.0);
	glUniform1f(glGetUniformLocation(program, "directional_flag"), 1.0);
}
void set_directional_light_floor(mat4 mv)
{       
        color4 global_a(1,1,1,1);
        color4 light_ambient( 0, 0, 0, 1.0 );
        color4 light_diffuse( 0.8, 0.8, 0.8, 1.0 );
        color4 light_specular( 0.2, 0.2, 0.2, 1.0 );
        color4 material_ambient( 0.2,0.2, 0.2, 1.0 );
        color4 material_diffuse( 0, 1.0, 0, 1 );
        color4 material_specular( 0, 0, 0, 1 );        
        float  material_shininess = 125;
        color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
        glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
                  1, ambient_product );
        glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
                  1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
                  1, specular_product );
        glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
	glUniform1f(glGetUniformLocation(program, "point_flag"), 0.0);
	glUniform1f(glGetUniformLocation(program, "spot_flag"), 0.0);
	glUniform1f(glGetUniformLocation(program, "directional_flag"), 1.0);
}
void set_point_light(mat4 mv)
{
        color4 global_a(1,1,1,1);
        color4 light_ambient( 0, 0, 0, 1.0 );
        color4 light_diffuse( 1,1,1,1 );
        color4 light_specular( 1,1,1,1 );
        float const_att = 2.0;
        float linear_att = 0.01;
        float quad_att = 0.001;
        point4 light_position(-14, 12.0, -3, 1.0 );
        vec4 light_position_eyeFrame = mv * light_position;
        color4 material_ambient( 0.2, 0.2, 0.2, 1.0 );
        color4 material_diffuse( 1.0, 0.84, 0, 1 );
        color4 material_specular( 1.0, 0.84, 0, 1 );
        float  material_shininess = 125;
	color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
        glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
                  1, ambient_product );
        glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
                  1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
                  1, specular_product );
        glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
        glUniform4fv( glGetUniformLocation(program_2, "LightPosition"),
                  1, light_position_eyeFrame);
        glUniform1f(glGetUniformLocation(program_2, "ConstAtt"),
                        const_att);
        glUniform1f(glGetUniformLocation(program_2, "LinearAtt"),
                        linear_att);
        glUniform1f(glGetUniformLocation(program_2, "QuadAtt"),
                        quad_att);
        glUniform1f(glGetUniformLocation(program_2, "Shininess"),
                        material_shininess );
	glUniform1f(glGetUniformLocation(program, "directional_flag"), 0.0);
	glUniform1f(glGetUniformLocation(program, "point_flag"),1.0);
	glUniform1f(glGetUniformLocation(program, "spot_flag"), 0.0);
}
void set_point_light_floor(mat4 mv)
{
        color4 global_a(1,1,1,1);
        color4 light_ambient( 0, 0, 0, 1.0 );
        color4 light_diffuse( 1,1,1,1 );
        color4 light_specular( 1,1,1,1 );
        float const_att = 2.0;
        float linear_att = 0.01;
        float quad_att = 0.001;
        point4 light_position(-14, 12.0, -3, 1.0 );
        vec4 light_position_eyeFrame = mv * light_position;
        color4 material_ambient( 0.2,0.2, 0.2, 1.0 );
    	color4 material_diffuse( 0, 1.0, 0, 1 );
    	color4 material_specular( 0, 0, 0, 1 );
	float  material_shininess = 125;
        color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
        glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
                  1, ambient_product );
        glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
                  1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
                  1, specular_product );
        glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
        glUniform4fv( glGetUniformLocation(program_2, "LightPosition"),
                  1, light_position_eyeFrame);
        glUniform1f(glGetUniformLocation(program_2, "ConstAtt"),
                        const_att);
        glUniform1f(glGetUniformLocation(program_2, "LinearAtt"),
                        linear_att);
        glUniform1f(glGetUniformLocation(program_2, "QuadAtt"),
                        quad_att);
        glUniform1f(glGetUniformLocation(program_2, "Shininess"),
                        material_shininess );
	glUniform1f(glGetUniformLocation(program, "directional_flag"), 0.0);
        glUniform1f(glGetUniformLocation(program, "point_flag"), 1.0);
        glUniform1f(glGetUniformLocation(program, "spot_flag"), 0.0);
}

void set_spot_light(mat4 mv)
{   color4 global_a(1,1,1,1); 
    color4 light_ambient( 0, 0, 0, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
    float const_att = 2.0;
    float linear_att = 0.01;
    float quad_att = 0.001;
    point4 light_position(-14, 12.0, -3, 1.0 ); 
	
	float spot_exp=15.0;
	float spot_ang=cos(20.0*PI/180);
          // In World frame.
          // Needs to transform it to Eye Frame
          // before sending it to the shader(s).
	vec4 light_position_eyeFrame = mv * light_position;
	color4 material_ambient( 0.2, 0.2, 0.2, 1.0 );
        color4 material_diffuse( 1.0, 0.84, 0, 1 );
        color4 material_specular( 1.0, 0.84, 0, 1 );
        float  material_shininess = 125;

	glUniform4fv( glGetUniformLocation(program, "LightPosition"),
   		  1, light_position_eyeFrame);

    	glUniform1f(glGetUniformLocation(program, "ConstAtt"),
		        const_att);
    	glUniform1f(glGetUniformLocation(program, "LinearAtt"),
		        linear_att);
    	glUniform1f(glGetUniformLocation(program, "QuadAtt"),
		        quad_att);
        color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
	glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
                  1, ambient_product );
        glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
                  1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
                  1, specular_product );
        glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
	point4 spot_direction=mv*vec4(-6.0,0.0,-4.5,1.0);
	glUniform4fv(glGetUniformLocation(program, "spot_direction"),1,
		        spot_direction);
	glUniform1f(glGetUniformLocation(program, "directional_flag"), 0.0);
        glUniform1f(glGetUniformLocation(program, "point_flag"), 0.0);
        glUniform1f(glGetUniformLocation(program, "spot_flag"), 1.0);
	glUniform1f(glGetUniformLocation(program, "spot_exp"), spot_exp);
	glUniform1f(glGetUniformLocation(program, "spot_ang"), spot_ang);

}
void set_spot_light_floor(mat4 mv)
{   color4 global_a(1,1,1,1);
    color4 light_ambient( 0, 0, 0, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
    float const_att = 2.0;
    float linear_att = 0.01;
    float quad_att = 0.001;
    point4 light_position(-14, 12.0, -3, 1.0 );

        float spot_exp=15.0;
        float spot_ang=cos(20.0*PI/180);
          // In World frame.
          // Needs to transform it to Eye Frame
          // before sending it to the shader(s).
        vec4 light_position_eyeFrame = mv * light_position;
        float  material_shininess = 125;
	color4 material_ambient( 0.2,0.2, 0.2, 1.0 );
        color4 material_diffuse( 0, 1.0, 0, 1 );
        color4 material_specular( 0, 0, 0, 1 );
        glUniform4fv( glGetUniformLocation(program, "LightPosition"),
                  1, light_position_eyeFrame);

        glUniform1f(glGetUniformLocation(program, "ConstAtt"),
                        const_att);
        glUniform1f(glGetUniformLocation(program, "LinearAtt"),
                        linear_att);
        glUniform1f(glGetUniformLocation(program, "QuadAtt"),
                        quad_att);
        color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;
	glUniform4fv( glGetUniformLocation(program_2, "AmbientProduct"),
                  1, ambient_product );
        glUniform4fv( glGetUniformLocation(program_2, "DiffuseProduct"),
                  1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program_2, "SpecularProduct"),
                  1, specular_product );
        glUniform4fv( glGetUniformLocation(program_2, "global_ambient"),
                  1, global_a*material_ambient );
        point4 spot_direction=mv*vec4(-6.0,0.0,-4.5,1.0);
        glUniform4fv(glGetUniformLocation(program, "spot_direction"),1,
                        spot_direction);
        glUniform1f(glGetUniformLocation(program, "directional_flag"), 0.0);
        glUniform1f(glGetUniformLocation(program, "point_flag"), 0.0);
        glUniform1f(glGetUniformLocation(program, "spot_flag"), 1.0);
        glUniform1f(glGetUniformLocation(program, "spot_exp"), spot_exp);
        glUniform1f(glGetUniformLocation(program, "spot_ang"), spot_ang);

}

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

    glGenBuffers(1, &shadow_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, shadow_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*sphere_NumVertices + sizeof(color3)*sphere_NumVertices,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*sphere_NumVertices, shadow_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*sphere_NumVertices, sizeof(color3)*sphere_NumVertices,
                        shadow_colors);
    
    glGenBuffers( 1, &s_flat_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, s_flat_buffer );
    glBufferData( GL_ARRAY_BUFFER, 3 * col * sizeof(point3) + 3 * col * sizeof(color3),
	         NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, 3 * col * sizeof(point3), sphere_points );
    glBufferSubData( GL_ARRAY_BUFFER, 3 * col * sizeof(point3),
	         3 * col * sizeof(color3), sphere_normal_flat );

    glGenBuffers( 1, &s_smooth_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, s_smooth_buffer );
    glBufferData( GL_ARRAY_BUFFER, 3 * col * sizeof(point3) + 3 * col * sizeof(color3),
                 NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, 3 * col * sizeof(point3), sphere_points );
    glBufferSubData( GL_ARRAY_BUFFER, 3 * col * sizeof(point3),
                 3 * col * sizeof(color3), sphere_normal_smooth );

    floor_light();
    glGenBuffers( 1, &floor_light_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, floor_light_buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(floor_points) + sizeof(floor_normals),
	              NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(floor_points),
	                 sizeof(floor_normals), floor_normals );

 // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader42.glsl", "fshader42.glsl");
    program_2 = InitShader("vshader53.glsl", "fshader53.glsl");
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
    glLineWidth(2.0);
}
//----------------------------------------------------------------------------
void drawObj(GLuint buffer, int num_vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GLuint vPosition = glGetAttribLocation(program_2, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
			  BUFFER_OFFSET(0) );
   GLuint vNormal = glGetAttribLocation( program_2, "vNormal" );
	glEnableVertexAttribArray( vNormal );
	glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
	                       BUFFER_OFFSET( num_vertices * sizeof(point3)) );
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormal);
}
//----------------------------------------------------------------------------
void drawObj2(GLuint buffer, int num_vertices)
{   
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    GLuint vPosition = glGetAttribLocation(program_2, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation(program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}
void ini_set_menu_flag()
{
	if (sphereFlag==0 || lighting==0)
	{
		flatshade=0;
		smoothshade=0;
	}
}
void display( void )
{   
    ini_set_menu_flag();
    GLuint model_view;
    GLuint projection;
    GLuint  ModelView;  // model-view matrix uniform shader variable location
    GLuint  Projection;  // projection matrix uniform shader variable location
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0.529, 0.807, 0.92, 0.0);
    mat4  p = Perspective(fovy, aspect, zNear, zFar);
    vec4    at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 1.0, 0.0, 0.0);
    mat4  mv = LookAt(eye, at, up);
    glUseProgram(program); // Use the shader program
    model_view = glGetUniformLocation(program, "model_view" );
    projection = glGetUniformLocation(program, "projection" );
    glUniformMatrix4fv(projection, 1, GL_TRUE, p);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj2(axis_buffer, 9); // draw the axis
    mat4 mv1=mv*Translate(current_point.x,current_point.y,current_point.z)*Rotate(angle,rot[current].x,rot[current].y,rot[current].z)*accmat;
        if (flatshade==1)
        {
            glUseProgram(program_2); // Use the shader program
            ModelView = glGetUniformLocation(program_2, "ModelView" );
            Projection = glGetUniformLocation(program_2, "Projection" );
            mat4  p = Perspective(fovy, aspect, zNear, zFar);
            glUniformMatrix4fv(Projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major
            if(pointsource)
            {
                set_point_light(mv);
            }
            else
            {
                set_spot_light(mv);
            }
	    //else
	//{
	 //set_directional_light(mv);
	//}
            glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv1); // GL_TRUE: matrix is row-major
            mat3 normal_matrix = NormalMatrix(mv1, 1);
            glUniformMatrix3fv(glGetUniformLocation(program_2, "Normal_Matrix"),
                               1, GL_TRUE, normal_matrix );
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            drawObj(s_flat_buffer, col * 3);
        }
        else if (smoothshade==1)
        {
            glUseProgram(program_2); // Use the shader program
            ModelView = glGetUniformLocation(program_2, "ModelView" );
            Projection = glGetUniformLocation(program_2, "Projection" );
            mat4  p = Perspective(fovy, aspect, zNear, zFar);
            glUniformMatrix4fv(Projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major
            if(pointsource)
            {
                set_point_light(mv);
            }
            else
            {
                set_spot_light(mv);
            }
            //else
        //{
         //set_directional_light(mv);
        //}
            glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv1); // GL_TRUE: matrix is row-major
            mat3 normal_matrix = NormalMatrix(mv1, 1);
            glUniformMatrix3fv(glGetUniformLocation(program_2, "Normal_Matrix"),
                               1, GL_TRUE, normal_matrix );
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            drawObj(s_smooth_buffer, col * 3);
        }
	else
        {
            glUseProgram(program);
            model_view = glGetUniformLocation(program, "model_view");
            projection = glGetUniformLocation(program, "projection");
            glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major
            glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1); // GL_TRUE: matrix is row-major
            if (sphereFlag==0)
            {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
            else
            {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
            drawObj2(sphere_buffer, sphere_NumVertices);
        }
	if (lighting ==0)
{   // Draw floor as per assignment2
    if(shadow==1)
    {
        //Disable writing to z-buffer and draw floor
        glDepthMask(GL_FALSE);
        glUseProgram(program);
        model_view = glGetUniformLocation(program, "model_view");
        projection = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projection, 1, GL_TRUE, p);
        glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawObj2(floor_buffer, floor_NumVertices); // draw the floor
        
        //Enable writing to z-buffer and draw shadow
        glDepthMask(GL_TRUE);
        glUseProgram(program);
        model_view = glGetUniformLocation(program, "model_view" );
        projection = glGetUniformLocation(program, "projection" );
        mat4 mv2 = mv*trans_p_to_q*Translate(current_point.x,current_point.y,current_point.z)*Rotate(angle,rot[current].x,rot[current].y,rot[current].z)*accmat;
        glUniformMatrix4fv(model_view,1,GL_TRUE,mv2);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (sphereFlag==0)
	{glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
	else
	{glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
        drawObj2(shadow_buffer, sphere_NumVertices);
        
        //To restore ground into z-buffer
        //Disable writing to frame buffer and draw ground
        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
        glUseProgram(program);
        model_view = glGetUniformLocation(program, "model_view");
        projection = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projection, 1, GL_TRUE, p);
        glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawObj2(floor_buffer, floor_NumVertices); // draw the floor
        
        //Enable writing to frame buffer
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    }
    else
    {
    glUseProgram(program);
    model_view = glGetUniformLocation(program, "model_view");
    projection = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projection, 1, GL_TRUE, p);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj2(floor_buffer, floor_NumVertices); // draw the floor
    }
}
else
{   //draw the floor as per assignment3
    if (shadow==1)
    {
        //Disable writing to z-buffer and draw floor
        glDepthMask(GL_FALSE);
        glUseProgram(program_2 );
        ModelView = glGetUniformLocation( program_2, "ModelView" );
        Projection = glGetUniformLocation( program_2, "Projection" );
        glUniformMatrix4fv(Projection, 1, GL_TRUE, p);
        if(pointsource)
        {
            set_point_light_floor(mv);
        }
else
            {
                set_spot_light_floor(mv);
            }
            //else
       // {
         //set_directional_light_floor(mv);
        //}        
glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv );
        mat3 normal_matrix = NormalMatrix(mv, 1);
        glUniformMatrix3fv(glGetUniformLocation(program_2, "Normal_Matrix"),
                           1, GL_TRUE, normal_matrix );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawObj(floor_light_buffer,floor_NumVertices);
        
        //Enable writing to z-buffer and draw shadow
        glDepthMask(GL_TRUE);
        glUseProgram(program);
        model_view = glGetUniformLocation(program, "model_view" );
        projection = glGetUniformLocation(program, "projection" );
        mat4 mv2 = mv*trans_p_to_q*Translate(current_point.x,current_point.y,current_point.z)*Rotate(angle,rot[current].x,rot[current].y,rot[current].z)*accmat;
        glUniformMatrix4fv(model_view,1,GL_TRUE,mv2);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (sphereFlag==0)
        {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
        else
        {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
        drawObj2(shadow_buffer, sphere_NumVertices);
        
        //To restore ground into z-buffer
        //Disable writing to frame buffer and draw ground
        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
        glUseProgram(program_2 );
        ModelView = glGetUniformLocation( program_2, "ModelView" );
        Projection = glGetUniformLocation( program_2, "Projection" );
        glUniformMatrix4fv(Projection, 1, GL_TRUE, p);
        if(pointsource)
        {
            set_point_light_floor(mv);
        }
else
            {
                set_spot_light_floor(mv);
            }
            //else
        //{
         //set_directional_light_floor(mv);
        //}        
glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv );
         normal_matrix = NormalMatrix(mv, 1);
        glUniformMatrix3fv(glGetUniformLocation(program_2, "Normal_Matrix"),
                           1, GL_TRUE, normal_matrix );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawObj(floor_light_buffer,floor_NumVertices);
        
        //Enable writing to frame buffer
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
        
    }
    else
    {
    glUseProgram(program_2 );
    ModelView = glGetUniformLocation( program_2, "ModelView" );
    Projection = glGetUniformLocation( program_2, "Projection" );
    glUniformMatrix4fv(Projection, 1, GL_TRUE, p);
    if(pointsource)
    {
        set_point_light_floor(mv);
    }
else
            {
                set_spot_light_floor(mv);
            }
    //        else
      //  {
        // set_directional_light_floor(mv);
        //}    
glUniformMatrix4fv(ModelView, 1, GL_TRUE, mv );
    mat3 normal_matrix = NormalMatrix(mv, 1);
    glUniformMatrix3fv(glGetUniformLocation(program_2, "Normal_Matrix"),
                       1, GL_TRUE, normal_matrix );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj(floor_light_buffer,floor_NumVertices);
    }
}

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
    float d;
    d = (angle * 2 * 3.14 * radius)/360;
	current_point = po[current] + mat[current]*d;    
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
void compute_flat_normal()
{
	int flat_index = 0,c=0;
	for (int j = 0; j < col; j++)
	{

		vec4 curr_p1 = shadow_points_4[3 * j];
		vec4 curr_p2 = shadow_points_4[3 * j + 1];
		vec4 curr_p3 = shadow_points_4[3 * j + 2];


		vec4 u = curr_p2 - curr_p1;
		vec4 v = curr_p3 - curr_p1;
                //std::cout<<normalize(cross(u,v))<<std::endl;
		point3 tmp_normal = normalize( cross(u, v) );
		std::cout << tmp_normal<<std::endl;
		sphere_normal_flat[flat_index] = tmp_normal;  flat_index++;
		sphere_normal_flat[flat_index] = tmp_normal;  flat_index++;
		sphere_normal_flat[flat_index] = tmp_normal;  flat_index++;
		
		//std::cout << sphere_normal[c] <<std::endl;
		//c++;
	}
        std::cout<<c;
}

void compute_smooth_normal()
{
	for(int i=0;i<col*3;i++)
	{
		vec4 curr_p=shadow_points_4[i];
		vec3 tmp=vec3(curr_p[0],curr_p[1],curr_p[2]);
		tmp=normalize(tmp);
		sphere_normal_smooth[i]=tmp;
	}
};
void file_in1(void)
{       
 	int count=0,points;
	float temp[3];
	std::ifstream input_file;
	input_file.open("/Users/suchethas/sphere.1024");
	input_file >> col;
	for (int i = 0; i < col; i++) {
		input_file >> points;
		for (int j = 0; j < points; j++) {
			for (int k = 0; k < 3; k++) {
				input_file >> temp[k];

			}
			point3 tmp( temp[0], temp[1], temp[2]);
			point4 tmp1(temp[0], temp[1], temp[2], 1);
			sphere_points[count] = tmp;
			shadow_points[count] = tmp;
			shadow_points_4[count] = tmp1;
			count++;
		}
	}
	for (int i = 0; i < col * 3; i++) 
	{
		sphere_colors[i] = color3(1.0, 0.84, 0.0);
		shadow_colors[i] = color3(0.25,0.25,0.25);
		
	}
	sphere_NumVertices = col * 3;
	compute_flat_normal();
	compute_smooth_normal();
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
	case(3) :
	{
		sphereFlag=!sphereFlag;
		break;
	}
	}
	display();
}
/* shadow menu handler
1) Turn on shadow
2) Turn off shadow
*/
void shadow_menu(int index){
	shadow = (index == 1)?false:true;
	display();
};
void lighting_menu(int index){
	lighting = (index == 1)?false:true;
	display();
};
/* shade menu handler
1) Flat shade
2) Smooth shade
*/
void shade_menu(int index){
	flatshade = (index == 1)?true:false;
	if(flatshade){
		flatshade=flatshade;
	}else {
		smoothshade=!smoothshade;
		if (smoothshade==1) {flatshade=0;}
	}
	display();
};
/* spotlight menu handler
1) Spotlight
2) Point light
*/
void spotlight_menu(int index){
	spotlight = (index == 1)?true:false;
	if(spotlight){
		spotlight=1;
		pointsource=0;
	}
	else
	{
		spotlight=0;
		pointsource=1;
	}
	display();
};
void addMenu() 
{
	int shadow = glutCreateMenu(shadow_menu);
	glutAddMenuEntry("No", 1);
	glutAddMenuEntry("Yes", 2);
	
	int lighting = glutCreateMenu(lighting_menu);
	glutAddMenuEntry("No", 1);
	glutAddMenuEntry("Yes", 2);
	
	int shade = glutCreateMenu(shade_menu);
	glutAddMenuEntry("flat shading", 1);
	glutAddMenuEntry("smooth shading", 2);

	int spotlight = glutCreateMenu(spotlight_menu);
	glutAddMenuEntry("Spot light", 1);
	glutAddMenuEntry("Point light", 2);

	glutCreateMenu(menu);
	glutAddMenuEntry("Default View Point", 1);
	glutAddMenuEntry("Wire Frame Sphere", 3);
	glutAddSubMenu("Shadow",shadow);
	glutAddSubMenu("Enable Lighting", lighting);
	glutAddSubMenu("Shading", shade);
	glutAddSubMenu("Lighting", spotlight);
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
