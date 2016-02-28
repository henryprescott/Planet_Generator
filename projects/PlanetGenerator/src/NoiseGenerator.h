#pragma once

#include "noiseutils.h"

class NoiseGenerator
{
public:
	explicit NoiseGenerator();
	~NoiseGenerator();

	void init();

private:
	module::Perlin m_perlin;
	utils::NoiseMap m_heightMap;
	utils::NoiseMapBuilderPlane m_heightMapBuilder;
	utils::RendererImage m_renderer;
	utils::Image m_image;
	utils::WriterBMP m_writer;
};
