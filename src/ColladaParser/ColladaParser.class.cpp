#include "ColladaParser/ColladaParser.class.hpp"
#include "ColladaParser/ColladaException.class.hpp"

namespace notrealengine
{
	ColladaParser::ColladaParser() :
		meshes(), accessors(), floats(), strings(),
		nodes(), images(), materials(), effects(),
		axis(Y_UP), rootNode(nullptr)
	{
	}

	ColladaParser::~ColladaParser()
	{
		for (auto& pair : this->meshes)
		{
			delete pair.second;
		}
	}

	void	ColladaParser::ReadArray(const lxml::Tag& arrayTag)
	{
		bool isString = false;
		if (arrayTag.name == "IDREF_array" || arrayTag.name == "Name_array")
			isString = true;

		std::string	id;
		lxml::GetStrAttribute(arrayTag, "id", id);

		int	count = 0;
		lxml::GetIntAttribute(arrayTag, "count", count);

		const char* content = arrayTag.content.c_str();

		if (!content)
			return;
		if (isString)
		{
			std::vector<std::string> strings;
			strings.reserve(count);

			std::string str;

			for (unsigned int i = 0; i < count; i++)
			{
				if (!*content)
					throw ColladaException("Not enough values in string array");

				str.clear();
				while (*content && !lxml::IsValidWhitespace(*content))
				{
					str += *content;
					content++;
				}
				strings.push_back(str);

				content = lxml::SkipWhitespaces(content);
			}

			this->strings[id] = strings;
		}
		else
		{
			std::vector<float> floats;
			floats.reserve(count);

			for (unsigned int i = 0; i < count; i++)
			{
				if (!*content)
					throw ColladaException("Not enough values in float array");

				float	nb;
				char* end = nullptr;
				nb = std::strtof(content, &end);
				content = end;

				floats.push_back(nb);
				content = lxml::SkipWhitespaces(content);
			}

			this->floats[id] = floats;
		}
	}

	void	ColladaParser::ReadAccessor(const lxml::Tag& accessorTag, const std::string& id)
	{
		std::string source;
		lxml::GetStrAttribute(accessorTag, "source", source);

		if (source[0] != '#')
			throw ColladaException("Invalid accessors format: missing '#'");

		unsigned int count = 0;
		lxml::GetUIntAttribute(accessorTag, "count", count);

		unsigned int offset = 0;
		lxml::GetUIntAttribute(accessorTag, "offset", offset);

		unsigned int stride = 1;
		lxml::GetUIntAttribute(accessorTag, "stride", stride);

		ColladaAccessor accessor = ColladaAccessor();
		accessor.count = count;
		accessor.offset = offset;
		accessor.stride = stride;
		accessor.sourceId = source.c_str() + 1;

		count = 0;
		for (const auto& child : accessorTag.children)
		{
			if (child.name == "param")
			{
				std::string name;
				lxml::GetStrAttribute(child, "name", name);
				if (name.empty())
					continue;
				// Vertices pos
				if (name == "X")
					accessor.subOffset[0] = count;
				else if (name == "Y")
					accessor.subOffset[1] = count;
				else if (name == "Z")
					accessor.subOffset[2] = count;

				// RGBA colors
				else if (name == "R")
					accessor.subOffset[0] = count;
				else if (name == "G")
					accessor.subOffset[1] = count;
				else if (name == "B")
					accessor.subOffset[2] = count;
				else if (name == "A")
					accessor.subOffset[3] = count;

				// Texture coordinates
				else if (name == "S")
					accessor.subOffset[0] = count;
				else if (name == "T")
					accessor.subOffset[1] = count;
				else if (name == "P")
					accessor.subOffset[2] = count;
				else if (name == "U")
					accessor.subOffset[0] = count;
				else if (name == "V")
					accessor.subOffset[1] = count;

				accessor.params[count] = name;
				count++;
			}
		}
		this->accessors[id] = accessor;
	}

	void	ColladaParser::ReadSource(const lxml::Tag& sourceTag)
	{
		std::string id;
		lxml::GetStrAttribute(sourceTag, "id", id);
		for (const auto& child : sourceTag.children)
		{
			if (child.name == "float_array" || child.name == "IDREF_array"
				|| child.name == "Name_array")
				ReadArray(child);
			else if (child.name == "technique_common")
			{
				for (const auto& techChild : child.children)
				{
					if (techChild.name == "accessor")
						ReadAccessor(techChild, id);
				}
			}
		}
	}

