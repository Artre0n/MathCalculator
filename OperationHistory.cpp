#include "OperationHistory.h"

OperationHistoryDialog::OperationHistoryDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("История операций");
    resize(800, 600);

#pragma region Создание виджетов{

    m_mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("История операций");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_mainLayout->addWidget(titleLabel);

    // Область прокрутки
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Виджет содержимого для области прокрутки
    m_scrollContent = new QWidget();
    m_contentLayout = new QVBoxLayout(m_scrollContent);
    m_contentLayout->setAlignment(Qt::AlignTop);

    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);

    // Панель кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Кнопка очистки истории
    QPushButton *clearBtn = new QPushButton("Очистить историю");
    clearBtn->setStyleSheet("background-color: #ff6b6b; color: white; font-weight: bold;");
    connect(clearBtn, &QPushButton::clicked, this, &OperationHistoryDialog::onClearHistory);

    // Кнопка закрытия
    QPushButton *closeBtn = new QPushButton("Закрыть");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(clearBtn);
    buttonLayout->addWidget(closeBtn);

    m_mainLayout->addLayout(buttonLayout);
}
#pragma endregion}

// Добавление операции в историю
void OperationHistoryDialog::addOperation(const QString& type, const MatrixD& mA,
                                          const MatrixD& mB, const MatrixD& res, bool hasB) {

    // Запись об операции
    OperationRecord record;
    record.operationType = type;
    record.matrixA = mA;
    record.matrixB = mB;
    record.result = res;
    record.hasMatrixB = hasB;

    m_history.push_back(record);

    updateHistoryDisplay();
}

// Очистка истории(метод)
void OperationHistoryDialog::clearHistory() {
    m_history.clear();
    updateHistoryDisplay();
}

// Очистка истории(слот)
void OperationHistoryDialog::onClearHistory() {
    clearHistory();
}

// Преобразование матрицы в строку
QString OperationHistoryDialog::matrixToString(const MatrixD& mat) {
    if (mat.getRows() == 0 || mat.getCols() == 0) {
        return "";
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

// Обновление истории
void OperationHistoryDialog::updateHistoryDisplay() {
    // Очистка существующих элементов
    QLayoutItem *child;
    while ((child = m_contentLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    if (m_history.empty()) {
        QLabel *emptyLabel = new QLabel("История пуста");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: gray; font-style: italic; padding: 20px;");
        m_contentLayout->addWidget(emptyLabel);
        return;
    }

    for (int i = m_history.size() - 1; i >= 0; --i) {
        const OperationRecord& record = m_history[i];

        QGroupBox *group = new QGroupBox(QString("Операция #%1: %2").arg(i + 1).arg(record.operationType));
        QVBoxLayout *groupLayout = new QVBoxLayout(group);

        // Матрица A
        QLabel *labelA = new QLabel("<b>Матрица A:</b>");
        labelA->setStyleSheet("font-weight: bold;");
        groupLayout->addWidget(labelA);

        QTextEdit *editA = new QTextEdit();
        editA->setPlainText(matrixToString(record.matrixA));
        editA->setReadOnly(true);
        editA->setMaximumHeight(100);
        editA->setFont(QFont("Courier New", 9));
        groupLayout->addWidget(editA);

        // Матрица B
        if (record.hasMatrixB) {
            QLabel *labelB = new QLabel("<b>Матрица B:</b>");
            labelB->setStyleSheet("font-weight: bold;");
            groupLayout->addWidget(labelB);

            QTextEdit *editB = new QTextEdit();
            editB->setPlainText(matrixToString(record.matrixB));
            editB->setReadOnly(true);
            editB->setMaximumHeight(100);
            editB->setFont(QFont("Courier New", 9));
            groupLayout->addWidget(editB);
        }

        // Результат
        QLabel *labelResult = new QLabel("<b>Результат:</b>");
        labelResult->setStyleSheet("font-weight: bold; color: green;");
        groupLayout->addWidget(labelResult);

        QTextEdit *editResult = new QTextEdit();
        editResult->setPlainText(matrixToString(record.result));
        editResult->setReadOnly(true);
        editResult->setMaximumHeight(100);
        editResult->setFont(QFont("Courier New", 9));
        groupLayout->addWidget(editResult);

        m_contentLayout->addWidget(group);
    }

    m_contentLayout->addStretch();
}