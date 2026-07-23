#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class FindReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit FindReplaceDialog(QPlainTextEdit *editor, QWidget *parent = nullptr);
    ~FindReplaceDialog() = default;

private slots:
    void findNext();
    void replaceCurrent();
    void replaceAll();

private:
    void setupUI();

    QPlainTextEdit *m_editor;
    
    QLineEdit *m_findInput;
    QLineEdit *m_replaceInput;
    
    QPushButton *m_findButton;
    QPushButton *m_replaceButton;
    QPushButton *m_replaceAllButton;
    QPushButton *m_closeButton;
    
    QCheckBox *m_caseCheckBox;
    QCheckBox *m_wholeWordCheckBox;
};

#endif // FINDREPLACEDIALOG_H