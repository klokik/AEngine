#version 120


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

uniform sampler2D u_source0;
uniform sampler2D u_source1;
uniform sampler2D u_source2;
uniform sampler2D u_source3;

uniform vec4 u_data0;

varying vec2 f_pos;
varying vec2 f_offset;


vec4 pos;

vec3 SetLighting(void)
{
	vec3 result = vec3(0.0);

	vec3 N = texture2D(u_source1,f_pos).rgb;

	for(int q=0;q<u_light_num;q++)
	{
		if(u_lights.color[q].a==0) continue;

		if(u_lights.type[q]==0x01)//Ambient light
		{
			result+=u_lights.color[q].rgb*u_lights.color[q].a;
			continue;
		}

		if(u_lights.type[q]==0x02)//directional light
		{
			vec3 L=u_lights.rotation[q];
			vec3 Lr=max(dot(L,N),0.0)*u_lights.color[q].rgb*u_lights.color[q].a;
			result+=Lr;
			continue;
		}

		if(u_lights.type[q]==0x03||u_lights.type[q]==0x04)//point or spot light
		{
			float dist=distance(u_lights.position[q],pos.xyz);
			float atten=1.0/(u_lights.attenuation[q].x+u_lights.attenuation[q].y*dist+u_lights.attenuation[q].z*pow(dist,2));

			vec3 L=normalize(pos.xyz-u_lights.position[q]);
			vec3 Lr=max(dot(-L,N),0.0)*u_lights.color[q].rgb*u_lights.color[q].a;

			if(u_lights.type[q]==0x04)
			{
				float cur_angle=dot(normalize(u_lights.rotation[q]),L);
				float cut_angle=cos(radians(u_lights.spot[q].x/2));

				if(cur_angle>cut_angle)
					Lr*=pow(cur_angle,u_lights.spot[q].y);
				else
					continue;
			}

			Lr=max(Lr*atten,vec3(0,0,0));
			result += Lr;
			continue;
		}
	}

	return result;
}

void main(void)
{
	pos = texture2D(u_source2,f_pos);
	pos.z *= -1;	
	if(length(pos.xyz)==0)
		discard;

	vec4 color = vec4(texture2D(u_source0,f_pos).rgb,1);


//	vec4 sample[25];
//	for (int i = 0; i < 25; i++) 
//	{
//		sample[i] = texture2D(u_source0, vec2(f_pos.s + f_offset.x*(i - (i/5)*5),f_pos.t + f_offset.y*(i/5)));
//	}
	//   1  4  7  4 1 
	//   4 16 26 16 4 
	//   7 26 41 26 7 / 273 
	//   4 16 26 16 4 
	//   1  4  7  4 1
//	color = (
//		(1.0  * (sample[0] + sample[4] + sample[20] + sample[24])) +
//		(4.0  * (sample[1] + sample[3] + sample[5] + sample[9] + 
//		sample[15] + sample[19] + sample[21] + sample[23])) +
//		(7.0  * (sample[2] + sample[10] + sample[14] + sample[22])) +
//		(16.0 * (sample[6] + sample[8] + sample[16] + sample[18])) +
//		(26.0 * (sample[7] + sample[11] + sample[13] + sample[17])) +
//		(41.0 * sample[12])
//		) / 273.0; 
//	vec3 eye = normalize(position.xyz);
	// float light = 1.0;//dot(eye,normal);
	float attenuation = 1.0;//position.z;

	//light *= smoothstep(0.95,1,eye.z);
	// if(eye.z<0.97)
	// {
	// 	light = pow(eye.z,100);
	// }

	vec3 light = vec3(1.0f);
	light = SetLighting();
	gl_FragData[0] = vec4(color.rgb*light*attenuation,1);
	gl_FragDepth = pos.w;
}