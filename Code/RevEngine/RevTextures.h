#ifndef _REV_TEXTURES_H
#define _REV_TEXTURES_H

#include <string>
#include <map>

struct ID3D11ShaderResourceView;
class RevEffect;

typedef std::map< const char*, ID3D11ShaderResourceView* > ShaderDictonary;

class RevTextures
{
public:
	RevTextures(void);
	~RevTextures(void);
	void AddShaderResource( const char* aResourceName,
							ID3D11ShaderResourceView* aSRV );
	void AddShaderResource( const char* aResourceName,
							const char* aTexturePath,
							bool aUseSrgbFlag=true );
	void Render(RevEffect* aEffect);

private:
	ShaderDictonary myDictonary;
	
	std::map<std::string, std::string > m_debugInfo;
};

#endif