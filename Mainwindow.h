#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

class MatrixTab;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget *m_tabWidget;
    MatrixTab *m_matrixTab;
    QWidget* createPlaceholder(const QString& text);
};

#endif // MAINWINDOW_H