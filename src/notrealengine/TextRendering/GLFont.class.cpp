#include "TextRendering/GLFont.class.hpp"
#include "TextRendering/Freetype.class.hpp"
#include "GL.hpp"
#include "CheckFileType.hpp"
#include "GLContext.class.hpp"

#include <fstream>
#include <string.h>

namespace notrealengine
{
	GLFont::GLFont(const std::string& path):
		Asset({path}), VAO(0), VBO(0),
#ifdef USING_EXTERNAL_LIBS
		characters(),
		shader(GLContext::getShader("text"))
#else
		glId(0), imgSize(mft::vec2i()), cellSize(mft::vec2i()),
		factor(mft::vec2()), widths(), firstChar(0),
		shader(GLContext::getShader("2d"))
#endif
	{

		std::cout << "Loading font '" << path << "'";

		if (!IsReg(path))
		{
			std::cerr << std::endl << "nre: Invalid font file type" << std::endl;
			this->loaded = false;
			return ;
		}

#ifdef USING_EXTERNAL_LIBS

		std::cout << " with freetype..." << std::endl;
		Freetype::Init();

		FT_Library const& ft = Freetype::getFT();
		FT_Face	face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			std::cerr << std::endl << "Could not init font " << path << std::endl;
			this->loaded = false;
			return ;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::cerr << std::endl << "Could not load Glyph" << std::endl;
			this->loaded = false;
			return;
		}

		GLCallThrow(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			characters.insert(std::make_pair(c, new GLCharacter(face, c)));
		}

		FT_Done_Face(face);

		GLCallThrow(glGenVertexArrays, 1, &VAO);
		GLCallThrow(glGenBuffers, 1, &VBO);
		GLCallThrow(glBindVertexArray, VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer, 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);
		GLCallThrow(glBindVertexArray, 0);

#else

		std::cout << " with custom parser..." << std::endl;
		LoadBFF(path);

#endif

	}

	GLFont::GLFont(GLFont&& ref) noexcept
		: Asset(std::move(ref)),
		VAO(std::exchange(ref.VAO, 0)),
		VBO(std::exchange(ref.VBO, 0)),
		shader(std::exchange(ref.shader, nullptr))
#ifdef USING_EXTERNAL_LIBS
		, characters(std::move(ref.characters))
#else
		, glId(std::exchange(ref.glId, 0)), firstChar(std::exchange(ref.firstChar, 0)),
		 imgSize(std::move(ref.imgSize)), cellSize(std::move(ref.cellSize)),
		 factor(std::move(ref.factor))
#endif
	{
#ifndef USING_EXTERNAL_LIBS
		std::move(std::begin(ref.widths), std::end(ref.widths), &this->widths[0]);
#endif
	}

	GLFont::~GLFont()
	{
		GLCallThrow(glDeleteVertexArrays, 1, &VAO);
		GLCallThrow(glDeleteBuffers, 1, &VBO);
#ifdef USING_EXTERNAL_LIBS
		for (auto& c: characters)
		{
			delete c.second;
		}
#else
		GLCallThrow(glDeleteBuffers, 1, &glId);
#endif
	}

	GLFont& GLFont::operator=(GLFont&& font) noexcept
	{
		Asset::operator=(std::move(font));
		this->VAO = std::exchange(font.VAO, 0);
		this->VBO = std::exchange(font.VBO, 0);
#ifdef USING_EXTERNAL_LIBS
		this->characters = std::move(font.characters);
#else
		this->glId = std::exchange(font.glId, 0);
		this->firstChar = std::exchange(font.firstChar, 0);
		this->imgSize = std::move(font.imgSize);
		this->cellSize = std::move(font.cellSize);
		this->factor = std::move(font.factor);
		std::move(std::begin(font.widths), std::end(font.widths), &this->widths[0]);
#endif
		return *this;
	}

