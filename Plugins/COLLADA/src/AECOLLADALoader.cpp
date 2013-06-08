/*
 * AECOLLADALoader.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: klokik
 */

//#include <dom.h>
#include <iostream>
#include <algorithm>
#include <set>

#include "AECOLLADALoader.h"
#include "AEObjectEmpty.h"
#include "AEObjectMesh.h"
#include "AEObjectCamera.h"
#include "AEObjectLight.h"

#include <dom/domCOLLADA.h>

using namespace ColladaDOM150;
using namespace aengine;

void AECOLLADALoader::processVisualScenes(void)
{
	library_visual_scenes=root->getDescendant("library_visual_scenes");
	if(!library_visual_scenes) return;

	daeElement *visual_scene=library_visual_scenes->getChild("visual_scene");
	if(!visual_scene) return;

	this->processSubNodes(visual_scene,NULL);
}

void AECOLLADALoader::processSubNodes(daeElement* node_parent,AEObject *parent)
{
	daeTArray<daeElementRef> children=node_parent->getChildren();

	for(unsigned int q=0;q<children.getCount();q++)
	{
		//check if it is really correct node
		std::string element=children[q]->getElementName();
		if(element!="node")
			continue;
		std::string type=children[q]->getAttribute("type").data();
		if(type!="NODE")
			continue;

		std::string name=children[q]->getAttribute("name").data();
		std::cout<<name<<std::endl;

		AEVector3f rotate={0,0,0};
		AEVector3f translate={0,0,0};
		AEVector3f scale={1,1,1};
		//get node transformation
		daeElement *etranslate=children[q]->getChild("translate");
		daeElement *escale=children[q]->getChild("scale");

		if(etranslate)
		{
			std::stringstream str_trans(etranslate->getCharData());
			str_trans>>translate.X>>translate.Y>>translate.Z;
		}

		if(escale)
		{
			std::stringstream str_scale(escale->getCharData());
			str_scale>>scale.X>>scale.Y>>scale.Z;
		}

		daeTArray<daeElementRef> arotate=children[q]->getChildren();
		//for each in node sub-elements
		for(unsigned int w=0;w<arotate.getCount();w++)
		{
			std::string elemname=arotate[w]->getElementName();
			if(elemname!="rotate")
				continue;
			std::stringstream strs(arotate[w]->getCharData());
			float angle,x,y,z;
			strs>>x>>y>>z>>angle;
			rotate.X+=x*angle;
			rotate.Y+=y*angle;
			rotate.Z+=z*angle;
		}

		AEObject *object=NULL;

		//detect if it's geometry
		domInstance_geometry *instance_geometry=(domInstance_geometry*)children[q]->getChild("instance_geometry");
		if(instance_geometry)
		{
			object=new AEObjectMesh;
			daeElement *geometry=instance_geometry->getUrl().getElement();
			if(geometry)
				((AEObjectMesh*)object)->mesh=this->processGeometry(geometry);
			domInstance_material *instance_material=(domInstance_material*)instance_geometry->getDescendant("instance_material");
			if(instance_material)
			{
				daeElement *mat=instance_material->getTarget().getElement();
				((AEObjectMesh*)object)->material=this->processMaterial(mat);
			}
			else
				((AEObjectMesh*)object)->material=scene->materials.New();
		}
		domInstance_camera *instance_camera=(domInstance_camera*)children[q]->getChild("instance_camera");
		if(instance_camera)
		{
			object=new AEObjectCamera;

			daeElement *camera=instance_camera->getUrl().getElement();
			processCamera(camera,(AEObjectCamera*)object);	
		}
		domInstance_light *instance_light=(domInstance_light*)children[q]->getChild("instance_light");
		if(instance_light)
		{
			object=new AEObjectLight;

			daeElement *light=instance_light->getUrl().getElement();
			processLight(light,(AEObjectLight*)object);
		}

		if(!object)
			object=new AEObjectEmpty;

		object->name=name;

		if(x_up)
		{
			std::swap(translate.X,translate.Y);
		}
		if(z_up)
		{
			std::swap(translate.Z,translate.Y);
			std::swap(rotate.Z,rotate.Y);
			std::swap(scale.Z,scale.Y);

			translate.X*=-1;
			rotate.X*=-1;
		}

		object->SetRotate(rotate);
		object->SetTranslate(translate);
		object->SetScale(scale);

		if(!parent)
			this->scene->AddObject(object);
		else
			parent->AddChild(object);

		processSubNodes(children[q],object);
	}
}

void AECOLLADALoader::processGeometries(void)
{
	this->library_geometries=root->getDescendant("library_geometries");
	if(!library_geometries)
		return;

	daeTArray<daeElementRef> geometry=library_geometries->getChildren();

	//foreach geometry element
	for(size_t q=0;q<geometry.getCount();q++)
	{

	}
}

void AECOLLADALoader::processMaterials(void)
{
}