	void	ColladaParser::ReadInput(const lxml::Tag& inputTag, std::vector<Input>& inputs)
	{
		std::string semantic;
		lxml::GetStrAttribute(inputTag, "semantic", semantic);

		InputType type = InvalidInput;
		if (semantic == "VERTEX")
			type = VertexInput;
		else if (semantic == "NORMAL")
			type = NormalInput;
		else if (semantic == "TEXCOORD")
			type = TexCoordInput;
		else if (semantic == "POSITION")
			type = PositionInput;
		else if (semantic == "COLOR")
			type = ColorInput;
		else if (semantic == "BINORMAL" || semantic == "TEXBINORMAL")
			type = BitangentInput;
		else if (semantic == "TANGENT" || semantic == "TEXTANGENT")
			type = TangentInput;
		if (type == InvalidInput)
		{
			std::cerr << "Invalid input semantic" << std::endl;
			//Can ignore instead of exiting
			//throw std::exception();
		}

		std::string source;
		lxml::GetStrAttribute(inputTag, "source", source);
		if (source[0] != '#')
			throw ColladaException("Invalid input source format: missing '#'");
		Input input = Input();
		input.type = type;
		input.id = source.c_str() + 1;
		//	We don't resolve references on the fly but at the
		//	last moment to make sure we have read all the data
		unsigned int offset = 0;
		lxml::GetUIntAttribute(inputTag, "offset", offset);
		input.offset = static_cast<size_t>(offset);
		unsigned int set = 0;
		lxml::GetUIntAttribute(inputTag, "set", set);
		if (type != TexCoordInput && type != ColorInput
			&& set != 0)
			throw ColladaException("Only Texture coordinates and colors accept more than 1 channel");
		input.set = static_cast<size_t>(set);
		if (type != InvalidInput)
			inputs.push_back(input);
	}

	void	ColladaParser::ResolveInputsReferences(std::vector<Input>& inputs,
		ColladaMesh& mesh)
	{
		for (auto& input : inputs)
		{
			//	Check that that "VERTEX" <input> tags
			//	correctly refers to the <vertex> tag
			if (input.type == VertexInput)
			{
				if (input.id != mesh.vertexId)
					throw ColladaException("Reference to non existing <vertices> input");
				continue;
			}
			input.accessor = &ResolveReference(this->accessors, input.id,
				"geometry input accessor");
			input.accessor->floatData = &ResolveReference(this->floats,
				input.accessor->sourceId, "geometry input float array");
		}
	}

	void	ColladaParser::ReadPrimitives(const lxml::Tag& primTag, ColladaMesh& mesh,
		std::vector<Input>& inputs, size_t count, std::vector<unsigned int>& vcount,
		PrimitiveType type)
	{
		//	The number of attributes each vertex has
		size_t	nbAttributes = 1;
		size_t	positionOffset = 0; // The actual POSITION offset is given in the VERTEX input..
		for (const auto& input : inputs)
		{
			nbAttributes = std::max(nbAttributes, input.offset);
			if (input.type == InputType::VertexInput)
				positionOffset = input.offset;
		}
		nbAttributes++; // If max offset is 3, there are 4 attributes
		for (auto& input : inputs)
		{
			if (input.type == InputType::PositionInput)
				input.offset = positionOffset;
		}

		size_t	expectedCount = 0;
		switch (type)
		{
		case P_Lines:
			expectedCount = count * 2;
			break;
		case P_Triangles:
			expectedCount = count * 3;
			break;
		case P_Polylist:
			for (const auto& i : vcount)
				expectedCount += i;
			break;
		default:
			break;
		}

		std::vector<unsigned int>	indices;
		if (expectedCount > 0)
			indices.reserve(expectedCount);

		if (count > 0)
		{
			const char* content = primTag.content.c_str();
			char* next = nullptr;
			lxml::SkipWhitespaces(content);
			while (*content)
			{
				long value = std::strtol(content, &next, 10);
				//	It seems that we can find negative values sometimes...
				value = std::max((long)0, value);
				indices.push_back(static_cast<unsigned int>(value));
				content = next;
				content = lxml::SkipWhitespaces(content);
			}
		}

		//	Some checks to ensure the number of read indices is valid
		if (expectedCount > 0 && indices.size() != expectedCount * nbAttributes)
			throw ColladaException("Invalid number of primitives: expected "
			+ std::to_string(expectedCount * nbAttributes) + ", got "
			+ std::to_string(indices.size()));
		if (expectedCount == 0 && (indices.size() % nbAttributes) != 0)
			throw ColladaException("Invalid number of primitives: "
			+ std::to_string(indices.size()) + " indices with "
			+ std::to_string(nbAttributes) + " attributes each is not round");

		//	We don't resolve references on the fly but at the
		//	last moment to make sure we have all the needed data
		ResolveInputsReferences(inputs, mesh);

		mesh.faceSizes.reserve(count);
		mesh.indices.reserve(indices.size() / nbAttributes);

		//	Actual data retrieving

		//	The number of vertices composing a primitive/face
		size_t primitiveSize = 0;

		//	For polylists we need to keep track of the total
		//	primitive's size because each primitive/face size
		//	can be different
		size_t	polylistVertexIndex = 0;

		for (size_t primitiveIndex = 0; primitiveIndex < count; primitiveIndex++)
		{
			switch (type)
			{
			case P_Lines:
				primitiveSize = 2;
				for (size_t vertexIndex = 0; vertexIndex < primitiveSize; vertexIndex++)
				{
					ReadVertex(primitiveIndex * primitiveSize * nbAttributes
						+ vertexIndex * nbAttributes,
						mesh, indices, inputs);
				}
				break;
			case P_Triangles:
				primitiveSize = 3;
				for (size_t vertexIndex = 0; vertexIndex < primitiveSize; vertexIndex++)
				{
					ReadVertex(primitiveIndex * primitiveSize * nbAttributes
						+ vertexIndex * nbAttributes,
						mesh, indices, inputs);
				}
				break;
			case P_Polylist:
				primitiveSize = vcount[primitiveIndex];
				for (size_t vertexIndex = 0; vertexIndex < primitiveSize; vertexIndex++)
				{
					//	Specific case here:
					//	We can't compute from a global primitive
					//	size since it can change for each primitive.
					//	Instead we keep track of the index by
					//	incrementing its value
					ReadVertex((polylistVertexIndex + vertexIndex) * nbAttributes,
						mesh, indices, inputs);
				}
				//	Keep track of the vertex index
				polylistVertexIndex += primitiveSize;
				break;
			default:
				break;
			}
			mesh.faceSizes.push_back(primitiveSize);
		}

	}

