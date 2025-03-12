#pragma once

#include <QWheelEvent>
#include "../GLib/LibVector.h"
#include "../GLib/LibMatrix.h"

class Camera
{
public:
    Camera();

    void Scale(double coef);

    void Translation(const LibVector<double>& vec);

    LibMatrix<double> ApplyTransform() const;

private:
    double m_scale;
    LibVector<double> m_translation;
    LibVector<double> m_axis;
    double m_angle;
};

