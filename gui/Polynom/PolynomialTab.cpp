#include "PolynomialTab.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QHeaderView>

#pragma region Стили оформления интерфейса

/// Стиль групп (рамки вокруг полиномов)
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

/// Стиль таблиц ввода коэффициентов
static const char* TABLE_STYLE = R"(
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
        padding: 6px;
        border-bottom: 1px solid #f0f0f0;
    }
    QTableWidget::item:selected {
        background-color: #bbdefb;
    }
    QHeaderView::section {
        background-color: #f5f5f5;
        color: #616161;
        padding: 8px;
        border: none;
        border-bottom: 2px solid #e0e0e0;
        font-weight: 600;
        font-size: 12px;
    }
    QTableCornerButton::section {
        background-color: #f5f5f5;
        border-bottom: 2px solid #e0e0e0;
    }
)";

/// Стиль предпросмотра полинома (синяя плашка с формулой)
static const char* PREVIEW_STYLE = R"(
    QLabel {
        background-color: #e3f2fd;
        color: #1565c0;
        padding: 12px 16px;
        border-radius: 8px;
        border-left: 4px solid #2196f3;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 15px;
        font-weight: 500;
        min-height: 24px;
    }
)";

/// Стиль поля результата (зелёная плашка)
static const char* RESULT_STYLE = R"(
    QTextEdit {
        background-color: #e8f5e9;
        color: #2e7d32;
        border: 2px solid #a5d6a7;
        border-radius: 10px;
        padding: 12px;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 14px;
        font-weight: 500;
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
        min-width: 220px;
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

PolynomialTab::PolynomialTab(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background-color: #f5f7fa;");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);


#pragma region Заголовок и панель управления

    auto* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    auto* titleLabel = new QLabel("🔢 Полиномиальный калькулятор");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #212121;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // Панель управления (белая карточка)
    auto* controlFrame = new QFrame();
    controlFrame->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #e0e0e0;");
    auto* controlLayout = new QHBoxLayout(controlFrame);
    controlLayout->setSpacing(12);
    controlLayout->setContentsMargins(16, 12, 16, 12);

    controlLayout->addWidget(new QLabel("Операция:"));
    m_operationCombo = new QComboBox(this);
    m_operationCombo->setStyleSheet(COMBO_STYLE);
    m_operationCombo->addItems({
        "Сложение (A + B)",
        "Вычитание (A − B)",
        "Умножение (A × B)",
        "Деление (A / B)",
        "Вычисление A(x)",
        "Производная A′",
        "Интеграл ∫A dx",
        "Корни A(x) = 0"
        });
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &PolynomialTab::onOperationChanged);
    controlLayout->addWidget(m_operationCombo);

    controlLayout->addSpacing(20);

    // Кнопка "Вычислить"
    m_calcBtn = new QPushButton("▶  Вычислить", this);
    m_calcBtn->setStyleSheet(BTN_PRIMARY);
    m_calcBtn->setCursor(Qt::PointingHandCursor);
    m_calcBtn->setMinimumHeight(40);
    connect(m_calcBtn, &QPushButton::clicked, this, &PolynomialTab::onCalculate);
    controlLayout->addWidget(m_calcBtn);

    // Кнопка "Очистить всё"
    m_clearBtn = new QPushButton("🗑  Очистить всё", this);
    m_clearBtn->setStyleSheet(BTN_DANGER);
    m_clearBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBtn, &QPushButton::clicked, this, &PolynomialTab::onClearAll);
    controlLayout->addWidget(m_clearBtn);

    controlLayout->addStretch();
    mainLayout->addWidget(controlFrame);
#pragma endregion

