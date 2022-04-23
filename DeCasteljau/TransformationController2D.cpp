#include "TransformationController2D.h"
namespace cogra::ui
{
TransformationController2D::TransformationController2D()
    : m_translationVector(0, 0)
    , m_isInTranslation(false)
    , m_previousTranslationMousePosition(0, 0)
    , m_scaleFactor(1.0f)
    , m_isInScale(false)
    , m_previousScaleMousePosition(0, 0)
{

}
void TransformationController2D::startTranslation(const cogra::f32vec2 & position)
{
    m_isInTranslation = true;
    m_previousTranslationMousePosition = position;
}
bool TransformationController2D::updateTranslation(const cogra::f32vec2 & position)
{
    if(m_isInTranslation)
    {
        const auto delta = position - m_previousTranslationMousePosition;
        m_previousTranslationMousePosition = position;
        m_translationVector += delta;
        return true;
    }
    else
    {
        return false;
    }
}
void TransformationController2D::endTranslation()
{
    m_isInTranslation = false;
}
void TransformationController2D::startScale(const cogra::f32vec2 position)
{
    m_isInScale = true;
    m_previousScaleMousePosition = position;

}
bool TransformationController2D::updateScale(const cogra::f32vec2 position)
{
    if(m_isInScale)
    {
        const auto delta = position - m_previousScaleMousePosition;
        m_previousScaleMousePosition = position;
        if(delta.y > 0)
        {
            m_scaleFactor *= 1.0f + glm::clamp(abs(delta.y), 0.0f, 1.0f);
        }
        else
        {
            m_scaleFactor *= 1.0f - glm::clamp(abs(delta.y), 0.0f, 1.0f);
        }

        return true;
    }
    else
    {
        return false;
    }
}
void TransformationController2D::endScale()
{
    m_isInScale = false;
}
cogra::f32mat3 TransformationController2D::transformation() const
{
    return translationMatrix() * scaleMatrix();
}
cogra::float32 TransformationController2D::getScaleFactor() const
{
    return m_scaleFactor;
}
cogra::f32mat3 TransformationController2D::translationMatrix() const
{
    return cogra::f32mat3(1, 0, 0,
                          0, 1, 0,
                          m_translationVector.x, m_translationVector.y, 1);
}
cogra::f32mat3 TransformationController2D::scaleMatrix() const
{
    return cogra::f32mat3(m_scaleFactor, 0, 0,
                          0, m_scaleFactor, 0,
                          0, 0, 1);
}
}