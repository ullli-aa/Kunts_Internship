#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
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

private:
    template<typename T>
    void DrawTriangle(const LibPoint<T>& A, const LibPoint<T>& B, const LibPoint<T>& C) {
        glColor4d(1, 0, 0, 0.1);
        glVertex3d(A.X(), A.Y(), A.Z());
        glVertex3d(B.X(), B.Y(), B.Z());
        glVertex3d(C.X(), C.Y(), C.Z());
    }

    template<typename T>
    void PaintModel() {
        if (!model.Triangles().empty() && !model.Points().empty()) {
            size_t trnglsSize = model.Triangles().size();

            glBegin(GL_TRIANGLES);

            for (size_t i = 0; i < trnglsSize; i += 3)
            {
                LibPoint<T> A = model.Points()[model.Triangles()[i]];
                LibPoint<T> B = model.Points()[model.Triangles()[i + 1]];
                LibPoint<T> C = model.Points()[model.Triangles()[i + 2]];

                DrawTriangle(A, B, C);
            }
            glEnd();

        }
        update();
    };
        
    LibModel<double> model;
};