#pragma region Область полиномов 

    auto* polyLayout = new QHBoxLayout();
    polyLayout->setSpacing(16);

    // ─── Полином A ───
    m_groupA = new QGroupBox("Полином A", this);
    m_groupA->setStyleSheet(GROUP_STYLE);
    auto* layoutA = new QVBoxLayout(m_groupA);
    layoutA->setSpacing(10);

    // Выбор степени для A
    auto* degreeLayoutA = new QHBoxLayout();
    degreeLayoutA->addWidget(new QLabel("Степень:"));
    m_degreeSpinA = new QSpinBox(this);
    m_degreeSpinA->setStyleSheet(SPIN_STYLE);
    m_degreeSpinA->setRange(0, 10);   // Максимальная степень — 10
    m_degreeSpinA->setValue(2);
    connect(m_degreeSpinA, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &PolynomialTab::onDegreeAChanged);
    degreeLayoutA->addWidget(m_degreeSpinA);
    degreeLayoutA->addStretch();

    // Кнопки управления A (случайные и очистка)
    m_randomABtn = new QPushButton("🎲 Случайные", this);
    m_randomABtn->setStyleSheet(BTN_ACCENT);
    m_randomABtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomABtn, &QPushButton::clicked, this, &PolynomialTab::onRandomFillA);
    degreeLayoutA->addWidget(m_randomABtn);

    m_clearABtn = new QPushButton("✕ Очистить", this);
    m_clearABtn->setStyleSheet(BTN_DANGER);
    m_clearABtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearABtn, &QPushButton::clicked, this, &PolynomialTab::onClearA);
    degreeLayoutA->addWidget(m_clearABtn);

    layoutA->addLayout(degreeLayoutA);

    // Предпросмотр формулы A
    auto* previewLabelA = new QLabel("Предпросмотр:");
    previewLabelA->setStyleSheet("color: #757575; font-size: 12px; font-weight: 600;");
    layoutA->addWidget(previewLabelA);

    m_previewA = new QLabel("0");
    m_previewA->setStyleSheet(PREVIEW_STYLE);
    m_previewA->setWordWrap(true);
    m_previewA->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layoutA->addWidget(m_previewA);

    // Таблица коэффициентов A
    m_tableA = new QTableWidget(this);
    m_tableA->setStyleSheet(TABLE_STYLE);
    m_tableA->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableA->verticalHeader()->setVisible(false);
    m_tableA->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableA->setAlternatingRowColors(true);
    layoutA->addWidget(m_tableA);

    polyLayout->addWidget(m_groupA, 1);

    // Полином B
    m_groupB = new QGroupBox("Полином B", this);
    m_groupB->setStyleSheet(GROUP_STYLE);
    auto* layoutB = new QVBoxLayout(m_groupB);
    layoutB->setSpacing(10);

    // Выбор степени для B
    auto* degreeLayoutB = new QHBoxLayout();
    degreeLayoutB->addWidget(new QLabel("Степень:"));
    m_degreeSpinB = new QSpinBox(this);
    m_degreeSpinB->setStyleSheet(SPIN_STYLE);
    m_degreeSpinB->setRange(0, 10);   // Максимальная степень — 10
    m_degreeSpinB->setValue(2);
    connect(m_degreeSpinB, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &PolynomialTab::onDegreeBChanged);
    degreeLayoutB->addWidget(m_degreeSpinB);
    degreeLayoutB->addStretch();

    // Кнопки управления B (случайные и очистка)
    m_randomBBtn = new QPushButton("🎲 Случайные", this);
    m_randomBBtn->setStyleSheet(BTN_ACCENT);
    m_randomBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomBBtn, &QPushButton::clicked, this, &PolynomialTab::onRandomFillB);
    degreeLayoutB->addWidget(m_randomBBtn);

    m_clearBBtn = new QPushButton("✕ Очистить", this);
    m_clearBBtn->setStyleSheet(BTN_DANGER);
    m_clearBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBBtn, &QPushButton::clicked, this, &PolynomialTab::onClearB);
    degreeLayoutB->addWidget(m_clearBBtn);

    layoutB->addLayout(degreeLayoutB);

    // Предпросмотр формулы B
    auto* previewLabelB = new QLabel("Предпросмотр:");
    previewLabelB->setStyleSheet("color: #757575; font-size: 12px; font-weight: 600;");
    layoutB->addWidget(previewLabelB);

    m_previewB = new QLabel("0");
    m_previewB->setStyleSheet(PREVIEW_STYLE);
    m_previewB->setWordWrap(true);
    m_previewB->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layoutB->addWidget(m_previewB);

    // Таблица коэффициентов B
    m_tableB = new QTableWidget(this);
    m_tableB->setStyleSheet(TABLE_STYLE);
    m_tableB->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableB->verticalHeader()->setVisible(false);
    m_tableB->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableB->setAlternatingRowColors(true);
    layoutB->addWidget(m_tableB);

    polyLayout->addWidget(m_groupB, 1);
    mainLayout->addLayout(polyLayout, 2);
#pragma endregion

#pragma region Область результата

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

    // Поле вывода результата
    m_resultEdit = new QTextEdit(this);
    m_resultEdit->setStyleSheet(RESULT_STYLE);
    m_resultEdit->setReadOnly(true);
    m_resultEdit->setPlaceholderText("Результат появится здесь...");
    m_resultEdit->setMaximumHeight(120);
    resultLayout->addWidget(m_resultEdit);

    mainLayout->addWidget(resultFrame);
