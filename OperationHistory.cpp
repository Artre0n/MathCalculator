#include "OperationHistory.h"
#include <QMessageBox>
#include <QFrame>

#pragma region Стили оформления истории

/// Стиль заголовка операции в истории
static const char* HISTORY_GROUP_STYLE = R"(
    QGroupBox {
        font-weight: bold;
        border: 2px solid #e0e0e0;
        border-radius: 10px;
        margin-top: 8px;
        padding: 10px;
        background-color: white;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 10px;
        padding: 0 6px;
        color: #424242;
        font-size: 12px;
    }
)";

/// Стиль текстового поля с матрицей
static const char* MATRIX_EDIT_STYLE = R"(
    QTextEdit {
        border: 1px solid #e0e0e0;
        border-radius: 6px;
        background-color: #fafafa;
        font-family: 'Cascadia Code', 'Fira Code', 'Courier New', monospace;
        font-size: 11px;
        padding: 6px;
    }
)";

/// Стиль кнопки загрузки
static const char* BTN_LOAD_STYLE = R"(
    QPushButton {
        background-color: #4caf50;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 6px 14px;
        font-weight: 600;
        font-size: 11px;
    }
    QPushButton:hover { background-color: #388e3c; }
)";

/// Стиль кнопки очистки истории
static const char* BTN_CLEAR_STYLE = R"(
    QPushButton {
        background-color: #ef5350;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 8px 18px;
        font-weight: 600;
        font-size: 12px;
    }
    QPushButton:hover { background-color: #c62828; }
)";

/// Стиль кнопки закрытия
static const char* BTN_CLOSE_STYLE = R"(
    QPushButton {
        background-color: #757575;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 8px 18px;
        font-weight: 600;
        font-size: 12px;
    }
    QPushButton:hover { background-color: #424242; }
)";
#pragma endregion

#pragma region Конструктор и инициализация интерфейса

OperationHistoryDialog::OperationHistoryDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("📜 История операций");
    resize(900, 700);
    setStyleSheet("background-color: #f5f7fa;");

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(12);
    m_mainLayout->setContentsMargins(16, 16, 16, 16);

    // Заголовок
    auto* titleLabel = new QLabel("📜 История операций");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #212121;");
    m_mainLayout->addWidget(titleLabel);

    // Область прокрутки для записей
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    // Виджет-контейнер для записей
    m_scrollContent = new QWidget();
    m_contentLayout = new QVBoxLayout(m_scrollContent);
    m_contentLayout->setAlignment(Qt::AlignTop);
    m_contentLayout->setSpacing(10);

    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);

    // Панель кнопок внизу
    auto* buttonLayout = new QHBoxLayout();

    auto* clearBtn = new QPushButton("🗑 Очистить историю");
    clearBtn->setStyleSheet(BTN_CLEAR_STYLE);
    clearBtn->setCursor(Qt::PointingHandCursor);
    connect(clearBtn, &QPushButton::clicked, this, &OperationHistoryDialog::onClearHistory);

    auto* closeBtn = new QPushButton("✕ Закрыть");
    closeBtn->setStyleSheet(BTN_CLOSE_STYLE);
    closeBtn->setCursor(Qt::PointingHandCursor);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);

    m_mainLayout->addLayout(buttonLayout);

    // Начальное состояние — пусто
    updateHistoryDisplay();
}
#pragma endregion

#pragma region Методы управления историей

/// Добавление операции в историю
void OperationHistoryDialog::addOperation(const QString& type, const MatrixD& mA,
    const MatrixD& mB, const MatrixD& res, bool hasB) {
    OperationRecord record;
    record.operationType = type;
    record.matrixA = mA;
    record.matrixB = mB;
    record.result = res;
    record.hasMatrixB = hasB;

    m_history.push_back(record);
    updateHistoryDisplay();
}


/// Очистка истории (метод)
void OperationHistoryDialog::clearHistory() {
    m_history.clear();
    updateHistoryDisplay();
}

/// Очистка истории (слот)
void OperationHistoryDialog::onClearHistory() {
    auto reply = QMessageBox::question(this, "Очистка истории",
        "Вы уверены, что хотите удалить все записи?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        clearHistory();
    }
}


/// Загрузка результата в матрицу A
void OperationHistoryDialog::onLoadToA(int index) {
    if (index >= 0 && index < static_cast<int>(m_history.size())) {
        emit loadMatrixToA(m_history[index].result);
    }
}


