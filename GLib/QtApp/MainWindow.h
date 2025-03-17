#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include "Camera.h"
#include "../GLib/LibModel.h"
#include <fstream>

class MainWindow : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    void LoadModel(const std::string& filePath);

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void DrawTriangle(const LibPoint<double>& A,
        const LibPoint<double>& B,
        const LibPoint<double>& C);

    void PaintModel();
        
    LibModel<double> m_model;
    Camera m_camera;

    bool m_isDragTransl = false;
    bool m_isDragRotat = false;
    QPoint m_lastMousePos;

    const double m_scale = 1.1;
};

