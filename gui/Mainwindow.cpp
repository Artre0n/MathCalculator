#include "MainWindow.h"
#include "Matrix/MatrixTab.h"
#include "Polynom/PolynomialTab.h"
#include "Complex/ComplexTab.h"

/// Стиль для вкладок
static const char* TABWIDGET_STYLE = R"(
    QTabWidget::pane {
        border: none;
        background-color: #f5f7fa;
        top: -1px;
    }
    QTabBar::tab {
        background-color: #e0e0e0;
        color: #616161;
        border: none;
        border-top-left-radius: 10px;
        border-top-right-radius: 10px;
        padding: 10px 20px;
        margin-right: 4px;
        font-weight: 600;
        font-size: 13px;
    }
    QTabBar::tab:hover {
        background-color: #eeeeee;
        color: #424242;
    }
    QTabBar::tab:selected {
        background-color: white;
        color: #2196f3;
        border-bottom: 3px solid #2196f3;
    }
    QTabBar::tab:!selected {
        margin-top: 3px;
    }
)";

/// Конструктор
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Математический калькулятор");
    resize(1000, 750);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setStyleSheet(TABWIDGET_STYLE);
    m_tabWidget->setDocumentMode(true);

    // Вкладка 1: Матрицы
    m_matrixTab = new MatrixTab(this);
    m_tabWidget->addTab(m_matrixTab, "🧮  Матрицы");

    // Вкладка 2: Полиномы
    m_polynomialTab = new PolynomialTab(this);
    m_tabWidget->addTab(m_polynomialTab, "🔢  Полиномы");

    // Вкладка 3: Комплексные числа
    m_complexTab = new ComplexTab(this);
    m_tabWidget->addTab(m_complexTab, "🔣  Комплексные");

    setCentralWidget(m_tabWidget);
}

/// Деструктор
MainWindow::~MainWindow() {}