	void	ColladaParser::ReadVertex(size_t index, ColladaMesh& mesh,
		std::vector<unsigned int>& indices, std::vector<Input>& inputs)
	{
		//	For each vertex we get every input one by one

		for (const auto& input : inputs)
		{
			//	Ignore Vertex input because it is a specific input
			//	that references the position input and does nothing else..
			if (input.type == VertexInput)
				continue;
			//	No risk here, if the accessor could not be resolved
			//	an expection was thrown
			const ColladaAccessor& acc = *input.accessor;
			//	Make sure we don't read out of range indices
			unsigned int readIndex = indices[index + input.offset];
			if (readIndex > acc.count + 1)
			{
				//InputType type = input.type;
				throw ColladaException("Invalid type " + std::to_string(input.type)
					+ " input: index: " + std::to_string(readIndex) + " / "
					+ std::to_string(acc.count) + " (index = " + std::to_string(index)
					+ ", offset " + std::to_string(input.offset) + ")");

			}
			//	No risk here, if the float array could not be resolved
			//	an expection was thrown
			const std::vector<float>& values = *input.accessor->floatData;
			switch (input.type)
			{
			case PositionInput:
			{
				mft::vec3	pos;
				pos.x = values[readIndex * acc.stride + acc.offset + acc.subOffset[0]];
				pos.y = values[readIndex * acc.stride + acc.offset + acc.subOffset[1]];
				pos.z = values[readIndex * acc.stride + acc.offset + acc.subOffset[2]];
				mesh.pos.push_back(pos);
				break;
			}
			case TexCoordInput:
			{
				if (input.set < MAX_TEXTURE_CHANNELS)
				{
					mft::vec3	tex;
					tex.s = values[readIndex * acc.stride + acc.offset + acc.subOffset[0]];
					tex.t = values[readIndex * acc.stride + acc.offset + acc.subOffset[1]];
					if (acc.subOffset[2] != 0)
						tex.p = values[readIndex * acc.stride + acc.offset + acc.subOffset[2]];
					if (acc.subOffset[3] != 0)
						tex.p = values[readIndex * acc.stride + acc.offset + acc.subOffset[3]];
					mesh.tex[input.set].push_back(tex);
					//	If component 3 or 4 exist, there are more than
					//	U and V tex coord but no more than 3
					if (acc.subOffset[2] != 0 || acc.subOffset[3] != 0)
						mesh.nbUVComponents[input.set] = 3;
				}
				break;
			}
			case ColorInput:
			{
				//	Ignore more than MAX_TEXTURE_CHANNELS textures
				if (input.set < MAX_TEXTURE_CHANNELS)
				{
					mft::vec4	color;
					color.r = values[readIndex * acc.stride + acc.offset + acc.subOffset[0]];
					color.g = values[readIndex * acc.stride + acc.offset + acc.subOffset[1]];
					color.b = values[readIndex * acc.stride + acc.offset + acc.subOffset[2]];
					color.a = values[readIndex * acc.stride + acc.offset + acc.subOffset[3]];
					mesh.colors[input.set].push_back(color);
				}
				break;
			}
			case NormalInput:
			{
				mft::vec3	norm;
				norm.x = values[readIndex * acc.stride + acc.offset + acc.subOffset[0]];
				norm.y = values[readIndex * acc.stride + acc.offset + acc.subOffset[1]];
				norm.z = values[readIndex * acc.stride + acc.offset + acc.subOffset[2]];
				mesh.norm.push_back(norm);
				break;
			}
			default:
				break;
			}
		}
	}

