//
// Created by lihua_000 on 3/13/2017.
//

#include <Game.h>
#include <Mesh.h>
#include <FrameBuffer.h>
#include <Shader.h>
#include <Texture.h>
#include <Util.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

class Example : public Game {
public:
    Example();
    virtual ~Example() override;
    virtual bool Init() override;

private:
    virtual void Update(float dt) override;
    virtual void Draw() override;
    virtual void Resize(int width, int height) override;

    bool InitShaders();
    void RenderDepthToFBO();

    // Declare shader variables
    GLuint _depth_shader;
    GLint  _depth_lightMVP;

    GLuint _light_shader;
    GLint  _light_M;
    GLint  _light_MVP;
    GLint  _light_Normal;
    GLint  _light_BiasedLightMVP;
    GLint  _light_ambient;
    GLint  _light_diffuse;
    GLint  _light_specular;
    GLint  _light_eyePosWS;
    GLint  _light_lightPosWS;
    GLint  _light_shadowMap;

    // Declare mesh variables
    Mesh_t _mesh;
    Mesh_t _floor;

    int _shadow_map_size;
    FrameBuffer_t _fb;

    // Declare transformation related variables
    glm::mat4 _cam_proj;
    glm::mat4 _light_proj;

    glm::mat4 _cam_view;
    glm::mat4 _floor_MV;
    glm::mat4 _floor_MVP;
    glm::mat3 _floor_Normal;
    glm::mat4 _mesh_MV;
    glm::mat4 _mesh_MVP;
    glm::mat3 _mesh_Normal;

    glm::vec3 _cam_pos;
    glm::vec3 _light_pos;
    glm::vec3 _light_lookAt;
    glm::vec3 _light_up;

    glm::mat4 _mesh_M;
    glm::mat4 _mesh_lightMVP;
    glm::mat4 _floor_M;
    glm::mat4 _floor_lightMVP;

    glm::mat4 _bias;
    Util::OverlayTexture* _overlay_texture;
};

Game* CreateGame() {
    Game* game = new Example();
    if (!game->Init()) {
        SAFE_DELETE(game);
    }
    return game;
}

void Example::Update(float dt) {
    // Process input
    if (_right_input.x != 0.0f) {
        float angle = dt * _right_input.x;
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec4 new_pos = rotate * glm::vec4(_light_pos, 1.0f);
        _light_pos = glm::vec3(new_pos.x, new_pos.y, new_pos.z);
    }
    if (_right_input.y != 0.0f) {
        float angle = dt * _right_input.y;
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec4 new_pos = rotate * glm::vec4(_light_pos, 1.0f);
        _light_pos = glm::vec3(new_pos.x, new_pos.y, new_pos.z);
    }

    double current_time = _timer.GetTime();
    // Setup the mesh's camera relative model view projection matrix
    glm::mat4 mesh_scale     = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 mesh_rotate    = glm::rotate(glm::mat4(1.0f), (float)current_time, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mesh_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    _mesh_M                  = mesh_translate * mesh_rotate * mesh_scale;
    _mesh_MV                 = _cam_view * _mesh_M;
    _mesh_MVP                = _cam_proj * _mesh_MV;
    _mesh_Normal             = glm::inverseTranspose(glm::mat3(_mesh_MV));

    // Setup the floor and mesh's light relative model view projection matrices
    glm::mat4 light_view = glm::lookAt(_light_pos, _light_lookAt, _light_up);
    glm::mat4 light_VP   = _light_proj * light_view;
    _floor_lightMVP      = light_VP * _floor_M;
    _mesh_lightMVP       = light_VP * _mesh_M;

    //LOGI("light pos: %f, %f, %f\n", _light_pos.x, _light_pos.y, _light_pos.z);
}

void Example::Draw() {
    GL_CHECK(RenderDepthToFBO());
    // Render to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _width, _height);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glUseProgram(_light_shader);
    glUniform3f(_light_eyePosWS, _cam_pos.x, _cam_pos.y, _cam_pos.z);
    glUniform3f(_light_lightPosWS, _light_pos.x, _light_pos.y, _light_pos.z);
    glUniform1i(_light_shadowMap, 0);
    Texture::Bind(GL_TEXTURE0, _fb.depth_texture);
    // Draw floor
    glUniform3f(_light_ambient, 0.0f, 0.0f, 0.0f);
    glUniform3f(_light_diffuse, 0.5f, 0.5f, 0.8f);
    glUniform4f(_light_specular, 0.0f, 0.0f, 0.0f, 0.0f);
    glUniformMatrix4fv(_light_M, 1, GL_FALSE, glm::value_ptr(_floor_M));
    glUniformMatrix4fv(_light_MVP, 1, GL_FALSE, glm::value_ptr(_floor_MVP));
    glUniformMatrix3fv(_light_Normal, 1, GL_FALSE, glm::value_ptr(_floor_Normal));
    glUniformMatrix4fv(_light_BiasedLightMVP, 1, GL_FALSE, glm::value_ptr(_bias * _floor_lightMVP));
    GL_CHECK(Mesh::Draw(_floor));
    // Draw mesh
    glUniform3f(_light_ambient, 0.0f, 0.0f, 0.0f);
    glUniform3f(_light_diffuse, 0.6f, 0.1f, 0.1f);
    glUniform4f(_light_specular, 1.0f, 0.6f, 0.65f, 10.0f);
    glUniformMatrix4fv(_light_M, 1, GL_FALSE, glm::value_ptr(_mesh_M));
    glUniformMatrix4fv(_light_MVP, 1, GL_FALSE, glm::value_ptr(_mesh_MVP));
    glUniformMatrix3fv(_light_Normal, 1, GL_FALSE, glm::value_ptr(_mesh_Normal));
    glUniformMatrix4fv(_light_BiasedLightMVP, 1, GL_FALSE, glm::value_ptr(_bias * _mesh_lightMVP));
    GL_CHECK(Mesh::Draw(_mesh));

    {
        // Render depth texture
        GL_CHECK(_overlay_texture->Draw(_fb.depth_texture));
    }
}

void Example::RenderDepthToFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fb.fbo);
    glViewport(0, 0, _shadow_map_size, _shadow_map_size);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 100.0f);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glUseProgram(_depth_shader);
    // Draw mesh
    glUniformMatrix4fv(_depth_lightMVP, 1, GL_FALSE, glm::value_ptr(_mesh_lightMVP));
    Mesh::Draw(_mesh);
    // Draw floor
    glUniformMatrix4fv(_depth_lightMVP, 1, GL_FALSE, glm::value_ptr(_floor_lightMVP));
    Mesh::Draw(_floor);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_CULL_FACE);
}

