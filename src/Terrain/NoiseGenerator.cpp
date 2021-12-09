#include "Terrain/NoiseGenerator.h"

std::shared_ptr<float[]> NoiseGenerator::PerlinNoise2D(const int _width, const int _height,
                                                       const int _octaves, const float _bias)
{
    std::mt19937 rnd_eng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0, 1);

    std::shared_ptr<float[]> seed(generateSeed(_width, _height, rnd_eng, dist));
    std::shared_ptr<float[]> height_map(new float[_width * _height]);

    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            float noise = 0.0f;
            float scale_acc = 0.0f;
            float scale = 1.0f;

            for (int o = 0; o < _octaves; o++)
            {
                int pitch = _width >> o;
                int sample_x1 = (x / pitch) * pitch;
                int sample_y1 = (y / pitch) * pitch;

                int sample_x2 = (sample_x1 + pitch) % _width;
                int sample_y2 = (sample_y1 + pitch) % _width;

                float blend_x = (float)(x - sample_x1) / (float)pitch;
                float blend_y = (float)(y - sample_y1) / (float)pitch;

                float sample_t = (1.0f - blend_x) * seed[sample_y1 * _width + sample_x1] + blend_x * seed[sample_y1 * _width + sample_x2];
                float sample_b = (1.0f - blend_x) * seed[sample_y2 * _width + sample_x1] + blend_x * seed[sample_y2 * _width + sample_x2];

                scale_acc += scale;
                noise += (blend_y * (sample_b - sample_t) + sample_t) * scale;
                scale = scale / _bias;
            }

            height_map[y * _width + x] = noise / scale_acc;
        }
    }

    return height_map;
}

std::shared_ptr<float[]> NoiseGenerator::generateSeed(const int _width, const int _height,
                                                      std::mt19937 &rnd_eng, std::uniform_real_distribution<float> &dist)
{
    std::shared_ptr<float[]> sp_seed(new float[_width * _height]);

    for (int i = 0; i < _width * _height; i++)
    {
        sp_seed[i] = dist(rnd_eng);
    }

    return sp_seed;
}

double NoiseGenerator::fade(const double &_t)
{
    // THIS IS UNUSED.
    // The equation proposed by Ken Perlin to replace the smoothstep function in 2002.
    // The equation is the following: 6 * t ** 5 - 15 * t ** 4 + 10 * t ** 3
    //
    return 6 * pow(_t, 5) - 15 * pow(_t, 4) + 10 * pow(_t, 3);
}

double NoiseGenerator::lerp(const double &_lo, const double &_hi,
                            const double &_t)
{
    // THIS IS UNUSED.
    // Linear interpolate but there is a difference from the ordinary linear interpolation funciton.
    // The normal equation is: t * p1 + (1 - t) * p2, but here we need a value that is between the low and high value,
    // so we can get a smooth transition, hence the equation adds t * the diffenece to the min.
    // So we get at minimum lo when t = 0 and at maximum hi when t = 1.
    //
    return _lo + _t * (_hi - _lo);
}

double NoiseGenerator::grad(const int _hash, const double _x,
                            const double _y, const double _z)
{
    int h = _hash & 15;
    double u = h < 8 ? _x : _y;
    double v = h < 4 ? _y : h == 12 || h == 14 ? _x
                                               : _z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
