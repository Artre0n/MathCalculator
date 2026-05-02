#include "PolynomialTab.h"
#include <QMessageBox>
#include <QGridLayout>

PolynomialTab::PolynomialTab(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);
    
    // Панель управления 
    auto *controlLayout = new QHBoxLayout();
    
	// Выбор степени полинома
    m_degreeSpin = new QSpinBox(this);
    m_degreeSpin->setRange(0, 10);
    m_degreeSpin->setValue(2);
	// Динамическое обновление полей коэффициентов при изменении степени
    connect(m_degreeSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &PolynomialTab::onDegreeChanged);
	// Выбор операции
    m_operationCombo = new QComboBox(this);
    m_operationCombo->addItems({
        "Сложение (A + B)",
        "Вычитание (A - B)",
        "Умножение (A * B)",
        "Деление (A / B)",
        "Вычисление значения A(x)",
        "Производная A'",
        "Интеграл ∫A",
        "Корни уравнения A(x) = 0"
    });
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &PolynomialTab::onOperationChanged);

	// Кнопка вычисления
    m_calcBtn = new QPushButton("Вычислить", this);
    connect(m_calcBtn, &QPushButton::clicked, this, &PolynomialTab::onCalculate);

	// Кнопка очистки
    m_clearBtn = new QPushButton("Очистить всё", this);
    m_clearBtn->setStyleSheet("background-color: #ff6b6b; color: white; font-weight: bold;");
    connect(m_clearBtn, &QPushButton::clicked, this, &PolynomialTab::onClearAll);
    
    controlLayout->addWidget(new QLabel("Степень:"));
    controlLayout->addWidget(m_degreeSpin);
    controlLayout->addWidget(new QLabel("Операция:"));
    controlLayout->addWidget(m_operationCombo);
    controlLayout->addWidget(m_calcBtn);
    controlLayout->addWidget(m_clearBtn);
    
    mainLayout->addLayout(controlLayout);
    
    // Полином A 
    QGroupBox *groupA = new QGroupBox("Полином A", this);
    auto *layoutA = new QVBoxLayout(groupA);
	// Область с прокруткой для коэффициентов
    QScrollArea *scrollA = new QScrollArea();
    scrollA->setWidgetResizable(true);
    scrollA->setMaximumHeight(120);
    m_scrollContentA = new QWidget();
    auto *coeffLayoutA = new QGridLayout(m_scrollContentA);
    coeffLayoutA->addWidget(new QLabel("Коэффициенты (a₀ + a₁x + a₂x² + ...):"), 0, 0, 1, -1);
    
    scrollA->setWidget(m_scrollContentA);
    layoutA->addWidget(scrollA);
    mainLayout->addWidget(groupA);
    
    // Полином B 
    m_groupB = new QGroupBox("Полином B", this);
    auto *layoutB = new QVBoxLayout(m_groupB);
	// Область с прокруткой для коэффициентов
    QScrollArea *scrollB = new QScrollArea();
    scrollB->setWidgetResizable(true);
    scrollB->setMaximumHeight(120);
    m_scrollContentB = new QWidget();
    auto *coeffLayoutB = new QGridLayout(m_scrollContentB);
    coeffLayoutB->addWidget(new QLabel("Коэффициенты (a₀ + a₁x + a₂x² + ...):"), 0, 0, 1, -1);
    
    scrollB->setWidget(m_scrollContentB);
    layoutB->addWidget(scrollB);
    mainLayout->addWidget(m_groupB);
    
    // Результат
    mainLayout->addWidget(new QLabel("Результат:"));
    m_resultEdit = new QTextEdit();
    m_resultEdit->setReadOnly(true);
    m_resultEdit->setMaximumHeight(80);
    m_resultEdit->setStyleSheet("font-family: 'Courier New'; font-size: 12px;");
    mainLayout->addWidget(m_resultEdit);
    
    m_statusLabel = new QLabel("Готов к работе (макс. степень 10)");
    m_statusLabel->setStyleSheet("color: blue; font-weight: bold;");
    mainLayout->addWidget(m_statusLabel);
    
    // Инициализация
    rebuildCoefficientInputs();
    onOperationChanged(0);
}

