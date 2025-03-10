#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtApp.h"
#include "MainWindow.h"

class QtApp : public QMainWindow
{
    Q_OBJECT

public:
    QtApp(QWidget *parent = nullptr);
    ~QtApp();

    void AddMenu();
    void AddMainWindow();

private slots:
    void OpenFileDialog();

private:
    Ui::QtAppClass ui;
};
