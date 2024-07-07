//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//
#include "graphics/renderers/tileMapRenderer.hxx"

#include "renderPass.hxx"
#include "screenRenderPass.hxx"
#include "render.hxx"
#include "engine.hxx"

#include "scenes/scene2D.hxx"
#include "gameObjects/tileMap.hxx"

namespace TileMapPass
{
    RenderPass* tileMapPass = nullptr;
    FrameBuffer* tileMapPassFrameBuffer = nullptr;
    Uniform* tileMapPassModelMatrixUniform = nullptr;
    Uniform* tileMapPassViewMatrixUniform = nullptr;
    Uniform* tileMapPassProjectionMatrixUniform = nullptr;
    Uniform* tileMapCellPalleteIndexUniform = nullptr;
    Uniform* tileMapPalleteTextureUniform = nullptr;
    Uniform* tileMapPalleteWidthUniform = nullptr;
    Uniform* tileMapPalleteHeightUniform = nullptr;
}

RenderPass* registerTileMapRenderPass()
{
    std::vector<std::string> depthPrePassUniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix", "u_cellPalleteIndex", "u_palleteTexture", "u_palleteWidth", "u_palleteHeight"};

    TileMapPass::tileMapPass = new RenderPass();
    TileMapPass::tileMapPass->makeProgram("tileMap", "tileMap");
    TileMapPass::tileMapPass->registerUniforms(depthPrePassUniformNames);

    TileMapPass::tileMapPassModelMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[0]);
    TileMapPass::tileMapPassViewMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[1]);
    TileMapPass::tileMapPassProjectionMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[2]);
    TileMapPass::tileMapCellPalleteIndexUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[3]);
    TileMapPass::tileMapPalleteTextureUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[4]);
    TileMapPass::tileMapPalleteWidthUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[5]);
    TileMapPass::tileMapPalleteHeightUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[6]);

    TileMapPass::tileMapPassFrameBuffer = Render::createCustomFrameBuffer(800, 600, 1);
    
    return TileMapPass::tileMapPass;
}

void TileMapRender(RenderInfo& renderInfo)
{
    SceneManager* manager = Engine::get()->getSceneManager();
    Scene2D* scene = dynamic_cast<Scene2D*>(manager->getScene());
    
    if (!scene)
    {
        return;
    }
    
    Camera* camera = scene->getCamera();
    std::vector<TileMap*> tileMaps = scene->getTileMaps();
    
    for (TileMap* tileMap : tileMaps)
    {
        if (!(tileMap->getMeshCount() > 0))
        {
            continue;
        }
        
        glm::mat4 viewMatrix = glm::mat4(1.0);
        glm::vec3 cameraPos = camera->getPosition();

        float tileSize = (float)tileMap->getTileSize();

        float x = fmod(cameraPos.x, tileSize);
        float y = fmod(cameraPos.y, tileSize);

        glm::vec3 position = glm::vec3(-x, -y, 0.0f);


        viewMatrix = glm::translate(viewMatrix, glm::vec3(position.x, position.y, 0));
        TileMapPass::tileMapPassViewMatrixUniform->setMatrix4x4(viewMatrix);
        
        glm::mat4& projection_matrix = scene->getCamera()->getOrthoMatrix();
        TileMapPass::tileMapPassProjectionMatrixUniform->setMatrix4x4(projection_matrix);
        
        glm::mat4 modelMatrix = glm::mat4(1.0);
        glm::vec3 scale = glm::vec3(tileSize);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
        
        TileMapPass::tileMapPassModelMatrixUniform->setMatrix4x4(modelMatrix);

        float* cellPalleteIndexArray = tileMap->getIndexes();
        uint32_t cellPalleteIndexCount = tileMap->getIndexesCount();
        TileMapPass::tileMapCellPalleteIndexUniform->setFloatArray(cellPalleteIndexArray, cellPalleteIndexCount);

        Texture* palleteTexture = tileMap->getPalleteTexture();
        TileMapPass::tileMapPalleteTextureUniform->setTexture(palleteTexture, 0);

        float palleteSize = 16.0f;
        TileMapPass::tileMapPalleteWidthUniform->setFloat(palleteSize);
        TileMapPass::tileMapPalleteHeightUniform->setFloat(palleteSize);
        
        Mesh* mesh = tileMap->getMesh(0);
        
        Render::drawMesh(mesh, renderInfo);

        float index = 215.0f;

        float py = floorf(index / 16.0f);
        float px = fmodf(index, 16.0f);

        int a = 12;
        
    }
}

void renderTileMapPass(RenderPass* renderPass, RenderInfo& renderInfo)
{
    Render::setViewport(0, 0, 800, 600);
    Render::useFrameBuffer(TileMapPass::tileMapPassFrameBuffer);

    Render::startRenderPass(TileMapPass::tileMapPass, renderInfo);

    Render::clearView(0.0f, 0.0f, 0.0f, 1.0);

    TileMapRender(renderInfo);

    Render::endRenderPass(TileMapPass::tileMapPass);

    Render::unUseFrameBuffer();
}

FrameBuffer* getTileMapPassFrameBuffer()
{
    return TileMapPass::tileMapPassFrameBuffer;
}