	void	ColladaParser::ReadIndices(const lxml::Tag& indicesTag,
		std::vector<Input>& inputs, ColladaMesh& mesh)
	{
		SubMesh	subMesh;

		lxml::GetStrAttribute(indicesTag, "material", subMesh.material);

		lxml::GetUIntAttribute(indicesTag, "count", subMesh.nbFaces);

		std::vector<unsigned int> vcount;

		PrimitiveType type = P_Invalid;
		if (indicesTag.name == "lines")
			type = P_Lines;
		//	Not supporting polygons yet
		//else if (indicesTag.name == "polygons")
		//	type = P_Polygons;
		else if (indicesTag.name == "polylist")
			type = P_Polylist;
		else if (indicesTag.name == "triangles")
			type = P_Triangles;
		else
			throw ColladaException("Unsupported primitive format: " + indicesTag.name);

		for (const auto& child : indicesTag.children)
		{
			if (child.name == "input")
			{
				ReadInput(child, inputs);
			}
			else if (child.name == "vcount")
			{
				const char* content;
				char* next = nullptr;
				content = child.content.c_str();
				vcount.reserve(subMesh.nbFaces);
				for (unsigned int i = 0; i < subMesh.nbFaces; i++)
				{
					if (!*content)
						throw ColladaException("Not enough values in vcount");
					vcount.push_back(strtoul(content, &next, 10));
					content = next;
					content = lxml::SkipWhitespaces(content);

				}
			}
			else if (child.name == "p")
			{
				ReadPrimitives(child, mesh, inputs, (size_t)subMesh.nbFaces, vcount, type);
			}
		}
		mesh.subMeshes.push_back(subMesh);
	}


	void	ColladaParser::ReadVertices(const lxml::Tag& verticesTag,
		std::vector<Input>& inputs, ColladaMesh& mesh)
	{
		//	Save the id of this <vertices> tag
		//	to later make sure every "VERTEX" <input>
		//	correctly refers to it
		lxml::GetStrAttribute(verticesTag, "id", mesh.vertexId);
		for (const auto& child : verticesTag.children)
		{
			if (child.name == "input")
				ReadInput(child, inputs);
		}
	}

	void	ColladaParser::ReadMesh(const lxml::Tag& meshTag, ColladaMesh& mesh)
	{
		std::vector<Input> inputs;
		for (const auto& child : meshTag.children)
		{
			if (child.name == "source")
				ReadSource(child);
			else if (child.name == "vertices")
				ReadVertices(child, inputs, mesh);
			else if (child.name == "triangles" || child.name == "polylist"
				|| child.name == "lines")// || child.name == "polylist")
				ReadIndices(child, inputs, mesh);
		}
	}

	void	ColladaParser::ReadGeometry(const lxml::Tag& geometryTag, ColladaMesh& mesh)
	{
		for (const auto& child : geometryTag.children)
		{
			if (child.name == "mesh")
				ReadMesh(child, mesh);
		}
	}

	void	ColladaParser::ReadGeometries(const lxml::Tag& geometriesTag)
	{
		for (const auto& child : geometriesTag.children)
		{
			if (child.name == "geometry")
			{
				std::string id;
				lxml::GetStrAttribute(child, "id", id);

				if (this->meshes.find(id) == this->meshes.end())
				{
					ColladaMesh* mesh = new ColladaMesh();
					mesh->id = id;

					lxml::GetStrAttribute(child, "name", mesh->name);

					ReadGeometry(child, *mesh);
					this->meshes.insert({ id, mesh });
				}
			}
		}
	}

	void		ColladaParser::ReadImages(const lxml::Tag& imagesTag)
	{
		for (const auto& child : imagesTag.children)
		{
			if (child.name != "image")
				continue;
			ColladaImage image;
			lxml::GetStrAttribute(child, "id", image.id);
			lxml::GetStrAttribute(child, "name", image.name);
			for (const auto& imageChild : child.children)
			{
				if (imageChild.name == "init_from")
				{
					image.path = imageChild.content;
					ConvertColladaPath(image.path);
				}
				else
				{
					// Just ignore the rest
					//throw ColladaException("Unsupported image format:"
					//	+ imageChild.name);
				}
			}
			this->images[image.id] = image;
		}
	}

	void		ColladaParser::ReadMaterial(const lxml::Tag& materialTag,
		ColladaMaterial& mat)
	{
		for (const auto& child : materialTag.children)
		{
			if (child.name != "instance_effect")
				continue;
			lxml::GetStrAttribute(child, "url", mat.effectId);
			if (mat.effectId[0] != '#')
				throw ColladaException("Invalid instance effect url format: missing '#'");
			mat.effectId = mat.effectId.c_str() + 1;
			//	Don't resolve here on the fly but when binding materials
		}
	}

	void		ColladaParser::ReadMaterials(const lxml::Tag& materialsTag)
	{
		for (const auto& child : materialsTag.children)
		{
			if (child.name != "material")
				continue;
			ColladaMaterial mat;
			lxml::GetStrAttribute(child, "id", mat.id);
			lxml::GetStrAttribute(child, "name", mat.name);
			ReadMaterial(child, mat);
			this->materials[mat.id] = mat;
		}
	}

