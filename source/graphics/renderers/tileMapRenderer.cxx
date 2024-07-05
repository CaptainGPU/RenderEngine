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
}

RenderPass* registerTileMapRenderPass()
{
    std::vector<std::string> depthPrePassUniformNames = { "u_modelMatrix", "u_viewMatrix", "u_projectionMatrix" };

    TileMapPass::tileMapPass = new RenderPass();
    TileMapPass::tileMapPass->makeProgram("tileMap", "tileMap");
    TileMapPass::tileMapPass->registerUniforms(depthPrePassUniformNames);

    TileMapPass::tileMapPassModelMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[0]);
    TileMapPass::tileMapPassViewMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[1]);
    TileMapPass::tileMapPassProjectionMatrixUniform = TileMapPass::tileMapPass->getUniform(depthPrePassUniformNames[2]);

    TileMapPass::tileMapPassFrameBuffer = Render::createCustomFrameBuffer(800, 600, 3);
    
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
        glm::vec3 pos = camera->getPosition();
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-pos.x, -pos.y, 0));
        TileMapPass::tileMapPassViewMatrixUniform->setMatrix4x4(viewMatrix);
        
        glm::mat4& projection_matrix = scene->getCamera()->getOrthoMatrix();
        TileMapPass::tileMapPassProjectionMatrixUniform->setMatrix4x4(projection_matrix);
        
        glm::mat4 modelMatrix = glm::mat4(1.0);
        glm::vec3 scale = glm::vec3((float)tileMap->getTileSize());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
        
        TileMapPass::tileMapPassModelMatrixUniform->setMatrix4x4(modelMatrix);
        
        Mesh* mesh = tileMap->getMesh(0);
        
        Render::drawMesh(mesh, renderInfo);
        
    }

    /*size_t numGameObject = scene->getGameObjectCount();

    
    
    
    

   


    for (size_t i = 0; i < numGameObject; i++)
    {
        GameObject* gameObject = scene->getGameObject(i);
        unsigned int meshCount = gameObject->getMeshCount();

        if (meshCount == 0 || !gameObject->isRenderingObject())
        {
            continue;
        }

        glm::mat4 modelMatrix = gameObject->getModelMatrix();
        TileMapPass::tileMapPassModelMatrixUniform->setMatrix4x4(modelMatrix);

        for (size_t j = 0; j < meshCount; j++)
        {
            Mesh* mesh = gameObject->getMesh(j);

            Render::drawMesh(mesh, renderInfo);
        }
    }*/
}

void renderTileMapPass(RenderPass* renderPass, RenderInfo& renderInfo)
{
    Render::setViewport(0, 0, 800, 600);
    Render::useFrameBuffer(TileMapPass::tileMapPassFrameBuffer);

    Render::startRenderPass(TileMapPass::tileMapPass, renderInfo);

    Render::clearView(0.1, 0.1, 0.1, 1.0);

    TileMapRender(renderInfo);

    Render::endRenderPass(TileMapPass::tileMapPass);

    Render::unUseFrameBuffer();
}

FrameBuffer* getTileMapPassFrameBuffer()
{
    return TileMapPass::tileMapPassFrameBuffer;
}

