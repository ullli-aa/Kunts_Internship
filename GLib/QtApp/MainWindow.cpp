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

    if (!m_model.Points().empty()) {
        m_model.Clear();
    }

    m_model.Load(in);
    in.close();
    m_camera.Init(m_model);

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

    m_camera.Apply();

    PaintModel();

    glFlush();
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        m_camera.Scale(m_scale);
    }
    else {
        m_camera.Scale(1 / m_scale);
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragTransl = true;
        m_lastMousePos = event->pos();
    }
    else if (event->button() == Qt::RightButton) {
        m_isDragRotat = true;
        m_lastMousePos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragTransl) {
        QPoint delta = event->pos() - m_lastMousePos;
        if (m_camera.IsIntersRayWithModel(event->pos().x(), event->pos().y())) {
            m_camera.Translation(delta.x(), delta.y());
        } 
    } else if (m_isDragRotat) {
        m_camera.Rotation(m_lastMousePos.x(), m_lastMousePos.y(), event->pos().x(), event->pos().y());
    }
    m_lastMousePos = event->pos();
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragTransl = false;
    } else if (event->button() == Qt::RightButton) {
        m_isDragRotat = false;
    }
}

void MainWindow::DrawTriangle(const LibPoint<double>& A, const LibPoint<double>& B, const LibPoint<double>& C)
{
    glColor4d(1, 0, 0, 0.01);
    glVertex3d(A.X(), A.Y(), A.Z());
    glVertex3d(B.X(), B.Y(), B.Z());
    glVertex3d(C.X(), C.Y(), C.Z());
}

void MainWindow::PaintModel()
{
    if (!m_model.Triangles().empty() && !m_model.Points().empty()) {
        size_t trnglsSize = m_model.Triangles().size();

        glBegin(GL_TRIANGLES);

        for (size_t i = 0; i < trnglsSize; i += 3)
        {
            const LibPoint<double>& A = m_model.Points()[m_model.Triangles()[i]];
            const LibPoint<double>& B = m_model.Points()[m_model.Triangles()[i + 1]];
            const LibPoint<double>& C = m_model.Points()[m_model.Triangles()[i + 2]];

            DrawTriangle(A, B, C);
        }
        glEnd();

    }
}
