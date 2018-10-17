/*
 * Tento kod byl upraven tak, aby vyhovoval ucelum kurzu PG2
 * Autor: David Prochazka
 */
/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Tutorial 4: 3d engine - 3ds models loader
 * 
 * File header: tutorial4.h
 *  
 */

/*
Linux port by Panteleakis Ioannis
mail: pioann@csd.auth.gr

just run: make and you are done.
of course you may need to change the makefile
*/

#include <QtGui/QOpenGLShaderProgram>


/**********************************************************
 *
 * TYPES DECLARATION
 *
 *********************************************************/

#define MAX_VERTICES 8000 // Max number of vertices (for each object)
#define MAX_POLYGONS 8000 // Max number of polygons (for each object)

// Our vertex type
typedef struct{
    GLfloat x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    GLint a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    GLfloat u,v;
}mapcoord_type;

// The object type
typedef struct {
	char name[20];
    
    GLint vertices_qty;
    GLint polygons_qty;

    vertex_type vertex[MAX_VERTICES]; 
    polygon_type polygon[MAX_POLYGONS];
    GLuint indices[MAX_POLYGONS*3];
    mapcoord_type mapcoord[MAX_VERTICES];
    GLuint id_texture;
} obj_type, *obj_type_ptr;
