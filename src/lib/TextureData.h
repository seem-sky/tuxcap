/*
 * File:   TextureData.h
 * Author: kees
 *
 * Created on March 13, 2011, 3:36 PM
 */

#ifndef TEXTUREDATA_H
#define	TEXTUREDATA_H

#include "Common.h"
#include "Color.h"
#include "Rect.h"

#include <vector>

#include <SDL.h>
#ifdef USE_OPENGLES
#include <SDL_opengles.h>
#else
#include <SDL_opengl.h>
#endif

namespace Sexy
{

class Image;
class SexyMatrix3;
class TriVertex;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct TextureDataPiece
{
    GLuint              mTexture;
    int                 mWidth, mHeight;
    int                 mX0, mY0;
    int                 mX1, mY1;
    unsigned int        texture_offset;
    unsigned int        color_offset;
    unsigned int        vertex_offset;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum PixelFormat
{
    PixelFormat_Unknown             =           0x0000,
    PixelFormat_A8R8G8B8            =           0x0001,
    PixelFormat_A4R4G4B4            =           0x0002,
    PixelFormat_R5G6B5              =           0x0004,
    PixelFormat_Palette8            =           0x0008
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct TextureData
{
private:
    std::vector<TextureDataPiece> mTextures;
    int                 mWidth, mHeight;
    int                 mTexVecWidth, mTexVecHeight;
    int                 mTexPieceWidth, mTexPieceHeight;
    int                 mBitsChangedCount;
    PixelFormat         mPixelFormat;
    Uint32              mImageFlags;             // See Image::mD3DFlags and enum D3DImageFlags
    bool                mHasAlpha;
    float               mMaxTotalU, mMaxTotalV;
    int                 mTexMemSize;
    GLuint              mVBO_static;
    GLuint              mVBO_colors; //this one is dynamic
    Color               mLast_color;
    SexyRGBA            mColors[4];

public:
    TextureData();
    ~TextureData();

    void    CheckCreateTextures(Image *theImage);
    void    Blt();
    void    Blt(const Color& theColor);
    void    Blt(float theX, float theY,const Rect& theSrcRect, const Color& theColor);
    void    BltTransformed(const SexyMatrix3 &theTrans, const Rect& theSrcRect, const Color& theColor, const Rect *theClipRect = NULL, float theX = 0, float theY = 0, bool center = false);
    void    BltTransformed(const Color& theColor, const Rect *theClipRect = NULL, float theX = 0, float theY = 0, bool center = false);
    void    BltTransformed(const Rect *theClipRect = NULL, float theX = 0, float theY = 0, bool center = false);
    void    BltTriangles(const TriVertex theVertices[][3], int theNumTriangles, Uint32 theColor, float tx = 0, float ty = 0);

    static void SetMinMaxTextureDimension(int minWidth, int miHeight, int maxWidth, int maxHeight, int maxAspectRatio);
    static void SetMaxTextureDimension(int maxWidth, int maxHeight);
    static void SetMaxTextureAspectRatio(int maxAspectRatio);
    bool hasAlpha() const { return mHasAlpha;}

private:
    void    ReleaseTextures();
    void    CreateTextureDimensions(Image *theImage);
    GLuint  GetTexture(int x, int y, int &width, int &height, float &u1, float &v1, float &u2, float &v2);
    void    CreateTextures(Image *theImage);
    void    CreateTexturesFromSubs(Image *theImage);
    void    GetBestTextureDimensions(int &theWidth, int &theHeight, bool isEdge, Uint32 theImageFlags, bool isPow2, bool isSquare);
};

}

#endif	/* TEXTUREDATA_H */
