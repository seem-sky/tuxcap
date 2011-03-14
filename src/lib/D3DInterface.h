#ifndef __D3DINTERFACE_H__
#define __D3DINTERFACE_H__

#include "Common.h"
#include "MemoryImage.h"
#include "SexyMatrix.h"
#include "Logging.h"
#include <SDL.h>
#ifdef USE_OPENGLES
#include <SDL_opengles.h>
#else
#include <SDL_opengl.h>
#endif
#include <string.h>

namespace Sexy
{

class DDInterface;
class SexyMatrix3;
class TriVertex;

#ifndef WIN32
//Aligned vertex structure
typedef struct {
    GLfloat tu;
    GLfloat tv;
    SexyRGBA color;
    GLfloat sx;
    GLfloat sy;
} D3DTLVERTEX;
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// The following flags apply to to the mD3DFlags member of MemoryImage
enum D3DImageFlags 
{
    D3DImageFlag_MinimizeNumSubdivisions    =           0x0001,     // subdivide image into fewest possible textures (may use more memory)
    D3DImageFlag_Use64By64Subdivisions      =           0x0002,     // good to use with image strips so the entire texture isn't pulled in when drawing just a piece
    D3DImageFlag_UseA4R4G4B4                =           0x0004,     // images with not too many color gradients work well in this format
    D3DImageFlag_UseA8R8G8B8                =           0x0008      // non-alpha images will be stored as R5G6B5 by default so use this option if you want a 32-bit non-alpha image
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class D3DInterface
{
public:
#if 0
    HWND                    mHWnd;
    LPDIRECTDRAW7           mDD;
    LPDIRECT3D7             mD3D;
    LPDIRECT3DDEVICE7       mD3DDevice;
    D3DVIEWPORT7            mD3DViewport;

#endif
    GLuint                  custom_cursor_texture;

    SDL_Surface*            mZBuffer;

    bool                    mSceneBegun;

    typedef std::list<SexyMatrix3> TransformStack;
    TransformStack          mTransformStack;

    static  std::string     mErrorString;

    static bool glIsExtensionSupported(const char *extension);
    static bool glIsVersionOrHigher(const char* version);

private:
    typedef std::set<MemoryImage*> ImageSet;
    ImageSet                mImageSet;

protected:
    int                     mWidth;
    int                     mHeight;
    //bool                    mIsWindowed;
    int                     lastDrawMode;

    LoggerFacil *           mLogFacil;

    void                    UpdateViewport();
    bool                    InitD3D();  
    void                    SetupDrawMode(int theDrawMode, const Color &theColor, Image *theImage);
#if 0
    static HRESULT CALLBACK PixelFormatsCallback(LPDDPIXELFORMAT theFormat, LPVOID lpContext);
#endif
public:
    D3DInterface();
    virtual ~D3DInterface();

    void                    Cleanup();
    void                    PushTransform(const SexyMatrix3 &theTransform, bool concatenate = true);
    void                    PopTransform();
    
    bool                    PreDraw();
    void                    Flush();
    void                    RemoveMemoryImage(MemoryImage *theImage);

    void                    FillOldCursorAreaTexture(GLint x, GLint y);
    void                    BltOldCursorArea(GLfloat x, GLfloat y, const Color& theColor);
    bool                    CreateImageTexture(MemoryImage *theImage);
    void                    Blt(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter = false);    
    void                    BltClipF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect *theClipRect, const Color& theColor, int theDrawMode);
    void                    BltMirror(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter = false);
    void                    StretchBlt(Image* theImage,  const Rect& theDestRect, const Rect& theSrcRect, const Rect* theClipRect, const Color &theColor, int theDrawMode, bool fastStretch, bool mirror = false);
    void                    BltRotated(Image* theImage, float theX, float theY, const Rect* theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY, const Rect& theSrcRect);
    void                    BltTransformed(Image* theImage, const Rect* theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, const SexyMatrix3 &theTransform, bool linearFilter, float theX = 0, float theY = 0, bool center = false);
    void                    DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode);
    void                    FillRect(const Rect& theRect, const Color& theColor, int theDrawMode);
    void                    DrawTriangle(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode);
    void                    DrawTriangleTex(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode, Image *theTexture, bool blend = true);
    void                    DrawTrianglesTex(const TriVertex theVertices[][3], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx = 0, float ty = 0, bool blend = true);
    void                    DrawTrianglesTexStrip(const TriVertex theVertices[], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx = 0, float ty = 0, bool blend = true);
    void                    FillPoly(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty);

    bool                    InitFromDDInterface(DDInterface *theInterface);
#if 0
    static void             MakeDDPixelFormat(PixelFormat theFormatType, DDPIXELFORMAT* theFormat);
    static PixelFormat      GetDDPixelFormat(LPDDPIXELFORMAT theFormat);
    static bool             CheckDXError(HRESULT theError, const char *theMsg="");
#endif

#if TARGET_OS_IPHONE == 0
    //vertex buffer object extension functions
    typedef void (APIENTRY * glBindBufferARB_Func) (GLenum, GLuint);
    typedef void (APIENTRY * glBufferDataARB_Func) (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
    typedef void (APIENTRY * glBufferSubDataARB_Func) (GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *);
    typedef void (APIENTRY * glDeleteBuffersARB_Func) (GLsizei, const GLuint *);
    typedef void (APIENTRY * glGenBuffersARB_Func) (GLsizei, GLuint *);
    typedef GLvoid* (APIENTRY * glMapBufferARB_Func) (GLenum, GLenum);
    typedef GLboolean (APIENTRY * glUnmapBufferARB_Func) (GLenum);

    glBindBufferARB_Func glBindBufferARB_ptr ; 
    glBufferDataARB_Func glBufferDataARB_ptr ;
    glBufferSubDataARB_Func glBufferSubDataARB_ptr ; 
    glDeleteBuffersARB_Func glDeleteBuffersARB_ptr ; 
    glGenBuffersARB_Func glGenBuffersARB_ptr ;
    glMapBufferARB_Func glMapBufferARB_ptr ;
    glUnmapBufferARB_Func glUnmapBufferARB_ptr ;
#endif

    //vertex buffer object functions part of opengl
    typedef void (APIENTRY * glBindBuffer_Func) (GLenum, GLuint);
    typedef void (APIENTRY * glBufferData_Func) (GLenum, GLsizeiptr, const GLvoid *, GLenum);
    typedef void (APIENTRY * glBufferSubData_Func) (GLenum, GLintptr, GLsizeiptr, const GLvoid *);
    typedef void (APIENTRY * glDeleteBuffers_Func) (GLsizei, const GLuint *);
    typedef void (APIENTRY * glGenBuffers_Func) (GLsizei, GLuint *);
    typedef GLvoid* (APIENTRY * glMapBuffer_Func) (GLenum, GLenum);
    typedef GLboolean (APIENTRY * glUnmapBuffer_Func) (GLenum);

    glBindBuffer_Func glBindBuffer_ptr ; 
    glBufferData_Func glBufferData_ptr ;
    glBufferSubData_Func glBufferSubData_ptr ; 
    glDeleteBuffers_Func glDeleteBuffers_ptr ; 
    glGenBuffers_Func glGenBuffers_ptr ;
    glMapBuffer_Func glMapBuffer_ptr ;
    glUnmapBuffer_Func glUnmapBuffer_ptr ;

    bool glEnableVertexBufferObjects();
};
}


#endif //__D3DINTERFACE_H__