AEMesh *AECOLLADALoader::processGeometry(daeElement *geometry)
{
	daeElement *emesh=geometry->getChild("mesh");

	/*
	 * read <polylist> section and read appropriate values
	 * then
	 * load vertices
	 * load normals
	 * load texture coordinates
	 * if they exists
	 */

	daeElement *epolylist=emesh->getDescendant("polylist");
	if(!epolylist) return NULL;
	daeTArray<daeElementRef> input=epolylist->getChildren();

	AEMesh *mesh=new AEMesh;

	size_t srcs=0;	//source count
	size_t voffset=0;
	size_t noffset=0;
	size_t toffset=0;

	std::set<int> offsets;	//counts unical offsets
	//foreach input
	for(size_t w=0;w<input.getCount();w++)
	{
		std::string ename(input[w]->getElementName());
		if(ename!="input") continue;

		int offs=this->processInput(mesh,input[w],&voffset,&noffset,&toffset);
		if(offs>=0) offsets.insert(offs);
	}
	srcs=offsets.size();

	//load indexes
	daeElement *ep=epolylist->getChild("p");
	daeElement *vcount=epolylist->getChild("vcount");
	if(ep&&vcount)
	{
		std::vector<size_t> pindex;
		AEVector3f *normals=NULL;
		std::stringstream indstr(ep->getCharData());
		std::stringstream vcstr(vcount->getCharData());

		size_t val;
		while(!indstr.eof())
		{
			indstr>>val;
			pindex.push_back(val);
		}

		if(mesh->nrmcount)
		{
			normals=(AEVector3f*)malloc(sizeof(AEVector3f)*mesh->nrmcount);
			memcpy(normals,mesh->nrm,sizeof(AEVector3f)*mesh->nrmcount);
			free(mesh->nrm);
			mesh->nrmcount=mesh->vtxcount;
			mesh->nrm=(AEVector3f*)malloc(sizeof(AEVector3f)*mesh->nrmcount);
		}

		std::vector<AETriangle> vfce;

		for(size_t q=0;!vcstr.eof();)
		{
			size_t vc;
			vcstr>>vc;

			size_t first=pindex[(q+0)*srcs+voffset];
			size_t last=pindex[(q+1)*srcs+voffset];
			size_t nlast=pindex[(q+1)*srcs+noffset];
			for(size_t w=2;w<vc;w++)
			{
				AETriangle triangle;
				triangle.A=first;
				triangle.B=last;
				triangle.C=pindex[(q+w)*srcs+voffset];
				last=triangle.C;
				vfce.push_back(triangle);

				if(normals)
				{
					mesh->nrm[triangle.A]=normals[pindex[(q+0)*srcs+noffset]];
					mesh->nrm[triangle.B]=normals[nlast];
					mesh->nrm[triangle.C]=normals[pindex[(q+w)*srcs+noffset]];

					nlast=pindex[(q+w)*srcs+noffset];
				}
			}
			q+=vc;
		}

		if(normals)
			free(normals);

		mesh->fcecount=vfce.size();
		mesh->fce=(AETriangle*)malloc(sizeof(AETriangle)*mesh->fcecount);
		for(size_t q=0;q<vfce.size();q++)
			mesh->fce[q]=vfce[q];

		//swap vertex positions
		for(size_t q=0;q<mesh->vtxcount;q++)
		{
			if(x_up) std::swap(mesh->vtx[q].X,mesh->vtx[q].Y);
			if(z_up)
			{
				std::swap(mesh->vtx[q].Z,mesh->vtx[q].Y);
				mesh->vtx[q].X*=-1;
			}
		}
		//swap normal positions
		for(size_t q=0;q<mesh->nrmcount;q++)
		{
			if(x_up) std::swap(mesh->nrm[q].X,mesh->nrm[q].Y);
			if(z_up)
			{
				 std::swap(mesh->nrm[q].Z,mesh->nrm[q].Y);
				 mesh->nrm[q].X*=-1;
			}
		}
	}

	return mesh;
}

AEMaterial* AECOLLADALoader::processMaterial(daeElement* material)
{
	AEMaterial *mat=NULL;

	if(!material) return mat;

	std::cout<<material->getAttribute("name").data()<<std::endl;

	domInstance_effect *instance_effect=(domInstance_effect*)material->getDescendant("instance_effect");
	if(!instance_effect)
		return mat;

	daeElement *effect=instance_effect->getUrl().getElement();
	if(!effect)
		return mat;

	mat=this->scene->materials.New();

	mat->diffuse=this->getColor(effect,"diffuse");
	mat->emissive=this->getColor(effect,"emission");
	mat->specular=this->getColor(effect,"specular");

	return mat;
}

AEColor AECOLLADALoader::getColor(daeElement *ancestor,const char *property)
{
	AEColor color={0.0f,0.0f,0.0f,1.0f};
	daeElement *prop=ancestor->getDescendant(property);
	if(!prop)
		return color;

	daeElement *ecolor=prop->getDescendant("color");

	if(!ecolor)
		return color;

	std::stringstream val(ecolor->getCharData());

	val>>color.R>>color.G>>color.B;
	if(!val.eof()) val>>color.A;

	return color;
}

