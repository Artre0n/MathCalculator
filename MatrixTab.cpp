#include "MatrixTab.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>

#pragma region Создание виджетов{

MatrixTab::MatrixTab(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);

    // Элементы управления
    auto *controlLayout = new QHBoxLayout();
    m_operationCombo = new QComboBox(this);
    m_calcBtn = new QPushButton("Вычислить", this);
    m_calcBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
    m_clearBtn = new QPushButton("Очистить всё", this);
    m_clearBtn->setStyleSheet("background-color: #ff6b6b; color: white; font-weight: bold;");

    // Кнопка истории
    m_historyBtn = new QPushButton("История операций", this);
    m_historyDialog = new OperationHistoryDialog(this);


    // Матрица А
    m_groupA = new QGroupBox("Матрица A", this);
    auto *layoutA = new QGridLayout(m_groupA);
    m_rowsSpinA = new QSpinBox(this);
    m_rowsSpinA->setRange(1, 6);
    m_rowsSpinA->setValue(3);
    m_colsSpinA = new QSpinBox(this);
    m_colsSpinA->setRange(1, 6);
    m_colsSpinA->setValue(3);
    m_inputTableA = new QTableWidget(this);

    // Матрица В
    m_groupB = new QGroupBox("Матрица B", this);
    m_groupB->setVisible(false);
    auto *layoutB = new QGridLayout(m_groupB);
    m_rowsSpinB = new QSpinBox(this);
    m_rowsSpinB->setRange(1, 6);
    m_rowsSpinB->setValue(3);
    m_colsSpinB = new QSpinBox(this);
    m_colsSpinB->setRange(1, 6);
    m_colsSpinB->setValue(3);
    m_inputTableB = new QTableWidget(this);

    // Статус
    m_outputTable = new QTableWidget(this);
    m_outputTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_statusLabel = new QLabel("Готов к работе (макс. размер 6x6)", this);
    m_statusLabel->setStyleSheet("color: blue; font-weight: bold;");

    m_operationCombo->addItems({
        "Определитель (A)",
        "Транспонирование (A)",
        "Обратная матрица (A)",
        "Сложение (A + B)",
        "Вычитание (A - B)",
        "Умножение (A * B)",
        "Умножение A на число (2)",
        "Возведение A в степень (2)"
    });
#pragma endregion}

#pragma region Настройка связей{

    connect(m_rowsSpinA, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixTab::onResizeMatrixA);
    connect(m_colsSpinA, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixTab::onResizeMatrixA);

    connect(m_rowsSpinB, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixTab::onResizeMatrixB);
    connect(m_colsSpinB, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixTab::onResizeMatrixB);

    connect(m_historyBtn, &QPushButton::clicked, this, &MatrixTab::showHistory);

    // Показ/скрытие матрицы B
    if (m_operationCombo && m_groupB) {
        QGroupBox* groupBPtr = m_groupB;
        connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [groupBPtr](int index) {
            bool needTwoMatrices = (index == 3 || index == 4 || index == 5);
            if (groupBPtr) groupBPtr->setVisible(needTwoMatrices);
        });
    }

    connect(m_calcBtn, &QPushButton::clicked, this, &MatrixTab::onCalculate);
    connect(m_clearBtn, &QPushButton::clicked, this, &MatrixTab::onClearAll);

#pragma endregion}

#pragma region Интерфейс{
    // Панель управления
    controlLayout->addWidget(new QLabel("Операция:"));
    controlLayout->addWidget(m_operationCombo);
    controlLayout->addWidget(m_calcBtn);
    controlLayout->addWidget(m_clearBtn);
    controlLayout->addWidget(m_historyBtn);

    // Матрица А
    layoutA->addWidget(new QLabel("Строк:"), 0, 0);
    layoutA->addWidget(m_rowsSpinA, 0, 1);
    layoutA->addWidget(new QLabel("Столбцов:"), 0, 2);
    layoutA->addWidget(m_colsSpinA, 0, 3);
    layoutA->addWidget(m_inputTableA, 1, 0, 1, 5);

    // Матрица B
    layoutB->addWidget(new QLabel("Строк:"), 0, 0);
    layoutB->addWidget(m_rowsSpinB, 0, 1);
    layoutB->addWidget(new QLabel("Столбцов:"), 0, 2);
    layoutB->addWidget(m_colsSpinB, 0, 3);
    layoutB->addWidget(m_inputTableB, 1, 0, 1, 5);

    // Главный макет
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_groupA);
    mainLayout->addWidget(m_groupB);
    mainLayout->addWidget(new QLabel("Результат:"));
    mainLayout->addWidget(m_outputTable);
    mainLayout->addWidget(m_statusLabel);

    // Инициализация таблиц
    onResizeMatrixA();
    onResizeMatrixB();
}
#pragma endregion}