#ifndef USING_EXTERNAL_LIBS
	void GLFont::LoadBFF(const std::string& path)
	{
		std::ifstream file;
		file.open(path, std::ios_base::binary | std::ios_base::in);

		if (file.fail())
		{
			std::cerr << std::endl << "nre: Unable to open file \"" << path << "\"" << std::endl;
			this->loaded = false;
			return;
		}

		file.seekg(0, std::ios_base::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios_base::beg);
		char* str = new char [size];
		file.read(str, size);

		if (file.fail())
		{
			std::cerr << std::endl << "nre: Unable to read file \"" << path << "\"" << std::endl;
			this->loaded = false;
			file.close();
			delete[] str;
			return;
		}

		file.close();

		if (size < 20)
		{
			std::cerr << "Bitmap font size is too low (" << size << ")" << std::endl;
			this->loaded = false;
			delete[] str;
			return ;
		}

		memcpy(&this->imgSize.x, &str[2], sizeof(int));
		memcpy(&this->imgSize.y, &str[6], sizeof(int));
		memcpy(&this->cellSize.x, &str[10], sizeof(int));
		memcpy(&this->cellSize.y, &str[14], sizeof(int));
		int bpp = str[18];
		this->firstChar = str[19];

		if (size != MAP_DATA_OFFSET + imgSize.x * imgSize.y * bpp / 8)
		{
			std::cerr << "Invalid bitmap font file size " << std::endl;
			std::cerr << "Expected file size = ";
			std ::cerr << MAP_DATA_OFFSET + imgSize.x * imgSize.y * bpp / 8 << std::endl;
			std::cerr << "Actual file sisze = " << size << std::endl;
			this->loaded = false;
			delete[] str;
			return ;
		}

		this->charsPerLine = this->imgSize.x / static_cast<float>(this->cellSize.x);
		if (this->charsPerLine == 0 || this->cellSize.x == 0)
		{
			std::cerr << "Invalid font size" << std::endl;
			this->loaded = false;
			delete[] str;
			return;
		}
		//	x0.5 because of the format GL_LUMINANCE_ALPHA
		this->factor.x = this->cellSize.x / static_cast<float>(this->imgSize.x);// *0.5f;
		this->factor.y = this->cellSize.y / static_cast<float>(this->imgSize.y);// *0.5f;

		memcpy(this->widths, &str[WIDTH_DATA_OFFSET], 256);
		unsigned char* img = new unsigned char [this->imgSize.x * this->imgSize.y * (bpp / 8)];
		memcpy(img, &str[MAP_DATA_OFFSET], this->imgSize.x * this->imgSize.y * (bpp / 8));

		delete[] str;

		GLCallThrow(glGenTextures, 1, &this->glId);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, this->glId);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		GLCallThrow(glTexParameteri, GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
		GLCallThrow(glTexParameteriv, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

		switch (bpp)
		{
			case 8:
				GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, (GLint)GL_RGBA, this->imgSize.x, this->imgSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, img);
				break;
			case 24:
				GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB, this->imgSize.x, this->imgSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
				break;
			case 32:
				GLCallThrow(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA, this->imgSize.x, this->imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
				break;
			default:
			{
				std::cerr << "Unsupported bits per pixel: " << bpp << std::endl;
				this->loaded = false;
				delete [] img;
				return ;
			}
		}

		delete [] img;

		GLCallThrow(glGenBuffers, 1, &VBO);
		GLCallThrow(glGenVertexArrays, 1, &VAO);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
		GLCallThrow(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STATIC_DRAW);

		GLCallThrow(glBindVertexArray, VAO);
		GLCallThrow(glEnableVertexAttribArray, 0);
		GLCallThrow(glVertexAttribPointer, 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);
		GLCallThrow(glBindVertexArray, 0);
	}

	const mft::vec2i GLFont::getCharacterSize(char c) const
	{
		return mft::vec2i(this->widths[c], this->cellSize.y);
	}
#endif

	//	Getters

#ifdef USING_EXTERNAL_LIBS
	const std::map<char, GLCharacter*>& GLFont::getCharacters() const
	{
		return characters;
	}

	const GLCharacter* GLFont::getCharacter(char c)
	{
		return characters[c];
	}
#endif

	const std::string	GLFont::getAssetType() const
	{
		return std::string("GLFont");
	}

	const GLShaderProgram* GLFont::getShader() const
	{
		return this->shader;
	}

	const unsigned int GLFont::getShaderID() const
	{
		if (this->shader != nullptr)
			return this->shader->programID;
		return 0;
	}

	//	Setters

	void	GLFont::setShader(GLShaderProgram* shader)
	{
		this->shader = shader;
	}

	void	GLFont::RenderText(std::string text, mft::vec2i pos, float size, const mft::vec4& color,
		GLShaderProgram* shader)
	{
		if (shader == nullptr)
		{
			if (this->shader == nullptr)
				return;
			shader = this->shader;
		}

#ifdef USING_EXTERNAL_LIBS

		bindVector(shader->programID, "textColor", color);
		bindMatrix(shader->programID, "model", mft::mat4());
		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindVertexArray, VAO);

		float scale = size / this->characters['C']->getSize().y;

		for (auto c : text)
		{
			GLCharacter* ch = this->characters[c];
			float	xpos = pos.x + ch->getBearing().x * scale;
			float	ypos = pos.y - (ch->getSize().y - ch->getBearing().y) * scale;

			float	w = ch->getSize().x * scale;
			float	h = ch->getSize().y * scale;

			float	vertices[6][4] =
			{
				{xpos,		ypos + h,	0.0f, 0.0f},
				{xpos,		ypos,		0.0f, 1.0f},
				{xpos + w,	ypos,		1.0f, 1.0f},

				{xpos,		ypos + h,	0.0f, 0.0f},
				{xpos + w,	ypos,		1.0f, 1.0f},
				{xpos + w,	ypos + h,	1.0f, 0.0f}
			};

			GLCallThrow(glBindTexture, GL_TEXTURE_2D, ch->getId());

			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
			GLCallThrow(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);

			GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 6);

			pos.x += (ch->getNext() >> 6) * scale;
		}

