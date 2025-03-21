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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    GLfloat lmodel_ambient[] = { 0.2f,0.2f,0.2f,1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GLfloat light_positions[8][4] = {
        {-1.0f, -1.0f, -1.0f, 0.0f},
        {-1.0f, 1.0f, -1.0f, 0.0f},
        {1.0f, 1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, -1.0f, 0.0f},
        {-1.0f, -1.0f, 1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, -1.0f, 1.0f, 0.0f}
    };

    GLfloat diffuse_color[] = {0.2f, 0.2f, 0.2f, 1.0f};

    for (int i = 0; i < 8; i++) {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light_positions[i]);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse_color);
    }
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

    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

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
    if (m_lastMousePos != event->pos()) {
        if (m_isDragTransl) {
            QPoint delta = event->pos() - m_lastMousePos;
            if (m_camera.IsIntersRayWithModel(event->pos().x(), event->pos().y())) {
                m_camera.Translation(delta.x(), delta.y());
            }
        }
        else if (m_isDragRotat) {
            m_camera.Rotation(m_lastMousePos.x(), m_lastMousePos.y(), event->pos().x(), event->pos().y());
        }
        m_lastMousePos = event->pos();
        update();
    }
    
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragTransl = false;
    } else if (event->button() == Qt::RightButton) {
        m_isDragRotat = false;
    }
}

void MainWindow::PaintModel()
{
    if (!m_model.Triangles().empty() && !m_model.Points().empty()) {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < m_model.TrinaglesNum(); i++)
        {
            const LibPoint<double>& A = m_model.GetPtInTrngl(i, 0);
            const LibPoint<double>& B = m_model.GetPtInTrngl(i, 1);
            const LibPoint<double>& C = m_model.GetPtInTrngl(i, 2);

            const LibVector<double>& nA = m_model.GetNrmlsInTrngl(i, 0);
            const LibVector<double>& nB = m_model.GetNrmlsInTrngl(i, 0);
            const LibVector<double>& nC = m_model.GetNrmlsInTrngl(i, 0);

            glNormal3d(nA.X(), nA.Y(), nA.Z());
            glVertex3d(A.X(), A.Y(), A.Z());

            glNormal3d(nB.X(), nB.Y(), nB.Z());
            glVertex3d(B.X(), B.Y(), B.Z());

            glNormal3d(nC.X(), nC.Y(), nC.Z());
            glVertex3d(C.X(), C.Y(), C.Z());

        }
        glEnd();
    }
}
