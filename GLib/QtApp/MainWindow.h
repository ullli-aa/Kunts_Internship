#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MainWindow : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr) : QOpenGLWidget(parent) {}

protected:
	void initializeGL() override {
		initializeOpenGLFunctions();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

