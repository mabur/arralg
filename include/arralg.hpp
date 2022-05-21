// Start implementing array algorithms for 1d arrays.
// Then generalize to 2d & 3d & nd.
// Do we need to handle non-contiguous arrays?
//
// Algorithms:
// * Box-blur
// * Derivative filter
// * Sliding window min/max
// * Morphological operations
// * Distance transform

// Investigate dlib


// ooXXXXXXoo
// oXo

/*
 Input:
 r = 1
 [0 3 0]

 diameter = 2 * 1 + 1 = 3

*/

template<typename T>
void boxBlur1d(const T* data_in, T* data_out, size_t size, size_t radius, T border_value) {
    size_t diameter = 2 * radius + 1;
    if (size < diameter) {
        return;
    }
    auto sum_front = T{};
    for (size_t front = 0; front < diameter; ++front) {
        sum_front += data_in[front];
    }
    auto sum_back = T{};
    for (size_t front = diameter - 1, back = 0, center = radius; front < size; ++front, ++back, ++center) {
        sum_front += data_in[front];
        sum_back += data_in[back];
        data_out[center] = (sum_front - sum_back) / diameter;
    }
    for (size_t center = 0; center < radius; ++center) {
        data_out[center] = border_value;
    }
    for (size_t center = size - radius; center < size; ++center) {
        data_out[center] = border_value;
    }
}
