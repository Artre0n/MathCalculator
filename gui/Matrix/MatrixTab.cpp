#include "MatrixTab.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QHeaderView>
#include <QFrame>
#include <QInputDialog>


#pragma region Стили оформления интерфейса


/// Стиль групп (рамки вокруг матриц)
static const char* GROUP_STYLE = R"(
    QGroupBox {
        font-weight: bold;
        border: 2px solid #e0e0e0;
        border-radius: 12px;
        margin-top: 12px;
        padding: 12px;
        background-color: #fafafa;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 12px;
        padding: 0 8px;
        color: #424242;
        font-size: 13px;
    }
)";

/// Стиль таблиц ввода матриц
static const char* TABLE_INPUT_STYLE = R"(
    QTableWidget {
        border: 1px solid #ddd;
        border-radius: 8px;
        gridline-color: #e8e8e8;
        background-color: white;
        selection-background-color: #bbdefb;
        selection-color: #1565c0;
        font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;
        font-size: 13px;
    }
    QTableWidget::item {
        padding: 4px;
        border-bottom: 1px solid #f0f0f0;
    }
    QTableWidget::item:selected {
        background-color: #bbdefb;
    }
    QHeaderView::section {
        background-color: #f5f5f5;
        color: #616161;
        padding: 6px;
        border: none;
        border-bottom: 2px solid #e0e0e0;
        border-right: 1px solid #e0e0e0;
        font-weight: 600;
        font-size: 11px;
    }
    QTableCornerButton::section {
        background-color: #f5f5f5;
        border-bottom: 2px solid #e0e0e0;
        border-right: 1px solid #e0e0e0;
    }
)";

/// Стиль таблицы результата (зелёная)
static const char* TABLE_RESULT_STYLE = R"(
    QTableWidget {
        border: 2px solid #a5d6a7;
        border-radius: 10px;
        gridline-color: #c8e6c9;
        background-color: #e8f5e9;
        selection-background-color: #a5d6a7;
        selection-color: #1b5e20;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 14px;
        font-weight: 500;
    }
    QTableWidget::item {
        padding: 6px;
        color: #2e7d32;
    }
    QTableWidget::item:selected {
        background-color: #a5d6a7;
    }
    QHeaderView::section {
        background-color: #c8e6c9;
        color: #2e7d32;
        padding: 6px;
        border: none;
        border-bottom: 2px solid #a5d6a7;
        border-right: 1px solid #a5d6a7;
        font-weight: 600;
        font-size: 11px;
    }
    QTableCornerButton::section {
        background-color: #c8e6c9;
        border-bottom: 2px solid #a5d6a7;
        border-right: 1px solid #a5d6a7;
    }
)";

