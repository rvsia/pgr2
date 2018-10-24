#include "graphicalobject.h"


GraphicalObject::GraphicalObject(QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string colorAttributeName)
{
    /// Tento klasicky zpusob ulozeni je trochu mene efektivni, nez
    /// m_program(program) v hlavicce, ale tady to prilis nevadi.
    /// Nicmene muzete udelat oboje.

    /// Ulozim ukazatele na pomocne tridy
    m_program = program;
    m_functions = functions;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_normalAttributeName = colorAttributeName;
    m_positionAttributeName = positionAttributeName;
}
