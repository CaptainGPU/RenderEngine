#pragma once

class RenderPass
{
public:
	RenderPass();

	void makeProgram();

	void startRenderPass();
	void endRenderPass();
};