/// Стиль основной кнопки (синяя)
static const char* BTN_PRIMARY = R"(
    QPushButton {
        background-color: #2196f3;
        color: white;
        border: none;
        border-radius: 8px;
        padding: 10px 24px;
        font-weight: bold;
        font-size: 13px;
    }
    QPushButton:hover { background-color: #1976d2; }
    QPushButton:pressed { background-color: #0d47a1; }
    QPushButton:disabled { background-color: #bbdefb; color: #757575; }
)";

/// Стиль кнопки опасности (красная)
static const char* BTN_DANGER = R"(
    QPushButton {
        background-color: #ef5350;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 6px 16px;
        font-weight: 600;
        font-size: 12px;
    }
    QPushButton:hover { background-color: #c62828; }
)";

/// Стиль акцентной кнопки (оранжевая)
static const char* BTN_ACCENT = R"(
    QPushButton {
        background-color: #ff9800;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 6px 16px;
        font-weight: 600;
        font-size: 12px;
    }
    QPushButton:hover { background-color: #f57c00; }
)";

/// Стиль информационной кнопки (серая)
static const char* BTN_INFO = R"(
    QPushButton {
        background-color: #607d8b;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 6px 16px;
        font-weight: 600;
        font-size: 12px;
    }
    QPushButton:hover { background-color: #455a64; }
)";

/// Стиль статусной строки
static const char* STATUS_STYLE = R"(
    QLabel {
        padding: 10px 16px;
        border-radius: 8px;
        font-weight: 600;
        font-size: 13px;
    }
)";

/// Стиль выпадающего списка операций
static const char* COMBO_STYLE = R"(
    QComboBox {
        padding: 8px 12px;
        border: 2px solid #e0e0e0;
        border-radius: 8px;
        background-color: white;
        font-size: 13px;
        min-width: 260px;
    }
    QComboBox:hover { border-color: #2196f3; }
    QComboBox::drop-down {
        border: none;
        width: 30px;
    }
    QComboBox QAbstractItemView {
        border: 1px solid #ddd;
        border-radius: 8px;
        background-color: white;
        selection-background-color: #e3f2fd;
        padding: 4px;
    }
)";

/// Стиль спинбокса выбора степени
static const char* SPIN_STYLE = R"(
    QSpinBox {
        padding: 8px;
        border: 2px solid #e0e0e0;
        border-radius: 8px;
        background-color: white;
        font-size: 13px;
        font-weight: 600;
        min-width: 80px;
    }
    QSpinBox:hover { border-color: #2196f3; }
    QSpinBox::up-button, QSpinBox::down-button {
        width: 20px;
        background: #f5f5f5;
    }
    QSpinBox::up-button:hover, QSpinBox::down-button:hover {
        background: #e0e0e0;
    }
)";
#pragma endregion

#pragma region Конструктор и инициализация интерфейса

MatrixTab::MatrixTab(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background-color: #f5f7fa;");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    /// Заголовок
    auto* headerLayout = new QHBoxLayout();
    auto* titleLabel = new QLabel("🧮 Матричный калькулятор");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #212121;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    /// Панель управления
    auto* controlFrame = new QFrame();
    controlFrame->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #e0e0e0;");
    auto* controlLayout = new QHBoxLayout(controlFrame);
    controlLayout->setSpacing(12);
    controlLayout->setContentsMargins(16, 12, 16, 12);

    controlLayout->addWidget(new QLabel("Операция:"));
    m_operationCombo = new QComboBox(this);
    m_operationCombo->setStyleSheet(COMBO_STYLE);
    m_operationCombo->addItems({
        "Определитель det(A)",
        "Транспонирование Aᵀ",
        "Обратная матрица A⁻¹",
        "Сложение (A + B)",
        "Вычитание (A − B)",
        "Умножение (A × B)",
        "Умножение A на число",
        "Возведение A в степень"
        });
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MatrixTab::onOperationChanged);
    controlLayout->addWidget(m_operationCombo);

    controlLayout->addSpacing(20);

    m_calcBtn = new QPushButton("▶  Вычислить", this);
    m_calcBtn->setStyleSheet(BTN_PRIMARY);
    m_calcBtn->setCursor(Qt::PointingHandCursor);
    m_calcBtn->setMinimumHeight(40);
    connect(m_calcBtn, &QPushButton::clicked, this, &MatrixTab::onCalculate);
    controlLayout->addWidget(m_calcBtn);

    m_clearBtn = new QPushButton("🗑  Очистить всё", this);
    m_clearBtn->setStyleSheet(BTN_DANGER);
    m_clearBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBtn, &QPushButton::clicked, this, &MatrixTab::onClearAll);
    controlLayout->addWidget(m_clearBtn);

    m_historyBtn = new QPushButton("📜  История", this);
    m_historyBtn->setStyleSheet(BTN_INFO);
    m_historyBtn->setCursor(Qt::PointingHandCursor);
    connect(m_historyBtn, &QPushButton::clicked, this, &MatrixTab::showHistory);
    controlLayout->addWidget(m_historyBtn);

    controlLayout->addStretch();
    mainLayout->addWidget(controlFrame);

    /// Область матриц
    auto* matricesLayout = new QHBoxLayout();
    matricesLayout->setSpacing(16);

    /// Матрица A
    m_groupA = new QGroupBox("Матрица A", this);
    m_groupA->setStyleSheet(GROUP_STYLE);
    auto* layoutA = new QVBoxLayout(m_groupA);
    layoutA->setSpacing(10);

    /// Размеры A
    auto* sizeLayoutA = new QHBoxLayout();
    sizeLayoutA->addWidget(new QLabel("Строк:"));
    m_rowsSpinA = new QSpinBox(this);
    m_rowsSpinA->setStyleSheet(SPIN_STYLE);
    m_rowsSpinA->setRange(1, 10);   // Максимум 10
    m_rowsSpinA->setValue(3);
    connect(m_rowsSpinA, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MatrixTab::onResizeMatrixA);
    sizeLayoutA->addWidget(m_rowsSpinA);

    sizeLayoutA->addWidget(new QLabel("Столбцов:"));
    m_colsSpinA = new QSpinBox(this);
    m_colsSpinA->setStyleSheet(SPIN_STYLE);
    m_colsSpinA->setRange(1, 10);   // Максимум 10
    m_colsSpinA->setValue(3);
    connect(m_colsSpinA, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MatrixTab::onResizeMatrixA);
    sizeLayoutA->addWidget(m_colsSpinA);
    sizeLayoutA->addStretch();

    m_randomABtn = new QPushButton("🎲 Случайные", this);
    m_randomABtn->setStyleSheet(BTN_ACCENT);
    m_randomABtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomABtn, &QPushButton::clicked, this, &MatrixTab::onRandomFillA);
    sizeLayoutA->addWidget(m_randomABtn);

    m_clearABtn = new QPushButton("✕ Очистить", this);
    m_clearABtn->setStyleSheet(BTN_DANGER);
    m_clearABtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearABtn, &QPushButton::clicked, this, &MatrixTab::onClearA);
    sizeLayoutA->addWidget(m_clearABtn);

    layoutA->addLayout(sizeLayoutA);

    /// Таблица A
    m_inputTableA = new QTableWidget(this);
    m_inputTableA->setStyleSheet(TABLE_INPUT_STYLE);
    m_inputTableA->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_inputTableA->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_inputTableA->setAlternatingRowColors(true);
    m_inputTableA->setMinimumHeight(150);
    layoutA->addWidget(m_inputTableA, 1);

    matricesLayout->addWidget(m_groupA, 1);

    /// Матрица B
    m_groupB = new QGroupBox("Матрица B", this);
    m_groupB->setStyleSheet(GROUP_STYLE);
    auto* layoutB = new QVBoxLayout(m_groupB);
    layoutB->setSpacing(10);

    /// Размеры B
    auto* sizeLayoutB = new QHBoxLayout();
    sizeLayoutB->addWidget(new QLabel("Строк:"));
    m_rowsSpinB = new QSpinBox(this);
    m_rowsSpinB->setStyleSheet(SPIN_STYLE);
    m_rowsSpinB->setRange(1, 10);
    m_rowsSpinB->setValue(3);
    connect(m_rowsSpinB, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MatrixTab::onResizeMatrixB);
    sizeLayoutB->addWidget(m_rowsSpinB);

    sizeLayoutB->addWidget(new QLabel("Столбцов:"));
    m_colsSpinB = new QSpinBox(this);
    m_colsSpinB->setStyleSheet(SPIN_STYLE);
    m_colsSpinB->setRange(1, 10);
    m_colsSpinB->setValue(3);
    connect(m_colsSpinB, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MatrixTab::onResizeMatrixB);
    sizeLayoutB->addWidget(m_colsSpinB);
    sizeLayoutB->addStretch();

    m_randomBBtn = new QPushButton("🎲 Случайные", this);
    m_randomBBtn->setStyleSheet(BTN_ACCENT);
    m_randomBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomBBtn, &QPushButton::clicked, this, &MatrixTab::onRandomFillB);
    sizeLayoutB->addWidget(m_randomBBtn);

    m_clearBBtn = new QPushButton("✕ Очистить", this);
    m_clearBBtn->setStyleSheet(BTN_DANGER);
    m_clearBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBBtn, &QPushButton::clicked, this, &MatrixTab::onClearB);
    sizeLayoutB->addWidget(m_clearBBtn);

    layoutB->addLayout(sizeLayoutB);

    /// Таблица B
    m_inputTableB = new QTableWidget(this);
    m_inputTableB->setStyleSheet(TABLE_INPUT_STYLE);
    m_inputTableB->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_inputTableB->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_inputTableB->setAlternatingRowColors(true);
    m_inputTableB->setMinimumHeight(150);
    layoutB->addWidget(m_inputTableB, 1);

    matricesLayout->addWidget(m_groupB, 1);
    mainLayout->addLayout(matricesLayout, 3);

    /// Область результата
    auto* resultFrame = new QFrame();
    resultFrame->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #e0e0e0;");
    auto* resultLayout = new QVBoxLayout(resultFrame);
    resultLayout->setSpacing(10);
    resultLayout->setContentsMargins(16, 16, 16, 16);

    auto* resultHeader = new QHBoxLayout();
    auto* resultTitle = new QLabel("📊 Результат");
    resultTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #2e7d32;");
    resultHeader->addWidget(resultTitle);
    resultHeader->addStretch();
    resultLayout->addLayout(resultHeader);

    m_outputTable = new QTableWidget(this);
    m_outputTable->setStyleSheet(TABLE_RESULT_STYLE);
    m_outputTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_outputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_outputTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_outputTable->setAlternatingRowColors(false);
    m_outputTable->setMaximumHeight(300);
    resultLayout->addWidget(m_outputTable);

    mainLayout->addWidget(resultFrame);

    /// Статусная строка
    m_statusLabel = new QLabel("Готов к работе • Максимальный размер 10×10", this);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    /// История
    m_historyDialog = new OperationHistoryDialog(this);
    // При загрузке из истории — помещаем результат в матрицу A
    connect(m_historyDialog, &OperationHistoryDialog::loadMatrixToA,
        this, [this](const MatrixD& mat) {
            setMatrixToTable(mat, m_inputTableA);
            m_rowsSpinA->setValue(static_cast<int>(mat.getRows()));
            m_colsSpinA->setValue(static_cast<int>(mat.getCols()));
            flashStatus("Результат из истории загружен в матрицу A", "#4caf50");
        });

    // Инициализация
    onResizeMatrixA();
    onResizeMatrixB();
    onOperationChanged(0);
}
#pragma endregion