	void		ColladaParser::ReadEffectParam(const lxml::Tag& paramTag,
		EffectParam& param)
	{
		for (const auto& child : paramTag.children)
		{
			if (child.name == "surface")
			{
				for (const auto& surfaceChild : child.children)
				{
					if (surfaceChild.name != "init_from")
						continue;
					param.ref = surfaceChild.content;
				}
				param.type = SurfaceParam;
			}
			else if (child.name == "sampler2D")
			{
				for (const auto& samplerChild : child.children)
				{
					if (samplerChild.name != "source")
						continue;
					param.ref = samplerChild.content;
				}
				param.type = SamplerParam;
			}
		}
	}

	void		ColladaParser::ReadEffect(const lxml::Tag& effectTag,
		ColladaEffect& effect)
	{
		for (const auto& child : effectTag.children)
		{
			if (child.name != "profile_COMMON")
				continue;
			for (const auto& effectChild : child.children)
			{
				if (effectChild.name == "newparam")
				{
					std::string sid;
					lxml::GetStrAttribute(effectChild, "sid", sid);
					EffectParam	param;
					ReadEffectParam(effectChild, param);
					effect.params[sid] = param;
				}
				else if (effectChild.name == "technique")
				{

				}
			}
		}
	}

	void		ColladaParser::ReadEffects(const lxml::Tag& effectsTag)
	{
		for (const auto& child : effectsTag.children)
		{
			if (child.name != "effect")
				continue;
			std::string id;
			lxml::GetStrAttribute(child, "id", id);
			ColladaEffect	effect;
			ReadEffect(child, effect);
			this->effects[id] = effect;
		}
	}

	void		ColladaParser::ReadScene(const lxml::Tag& sceneTag)
	{
		for (const auto& child : sceneTag.children)
		{
			if (child.name == "instance_visual_scene")
			{
				if (this->rootNode)
				{
					throw ColladaException("More than one root scene");
				}
				std::string url;
				lxml::GetStrAttribute(child, "url", url);
				if (url[0] != '#')
				{
					throw ColladaException("Invalid scene url format, missing '#'");
				}
				const char* id = url.c_str() + 1;
				if (this->nodes.find(id) == this->nodes.end())
				{
					throw ColladaException("Unknown scene url: " + std::string(id));
				}
				this->rootNode = &this->nodes[id];
			}
		}
	}

	void		ColladaParser::ReadTransform(TransformType type, const lxml::Tag& nodeTag, ColladaNode& node)
	{
		mft::mat4	transform;

		//	Prepare to read up to 16 values
		float	values[16] = { 0.0f };

		const char* str = nodeTag.content.c_str();
		char* next = nullptr;
		for (unsigned short i = 0; i < 16; i++)
		{
			values[i] = std::strtof(str, &next);
			str = next;
		}

		switch (type)
		{
		case LookAtTranform:
		{
			mft::vec3	pos(values[0], values[1], values[2]);
			mft::vec3	target(values[3], values[4], values[5]);
			mft::vec3	up(values[6], values[7], values[8]);
			transform = mft::mat4::lookAt(pos, target, up);
			break;
		}
		case RotateTransform:
		{
			mft::vec3	axis(values[0], values[1], values[2]);
			float		angle = values[3];
			transform = mft::mat4::rotate(angle, axis);
			break;
		}
		case TranslateTransform:
		{
			mft::vec3	trans(values[0], values[1], values[2]);
			transform = mft::mat4::translate(trans);
			break;
		}
		case ScaleTransform:
		{
			mft::vec3	scale(values[0], values[1], values[2]);
			transform = mft::mat4::scale(scale);
			break;
		}
		case SkewTransform:
		{
			break;
		}
		case MatrixTransform:
		{
			transform = mft::mat4(
				{ values[0], values[1], values[2], values[3] },
				{ values[4], values[5], values[6], values[7] },
				{ values[8], values[9], values[10], values[11] },
				{ values[12], values[13], values[14], values[15] } );
			break;
		}
		}

		node.transforms.push_back(transform);
	}

	void		ColladaParser::ReadMaterialInstance(const lxml::Tag& instanceTag,
		MaterialInstance& instance)
	{
		for (const auto& child : instanceTag.children)
		{
			if (child.name != "bind_vertex_input")
				continue;
			std::string semantic;
			lxml::GetStrAttribute(child, "semantic", semantic);

			std::string input_semantic;
			lxml::GetStrAttribute(child, "input_semantic", input_semantic);

			InputType type = InvalidInput;
			if (input_semantic == "TEXCOORD")
				type = TexCoordInput;
			else if (input_semantic == "COLOR")
				type = ColorInput;
			else if (input_semantic == "BINORMAL" || input_semantic == "TEXBINORMAL")
				type = BitangentInput;
			else if (input_semantic == "TANGENT" || input_semantic == "TEXTANGENT")
				type = TangentInput;
			if (type == InvalidInput)
			{
				std::cerr << "Invalid instance material semantic: " << input_semantic << std::endl;
				//Can ignore instead of exiting
				//throw std::exception();
			}
			unsigned int input_set = 0;
			lxml::GetUIntAttribute(child, "input_set", input_set);
			MaterialInstanceSemantic tex = MaterialInstanceSemantic();
			tex.set = input_set;
			tex.type = type;

			instance.bindings[semantic] = tex;
		}
	}

