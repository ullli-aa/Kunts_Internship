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

private slots:
    void OpenFileDialog();

private:
    void AddMenu();
    void AddMainWindow();

    Ui::QtAppClass ui;
    MainWindow* widget;
};
