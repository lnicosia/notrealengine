
#ifndef  __GLCONTEXT_CLASS_H__
# define __GLCONTEXT_CLASS_H__

# include "GLShaderProgram.class.hpp"

# include <filesystem>
# include <map>
# include <string>

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
			makeCurrent( ) = 0;
		virtual bool
			isCurrent( ) = 0;

		void
			registerShader( std::string name, const std::filesystem::path&, const std::filesystem::path& );
		GLShaderProgram *
			getShader( const std::string& );

		static std::filesystem::path
			DefaultShaderPath;

	protected:
		static long
			CurrentContext;
		GLContext( ); // Abstract class, constructor disaled
		~GLContext( );

	private:
		std::map<std::string,GLShaderProgram>
			shaders;

	};
} // namespace notrealengine

#endif

