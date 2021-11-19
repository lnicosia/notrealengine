
#ifndef  __GLCONTEXT_CLASS_H__
# define __GLCONTEXT_CLASS_H__

# include "GLShaderProgram.class.hpp"

# include <string>
# include <filesystem>
# include <map>

# include "GL.hpp"

namespace notrealengine
{

	/*
	 *  This class works as an interface, could be instantiated
	 *  using different libraries in the future
	 */
	class GLContext {

	public:
		virtual void
			makeCurrent( void );
		virtual bool
			isCurrent( void );

		void
			registerShader( std::string, std::filesystem::path, std::filesystem::path );
		GLShaderProgram *
			getShader( std::string );

		static std::filesystem::path
			DefaultShaderPath;

	protected:
		static long
			CurrentContext;
		GLContext( void ); // Abstract class, constructor disaled
		~GLContext( void );

	private:
		std::map<std::string,GLShaderProgram>
			shaders;

	};
}

#endif