void PolynomialTab::rebuildCoefficientInputs() {
    int degree = m_degreeSpin->value();
    
	// Удаляем старые поля ввода коэффициентов
    for (auto *edit : m_coefficientsA) {
        edit->deleteLater();
    }
    for (auto *edit : m_coefficientsB) {
        edit->deleteLater();
    }
    m_coefficientsA.clear();
    m_coefficientsB.clear();
    
	// Очищаем старые виджеты из разметки
    QGridLayout *layoutA = qobject_cast<QGridLayout*>(m_scrollContentA->layout());
    QGridLayout *layoutB = qobject_cast<QGridLayout*>(m_scrollContentB->layout());
    
	// Удаляем все виджеты, кроме заголовков
    QLayoutItem *child;
    while ((child = layoutA->takeAt(1)) != nullptr) {  // Начинаем с 1, чтобы сохранить заголовок
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    while ((child = layoutB->takeAt(1)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    
	// Создаем новые поля ввода для коэффициентов
    for (int i = 0; i <= degree; ++i) {
        int row = 1 + i / 5;
        int col = (i % 5) * 2;
        
        // Полином A
        QLabel *labelA = new QLabel(QString("a%1:").arg(i));
        labelA->setAlignment(Qt::AlignRight);
        QLineEdit *editA = new QLineEdit("0");
        editA->setMaximumWidth(70);
        editA->setPlaceholderText(QString("a%1").arg(i));
        layoutA->addWidget(labelA, row, col);
        layoutA->addWidget(editA, row, col + 1);
        m_coefficientsA.push_back(editA);
        
        // Полином B
        QLabel *labelB = new QLabel(QString("a%1:").arg(i));
        labelB->setAlignment(Qt::AlignRight);
        QLineEdit *editB = new QLineEdit("0");
        editB->setMaximumWidth(70);
        editB->setPlaceholderText(QString("a%1").arg(i));
        layoutB->addWidget(labelB, row, col);
        layoutB->addWidget(editB, row, col + 1);
        m_coefficientsB.push_back(editB);
    }
}

// Слот для обработки изменения степени полинома
void PolynomialTab::onDegreeChanged(int newDegree) {
    rebuildCoefficientInputs();
    m_statusLabel->setText("Степень изменена: " + QString::number(newDegree));
}

// Слот для обработки изменения выбранной операции
void PolynomialTab::onOperationChanged(int index) {
    bool needPolyB = (index <= 3);
    m_groupB->setVisible(needPolyB);
}
// Получение полинома A из полей ввода
PolynomialD PolynomialTab::getPolynomialA() const {
    std::vector<double> coeffs;
    for (auto *edit : m_coefficientsA) {
        coeffs.push_back(edit->text().toDouble());
    }
    return PolynomialD(coeffs);
}
// Получение полинома B из полей ввода
PolynomialD PolynomialTab::getPolynomialB() const {
    std::vector<double> coeffs;
    for (auto *edit : m_coefficientsB) {
        coeffs.push_back(edit->text().toDouble());
    }
    return PolynomialD(coeffs);
}
// Слот для обработки нажатия кнопки "Очистить всё"
void PolynomialTab::onClearAll() {
    for (auto *edit : m_coefficientsA) edit->setText("0");
    for (auto *edit : m_coefficientsB) edit->setText("0");
    m_resultEdit->clear();
    m_statusLabel->setText("Все данные очищены");
    m_statusLabel->setStyleSheet("color: blue; font-weight: bold;");
}
// Слот для обработки нажатия кнопки "Вычислить"
void PolynomialTab::onCalculate() {
    try {
        int op = m_operationCombo->currentIndex();
        PolynomialD polyA = getPolynomialA();
        QString msg;
        
        if (op == 0) { // Сложение A + B
            PolynomialD polyB = getPolynomialB();
            PolynomialD res = polyA + polyB;
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()));
            msg = "Сложение выполнено";
        }
        else if (op == 1) { // Вычитание A - B
            PolynomialD polyB = getPolynomialB();
            PolynomialD res = polyA - polyB;
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()));
            msg = "Вычитание выполнено";
        }
        else if (op == 2) { // Умножение A * B
            PolynomialD polyB = getPolynomialB();
            PolynomialD res = polyA * polyB;
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()));
            msg = "Умножение выполнено";
        }
        else if (op == 3) { // Деление A / B
            PolynomialD polyB = getPolynomialB();
            PolynomialD res = polyA / polyB;
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()));
            msg = "Деление выполнено";
        }
        else if (op == 4) { // Вычисление A(x)
            bool ok;
            double x = QInputDialog::getDouble(this, "Значение x", "Введите x:", 0, -1000, 1000, 4, &ok);
            if (!ok) return;
            double res = polyA.evaluate(x);
            m_resultEdit->setPlainText(QString("A(%1) = %2").arg(x).arg(res, 0, 'g', 6));
            msg = QString("Вычислено A(%1)").arg(x);
        }
        else if (op == 5) { // Производная
            PolynomialD res = polyA.derivative();
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()));
            msg = "Производная вычислена";
        }
        else if (op == 6) { // Интеграл
            PolynomialD res = polyA.integral(0);
            m_resultEdit->setPlainText(QString::fromStdString(res.toString()) + " + C");
            msg = "Интеграл вычислен";
        }
        else if (op == 7) { // Корни
            std::vector<double> roots = polyA.roots();
            if (roots.empty()) {
                m_resultEdit->setPlainText("Действительных корней нет (или степень > 2)");
            } else {
                QString text;
                for (size_t i = 0; i < roots.size(); ++i) {
                    text += QString("x%1 = %2\n").arg(i + 1).arg(roots[i], 0, 'g', 6);
                }
                m_resultEdit->setPlainText(text);
            }
            msg = "Корни найдены";
        }
        
        m_statusLabel->setText(msg);
        m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления", e.what());
        m_statusLabel->setText("Ошибка: " + QString(e.what()));
        m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
}