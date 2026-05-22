#include "ComplexTab.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QDoubleValidator>


#pragma region Стили оформления

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

static const char* INPUT_STYLE = R"(
    QLineEdit {
        padding: 8px 12px;
        border: 2px solid #e0e0e0;
        border-radius: 8px;
        background-color: white;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 14px;
        font-weight: 500;
        min-width: 100px;
    }
    QLineEdit:hover { border-color: #2196f3; }
    QLineEdit:focus { border-color: #2196f3; }
)";

static const char* PREVIEW_STYLE = R"(
    QLabel {
        background-color: #f3e5f5;
        color: #7b1fa2;
        padding: 12px 16px;
        border-radius: 8px;
        border-left: 4px solid #9c27b0;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 15px;
        font-weight: 500;
        min-height: 24px;
    }
)";

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
)";

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

static const char* STATUS_STYLE = R"(
    QLabel {
        padding: 10px 16px;
        border-radius: 8px;
        font-weight: 600;
        font-size: 13px;
    }
)";

static const char* COMBO_STYLE = R"(
    QComboBox {
        padding: 8px 12px;
        border: 2px solid #e0e0e0;
        border-radius: 8px;
        background-color: white;
        font-size: 13px;
        min-width: 280px;
    }
    QComboBox:hover { border-color: #2196f3; }
    QComboBox::drop-down { border: none; width: 30px; }
    QComboBox QAbstractItemView {
        border: 1px solid #ddd;
        border-radius: 8px;
        background-color: white;
        selection-background-color: #e3f2fd;
        padding: 4px;
    }
)";
#pragma endregion

#pragma region Конструктор и интерфейс

