#include "Object/MeshData.class.hpp"
#include <math.h>
#include <string>

//			TMP - REMOVE AFTER MATH LIB UPDATE			\\
//			vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv			\\

namespace mft
{
	float		radians(float degrees) {
		return (degrees * 0.01745329251994329576923690768489);
	}
	float		degrees(float radians) {
		return (radians * 57.295779513082320876798154814105);
	}

	float	rsqrt(float nb)
	{
		unsigned int		i;
		float		x2, y;

		x2 = nb * 0.5f;
		y = nb;
		unsigned int* p1 = reinterpret_cast<unsigned int*>(&nb);
		i = 0x5f3759df - (*p1 >> 1);
		float* p2 = reinterpret_cast<float*>(&i);
		float tmp = *p2;
		y = tmp * (1.5f - (x2 * tmp * tmp));
		return y;
	}

	template<int l, typename T>
	vec<l, T>	rsqrt(vec<l, T> const& v)
	{
		vec<l, float>	x2(v * 0.5f);
		vec<l, float>	y(v);
		vec<l, unsigned int>* p1 = reinterpret_cast<vec<l, unsigned int>*>(const_cast<vec<l, float>*>(&v));
		vec<l, unsigned int>	i = (unsigned int)0x5f3759df - (*p1 >> (unsigned int)1);
		vec<l, float>* p2 = reinterpret_cast<vec<l, float>*>(&i);
		vec<l, float>	tmp = *p2;
		vec<l, float>	res = tmp * (1.5f - (x2 * tmp * tmp));
		return res;
	}

	template<typename T>
	vec<3, T>	cross(vec<3, T> const& v1, vec<3, T> const& v2)
	{
		return vec<3, T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	template<typename T>
	T	dot(vec<3, T> const& v1, vec<3, T> const& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}


	template<typename T>
	T	dot(vec<4, T> const& v1, vec<4, T> const& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
	}

	//	Normalized vector = v / |v|
	//	wich is also v * 1 / |v|
	//	and |v| is sqrt(v.x * v.x + ... + v.n * v.n)
	//	which is sqrt(dot(v, v))
	//	that gives us v * 1 / sqrt(v * v)
	//	ie v * rsqrt(v * v)
	template <int l, typename T>
	vec<l, T>	normalize(vec<l, T> const& v)
	{
		return (v * rsqrt(dot(v, v)));
	}

	template<typename T>
	mat<4, 4, T> translate(mat<4, 4, T> const& m, vec<3, T> const& v)
	{
		mat<4, 4, T>	res(m);
		//res[3][0] += v.x;
		//res[3][1] += v.y;
		//res[3][2] += v.z;
		res[0][3] += v.x;
		res[1][3] += v.y;
		res[2][3] += v.z;
		return res;
	}

	template<typename T>
	mat<4, 4, T> scale(mat<4, 4, T> const& m, vec<3, T> const& v)
	{
		mat<4, 4, T>	res(m);
		res[0][0] *= v.x;
		res[1][1] *= v.y;
		res[2][2] *= v.z;
		return res;
	}

	template<typename T>
	mat<4, 4, T> rotate(mat<4, 4, T> const& m, float angle, vec<3, T> const& v)
	{
		float	c = cos(angle);
		float	s = sin(angle);
		float	minc = 1 - c;

		vec<3, T>	unit = normalize(v);
		mat<4, 4, T>	rot(
			c + unit.x * unit.x * minc, unit.x * unit.y * minc - unit.z * s, unit.x * unit.z * minc + unit.y * s, 0,
			unit.y * unit.x * minc + unit.z * s, c + unit.y * unit.y * minc, unit.y * unit.z * minc - unit.x * s, 0,
			unit.z * unit.x * minc - unit.y * s, unit.z * unit.y * minc - unit.x * s, c + unit.z * unit.z * minc, 0,
			0, 0, 0, 1
		);
		return (m * rot);
	}

	template<typename T>
	mat<4, 4, T> look_at(vec<3, T> const& pos, vec<3, T> const& target, vec<3, T> const& up)
	{
		vec<3, T>	right = normalize(cross(normalize(up), target));
		mat<4, 4, T>	res(
			right.x, right.y, right.z, 0,
			up.x, up.y, up.z, 0,
			target.x, target.y, target.z, 0,
			0, 0, 0, 1
		);
		return translate(res, pos);
	}
}
//			^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^			\\
//			TMP - REMOVE AFTER MATH LIB UPDATE			\\

namespace notrealengine
{
	//	Constructors

