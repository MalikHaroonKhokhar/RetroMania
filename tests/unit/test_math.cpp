#include <gtest/gtest.h>
#include "math/Vec2.h"

using namespace Forge;

TEST(Vec2Test, DefaultConstructor) {
    Vec2 v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vec2Test, ParameterizedConstructor) {
    Vec2 v(1.5f, -2.5f);
    EXPECT_FLOAT_EQ(v.x, 1.5f);
    EXPECT_FLOAT_EQ(v.y, -2.5f);
}

TEST(Vec2Test, Addition) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    Vec2 result = v1 + v2;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}

TEST(Vec2Test, Subtraction) {
    Vec2 v1(5.0f, 6.0f);
    Vec2 v2(2.0f, 1.0f);
    Vec2 result = v1 - v2;
    EXPECT_FLOAT_EQ(result.x, 3.0f);
    EXPECT_FLOAT_EQ(result.y, 5.0f);
}

TEST(Vec2Test, ScalarMultiplication) {
    Vec2 v(2.0f, -3.0f);
    Vec2 result = v * 2.0f;
    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, -6.0f);

    Vec2 result2 = 3.0f * v;
    EXPECT_FLOAT_EQ(result2.x, 6.0f);
    EXPECT_FLOAT_EQ(result2.y, -9.0f);
}

TEST(Vec2Test, ScalarDivision) {
    Vec2 v(4.0f, -6.0f);
    Vec2 result = v / 2.0f;
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, -3.0f);
}

TEST(Vec2Test, DotProduct) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v1.dot(v2), 11.0f);
}

TEST(Vec2Test, CrossProduct) {
    Vec2 v1(1.0f, 0.0f);
    Vec2 v2(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(v1.cross(v2), 1.0f);
}

TEST(Vec2Test, Length) {
    Vec2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.lengthSquared(), 25.0f);
    EXPECT_FLOAT_EQ(v.length(), 5.0f);
}

TEST(Vec2Test, Normalize) {
    Vec2 v(3.0f, 4.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.length(), 1.0f);
    EXPECT_FLOAT_EQ(v.x, 3.0f / 5.0f);
    EXPECT_FLOAT_EQ(v.y, 4.0f / 5.0f);
}

TEST(Vec2Test, NormalizeZeroVector) {
    Vec2 v(0.0f, 0.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vec2Test, NormalizeUnitVector) {
    Vec2 v(1.0f, 0.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vec2Test, Lerp) {
    Vec2 v1(0.0f, 0.0f);
    Vec2 v2(10.0f, 20.0f);
    Vec2 result = v1.lerp(v2, 0.5f);
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 10.0f);
}

TEST(Vec2Test, EqualityWithTolerance) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(1.0f + 1e-6f, 2.0f - 1e-6f);
    EXPECT_TRUE(v1 == v2);
}