#pragma endregion

#pragma region Статусная строка

    m_statusLabel = new QLabel("Готов к работе • Введите коэффициенты и выберите операцию", this);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    // Инициализация таблиц
    onDegreeAChanged(2);
    onDegreeBChanged(2);
    onOperationChanged(0);
#pragma endregion
}
#pragma endregion

#pragma region Построение таблицы коэффициентов

void PolynomialTab::buildPolynomialTable(QTableWidget* table, int degree) {
    table->clear();
    table->setRowCount(1);
    table->setColumnCount(degree + 1);

    // Формируем заголовки столбцов (от старшей степени к младшей)
    QStringList headers;
    for (int i = degree; i >= 0; --i) {
        switch (i) {
        case 10: headers << "x¹⁰"; break;
        case 9:  headers << "x⁹";  break;
        case 8:  headers << "x⁸";  break;
        case 7:  headers << "x⁷";  break;
        case 6:  headers << "x⁶";  break;
        case 5:  headers << "x⁵";  break;
        case 4:  headers << "x⁴";  break;
        case 3:  headers << "x³";  break;
        case 2:  headers << "x²";  break;
        case 1:  headers << "x¹";  break;
        case 0:  headers << "x⁰";  break;
        default: headers << QString("x^%1").arg(i); break;
        }
    }
    table->setHorizontalHeaderLabels(headers);

    // Заполняем ячейки нулями
    for (int col = 0; col <= degree; ++col) {
        auto* item = new QTableWidgetItem("0");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
        table->setItem(0, col, item);
    }

    // Ограничиваем высоту таблицы (одна строка)
    table->setMaximumHeight(70);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
#pragma endregion

#pragma region Считывание и запись полинома в таблицу

// Считывание полинома из таблицы
PolynomialD PolynomialTab::getPolynomialFromTable(QTableWidget* table) const {
    int cols = table->columnCount();
    std::vector<double> coeffs(cols);

    for (int col = 0; col < cols; ++col) {
        auto* item = table->item(0, col);
        double val = 0.0;
        if (item && !item->text().isEmpty()) {
            bool ok;
            // Заменяем запятую на точку для корректного парсинга
            val = item->text().replace(',', '.').toDouble(&ok);
            if (!ok) val = 0.0;
        }
        // Переворачиваем порядок: в таблице старшая степень слева,
        // а в Polynomial коэффициенты идут от младшей к старшей
        coeffs[cols - 1 - col] = val;
    }

    return PolynomialD(coeffs);
}


// Запись полинома в таблицу
void PolynomialTab::setPolynomialToTable(const PolynomialD& poly, QTableWidget* table) {
    const auto& coeffs = poly.getCoefficients();
    int degree = static_cast<int>(coeffs.size()) - 1;

    // Определяем, какой спинбокс степени соответствует этой таблице
    QSpinBox* spin = (table == m_tableA) ? m_degreeSpinA : m_degreeSpinB;
    spin->setValue(degree);

    // Перестраиваем таблицу под новую степень
    buildPolynomialTable(table, degree);

    // Заполняем коэффициенты
    for (int i = 0; i <= degree; ++i) {
        int col = degree - i;  // Переворачиваем: старшая степень слева
        auto* item = new QTableWidgetItem(QString::number(coeffs[i], 'g', 6));
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, col, item);
    }

    // Обновляем предпросмотр
    if (table == m_tableA) updatePreviewA();
    else updatePreviewB();
}
#pragma endregion

#pragma region Форматирование и отображение полинома

// Форматирование степеней полинома
QString PolynomialTab::polynomialToHtml(const PolynomialD& poly) const {
    QString html = polynomialToString(poly);
    // Заменяем обычные степени на Unicode-символы 
    html.replace("x^10", "x¹⁰");
    html.replace("x^0", "x⁰");
    html.replace("x^1", "x¹");
    html.replace("x^2", "x²");
    html.replace("x^3", "x³");
    html.replace("x^4", "x⁴");
    html.replace("x^5", "x⁵");
    html.replace("x^6", "x⁶");
    html.replace("x^7", "x⁷");
    html.replace("x^8", "x⁸");
    html.replace("x^9", "x⁹");
    return html;
}

