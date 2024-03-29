#include <cassert>
#include <iostream>
#include <valarray>
#include <vector>

#include "arralg.hpp"

template<typename Container>
void printContainerDifference(const Container& left, const Container& right)
{
    using namespace std;
    for (const auto x : left) {
        cout << x << " ";
    }
    cout << "!= ";
    for (const auto x : right) {
        cout << x << " ";
    }
    cout << endl;
}

template<typename Container>
void printContainerDifference(const Container& left, const Container& right, int width, int height)
{
    using namespace std;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cout << left[x + y * width] << " ";
        }
        cout << endl;
    }
    cout << "!= " << endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cout << right[x + y * width] << " ";
        }
        cout << endl;
    }
}

template<typename T>
void assert_equal(const T& left, const T& right)
{
    assert(left == right);
}

template<typename T>
void assert_equal(const std::vector<T>& left, const std::vector<T>& right)
{
    if (left != right) {
        printContainerDifference(left, right);
    }
}

template<typename T>
void assert_equal(const std::vector<T>& left, const std::vector<T>& right, int width, int height)
{
    if (left != right) {
        printContainerDifference(left, right, width, height);
    }
}

template<typename T>
void assert_equal(const std::valarray<T>& left, const std::valarray<T>& right)
{
    if (left.size() != right.size()) {
        printContainerDifference(left, right);
        return;
    }
    for (size_t i = 0; i < left.size(); ++i) {
        if (left[i] != right[i]) {
            printContainerDifference(left, right);
            return;
        }
    }
}

void assert_equal(int left, int right)
{
    if (left != right) {
        using namespace std;
        cout << left << " != " << right << endl;
        assert(left == right);
    }
}

using Vec = std::vector<double>;
using Vecb = std::valarray<bool>;

Vec zeros(const Vec& v) {
    return Vec(v.size(), 0.0);
}

Vecb falses(const Vecb& v) {
    return Vecb(false, v.size());
}

