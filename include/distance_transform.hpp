#pragma once

#include <cstddef>

template<typename T>
void fill(T* data_out, size_t size, T value) {
    for (size_t x = 0; x < size; ++x) {
        data_out[x] = value;
    }
}

template<typename T, typename S>
S updateDistance(S distance, T data_in, S data_out, T query_value, S max_value)
{
    if (data_in == query_value) return S{0};
    if (data_out <= distance) return data_out;
    if (distance < max_value) return distance + S{1};
    return max_value;
}

template<typename T, typename S>
void distanceTransformPass1d(
    const T* data_in, S* data_out, int num_steps, int step, T query_value, S max_value
) {
    auto distance = max_value;
    for (int s = 0; s < num_steps; ++s) {
        distance = updateDistance(distance, *data_in, *data_out, query_value, max_value);
        *data_out = distance;
        data_in += step;
        data_out += step;
    }
}

template<typename T, typename S>
void distanceTransform1d(
    const T* data_in, S* data_out, size_t size, T query_value, S max_value
) {
    fill(data_out, size, max_value);
    distanceTransformPass1d(data_in, data_out, size, +1, query_value, max_value);
    distanceTransformPass1d(data_in + size - 1, data_out + size - 1, size, -1, query_value, max_value);
}

// 2D distance transform assuming Manhattan distance.
template<typename T, typename S>
void distanceTransform2d(
    const T* data_in, S* data_out, int width, int height, T query_value, S max_value
) {
    const auto size = width * height;
    fill(data_out, size, max_value);
    for (int y = 0; y < height; ++y)
    {
        const auto row_in_first = data_in + y * width;
        const auto row_out_first = data_out + y * width;
        const auto row_in_last = data_in + (y + 1) * width - 1;
        const auto row_out_last = data_out + (y + 1) * width - 1;
        distanceTransformPass1d(
            row_in_first, row_out_first, width, +1, query_value, max_value);
        distanceTransformPass1d(
            row_in_last, row_out_last, width, -1, query_value, max_value);
    }
    for (int x = 0; x < width; ++x)
    {
        const auto column_in_first = data_in + x;
        const auto column_out_first = data_out + x;
        const auto column_in_last = data_in + width * (height - 1) + x;
        const auto column_out_last = data_out + width * (height - 1) + x;
        distanceTransformPass1d(
            column_in_first, column_out_first, height, +width, query_value, max_value);
        distanceTransformPass1d(
            column_in_last, column_out_last, height, -width, query_value, max_value);
    }
}
