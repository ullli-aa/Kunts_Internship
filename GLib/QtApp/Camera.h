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

    LibPoint<double> PxlToScrnPt(int x_px, int y_px) const;

    LibVector<double> PxlToScrnVec(int x_px, int y_px) const;

    LibPoint<double> PxlToMdlPt(int x_px, int y_px);

    LibVector<double> PxlToMdlVec(int x_px, int y_px);

    LibRay<double> GetRayFromPx(int x_px, int y_px);

    bool IsIntersRayWithModel(int x_px, int y_px, int& srfc);

    const LibMatrix<double>& MdlToScrn() const;

    const LibMatrix<double>& ScrnToMdl() const;

    void Update();

    void Scale(double coef);

    void Translation(int deltaX, int deltaY);

    void Rotation(int f_x, int f_y, int s_x, int s_y);

    void Apply() const;

private:
    LibVector<double> VecOnSphere(int x, int y, double radius2);

    double m_Scale;
    LibVector<double> m_Translation;
    LibMatrix<double> m_Rotation;

    double m_aspectRatio;
    int m_Width;
    int m_Height;

    LibMatrix<double> m_ModelToScreen;
    LibMatrix<double> m_ScreenToModel;

    LibModel<double> m_model;
};

