#pragma once

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <vector>
#include <QTextEdit>
#include <QFont>
#include <QHBoxLayout>
#include <QTableWidget>
#include "Matrix.h"

using MatrixD = Matrix<double>;

/// Запись об одной операции в истории
struct OperationRecord {
    QString operationType;  // Название операции
    MatrixD matrixA;        // Матрица A (всегда присутствует)
    MatrixD matrixB;        // Матрица B (может быть пустой)
    MatrixD result;         // Результат операции
    bool hasMatrixB;        // Была ли использована матрица B
};


/// Диалог просмотра истории матричных операций
class OperationHistoryDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperationHistoryDialog(QWidget* parent = nullptr);

    /// Добавить новую операцию в историю
    void addOperation(const QString& type, const MatrixD& mA,
        const MatrixD& mB, const MatrixD& res, bool hasB);

    /// Очистить всю историю
    void clearHistory();

signals:
    /// Сигнал: загрузить матрицу в поле A основного окна
    void loadMatrixToA(const MatrixD& mat);

private slots:
    void onClearHistory();       // Очистить историю
    void onLoadToA(int index);   // Загрузить результат операции в A

private:
    /// Обновить отображение истории (перестроить виджеты)
    void updateHistoryDisplay();

    /// Преобразовать матрицу в текст для отображения
    QString matrixToString(const MatrixD& mat);

    std::vector<OperationRecord> m_history;  // Все записи истории

    // Виджеты интерфейса
    QVBoxLayout* m_mainLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;
    QVBoxLayout* m_contentLayout;
};