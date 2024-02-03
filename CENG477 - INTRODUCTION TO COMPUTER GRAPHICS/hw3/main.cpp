#include <cstdio> //ok
#include <cstdlib> //ok
#include <cstring> //ok
#include <ctime> //random
#include <string> // ok
#include <fstream> //ok
#include <iostream> //ok
#include <sstream> //ok
#include <vector> //ok
#include <map> //ok
#define _USE_MATH_DEFINES
//#include <math.h>
#include <GL/glew.h> //ok
#include <GL/gl.h>   // The GL Header File
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

using namespace std;
std::string scoreText;
GLint gIntensityLoc;
float gIntensity = 1000;
int score = 0;
GLuint gProgram[6], vao, vaoSky, vaoCube, textProg;
GLuint gTextVBO;
GLuint skySampler, skyTexture, cubeSampler, cubeTexture;
GLuint gWVPLocation;
GLuint gSamplerLocation;
int gWidth, gHeight;
static bool slidingRight = false; 
static bool slidingLeft = false;
static float cubeZ = -50.f;
static float cubeZGeneral = -50.f;
static float cubeZinfinite = -50.f;
GLint cubeZLocation; 
static float cubeZ1 = -50.f;
static float cubeZ2 = -50.f;
static float cubeZinitial = -50.f;
	static float cubeY = 7.5f;
	static float cubeYinitial = 7.5f;
		static float angle = 90;
			static float deadAngle = 0;
			static float speed = 0.4f;

bool goingUp = true;

bool celebrating = false;

bool running = true;

glm::vec3 textColor = glm::vec3(1, 1, 0);

static float bunnyVerPos = 0.0f;
static float vertical_speed = 0.15f;
static int choosenCubeIndex;

GLint modelingMatrixLoc[5];
GLint viewingMatrixLoc[5];
GLint projectionMatrixLoc[5];
GLint eyePosLoc[5];

glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;
glm::mat4 modelingMatrix;
glm::mat4 modelingMatrix2;
glm::mat4 modelingMatrix3;
glm::mat4 modelingMatrix4;
glm::mat4 modelingMatrix5;
glm::mat4 modelingMatrixSky;
glm::vec3 eyePos(0, 0, 0);

int activeProgramIndex = 0;

glm::vec2 TexCoord;

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

struct Vertex
{
	Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
	GLfloat x, y, z;
};

struct Texture
{
	Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
	GLfloat u, v;
};

struct Normal
{
	Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
	GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
	GLuint vIndex[3], tIndex[3], nIndex[3];
};

vector<Vertex> gVertices;
vector<Texture> gTextures;
vector<Normal> gNormals;
vector<Face> gFaces;
vector<Vertex> gVertices2;
vector<Vertex> gVertices3;
vector<Texture> gTextures2;
vector<Texture> gTextures3;
vector<Normal> gNormals2;
vector<Normal> gNormals3;
vector<Face> gFaces2;
vector<Face> gFaces3;

GLuint gVertexAttribBuffer, gIndexBuffer;
GLuint gVertexAttribBufferSky, gIndexBufferSky;
GLuint gVertexAttribBufferCube, gIndexBufferCube;
GLint gInVertexLoc, gInNormalLoc;
int gVertexDataSizeInBytes, gNormalDataSizeInBytes;

bool ReadDataFromFile(
	const string& fileName, ///< [in]  Name of the shader file
	string& data)     ///< [out] The contents of the file
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			data += curLine;
			if (!myfile.eof())
			{
				data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}

	return true;
}
void createFS1(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    glAttachShader(program, fs);
}
void createVS1(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    glAttachShader(program, vs);
}

