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
void distanceTransform1d(
    const T* data_in, S* data_out, size_t size, T query_value, S max_value
) {
    for (size_t x = 0; x < size; ++x) {
        data_out[x] = max_value;
    }
    // Left pass:
    auto distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            data_in[x] == query_value ?
                0 : distance == max_value ?
                    distance : distance + 1;
        data_out[x] = getMin(data_out[x], distance);
    }
    // Right pass:
    distance = max_value;
    for (size_t x = 0; x < size; ++x) {
        distance =
            data_in[size - 1 - x] == query_value ?
                0 : distance == max_value ?
                    distance : distance + 1;
        data_out[size - 1 - x] = getMin(data_out[size - 1 - x], distance);
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