int main() {
    using std::cout;
    using std::endl;
    cout << "Begin testing boxBlur1d..." << endl;
    {
        const auto input = Vec{};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 0, 0.0);
        assert_equal(output, zeros(input));
    }
    {
        const auto input = Vec{1.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 0.0);
        assert_equal(output, zeros(input));
    }
    {
        const auto input = Vec{1.0, 1.0, 1.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 2.0);
        assert_equal(output, Vec{2.0, 1.0, 2.0});
    }
    {
        const auto input = Vec{0.0, 3.0, 0.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 2.0);
        assert_equal(output, Vec{2.0, 1.0, 2.0});
    }
    {
        const auto input = Vec{1.0, 1.0, 1.0, 1.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 2.0);
        assert_equal(output, Vec{2.0, 1.0, 1.0, 2.0});
    }
    {
        const auto input = Vec{1.0, 2.0, 3.0, 4.0, 5.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 0.0);
        assert_equal(output, Vec{0.0, 2.0, 3.0, 4.0, 0.0});
    }
    {
        const auto input = Vec{2.0, 2.0, 5.0, 2.0, 2.0};
        auto output = zeros(input);
        boxBlur1d(input.data(), output.data(), input.size(), 1, 0.0);
        assert_equal(output, Vec{0.0, 3.0, 3.0, 3.0, 0.0});
    }
    cout << "Testing boxBlur1d done." << endl << endl;
    cout << "Begin testing distanceTransform1d..." << endl;
    {
        const auto input = Vec{1.0};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 1.0, 100.0);
        assert_equal(output, Vec{0.0});
    }
    {
        const auto input = Vec{9.0};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 1.0, 100.0);
        assert_equal(output, Vec{100.0});
    }
    {
        const auto input = Vec{1.0, 0.0, 0.0};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 1.0, 100.0);
        assert_equal(output, Vec{0.0, 1.0, 2.0});
    }
    {
        const auto input = Vec{0.0, 0.0, 1.0};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 1.0, 100.0);
        assert_equal(output, Vec{2.0, 1.0, 0.0});
    }
    {
        const auto input = Vec{5, 7, 3, 8, 9, 4, 5, 3, 6, 8, 4, 2, 4, 5};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 8.0, 100.0);
        assert_equal(output, Vec{3, 2, 1, 0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 4});
    }
    {
        const auto input = Vec{5, 7, 3, 8, 9, 4, 5, 3, 6, 8, 4, 2, 4, 5};
        auto output = zeros(input);
        distanceTransform1d(input.data(), output.data(), input.size(), 8.0, 2.0);
        assert_equal(output, Vec{2, 2, 1, 0, 1, 2, 2, 2, 1, 0, 1, 2, 2, 2});
    }
    cout << "Testing distanceTransform1d done." << endl << endl;
    cout << "Begin testing dilate1d..." << endl;
    {
        const auto input = Vecb{false, true, false};
        auto output = falses(input);
        dilate1d(begin(input), begin(output), input.size(), 1);
        assert_equal(output, Vecb{true, true, true});
    }
    {
        const auto input = Vecb{false, false, false};
        auto output = falses(input);
        dilate1d(begin(input), begin(output), input.size(), 1);
        assert_equal(output, Vecb{false, false, false});
    }
    {
        const auto input = Vecb{true, false, false};
        auto output = falses(input);
        dilate1d(begin(input), begin(output), input.size(), 1);
        assert_equal(output, Vecb{true, true, false});
    }
    {
        const auto input = Vecb{true, false, false, false, true, true, false, false};
        auto output = falses(input);
        dilate1d(begin(input), begin(output), input.size(), 1);
        assert_equal(output, Vecb{true, true, false, true, true, true, true, false});
    }
    {
        const auto input = Vec{1, 0, 0, 1, 0, 0, 0, 1, 1, 0};
        auto output = zeros(input);
        dilate1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{1, 1, 1, 1, 1, 0, 1, 1, 1, 1});
    }
    cout << "Testing dilate1d done." << endl << endl;
    cout << "Begin testing erode1d..." << endl;
    {
        const auto input = Vec{0, 0, 0};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0, 0, 0});
    }
    {
        const auto input = Vec{0, 1, 0};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0, 0, 0});
    }
    {
        const auto input = Vec{0, 0, 1};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0, 0, 0});
    }
    {
        const auto input = Vec{1, 0, 0};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0, 0, 0});
    }
    {
        const auto input = Vec{0, 1, 1, 1, 0};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0, 0, 1, 0, 0});
    }
    {
        const auto input = Vec{1,0,0,1,1,1,0,1,0,1,1,0,0,1,1};
        auto output = zeros(input);
        erode1d(input.data(), output.data(), input.size(), 1);
        assert_equal(output, Vec{0,0,0,0,1,0,0,0,0,0,0,0,0,0,1});
    }
    cout << "Testing erode1d done." << endl << endl;
    cout << "Begin testing distanceTransform2d..." << endl;
    {
        const auto input = Vec{
            0,0,0,
            0,1,0,
            0,0,0,
        };
        const auto expected_input = Vec{
            2,1,2,
            1,0,1,
            2,1,2,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 3, 3, 1.0, 100.0);
        assert_equal(output, expected_input, 3, 3);
    }
    {
        const auto input = Vec{
            1,
            0,
            0,
        };
        const auto expected_input = Vec{
            0,
            1,
            2,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 1, 3, 1.0, 100.0);
        assert_equal(output, expected_input, 1, 3);
    }
    {
        const auto input = Vec{
            0,
            0,
            1,
        };
        const auto expected_input = Vec{
            2,
            1,
            0,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 1, 3, 1.0, 100.0);
        assert_equal(output, expected_input, 1, 3);
    }
    {
        const auto input = Vec{
            0,0,0,
            0,0,0,
            0,0,1,
        };
        const auto expected_input = Vec{
            4,3,2,
            3,2,1,
            2,1,0,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 3, 3, 1.0, 100.0);
        assert_equal(output, expected_input, 3, 3);
    }
    {
        const auto input = Vec{
            0,0,0,
            0,0,0,
            1,0,1,
        };
        const auto expected_output = Vec{
            2,3,2,
            1,2,1,
            0,1,0,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 3, 3, 1.0, 100.0);
        assert_equal(output, expected_output, 3, 3);
    }
    {
        const auto input = Vec{
            0,0,0,1,
            0,0,0,0,
            1,0,0,0,
        };
        const auto expected_output = Vec{
            2,2,1,0,
            1,2,2,1,
            0,1,2,2,
        };
        auto output = zeros(input);
        distanceTransform2d(input.data(), output.data(), 4, 3, 1.0, 100.0);
        assert_equal(output, expected_output, 4, 3);
    }
    cout << "Testing distanceTransform2d done." << endl << endl;
    cout << "Begin testing transpose2d..." << endl;
    {
        const auto input = Vec{
            1,1,1,
            0,1,0,
        };
        const auto expected_output = Vec{
            1,0,
            1,1,
            1,0,
        };
        auto output = zeros(input);
        transpose2d(input.data(), output.data(), 3, 2);
        assert_equal(output, expected_output, 2, 3);
    }
    cout << "Testing transpose2d done." << endl << endl;
    cout << "Begin testing boxBlur2d..." << endl;
    {
        const auto input = Vec{
            1,1,1,
            1,1,1,
            1,1,1,
        };
        const auto expected_output = Vec{
            0,0,0,
            0,1,0,
            0,0,0,
        };
        auto output = zeros(input);
        auto buffer = zeros(input);
        boxBlur2d(input.data(), buffer.data(), output.data(), 3, 3, 1, 0.0);
        assert_equal(output, expected_output, 3, 3);
    }
    {
        const auto input = Vec{
            1,1,1,
            1,10,1,
            1,1,1,
        };
        const auto expected_output = Vec{
            0,0,0,
            0,2,0,
            0,0,0,
        };
        auto output = zeros(input);
        auto buffer = zeros(input);
        boxBlur2d(input.data(), buffer.data(), output.data(), 3, 3, 1, 0.0);
        assert_equal(output, expected_output, 3, 3);
    }
    {
        const auto input = Vec{
            1,0,0,1,
            0,2,2,0,
            0,2,2,0,
            1,0,0,1,
        };
        const auto expected_output = Vec{
            0,0,0,0,
            0,1,1,0,
            0,1,1,0,
            0,0,0,0,
        };
        auto output = zeros(input);
        auto buffer = zeros(input);
        boxBlur2d(input.data(), buffer.data(), output.data(), 4, 4, 1, 0.0);
        assert_equal(output, expected_output, 4, 4);
    }
    cout << "Testing boxBlur2d done." << endl << endl;
    return 0;
}
