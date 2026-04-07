#ifndef MATRIXTAB_H
#define MATRIXTAB_H

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include "OperationHistory.h"

using MatrixD = Matrix<double>;

class MatrixTab : public QWidget {
    Q_OBJECT
public:
    explicit MatrixTab(QWidget *parent = nullptr);

private slots:
    void onResizeMatrixA();
    void onResizeMatrixB();
    void onCalculate();
    void onClearAll();
    void showHistory();

private:
    QTableWidget *m_inputTableA;
    QTableWidget *m_inputTableB;
    QTableWidget *m_outputTable;

    QSpinBox *m_rowsSpinA;
    QSpinBox *m_colsSpinA;
    QGroupBox *m_groupA;

    QSpinBox *m_rowsSpinB;
    QSpinBox *m_colsSpinB;
    QGroupBox *m_groupB;

    QComboBox *m_operationCombo;
    QPushButton *m_calcBtn;
    QPushButton *m_clearBtn;
    QPushButton *m_historyBtn;
    QLabel *m_statusLabel;

    OperationHistoryDialog *m_historyDialog;

    MatrixD getMatrixFromTable(QTableWidget* table) const;
    void setMatrixToTable(const MatrixD& mat, QTableWidget* table);
};

#endif // MATRIXTAB_H