void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    glUseProgram(textProg);
    glUniform3f(glGetUniformLocation(textProg, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
void initFonts(int windowWidth, int windowHeight)
{
    // Set OpenGL options
    //glEnable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
    glUseProgram(textProg);
    glUniformMatrix4fv(glGetUniformLocation(textProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSerif-Italic.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //
    // Configure VBO for texture quads
    //
    glGenBuffers(1, &gTextVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ParseObj(const string& fileName, int objIndex)
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			stringstream str(curLine);
			GLfloat c1, c2, c3;
			GLuint index[9];
			string tmp;

			if (curLine.length() >= 2)
			{
				if (curLine[0] == 'v')
				{
					if (curLine[1] == 't') // texture
					{
						str >> tmp; // consume "vt"
						str >> c1 >> c2;
						switch (objIndex)
						{
						case 0:
							gTextures.push_back(Texture(c1, c2));
							break;
						case 1:
							gTextures2.push_back(Texture(c1, c2));
							break;
						case 2:
							gTextures3.push_back(Texture(c1, c2));
							break;
						default:
							break;
						}	
					}
					else if (curLine[1] == 'n') // normal
					{
						str >> tmp; // consume "vn"
						str >> c1 >> c2 >> c3;
						switch (objIndex)
						{
						case 0:
							gNormals.push_back(Normal(c1, c2, c3));
							break;
						case 1:
							gNormals2.push_back(Normal(c1, c2, c3));
							break;
						case 2:
							gNormals3.push_back(Normal(c1, c2, c3));
							break;
						default:
							break;
						}	
					}
					else // vertex
					{
						str >> tmp; // consume "v"
						str >> c1 >> c2 >> c3;
						switch (objIndex)
						{
						case 0:
							gVertices.push_back(Vertex(c1, c2, c3));
							break;
						case 1:
							gVertices2.push_back(Vertex(c1, c2, c3));
							break;
						case 2:
							gVertices3.push_back(Vertex(c1, c2, c3));
							break;
						default:
							break;
						}	
						
						
					}
				}
				else if (curLine[0] == 'f') // face
				{
					str >> tmp; // consume "f"
					char c;
					int vIndex[3], nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0];
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1];
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2];

					assert(vIndex[0] == nIndex[0] &&
						vIndex[1] == nIndex[1] &&
						vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}
					switch (objIndex)
					{
					case 0:
						gFaces.push_back(Face(vIndex, tIndex, nIndex));
						break;
					case 1:
						gFaces2.push_back(Face(vIndex, tIndex, nIndex));
						break;
					case 2:
						gFaces3.push_back(Face(vIndex, tIndex, nIndex));
						break;
					default:
						break;
					}
				}
				else
				{
					cout << "Ignoring unidentified line in obj file: " << curLine << endl;
				}
			}

			//data += curLine;
			if (!myfile.eof())
			{
				//data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}

	assert(gVertices.size() == gNormals.size());

	return true;
}


GLuint createVS(const char* shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &shader, &length);
	glCompileShader(vs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(vs, 1024, &length, output);
	printf("VS compile log: %s\n", output);

	return vs;
}

GLuint createFS(const char* shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &shader, &length);
	glCompileShader(fs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(fs, 1024, &length, output);
	printf("FS compile log: %s\n", output);

	return fs;
}

GLuint vs1,fs1,vs2,fs2,vs3,fs3,vs4,fs4,vs5,fs5,vs6,fs6;
void initShaders2()
{
    /**gProgram[0] = glCreateProgram();
    gProgram[1] = glCreateProgram();**/
    textProg = glCreateProgram();

    /**createVS(gProgram[0], "vert0.glsl");
    createFS(gProgram[0], "frag0.glsl");

    createVS(gProgram[1], "vert1.glsl");
    createFS(gProgram[1], "frag1.glsl");**/

    createVS1(textProg, "vert_text.glsl");
    createFS1(textProg, "frag_text.glsl");

    /**glBindAttribLocation(gProgram[0], 0, "inVertex");
    glBindAttribLocation(gProgram[0], 1, "inNormal");
    glBindAttribLocation(gProgram[1], 0, "inVertex");
    glBindAttribLocation(gProgram[1], 1, "inNormal");**/
    glBindAttribLocation(textProg, 2, "vertex");

    //glLinkProgram(gProgram[0]);
    //glLinkProgram(gProgram[1]);
    glLinkProgram(textProg);
    //glUseProgram(gProgram[0]);
}
void initShaders()
{
	// Create the programs

	gProgram[0] = glCreateProgram();
	gProgram[1] = glCreateProgram();
	gProgram[2] = glCreateProgram();
	gProgram[3] = glCreateProgram();
	gProgram[4] = glCreateProgram();
	gProgram[5] = glCreateProgram();
	textProg = glCreateProgram();


	glUseProgram(gProgram[0]);
    //glUniform1i(glGetUniformLocation(gProgram[0], "bunnyTexture"), 0); // 0 corresponds to texture unit 0
    glUseProgram(gProgram[1]);
    glUniform1i(glGetUniformLocation(gProgram[1], "skyTexture"), 1);
	glUseProgram(gProgram[2]);
	glUseProgram(gProgram[3]);
	glUseProgram(gProgram[4]);
	
	glUseProgram(gProgram[5]);

	// Create the shaders for both programs
	vs1 = createVS("vertSky.glsl");
	fs1 = createFS("fragSky.glsl");

	vs2 = createVS("vertRedCheck.glsl");
	fs2 = createFS("fragRedCheck.glsl");

	vs3 = createVS("vert.glsl");
	fs3 = createFS("frag.glsl");

	vs4 = createVS("vert.glsl");
	fs4 = createFS("frag.glsl");

	vs5 = createVS("vertGround.glsl");
	fs5 = createFS("fragGround.glsl");

	createVS1(textProg,"vert_text.glsl");
	createFS1(textProg,"frag_text.glsl");
	

	// Attach the shaders to the programs

	glAttachShader(gProgram[0], vs3);
	glAttachShader(gProgram[0], fs3);
	glAttachShader(gProgram[1], vs1);
	glAttachShader(gProgram[1], fs1);

	glAttachShader(gProgram[2], vs2);
	glAttachShader(gProgram[2], fs2);

	glAttachShader(gProgram[3], vs4);
	glAttachShader(gProgram[3], fs4);

	glAttachShader(gProgram[4], vs5);
	glAttachShader(gProgram[4], fs5);


	glBindAttribLocation(textProg, 2, "vertex");

	// Link the programs

	glLinkProgram(gProgram[0]);
	GLint status;
	glGetProgramiv(gProgram[0], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	glLinkProgram(gProgram[1]);
	glGetProgramiv(gProgram[1], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	glLinkProgram(gProgram[2]);
	glGetProgramiv(gProgram[2], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	glLinkProgram(gProgram[3]);
	glGetProgramiv(gProgram[3], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	glLinkProgram(gProgram[4]);
	glGetProgramiv(gProgram[4], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	glLinkProgram(textProg);
	glGetProgramiv(textProg, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}

	/**glUseProgram(gProgram[0]);
	gIntensityLoc = glGetUniformLocation(gProgram[0], "intensity");
    cout << "gIntensityLoc = " << gIntensityLoc << endl;
    glUniform1f(gIntensityLoc, gIntensity);**/

}

void drawModel();

void initVBO()
{
    // Setup VAO and VBO for bunny
    glGenVertexArrays(1, &vao);
    assert(vao > 0);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    int indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);

    GLfloat* vertexData = new GLfloat[gVertices.size() * 3];
    GLfloat* normalData = new GLfloat[gNormals.size() * 3];
    GLuint* indexData = new GLuint[gFaces.size() * 3];

    float minX = 1e6, maxX = -1e6;
    float minY = 1e6, maxY = -1e6;
    float minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices.size(); ++i)
    {
        vertexData[3 * i] = gVertices[i].x;
        vertexData[3 * i + 1] = gVertices[i].y;
        vertexData[3 * i + 2] = gVertices[i].z;

        minX = std::min(minX, gVertices[i].x);
        maxX = std::max(maxX, gVertices[i].x);
        minY = std::min(minY, gVertices[i].y);
        maxY = std::max(maxY, gVertices[i].y);
        minZ = std::min(minZ, gVertices[i].z);
        maxZ = std::max(maxZ, gVertices[i].z);
    }
    for (int i = 0; i < gNormals.size(); ++i)
    {
        normalData[3 * i] = gNormals[i].x;
        normalData[3 * i + 1] = gNormals[i].y;
        normalData[3 * i + 2] = gNormals[i].z;
    }

    for (int i = 0; i < gFaces.size(); ++i)
    {
        indexData[3 * i] = gFaces[i].vIndex[0];
        indexData[3 * i + 1] = gFaces[i].vIndex[1];
        indexData[3 * i + 2] = gFaces[i].vIndex[2];
    }

    // ... (similar code for normalData and indexData as before)

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // Done copying to GPU memory; can free now from CPU memory
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));


	glBindVertexArray(0); // Unbind the VAO


// Reset active program to bunny's program
    //glUseProgram(gProgram[0]);

    // Setup VAO and VBO for ground

    glGenVertexArrays(1, &vaoSky);
    assert(vaoSky > 0);
    glBindVertexArray(vaoSky);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
    //assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBufferSky);
    glGenBuffers(1, &gIndexBufferSky);

    assert(gVertexAttribBufferSky > 0 && gIndexBufferSky > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBufferSky);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferSky);

    gVertexDataSizeInBytes = gVertices2.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals2.size() * 3 * sizeof(GLfloat);
    indexDataSizeInBytes = gFaces2.size() * 3 * sizeof(GLuint);

    vertexData = new GLfloat[gVertices2.size() * 3];
    normalData = new GLfloat[gNormals2.size() * 3];
    indexData = new GLuint[gFaces2.size() * 3];

    minX = 1e6, maxX = -1e6;
    minY = 1e6, maxY = -1e6;
    minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices2.size(); ++i)
    {
        vertexData[3 * i] = gVertices2[i].x;
        vertexData[3 * i + 1] = gVertices2[i].y;
        vertexData[3 * i + 2] = gVertices2[i].z;

        minX = std::min(minX, gVertices2[i].x);
        maxX = std::max(maxX, gVertices2[i].x);
        minY = std::min(minY, gVertices2[i].y);
        maxY = std::max(maxY, gVertices2[i].y);
        minZ = std::min(minZ, gVertices2[i].z);
        maxZ = std::max(maxZ, gVertices2[i].z);
    }
    for (int i = 0; i < gNormals2.size(); ++i)
    {
        normalData[3 * i] = gNormals2[i].x;
        normalData[3 * i + 1] = gNormals2[i].y;
        normalData[3 * i + 2] = gNormals2[i].z;
    }

    for (int i = 0; i < gFaces2.size(); ++i)
    {
        indexData[3 * i] = gFaces2[i].vIndex[0];
        indexData[3 * i + 1] = gFaces2[i].vIndex[1];
        indexData[3 * i + 2] = gFaces2[i].vIndex[2];
    }
	GLuint gTextureData2SizeInBytes = gTextures2.size() * 2 * sizeof(GLfloat);
	GLfloat* textureData2 = new GLfloat[gTextures2.size() * 2];
	
		for (int i = 0; i < gTextures2.size(); ++i)
    {
        textureData2[2 * i] = gTextures2[i].u;  // assuming u is the first texture coordinate
    	textureData2[2 * i + 1] = gTextures2[i].v;  
    }
    // ... (similar code for normalData and indexData as before)

glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes + gTextureData2SizeInBytes, 0, GL_STATIC_DRAW);
glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, gTextureData2SizeInBytes, textureData2);
delete[] vertexData;
delete[] normalData;
delete[] textureData2;

// Allocate space for index data
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, 0, GL_STATIC_DRAW);
glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexDataSizeInBytes, indexData);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes + gNormalDataSizeInBytes));  // Texture attribute

	glBindVertexArray(0);
	glGenVertexArrays(1, &vaoCube);
    assert(vaoCube > 0);
    glBindVertexArray(vaoCube);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBufferCube);
    glGenBuffers(1, &gIndexBufferCube);

    assert(gVertexAttribBufferCube > 0 && gIndexBufferCube > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBufferCube);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferCube);

    gVertexDataSizeInBytes = gVertices3.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals3.size() * 3 * sizeof(GLfloat);
    indexDataSizeInBytes = gFaces3.size() * 3 * sizeof(GLuint);

    vertexData = new GLfloat[gVertices3.size() * 3];
    normalData = new GLfloat[gNormals3.size() * 3];
    indexData = new GLuint[gFaces3.size() * 3];

    minX = 1e6, maxX = -1e6;
    minY = 1e6, maxY = -1e6;
    minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices3.size(); ++i)
    {
        vertexData[3 * i] = gVertices3[i].x;
        vertexData[3 * i + 1] = gVertices3[i].y;
        vertexData[3 * i + 2] = gVertices3[i].z;

        minX = std::min(minX, gVertices3[i].x);
        maxX = std::max(maxX, gVertices3[i].x);
        minY = std::min(minY, gVertices3[i].y);
        maxY = std::max(maxY, gVertices3[i].y);
        minZ = std::min(minZ, gVertices3[i].z);
        maxZ = std::max(maxZ, gVertices3[i].z);
    }
    for (int i = 0; i < gNormals3.size(); ++i)
    {
        normalData[3 * i] = gNormals3[i].x;
        normalData[3 * i + 1] = gNormals3[i].y;
        normalData[3 * i + 2] = gNormals3[i].z;
    }

    for (int i = 0; i < gFaces3.size(); ++i)
    {
        indexData[3 * i] = gFaces3[i].vIndex[0];
        indexData[3 * i + 1] = gFaces3[i].vIndex[1];
        indexData[3 * i + 2] = gFaces3[i].vIndex[2];
    }

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));
}
void parseTextures() {
    int width, height, channels;
	//stbi_set_flip_vertically_on_load(1);
    unsigned char* skyImage = stbi_load("sky.jpg", &width, &height, &channels, 3); // 3 channels for RGB

    if (!skyImage) {
        std::cerr << "Failed to load texture image: sky.jpg" << std::endl;
        exit(EXIT_FAILURE);
    }

    //glGenSamplers(1, &skySampler);
    glUseProgram(gProgram[1]);
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &skyTexture);
	glBindTexture(GL_TEXTURE_2D, skyTexture);

	glUniform1i(glGetUniformLocation(gProgram[1], "skySampler"), 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyImage);

    //glSamplerParameteri(skySampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glSamplerParameteri(skySampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glSamplerParameteri(skySampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glSamplerParameteri(skySampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //glBindSampler(0, skySampler);

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    
	//glGenerateMipmap(GL_TEXTURE_2D);

    
	
	stbi_image_free(skyImage); // Free the image data
}

