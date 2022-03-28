#ifndef _COLLADA_PARSER_CLASS_H_
#define	_COLLADA_PARSER_CLASS_H_

#include "LXML/LXML.class.hpp"
#include "ColladaParser/ColladaException.class.hpp"
#include "mft/mft.hpp"
#include <string>

namespace notrealengine
{

	class ColladaParser
	{

	public:

		//	Structures to store all the string data from the XML as we want

		enum PrimitiveType
		{
			P_Invalid,
			P_Lines,
			P_Triangles,
			P_Polygons,
			P_Polylist
		};
		enum Axis
		{
			X_UP,
			Y_UP,
			Z_UP
		};
		enum InputType
		{
			InvalidInput,
			VertexInput,
			PositionInput,
			TexCoordInput,
			NormalInput,
			ColorInput,
			//	The following is not supported at the moment
			TangentInput,
			BitangentInput
		};
		enum TransformType
		{
			LookAtTranform,
			RotateTransform,
			TranslateTransform,
			ScaleTransform,
			SkewTransform,
			MatrixTransform
		};
		enum ControllerType
		{
	    Skin,
		//	The following is not supported at the moment
	    Morph
		};

		/**	Types of effects
		**	At the moment only surface (i.e. texture)
		**	and sampler2D (i.e. ref to a surface) are supported
		*/
		enum ParamType
		{
			SurfaceParam,
			SamplerParam
		};

		/**	A parameter for an effect */
		struct EffectParam
		{
			std::string ref;
			ParamType	type;
		};

		/**	I.E.Material definition
		**	The actual material parameters definition
		*/
		struct ColladaEffect
		{
			std::map<std::string, EffectParam> params;
		};

		/**	Name/path of an image used for materials
		**	Can also be embedded data but it is not supported here
		*/
		struct ColladaImage
		{
			std::string id;
			std::string	name;
			std::string	path;
		};

		/**	Material declaration only.
		**	Refers to an Effect
		**	where is the definition
		*/
		struct ColladaMaterial
		{
			std::string name;
			std::string id;
			std::string effectId;
		};

		struct ColladaAccessor
		{
			//	How many floats are there
			size_t	count;
			//	Sometimes accessors have an offset
			//	but I still don't know what for
			size_t	offset;
			//	The stride between two consecutive datas
			size_t	stride;
			//	The array of what data we need to read
			//	For exemple, for a vertex array:
			//	params[0] = "X", params[1] = "Y", params[2] = "Z"
			std::vector<std::string> params;
			//	The offset of each param. Maximum 4 params;
			size_t	subOffset[4];
			//	The id of the array it is refering to
			std::string sourceId;
			//	A pointer to the actual float/string array
			//	We resolve it once we read all the data,
			//	just before reading actual vertex indices
			mutable const std::vector<float>* floatData;
			mutable const std::vector<float>* stringData;

			ColladaAccessor() :
				count(0), offset(0), stride(0),
				params(), subOffset{ 0, 0, 0, 0 }, sourceId(),
				floatData(nullptr), stringData(nullptr)
			{
			}
		};
		struct Input
		{
			InputType type;
			//	<input> tags refer to a <source>
			//	or to the <vertex> tag
			std::string id;
			//	Offset in the index list
			size_t	offset;
			//	For textures
			size_t	set;
			//	The "source" attribute of <input>
			//	is an id refering to an <accessor>
			//	telling us how to read the data
			//	We resolve the pointer once we read all the data,
			//	just before reading actual vertex indices
			//	-- Set as mutable to enable writing when resolving references
			mutable const ColladaAccessor* accessor;

			Input() :
				type(InvalidInput), id(), offset(0), set(0), accessor(nullptr)
			{
			}
		};
		/**	Sometimes more than one <triangles>/<polyist>/etc
		**	tags may be found for one mesh
		**	This means that this mesh is divided in multiple submeshes
		**	The struct keeps tracks of the size and materials of each
		*/
		struct SubMesh
		{
			std::string	material;
			unsigned int nbFaces;
		};
		struct AnimationChannel
		{
			std::string id;
			std::string target;

			std::string	valuesSource;
			std::string	timesSource;
			std::string interpolationSource;
		};
		struct ColladaAnimation
		{
			std::string id;
			std::string name;
			std::vector<AnimationChannel>	channels;
			std::vector<ColladaAnimation>	children;
		};
		struct	ColladaController
		{
			ControllerType	type;

			std::string	meshId;
			std::string	name;
			std::string boneSource;
			std::string boneOffsetMatrixSource;
			Input				boneInput;
			Input				weightInput;
			mft::mat4		bindShapeMatrix;
			std::vector<size_t>	weightCounts;
			std::vector<std::pair<size_t, size_t>>	weights;
		};
		/**	All the data we can find about a <mesh> in the file
		*/
		struct ColladaMesh
		{
			std::string	id;
			std::string	name;

