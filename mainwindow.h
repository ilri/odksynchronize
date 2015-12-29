#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filedownloader.h"
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_cmdsalir_clicked();
    void on_cmdact_clicked();
    void fileLoaded(QString fileName,QByteArray data);

    void loadXML(QString fileName, QByteArray data);
    void replaceFiles();
    void on_cmdactxml_clicked();

    void on_cmdupd_clicked();

private:
    Ui::MainWindow *ui;
    FileDownloader *m_XMLFile;
    QList<FileDownloader *> files;
    void procError(QString text);
    QString appPath;
    QString dataPath;
};

#endif // MAINWINDOW_H
