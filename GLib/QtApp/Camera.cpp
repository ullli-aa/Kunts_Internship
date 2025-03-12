#include "Camera.h"

Camera::Camera() {
    m_scale = 1;
    m_translation = LibVector<double>(0, 0, 0);
    m_axis = LibVector<double>(0, 0, 1);
    m_angle = 0;
}

void Camera::Scale(double coef)
{
    m_scale *= coef;
}

void Camera::Translation(const LibVector<double>& vec)
{
    m_translation += vec;
}

LibMatrix<double> Camera::ApplyTransform() const
{

    LibMatrix<double> mtrx =
        LibMatrix<double>::TranslationInit(m_translation);

    mtrx *=
        LibMatrix<double>::ScalingInit(LibVector<double>(m_scale, m_scale, m_scale));

    

    return mtrx;
}

