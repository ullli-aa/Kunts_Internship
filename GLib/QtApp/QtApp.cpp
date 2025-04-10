#include "QtApp.h"
#include "QFileDialog"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#include <sstream>

QtApp::QtApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    showMaximized();

    AddMenu();

    widget = new MainWindow(this);
    AddMainWindow();
}

QtApp::~QtApp()
{}

void QtApp::AddMenu() {
    QToolBar* menu = addToolBar("Menu");
    QPushButton* but_OpenFile = new QPushButton("Open File", this);

    menu->addWidget(but_OpenFile);
    connect(but_OpenFile, &QPushButton::clicked, this, &QtApp::OpenFileDialog);

    QPushButton* but_OpenZip = new QPushButton("Open zip", this);
    menu->addWidget(but_OpenZip);
    connect(but_OpenZip, &QPushButton::clicked, this, &QtApp::OpenZip);
}

void QtApp::AddMainWindow() {
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

    widget->LoadModel(fileName.toStdWString());
}

void QtApp::OpenZip()
{
    QString zipName = QFileDialog::getOpenFileName(this, "Choose file", "", "All files (*)");
    if (zipName.isEmpty()) {
        qDebug() << "The file is not selected";
        return;
    }
    
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));

    mz_bool status = mz_zip_reader_init_file(&zipArchive, &zipName.toStdString()[0], 0);
    if (!status) {
        qDebug() << "Failed to initialize ZIP archive.";
        return;
    }

    int pos = mz_zip_reader_locate_file(&zipArchive, "body0", nullptr, 0);
    if (pos < 0) {
        qDebug() << "File body0 not found in the archive.";
        mz_zip_reader_end(&zipArchive);
        return;
    }

    mz_zip_archive_file_stat fileInfo;
    if (!mz_zip_reader_file_stat(&zipArchive, pos, &fileInfo)) {
        qDebug() << "Failed to get file info.";
        mz_zip_reader_end(&zipArchive);
        return;
    }

    std::vector<char> buffer(fileInfo.m_uncomp_size);
    if (!mz_zip_reader_extract_to_mem(&zipArchive, pos, buffer.data(), buffer.size(), 0)) {
        qDebug() << "Failed to extract file to memory.";
        mz_zip_reader_end(&zipArchive);
        return;
    }

    mz_zip_reader_end(&zipArchive);

    std::istringstream stream(std::string(buffer.begin(), buffer.end()));

    LibModel<double> model;
    std::vector<LibPoint<double>> pts;
    std::vector<LibVector<double>> nrmls;
    std::vector<size_t> trngls;
    std::vector<LibModel<double>::Surface> srfcs;

    char header[4];
    stream.read(header, sizeof(header));

    int ptsCount;
    stream.read((char*)(&ptsCount), sizeof(int));

    bool hasNormals;
    stream.read(reinterpret_cast<char*>(&hasNormals), sizeof(hasNormals));

    pts.resize(ptsCount);
    if (hasNormals) {
        nrmls.resize(ptsCount);
    }

    int triangleCount;
    stream.read((char*)(&triangleCount), sizeof(int));
    trngls.resize(triangleCount * 3);

    int surfaceCount;
    stream.read((char*)(&surfaceCount), sizeof(int));
    srfcs.resize(surfaceCount);

    int n;
    stream.read((char*)(&n), sizeof(int));
    stream.read((char*)(&n), sizeof(int));

    double num;
    if (header[3] > '6') {
        stream.read((char*)&num, sizeof(double));
    }

    for (int i = 0; i < ptsCount; ++i) {
        stream.read((char*)&num, sizeof(double));
        pts[i].SetX(num);
        stream.read((char*)&num, sizeof(double));
        pts[i].SetY(num);
        stream.read((char*)&num, sizeof(double));
        pts[i].SetZ(num);

        if (hasNormals) {
            stream.read((char*)&num, sizeof(double));
            nrmls[i].SetX(num);
            stream.read((char*)&num, sizeof(double));
            nrmls[i].SetY(num);
            stream.read((char*)&num, sizeof(double));
            nrmls[i].SetZ(num);
        }
    }

    size_t ind;
    for (int i = 0; i < trngls.size(); ++i) {
        stream.read((char*)(&ind), sizeof(size_t));
        trngls[i] = ind;
    }

    for (int i = 0; i < srfcs.size(); ++i) {
        size_t beg;
        stream.read((char*)(&beg), sizeof(size_t));
        stream.read((char*)(&beg), sizeof(size_t));
        size_t count;
        stream.read((char*)(&count), sizeof(size_t));
        int type;
        stream.read((char*)(&type), sizeof(int));

        srfcs[i] = LibModel<double>::Surface(beg, beg + count);
    }

    model.SetPoints(pts);
    model.SetNormals(nrmls);
    model.SetTriangles(trngls);
    model.SetSurfaces(srfcs);

    widget->SetModel(model);

}