	void		ColladaParser::ReadNodeGeometry(const lxml::Tag& geometryTag,
		ColladaNode& node)
	{
		std::string	url;
		lxml::GetStrAttribute(geometryTag, "url", url);
		if (url[0] != '#')
			throw ColladaException("Invalid geometry instance url, missing '#'");
		//	Don't resolve here on the fly, wait until we've read all the data

		ColladaInstance	instance;
		instance.id = url.c_str() + 1;

		for (const auto& child : geometryTag.children)
		{
			if (child.name != "bind_material")
				continue;
			for (const auto& bindChild : child.children)
			{
				if (bindChild.name != "technique_common")
					continue;
				for (const auto& techChild : bindChild.children)
				{
					if (techChild.name != "instance_material")
						continue;
					std::string symbol;
					lxml::GetStrAttribute(techChild, "symbol", symbol);
					std::string target;
					lxml::GetStrAttribute(techChild, "target", target);
					if (target[0] != '#')
					{
						throw ColladaException("Invalid material target, missing '#'");
					}
					MaterialInstance	binding;
					binding.name = target.c_str() + 1;
					//	Don't resolve here on the fly, wait until we've read all the data
					ReadMaterialInstance(techChild, binding);
					instance.materials[symbol] = binding;
				}
			}
		}
		node.meshes.push_back(instance);
	}

	void		ColladaParser::ReadNode(const lxml::Tag& nodeTag, ColladaNode& node)
	{
		for (const auto& child : nodeTag.children)
		{
			if (child.name == "node")
			{
				ColladaNode newNode;
				lxml::GetStrAttribute(child, "id", newNode.id);
				lxml::GetStrAttribute(child, "sid", newNode.sid);
				lxml::GetStrAttribute(child, "name", newNode.name);

				newNode.parent = &node;
				ReadNode(child, newNode);
				node.children.push_back(newNode);
				this->nodes[newNode.id] = newNode;
			}
			else if (child.name == "lookat")
			{
				ReadTransform(LookAtTranform, child, node);
			}
			else if (child.name == "rotate")
			{
				ReadTransform(RotateTransform, child, node);
			}
			else if (child.name == "translate")
			{
				ReadTransform(TranslateTransform, child, node);
			}
			else if (child.name == "scale")
			{
				ReadTransform(ScaleTransform, child, node);
			}
			else if (child.name == "skew")
			{
				ReadTransform(SkewTransform, child, node);
			}
			else if (child.name == "matrix")
			{
				ReadTransform(MatrixTransform, child, node);
			}
			else if (child.name == "render")
			{

			}
			else if (child.name == "instance_geometry"
				|| child.name == "instance_controller")
			{
				ReadNodeGeometry(child, node);
			}
			else if (child.name == "instance_node")
			{
				std::string url;
				lxml::GetStrAttribute(child, "url", url);
				if (url[0] != '#')
				{
					throw ColladaException("Invalid instance_node format: missing '#'");
				}
				node.nodeInstances.push_back(url.c_str() + 1);
			}
			//	The following is not supported yet
			else if (child.name == "instance_light")
			{

			}
			else if (child.name == "instance_camera")
			{

			}
		}
	}

	void		ColladaParser::ReadScenes(const lxml::Tag& scenesTag)
	{
		for (const auto& child : scenesTag.children)
		{
			if (child.name == "visual_scene")
			{
				std::string id;
				lxml::GetStrAttribute(child, "id", id);
				std::string name;
				lxml::GetStrAttribute(child, "name", name);

				ColladaNode	node;
				node.id = id;
				node.name = name;
				ReadNode(child, node);
				this->nodes[id] = node;
			}
		}
	}

	void		ColladaParser::ReadAnimSampler(const lxml::Tag& samplerTag,
		AnimationChannel& channel)
	{
		for (const auto& child : samplerTag.children)
		{
			if (child.name != "input")
				continue;
			std::string semantic;
			std::string source;
			lxml::GetStrAttribute(child, "semantic", semantic);
			lxml::GetStrAttribute(child, "source", source);
			if (source[0] != '#')
				throw ColladaException("Invalid animation sampler source format: missing '#'");
			source = source.c_str() + 1;
			if (semantic == "INPUT")
				channel.timesSource = source;
			else if (semantic == "OUTPUT")
				channel.valuesSource = source;
			else if (semantic == "INTERPOLATION")
				channel.interpolationSource = source;
			else
			{
				std::cerr << "Animation sampler type not handled for ";
				std::cerr << channel.id << " sampler: " << source << std::endl;
			}
		}
	}

