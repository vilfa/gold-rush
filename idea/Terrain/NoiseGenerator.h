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
    NoiseGenerator();
    ~NoiseGenerator();

    float* PerlinNoise2D(const int width, const int height, const int octaves = 1, const float bias = 0.2f);

private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

    float* seed;

    void generateSeed(const int width, const int height);

    double fade(const double& t) const;
    double lerp(const double& lo, const double& hi, const double& t) const;
    double grad(const int hash, const double x, const double y, const double z) const;
};
