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