ComplexTab::ComplexTab(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color: #f5f7fa;");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок
    auto *headerLayout = new QHBoxLayout();
    auto *titleLabel = new QLabel("🔣 Комплексный калькулятор");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #212121;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // Панель управления
    auto *controlFrame = new QFrame();
    controlFrame->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #e0e0e0;");
    auto *controlLayout = new QHBoxLayout(controlFrame);
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
        "Модуль |A|",
        "Аргумент arg(A)",
        "Сопряжение A*",
        "Возведение A в степень",
        "Корень n-й степени из A",
        "Экспонента eᴬ",
        "Натуральный логарифм ln(A)",
        "Синус sin(A)",
        "Косинус cos(A)",
        "Гиперболический синус sinh(A)",
        "Гиперболический косинус cosh(A)"
    });
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComplexTab::onOperationChanged);
    controlLayout->addWidget(m_operationCombo);

    controlLayout->addSpacing(20);

    m_calcBtn = new QPushButton("▶  Вычислить", this);
    m_calcBtn->setStyleSheet(BTN_PRIMARY);
    m_calcBtn->setCursor(Qt::PointingHandCursor);
    m_calcBtn->setMinimumHeight(40);
    connect(m_calcBtn, &QPushButton::clicked, this, &ComplexTab::onCalculate);
    controlLayout->addWidget(m_calcBtn);

    m_clearBtn = new QPushButton("🗑  Очистить всё", this);
    m_clearBtn->setStyleSheet(BTN_DANGER);
    m_clearBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBtn, &QPushButton::clicked, this, &ComplexTab::onClearAll);
    controlLayout->addWidget(m_clearBtn);

    controlLayout->addStretch();
    mainLayout->addWidget(controlFrame);

    //Область чисел
    auto *numbersLayout = new QHBoxLayout();
    numbersLayout->setSpacing(16);

    // Число A
    m_groupA = new QGroupBox("Комплексное число A", this);
    m_groupA->setStyleSheet(GROUP_STYLE);
    auto *layoutA = new QVBoxLayout(m_groupA);
    layoutA->setSpacing(10);

    auto *inputLayoutA = new QHBoxLayout();
    inputLayoutA->addWidget(new QLabel("Re:"));
    m_realA = new QLineEdit("0");
    m_realA->setStyleSheet(INPUT_STYLE);
    m_realA->setValidator(new QDoubleValidator(-1e6, 1e6, 6, this));
    inputLayoutA->addWidget(m_realA);

    inputLayoutA->addWidget(new QLabel("Im:"));
    m_imagA = new QLineEdit("0");
    m_imagA->setStyleSheet(INPUT_STYLE);
    m_imagA->setValidator(new QDoubleValidator(-1e6, 1e6, 6, this));
    inputLayoutA->addWidget(m_imagA);
    inputLayoutA->addStretch();

    m_randomABtn = new QPushButton("🎲 Случайные", this);
    m_randomABtn->setStyleSheet(BTN_ACCENT);
    m_randomABtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomABtn, &QPushButton::clicked, this, &ComplexTab::onRandomFillA);
    inputLayoutA->addWidget(m_randomABtn);

    m_clearABtn = new QPushButton("✕ Очистить", this);
    m_clearABtn->setStyleSheet(BTN_DANGER);
    m_clearABtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearABtn, &QPushButton::clicked, this, &ComplexTab::onClearA);
    inputLayoutA->addWidget(m_clearABtn);

    layoutA->addLayout(inputLayoutA);

    auto *previewLabelA = new QLabel("Предпросмотр:");
    previewLabelA->setStyleSheet("color: #757575; font-size: 12px; font-weight: 600;");
    layoutA->addWidget(previewLabelA);

    m_previewA = new QLabel("0");
    m_previewA->setStyleSheet(PREVIEW_STYLE);
    m_previewA->setWordWrap(true);
    m_previewA->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layoutA->addWidget(m_previewA);

    numbersLayout->addWidget(m_groupA, 1);

    // Число B
    m_groupB = new QGroupBox("Комплексное число B", this);
    m_groupB->setStyleSheet(GROUP_STYLE);
    auto *layoutB = new QVBoxLayout(m_groupB);
    layoutB->setSpacing(10);

    auto *inputLayoutB = new QHBoxLayout();
    inputLayoutB->addWidget(new QLabel("Re:"));
    m_realB = new QLineEdit("0");
    m_realB->setStyleSheet(INPUT_STYLE);
    m_realB->setValidator(new QDoubleValidator(-1e6, 1e6, 6, this));
    inputLayoutB->addWidget(m_realB);

    inputLayoutB->addWidget(new QLabel("Im:"));
    m_imagB = new QLineEdit("0");
    m_imagB->setStyleSheet(INPUT_STYLE);
    m_imagB->setValidator(new QDoubleValidator(-1e6, 1e6, 6, this));
    inputLayoutB->addWidget(m_imagB);
    inputLayoutB->addStretch();

    m_randomBBtn = new QPushButton("🎲 Случайные", this);
    m_randomBBtn->setStyleSheet(BTN_ACCENT);
    m_randomBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_randomBBtn, &QPushButton::clicked, this, &ComplexTab::onRandomFillB);
    inputLayoutB->addWidget(m_randomBBtn);

    m_clearBBtn = new QPushButton("✕ Очистить", this);
    m_clearBBtn->setStyleSheet(BTN_DANGER);
    m_clearBBtn->setCursor(Qt::PointingHandCursor);
    connect(m_clearBBtn, &QPushButton::clicked, this, &ComplexTab::onClearB);
    inputLayoutB->addWidget(m_clearBBtn);

    layoutB->addLayout(inputLayoutB);

    auto *previewLabelB = new QLabel("Предпросмотр:");
    previewLabelB->setStyleSheet("color: #757575; font-size: 12px; font-weight: 600;");
    layoutB->addWidget(previewLabelB);

    m_previewB = new QLabel("0");
    m_previewB->setStyleSheet(PREVIEW_STYLE);
    m_previewB->setWordWrap(true);
    m_previewB->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layoutB->addWidget(m_previewB);

    numbersLayout->addWidget(m_groupB, 1);
    mainLayout->addLayout(numbersLayout, 1);

    //Область результата
    auto *resultFrame = new QFrame();
    resultFrame->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #e0e0e0;");
    auto *resultLayout = new QVBoxLayout(resultFrame);
    resultLayout->setSpacing(10);
    resultLayout->setContentsMargins(16, 16, 16, 16);

    auto *resultHeader = new QHBoxLayout();
    auto *resultTitle = new QLabel("📊 Результат");
    resultTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: #2e7d32;");
    resultHeader->addWidget(resultTitle);
    resultHeader->addStretch();
    resultLayout->addLayout(resultHeader);

    m_resultEdit = new QTextEdit(this);
    m_resultEdit->setStyleSheet(RESULT_STYLE);
    m_resultEdit->setReadOnly(true);
    m_resultEdit->setPlaceholderText("Результат появится здесь...");
    m_resultEdit->setMaximumHeight(120);
    resultLayout->addWidget(m_resultEdit);

    mainLayout->addWidget(resultFrame);

    //Статусная строка
    m_statusLabel = new QLabel("Готов к работе • Введите действительную и мнимую части", this);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    // Подключаем обновление предпросмотра
    connect(m_realA, &QLineEdit::textChanged, this, [this]() {
        m_previewA->setText(complexToString(getComplexFromEdits(m_realA, m_imagA)));
    });
    connect(m_imagA, &QLineEdit::textChanged, this, [this]() {
        m_previewA->setText(complexToString(getComplexFromEdits(m_realA, m_imagA)));
    });
    connect(m_realB, &QLineEdit::textChanged, this, [this]() {
        m_previewB->setText(complexToString(getComplexFromEdits(m_realB, m_imagB)));
    });
    connect(m_imagB, &QLineEdit::textChanged, this, [this]() {
        m_previewB->setText(complexToString(getComplexFromEdits(m_realB, m_imagB)));
    });

    onOperationChanged(0);
}
#pragma endregion