	void		ColladaParser::ReadAnimation(const lxml::Tag& animTag,
		ColladaAnimation& anim)
	{
		lxml::GetStrAttribute(animTag, "id", anim.id);
		lxml::GetStrAttribute(animTag, "name", anim.name);
		if (anim.name.empty())
			anim.name == anim.id;
		for (const auto& child : animTag.children)
		{
			if (child.name == "source")
				ReadSource(child);
			else if (child.name == "sampler")
			{
				AnimationChannel channel;
				lxml::GetStrAttribute(animTag, "id", channel.id);
				ReadAnimSampler(child, channel);
				anim.channels.push_back(channel);
			}
			else if (child.name == "channel")
			{
				std::string source;
				std::string	target;
				lxml::GetStrAttribute(child, "source", source);
				if (source[0] != '#')
					throw ColladaException("Invalid animation channel source format: missing '#'");
				source = source.c_str() + 1;
				for (auto& channel: anim.channels)
				{
					if (channel.id == source)
					{
						lxml::GetStrAttribute(child, "target", channel.target);
					}
				}
			}
			else if (child.name == "animation")
			{
				ColladaAnimation newAnim;
				ReadAnimation(child, newAnim);
				anim.children.push_back(newAnim);
			}
		}
	}

	void		ColladaParser::ReadAnimations(const lxml::Tag& animationsTag)
	{
		for (const auto& animTag : animationsTag.children)
		{
			if (animTag.name != "animation")
				continue;
			ColladaAnimation newAnim;
			ReadAnimation(animTag, newAnim);
			this->animations[newAnim.id] = newAnim;
		}
	}

	void		ColladaParser::ReadJoint(const lxml::Tag& jointTag,
		ColladaController& controller)
	{
		for (const auto& child: jointTag.children)
		{
			if (child.name != "input")
				continue;

			std::string semantic;
			lxml::GetStrAttribute(child, "semantic", semantic);

			std::string source;
			lxml::GetStrAttribute(child, "source", source);
			if (source[0] != '#')
			{
				throw ColladaException("Invalid joint format: missing '#'");
			}

			if (semantic == "JOINT")
			{
				controller.boneSource = source.c_str() + 1;
			}
			else if (semantic == "INV_BIND_MATRIX")
			{
				controller.boneOffsetMatrixSource = source.c_str() + 1;
			}
		}
	}

	void		ColladaParser::ReadVertexWeight(const lxml::Tag& vertexWeightTag,
		ColladaController& controller)
	{
		unsigned int vertexCount;
		lxml::GetUIntAttribute(vertexWeightTag, "count", vertexCount);
		for (const auto& child: vertexWeightTag.children)
		{
			if (child.name == "input")
			{
				std::string semantic;
				lxml::GetStrAttribute(child, "semantic", semantic);

				std::string source;
				lxml::GetStrAttribute(child, "source", source);
				if (source[0] != '#')
				{
					throw ColladaException("Invalid vertex weight format: missing '#'");
				}

				Input input;
				unsigned int offset;
				lxml::GetUIntAttribute(child, "offset", offset);
				input.offset = static_cast<size_t>(offset);
				input.id = source.c_str() + 1;
				if (semantic == "JOINT")
					controller.boneInput = input;
				else if (semantic == "WEIGHT")
					controller.weightInput = input;

			}
			else if (child.name == "vcount")
			{
				controller.weightCounts.resize(vertexCount);
				const char* content = child.content.c_str();
				char *next = nullptr;
				size_t index = 0;
				size_t totalWeights = 0;
				while (*content)
				{
					if (index >= vertexCount)
						throw ColladaException("Too much values in vertex weight vcount ("
						+ std::to_string(index) + " / " + std::to_string(vertexCount) + ")");
					unsigned long value = strtoul(content, &next, 10);
					controller.weightCounts[index++] = static_cast<size_t>(value);
					totalWeights += value;
					content = next;
					content = lxml::SkipWhitespaces(content);
				}
				if (index != vertexCount)
					throw ColladaException("Not enough values in ertex weight vcount ("
					+ std::to_string(index) + " / " + std::to_string(vertexCount) + ")");
				controller.weights.resize(totalWeights);
			}
			else if (child.name == "v")
			{
				const char* content = child.content.c_str();
				char* next = nullptr;
				size_t index = 0;
				while (*content)
				{
					if (index >= controller.weights.size())
						throw ColladaException("Too much values in vertex weight v's ("
						+ std::to_string(index) + " / " + std::to_string(vertexCount) + ")");
					std::pair<size_t, size_t> pair;
					unsigned long value = strtoul(content, &next, 10);
					content = next;
					content = lxml::SkipWhitespaces(content);
					pair.first = static_cast<size_t>(value);
					if (!*content)
						throw ColladaException("Not enough values in vertex weight v's ("
						+ std::to_string(index) + " / " + std::to_string(vertexCount) + ")");
					value = strtoul(content, &next, 10);
					content = next;
					content = lxml::SkipWhitespaces(content);
					pair.second = static_cast<size_t>(value);
					controller.weights[index++] = pair;
				}
			}
		}
	}

