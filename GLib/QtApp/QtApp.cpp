#include "QtApp.h"
#include "QFileDialog"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

QtApp::QtApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    showMaximized();

    AddMenu();
    AddMainWindow();
}

QtApp::~QtApp()
{}

void QtApp::AddMenu() {
    QToolBar* menu = addToolBar("Menu");
    QPushButton* but_OpenFile = new QPushButton("Open File", this);

    menu->addWidget(but_OpenFile);
    connect(but_OpenFile, &QPushButton::clicked, this, &QtApp::OpenFileDialog);

    // ��� �������� ������ �� �����
    // ���������� ������
    // ������ ��� ������ ������������ ��� ��������. GLbegin, glEnd
    
}

void QtApp::AddMainWindow() {
    MainWindow* widget = new MainWindow(this);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(2, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    widget->setFormat(format);

    setCentralWidget(widget);
}

void QtApp::OpenFileDialog() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");
    if (fileName.isEmpty()) {
        qDebug() << "The file is not selected";
    }
}
