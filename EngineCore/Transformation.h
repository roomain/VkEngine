#pragma once
/***********************************************
* @headerfile Transformation.h
* @date 02 / 03 / 2026
* @author Roomain
************************************************/
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#pragma warning(push)
#pragma warning( disable : 4244 )
#include <glm/gtx/matrix_decompose.hpp>
#pragma warning(pop)

class Transformation
{
private:
    glm::dmat4 m_transform;     /*!<transformation*/

    explicit Transformation(glm::dmat4&& a_transform)noexcept : m_transform{ a_transform } {}

    static inline glm::dmat4 recompose(
        const glm::dvec3& scale, const glm::dquat& orientation, const glm::dvec3& translation,
        const glm::dvec3& skew, const glm::dvec4& perspective)
    {
        glm::dmat4 m (1.);

        m[0][3] = perspective.x;
        m[1][3] = perspective.y;
        m[2][3] = perspective.z;
        m[3][3] = perspective.w;

        m *= glm::translate(translation);
        m *= glm::mat4_cast(orientation);

        if (abs(skew.x) > 0.) {
            glm::dmat4 tmp(1.f);
            tmp[2][1] = skew.x;
            m *= tmp;
        }

        if (abs(skew.y) > 0.) {
            glm::dmat4 tmp(1.f);
            tmp[2][0] = skew.y;
            m *= tmp;
        }

        if (abs(skew.z) > 0.) {
            glm::dmat4 tmp(1.f);
            tmp[1][0] = skew.z;
            m *= tmp;
        }

        m *= glm::scale(scale);

        return m;
    }

public:
    Transformation() : m_transform{ glm::identity< glm::dmat4>() } {}
    explicit Transformation(const Transformation& a_other) = default;
    Transformation(Transformation&& a_other) noexcept : m_transform{ a_other.m_transform } {}

    explicit Transformation(const glm::dvec3& a_xAxis, const glm::dvec3& a_yAxis, const glm::dvec3& a_zAxis, const glm::dvec3& a_origin)
    {
        m_transform[0] = glm::dvec4(a_xAxis, 0);
        m_transform[1] = glm::dvec4(a_yAxis, 0);
        m_transform[2] = glm::dvec4(a_zAxis, 0);
        m_transform[3] = glm::dvec4(a_origin, 1);
    }

    [[nodiscard]] inline const glm::dmat4& matrix()const noexcept { return m_transform; }
#pragma region Edition
    /////////////////////////////////////////////
    // EDITION FUNCTION
    /////////////////////////////////////////////

    inline void setPosition(const glm::dvec3& a_position)
    {
        m_transform[3] = glm::dvec4(a_position, m_transform[3][3]);
    }

    inline void setRotation(const glm::dvec3& a_axisAngles)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        rotation = glm::dquat(a_axisAngles);
        m_transform = Transformation::recompose(scale, rotation, translation, skew, perspective);
    }

    inline void setScale(const glm::dvec3& a_scale)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        scale = a_scale;
        m_transform = Transformation::recompose(scale, rotation, translation, skew, perspective);
    }

    inline void setQuaternion(const glm::dquat& a_quaternion)
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        rotation = a_quaternion;
        m_transform = Transformation::recompose(scale, rotation, translation, skew, perspective);
    }

    [[nodiscard]] inline glm::dvec3 position()const
    {
        return glm::xyz(m_transform[3]) * m_transform[3].w;
    }

    [[nodiscard]] inline glm::dvec3 rotation()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return glm::eulerAngles(rotation);
    }

    [[nodiscard]] inline glm::dvec3 scale()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return scale;
    }

    [[nodiscard]] inline glm::dquat quaternion()const
    {
        glm::dvec3 translation;
        glm::dvec3 scale;
        glm::dquat rotation;
        glm::dvec3 skew;
        glm::dvec4 perspective;
        glm::decompose(m_transform, scale, rotation, translation, skew, perspective);
        return rotation;
    }

    inline void rotateAround(const double a_angle, const glm::dvec3& a_axis, const glm::dvec3& a_target)
    {
        glm::dvec3 offset = a_target - position();
        m_transform = glm::translate(m_transform, offset);
        m_transform = glm::rotate(m_transform, a_angle, a_axis);
        m_transform = glm::translate(m_transform, -offset);
    }

#pragma endregion
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] inline Transformation& operator = (const Transformation& a_other) = default;
    [[nodiscard]] inline Transformation& operator = (Transformation&& a_other)noexcept = default;

    [[nodiscard]] friend inline Transformation operator + (const Transformation& a_transform, const Transformation& a_other)
    {
        return Transformation(a_transform.m_transform + a_other.m_transform);
    }

    [[nodiscard]] friend inline Transformation operator - (const Transformation& a_transform, const Transformation& a_other)
    {
        return Transformation(a_transform.m_transform - a_other.m_transform);
    }

    [[nodiscard]] friend inline Transformation operator * (const Transformation& a_transform, const Transformation& a_other)
    {
        return Transformation(a_transform.m_transform * a_other.m_transform);
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    [[nodiscard]] friend inline Transformation operator * (const Transformation& a_transform, const ArithmeticType a_other)
    {
        return Transformation(a_transform.m_transform * a_other);
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    [[nodiscard]] friend inline Transformation operator / (const Transformation& a_transform, const ArithmeticType a_other)
    {
        return Transformation(a_transform.m_transform / a_other);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void operator += (const Transformation& a_other)
    {
        m_transform += a_other.m_transform;
    }

    inline void operator -= (const Transformation& a_other)
    {
        m_transform -= a_other.m_transform;
    }

    inline void operator *= (const Transformation& a_other)
    {
        m_transform *= a_other.m_transform;
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    inline void operator *= (const ArithmeticType a_other)
    {
        m_transform *= a_other;
    }

    template<typename ArithmeticType> requires std::is_arithmetic_v<ArithmeticType>
    inline void operator /= (const ArithmeticType a_other)
    {
        m_transform /= a_other;
    }

};