#pragma region Область таблиц матриц и обработки событий

/// Проверки для операций
bool MatrixTab::needsMatrixB(int opIndex) const {
    return (opIndex >= 3 && opIndex <= 5);  // +, −, ×
}

bool MatrixTab::needsSquareA(int opIndex) const {
    return (opIndex == 0 || opIndex == 2 || opIndex == 7);  // det, inv, power
}

/// Изменение размеров матриц
void MatrixTab::onResizeMatrixA() {
    int r = m_rowsSpinA->value();
    int c = m_colsSpinA->value();

    m_inputTableA->setRowCount(r);
    m_inputTableA->setColumnCount(c);

    // Заполняем пустые ячейки нулями
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (!m_inputTableA->item(i, j)) {
                auto* item = new QTableWidgetItem("0");
                item->setTextAlignment(Qt::AlignCenter);
                m_inputTableA->setItem(i, j, item);
            }
        }
    }
}

void MatrixTab::onResizeMatrixB() {
    int r = m_rowsSpinB->value();
    int c = m_colsSpinB->value();

    m_inputTableB->setRowCount(r);
    m_inputTableB->setColumnCount(c);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (!m_inputTableB->item(i, j)) {
                auto* item = new QTableWidgetItem("0");
                item->setTextAlignment(Qt::AlignCenter);
                m_inputTableB->setItem(i, j, item);
            }
        }
    }
}


