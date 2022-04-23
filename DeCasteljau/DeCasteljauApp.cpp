/// Cogra --- Coburg Graphics Framework 201
/// (C) 2017 by Quirin Meyer
/// quirin.meyer@hs-coburg.de
#include <iostream>
#include <array>
#include <glad/glad.h>
#include <cogra/types.h>

#include <cogra/gl/GLSLProgram.h>
#include <cogra/ui/GLFWApp.h>
#include <cogra/ui/GLFWWindow.h>
#include <cogra/graphics/drawable/PolyLineDrawable.h>

#include <cogra/ui/PointDragger.h>
#include "BezierCurve.h"
#include "BezierSpline.h"

#include <imgui/imgui.h>
#include <algorithm>
#include "BaseApp2D.h"

using cogra::ui::GLFWWindow;
using cogra::ui::GLFWWindowConfig;
using cogra::ui::PointDragger;
using namespace cogra::graphics::drawable;


namespace
{
cogra::f32vec2 transformPoint(f32mat3 m, f32vec2 p)
{
    f32vec3 p3(p.x, p.y, 1.0f);
    const auto t = glm::inverse(m) * p3;
    return f32vec2(t.x, t.y);
}
}

namespace cogra::gmca
{
/// <summary>
/// An appllication that is draws 2D Bezier curves.
/// </summary>
class DeCasteljauApp : public cogra::ui::BaseApp2D
{
public:
    explicit DeCasteljauApp(GLFWwindow* window)
        : BaseApp2D(window)
        , m_drawCurveProgram("../shaders/drawCurve.vert.glsl", "../shaders/drawCurve.geom.glsl", "../shaders/drawCurve.frag.glsl")
        , m_drawPointsProgram("../shaders/drawPoints.vert.glsl", "../shaders/drawPoints.geom.glsl", "../shaders/drawPoints.frag.glsl")

    {        
        updateCurveInfoUI();
        updateCurve();
        GL_SAFE_CALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
        onFramebufferSize(this->getFramebufferWidth(), this->getFramebufferHeight());
    }

    ~DeCasteljauApp() override = default;

private:

    //! The GPU program that draws the curve.
    cogra::gl::GLSLProgram                                              m_drawCurveProgram;

    //! The GPU program that draws the points
    cogra::gl::GLSLProgram                                              m_drawPointsProgram;

    //! The drawable that holds GPU data for drawing the curve.
    std::vector<PolyLineDrawable>                                       m_lineDrawable;

    std::vector<PolyLineDrawable>                                       m_controlNetMesh;

    std::vector<PolyLineDrawable>                                       m_deCasteljauMeshes;

    PointDragger                                                        m_pointDragger;

    cogra::gmca::BezierSpline                                           m_bezierSpline;

    //! Data obtained by the user inteface.
    struct UIData
    {
        int32 selectedCurveIndex = 0;

        //! Center of the cruve
        std::vector<f32vec2>* controlPoints;
       

        std::vector<bool> showDecasteljau;

        std::vector<float32> sampleValueDeCasteljau;

        //! Number of samples that is used to sample the curve.
        int32 nSamples = 64;

        //! The linewidth in pixels that is used to draw the curve.
        float32 curveLineWidth = 16.0f;

        float32 controlPolygonLineWidth = 10.0f;

        //! Point size of the control coefficients.
        float32 controlPointSize = 32;

        //! A type for selecting the curve.
        enum CurveType : int32 { Monomial, Lagrange, Bezier};

        bool showControlPolygon = true;

        bool showControlPoints = true;

        bool showCurve = true;

        f32vec3 curveColor = f32vec3(0.0f, 0.0f, 0.0f);

        f32vec3 controlPolygonColor = f32vec3(0.5f, 0.5f, 0.5f);

        f32vec3 controlPointColor = f32vec3(0.5f, 0.5f, 0.5f);        

        bool tieParameters = true;
    };

    //! The ui data.
    UIData m_uiData;

public:
    
