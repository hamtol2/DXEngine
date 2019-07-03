#include "Shader.h"

Shader::Shader()
{

}

Shader::Shader(LPCTSTR fileName)
{
	this->fileName = fileName;
	entryPoint = "main";
}

Shader::Shader(LPCTSTR fileName, LPCSTR entry, LPCSTR profile)
{
	this->fileName = fileName;
	this->entryPoint = entry;
	this->profile = profile;
}

Shader::~Shader()
{

}

void Shader::Release()
{
	Memory::SafeRelease(shaderBuffer);
}