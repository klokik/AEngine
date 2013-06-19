#version 120

uniform vec4 u_color;

uniform sampler2D u_texture0;
uniform int u_istextured0;
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

varying vec3 f_normal;
varying vec2 f_texcoord0;
varying vec3 f_pos;
varying vec3 f_eyevec;

vec3 N;

void setTexture(void);
void setLighting(void);

vec3 process_point_spot(int id);

void main(void)
{
	//setTexture();
	gl_FragColor=vec4(1,1,1,1);
	if(u_shaded!=0)
		setLighting();
}

void setTexture(void)
{
	gl_FragColor=vec4(u_color.rgb,1.0);
	if(u_istextured0!=0)
	{
		gl_FragColor*=texture2D(u_texture0,f_texcoord0.st);              
	}
}

void setLighting(void)
{
	vec3 r_color=vec3(0,0,0);

	N=normalize(f_normal);

	for(int q=0;q<u_light_num;q++)
	{
		if(u_lights.color[q].a==0) continue;

		// if(u_lights.type[q]==0x01)//Ambient light
		// {
		// 	r_color+=u_lights.color[q].rgb*u_lights.color[q].a;
		// }
		// if(u_lights.type[q]==0x02)//directional light
		// {
		// 	vec3 L=u_lights.rotation[q];
		// 	vec3 Lr=max(dot(L,N),0.0)*u_lights.color[q].rgb*u_lights.color[q].a;
		// 	r_color+=Lr;
		// }
		if(u_lights.type[q]==0x03||u_lights.type[q]==0x04)//point or spot light
			r_color.rgb+=process_point_spot(q);
	}

	gl_FragColor.rgb*=r_color;
}

vec3 process_point_spot(int id)
{
	float dist=distance(u_lights.position[id],f_pos);
	float atten=1/(u_lights.attenuation[id].x+u_lights.attenuation[id].y*dist);//+u_lights.attenuation[id].z*pow(dist,2));

	vec3 L=normalize(f_pos.xyz-u_lights.position[id]);
	vec3 Lr=max(dot(-L,N),0.0)*u_lights.color[id].rgb*u_lights.color[id].a;


	//FIXIT it's slow
	if(u_lights.type[id]==0x04)
	{
		float cur_angle=dot(normalize(u_lights.rotation[id]),L);
		float cut_angle=cos(radians(u_lights.spot[id].x/2));

		// FIXIT cut_off light exponents seems not working
		if(cur_angle>cut_angle)
			Lr*=1;//pow(cur_angle,u_lights.spot[id].y);
		else
			return vec3(0,0,0);
	}
	//---------------

	return max(Lr*atten,vec3(0,0,0));
}