    void onMouseButton(int32_t button, int32_t action, int32_t mods) override
    {
        BaseApp2D::onMouseButton(button, action, mods);

        if(button == GLFW_MOUSE_BUTTON_1)
        {
            if(action == GLFW_PRESS)
            {
                const auto d = getNormalizedMousePosition();
                m_pointDragger.onMouseDown(transformPoint(getAspectCorrectionScale() * getCameraTransformation(),
                    f32vec2(static_cast<float32>(d.x), static_cast<float32>(d.y))),
                    *m_uiData.controlPoints,
                    m_uiData.controlPointSize * 2.0f / glm::max(getFramebufferDimensions().x, getFramebufferDimensions().y) / getScaleFactor());
            }
            else
            {
                m_pointDragger.onMouseUp();
            }
        }
    }

    void onCursorPosition(float64 xpos, float64 ypos) override
    {
        BaseApp2D::onCursorPosition(xpos, ypos);
        const auto d = getNormalizedMousePosition();
        if(m_pointDragger.onMouseMove(transformPoint(getAspectCorrectionScale() * getCameraTransformation(),
            f32vec2(static_cast<float32>(d.x), static_cast<float32>(d.y))),
           *m_uiData.controlPoints))
        {
            updateCurve();
        }
    }

    /// <summary>
    /// Draw the curve.
    /// </summary>
    void onDraw() override
    {
		// Clear the window.
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT));
		const auto pixelScale = (2.0f / std::min(getFramebufferWidth(), getFramebufferHeight()));
		const auto a = getAspectCorrectionScale();
		const auto t = getCameraTransformation();
		const auto m = a * t;

        // Use the draw curve shader to draw the program.

        if(m_uiData.showControlPoints)
        {
            m_drawPointsProgram.use();
            m_drawPointsProgram.setUniform("u_transformationMatrix", m);
            m_drawPointsProgram.setUniform("u_color", m_uiData.controlPointColor);
            m_drawPointsProgram.setUniform("u_radius", 0.5f * m_uiData.controlPointSize * f32vec2(2.0f / getFramebufferWidth(), 2.0f / getFramebufferHeight()));
            for(auto& c : m_controlNetMesh)
            {
                c.setPrimitiveType(PolyLineDrawable::Points);
                c.draw();
            }
        }

        if(m_uiData.showControlPolygon)
        {
            m_drawCurveProgram.use();
            m_drawCurveProgram.setUniform("u_color", m_uiData.controlPolygonColor);
            m_drawCurveProgram.setUniform("u_transformationMatrix", m);
           // m_drawCurveProgram.setUniform("u_halfLineWidth", 0.5f * m_uiData.curveLineWidth * pixelScale);
            m_drawCurveProgram.setUniform("u_halfLineWidth", 0.5f * m_uiData.curveLineWidth * pixelScale);
            for(auto& c : m_controlNetMesh)
            {
                c.setPrimitiveType(PolyLineDrawable::LineStripAdjacency);
                c.draw();
            }
        }

        if(m_uiData.showCurve)
        {
            m_drawCurveProgram.use();
            m_drawCurveProgram.setUniform("u_transformationMatrix", m);
            m_drawCurveProgram.setUniform("u_color", m_uiData.curveColor);
            m_drawCurveProgram.setUniform("u_halfLineWidth", 0.5f * m_uiData.controlPolygonLineWidth * pixelScale);
            //m_drawCurveProgram.setUniform("u_halfLineWidth", 1.0f);//
            for(auto& l : m_lineDrawable)
            {
                l.setPrimitiveType(PolyLineDrawable::LineStripAdjacency);
                l.draw();
            }
        }

