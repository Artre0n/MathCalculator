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
#include "Matrix.h"

using MatrixD = Matrix<double>;

struct OperationRecord {
    QString operationType;
    MatrixD matrixA;
    MatrixD matrixB;
    MatrixD result;
    bool hasMatrixB;
};

class OperationHistoryDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperationHistoryDialog(QWidget *parent = nullptr);
    void addOperation(const QString& type, const MatrixD& mA, const MatrixD& mB,
                      const MatrixD& res, bool hasB);
    void clearHistory();

private slots:
    void onClearHistory();

private:
    void updateHistoryDisplay();
    QString matrixToString(const MatrixD& mat);

    std::vector<OperationRecord> m_history;
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QVBoxLayout *m_contentLayout;
};