void init()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate a random number between 1, 2, and 3
    choosenCubeIndex = (std::rand() % 3);

	ParseObj("bunny.obj",0);
	ParseObj("quad.obj",1);
	ParseObj("cube.obj",2);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);
	//initShaders2();
	initShaders();
	parseTextures();
	initFonts(640,480);
	initVBO();
}

void drawModel()
{
	GLuint cube0program, cube1program, cube2program;
    // Draw the bunny model
    glUseProgram(gProgram[0]); // Use the bunny shader program
    glUniformMatrix4fv(projectionMatrixLoc[0], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[0], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[0], 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniform3fv(eyePosLoc[0], 1, glm::value_ptr(eyePos));


    glBindVertexArray(vao); // Use the bunny VAO
    glDrawElements(GL_TRIANGLES, gFaces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO


	//draw the sky
	glUseProgram(gProgram[1]); // Use the ground shader program
    glUniformMatrix4fv(projectionMatrixLoc[1], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[1], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[1], 1, GL_FALSE, glm::value_ptr(modelingMatrixSky));
    glUniform3fv(eyePosLoc[1], 1, glm::value_ptr(eyePos));

	//glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, skyTexture);

    glBindVertexArray(vaoSky); // Use the ground VAO
    glDrawElements(GL_TRIANGLES, gFaces2.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO
    glUseProgram(0); // Unuse the shader program

    // Draw the ground
    glUseProgram(gProgram[4]); // Use the ground shader program
	cubeZLocation= glGetUniformLocation(gProgram[4], "cubeZGeneral");
	glUniform1f(cubeZLocation, cubeZinfinite);
    glUniformMatrix4fv(projectionMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(modelingMatrix2));
    glUniform3fv(eyePosLoc[4], 1, glm::value_ptr(eyePos));

	//glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, skyTexture);

    glBindVertexArray(vaoSky); // Use the ground VAO
    glDrawElements(GL_TRIANGLES, gFaces2.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO
    glUseProgram(0); // Unuse the shader program

	switch (choosenCubeIndex)
	{
	case 0:
		cube0program = gProgram[0];
		cube1program = gProgram[2];
		cube2program = gProgram[2];
		break;
	case 1:
		cube0program = gProgram[2];
		cube1program = gProgram[0];
		cube2program = gProgram[2];
		break;
	case 2:
		cube0program = gProgram[2];
		cube1program = gProgram[2];
		cube2program = gProgram[0];
		break;
	
	default:
		break;
	}


	// Draw the cube objects
    glUseProgram(cube0program); // Use the bunny shader program
    glUniformMatrix4fv(projectionMatrixLoc[2], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[2], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[2], 1, GL_FALSE, glm::value_ptr(modelingMatrix3));
    glUniform3fv(eyePosLoc[2], 1, glm::value_ptr(eyePos));



    glBindVertexArray(vaoCube); // Use the bunny VAO
    glDrawElements(GL_TRIANGLES, gFaces3.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO

	// Draw the cube objects
    glUseProgram(cube1program); // Use the bunny shader program
    glUniformMatrix4fv(projectionMatrixLoc[3], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[3], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[3], 1, GL_FALSE, glm::value_ptr(modelingMatrix4));
    glUniform3fv(eyePosLoc[3], 1, glm::value_ptr(eyePos));


    glBindVertexArray(vaoCube); // Use the bunny VAO
    glDrawElements(GL_TRIANGLES, gFaces3.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO

	// Draw the cube objects
    glUseProgram(cube2program); // Use the bunny shader program
    glUniformMatrix4fv(projectionMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(viewingMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniformMatrix4fv(modelingMatrixLoc[4], 1, GL_FALSE, glm::value_ptr(modelingMatrix5));
    glUniform3fv(eyePosLoc[4], 1, glm::value_ptr(eyePos));


    glBindVertexArray(vaoCube); // Use the bunny VAO
    glDrawElements(GL_TRIANGLES, gFaces3.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the VAO**/

	/**glAttachShader(gProgram[4], vs2);
	glAttachShader(gProgram[4], fs2);
	GLint status;
	glLinkProgram(gProgram[4]);
	glGetProgramiv(gProgram[4], GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Program link failed" << endl;
		exit(-1);
	}**/


}
void display2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    scoreText = "Score: " + std::to_string(score);
	renderText(scoreText, 0, 0, 1, glm::vec3(0, 1, 1));
}
void display()
{
	static float currentY = -1.5f;  // Replace 0.0f with your actual minimum Y value


	float minZ = -0.8f;
	
	static bool scoreReady = true;
	if (slidingLeft)
	{
		if(bunnyVerPos > -2.8f && running)
		{
			bunnyVerPos -= vertical_speed;
		}
		
	}
	else if(slidingRight){
		if(bunnyVerPos < +2.8f && running)
		{
			bunnyVerPos += vertical_speed;
		}
	}
	if (running)
	{
		score+= (int) (2.6f*speed);
	}
	
	
	//currentY += 1.f;
	
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



	float angleRad = (float)(angle / 180.0) * M_PI;

	// Compute the modeling matrix 
    glm::mat4 matT = glm::translate(glm::mat4(1.0), glm::vec3(bunnyVerPos, currentY, -minZ - 3.f));
    glm::mat4 matS = glm::scale(glm::mat4(1.0), glm::vec3(0.4, 0.4, 0.4));
    glm::mat4 matR = glm::rotate<float>(glm::mat4(1.0), deadAngle, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 matRz = glm::rotate(glm::mat4(1.0), -angleRad, glm::vec3(0.0, 1.0, 0.0));
    modelingMatrix = matT * matR * matRz * matS;
	glm::mat4 matTQuad = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.f, -minZ - 20.f));
	glm::mat4 matSQuad = glm::scale(glm::mat4(1.0),glm::vec3(6, 50, 30)); //glm::vec3(4, 10, 30));
	glm::mat4 matRx = glm::rotate<float>(glm::mat4(1.0), -1.4f, glm::vec3(1.0, 0.0, 0.0)); // Add this line for rotation around x-axis
	glm::mat4 matRy = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0)); // Adjust the angle and axis as needed
	matRz = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 0.0, 1.0)); // Adjust the angle and axis as needed
	modelingMatrix2 = matTQuad * matRx * matRy * matRz * matSQuad; // Include the new rotation matrix
	glm::mat4 matTCube = glm::translate(glm::mat4(1.0), glm::vec3(3.5f, cubeY,cubeZ));  //-50.f+ -currentY*30));
	glm::mat4 matSCube = glm::scale(glm::mat4(1.0),glm::vec3(0.5, 0.3, 1.5)); //glm::vec3(0.5, 0.3, 1.5));
	glm::mat4 matRxCube = glm::rotate<float>(glm::mat4(1.0), -1.2f, glm::vec3(1.0, 0.0, 0.0)); // Add this line for rotation around x-axis
	matRy = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0)); // Adjust the angle and axis as needed
	matRz = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 0.0, 1.0)); // Adjust the angle and axis as needed
	modelingMatrix3 = matTCube * matRxCube * matRy * matRz * matSCube; // Include the new rotation matrix
	glm::mat4 matTCube2 = glm::translate(glm::mat4(1.0), glm::vec3(0.f, cubeY, cubeZ1));
	glm::mat4 matSCube2 = glm::scale(glm::mat4(1.0),glm::vec3(0.5, 0.3, 1.5)); //glm::vec3(0.5, 0.3, 1.5));
	glm::mat4 matRxCube2 = glm::rotate<float>(glm::mat4(1.0), -1.2f, glm::vec3(1.0, 0.0, 0.0)); // Add this line for rotation around x-axis
	glm::mat4 matRyCube2 = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0)); // Adjust the angle and axis as needed
	glm::mat4 matRzCube2 = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 0.0, 1.0)); // Adjust the angle and axis as needed
	modelingMatrix4 = matTCube2 * matRxCube2 * matRyCube2 * matRzCube2 * matSCube2; // Include the new rotation matrix
	glm::mat4 matTCube3 = glm::translate(glm::mat4(1.0), glm::vec3(-3.5f, cubeY,cubeZ2));
	glm::mat4 matSCube3 = glm::scale(glm::mat4(1.0),glm::vec3(0.5, 0.3, 1.5)); //glm::vec3(0.5, 0.3, 1.5));
	glm::mat4 matRxCube3 = glm::rotate<float>(glm::mat4(1.0), -1.2f, glm::vec3(1.0, 0.0, 0.0)); // Add this line for rotation around x-axis
	glm::mat4 matRyCube3 = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0)); // Adjust the angle and axis as needed
	glm::mat4 matRzCube3 = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 0.0, 1.0)); // Adjust the angle and axis as needed
	modelingMatrix5 = matTCube3 * matRxCube3 * matRyCube3 * matRzCube3 * matSCube3; // Include the new rotation matrix
	glm::mat4 matTSky = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 2.f,-80.f));
	glm::mat4 matSSky = glm::scale(glm::mat4(1.0),glm::vec3(120, 100, 100)); //glm::vec3(0.5, 0.3, 1.5));
	glm::mat4 matRxSky = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(1.0, 0.0, 0.0)); // Add this line for rotation around x-axis
	glm::mat4 matRySky = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 1.0, 0.0)); // Adjust the angle and axis as needed
	glm::mat4 matRzSky = glm::rotate<float>(glm::mat4(1.0), 0.0f, glm::vec3(0.0, 0.0, 1.0)); // Adjust the angle and axis as needed
	modelingMatrixSky = matTSky * matRxSky * matRySky * matRzSky * matSSky; // Include the new rotation matrix
	for (int i = 0; i < 5; ++i)
	{
		modelingMatrixLoc[i] = glGetUniformLocation(gProgram[i], "modelingMatrix");
		viewingMatrixLoc[i] = glGetUniformLocation(gProgram[i], "viewingMatrix");
		projectionMatrixLoc[i] = glGetUniformLocation(gProgram[i], "projectionMatrix");
		eyePosLoc[i] = glGetUniformLocation(gProgram[i], "eyePos");
	}
	scoreText = "Score: " + std::to_string(score);
	renderText(scoreText, 0, 450, 1, textColor);
	drawModel();

	//renderText("CENG 477 - 2022", 0, 0, 1, glm::vec3(0, 1, 1));
	
	//angle += speed*15;

	
	if (running)
	{
		cubeZ +=speed;
		cubeZGeneral +=speed;
		cubeZinfinite +=speed;
		cubeZ1 +=speed;
		cubeZ2 +=speed;
		cubeY -= speed/5;

		if (celebrating)
		{
			angle += speed*15;
		}
		if (angle > 450)
		{
			celebrating = false;
			scoreReady = true;
			angle = 90;
		}
		
		
		if (cubeZ >= -5.f)
		{
			if (bunnyVerPos < -1.5f)
			{
				
				cubeZ2 = 5.f;
				if (choosenCubeIndex == 2)
				{
					if (scoreReady)
					{
						score +=1000;
						scoreReady = false;
					}
					
					celebrating = true;	
					
				}
				else{
					textColor = glm::vec3(1,0,0);
					running = false;
					currentY = -1.5f;
					deadAngle = -1.57f;
				}
			}
			else if(bunnyVerPos > -0.8f && bunnyVerPos < 0.8f){
				
				cubeZ1 = 5.f;
				if (choosenCubeIndex == 1)
				{
					celebrating = true;	
					if (scoreReady)
					{
						score +=1000;
						scoreReady = false;
					}
				}
				else{
					textColor = glm::vec3(1,0,0);
					running = false;
					currentY = -1.5f;
					deadAngle = -1.57f;
				}
			}
			else if(bunnyVerPos > 1.5f){
				
				cubeZ = 5.f;
				if (choosenCubeIndex == 0)
				{
					celebrating = true;	
					if (scoreReady)
					{
						score +=1000;
						scoreReady = false;
					}
				}
				else{
					textColor = glm::vec3(1,0,0);
					running = false;
					currentY = -1.5f;
					deadAngle = -1.57f;
				}
			} 
		}
		if (cubeZGeneral >= 0.f)
		{
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			choosenCubeIndex = (std::rand() % 3);
			cubeZ = cubeZinitial;
			cubeZ1 = cubeZinitial;
			cubeZ2 = cubeZinitial;
			cubeZGeneral = cubeZinitial;
			cubeY = cubeYinitial;
			speed += 0.03f;
			vertical_speed += 0.01f;
		}

		if (goingUp){
			if (currentY < -1.f)
			{
				currentY += (2*vertical_speed/3);
			}
			else{
				goingUp = false;
			}
			
		}
		else{
			if (currentY > -1.5f)
			{
				currentY -= (2*vertical_speed/3);
			}
			else{
				goingUp = true;
			}
			
		}
	}
	
	
	

	//currentY += 0.1f;
	//glfwPollEvents();
	

	// Draw the scene**/
	

	
}

