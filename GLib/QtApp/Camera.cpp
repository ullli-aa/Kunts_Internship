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
    LibPoint<double> minPoint = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    LibPoint<double> maxPoint = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };

    for (const auto& point : model.Points()) {
        if (point.X() < minPoint.X()) {
            minPoint.SetX(point.X());
        }
        if (point.Y() < minPoint.Y()) {
            minPoint.SetY(point.Y());
        }
        if (point.Z() < minPoint.Z()) {
            minPoint.SetZ(point.Z());
        }

        if (point.X() > maxPoint.X()) {
            maxPoint.SetX(point.X());
        }
        if (point.Y() > maxPoint.Y()) {
            maxPoint.SetY(point.Y());
        }
        if (point.Z() > maxPoint.Z()) {
            maxPoint.SetZ(point.Z());
        }
    }

    LibVector<double> delta = maxPoint - minPoint;

    double maxDelta = std::max(delta.X(), delta.Y());

    m_Scale = aspectRatio;
    if (maxDelta != 0) {
        m_Scale /= maxDelta;
    }
    m_Translation = (-1) * delta / 2;

    Update();
}

void Camera::Resize(int w, int h)
{
    aspectRatio = static_cast<double>(h) / static_cast<double>(w);
    m_Width = w;
    m_Height = h;
    glOrtho(-1.0, 1.0, -aspectRatio, aspectRatio, -1.0, 1.0);
}

LibPoint<double> Camera::PxlToScrn(int x_px, int y_px) const
{
    double x_scr = (2 * static_cast<double>(x_px) / m_Width - 1);
    double y_scr = (2 * static_cast<double>(y_px) / m_Height - 1) * aspectRatio;

    return LibPoint<double>(x_scr, -y_scr, 0);
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

LibPoint<double> Camera::PxlToMdl(int x_px, int y_px)
{
    LibPoint<double> scrn = PxlToScrn(x_px, y_px);
    return LibMatrix<double>::MultPt(scrn, ScrnToMdl());
}

void Camera::Scale(double coef)
{
    m_Scale *= coef;
    Update();
}

void Camera::Translation(int begX, int begY, int endX, int endY)
{
    LibVector<double> deltaModel = PxlToMdl(endX, endY) - PxlToMdl(begX, begY);
    m_Translation += deltaModel;
    Update();
}

//void Camera::Rotation(const LibVector<double>& vec, double angle)
//{
//    m_axis = vec;
//    m_angle = angle;
//}

const double* Camera::Apply() const
{
    return MdlToScrn().Data();
}