/// Считывание матрицы из таблицы
MatrixD MatrixTab::getMatrixFromTable(QTableWidget* table) const {
    int r = table->rowCount();
    int c = table->columnCount();
    MatrixD m(r, c);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            QString txt = table->item(i, j) ? table->item(i, j)->text() : "0";
            bool ok;
            double val = txt.replace(',', '.').toDouble(&ok);
            m(i, j) = ok ? val : 0.0;
        }
    }
    return m;
}

/// Запись матрицы в таблицу
void MatrixTab::setMatrixToTable(const MatrixD& mat, QTableWidget* table) {
    table->setRowCount(static_cast<int>(mat.getRows()));
    table->setColumnCount(static_cast<int>(mat.getCols()));

    for (size_t i = 0; i < mat.getRows(); ++i) {
        for (size_t j = 0; j < mat.getCols(); ++j) {
            auto* item = new QTableWidgetItem(QString::number(mat(i, j), 'g', 6));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(static_cast<int>(i), static_cast<int>(j), item);
        }
    }
}

/// Вспышка уведомления
void MatrixTab::flashStatus(const QString& text, const QString& color) {
    m_statusLabel->setText(text);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: %1; background-color: %2;")
        .arg(color).arg(color + "20"));
}


/// Очистка
void MatrixTab::onClearAll() {
    onClearA();
    onClearB();
    m_outputTable->clearContents();
    m_outputTable->setRowCount(0);
    m_outputTable->setColumnCount(0);
    m_statusLabel->setText("Все данные очищены");
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #757575; background-color: #f5f5f5;"));
}

void MatrixTab::onClearA() {
    int r = m_rowsSpinA->value();
    int c = m_colsSpinA->value();
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            auto* item = new QTableWidgetItem("0");
            item->setTextAlignment(Qt::AlignCenter);
            m_inputTableA->setItem(i, j, item);
        }
    }
}

void MatrixTab::onClearB() {
    int r = m_rowsSpinB->value();
    int c = m_colsSpinB->value();
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            auto* item = new QTableWidgetItem("0");
            item->setTextAlignment(Qt::AlignCenter);
            m_inputTableB->setItem(i, j, item);
        }
    }
}


