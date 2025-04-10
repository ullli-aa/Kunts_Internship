#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();
    setMouseTracking(true);
}

void MainWindow::LoadModel(const std::wstring& filePath)
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
    m_upd = true;

    update();
}

void MainWindow::SetModel(const LibModel<double>& mdl)
{
    m_model.SetModel(mdl);
}

void MainWindow::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    GLfloat lmodel_ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);

    glEnable(GL_NORMALIZE);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

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

    GLfloat diffuse_color1[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    for (int i = 0; i < 8; i+=2) {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light_positions[i]);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse_color1);
    }

    GLfloat diffuse_color2[] = { 0.4f, 0.4f, 0.4f, 1.0f };

    for (int i = 1; i < 8; i+=2) {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light_positions[i]);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse_color2);
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
    if (m_upd) {
        m_upd = false;

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        std::vector<unsigned int> tr(3 * m_model.TrinaglesNum());
        for (size_t i = 0; i < m_model.Triangles().size(); i++)
        {
            tr[i] = m_model.Triangles()[i];
        }

        glGenBuffers(1, &m_vboTriangles);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboTriangles);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_model.Triangles().size() * sizeof(unsigned int),
            tr.data(), GL_STATIC_DRAW);

        std::vector<double> vertData(m_model.Points().size() * 3);
        for (size_t i = 0; i < m_model.Points().size(); i++)
        {
            vertData[3 * i] = m_model.Points()[i].X();
            vertData[3 * i + 1] = m_model.Points()[i].Y();
            vertData[3 * i + 2] = m_model.Points()[i].Z();
        }

        glGenBuffers(1, &m_vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(double),
            (void*)vertData.data(), GL_STATIC_DRAW);

        std::vector<double> nrmlsData(m_model.Normals().size() * 3);
        for (size_t i = 0; i < m_model.Normals().size(); i++)
        {
            nrmlsData[3 * i] = m_model.Normals()[i].X();
            nrmlsData[3 * i + 1] = m_model.Normals()[i].Y();
            nrmlsData[3 * i + 2] = m_model.Normals()[i].Z();
        }

        glGenBuffers(1, &m_vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
        glBufferData(GL_ARRAY_BUFFER, nrmlsData.size() * sizeof(double),
            (void*)nrmlsData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_DOUBLE, 0, NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboTriangles);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    m_camera.Apply();

    GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glDrawElements(GL_TRIANGLES, m_model.Triangles().size(), GL_UNSIGNED_INT, 0);

    if (m_indSurfSel != -1) {
        GLfloat mat_ambient2[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        GLfloat mat_diffuse2[] = { 1.0f, 0.0f, 0.0f, 1.0f };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient2);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
        LibModel<double>::Surface srfc = m_model.Surfaces()[m_indSurfSel];

        size_t size = 3 * (srfc.End() - srfc.Begin());

        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)(srfc.Begin() * 3 * sizeof(unsigned int)));
        m_indSurfSel = -1;
    }
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
    m_pressed = true;
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    m_camera.IsIntersRayWithModel(event->pos().x(), event->pos().y(), m_indSurfSel);
    if (m_lastMousePos != event->pos()) {
        if (m_isDragTransl) {
            QPoint delta = event->pos() - m_lastMousePos;
            int srfc;
            if (m_camera.IsIntersRayWithModel(event->pos().x(), event->pos().y(), srfc)) {
                m_camera.Translation(delta.x(), delta.y());
            }
        }
        else if (m_isDragRotat) {
            m_camera.Rotation(m_lastMousePos.x(), m_lastMousePos.y(), event->pos().x(), event->pos().y());
        }
        m_lastMousePos = event->pos();
    }
    update();
    
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragTransl = false;
    } else if (event->button() == Qt::RightButton) {
        m_isDragRotat = false;
    }
    m_pressed = false;
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
