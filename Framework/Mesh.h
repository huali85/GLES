//
// Created by lihua_000 on 3/4/2017.
//

#ifndef MESH_H
#define MESH_H

#include "Platform.h"

namespace Mesh
{
    struct Mesh {
        GLuint vao = 0;
        GLuint* vbo = nullptr;
        int index_count = 0;
        int num_vbo = 0;
        //Mesh() : vao(0), vbo(nullptr), index_count(0), num_vbo(0) {}
    };

    bool CreateFromFile(const char* filename, Mesh& mesh);
    bool CreateFromMemsory(const char* buf, unsigned int buf_size, Mesh& mesh,const char* hint);
    bool CreateQuad(Mesh& mesh);
    bool CreateCube(Mesh& mesh);
    bool CreateSphere(Mesh& mesh);
    void Destroy(Mesh& mesh);
    void Draw(Mesh& mesh);
}

typedef Mesh::Mesh Mesh_t;

#endif //MESH_H