#pragma region Вспомогательные методы

ComplexD ComplexTab::getComplexFromEdits(QLineEdit* realEdit, QLineEdit* imagEdit) const {
    bool ok1, ok2;
    double re = realEdit->text().replace(',', '.').toDouble(&ok1);
    double im = imagEdit->text().replace(',', '.').toDouble(&ok2);
    return ComplexD(ok1 ? re : 0.0, ok2 ? im : 0.0);
}

void ComplexTab::setComplexToEdits(const ComplexD& z, QLineEdit* realEdit, QLineEdit* imagEdit) {
    realEdit->setText(QString::number(z.getReal(), 'g', 10));
    imagEdit->setText(QString::number(z.getImag(), 'g', 10));
}

QString ComplexTab::complexToString(const ComplexD& z) const {
    return QString::fromStdString(z.toString());
}

void ComplexTab::flashStatus(const QString& text, const QString& color) {
    m_statusLabel->setText(text);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: %1; background-color: %2;")
                                                    .arg(color).arg(color + "20"));
}

bool ComplexTab::needsB(int opIndex) const {
    return (opIndex >= 0 && opIndex <= 3);  // +, −, ×, ÷
}

// Очистка
void ComplexTab::onClearAll() {
    onClearA();
    onClearB();
    m_resultEdit->clear();
    m_statusLabel->setText("Все данные очищены");
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #757575; background-color: #f5f5f5;"));
}

void ComplexTab::onClearA() {
    m_realA->setText("0");
    m_imagA->setText("0");
}

void ComplexTab::onClearB() {
    m_realB->setText("0");
    m_imagB->setText("0");
}

// Случайное заполнение
void ComplexTab::onRandomFillA() {
    double re = QRandomGenerator::global()->bounded(-10, 11);
    double im = QRandomGenerator::global()->bounded(-10, 11);
    m_realA->setText(QString::number(re));
    m_imagA->setText(QString::number(im));
    flashStatus("Число A заполнено случайными значениями", "#ff9800");
}

void ComplexTab::onRandomFillB() {
    double re = QRandomGenerator::global()->bounded(-10, 11);
    double im = QRandomGenerator::global()->bounded(-10, 11);
    m_realB->setText(QString::number(re));
    m_imagB->setText(QString::number(im));
}

// Изменение операции
void ComplexTab::onOperationChanged(int index) {
    bool needB = needsB(index);
    m_groupB->setVisible(needB);

    QString hint;
    switch(index) {
    case 0: hint = "Сложение: A + B"; break;
    case 1: hint = "Вычитание: A − B"; break;
    case 2: hint = "Умножение: A × B"; break;
    case 3: hint = "Деление: A / B"; break;
    case 4: hint = "Модуль: |A|"; break;
    case 5: hint = "Аргумент: arg(A)"; break;
    case 6: hint = "Сопряжение: A*"; break;
    case 7: hint = "Степень: Aⁿ"; break;
    case 8: hint = "Корень: ⁿ√A"; break;
    case 9: hint = "Экспонента: eᴬ"; break;
    case 10: hint = "Логарифм"; break;
    case 11: hint = "Синус: sin(A)"; break;
    case 12: hint = "Косинус: cos(A)"; break;
    case 13: hint = "Гиперболический синус: sinh(A)"; break;
    case 14: hint = "Гиперболический косинус: cosh(A)"; break;
    }
    m_statusLabel->setText(hint);
    m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #1565c0; background-color: #e3f2fd;"));
}
#pragma endregion