#pragma region Реализация функционала{

void MatrixTab::showHistory() {
    m_historyDialog->show();
}

void MatrixTab::onResizeMatrixA() {
    int r = m_rowsSpinA->value();
    int c = m_colsSpinA->value();

    m_inputTableA->setRowCount(r);
    m_inputTableA->setColumnCount(c);
}

void MatrixTab::onResizeMatrixB() {
    int r = m_rowsSpinB->value();
    int c = m_colsSpinB->value();

    m_inputTableB->setRowCount(r);
    m_inputTableB->setColumnCount(c);
}

MatrixD MatrixTab::getMatrixFromTable(QTableWidget* table) const {
    int r = table->rowCount();
    int c = table->columnCount();
    MatrixD m(r, c);
    for(int i = 0; i < r; ++i) {
        for(int j = 0; j < c; ++j) {
            QString txt = table->item(i, j) ? table->item(i, j)->text() : "0";
            m(i, j) = txt.toDouble();
        }
    }
    return m;
}

void MatrixTab::setMatrixToTable(const MatrixD& mat, QTableWidget* table) {
    table->setRowCount(mat.getRows());
    table->setColumnCount(mat.getCols());
    for(size_t i = 0; i < mat.getRows(); ++i) {
        for(size_t j = 0; j < mat.getCols(); ++j) {
            table->setItem(i, j, new QTableWidgetItem(QString::number(mat(i, j), 'g', 6)));
        }
    }
}

void MatrixTab::onClearAll() {
    if (!m_inputTableA || !m_inputTableB || !m_outputTable) return;

    m_inputTableA->clearContents();
    m_inputTableB->clearContents();
    m_outputTable->clearContents();
    m_outputTable->setRowCount(0);
    m_outputTable->setColumnCount(0);

    m_statusLabel->setText("Все данные очищены");
    m_statusLabel->setStyleSheet("color: blue; font-weight: bold;");
}

void MatrixTab::onCalculate() {
    try {
        MatrixD mA = getMatrixFromTable(m_inputTableA);
        MatrixD res(1, 1);
        MatrixD mB;
        QString msg;
        int op = m_operationCombo->currentIndex();
        QString operationName = m_operationCombo->currentText();
        bool hasMatrixB = false;

        if (op == 0) { // Определитель
            if (!mA.isSquare()) throw std::invalid_argument("Нужна квадратная матрица");
            double d = mA.determinant();
            msg = "Определитель A: " + QString::number(d, 'g', 6);
            m_outputTable->setRowCount(1);
            m_outputTable->setColumnCount(1);
            m_outputTable->setItem(0, 0, new QTableWidgetItem(QString::number(d, 'g', 6)));
            res = MatrixD(1, 1);
            res(0, 0) = d;
        }
        else if (op == 1) { // Транспонирование
            res = mA.transpose();
            setMatrixToTable(res, m_outputTable);
            msg = "Транспонирование A выполнено";
        }
        else if (op == 2) { // Обратная
            if (!mA.isSquare()) throw std::invalid_argument("Нужна квадратная матрица");
            res = mA.inverse();
            setMatrixToTable(res, m_outputTable);
            msg = "Обратная матрица A найдена";
        }
        else if (op == 3) { // Сложение A + B
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA + mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Сложение A + B выполнено";
        }
        else if (op == 4) { // Вычитание A - B
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA - mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Вычитание A - B выполнено";
        }
        else if (op == 5) { // Умножение A * B
            mB = getMatrixFromTable(m_inputTableB);
            hasMatrixB = true;
            res = mA * mB;
            setMatrixToTable(res, m_outputTable);
            msg = "Умножение A * B выполнено";
        }
        else if (op == 6) { // Умножение на число
            res = mA * 2.0;
            setMatrixToTable(res, m_outputTable);
            msg = "Умножение A на 2 выполнено";
        }
        else if (op == 7) { // Степень
            if (!mA.isSquare()) throw std::invalid_argument("Нужна квадратная матрица");
            res = mA.power(2);
            setMatrixToTable(res, m_outputTable);
            msg = "A возведено в степень 2";
        }

        // Добавление в историю
        m_historyDialog->addOperation(operationName, mA, mB, res, hasMatrixB);

        m_statusLabel->setText(msg);
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления", e.what());
        m_statusLabel->setText("Ошибка: " + QString(e.what()));
        m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
#pragma endregion}
}