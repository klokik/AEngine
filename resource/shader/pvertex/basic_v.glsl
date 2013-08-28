#version 100

uniform mat4 u_object_matrix;
uniform mat4 u_camera_matrix;
uniform mat4 u_projection_matrix;
uniform int u_shaded;

const int max_light=8;
struct LightSource
{
	int type	[max_light];
	vec3 position	[max_light];
	vec3 rotation	[max_light];
	vec4 color	[max_light];
	vec3 attenuation [max_light];
	vec2 spot	[max_light];
};
uniform LightSource u_lights;
uniform int u_light_num;

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;

// varying vec3 f_normal;
varying vec2 f_texcoord0;
//varying vec3 f_pos;
//varying vec3 f_eyevec;
varying vec3 f_light;

//forward declaration
void passData(void);
void setLighting(void);

vec4 pos;

void main(void)
{
	pos=u_object_matrix*vec4(a_vertex,1.0);
	gl_Position=u_projection_matrix*u_camera_matrix*pos;

	setLighting();

	passData();
}

void passData(void)
{
	f_texcoord0=a_texcoord0;
	// f_normal=mat3x3(u_camera_matrix*u_object_matrix)*a_normal;
}

void setLighting(void)
{
	vec3 r_color=vec3(0,0,0);

	// vec3 N=normalize(mat3x3(u_camera_matrix*u_object_matrix)*a_normal);
	f_light=vec3(0.5);//normalize(N)/2+0.5;
	return;

	vec3 N=normalize((u_camera_matrix*u_object_matrix*vec4(a_normal,1.0)).xyz);

	for(int q=0;q<u_light_num;q++)
	{
		if(u_lights.color[q].a==0.0) continue;

		if(u_lights.type[q]==0x01)//Ambient light
		{
			r_color+=u_lights.color[q].rgb*u_lights.color[q].a;
		}
		if(u_lights.type[q]==0x02)//directional light
		{
			vec3 L=u_lights.rotation[q];
			vec3 Lr=max(dot(L,N),0.0)*u_lights.color[q].rgb*u_lights.color[q].a;
			r_color+=Lr;
		}
		if(u_lights.type[q]==0x03||u_lights.type[q]==0x04)//point or spot light
		{
			float dist=distance(u_lights.position[q],pos.xyz);
			float atten=1.0/(u_lights.attenuation[q].x+u_lights.attenuation[q].y*dist+u_lights.attenuation[q].z*pow(dist,2.0));

			vec3 L=normalize(pos.xyz-u_lights.position[q]);
			vec3 Lr=max(dot(-L,N),0.0)*u_lights.color[q].rgb*u_lights.color[q].a;


			// Spot light
			if(u_lights.type[q]==0x04)
			{
				float cur_angle=dot(normalize(u_lights.rotation[q]),L);
				float cut_angle=cos(radians(u_lights.spot[q].x*0.5));

				if(cur_angle>cut_angle)
					Lr*=pow(cur_angle,u_lights.spot[q].y);
				else
					continue;
			}

			Lr=max(Lr*atten,vec3(0,0,0));
			r_color.rgb+=Lr;
		}
	}

	f_light=r_color;
}

