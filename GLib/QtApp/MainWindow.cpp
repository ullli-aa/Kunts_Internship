#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();
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

void MainWindow::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);

    GLfloat lmodel_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);

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

    GLfloat diffuse_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

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
    if (m_upd) {
        m_upd = false;

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        //const GLdouble* vertexData = reinterpret_cast<const GLdouble*>(m_model.Points().data());
        std::vector<GLdouble> vertData(m_model.Points().size() * 3);
        for (size_t i = 0; i < m_model.Points().size(); i++)
        {
            vertData[3 * i] = m_model.Points()[i].X();
            vertData[3 * i + 1] = m_model.Points()[i].Y();
            vertData[3 * i + 2] = m_model.Points()[i].Z();
        }

        glGenBuffers(1, &m_vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(GLdouble),
            vertData.data(), GL_STATIC_DRAW);

        //const GLdouble* normalsData = reinterpret_cast<const GLdouble*>(m_model.Normals().data());
        std::vector<GLdouble> nrmlsData(m_model.Normals().size() * 3);
        for (size_t i = 0; i < m_model.Normals().size(); i++)
        {
            nrmlsData[3 * i] = m_model.Normals()[i].X();
            nrmlsData[3 * i + 1] = m_model.Normals()[i].Y();
            nrmlsData[3 * i + 2] = m_model.Normals()[i].Z();
        }

        glGenBuffers(1, &m_vboNormals);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
        glBufferData(GL_ARRAY_BUFFER, nrmlsData.size() * sizeof(GLdouble),
            nrmlsData.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_vboTriangles);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboTriangles);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_model.Triangles().size() * sizeof(size_t),
            m_model.Triangles().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
        glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.Apply();

    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
    GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_model.Triangles().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glFinish();
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
