#include "3dmodel.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
CModel3DS::CModel3DS(std::string file,
                     QOpenGLShaderProgram* program,
                     const std::string positionAttributeName,
                     const std::string normalAttributeName)
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_normalAttributeName = normalAttributeName;
    m_positionAttributeName = positionAttributeName;

    /// Inicializuje pocet kolik ma objekt trojuhelniku
    m_TotalFaces = 0;

    /// Nactu 3D model ze souboru
    m_model = lib3ds_file_load(file.c_str());

    /// Pokud se model nepovedlo nacist vyhodim zakladni vyjimku
    if(!m_model)
    {
        throw strcat("Unable to load ", file.c_str());
    }
}

/// Pomocna metoda, ktera spocte pocet projuhelniku meshe
void CModel3DS::GetFaces()
{
    assert(m_model != NULL);

    m_TotalFaces = 0;
    Lib3dsMesh * mesh;
    // Loop through every mesh
    for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next)
    {
        // Add the number of faces this mesh has to the total faces
        m_TotalFaces += mesh->faces;
    }
}

/// Metoda, kterou musite rucne zavola pro vytvoreni VBO
void CModel3DS::CreateVBO()
{
    assert(m_model != NULL);

    // Calculate the number of faces we have in total
    GetFaces();

    // Allocate memory for our vertices and normals
    Lib3dsVector * vertices = new Lib3dsVector[m_TotalFaces * 3];
    Lib3dsVector * normals = new Lib3dsVector[m_TotalFaces * 3];

    Lib3dsMesh * mesh;
    unsigned int FinishedFaces = 0;
    // Loop through all the meshes
    for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next)
    {
        lib3ds_mesh_calculate_normals(mesh, &normals[FinishedFaces*3]);
        // Loop through every face
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++)
        {
            Lib3dsFace * face = &mesh->faceL[cur_face];
            for(unsigned int i = 0;i < 3;i++)
            {
                memcpy(&vertices[FinishedFaces*3 + i], mesh->pointL[face->points[ i ]].pos, sizeof(Lib3dsVector));
            }
            FinishedFaces++;
        }
    }

    // Generate a Vertex Buffer Object and store it with our vertices
    glGenBuffers(1, &m_VertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, vertices, GL_STATIC_DRAW);

    // Generate another Vertex Buffer Object and store the normals in it
    glGenBuffers(1, &m_NormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, normals, GL_STATIC_DRAW);

    // Clean up our allocated memory
    delete vertices;
    delete normals;

    // We no longer need lib3ds
    lib3ds_file_free(m_model);
    m_model = NULL;
}

/// Metoda pro vykresleni 3D modelu
void CModel3DS::Draw() const{
    assert(m_TotalFaces != 0);

    m_program->bind();

    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme

    /// Nabinduju potrebne buffery
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, 0, 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);

    m_program->enableAttributeArray(m_normalAttributeName.c_str());
    m_program->setAttributeBuffer(m_normalAttributeName.c_str(), GL_FLOAT, 0, 3, 0);

    /// Provedeme samotne vykresleni
    glDrawArrays(GL_TRIANGLES, 0, m_TotalFaces * 3);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_normalAttributeName.c_str());
}


CModel3DS::~CModel3DS(){


}
