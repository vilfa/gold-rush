#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

class NoiseGenerator
{
public:
    static std::shared_ptr<float[]> PerlinNoise2D(
        const int width, 
        const int height, 
        const int octaves = 1, 
        const float bias = 0.2f
    );

private:
    NoiseGenerator();

    static std::shared_ptr<float[]> generateSeed(
        const int width,
        const int height,
        std::mt19937& reng,
        std::uniform_real_distribution<float>& dist
    );

    double fade(const double& t) const;
    double lerp(const double& lo, const double& hi, const double& t) const;
    double grad(const int hash, const double x, const double y, const double z) const;
};