	void		ColladaParser::ReadSkin(const lxml::Tag& skinTag,
		ColladaController& controller)
	{
		for (const auto& child: skinTag.children)
		{
			if (child.name == "joints")
			{
				ReadJoint(child, controller);
			}
			else if (child.name == "vertex_weights")
			{
				ReadVertexWeight(child, controller);
			}
			else if (child.name == "source")
			{
				ReadSource(child);
			}
			else if (child.name == "bind_shape_matrix")
			{
				const char* content = child.content.c_str();
				char* next = nullptr;
				for (unsigned int i = 0; i < 4; i++)
				{
					for (unsigned int j = 0; j < 4; j++)
					{
						if (!*content)
							throw ColladaException("Not enough values in bind shape matrix");
						controller.bindShapeMatrix[i][j] = strtof(content, &next);
						content = next;
					}
				}
				if (*content)
					throw ColladaException("Too much values in bind shape matrix");
			}
		}
	}

	void		ColladaParser::ReadController(const lxml::Tag& controllerTag,
		ColladaController& controller)
	{
		for (const auto& child: controllerTag.children)
		{
			if (child.name == "skin")
			{
				controller.type = ControllerType::Skin;
				lxml::GetStrAttribute(child, "source", controller.meshId);
				controller.meshId = controller.meshId.c_str() + 1;

				ReadSkin(child, controller);
			}
		}
	}

	void		ColladaParser::ReadControllers(const lxml::Tag& controllersTag)
	{
		for (const auto& controller : controllersTag.children)
		{
			if (controller.name != "controller")
				continue;
				std::string id;
				lxml::GetStrAttribute(controller, "id", id);
				lxml::GetStrAttribute(controller, "name", this->controllers[id].name);
				ReadController(controller, this->controllers[id]);
		}
	}

	void		ColladaParser::ReadAsset(const lxml::Tag& assetTag)
	{
		for (const auto& child : assetTag.children)
		{
			if (child.name == "up_axis")
			{
				if (child.content == "X_UP")
					this->axis = Z_UP;
				else if (child.content == "Y_UP")
					this->axis = Y_UP;
				if (child.content == "Z_UP")
					this->axis = Z_UP;
			}
		}
	}

	void		ColladaParser::ReadStructure(const lxml::Importer& parser)
	{
		for (const auto& tag : parser.RootTag.children)
		{
			if (tag.name != "COLLADA")
				continue;
			for (const auto& child : tag.children)
			{
				if (child.name == "library_geometries")
					ReadGeometries(child);
				if (child.name == "library_images")
					ReadImages(child);
				if (child.name == "library_materials")
					ReadMaterials(child);
				if (child.name == "library_effects")
					ReadEffects(child);
				if (child.name == "library_controllers")
					ReadControllers(child);
				if (child.name == "library_animations")
					ReadAnimations(child);
				if (child.name == "library_visual_scenes")
					ReadScenes(child);
				if (child.name == "scene")
					ReadScene(child);
				if (child.name == "asset")
					ReadAsset(child);
			}
		}
	}

	void	ColladaParser::ReadFile(const std::string& path, unsigned int flags)
	{
		lxml::Importer	xmlParser;
		xmlParser.ReadFile(path);

		if (xmlParser.RootTag.valid == false)
			return ;

		try
		{
			ReadStructure(xmlParser);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return ;
		}
	}

	std::ostream& operator<<(std::ostream& o, ColladaParser::InputType& type) {
		switch (type)
		{
		case ColladaParser::InvalidInput:
			std::cout << "Invalid input";
			break;
		case ColladaParser::VertexInput:
			std::cout << "Vertex input";
			break;
		case ColladaParser::PositionInput:
			std::cout << "Position input";
			break;
		case ColladaParser::TexCoordInput:
			std::cout << "Texture coordinate input";
			break;
		case ColladaParser::NormalInput:
			std::cout << "Normal input";
			break;
		case ColladaParser::ColorInput:
			std::cout << "Color input";
			break;
			//	The following is ignores at the moment
		case ColladaParser::TangentInput:
			std::cout << "Tangent input";
			break;
		case ColladaParser::BitangentInput:
			std::cout << "Bitangent input";
			break;
		}
		return o;
	}

	void	ColladaParser::ConvertColladaPath(std::string& path)
	{
		char* out = path.data();
		size_t fromIndex = 0, outIndex = 0, size = path.size();
		while (fromIndex < size)
		{
			if (path[fromIndex] == '%' && fromIndex + 3 < size)
			{
				char nbStr[3] = { path[fromIndex + 1], path[fromIndex + 2], '\0' };
				unsigned long nb = strtoul(nbStr, 0, 16);
				if (nb >= 128)
					throw ColladaException("Invalid special % char value in path: "
						+ std::to_string(nb));
				out[outIndex] = static_cast<char>(nb);
				fromIndex += 3;
			}
			else
			{
				out[outIndex] = path[fromIndex];
				fromIndex++;
			}
			outIndex++;
		}
		out[outIndex] = '\0';
	}
}
