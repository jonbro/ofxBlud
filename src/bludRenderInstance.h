#pragma once
#include <vector>
// an object that keeps around a bludSpriteSheet, so that it can be rendered outside of the lua loop.
// this should allow the lua loop to not block the main thread for as long

#include "bludSpriteSheet.h"
#include "ofGraphics.h"
#include "ofShader.h"
#if defined TARGET_OF_IPHONE
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
#endif
#define GL_CHECK(x) {                       \
    (x);                                    \
    GLenum error = glGetError();            \
    if (GL_NO_ERROR != error) {             \
        printf("%s\n", gluErrorString(error));\
    }                                       \
}

#define DISTORTIONMESH 1

class bludRenderSingleton{
public:
    bludRenderSingleton(){
        pointCount = 0;
        mainShader.load("Shader");
        noTexShader.load("NoTexShader");
        fboShader = new ofShader();
        fboShader->load("FBOShader");
#if defined TARGET_OF_IPHONE
        fboTex = new ofTexture();
        fboTex->allocate(ofNextPow2(ofGetWidth()), ofNextPow2(ofGetHeight()), GL_RGBA);
        GL_CHECK(glGenFramebuffers(1, &framebuffer));
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex->getTextureData().textureID, 0);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            printf("failed to make complete framebuffer object %x", status);
        }else{
            printf("framebuffer creation worked %x\n", status);
        }
#endif
        points = new vertexStruct[DISTORTIONMESH * DISTORTIONMESH * 4];
        // build out the points for the distortion mesh
        int x=0;
        float h = ofGetHeight();
        float w = ofGetWidth();
        for (int y=0; y<DISTORTIONMESH;y++) {
            for (int x=0; x<DISTORTIONMESH; x++) {
                //tl
                points[pointCount].position[0] = (float)x/(float)DISTORTIONMESH*w;
                points[pointCount].position[1] = (float)y/(float)DISTORTIONMESH*h;
                points[pointCount].texCoord[0] = (float)x/(float)DISTORTIONMESH;
                points[pointCount].texCoord[1] = (float)y/(float)DISTORTIONMESH;
                pointCount++;
                
                // tr
                points[pointCount].position[0] = (float)(x+1)/(float)DISTORTIONMESH*w;
                points[pointCount].position[1] = (float)(y)/(float)DISTORTIONMESH*h;
                points[pointCount].texCoord[0] = (float)(x+1)/(float)DISTORTIONMESH;
                points[pointCount].texCoord[1] = (float)(y)/(float)DISTORTIONMESH;
                pointCount++;
                
                //br
                points[pointCount].position[0] = (float)(x+1)/(float)DISTORTIONMESH*w;
                points[pointCount].position[1] = (float)(y+1)/(float)DISTORTIONMESH*h;
                points[pointCount].texCoord[0] = (float)(x+1)/(float)DISTORTIONMESH;
                points[pointCount].texCoord[1] = (float)(y+1)/(float)DISTORTIONMESH;
                pointCount++;
                
                //bl
                points[pointCount].position[0] = (float)(x)/(float)DISTORTIONMESH*w;
                points[pointCount].position[1] = (float)(y+1)/(float)DISTORTIONMESH*h;
                points[pointCount].texCoord[0] = (float)(x)/(float)DISTORTIONMESH;
                points[pointCount].texCoord[1] = (float)(y+1)/(float)DISTORTIONMESH;
                pointCount++;
                
            }
        }
 
        
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "TexCoordIn"));

        hasSpriteSheet = false;
        currentBlend = 0;
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "TexCoordIn"));

        glEnableVertexAttribArray(glGetAttribLocation(noTexShader.getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(noTexShader.getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(noTexShader.getProgram(), "TexCoordIn"));
        // just for fun, lets render everything into an fbo
        currentShader = &mainShader;
    }
    void loadFboPixelShader(lua_State *l){
//        fboShader.unload();
        // clear out any errors that may be in the buffer
        glGetError();
        ofSetLogLevel(OF_LOG_VERBOSE);
        fboShader->unload();
        delete fboShader;
        fboShader = new ofShader();
        fboShader->setupShaderFromFile(GL_VERTEX_SHADER, "FBOShader.vert");
        fboShader->setupShaderFromSource(GL_FRAGMENT_SHADER, luaL_checkstring(l, 1));
        fboShader->linkProgram();
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader->getProgram(), "TexCoordIn"));
        ofSetLogLevel(OF_LOG_ERROR);

    };
    void addAtStart(bludSpriteSheet *_sheet){
        vector<bludSpriteSheet*>::iterator it;
        it = sheets.begin();
        it = sheets.insert( it , _sheet );
    }
    void addSheet(bludSpriteSheet *_sheet){
        sheets.push_back(_sheet);
    }
    void removeSheet(bludSpriteSheet *_sheet){
        vector<bludSpriteSheet*>::iterator it;

        for ( it = sheets.begin(); it != sheets.end(); ){
            if( (*it) == _sheet ){
                it = sheets.erase(it);
            }else{
                ++it;
            }
        }
    }

    vertexStruct * points;

    GLuint framebuffer;
    GLint baseFramebuffer;
    ofImage             testImg;
    ofTexture       *fboTex;
    ofShader        *fboShader;
    ofShader mainShader, noTexShader;
    ofShader *currentShader;
    void render();
    vector<bludSpriteSheet*> sheets;
    bool hasSpriteSheet;
    bool alpha;
    int currentBlend;
    int pointCount;
};

class bludRenderInstance {
public:
	static bludRenderSingleton* getInstance();
private:
	static bludRenderSingleton *instance;
};

class bludRenderer{
public:
	static const char className[];
	static Lunar<bludRenderer>::RegType methods[];
	bludRenderer(lua_State *L) {
        renderer = bludRenderInstance::getInstance();
        renderer->hasSpriteSheet = false;
	}
    // lol, now I am not even pretending to have an api :D hack it up hacky!
    int loadFboPixelShader(lua_State *L){
        renderer->loadFboPixelShader(L);
        return 0;
    }
    int setSpriteSheet(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->addSheet(s);
        return 1;
    }
    int removeSheet(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->removeSheet(s);
        return 1;
    }
    int addAtStart(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->addAtStart(s);
        return 1;
    }
private:
    bludRenderSingleton *renderer;
};