			//	Save the <vertex> tag id to
			//	make sure every "VERTEX" <input> correctly refers to it
			//	before reading vertex data
			std::string	vertexId;

			std::vector<mft::vec3>	pos;
			std::vector<mft::vec3>	norm;
#define	MAX_TEXTURE_CHANNELS 4
			std::vector<mft::vec3>	tex[MAX_TEXTURE_CHANNELS];
			std::vector<mft::vec4>	colors[MAX_TEXTURE_CHANNELS];

			unsigned int nbUVComponents[MAX_TEXTURE_CHANNELS];
			//	Save the size of each primitive/face to build the mesh later
			std::vector<size_t> faceSizes;
			std::vector<size_t> posIndices;
			//	Save each vertex's index in the source array
			//	to assign bone weights

			//	Meshes are decomposed in submeshes
			//	one for each <triangle>/<polylist>/<lines> tag
			std::vector<SubMesh>	subMeshes;
			ColladaMesh()
			{
				for (unsigned int i = 0; i < MAX_TEXTURE_CHANNELS; i++)
				{
					// Only U and V by default
					nbUVComponents[i] = 2;
				}
			}
		};

		//	Material instances given in <node>s from <library_visual_scenes>
		//	have an associated semantic giving their type (texture, color, etc)
		//	and the associated channel (from 0 to MAX_TEXTURE_CHANNELS)
		struct MaterialInstanceSemantic
		{
			unsigned int	set;
			InputType		type;
			MaterialInstanceSemantic() : set(0), type(InvalidInput)
			{
			}
		};

		//	Data used to bind materials to geometry
		//	in nodes from <library_visual_scenes>
		struct MaterialInstance
		{
			std::string		name;
			std::map<std::string, MaterialInstanceSemantic> bindings;
		};

		//	A mesh or a skeleton/controller refered to by a node in the scene hierarchy
		//	and its materials
		struct ColladaInstance
		{
			std::string id;
			std::map<std::string, MaterialInstance> materials;
		};

		struct ColladaTransform
		{
			std::string id;
			TransformType type;
			mft::mat4		matrix;
		};

		//	A node of the scene hierarchy
		//	Can contain transforms, bones, meshes, cameras, lights
		struct ColladaNode
		{
			std::string	id;
			std::string	sid;
			std::string	name;
			//	When building the final cpNode's hierarchy, we'll have
			//	to choose a final name (id if there is one,
			//	otherwise sid, otherwise name)
			//	We will also have to solve bones references with ColladNodes
			//	and set their name's to the final one, so we save it here
			std::string finalName;

			std::vector<ColladaTransform>	transforms;

			std::vector<ColladaInstance>	meshes;

			std::vector<std::string>	nodeInstances;

			ColladaNode* parent;
			std::vector<ColladaNode>	children;
		};

		ColladaParser();
		~ColladaParser();

		void
			ReadFile(const std::string& path, unsigned int flags);

		ColladaNode* rootNode;

		std::map<std::string, ColladaMesh*>	meshes;
		std::map<std::string, ColladaController> controllers;
		std::map<std::string, ColladaAccessor>		accessors;
		std::map<std::string, std::vector<float>> floats;
		std::map<std::string, std::vector<std::string>> strings;
		std::map<std::string, ColladaNode>		nodes;
		std::map<std::string, ColladaImage>		images;
		std::map<std::string, ColladaMaterial>	materials;
		std::map<std::string, ColladaEffect>	effects;
		std::map<std::string, ColladaAnimation> animations;

		Axis	axis;

	private:

		/**	Read all the structure of the xml data
		*/
		void
			ReadStructure(const lxml::Importer& parser);

		/**	Retrieve controllers, i.e. skeletons/bones from the file
		*/
		void
			ReadControllers(const lxml::Tag& controllersTag);

		/**	Retrieve one controller
		*/
		void
			ReadController(const lxml::Tag& controllerTag, ColladaController& controller);

		/**	Retrieve a skin in a controller
		*/
		void
			ReadSkin(const lxml::Tag& skinTag, ColladaController& controller);

		/**	Retrieve a <joint> (we will call it "bone") in a skin
		*/
		void
			ReadJoint(const lxml::Tag& jointTag, ColladaController& controller);

		/**	Retrieve a vertex weight in a skin
		*/
		void
			ReadVertexWeight(const lxml::Tag& vertexWeightTag, ColladaController& controller);

		/**	Retrieve an animation sampler  from the file
		*/
		void
			ReadAnimSampler(const lxml::Tag& samplerTag, AnimationChannel& channel);

		/**	Retrieve an animation from the file
		*/
		void
			ReadAnimations(const lxml::Tag& animationsTag);

