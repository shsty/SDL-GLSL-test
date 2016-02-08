#include <cstdio>
#include <iostream>
#include <cmath>
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

char* filetobuf(const char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

void checkglerr(){
    GLenum glerror = glGetError();
    if (glerror != GL_NO_ERROR){
        std::cerr << "GL Error : " << glerror << std::endl;
        throw;
    }
}

/*SDL_Texture* LoadImage(SDL_Renderer renderer, const char * file){
    SDL_Texture * tex;
    tex = IMG_LoadTexture(renderer, file);
    if (!tex) {
        std::cerr << "Failed to load image : " << file << std::endl;
        throw;
    }
    return tex;
}*/

void drawscene(SDL_Window * window){
    int IsCompiled_VS, IsCompiled_FS;
    int IsLinked;
    int maxLength;
    char *vertexInfoLog;
    char *fragmentInfoLog;
    char *shaderProgramInfoLog;

    const char *vertexfile = "tutorial2.vert";
    const char *fragmentfile = "tutorial2.frag";

    /* These pointers will receive the contents of our shader source code files */
    GLchar *vertexsource, *fragmentsource;
    GLuint vertexshader, fragmentshader;
    GLuint shaderprogram;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Read our shaders into the appropriate buffers */
    vertexsource = filetobuf(vertexfile);
    fragmentsource = filetobuf(fragmentfile);

    if (!vertexsource) {
        std::cerr << "Cannot open vertex shader file : " << vertexfile << std::endl;
        return;
    }

    if (!fragmentsource) {
        std::cerr << "Cannot open fragment shader file : " << fragmentfile << std::endl;
        return;
    }

    /* Create an empty vertex shader handle */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glCompileShader(vertexshader);
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if(IsCompiled_VS == false)
    {
        glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
        vertexInfoLog = (char *)malloc(maxLength);
        glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
        std::cerr << "Vertex shader compilation failed: \n" << vertexInfoLog;
        free(vertexInfoLog);
        return;
    }

    /* Create an empty fragment shader handle */
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == false)
    {
        glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
        fragmentInfoLog = (char *)malloc(maxLength);
        glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
        std::cerr << "Fragment shader compilation failed: \n" << fragmentInfoLog;
        free(fragmentInfoLog);
        return;
    }

    shaderprogram = glCreateProgram();

    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");

    glLinkProgram(shaderprogram);

    glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
    if(IsLinked == false)
    {
        glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
        shaderProgramInfoLog = (char *)malloc(maxLength);
        glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);
        std::cerr << "Shader Program linking failed: \n" << shaderProgramInfoLog;
        free(shaderProgramInfoLog);
        return;
    }

    /* Load the shader into the rendering pipeline */
    glUseProgram(shaderprogram);


    GLuint vao, vbo[2]; /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */
    const GLfloat diamond[4][2] = {
        {  1.0,  1.0  }, 
        {  1.0, -1.0  },
        { -1.0, -1.0  },
        { -1.0,  1.0  } };
    const GLfloat colors[4][3] = {
        {  1.0,  0.0,  0.0  }, /* Red */
        {  0.0,  1.0,  0.0  }, /* Green */
        {  0.0,  0.0,  1.0  }, /* Blue */
        {  1.0,  1.0,  1.0  } }; /* White */
    const GLfloat bordercolor[4] = {0.0, 0.0, 0.0, 0.0};
    const char * imgfile = "images2.png";
    SDL_Surface *TextureImage[1], *tmpimage;
    GLuint texture[1];

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint trID = glGetUniformLocation(shaderprogram, "tr");

    checkglerr();

    tmpimage = IMG_Load(imgfile); 
    if (!tmpimage){
        std::cerr << "Failed to load image : " << imgfile << std::endl;
        return;
    }
    TextureImage[0] = SDL_ConvertSurfaceFormat(tmpimage, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!TextureImage[0]){
        std::cerr << "Failed to convert image : \n" << SDL_GetError() << std::endl;
        return;
    }
    SDL_FreeSurface(tmpimage);

    glGenTextures(1, &texture[0]);
    glBindTexture( GL_TEXTURE_2D, texture[0]);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->w, TextureImage[0]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage[0]->pixels );

    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 3 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    SDL_FreeSurface(TextureImage[0]);
    checkglerr();

    for (float i = 0.0; i <= 3.0; i+=0.033)
    {
        glUniform4f(trID, cosh(i/2), 0.0f, sinh(i/2), 0.0f);
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_POLYGON, 0, 4);
        checkglerr();
        SDL_GL_SwapWindow(window);
        SDL_Delay(33);
    }

    /* Cleanup all the things we bound and allocated */
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
    free(vertexsource);
    free(fragmentsource);
}

