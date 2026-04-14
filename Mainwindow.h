#pragma once

#include <QMainWindow>
#include <QTabWidget>

class MatrixTab;
class PolynomialTab;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget *m_tabWidget;
    MatrixTab *m_matrixTab;
	PolynomialTab* m_polynomialTab;
    QWidget* createPlaceholder(const QString& text);
};