	MeshData::MeshData() 
		: vertices(), indices(), textures(),
			transform{ mft::vec3(0, 0, 0),
			mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) }, matrix()
	{
	}

	MeshData::MeshData(MeshData const& meshData)
	{
		*this = meshData;
	}

	MeshData::~MeshData()
	{
	}

	MeshData::MeshData(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures)
		: vertices(vertices), indices(indices), textures(textures),
			transform { mft::vec3(0, 0, 0), mft::vec3(0, 0, 0), mft::vec3(1, 1, 1) },
			matrix()
	{
		setup();
	}

	MeshData& MeshData::operator=(MeshData const& meshData)
	{
		this->vertices = meshData.vertices;
		this->indices = meshData.indices;
		this->textures = meshData.textures;

		this->name = meshData.name;

		this->transform = meshData.transform;
		this->matrix = meshData.matrix;
		return *this;
	}

	//	Accessors

	std::vector<Vertex>
		MeshData::getVertices() const
	{
		return vertices;
	}

	std::vector<unsigned int>
		MeshData::getIndices() const
	{
		return indices;
	}

	std::vector<Texture>
		MeshData::getTextures() const
	{
		return textures;
	}

	mft::mat4	MeshData::getMatrix() const
	{
		return matrix;
	}

	void	MeshData::update(void)
	{
		matrix = mft::mat4(1);
		matrix = mft::translate(matrix, transform.pos);
		matrix = mft::rotate(matrix, transform.rotation.x, mft::vec3(1.0f, 0.0f, 0.0f));
		matrix = mft::rotate(matrix, transform.rotation.y, mft::vec3(0.0f, 1.0f, 0.0f));
		matrix = mft::rotate(matrix, transform.rotation.z, mft::vec3(0.0f, 0.0f, 1.0f));
		matrix = mft::scale(matrix, transform.scale);
		std::cout << "Mesh matrix = " << std::endl << matrix << std::endl;
	}

	void	MeshData::move(mft::vec3 move)
	{
		transform.pos = transform.pos + move;
		update();
	}

	void	MeshData::rotate(mft::vec3 rotation)
	{
		transform.rotation = transform.rotation + rotation;
		update();
	}

	void	MeshData::scale(mft::vec3 scale)
	{
		transform.scale = transform.scale + scale;
		update();
	}

	//	Main functions

	//		Initalize buffers and vertex array for the mesh
	void	MeshData::setup()
	{
	}

	std::ostream& operator<<(std::ostream& o, MeshData const& meshData)
	{
		std::vector<Vertex>			vertices = meshData.getVertices();
		std::vector<unsigned int>	indices = meshData.getIndices();
		std::vector<Texture>		textures = meshData.getTextures();
		std::cout << "\t--Vertices--" << std::endl;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			std::cout << "\t" << vertices[i].pos << vertices[i].norm << vertices[i].uv << std::endl;
		}
		std::cout << "\t--Indices--" << std::endl;
		for (size_t i = 0; i < indices.size(); i++)
		{
			std::cout << "\t" << indices[i] << std::endl;
		}
		std::cout << "\t--Textures--" << std::endl;
		for (size_t i = 0; i < textures.size(); i++)
		{
			std::cout << "\t" << textures[i].path << std::endl;
		}
		return o;
	}
}