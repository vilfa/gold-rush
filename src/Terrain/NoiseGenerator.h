#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>

class NoiseGenerator
{
public:
    static std::shared_ptr<float[]> PerlinNoise2D(const int _width,
                                                  const int _height,
                                                  const int _octaves = 1,
                                                  const float _bias = 0.2f);

private:
    NoiseGenerator();

    static std::shared_ptr<float[]> generateSeed(const int _width,
                                                 const int _height,
                                                 std::mt19937 &rnd_eng,
                                                 std::uniform_real_distribution<float> &dist);

    static double fade(const double &_t);
    static double lerp(const double &_lo, const double &_hi, const double &_t);
    static double grad(const int _hash, const double _x, const double _y, const double _z);
};
