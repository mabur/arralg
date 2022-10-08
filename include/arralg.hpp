// Start implementing array algorithms for 1d arrays.
// Then generalize to 2d & 3d & nd.
// Do we need to handle non-contiguous arrays?
// Investigate dlib.
//
// Algorithms:
// * Box-blur
// * Distance transform
// * Morphological operations
// * Derivative filter
// * Sliding window min/max
// * Transpose

#include "distance_transform.hpp"

template<typename T>
void transpose2d(const T* data_in, T* data_out, size_t width_in, size_t height_in) {
    for (size_t y = 0; y < height_in; ++y) {
        for (size_t x = 0; x < width_in; ++x) {
            data_out[x * height_in + y] = data_in[y * width_in + x];
        }
    }
}

size_t getDiameter(size_t radius) {
    return 2 * radius + 1;
}

template<typename T>
void boxBlur1d(
    const T* data_in, T* data_out, size_t size, size_t radius, T border_value
) {
    const size_t diameter = getDiameter(radius);
    if (size < diameter) {
        return;
    }
    auto sum_back = T{};
    auto sum_front = T{};
    size_t back = 0;
    size_t center = 0;
    size_t front = 0;
    while (center < radius) {
        data_out[center++] = border_value;
    }
    while (front < diameter - 1) {
        sum_front += data_in[front++];
    }
    while (front < size) {
        sum_front += data_in[front++];
        data_out[center++] = (sum_front - sum_back) / diameter;
        sum_back += data_in[back++];
    }
    while (center < size) {
        data_out[center++] = border_value;
    }
}


template<typename T>
void boxBlur2d(
    const T* data_in,
    T* data_buffer,
    T* data_out,
    size_t width,
    size_t height,
    size_t radius,
    T border_value
) {
    for (size_t y = 0, offset = 0; y < height; ++y, offset += width) {
        boxBlur1d(data_in + offset, data_buffer + offset, width, radius, border_value);
    }
    transpose2d(data_buffer, data_out, width, height);
    for (size_t x = 0, offset = 0; x < width; ++x, offset += height) {
        boxBlur1d(data_out + offset, data_buffer + offset, height, radius, border_value);
    }
    transpose2d(data_buffer, data_out, height, width);
}

template<typename T, typename S>
void dilate1d(
    const T* data_in, S* data_out, size_t size, size_t radius
) {
    const size_t diameter = getDiameter(radius);
    if (size < diameter) {
        return;
    }
    const auto max_value = size;
    // Left pass:
    auto distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            data_in[x] ?
            0 : distance == max_value ?
                distance : distance + 1;
        data_out[x] = distance <= radius;
    }
    // Right pass:
    distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            data_in[size - 1 - x] ?
            0 : distance == max_value ?
                distance : distance + 1;
        data_out[size - 1 - x] = data_out[size - 1 - x] || distance <= radius;
    }
}

template<typename T, typename S>
void erode1d(
    const T* data_in, S* data_out, size_t size, size_t radius
) {
    const size_t diameter = getDiameter(radius);
    if (size < diameter) {
        return;
    }
    const auto max_value = size;
    // Left pass:
    auto distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            !data_in[x] ?
            0 : distance == max_value ?
                distance : distance + 1;
        data_out[x] = distance > radius;
    }
    // Right pass:
    distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            !data_in[size - 1 - x] ?
            0 : distance == max_value ?
                distance : distance + 1;
        data_out[size - 1 - x] = data_out[size - 1 - x] && distance > radius;
    }
}
