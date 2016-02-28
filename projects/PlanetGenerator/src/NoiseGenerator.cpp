#include "stdafx.h"
#include "NoiseGenerator.h"
#include <noise\noise.h>
using namespace noise;

NoiseGenerator::NoiseGenerator()
{
}

NoiseGenerator::~NoiseGenerator()
{
}

void NoiseGenerator::init()
{
	utils::NoiseMapBuilderPlane m_heightMapBuilder;
	m_heightMapBuilder.SetSourceModule(m_perlin);
	m_heightMapBuilder.SetDestNoiseMap(m_heightMap);
	m_heightMapBuilder.SetDestSize(256, 256);
	m_heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	m_heightMapBuilder.Build();

	m_renderer.SetSourceNoiseMap(m_heightMap);
	m_renderer.SetDestImage(m_image);
	m_renderer.ClearGradient();
	m_renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	m_renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	m_renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	m_renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	m_renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	m_renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	m_renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	m_renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	m_renderer.Render();
	
	m_writer.SetSourceImage(m_image);
	m_writer.SetDestFilename("tutorial.bmp");
	m_writer.WriteDestFile();
}