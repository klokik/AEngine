#include <iostream>
#include <fstream>
#include <string>

namespace Zombie
{
	class Mob;
	class ZombieWorld;

	class Item
	{
	public:
		enum ItemType {WEAPON,FOOD,TOOL};
		enum ItemState {ACTIVE=1,INACTIVE=2,BROKEN=4};

		ItemType item_type;
		ItemState state;

		std::string name;
		float value;
		float health;
		int amount;


		void apply(Mob &mob);

		void activate();
		void deactivate();
		void repair();
	};

	struct Container
	{
	public:
		int id;
		std::string name;
		std::vector<Item> items;
	};

	class Graph
	{
	public:
		class Edge
		{
			uint a;
			uint b;
		};

		std::vector<Vec3f> vertexes;
		std::vector<Edge> edges;
		// std::vector<AETriangle> triangles;
		// std::vector<Tetrahedron> tetrahedrons;

		struct Trianglef
		{
			Vec3f A;
			Vec3f B;
			Vec3f C;

			enum Flags{
				NONE	=	0,
				GROUND	=	1,
				GRASS	=	2,
				SAND	=	4,
				ROCK	=	8,
				CONCRETE=	16,
				WATER	=	32,
				FIRE	=	64,
				SHELTER	=	128
			};

			Flags flags;

			int container_id;
		};

		std::vector<Trianglef> triangles;

		std::map<int,Container> containers;

		bool sign(Vec3f p1,Vec3f p2,Vec3f p3)
		{
			return ((p1.X - p3.X) * (p2.Z - p3.Z) - (p2.X - p3.X) * (p1.Z - p3.Z)) < 0.0f;
		}

		struct TestResult
		{
			float altitude;
			Vec3f move_vec;
			bool passed;
			int container_id;

			Trianglef::Flags flags;
		};

		int isInside(Vec3f pos)
		{
			for(size_t q=0;q<triangles.size();q++)
			{
				Trianglef &triag = triangles[q];

				bool b1 = sign(pos,triag.A,triag.B);
				bool b2 = sign(pos,triag.B,triag.C);
				bool b3 = sign(pos,triag.C,triag.A);

				if(b1==b2&&b2==b3)
					return q;
			}

			return -1;
		}

		float getAltitude(Vec3f pos, int id)
		{
			Trianglef t = triangles[id];
			float a11 = pos.X - t.A.X;
			float a13 = pos.Z - t.A.Z;
			float a21 = t.A.X - t.B.X;
			float a22 = t.A.Y - t.B.Y;
			float a23 = t.A.Z - t.B.Z;
			float a31 = t.A.X - t.C.X;
			float a32 = t.A.Y - t.C.Y;
			float a33 = t.A.Z - t.C.Z;

			float A12 = -(a21*a33 - a31*a23);
			float A22 = a11*a33 - a31*a13;
			float A32 = -(a11*a23 - a21*a13);
			float delta = a22*A22 + a32*A32;

			return t.A.Y - delta/A12;
		}

		TestResult test(Vec3f pos,Vec3f chunk_offset)
		{
			Vec3f point = pos-chunk_offset;
			int index;
			index = isInside(point);

			TestResult result;
			result.passed = (index!=-1);

			if(result.passed)
				result.altitude = getAltitude(point,index);
			else
				result.altitude = 0.0f;

			result.move_vec = vec3f(0,0,0);
			result.flags = Trianglef::NONE;

			result.container_id = triangles[index].container_id;

			return result;
		}

		void addVertex(Vec3f pos);
		void addEdge(Edge edge);
		void addEdge(Vec3f a, Vec3f b);

		void addTriangle(Vec3f a, Vec3f b, Vec3f c)
		{
			Trianglef triag = {a,b,c};
			triangles.push_back(triag);
		}

		void addTetrahedron(Vec3f a, Vec3f b, Vec3f c, Vec3f d);

		void loadFromFile(const char *filename)
		{
			std::ifstream istr;

			istr.open(filename, std::fstream::in);

			Trianglef triag;

			while(istr.good())
			{
				istr >> triag.A.X >> triag.A.Y >> triag.A.Z;
				istr >> triag.B.X >> triag.B.Y >> triag.B.Z;
				istr >> triag.C.X >> triag.C.Y >> triag.C.Z;
				istr >> (int&)triag.flags;

				triangles.push_back(triag);
			}

			std::cout<<triangles.size()<<" Triangles loaded"<<std::endl;
		}
	};
}