Example::Example() :
    _depth_shader(0),
    _light_shader(0),
    _shadow_map_size(256),
    _overlay_texture(nullptr)
{ }

bool Example::Init() {
    if (!InitShaders())
        return false;

    //if (!Mesh::CreateFromFile("Models/cube.obj", _mesh))
    if (!Mesh::CreateFromFile("Models/buddha.obj", _mesh))
        return false;

    if (!Mesh::CreateQuad(_floor))
        return false;

    if (!FrameBuffer::CreateShadowMap(_shadow_map_size, _shadow_map_size, _fb))
        return false;

    // Setup the camera view matrix
    _cam_pos = glm::vec3(0.0f, 6.0f, 10.0f);
    glm::vec3 cam_lookAt(0.0f, 0.0f, -2.0f);
    glm::vec3 cam_up(0.0f, 1.0f, 0.0f);
    _cam_view = glm::lookAt(_cam_pos, cam_lookAt, cam_up);

    // Setup the floor's camera relative model view projection matrix
    glm::mat4 floor_scale     = glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
    glm::mat4 floor_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f));
    _floor_M                  = floor_translate * floor_scale;
    _floor_MV                 = _cam_view * _floor_M;
    _floor_Normal             = glm::inverseTranspose(glm::mat3(_floor_MV));

    // Setup the light look at and up
    _light_lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    _light_up     = glm::vec3(0.0f, 1.0f, 0.0f);
    _light_pos    = glm::vec3(0.0f, 8.0f, 0.01f);

    // Create bias matrix
    _bias = glm::mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    // Setup GL state
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    return true;
}

bool Example::InitShaders() {
    // Create shadow map shader
    {
        if (!Shader::CreateFromFile("Shaders/ShadowMap.vert", "Shaders/ShadowMap.frag", _light_shader))
            return false;
        _light_M              = glGetUniformLocation(_light_shader, "u_M");
        _light_MVP            = glGetUniformLocation(_light_shader, "u_MVP");
        _light_Normal         = glGetUniformLocation(_light_shader, "u_Normal");
        _light_BiasedLightMVP = glGetUniformLocation(_light_shader, "u_BiasedLightMVP");
        _light_ambient        = glGetUniformLocation(_light_shader, "u_Ambient");
        _light_diffuse        = glGetUniformLocation(_light_shader, "u_Diffuse");
        _light_specular       = glGetUniformLocation(_light_shader, "u_Specular");
        _light_eyePosWS       = glGetUniformLocation(_light_shader, "u_EyePosWS");
        _light_lightPosWS     = glGetUniformLocation(_light_shader, "u_LightPosWS");
        _light_shadowMap      = glGetUniformLocation(_light_shader, "u_ShadowMap");
    }

    // Create depth shader
    {
        if (!Shader::CreateFromFile("Shaders/Depth.vert", "Shaders/Depth.frag", _depth_shader))
            return false;
        _depth_lightMVP = glGetUniformLocation(_depth_shader, "u_LightMVP");
    }

    return true;
}

void Example::Resize(int width, int height) {
    Game::Resize(width, height);
    // Initialize the camera and light perspective matrices
    _cam_proj   = glm::perspective(glm::radians(60.0f), (float)_width / (float)_height, 1.0f, 100.0f);
    _light_proj = glm::perspective(glm::radians(60.0f), 1.0f, 2.0f, 20.0f);

    _floor_MVP = _cam_proj * _floor_MV;

    _overlay_texture = Util::OverlayTexture::CreateOverlayTexture(0, 0, 512, 512);
}

Example::~Example() {
    // Destroy meshes
    Mesh::Destroy(_mesh);
    Mesh::Destroy(_floor);
    // Destroy frame buffer
    FrameBuffer::Destroy(_fb);
    // Destroy shaders
    Shader::Destroy(_depth_shader);
    Shader::Destroy(_light_shader);
    SAFE_DELETE(_overlay_texture);
}
