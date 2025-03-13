#pragma once

#include <QWheelEvent>
#include "../GLib/LibVector.h"
#include "../GLib/LibMatrix.h"
#include "../GLib/LibModel.h"

class Camera
{
public:
    Camera();

    void Init(const LibModel<double>& model);

    void Resize(int w, int h);

    LibPoint<double> PxlToScrn(int x_px, int y_px) const;

    LibPoint<double> PxlToMdl(int x_px, int y_px);

    const LibMatrix<double>& MdlToScrn() const;

    const LibMatrix<double>& ScrnToMdl() const;

    void Update();

    void Scale(double coef);

    void Translation(int begX, int begY, int endX, int endY);

//    void Rotation(const LibVector<double>& vec, double angle);

    const double* Apply() const;

private:
    double m_Scale;
    LibVector<double> m_Translation;
    LibMatrix<double> m_Rotation;

    double aspectRatio;
    int m_Width;
    int m_Height;

    LibMatrix<double> m_ModelToScreen;
    LibMatrix<double> m_ScreenToModel;
};

