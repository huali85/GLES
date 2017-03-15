//
// Created by lihua_000 on 3/4/2017.
//

#include "Mesh.h"
#include <string.h>
#include <vector>
#include "glm/glm.hpp"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

bool Mesh::CreateFromFile(const char* filename, Mesh& mesh) {
    void* file_data = nullptr;
    size_t file_size;
    if (!LoadFile(filename, &file_data, &file_size)) {
        return false;
    }
    const char* ext = strrchr(filename, '.');
    bool result = CreateFromMemsory((const char*)file_data, file_size, mesh, ext);
    free(file_data);
    return result;
}

bool Mesh::CreateFromMemsory(const char* buf, unsigned int buf_size, Mesh& mesh, const char* hint) {
    const aiScene* scene = aiImportFileFromMemory(buf, buf_size, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices, hint);
    if (!scene) {
        LOGE("Open Asset Importer could not load scene.\n");
        return false;
    }
    aiVector3D zero(0.0f, 0.0f, 0.0f);
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<GLushort> indices;
    unsigned int vertex_count = 0;
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* current_mesh = scene->mMeshes[i];
        bool has_normal = current_mesh->HasNormals();
        bool has_uv = current_mesh->HasTextureCoords(0);
        // Add vertices
        for (int j = 0; j < current_mesh->mNumVertices; ++j) {
            glm::vec3 pos(current_mesh->mVertices[j].x, current_mesh->mVertices[j].y, current_mesh->mVertices[j].z);
            vertices.push_back(pos);
            if (has_normal) {
                glm::vec3 normal(current_mesh->mNormals[j].x, current_mesh->mNormals[j].y, current_mesh->mNormals[j].z);
                normals.push_back(normal);
            }
            if (has_uv) {
                glm::vec2 uv(current_mesh->mTextureCoords[0][j].x, current_mesh->mTextureCoords[0][j].y);
                uvs.push_back(uv);
            }
        }
        /*
         * Add all the indices in the mesh to our array.
         * Indices are listed in the Open Asset importer relative to the mesh they are in.
         * Because we are adding all vertices from all meshes to one array we must add an offset
         * to the indices to correct for this.
         */
        for (unsigned int j = 0; j < current_mesh->mNumFaces; ++j) {
            const aiFace& face = current_mesh->mFaces[j];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0] + vertex_count);
            indices.push_back(face.mIndices[1] + vertex_count);
            indices.push_back(face.mIndices[2] + vertex_count);
        }
        /* Keep track of number of vertices loaded so far to use as an offset for the indices. */
        vertex_count += current_mesh->mNumVertices;
    }

    GL_CHECK(glGenVertexArrays(1, &mesh.vao));
    GL_CHECK(glBindVertexArray(mesh.vao));
    {
        int num_vbo = 2;
        if (!normals.empty()) ++num_vbo;
        if (!uvs.empty()) ++num_vbo;
        mesh.vbo = new GLuint[num_vbo];
        mesh.num_vbo = num_vbo;
        GL_CHECK(glGenBuffers(num_vbo, mesh.vbo));
    }
    int vbo_index = 0;
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[vbo_index]));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(vbo_index));
    GL_CHECK(glVertexAttribPointer(vbo_index, 3, GL_FLOAT, GL_FALSE, 0, 0));
    if (!normals.empty()) {
        ++vbo_index;
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[vbo_index]));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(vbo_index));
        GL_CHECK(glVertexAttribPointer(vbo_index, 3, GL_FLOAT, GL_FALSE, 0, 0));
    }
    if (!uvs.empty()) {
        ++vbo_index;
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[vbo_index]));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW));
        GL_CHECK(glEnableVertexAttribArray(vbo_index));
        GL_CHECK(glVertexAttribPointer(vbo_index, 2, GL_FLOAT, GL_FALSE, 0, 0));
    }
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[++vbo_index]));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW));
    GL_CHECK(glBindVertexArray(0));
    mesh.index_count = indices.size();
    return true;
}

bool Mesh::CreateQuad(Mesh& mesh) {
    /* Example:
     *  z   D __________ C
     *  .    |        / |
     * / \   |     /    |
     *  |    |  /       |
     *  |    |/_________|
     *  |   A            B
     *  |----------> x
     */
    /* Coordinates of a points: A, B, C and D as shown in a schema above. */
    const glm::vec3 vertices[] = {
        {-1.0f, 0.0f,  1.0f},
        { 1.0f, 0.0f,  1.0f},
        { 1.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, -1.0f}
    };
    const glm::vec3 normals[] = {
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };
    const glm::vec2 uvs[] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };
    const GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    mesh.vbo = new GLuint[4];
    mesh.num_vbo = 4;
    glGenBuffers(4, mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    mesh.index_count = 6;
    return true;
}

bool Mesh::CreateCube(Mesh& mesh) {
    const GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f,  0.5f, 0.5f,
            0.5f,  0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
    };
    const GLfloat normals[] = {
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
    };
    const GLfloat uvs[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };
    const GLushort indices[] = {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 15, 14,
            12, 14, 13,
            16, 17, 18,
            16, 18, 19,
            20, 23, 22,
            20, 22, 21
    };
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    mesh.vbo = new GLuint[4];
    mesh.num_vbo = 4;
    glGenBuffers(4, mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    mesh.index_count = 36;
    return true;
}

bool Mesh::CreateSphere(Mesh &mesh) {
    std::string sphere = "s 0 0 0 10";
    return CreateFromMemsory(sphere.c_str(), sphere.length(), mesh, ".nff");
}

void Mesh::Destroy(Mesh &mesh) {
    if (mesh.num_vbo) {
        glDeleteBuffers(mesh.num_vbo, mesh.vbo);
        glDeleteVertexArrays(1, &mesh.vao);
        delete[] mesh.vbo;
        mesh.vbo = nullptr;
        mesh.num_vbo = 0;
    }
}

void Mesh::Draw(Mesh &mesh) {
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_SHORT, 0);
}