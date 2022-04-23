#include "BaseApp2D.h"
#include <GLFW/glfw3.h>
#include <cogra/gl/OpenGLRuntimeError.h>
namespace cogra::ui
{
BaseApp2D::BaseApp2D(GLFWwindow* window) : GLFWApp(window) { }

void BaseApp2D::onKey(int32_t key, int32_t, int32_t action, int32_t)
{
    if(key == GLFW_KEY_F5 && action == GLFW_PRESS)
    {
        m_screenshot.triggerSaveToClipboard();
    }

    if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        setIsUIEnabled(!isUIEnabled());
    }
}

void BaseApp2D::onPostDraw()
{
    m_screenshot.saveToClipboardIfTriggered();
}


void BaseApp2D::onMouseButton(int32_t button, int32_t action, int32_t mods)
{

    if(button == GLFW_MOUSE_BUTTON_2)
    {
        if(action == GLFW_PRESS)
        {
            const auto d = getNormalizedMousePosition();
            m_transformationController.startTranslation(d);
        }
        else
        {
            m_transformationController.endTranslation();
        }
    }

    if(button == GLFW_MOUSE_BUTTON_3)
    {
        if(action == GLFW_PRESS)
        {
            const auto d = getNormalizedMousePosition();
            m_transformationController.startScale(d);
        }
        else
        {
            m_transformationController.endScale();
        }
    }
}


void BaseApp2D::onCursorPosition(float64, float64)
{
    const auto d = getNormalizedMousePosition();
    m_transformationController.updateTranslation(d);
    m_transformationController.updateScale(d);
}


f32mat3 BaseApp2D::getCameraTransformation() const
{
    return m_transformationController.transformation();
}


void BaseApp2D::onFramebufferSize(int width, int height)
{
    GL_SAFE_CALL(glViewport(0, 0, width, height));
    m_aspectCorrection = (width < height) ? f32vec2(1.0f, static_cast<float>(width) / static_cast<float>(height)) : f32vec2(static_cast<float>(height) / static_cast<float>(width), 1.0f);
}


f32mat3 BaseApp2D::getAspectCorrectionScale() const
{
    return f32mat3(m_aspectCorrection.x, 0, 0, 0, m_aspectCorrection.y, 0, 0, 0, 1);
}


float32 BaseApp2D::getScaleFactor() const
{
    return m_transformationController.getScaleFactor();
}
}