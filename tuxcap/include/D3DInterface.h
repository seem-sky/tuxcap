#ifndef __D3DINTERFACE_H__
#define __D3DINTERFACE_H__

#include "Common.h"
#include "MemoryImage.h"
#include "SexyMatrix.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

namespace Sexy
{

class DDInterface;
class SexyMatrix3;
class TriVertex;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// The following flags apply to to the mD3DFlags member of MemoryImage
enum D3DImageFlags 
{
	D3DImageFlag_MinimizeNumSubdivisions	=			0x0001,		// subdivide image into fewest possible textures (may use more memory)
	D3DImageFlag_Use64By64Subdivisions		=			0x0002,		// good to use with image strips so the entire texture isn't pulled in when drawing just a piece
	D3DImageFlag_UseA4R4G4B4				=			0x0004,		// images with not too many color gradients work well in this format
	D3DImageFlag_UseA8R8G8B8				=			0x0008		// non-alpha images will be stored as R5G6B5 by default so use this option if you want a 32-bit non-alpha image
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct TextureDataPiece
{
	GLuint mTexture;
	int mWidth,mHeight;
  GLfloat mTexCoords[4]; //FIXME probably remove
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum PixelFormat
{
	PixelFormat_Unknown				=			0x0000,
	PixelFormat_A8R8G8B8			=			0x0001,
	PixelFormat_A4R4G4B4			=			0x0002,
	PixelFormat_R5G6B5				=			0x0004,
	PixelFormat_Palette8			=			0x0008
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct TextureData
{
public:
	typedef std::vector<TextureDataPiece> TextureVector;

	TextureVector mTextures;
#if 0
	LPDIRECTDRAWPALETTE mPalette;
#endif	
	int mWidth,mHeight;
	int mTexVecWidth, mTexVecHeight;
	int mTexPieceWidth, mTexPieceHeight;
	int mBitsChangedCount;
	int mTexMemSize;
	float mMaxTotalU, mMaxTotalV;
	PixelFormat mPixelFormat;
	Uint32 mImageFlags;

	TextureData();
	~TextureData();

	void ReleaseTextures();
	void CreateTextureDimensions(MemoryImage *theImage);
	GLuint GetTexture(int x, int y, int &width, int &height, float &u1, float &v1, float &u2, float &v2);
	GLuint GetTextureF(float x, float y, float &width, float &height, float &u1, float &v1, float &u2, float &v2);
  void CreateTextures(MemoryImage *theImage);
	void CheckCreateTextures(MemoryImage *theImage);
	void Blt(float theX, float theY, const Rect& theSrcRect, const Color& theColor);
	void BltTransformed(const SexyMatrix3 &theTrans, const Rect& theSrcRect, const Color& theColor, const Rect *theClipRect = NULL, float theX = 0, float theY = 0, bool center = false);	
	void BltTriangles(const TriVertex theVertices[][3], int theNumTriangles, Uint32 theColor, float tx = 0, float ty = 0);
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class D3DInterface
{
public:
#if 0
	HWND					mHWnd;
	LPDIRECTDRAW7			mDD;
	LPDIRECT3D7				mD3D;
	LPDIRECT3DDEVICE7		mD3DDevice;
	D3DVIEWPORT7			mD3DViewport;

#endif
	int						mWidth;
	int						mHeight;


	SDL_Surface*	mDDSDrawSurface;
	SDL_Surface*	mZBuffer;

	bool					mSceneBegun;
	bool					mIsWindowed;

	typedef std::set<MemoryImage*> ImageSet;
	ImageSet mImageSet;

	typedef std::list<SexyMatrix3> TransformStack;
	TransformStack mTransformStack;

static	std::string mErrorString;

protected:
	void					UpdateViewport();
	bool					InitD3D();	
	void					SetupDrawMode(int theDrawMode, const Color &theColor, Image *theImage);
#if 0
	static HRESULT CALLBACK PixelFormatsCallback(LPDDPIXELFORMAT theFormat, LPVOID lpContext);
#endif
public:
	D3DInterface();
	virtual ~D3DInterface();

	void					Cleanup();
	void					PushTransform(const SexyMatrix3 &theTransform, bool concatenate = true);
	void					PopTransform();
	
	bool					PreDraw();
	void					Flush();
	void					RemoveMemoryImage(MemoryImage *theImage);

	bool					CreateImageTexture(MemoryImage *theImage);
	bool					RecoverBits(MemoryImage* theImage);
	void					SetCurTexture(MemoryImage *theImage);
	void					Blt(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter = false);
	void					BltClipF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect *theClipRect, const Color& theColor, int theDrawMode);
	void					BltMirror(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter = false);
	void					StretchBlt(Image* theImage,  const Rect& theDestRect, const Rect& theSrcRect, const Rect* theClipRect, const Color &theColor, int theDrawMode, bool fastStretch, bool mirror = false);
	void					BltRotated(Image* theImage, float theX, float theY, const Rect* theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY, const Rect& theSrcRect);
	void					BltTransformed(Image* theImage, const Rect* theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, const SexyMatrix3 &theTransform, bool linearFilter, float theX = 0, float theY = 0, bool center = false);
	void					DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode);
	void					FillRect(const Rect& theRect, const Color& theColor, int theDrawMode);
	void					DrawTriangle(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode);
	void					DrawTriangleTex(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode, Image *theTexture, bool blend = true);
	void					DrawTrianglesTex(const TriVertex theVertices[][3], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx = 0, float ty = 0, bool blend = true);
	void					DrawTrianglesTexStrip(const TriVertex theVertices[], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx = 0, float ty = 0, bool blend = true);
	void					FillPoly(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty);

	bool					InitFromDDInterface(DDInterface *theInterface);
#if 0
	static void				MakeDDPixelFormat(PixelFormat theFormatType, DDPIXELFORMAT* theFormat);
	static PixelFormat		GetDDPixelFormat(LPDDPIXELFORMAT theFormat);
	static bool				CheckDXError(HRESULT theError, const char *theMsg="");
#endif
};


}


#endif //__D3DINTERFACE_H__