// Преобразование полинома в строку
QString PolynomialTab::polynomialToString(const PolynomialD& poly) const {
    std::string str = poly.toString();
    return QString::fromStdString(str);
}

// Обновление предпросмотра полинома A
void PolynomialTab::updatePreviewA() {
    PolynomialD poly = getPolynomialFromTable(m_tableA);
    m_previewA->setText(polynomialToHtml(poly));
}

// Обновление предпросмотра полинома B
void PolynomialTab::updatePreviewB() {
    PolynomialD poly = getPolynomialFromTable(m_tableB);
    m_previewB->setText(polynomialToHtml(poly));
}

// Обработчик изменения степени полинома A
void PolynomialTab::onDegreeAChanged(int newDegree) {
    buildPolynomialTable(m_tableA, newDegree);
    // Подключаем обновление предпросмотра при изменении любой ячейки
    connect(m_tableA, &QTableWidget::itemChanged, this, &PolynomialTab::updatePreviewA, Qt::UniqueConnection);
    updatePreviewA();
    flashStatus("Степень A изменена на " + QString::number(newDegree), "#1565c0");
}

// Обработчик изменения степени полинома B
void PolynomialTab::onDegreeBChanged(int newDegree) {
    buildPolynomialTable(m_tableB, newDegree);
    connect(m_tableB, &QTableWidget::itemChanged, this, &PolynomialTab::updatePreviewB, Qt::UniqueConnection);
    updatePreviewB();
}

// Обработчик изменения операции
void PolynomialTab::onOperationChanged(int index) {
    // Операции 0-3 требуют полином B, остальные — только A
    bool needB = (index <= 3);

    if (needB) {
        m_groupB->setVisible(true);
        m_groupB->setEnabled(true);
    }
    else {
        m_groupB->setVisible(false);
    }

    // Подсказка в статус-баре в зависимости от операции
    QString hint;
    switch (index) {
    case 0: hint = "Сложение: A(x) + B(x)"; break;
    case 1: hint = "Вычитание: A(x) − B(x)"; break;
    case 2: hint = "Умножение: A(x) × B(x)"; break;
    case 3: hint = "Деление: частное A/B и остаток A%B"; break;
    case 4: hint = "Вычисление: подставьте значение x в A(x)"; break;
    case 5: hint = "Производная: d/dx[A(x)]"; break;
    case 6: hint = "Интеграл: ∫A(x)dx + C"; break;
    case 7: hint = "Корни: решаем A(x) = 0 (степень ≤ 2)"; break;
    }
    m_statusLabel->setText(hint);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
}

// Вспышка уведомления в статус-баре
void PolynomialTab::flashStatus(const QString& text, const QString& color) {
    m_statusLabel->setText(text);
    // color + "20" — добавляем 20 в hex = 12% непрозрачности для фона
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: %1; background-color: %2;")
        .arg(color).arg(color + "20"));
}

// Очистка всех полей
void PolynomialTab::onClearAll() {
    onClearA();
    onClearB();
    m_resultEdit->clear();
    m_statusLabel->setText("Все данные очищены");
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #757575; background-color: #f5f5f5;"));
}


// Очистка только полинома A
void PolynomialTab::onClearA() {
    int deg = m_degreeSpinA->value();
    for (int col = 0; col <= deg; ++col) {
        auto* item = new QTableWidgetItem("0");
        item->setTextAlignment(Qt::AlignCenter);
        m_tableA->setItem(0, col, item);
    }
    updatePreviewA();
}

// Очистка только полинома B
void PolynomialTab::onClearB() {
    int deg = m_degreeSpinB->value();
    for (int col = 0; col <= deg; ++col) {
        auto* item = new QTableWidgetItem("0");
        item->setTextAlignment(Qt::AlignCenter);
        m_tableB->setItem(0, col, item);
    }
    updatePreviewB();
}


// Случайное заполнение полинома A
void PolynomialTab::onRandomFillA() {
    int deg = m_degreeSpinA->value();
    for (int col = 0; col <= deg; ++col) {
        // Случайное число от -10 до 10
        double val = QRandomGenerator::global()->bounded(-10, 11);
        // Старший коэффициент не должен быть нулём
        if (col == 0) val = QRandomGenerator::global()->bounded(1, 10);
        auto* item = new QTableWidgetItem(QString::number(val));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableA->setItem(0, col, item);
    }
    updatePreviewA();
    flashStatus("Полином A заполнен случайными значениями", "#ff9800");
}


