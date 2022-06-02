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

template<typename T>
T getMin(T a, T b) {
    return a < b ? a : b;
}

size_t getDiameter(size_t radius) {
    return 2 * radius + 1;
}

template<typename T>
void fill(T* data_out, size_t size, T value) {
    for (size_t x = 0; x < size; ++x) {
        data_out[x] = value;
    }
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

template<typename T, typename S>
void distanceTransformPass1d(
    const T* data_in, S* data_out, int num_steps, int step, T query_value, S max_value
) {
    auto distance = max_value;
    for (int s = 0; s < num_steps; ++s) {
        distance =
            *data_in == query_value ?
            S{0} : distance == max_value ?
                distance : distance + S{1};
        distance = getMin(*data_out, distance);
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
