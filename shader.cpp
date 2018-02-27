#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader.h"
using namespace std;

GLuint LoadShaders( const char * vertexFilePath,const char * fragmentFilePath ){
	// Create shaders
	GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read Vertex Shader code file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream( vertexFilePath, std::ios::in );
	if( VertexShaderStream.is_open() ){
		std::string Line = "";
		while( getline( VertexShaderStream, Line ) )
			VertexShaderCode += "\n" + Line;
			std::cout << "Loading vertex shader source.." << VertexShaderCode << '\n' << '\n';
		VertexShaderStream.close();
	}else{
		printf( "Failed to open.. %s. check path..\n", vertexFilePath );
		getchar();
		return 0;
	}
	// Read Fragment Shader code file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream( fragmentFilePath, std::ios::in );
	if( FragmentShaderStream.is_open() ){
		std::string Line = "";
		while( getline( FragmentShaderStream, Line ) )
			FragmentShaderCode += "\n" + Line;
			std::cout << "Loading fragment shader source.." << FragmentShaderCode << '\n' << '\n';
		FragmentShaderStream.close();
	}
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf( "Compiling shader: %s\n", vertexFilePath );
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource( VertexShaderID, 1, &VertexSourcePointer , NULL );
	glCompileShader( VertexShaderID );

	// Check Vertex Shader
	glGetShaderiv( VertexShaderID, GL_COMPILE_STATUS, &Result );
	glGetShaderiv( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage( InfoLogLength+1 );
		glGetShaderInfoLog( VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0] );
		printf( "%s\n", &VertexShaderErrorMessage[0] );
	}
	// Compile Fragment Shader
	printf( "Compiling shader: %s\n", fragmentFilePath );
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource( FragmentShaderID, 1, &FragmentSourcePointer , NULL );
	glCompileShader( FragmentShaderID );

	// Check Fragment Shader
	glGetShaderiv( FragmentShaderID, GL_COMPILE_STATUS, &Result );
	glGetShaderiv( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage( InfoLogLength+1 );
		glGetShaderInfoLog( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0] );
		printf( "%s\n", &FragmentShaderErrorMessage[0] );
	}
	// Link the program
	printf( "Linking program..\n" );
	GLuint ProgramID = glCreateProgram();
	glAttachShader( ProgramID, VertexShaderID );
	glAttachShader( ProgramID, FragmentShaderID );
	// glBindAttribLocation( ProgramID, 0, "vposition" );
	// glBindAttribLocation( ProgramID, 1, "vcolor" );
	// glBindAttribLocation( ProgramID, 2, "voffset" );

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv( ProgramID, GL_LINK_STATUS, &Result );
	glGetProgramiv( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage( InfoLogLength+1 );
		glGetProgramInfoLog( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0] );
		printf( "%s\n", &ProgramErrorMessage[0] );
	}
	glDetachShader( ProgramID, VertexShaderID );
	glDetachShader( ProgramID, FragmentShaderID );
	glDeleteShader( VertexShaderID );
	glDeleteShader( FragmentShaderID );

	return ProgramID;
}
