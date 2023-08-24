/**
 * @file vector.h
 * @brief Provides basic structure and functions to handle vectors
 * @author Théo Magne
 * @date 25/08/2023
 * @see vector.c
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

/* ************************************* Includes *********************************************** */
#include <math.h>

/* ************************************* Public macros ****************************************** */

/* ************************************* Public type definition ********************************* */
/**
 * @brief 3D vector structure
 */
typedef union vector3_t
{
    struct
    {
        float x; /**< X coordinate */
        float y; /**< Y coordinate */
        float z; /**< Z coordinate */
    };
    float v[3]; /**< Vector coordinates */
} vector3_t;

/**
 * @brief 2D vector structure
 */
typedef union vector2_t
{
    struct
    {
        float x; /**< X coordinate */
        float y; /**< Y coordinate */
    };
    float v[2]; /**< Vector coordinates */
} vector2_t;


/* ************************************* Public variables *************************************** */

/* ************************************* Public functions *************************************** */

/**
 * @brief Computes the cross product of two vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @param[out] result Result of the cross product
 */
static inline void vector3_cross(const vector3_t *a, const vector3_t *b, vector3_t *result)
{
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

/**
 * @brief Computes the dot product of two vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @return Dot product of the two vectors
 */
static inline float vector3_dot(const vector3_t *a, const vector3_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/**
 * @brief Computes the norm of a vector
 * @param[in] a Vector
 * @return norm of the vector
 */
static inline float vector3_norm(const vector3_t *a)
{
    return sqrtf(vector3_dot(a, a));
}

/**
 * @brief Computes the sum of two vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @param[out] result Result of the sum
 */
static inline void vector3_add(const vector3_t *a, const vector3_t *b, vector3_t *result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

/**
 * @brief Computes the difference of two vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @param[out] result Result of the difference
 */
static inline void vector3_sub(const vector3_t *a, const vector3_t *b, vector3_t *result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

/**
 * @brief Computes the product of a vector by a scalar
 * @param[in] a Vector
 * @param[in] b Scalar
 * @param[out] result Result of the product
 */
static inline void vector3_scale(const vector3_t *a, float b, vector3_t *result)
{
    result->x = a->x * b;
    result->y = a->y * b;
    result->z = a->z * b;
}

/**
 * @brief Normalizes a vector
 * @param[in] a Vector
 * @param[out] result Result of the normalization
 */
static inline void vector3_normalize(const vector3_t *a, vector3_t *result)
{
    vector3_scale(a, 1.0f / vector3_norm(a), result);
}

/**
 * @brief Computes the dot product of two 2D vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @return Dot product of the two vectors
 */
static inline float vector2_dot(const vector2_t *a, const vector2_t *b)
{
    return a->x * b->x + a->y * b->y;
}

/**
 * @brief Computes the norm of a 2D vector
 * @param[in] a Vector
 * @return norm of the vector
 */
static inline float vector2_norm(const vector2_t *a)
{
    return sqrtf(vector2_dot(a, a));
}

/**
 * @brief Computes the sum of two 2D vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @param[out] result Result of the sum
 */
static inline void vector2_add(const vector2_t *a, const vector2_t *b, vector2_t *result)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

/**
 * @brief Computes the difference of two 2D vectors
 * @param[in] a First vector
 * @param[in] b Second vector
 * @param[out] result Result of the difference
 */
static inline void vector2_sub(const vector2_t *a, const vector2_t *b, vector2_t *result)
{
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

/**
 * @brief Computes the product of a 2D vector by a scalar
 * @param[in] a Vector
 * @param[in] b Scalar
 * @param[out] result Result of the product
 */
static inline void vector2_scale(const vector2_t *a, float b, vector2_t *result)
{
    result->x = a->x * b;
    result->y = a->y * b;
}

/**
 * @brief Normalizes a 2D vector
 * @param[in] a Vector
 * @param[out] result Result of the normalization
 */
static inline void vector2_normalize(const vector2_t *a, vector2_t *result)
{
    vector2_scale(a, 1.0f / vector2_norm(a), result);
}


/* ************************************* Public callback functions ****************************** */

#endif /* _VECTOR_H_ */