/// Случайное заполнение
void MatrixTab::onRandomFillA() {
    int r = m_rowsSpinA->value();
    int c = m_colsSpinA->value();
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            double val = QRandomGenerator::global()->bounded(-10, 11);
            auto* item = new QTableWidgetItem(QString::number(val));
            item->setTextAlignment(Qt::AlignCenter);
            m_inputTableA->setItem(i, j, item);
        }
    }
    flashStatus("Матрица A заполнена случайными значениями", "#ff9800");
}

void MatrixTab::onRandomFillB() {
    int r = m_rowsSpinB->value();
    int c = m_colsSpinB->value();
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            double val = QRandomGenerator::global()->bounded(-10, 11);
            auto* item = new QTableWidgetItem(QString::number(val));
            item->setTextAlignment(Qt::AlignCenter);
            m_inputTableB->setItem(i, j, item);
        }
    }
    flashStatus("Матрица B заполнена случайными значениями", "#ff9800");
}


/// Изменение операции
void MatrixTab::onOperationChanged(int index) {
    bool needB = needsMatrixB(index);
    bool needSquare = needsSquareA(index);

    m_groupB->setVisible(needB);

    // Подсказка в статус-баре
    QString hint;
    switch (index) {
    case 0: hint = "Определитель: требуется квадратная матрица A"; break;
    case 1: hint = "Транспонирование: строки ↔ столбцы"; break;
    case 2: hint = "Обратная матрица: требуется квадратная невырожденная A"; break;
    case 3: hint = "Сложение: A и B должны быть одинакового размера"; break;
    case 4: hint = "Вычитание: A и B должны быть одинакового размера"; break;
    case 5: hint = "Умножение: столбцы A = строки B"; break;
    case 6: hint = "Умножение на число: введите множитель"; break;
    case 7: hint = "Степень: требуется квадратная матрица A"; break;
    }
    m_statusLabel->setText(hint);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
}

/// Показать историю
void MatrixTab::showHistory() {
    m_historyDialog->show();
    m_historyDialog->raise();
    m_historyDialog->activateWindow();
}
#pragma endregion

#pragma region Вычисление результата

/// Основная функция вычисления результата
void MatrixTab::onCalculate() {
    try {
        int op = m_operationCombo->currentIndex();
        MatrixD mA = getMatrixFromTable(m_inputTableA);
        QString operationName = m_operationCombo->currentText();
        QString msg;
        bool hasMatrixB = false;
        MatrixD mB;
        MatrixD res;

        // Проверка квадратности для det, inv, power
        if (needsSquareA(op) && !mA.isSquare()) {
            throw std::invalid_argument("Требуется квадратная матрица A (строки = столбцы)");
        }

        if (op == 0) { // Определитель
            double d = mA.determinant();
            msg = "Определитель A: " + QString::number(d, 'g', 10);
            // Результат 1×1
            res = MatrixD(1, 1);
            res(0, 0) = d;
            setMatrixToTable(res, m_outputTable);
        }
        else if (op == 1) { // Транспонирование
            res = mA.transpose();
            setMatrixToTable(res, m_outputTable);
            msg = "Транспонирование A выполнено";
        }
        else if (op == 2) { // Обратная
            res = mA.inverse();
            setMatrixToTable(res, m_outputTable);
            msg = "Обратная матрица A найдена";
        }
        else if (op == 3) { // Сложение
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA + mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Сложение A + B выполнено";
        }
        else if (op == 4) { // Вычитание
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA - mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Вычитание A − B выполнено";
        }
        else if (op == 5) { // Умножение
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA * mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Умножение A × B выполнено";
        }
        else if (op == 6) { // Умножение на число
            bool ok;
            double scalar = QInputDialog::getDouble(this, "Умножение на число",
                "Введите множитель:", 2.0, -1e6, 1e6, 4, &ok);
            if (!ok) return;
            res = mA * scalar;
            setMatrixToTable(res, m_outputTable);
            msg = QString("Умножение A на %1 выполнено").arg(scalar);
        }
        else if (op == 7) { // Степень
            bool ok;
            int power = QInputDialog::getInt(this, "Возведение в степень",
                "Введите степень (целое число):", 2, -10, 10, 1, &ok);
            if (!ok) return;
            res = mA.power(power);
            setMatrixToTable(res, m_outputTable);
            msg = QString("A возведено в степень %1").arg(power);
        }

        // Добавление в историю
        m_historyDialog->addOperation(operationName, mA, mB, res, hasMatrixB);

        m_statusLabel->setText(msg);
        m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #2e7d32; background-color: #e8f5e9;"));

    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления", e.what());
        m_statusLabel->setText("Ошибка: " + QString(e.what()));
        m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #c62828; background-color: #ffebee;"));
    }
}
#pragma endregion