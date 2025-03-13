#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();
}

void MainWindow::LoadModel(const std::string& filePath)
{
    std::ifstream in(filePath, std::ios::binary);
    if (!in.is_open()) {
        qWarning() << "Can't open file: " << filePath.c_str();
        return;
    }

    if (!model.Points().empty()) {
        model.Clear();
    }

    model.Load(in);
    in.close();
    m_camera.Init(model);

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

    m_camera.Resize(w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadMatrixd(m_camera.Apply());

    PaintModel<double>();

    glFlush();
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        m_camera.Scale(scale);
    }
    else {
        m_camera.Scale(1 / scale);
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_camera.Translation(m_lastMousePos.x(), m_lastMousePos.y(), event->pos().x(), event->pos().y());

        m_lastMousePos = event->pos();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}
