#include "Camera.h"
#include <QOpenGLFunctions>

Camera::Camera() {
    m_Scale = 1;
    m_Translation = LibVector<double>(0, 0, 0);
    m_Rotation = LibMatrix<double>::IdentityMatrix();

    m_ModelToScreen = LibMatrix<double>::IdentityMatrix();
    m_ScreenToModel = LibMatrix<double>::IdentityMatrix();
}

void Camera::Init(const LibModel<double>& model)
{
    m_model = model;
    LibVector<double> diag = model.Diagonal();

    double maxDelta = diag.LengthVector();

    m_Scale = aspectRatio;
    if (maxDelta != 0) {
        m_Scale /= maxDelta;
    }
    m_Translation = (-1) * diag / 2;

    Update();
}

void Camera::Resize(int w, int h)
{
    aspectRatio = static_cast<double>(h) / static_cast<double>(w);
    m_Width = w;
    m_Height = h;
    glOrtho(-1.0, 1.0, -aspectRatio, aspectRatio, -1.0, 1.0);
}

LibPoint<double> Camera::PxlToScrnPt(int x_px, int y_px) const
{
    double x_scr = (2 * static_cast<double>(x_px) / m_Width - 1);
    double y_scr = (2 * static_cast<double>(y_px) / m_Height - 1) * aspectRatio;

    return LibPoint<double>(x_scr, -y_scr, 0);
}

LibVector<double> Camera::PxlToScrnVec(int vec_px, int vec_py) const
{
    LibPoint<double> scr = PxlToScrnPt(vec_px + m_Width / 2, vec_py + m_Height / 2);
    return scr.AsVector();
}

const LibMatrix<double>& Camera::MdlToScrn() const
{
    return m_ModelToScreen;
}

const LibMatrix<double>& Camera::ScrnToMdl() const
{
    return m_ScreenToModel;
}

void Camera::Update()
{
    m_ModelToScreen = LibMatrix<double>::TranslationInit(m_Translation) *
        LibMatrix<double>::ScalingInit(LibVector<double>(m_Scale, m_Scale, m_Scale)) *
        m_Rotation;
    m_ScreenToModel = m_ModelToScreen.InverseCopy();
}

LibPoint<double> Camera::PxlToMdlPt(int x_px, int y_px)
{
    LibPoint<double> scrn = PxlToScrnPt(x_px, y_px);
    return LibMatrix<double>::MultPt(scrn, ScrnToMdl());
}

LibVector<double> Camera::PxlToMdlVec(int x_px, int y_px)
{
    LibVector<double> scr = PxlToScrnVec(x_px, y_px);
    return LibMatrix<double>::MultVec(scr, ScrnToMdl());
}

LibRay<double> Camera::GetRayFromPx(int x_px, int y_px)
{
    LibPoint<double> origin = PxlToMdlPt(x_px, y_px);
    origin.SetZ(-2 * m_model.Diagonal().LengthVector());

    LibVector<double> direction = LibMatrix<double>::MultVec(LibVector<double>(0, 0, 1), m_ScreenToModel);
    direction.NormalizeThis();

    return LibRay<double>(origin, direction);
}

bool Camera::IsIntersRayWithModel(int x_px, int y_px)
{
    LibPoint<double> pt;
    LibModel<double>::Surface srfc;
    return m_model.IsIntersectionRay(GetRayFromPx(x_px, y_px), pt, srfc);
}

void Camera::Scale(double coef)
{
    m_Scale *= coef;
    Update();
}

void Camera::Translation(int deltaX, int deltaY)
{
    LibVector<double> delta = PxlToMdlVec(deltaX, deltaY);
    m_Translation += delta;
    Update();
}

//void Camera::Rotation(const LibVector<double>& vec, double angle)
//{
//    m_axis = vec;
//    m_angle = angle;
//}

void Camera::Apply() const
{
    glLoadMatrixd(MdlToScrn().Data());
}


