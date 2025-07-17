#ifndef ADDDAYDIALOG_H
#define ADDDAYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>

// 添加重要日期对话框类
class AddDayDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数
    explicit AddDayDialog(QWidget *parent = nullptr);

    // 获取标题
    QString title() const;

    // 获取日期
    QDate date() const;

    // 设置标题
    void setTitle(const QString &text);

    // 设置日期
    void setDate(const QDate &d);

private:
    // 标题输入框
    QLineEdit *m_titleEdit;

    // 日期选择框
    QDateEdit *m_dateEdit;
};

#endif // ADDDAYDIALOG_H