#else

		bindVector(shader->programID, "color", color);
		bindMatrix(shader->programID, "model", mft::mat4());
		GLCallThrow(glActiveTexture, GL_TEXTURE0);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, glId);
		GLCallThrow(glBindVertexArray, VAO);

		float scale = size / (float)this->cellSize.y;

		/*std::cout << "text = '" << text << "'" << std::endl;
		std::cout << "Cell size y = " << this->cellSize.y << std::endl;
		std::cout << "Scale = " << scale << std::endl;*/
		for (auto c: text)
		{
			//std::cout << "Char = '" << c << "'" << std::endl;
			unsigned int row = (c - this->firstChar) / this->charsPerLine;
			unsigned int col = (c - this->firstChar) - row * this->charsPerLine;

			float u = col * factor.x;
			float v = row * factor.y;

			float u1 = u + factor.x * (this->widths[c] / static_cast<float>(this->cellSize.x));
			float v1 = v + factor.y;

			float xpos = pos.x;
			float ypos = pos.y;

			float xend = xpos + this->widths[c] * scale;
			float yend = ypos + this->cellSize.y * scale;
			/*std::cout << "ystart = " << ypos << std::endl;
			std::cout << "yend = " << yend << std::endl;
			std::cout << "xstart = " << xpos << std::endl;
			std::cout << "xend = " << xend << std::endl;

			std::cout << "u = " << u << std::endl;
			std::cout << "v = " << v << std::endl;
			std::cout << "u1 = " << u1 << std::endl;
			std::cout << "v1 = " << v1 << std::endl;*/

			float	vertices[] =
			{
				xpos, yend, u, v,
				xend, ypos, u1, v1,
				xpos, ypos, u, v1,

				xpos, yend, u, v,
				xend, yend, u1, v,
				xend, ypos, u1, v1
			};

			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, VBO);
			GLCallThrow(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			GLCallThrow(glBindBuffer, GL_ARRAY_BUFFER, 0);

			GLCallThrow(glDrawArrays, GL_TRIANGLES, 0, 6);

			pos.x = xend;
		}
		//std::cout << std::endl;

#endif

		GLCallThrow(glBindVertexArray, 0);
		GLCallThrow(glBindTexture, GL_TEXTURE_2D, 0);

	}
}
