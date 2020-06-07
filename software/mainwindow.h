#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

#if DOWNLOADs
    void Download(QString URL);
    DownloadManager manager;
void Upload(QString URL,QString User,QString Password,QString port);

#endif

    int adminftp=0;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ftpServerbtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
