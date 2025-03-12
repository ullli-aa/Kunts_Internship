#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QOpenGLWidget(parent) {}

void MainWindow::LoadModel(const std::string& filePath)
{
    std::ifstream in(filePath, std::ios::binary);
    if (!in.is_open()) {
        qWarning() << "Can't open file: " << filePath.c_str();
        return;
    }

    model.Load(in);
    in.close();

    update();
}

void MainWindow::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MainWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadMatrixd(m_camera.ApplyTransform().Data());

    PaintModel<double>();

    glFlush();
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        m_camera.Scale(1.1);
    }
    else {
        m_camera.Scale(1 / 1.1);
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    const double move = 0.1;

    switch (event->key()) {
    case Qt::Key_Up:
        m_camera.Translation(LibVector<double>(0, move, 0));
        break;
    case Qt::Key_Down:
        m_camera.Translation(LibVector<double>(0, -move, 0));
        break;
    case Qt::Key_Left:
        m_camera.Translation(LibVector<double>(-move, 0, 0));
        break;
    case Qt::Key_Right:
        m_camera.Translation(LibVector<double>(move, 0, 0));
        break;
    }

    update();
}