/// Преобразование матрицы в текст
QString OperationHistoryDialog::matrixToString(const MatrixD& mat) {
    if (mat.getRows() == 0 || mat.getCols() == 0) {
        return "(пустая)";
    }

    QString result;
    for (size_t i = 0; i < mat.getRows(); ++i) {
        for (size_t j = 0; j < mat.getCols(); ++j) {
            result += QString::number(mat(i, j), 'g', 4);
            if (j < mat.getCols() - 1) result += "\t";
        }
        result += "\n";
    }
    return result;
}


/// Обновление отображения истории
void OperationHistoryDialog::updateHistoryDisplay() {
    // Удаляем все старые виджеты
    QLayoutItem* child;
    while ((child = m_contentLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // Если история пуста — показываем сообщение
    if (m_history.empty()) {
        auto* emptyLabel = new QLabel("История пуста\nВыполните операцию, чтобы увидеть запись");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: #9e9e9e; font-style: italic; padding: 40px; font-size: 14px;");
        m_contentLayout->addWidget(emptyLabel);
        return;
    }

    // Отображаем записи в обратном порядке (новые сверху)
    for (int i = static_cast<int>(m_history.size()) - 1; i >= 0; --i) {
        const OperationRecord& record = m_history[i];

        // Группа для одной операции
        auto* group = new QGroupBox(QString("Операция #%1: %2")
            .arg(i + 1).arg(record.operationType));
        group->setStyleSheet(HISTORY_GROUP_STYLE);
        auto* groupLayout = new QVBoxLayout(group);
        groupLayout->setSpacing(8);

        // Матрица A
        auto* labelA = new QLabel("<b>🧮 Матрица A</b> (" +
            QString::number(record.matrixA.getRows()) + "×" +
            QString::number(record.matrixA.getCols()) + ")");
        labelA->setStyleSheet("color: #1565c0; font-size: 12px;");
        groupLayout->addWidget(labelA);

        auto* editA = new QTextEdit();
        editA->setPlainText(matrixToString(record.matrixA));
        editA->setReadOnly(true);
        editA->setMaximumHeight(80);
        editA->setStyleSheet(MATRIX_EDIT_STYLE);
        groupLayout->addWidget(editA);

        // Матрица B
        if (record.hasMatrixB) {
            auto* labelB = new QLabel("<b>🧮 Матрица B</b> (" +
                QString::number(record.matrixB.getRows()) + "×" +
                QString::number(record.matrixB.getCols()) + ")");
            labelB->setStyleSheet("color: #1565c0; font-size: 12px;");
            groupLayout->addWidget(labelB);

            auto* editB = new QTextEdit();
            editB->setPlainText(matrixToString(record.matrixB));
            editB->setReadOnly(true);
            editB->setMaximumHeight(80);
            editB->setStyleSheet(MATRIX_EDIT_STYLE);
            groupLayout->addWidget(editB);
        }

        // Результат
        auto* labelResult = new QLabel("<b>📊 Результат</b> (" +
            QString::number(record.result.getRows()) + "×" +
            QString::number(record.result.getCols()) + ")");
        labelResult->setStyleSheet("color: #2e7d32; font-size: 12px;");
        groupLayout->addWidget(labelResult);

        auto* editResult = new QTextEdit();
        editResult->setPlainText(matrixToString(record.result));
        editResult->setReadOnly(true);
        editResult->setMaximumHeight(80);
        editResult->setStyleSheet(MATRIX_EDIT_STYLE + QString("border-color: #a5d6a7; background-color: #e8f5e9;"));
        groupLayout->addWidget(editResult);

        // ─── Кнопка загрузки в A ───
        auto* loadBtn = new QPushButton("📥 Загрузить результат в матрицу A");
        loadBtn->setStyleSheet(BTN_LOAD_STYLE);
        loadBtn->setCursor(Qt::PointingHandCursor);
        // Используем lambda для захвата индекса
        connect(loadBtn, &QPushButton::clicked, this, [this, i]() {
            onLoadToA(i);
            });
        groupLayout->addWidget(loadBtn);

        m_contentLayout->addWidget(group);
    }

    // Растяжка внизу, чтобы записи были сверху
    m_contentLayout->addStretch();
}
#pragma endregion