void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);

	// Use perspective projection
	float fovyRad = (float)(90.0 / 180.0) * M_PI;
	projectionMatrix = glm::perspective(fovyRad, w / (float)h, 1.0f, 100.0f);

	// Assume default camera position and orientation (camera is at
	// (0, 0, 0) with looking at -z direction and its up vector pointing
	// at +y direction)
	// 
	//viewingMatrix = glm::mat4(1);
	viewingMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS)){
			slidingLeft = true;

	}
	
	
	if (key == GLFW_KEY_D && (action == GLFW_PRESS)){
			slidingRight = true;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS){
		if (!running)
		{
			cubeZ = cubeZinitial;
			cubeZ1 = cubeZinitial;
			cubeZ2 = cubeZinitial;
			cubeZGeneral = cubeZinitial;
			cubeY = cubeYinitial;
			running = true;
			deadAngle = 0;
			textColor = glm::vec3(1, 1, 0);
			score = 0;
			speed = 0.4f;
			vertical_speed = 0.15f;
			bunnyVerPos = 0;
			slidingRight = false;
			slidingLeft = false;
		}	
	}
	if (action == GLFW_RELEASE){
		if (key == GLFW_KEY_A)
        {
            slidingLeft = false;
        }
        else if (key == GLFW_KEY_D)
        {
            slidingRight = false;
        }
	}
}

void mainLoop(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		//display2();
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
	//Initialize OpenGL window
    GLFWwindow* window;
    if (!glfwInit()) {
        exit(-1);
    }

    // Set OpenGL context and version hints
    /**glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);**/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create GLFW window by specified sizes
    gWidth = 640, gHeight = 480;
    window = glfwCreateWindow(gWidth, gHeight, "Bunny Rendering", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    // Make the current window the OpenGL context
    glfwMakeContextCurrent(window);
    // Enable V-Sync
    glfwSwapInterval(1);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Get and display renderer and version information (show them in window title)
    char rendererInfo[512] = { 0 };
    strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER));
    strcat(rendererInfo, " - ");
    strcat(rendererInfo, (const char*)glGetString(GL_VERSION));
    glfwSetWindowTitle(window, rendererInfo);

    // Parse objects, initialize buffers and shaders
    init();

    // Set callback functions
    glfwSetKeyCallback(window, keyboard);
    glfwSetWindowSizeCallback(window, reshape);

    // Set initial window size and projection matrix
    reshape(window, gWidth, gHeight); // need to call this once ourselves

    // Main rendering loop, redrawing occurs here
    mainLoop(window); // this does not return unless the window is closed

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

