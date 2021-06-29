#include <iostream>
#include <type_traits>

enum qualifier {
    highp,
    mediump,
    lowp,
    defaultp = highp
};

typedef int length_t;
typedef qualifier precision;

template<length_t L, typename T, qualifier Q = defaultp> struct vec;
template<length_t R, length_t C, typename T, qualifier Q = defaultp> struct mat;

template<int N, typename T, qualifier Q, typename Enabled = void>
struct vec_data;

template <int N, typename T, qualifier Q>
struct vec_data<N, T, Q, std::enable_if_t<N == 2>> {
    union {
        T m[2] = {0, 0};
        struct {
            union { T x, r, s; };
            union { T y, g, t; };
        };
    };
};

template <int N, typename T, qualifier Q>
struct vec_data<N, T, Q, std::enable_if_t<N == 3>> {
    union {
        T m[3] = {0, 0, 0};
        struct {
            union { T x, r, s; };
	        union { T y, g, t; };
            union { T z, b, p; };
        };
    };
};

template <int N, typename T, qualifier Q>
struct vec_data<N, T, Q, std::enable_if_t<N == 4>> {
    union {
        T m[4] = {0, 0, 0, 0};
        struct {
            union { T x, r, s; };
            union { T y, g, t; };
            union { T z, b, p; };
            union { T w, a, q; };
        };
    };
};

template <int N, typename T, qualifier Q>
struct vec_data<N, T, Q, std::enable_if_t<(N != 2 && N != 3 && N != 4)>> {
    T m[N];
};

template <int N, typename T, qualifier Q>
struct vec : vec_data<N, T, Q> {

    // --- Constructors --- //
    vec(T scalar = 0) {
        for (int i = 0; i < this->len(); i++) {
            this->operator[](i) = scalar;
        }
    }


    // --- Unary Selector Functions --- //

    // Index Operator
    T & operator[](int i) {
        if (i > N || i < 0) {
            throw std::out_of_range("index out of bounds");
        }
        return this->m[i];
    }


    // --- Unary Arithmetic Operator Functions --- //

    // Addition and assign
    vec<N, T, Q> & operator+=(T scalar) {
        for (int i = 0; i < this->len(); i++) {
            this->operator[](i) += scalar;
        }
        return *this;
    }
    vec<N, T, Q> & operator+=(const vec<N, T, Q>& v) {
        for (int i = 0; i < this->len(); i++) {
            this->operator[](i) += v.operator[](i);
        }
        return *this;
    }

    // Subtract and assign
    vec<N, T, Q> & operator-=(T scalar) {
        for (int i = 0; i < this->len(); i++) {
            this->operator[](i) -= scalar;
        }
        return *this;
    }
    vec<N, T, Q> & operator-=(const vec<N, T, Q>& v) {
        for (int i = 0; i < this->len(); i++) {
            this->operator[](i) -= v.operator[](i);
        }
        return *this;
    }

    // Scalar multiplication
    vec<N, T, Q> & operator*=(T scalar);
    // Component-wise multiplication
    vec<N, T, Q> & operator*=(const vec<N, T, Q>& v);

    // Scalar division
    vec<N, T, Q> & operator/=(T scalar);
    // Component-wise division
    vec<N, T, Q> & operator/=(const vec<N, T, Q>& v);

    // Pre-Fix Increment
    vec<N, T, Q> & operator++();
    // Pre-Fix Decrement
	vec<N, T, Q> & operator--();
    // Post-Fix Increment
	vec<N, T, Q> operator++(int);
    // Post-Fix Decrement
	vec<N, T, Q> operator--(int);


    // ---  Nullary Functions --- //

    int len() {
        return N;
    }
};

// --- Binary Arithmetic Operator Functions --- //

// Addition
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator+(vec<N, T, Q> const& v, T scalar);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator+(T scalar, vec<N, T, Q> const& v);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator+(vec<N, T, Q> const& v1, vec<N, T, Q> const& v2);

// Subtraction
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator-(vec<N, T, Q> const& v, T scalar);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator-(T scalar, vec<N, T, Q> const& v);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator-(vec<N, T, Q> const& v1, vec<N, T, Q> const& v2);

// Scalar multiplication
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator*(vec<N, T, Q> const& v, T scalar);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator*(T scalar, vec<N, T, Q> const& v);
// Component-wise multiplication
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator*(vec<N, T, Q> const& v1, vec<N, T, Q> const& v2);

// Scalar division
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator/(vec<N, T, Q> const& v, T scalar);
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator/(T scalar, vec<N, T, Q> const& v);
// Component-wise division
template<int N, typename T, qualifier Q>
vec<N, T, Q> operator/(vec<N, T, Q> const& v1, vec<N, T, Q> const& v2);