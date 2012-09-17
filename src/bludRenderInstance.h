#pragma once
#include <vector>
// an object that keeps around a bludSpriteSheet, so that it can be rendered outside of the lua loop.
// this should allow the lua loop to not block the main thread for as long

#include "bludSpriteSheet.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define GL_CHECK(x) {                       \
    (x);                                    \
    GLenum error = glGetError();            \
    if (GL_NO_ERROR != error) {             \
        printf("%s\n", gluErrorString(error));\
    }                                       \
}
class bludRenderSingleton{
public:
    bludRenderSingleton(){
        mainShader.load("Shader");
        fboShader.load("FBOShader");
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
         

        glEnableVertexAttribArray(glGetAttribLocation(fboShader.getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader.getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(fboShader.getProgram(), "TexCoordIn"));

        hasSpriteSheet = false;
        currentBlend = 0;
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "position"));
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "color"));
        glEnableVertexAttribArray(glGetAttribLocation(mainShader.getProgram(), "TexCoordIn"));
        // just for fun, lets render everything into an fbo
    }
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
    GLuint framebuffer;
    GLint baseFramebuffer;
    ofImage             testImg;
    ofTexture       *fboTex;
    ofShader        fboShader;
    ofShader mainShader;
    void render();
    vector<bludSpriteSheet*> sheets;
    bool hasSpriteSheet;
    bool alpha;
    int currentBlend;
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