float AECOLLADALoader::getFloat(daeElement *ancestor,const char *property)
{
	daeElement *prop=ancestor->getDescendant(property);
	if(!prop) return 0;

	return atof(prop->getCharData().c_str());
}

void AECOLLADALoader::processLight(daeElement *elight,AEObjectLight *light)
{
	if(!elight||!light) return;
	light->color=getColor(elight,"technique_common");

	//Detect light emmiter type
	daeElement *el=NULL;

	el=elight->getDescendant("ambient");
	if(el) light->light_type=AE_AMBIENT_LIGHT;
	el=elight->getDescendant("directional");
	if(el) light->light_type=AE_DIRECTIONAL_LIGHT;
	el=elight->getDescendant("point");
	if(el) light->light_type=AE_POINT_LIGHT;
	el=elight->getDescendant("spot");
	if(el) light->light_type=AE_SPOT_LIGHT;

	if(light->light_type==AE_SPOT_LIGHT||light->light_type==AE_POINT_LIGHT)
	{
		AEVector3f attenuation={1.0f,0.0f,0.0f}; //constant|linear|quadratic

		attenuation.X=getFloat(elight,"constant_attenuation");
		attenuation.Y=getFloat(elight,"linear_attenuation");
		attenuation.Z=getFloat(elight,"quadratic_attenuation");

		light->attenuation=attenuation;

		if(light->light_type==AE_SPOT_LIGHT)
		{
			light->spot.X=getFloat(elight,"falloff_angle");
			light->spot.Y=getFloat(elight,"falloff_exponent");
		}
	}
}

void AECOLLADALoader::processCamera(daeElement *camera,AEObjectCamera *object)
{
	if(!camera||!object)
		return;

	object->angle=getFloat(camera,"xfov");
}

int AECOLLADALoader::processInput(AEMesh *mesh,daeElement *input,
		size_t *voffset,
		size_t *noffset,
		size_t *toffset)
{
	if((!mesh)||(!input)) return -1;

	std::string ename(input->getElementName());
	if(ename!="input") return -1;

	std::string semantics=input->getAttribute("semantic");
	std::string source=input->getAttribute("source");
	source.erase(0,1);
	size_t att_offset=atoi(input->getAttribute("offset").c_str());

	//unified mesh destinations
	size_t *offset=NULL;
	size_t unit_size=0;
	AEVector3f **units=NULL;
	size_t *unit_count=NULL;

	//load vertices
	if(semantics=="VERTEX")
	{
		offset=voffset;
		unit_size=sizeof(AEVertexf);
		units=&mesh->vtx;
		unit_count=(size_t*)&mesh->vtxcount;

		//look for <vertices> element and get it's input.source
		daeElement *evertices_input=dae.getDatabase()
				->idLookup(source,root->getDocument())
				->getChild("input");
		source=evertices_input->getAttribute("source");
		source.erase(0,1);
	}
	else
	{
		if(semantics=="NORMAL")
		{
			offset=noffset;
			unit_size=sizeof(AEVector3f);
			units=&mesh->nrm;
			unit_count=(size_t*)&mesh->nrmcount;
		}
		else
			return att_offset;
	}

	*offset=att_offset;

	daeElement *esource=dae.getDatabase()->idLookup(source,root->getDocument());
	daeElement *efarray=esource->getChild("float_array");

	*unit_count=atoi(efarray->getAttribute("count").c_str())/3;
	std::stringstream fstr(efarray->getCharData());
	std::cout<<"count: "<<*unit_count<<std::endl;

	*units=(AEVector3f*)malloc(sizeof(unit_size)*(*unit_count));

	for(size_t e=0;e<*unit_count;e++)
		fstr>>(*units)[e].X>>(*units)[e].Y>>(*units)[e].Z;

	return *offset;
}

AECOLLADALoader::AECOLLADALoader(void)
{
	this->root=NULL;
	this->library_visual_scenes=NULL;
	this->library_geometries=NULL;
	this->library_materials=NULL;
	this->library_effects=NULL;

	this->scene=NULL;

	this->x_up=y_up=z_up=0;
}

int AECOLLADALoader::Load(AEScene* _scene, const char* filename)
{
	this->scene=_scene;
	if(!_scene||!filename) return 0;

	root=dae.open(filename);
	if(!root) return 0;

	daeElement *up_axis=root->getDescendant("up_axis");
	this->x_up=y_up=z_up=0;
	char axis='y';
	if(up_axis) axis=up_axis->getCharData()[0];
	switch(axis)
	{
		case 'x':
		case 'X':
			this->x_up=1;
		break;
		case 'y':
		case 'Y':
			this->y_up=1;
		break;
		case 'z':
		case 'Z':
			this->z_up=1;
		break;
	}

	this->processVisualScenes();

	dae.close(filename);

	return _scene->objects.size();
}
