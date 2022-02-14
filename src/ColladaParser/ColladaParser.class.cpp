#include "ColladaParser/ColladaParser.class.hpp"

namespace notrealengine
{
	ColladaParser::ColladaParser(): scene(nullptr)
	{
	}

	ColladaParser::~ColladaParser()
	{
		std::cout << "ColladaParser destructor" << std::endl;
		if (this->scene == nullptr)
			return;
		std::cout << "Freeing scene" << std::endl;
		for (unsigned int i = 0; i < this->scene->mNumAnimations; i++)
		{

		}
		for (unsigned int i = 0; i < this->scene->mNumMeshes; i++)
		{
			delete this->scene->mMeshes[i];
		}
		for (unsigned int i = 0; i < this->scene->mNumMaterials; i++)
		{

		}
		for (unsigned int i = 0; i < this->scene->mNumTextures; i++)
		{

		}
		delete this->scene->mMeshes;
		delete this->scene;
	}

	bool	ColladaParser::GetAttribute(const lxml::Tag& tag, const std::string& attribute,
		std::string& value)
	{
		for (const auto& attr : tag.attributes)
		{
			if (attr.name == attribute)
			{
				value = attr.value;
				return true;
			}
		}
		return false;
	}

	unsigned long	ColladaParser::GetUIntAttribute(const std::vector<lxml::Attribute>& attributes, const std::string& name)
	{
		unsigned long	res = 0;
		for (const auto& attr : attributes)
		{
			if (attr.name == "offset")
			{
				try
				{
					res = std::stoul(attr.value);
				}
				catch (std::out_of_range& e)
				{
					throw e;
				}
			}
		}
		return res;
	}
	int		ColladaParser::GetIntAttribute(const std::vector<lxml::Attribute>& attributes, const std::string& name)
	{
		int	res = 0;
		for (const auto& attr : attributes)
		{
			if (attr.name == "offset")
			{
				try
				{
					res = std::stoi(attr.value);
				}
				catch (std::out_of_range& e)
				{
					throw e;
				}
			}
		}
		return res;
	}

	cpMesh* ColladaParser::ReadMesh(const lxml::Tag& geometry, const lxml::Tag& mesh)
	{
		cpMesh*	newMesh = new cpMesh();
		int vertexOffset = 0;
		int normalOffset = 0;
		int textureOffset = 0;
		for (const auto& child : mesh.children)
		{
			if (child.name == "input")
			{
				std::string value;
				if (GetAttribute(child, "semantic", value) == true)
				{
					if (value == "VERTEX")
					{
						try {
							vertexOffset = GetIntAttribute(child.attributes, "offset");
						}
						catch (std::out_of_range& e) {
							std::cerr << "Vertex offset overflow" << std::endl;
							throw e;
						}
					}
					if (value == "NORMAL")
					{
						try {
							normalOffset = GetIntAttribute(child.attributes, "offset");
						}
						catch (std::out_of_range& e) {
							std::cerr << "Normal offset overflow" << std::endl;
							throw e;
						}
					}
					if (value == "TEXCOORD")
					{
						try {
							textureOffset = GetIntAttribute(child.attributes, "offset");
						}
						catch (std::out_of_range& e) {
							std::cerr << "Texture offset overflow" << std::endl;
							throw e;
						}
					}
				}
			}
		}
		std::cout << "Vertex offset = " << vertexOffset << std::endl;
		std::cout << "Normal offset = " << normalOffset << std::endl;
		std::cout << "Texture offset = " << textureOffset << std::endl << std::endl;
		unsigned int long count = GetUIntAttribute(mesh.attributes, "count");
		std::cout << "Mesh has " << count << " triangles" << std::endl;
		for (const auto& child : mesh.children)
		{
			if (child.name == "p")
			{

			}
		}
		return newMesh;
	}

	void	ColladaParser::ReadGeometry(const lxml::Importer& parser)
	{
		const lxml::Tag* geometries = parser.FindTag(parser.RootTag, "library_geometries");
		if (geometries == nullptr)
		{
			std::cerr << "No geometry found" << std::endl;
			return;
		}
		for (const auto& child : geometries->children)
		{
			if (child.name == "geometry")
			{
				for (const auto& geoChild : child.children)
				{
					if (geoChild.name == "mesh")
					{
						this->scene->mNumMeshes++;
					}
				}
			}
		}
		this->scene->mMeshes = new cpMesh * [this->scene->mNumMeshes];
		unsigned int meshIndex = 0;
		for (const auto& child : geometries->children)
		{
			if (child.name != "geometry")
				continue;
			for (const auto& geoChild : child.children)
			{
				if (geoChild.name != "mesh")
					continue;
				for (const auto& meshChild : geoChild.children)
				{
					if (meshChild.name == "triangles"
						)//&& meshChild.name != "polygons")
					{
						this->scene->mMeshes[meshIndex] = ReadMesh(geoChild, meshChild);
						if (this->scene->mMeshes[meshIndex] == nullptr)
							throw std::exception();
						for (const auto& attribute : child.attributes)
						{
							if (attribute.name == "id")
							{
								this->scene->mMeshes[meshIndex]->mName = attribute.value;
								break;
							}
						}
						meshIndex++;
					}
				}
			}
		}
	}

	cpScene*	ColladaParser::ReadFile(const std::string& path, unsigned int flags)
	{
		lxml::Importer	xmlParser;
		xmlParser.ReadFile(path);

		if (xmlParser.RootTag.valid == false)
			return nullptr;

		this->scene = new cpScene();

		try
		{
			ReadGeometry(xmlParser);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return nullptr;
		}

		std::cout << "My parser reads " << scene->mNumMeshes << " meshes " << std::endl;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			std::cout << "Mesh " << i << " name is " << scene->mMeshes[i]->mName << std::endl;
		}

		return nullptr;
	}
}