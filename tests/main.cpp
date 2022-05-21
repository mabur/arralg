#include <cassert>
#include <iostream>
#include <vector>

#include "arralg.hpp"

template<typename T>
void assert_equal(const T& left, const T& right)
{
    assert(left == right);
}

template<typename T>
void assert_equal(const std::vector<T>& left, const std::vector<T>& right)
{
    if (left != right) {
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

Vec zeros(const Vec& v) {
    return Vec(v.size(), 0.0);
}

int main() {
    using std::cout;
    using std::endl;
    cout << "Begin testing..." << endl;
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
    std::cout << "Testing done.";
    return 0;
}
