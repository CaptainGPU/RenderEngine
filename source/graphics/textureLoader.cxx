#include "graphics/textureLoader.hxx"

#include "render.hxx"

#include "stb_image.hxx"

Texture* loadTexture(std::string textureName)
{
	std::string textureFolder = "../textures/";

#if CURRENT_PLATFORM == PLATFORM_MAC
	textureFolder = "../../textures/";
#endif

	std::string textureFilePath = textureFolder + textureName;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	if (!data)
	{
		return nullptr;
	}

	Texture* texture = Render::createTexture();

	Render::useTexture(texture);
	Render::loadDataToTexture(texture, width, height, data);
	Render::unUseTexture();

	stbi_image_free(data);

	return texture;
}