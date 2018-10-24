#include <QOpenGLBuffer>
#include "graphicalobject.h"

struct Vertex{
    GLfloat position[3];
    GLfloat color[3];
    GLfloat normal[3];
    GLfloat texturecoord[2];
};

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Cube: public GraphicalObject{
private:
    /// Buffer s vrcholy objektu
    QOpenGLBuffer m_vertexBuffer;
public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Cube(GLfloat size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
         const std::string positionAttributeName, const std::string colorAttributeName);
    /// Provede vykresleni objektu
    void draw();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
	~Cube();
};
