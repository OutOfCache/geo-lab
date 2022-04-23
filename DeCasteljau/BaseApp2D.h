#pragma once
#include <cogra/types.h>
#include <cogra/ui/GLFWApp.h>
#include <cogra/ui/GLFWWindow.h>
#include "TransformationController2D.h"
#include <cogra/gl/Screenshot.h>
namespace cogra::ui
{
class BaseApp2D : public GLFWApp
{
public:
	BaseApp2D(GLFWwindow* window);

	void onKey(int32_t key, int32_t, int32_t action, int32_t) override;

	void onPostDraw();

	void onMouseButton(int32_t button, int32_t action, int32_t mods) override;

	void onCursorPosition(float64, float64) override;

	f32mat3 getCameraTransformation() const;

	void onFramebufferSize(int width, int height) override;

	f32mat3 getAspectCorrectionScale() const;

	float32 getScaleFactor() const;

private:

	cogra::gl::Screenshot               m_screenshot;

	cogra::ui::TransformationController2D    m_transformationController;

	f32vec2                             m_aspectCorrection;
};


}