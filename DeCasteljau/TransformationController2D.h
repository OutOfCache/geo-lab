#pragma once
#include <cogra/types.h>
namespace cogra::ui
{
class TransformationController2D
{
public:
    TransformationController2D();

    void startTranslation(const cogra::f32vec2& position);

    bool updateTranslation(const cogra::f32vec2& position);

    void endTranslation();

    void startScale(const cogra::f32vec2 position);

    bool updateScale(const cogra::f32vec2 position);

    void endScale();

    cogra::f32mat3 transformation() const;

    cogra::float32 getScaleFactor() const;

private:
    cogra::f32mat3 translationMatrix() const;

    cogra::f32mat3 scaleMatrix() const;

    cogra::f32vec2  m_translationVector;

    bool            m_isInTranslation;

    cogra::f32vec2  m_previousTranslationMousePosition;

    cogra::float32  m_scaleFactor;

    bool            m_isInScale;

    cogra::f32vec2  m_previousScaleMousePosition;
};
}