		/**	Retrieve animations from the file
		*/
		void
			ReadAnimation(const lxml::Tag& animationTag, ColladaAnimation& anim);

		/**	Retrieve images from the file
		*/
		void
			ReadImages(const lxml::Tag& imagesTag);

		/**	Retrieve materials from the file
		*/
		void
			ReadMaterials(const lxml::Tag& materialsTag);

		/**	Retrieve one material from the file
		*/
		void
			ReadMaterial(const lxml::Tag& materialTag,
				ColladaMaterial& mat);

		/**	Retrieve effects from the file
		*/
		void
			ReadEffects(const lxml::Tag& effectsTag);

		/**	Retrieve effect parameter from the file
		*/
		void
			ReadEffectParam(const lxml::Tag& paramTag, EffectParam& param);

		/**	Retrieve an effect from the file
		*/
		void
			ReadEffect(const lxml::Tag& effectTag, ColladaEffect& effect);

		/**	Retrieve the asset info from the file
		*/
		void
			ReadAsset(const lxml::Tag& assetTag);

		/**	Retrieve the root node from the file
		*/
		void
			ReadScene(const lxml::Tag& sceneTag);

		/**	Retrieve scenes from the file
		*/
		void
			ReadScenes(const lxml::Tag& scenesTag);

		/**	Retrieve a node from the file
		*/
		void
			ReadNode(const lxml::Tag& nodeTag, ColladaNode& node);

		/**	Retrieve a transform from a node
		*/
		void
			ReadTransform(TransformType type, const lxml::Tag& nodeTag, ColladaNode& node);

		/**	Retrieve a geometry node from the file
		*/
		void
			ReadNodeGeometry(const lxml::Tag& geometryTag, ColladaNode& node);

		/**	Retrieve an <instance_material> from the file
		*/
		void
			ReadMaterialInstance(const lxml::Tag& instanceTag, MaterialInstance& matInstance);

		/**	Retrieve scene geometry from the file
		*/
		void
			ReadGeometries(const lxml::Tag& geometriesTag);

		/**	Retrieve scene geometry from the file
		*/
		void
			ReadGeometry(const lxml::Tag& geometryTag, ColladaMesh& mesh);

		/**	Read mesh data
		*/
		void
			ReadMesh(const lxml::Tag& meshTag, ColladaMesh& mesh);

		/**	Read source tag data
		*/
		void
			ReadSource(const lxml::Tag& sourceTag);

		/**	Read array tag data
		*/
		void
			ReadArray(const lxml::Tag& arrayTag);

		/**	Read accessor tag data
		*/
		void
			ReadAccessor(const lxml::Tag& accessorTag, const std::string& id);

		/**	Read array tag data
		*/
		void
			ReadVertices(const lxml::Tag& verticesTag,
				std::vector<Input>& inputs, ColladaMesh& mesh);

		/**	Read indices tag data
		*/
		void
			ReadIndices(const lxml::Tag& indicesTag,
				std::vector<Input>& inputs, ColladaMesh& mesh);

		/** Read input tag data
		*/
		void
			ReadInput(const lxml::Tag& inputTag, std::vector<Input>& inputs);

		/** Read primitives tag data
		*/
		void
			ReadPrimitives(const lxml::Tag& primTag, ColladaMesh& mesh,
				std::vector<Input>& inputs, size_t count,
				std::vector<unsigned int>& vcount, PrimitiveType type);

		/**	Read one vertex from <p> indices
		*/
		void
			ReadVertex(size_t index, ColladaMesh& mesh,
				std::vector<unsigned int>& indices, std::vector<Input>& inputs);

		/**	Try to resolve references for inputs and their accessors
		**	i.e. check if the source id links to an existing array of data
		*/
		void
			ResolveInputsReferences(std::vector<Input>& inputs,
				ColladaMesh& mesh);

		/**	Read a path from the file and convert it to an
		**	actual usable path (convert %x characters, etc)
		**	TODO: handle weird path beginnings: file:///, etc
		*/
		void
			ConvertColladaPath(std::string& path);

	};

	/**	Search for an entry in a given map
	**	and throw if not found
	**	Use it when resolving a reference is critical
	**	and the reference will not be modified
	*/
	template < typename T >
	const T&
		ResolveReference(const std::map<std::string, T>& srcMap, const std::string& str,
			const std::string& debugStr)
	{
		const typename std::map<std::string, T>::const_iterator it =
			srcMap.find(str);
		if (it == srcMap.end())
			throw ColladaException("Unknown reference for " + debugStr + ": " + str);
		return it->second;
	}

	std::ostream& operator<<(std::ostream& o, ColladaParser::InputType& type);

	/**	Find a node by name in the tree
	*/
	const ColladaParser::ColladaNode*
		FindNode(const ColladaParser::ColladaNode* node, const std::string& name);

}

#endif // !_COLLADA_PARSER_CLASS_H_