// Случайное заполнение полинома B
void PolynomialTab::onRandomFillB() {
    int deg = m_degreeSpinB->value();
    for (int col = 0; col <= deg; ++col) {
        double val = QRandomGenerator::global()->bounded(-10, 11);
        if (col == 0) val = QRandomGenerator::global()->bounded(1, 10);
        auto* item = new QTableWidgetItem(QString::number(val));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableB->setItem(0, col, item);
    }
    updatePreviewB();
}
#pragma endregion

#pragma region Вычисление результата
// Обработчик нажатия кнопки "Вычислить"
void PolynomialTab::onCalculate() {
    try {
        int op = m_operationCombo->currentIndex();
        PolynomialD polyA = getPolynomialFromTable(m_tableA);
        QString resultText;
        QString statusMsg;
        bool hasResult = true;

        switch (op) {
        case 0: { // Сложение A + B
            PolynomialD polyB = getPolynomialFromTable(m_tableB);
            if (polyB.isZero() && polyA.isZero()) {
                throw std::invalid_argument("Оба полинома нулевые");
            }
            PolynomialD res = polyA + polyB;
            resultText = polynomialToString(res);
            statusMsg = "Сложение выполнено";
            break;
        }
        case 1: { // Вычитание A − B
            PolynomialD polyB = getPolynomialFromTable(m_tableB);
            PolynomialD res = polyA - polyB;
            resultText = polynomialToString(res);
            statusMsg = "Вычитание выполнено";
            break;
        }
        case 2: { // Умножение A × B
            PolynomialD polyB = getPolynomialFromTable(m_tableB);
            PolynomialD res = polyA * polyB;
            resultText = polynomialToString(res);
            statusMsg = "Умножение выполнено";
            break;
        }
        case 3: { // Деление A / B + остаток A % B
            PolynomialD polyB = getPolynomialFromTable(m_tableB);
            if (polyB.isZero()) {
                throw std::invalid_argument("Деление на нулевой полином невозможно");
            }
            PolynomialD res = polyA / polyB;
            PolynomialD remainder = polyA % polyB;
            resultText = "Частное (A / B):\n" + polynomialToString(res) +
                "\n\nОстаток (A % B):\n" + polynomialToString(remainder);
            statusMsg = "Деление выполнено: частное и остаток";
            break;
        }
        case 4: { // Вычисление A(x)
            bool ok;
            double x = QInputDialog::getDouble(this, "Вычисление A(x)",
                "Введите значение x:", 0.0, -1e6, 1e6, 6, &ok);
            if (!ok) { hasResult = false; break; }  // Пользователь нажал "Отмена"
            double val = polyA.evaluate(x);
            resultText = QString("A(%1) = %2").arg(x, 0, 'g', 10).arg(val, 0, 'g', 10);
            statusMsg = QString("A(%1) = %2").arg(x, 0, 'g', 6).arg(val, 0, 'g', 6);
            break;
        }
        case 5: { // Производная
            PolynomialD res = polyA.derivative();
            resultText = polynomialToString(res);
            statusMsg = "Производная найдена";
            break;
        }
        case 6: { // Интеграл
            PolynomialD res = polyA.integral(0.0);
            resultText = polynomialToString(res) + " + C";
            statusMsg = "Интеграл найден";
            break;
        }
        case 7: { // Корни
            std::vector<double> roots = polyA.roots();
            if (roots.empty()) {
                if (polyA.degree() > 2) {
                    resultText = "Корни для полиномов степени > 2 не поддерживаются\n"
                        "(Используйте численные методы)";
                }
                else {
                    resultText = "Корней нет (или полином нулевой)";
                }
            }
            else {
                for (size_t i = 0; i < roots.size(); ++i) {
                    resultText += QString("x%1 = %2\n").arg(i + 1).arg(roots[i], 0, 'g', 10);
                }
            }
            statusMsg = "Корни найдены";
            break;
        }
        }

        // Выводим результат, если вычисление было выполнено
        if (hasResult) {
            m_resultEdit->setPlainText(resultText);
            m_statusLabel->setText("✓ " + statusMsg);
            m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #2e7d32; background-color: #e8f5e9;"));
        }

    }
    catch (const std::exception& e) {
        // Показываем окно ошибки
        QMessageBox::critical(this, "Ошибка вычисления", e.what());
        m_statusLabel->setText("✗ Ошибка: " + QString(e.what()));
        m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #c62828; background-color: #ffebee;"));
        m_resultEdit->setPlainText("Ошибка: " + QString(e.what()));
    }
}
#pragma endregion