#pragma region Вычисление результата
// Вычисление результата
void ComplexTab::onCalculate() {
    try {
        int op = m_operationCombo->currentIndex();
        ComplexD A = getComplexFromEdits(m_realA, m_imagA);
        QString resultText;
        QString statusMsg;

        switch(op) {
        case 0: { // Сложение
            ComplexD B = getComplexFromEdits(m_realB, m_imagB);
            ComplexD res = A + B;
            resultText = complexToString(res);
            statusMsg = "Сложение выполнено";
            break;
        }
        case 1: { // Вычитание
            ComplexD B = getComplexFromEdits(m_realB, m_imagB);
            ComplexD res = A - B;
            resultText = complexToString(res);
            statusMsg = "Вычитание выполнено";
            break;
        }
        case 2: { // Умножение
            ComplexD B = getComplexFromEdits(m_realB, m_imagB);
            ComplexD res = A * B;
            resultText = complexToString(res);
            statusMsg = "Умножение выполнено";
            break;
        }
        case 3: { // Деление
            ComplexD B = getComplexFromEdits(m_realB, m_imagB);
            if (B.abs() == 0.0) {
                throw std::invalid_argument("Деление на ноль невозможно");
            }
            ComplexD res = A / B;
            resultText = complexToString(res);
            statusMsg = "Деление выполнено";
            break;
        }
        case 4: { // Модуль
            double mod = A.abs();
            resultText = QString("|A| = %1").arg(mod, 0, 'g', 10);
            statusMsg = QString("Модуль |A| = %1").arg(mod, 0, 'g', 6);
            break;
        }
        case 5: { // Аргумент
            double arg = A.arg();
            resultText = QString("arg(A) = %1 рад\narg(A) = %2°")
                             .arg(arg, 0, 'g', 10)
                             .arg(arg * 180.0 / M_PI, 0, 'g', 6);
            statusMsg = QString("Аргумент = %1 рад").arg(arg, 0, 'g', 6);
            break;
        }
        case 6: { // Сопряжение
            ComplexD res = A.conjugate();
            resultText = complexToString(res);
            statusMsg = "Сопряжение выполнено";
            break;
        }
        case 7: { // Степень
            bool ok;
            int n = QInputDialog::getInt(this, "Возведение в степень",
                                         "Введите целую степень n:", 2, -10, 10, 1, &ok);
            if (!ok) return;
            ComplexD res = A.pow(n);
            resultText = complexToString(res);
            statusMsg = QString("A^%1 выполнено").arg(n);
            break;
        }
        case 8: { // Корень
            bool ok;
            int n = QInputDialog::getInt(this, "Корень n-й степени",
                                         "Введите степень корня n (положительное):", 2, 1, 10, 1, &ok);
            if (!ok) return;
            ComplexD res = A.root(n);
            resultText = complexToString(res);
            statusMsg = QString("Корень %1-й степени из A").arg(n);
            break;
        }
        case 9: { // Экспонента
            ComplexD res = A.exp();
            resultText = complexToString(res);
            statusMsg = "Экспонента eᴬ выполнена";
            break;
        }
        case 10: { // Логарифм
            if (A.abs() == 0.0) {
                throw std::invalid_argument("Логарифм нуля не определён");
            }
            ComplexD res = A.log();
            resultText = complexToString(res);
            statusMsg = "Натуральный логарифм выполнен";
            break;
        }
        case 11: { // Синус
            ComplexD res = A.sin();
            resultText = complexToString(res);
            statusMsg = "Синус sin(A) выполнен";
            break;
        }
        case 12: { // Косинус
            ComplexD res = A.cos();
            resultText = complexToString(res);
            statusMsg = "Косинус cos(A) выполнен";
            break;
        }
        case 13: { // Гиперболический синус
            ComplexD res = A.sinh();
            resultText = complexToString(res);
            statusMsg = "Гиперболический синус sinh(A) выполнен";
            break;
        }
        case 14: { // Гиперболический косинус
            ComplexD res = A.cosh();
            resultText = complexToString(res);
            statusMsg = "Гиперболический косинус cosh(A) выполнен";
            break;
        }
        }

        m_resultEdit->setPlainText(resultText);
        m_statusLabel->setText("✓ " + statusMsg);
        m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #2e7d32; background-color: #e8f5e9;"));

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления", e.what());
        m_statusLabel->setText("✗ Ошибка: " + QString(e.what()));
        m_statusLabel->setStyleSheet(STATUS_STYLE + QString("color: #c62828; background-color: #ffebee;"));
        m_resultEdit->setPlainText("Ошибка: " + QString(e.what()));
    }
}
#pragma endregion