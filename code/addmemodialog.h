#ifndef ADDMEMODIALOG_H
#define ADDMEMODIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;

// 添加备忘录对话框类
class AddMemoDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数
    explicit AddMemoDialog(QWidget *parent = nullptr);

    // 获取标题
    QString title() const;

    // 获取内容
    QString content() const;

    // 设置标题
    void setTitle(const QString &text);

    // 设置内容
    void setContent(const QString &text);

private:
    // 标题输入框
    QLineEdit *m_titleEdit;

    // 内容编辑框
    QTextEdit *m_contentEdit;
};

#endif // ADDMEMODIALOG_H
