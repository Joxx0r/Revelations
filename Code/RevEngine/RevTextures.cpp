#include "stdafx.h"
#include "RevTextures.h"
#include "RevEffect.h"
#include "RevTexture.h"

RevTextures::RevTextures(void)
{
}


RevTextures::~RevTextures(void)
{
}

#pragma optimize("", off)

void RevTextures::AddShaderResource( 
	const char* aResourceName,
	ID3D11ShaderResourceView* aSRV )
{
	myDictonary[ aResourceName ] = aSRV;
}

void RevTextures::AddShaderResource( const char* aResourceName,
									const char* aTexturePath,
									bool aUseSrgbFlag)
{
	RevTexture* tex = new RevTexture();
	if (!tex->LoadTexture(aTexturePath, aUseSrgbFlag))
	{
		assert(0 && "FAILED AT LOADING: %s", aTexturePath);
		delete tex;
		return;
	}
	myDictonary[ aResourceName ] = tex->GetSRV();
	m_debugInfo[aResourceName] = aTexturePath;
}

void RevTextures::Render( RevEffect* aEffect )
{
	assert( aEffect != NULL );
	ShaderDictonary::iterator iter = myDictonary.begin();
	for(; iter != myDictonary.end(); iter++)
	{
		aEffect->SetTextureVarible( iter->first, iter->second ); 
	}
}

#pragma optimize("", on)