/* 
File Name: "vshader53.glsl":
Vertex shader:
  - Per vertex shading for a single point light source;
    distance attenuation is Yet To Be Completed.
  - Entire shading computation is done in the Eye Frame.
*/

#version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat3 Normal_Matrix;
uniform vec4 LightPosition;   // Must be in Eye Frame
uniform float Shininess;
uniform vec4 global_ambient;
uniform float ConstAtt;  // Constant Attenuation
uniform float LinearAtt; // Linear Attenuation
uniform float QuadAtt;   // Quadratic Attenuation
uniform float spot_exp;
uniform float spot_flag;
uniform float point_flag;
uniform float directional_flag;
uniform float spot_ang;
uniform vec4 spot_direction;
void main()
{
    // Transform vertex  position into eye coordinates
    vec3 pos = (ModelView * vPosition).xyz;
    //vec4 global_ambient = vec4 (1,1,1,1);
    vec3 light_direction = vec3 (0.1,0,-1);
    vec3 init_L = normalize( -light_direction );
    vec3 init_E = normalize( -pos );
    vec3 init_H = normalize( init_L + init_E );
    
    vec3 N = normalize(Normal_Matrix * vNormal);
    
    float attenuation = 1.0;
    
    // Compute terms in the illumination equation
    vec4 init_AmbientProduct = AmbientProduct;
    vec4 init_DiffuseProduct = DiffuseProduct;
    vec4 init_SpecularProduct = SpecularProduct;
    
    vec4 init_ambient = init_AmbientProduct;
    
    float init_d = max( dot(init_L, N), 0.0 );
    vec4  init_diffuse = init_d *  init_DiffuseProduct;
    
    float init_s = pow( max(dot(N, init_H), 0.0), Shininess );
    vec4  init_specular = init_s * init_SpecularProduct;
    
    if( dot(init_L, N) < 0.0 ) {
        init_specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
    
    vec4 init_color =1.0 * (init_ambient + init_diffuse + init_specular);
    vec3 L = normalize( LightPosition.xyz - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );
    
    vec3 tmp_cal_d = LightPosition.xyz - pos;
    float cur_distance=length(tmp_cal_d);
    attenuation=1/(ConstAtt + LinearAtt*cur_distance + QuadAtt*cur_distance*cur_distance);
    
    vec4 ambient = AmbientProduct;
    float d = max( dot(L, N), 0.0 );
    vec4  diffuse = d * DiffuseProduct;
    float s = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = s * SpecularProduct;
    if( dot(L, N) < 0.0 ) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
    gl_Position = Projection * ModelView * vPosition;

    vec4 color_add_point = attenuation * (ambient + diffuse + specular);
      
	vec3 spot_l = -L;
   vec3 spot_l_f= normalize( spot_direction.xyz-LightPosition.xyz );
  
   float spot_att=pow(dot(spot_l,spot_l_f),spot_exp);
   
   vec4 color_add_spot=spot_att*color_add_point;
   
   if ( dot(spot_l,spot_l_f) < spot_ang)
   {color_add_spot=vec4(0,0,0,1);}
	

	//if (directional_flag==1.0)
	//{color = init_color+global_ambient;}
	if (spot_flag == 1.0 )
	{color = init_color+global_ambient+color_add_spot;}
	else    
	{color=init_color+global_ambient+color_add_point;}
}