        std::vector<f32vec3> colors
            =
        {
            f32vec3(1,0,0),
            f32vec3(1,1,0),
            f32vec3(0,1,0),
            f32vec3(0,1,1),

            f32vec3(0,0,1),
            f32vec3(1,0,1)
        };
        for(size_t i = 0; i < m_deCasteljauMeshes.size(); i++)
        {
            if(m_uiData.showDecasteljau[i])
            {
                m_drawCurveProgram.use();
                m_drawCurveProgram.setUniform("u_color", 0.7f * colors[i%colors.size()]);
                m_drawCurveProgram.setUniform("u_halfLineWidth", 0.5f * m_uiData.controlPolygonLineWidth * pixelScale);
                m_drawCurveProgram.setUniform("u_transformationMatrix", getAspectCorrectionScale() * getCameraTransformation());

                m_deCasteljauMeshes[i].setPrimitiveType(PolyLineDrawable::LineStripAdjacency);
                m_deCasteljauMeshes[i].draw();

                m_drawPointsProgram.use();
                m_drawPointsProgram.setUniform("u_transformationMatrix", m);
                m_drawPointsProgram.setUniform("u_color", 0.7f * colors[i % colors.size()]);
                m_drawPointsProgram.setUniform("u_radius", 0.5f * m_uiData.controlPointSize * f32vec2(2.0f / getFramebufferWidth(), 2.0f / getFramebufferHeight()));
              
                m_deCasteljauMeshes[i].setPrimitiveType(PolyLineDrawable::Points);
                m_deCasteljauMeshes[i].draw();
                
            }
        }        
    }

    BezierCurve<f32vec2>& getSelectedCurve()
    {
        return m_bezierSpline.m_curves[m_uiData.selectedCurveIndex];
    }

    void updateCurveInfoUI()
    {
        BezierCurve<f32vec2>& curve = getSelectedCurve();
        const auto nControlPoints = curve.getOrder();
        m_uiData.controlPoints = &curve.getCoefficients();
        m_uiData.showDecasteljau = std::vector<bool>(nControlPoints);
        m_uiData.sampleValueDeCasteljau = std::vector<float32>(nControlPoints);
    }

    /// <summary>
    /// Draw the user interface.
    /// </summary>
    void onDrawUI() override
    {
        bool curveChanged = false;
        auto& selectedCurve = m_bezierSpline.m_curves[m_uiData.selectedCurveIndex];
        ImGui::Begin("Options");
        {
            if(ImGui::SliderInt("Curve Idx", &m_uiData.selectedCurveIndex, 0, m_bezierSpline.getNumberOfCurves() - 1))
            {                
                curveChanged = true;
                updateCurveInfoUI();
            }
            
            if(ImGui::CollapsingHeader("Control Points"))
            {
                for(size_t i = 0; i < m_uiData.controlPoints->size(); i++)
                {
                    std::string name = "C" + std::to_string(i);
                    curveChanged |= ImGui::SliderFloat2(name.c_str(), &(*m_uiData.controlPoints)[i].x, -2.0f, 2.0f);
                }
            }

            if(ImGui::CollapsingHeader("Evaluation"))
            {
                curveChanged |= ImGui::SliderInt("Number of Samples", &m_uiData.nSamples, 2, 4096);
            }

            if(ImGui::Button("Elevate Degree"))
            {               
                getSelectedCurve().elevateDegree();
                updateCurveInfoUI();
                curveChanged = true;
            }

            if(ImGui::Button("Subdivide"))
            {
                m_bezierSpline.subdivide(m_uiData.selectedCurveIndex);
                updateCurveInfoUI();
                curveChanged = true;
            }

            if(ImGui::CollapsingHeader("Rendering"))
            {
                ImGui::Checkbox("Show Curve", &m_uiData.showCurve);
                if(m_uiData.showCurve)
                {
                    ImGui::SliderFloat("Curve Width", &m_uiData.curveLineWidth, 1.0f, 32.0f);
                    ImGui::ColorEdit3("Curve Color", &m_uiData.curveColor.x);
                }

                ImGui::Checkbox("Show Control Polygon", &m_uiData.showControlPolygon);
                if(m_uiData.showControlPolygon)
                {
                    ImGui::SliderFloat("Control Polygon Width", &m_uiData.controlPolygonLineWidth, 1.0f, 32.0f);
                    ImGui::ColorEdit3("Conrol Polygon Color", &m_uiData.controlPolygonColor.x);
                }

                ImGui::Checkbox("Show Control Points", &m_uiData.showControlPoints);
                if(m_uiData.showControlPoints)
                {
                    ImGui::SliderFloat("Control Point Size", &m_uiData.controlPointSize, 1.0f, 32.0f);
                    ImGui::ColorEdit3("Control Point Color", &m_uiData.controlPointColor.x);
                }
            }

            if(ImGui::CollapsingHeader("de Casteljau"))
            {
                curveChanged |= ImGui::Checkbox("Tie Paramters", &m_uiData.tieParameters);
                if(m_uiData.tieParameters)
                {
                    curveChanged |= ImGui::SliderFloat("t", &m_uiData.sampleValueDeCasteljau[0], 0.0f, 1.0f);
                    for(size_t i = 1; i < m_uiData.sampleValueDeCasteljau.size(); i++)
                    {                        
                        m_uiData.sampleValueDeCasteljau[i] = m_uiData.sampleValueDeCasteljau[0];
                    }
                }
                else
                {
                    for(size_t i = 0; i < m_uiData.sampleValueDeCasteljau.size()-1; i++)
                    {
                        std::string name = "t" + std::to_string(i);
                        curveChanged |= ImGui::SliderFloat(name.c_str(), &m_uiData.sampleValueDeCasteljau[i], 0.0f, 1.0f);
                    }
                }
                
                for(size_t i = 0; i < m_deCasteljauMeshes.size(); i++)
                {
                    std::string name = "D" + std::to_string(i);
                    bool y = m_uiData.showDecasteljau[i];
                    curveChanged |= ImGui::Checkbox(name.c_str(), &y);
                    m_uiData.showDecasteljau[i] = y;
                }
            }
        }
        ImGui::End();

        if(curveChanged)
        {
            updateCurve();
        }
    }

