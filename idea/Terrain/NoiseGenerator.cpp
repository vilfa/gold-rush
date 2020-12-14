#include "Terrain/NoiseGenerator.h"

NoiseGenerator::NoiseGenerator() :
    rd(std::random_device()),
    mt(std::mt19937(rd())),
    dist(std::uniform_real_distribution<float> (0, 1)),
    seed(nullptr)
{
}

NoiseGenerator::~NoiseGenerator()
{
    delete[] seed;
}

float* NoiseGenerator::PerlinNoise2D(const int width, const int height, const int octaves, const float bias)
{
    generateSeed(width, height);

    float* heightMap = new float[(width * height)];

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            float noise = 0.0f;
            float scaleAcc = 0.0f;
            float scale = 1.0f;

            for (int o = 0; o < octaves; o++)
            {
                int pitch = width >> o;
                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;

                int sampleX2 = (sampleX1 + pitch) % width;
                int sampleY2 = (sampleY1 + pitch) % width;

                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;

                float sampleT = (1.0f - blendX) * seed[sampleY1 * width + sampleX1] + blendX * seed[sampleY1 * width + sampleX2];
                float sampleB = (1.0f - blendX) * seed[sampleY2 * width + sampleX1] + blendX * seed[sampleY2 * width + sampleX2];

                scaleAcc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = scale / bias;
            }

            heightMap[y * width + x] = noise / scaleAcc;
        }
    }


    std::cout << "NOISE_GENERATOR::PERLIN_NOISE_2D::GENERATE_NOISE" << std::endl;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            std::cout << heightMap[y * width + x] << " ";
        }
        std::cout << std::endl;
    }

    return heightMap;
}

void NoiseGenerator::generateSeed(const int width, const int height)
{
    seed = new float[(width * height)];

    for (int i = 0; i < width * height; i++)
    {
        seed[i] = dist(mt);
    }
}

double NoiseGenerator::fade(const double& t) const
{
    // The equation proposed by Ken Perlin to replace the smoothstep function in 2002.
    // The equation is the following: 6 * t ** 5 - 15 * t ** 4 + 10 * t ** 3
    return 6 * pow(t, 5) - 15 * pow(t, 4) + 10 * pow(t, 3);
}

double NoiseGenerator::lerp(const double& lo, const double& hi, const double& t) const
{
    // Linear interpolate but there is a difference from the ordinary linear interpolation funciton.
    // The normal equation is: t * p1 + (1 - t) * p2, but here we need a value that is between the low and high value,
    // so we can get a smooth transition, hence the equation adds t * the diffenece to the min.
    // So we get at minimum lo when t = 0 and at maximum hi when t = 1.
    return lo + t * (hi - lo);
}

double NoiseGenerator::grad(const int hash, const double x, const double y, const double z) const
{
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}