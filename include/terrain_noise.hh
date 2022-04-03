#pragma once

#include "fastnoise.hh"

class Terrain_Noise
{
public:
    Terrain_Noise(int seed)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
    }

    char get_noise(size_t x, size_t y)
    {
        double terrain = sum_octave(8, x, y, 0.5, 0.5, 0, 255);
        return static_cast<char>(terrain);
    }

private:
    int seed_;
    FastNoiseLite noise_;
    double sum_octave(size_t num_iterations, double x, double y,
                      double persistence, double scale, double low, double high)
    {
        double maxAmp = 0;
        double amp = 1;
        double freq = scale;
        double noise = 0;

        for (size_t i = 0; i < num_iterations; i++)
        {
            noise += noise_.GetNoise(x * freq, y * freq) * amp;
            maxAmp += amp;
            amp *= persistence;
            freq *= 2;
        }

        noise /= maxAmp;

        noise = noise * (high - low) / 2 + (high + low) / 2;

        return noise;
    }

    double sum_octave_bis(size_t w, double x, double y, double ko, double fs,
                          double v, double alpha,
                          std::function<double(double)> theta, double low,
                          double high)
    {
        double noise = 0;

        for (double k = 1; k <= w; k++)
        {
            double coef = ko + fs * k;
            double random = theta(noise_.GetNoise(coef * x, coef * y));
            noise += theta(pow(coef, -v) * (coef * random));
        }

        noise = pow(noise, alpha);
        return noise * (high - low) / 2 + (high + low) / 2;
    }
};