private:
    /// /// <summary>
    /// Called every time the user changes parameters of the curve.
    /// </summary>
    void updateCurve()
    {                           
        m_lineDrawable.clear();
        for(int32 i = 0; i < m_bezierSpline.m_curves.size(); i++)
        {
            const auto& curve = m_bezierSpline.m_curves[i];
            std::vector<f32vec2> sampledPoints = curve.sample(m_uiData.nSamples);            
            m_lineDrawable.emplace_back(sampledPoints);
            m_lineDrawable.back().setPrimitiveType(PolyLineDrawable::LineStrip);
        }

        m_controlNetMesh.clear();
        for(int32 i = 0; i < m_bezierSpline.m_curves.size(); i++)
        {
            const auto& curve = m_bezierSpline.m_curves[i];
            m_controlNetMesh.emplace_back(curve.getCoefficients());
            m_controlNetMesh.back().setPrimitiveType(PolyLineDrawable::LineStrip);
        }
        const auto& curve = getSelectedCurve();
        
        m_deCasteljauMeshes.clear();
        auto deCasteljauPyramid = curve.deCasteljau(m_uiData.sampleValueDeCasteljau);
            
        for(auto c : deCasteljauPyramid)
        {            
            m_deCasteljauMeshes.emplace_back(c);
            m_deCasteljauMeshes.back().setPrimitiveType(PolyLineDrawable::LineStrip);
        }
    }
};
}


int main()
{
    try
    {
        GLFWWindowConfig c;
        c.width = 768;
        c.height = 768;
        c.windowTitle = "De Casteljau";
        c.glMajorVersion = 3;
        c.glMinorVersion = 3;
        c.loadGLExtensions = true;
        c.disableDeprecatedFunctions = true;
        GLFWWindow<cogra::gmca::DeCasteljauApp> app(c);
        app.run();
    }
    catch(std::exception& exception)
    {
        std::cerr << exception.what() << "\n";
    }
    return 1;
}

