#include "adddaydialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>

AddDayDialog::AddDayDialog(QWidget *parent)
    : QDialog(parent) {

    // 设置窗口标题和属性
    setWindowTitle("编辑重要日");
    setModal(true);

    // 创建标题输入框
    m_titleEdit = new QLineEdit(this);

    // 创建日期选择框
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setMinimumDate(QDate::currentDate().addYears(-100));
    m_dateEdit->setMaximumDate(QDate::currentDate().addYears(100));

    // 创建表单布局
    auto *lay = new QFormLayout(this);
    lay->addRow("标题", m_titleEdit);
    lay->addRow("日期", m_dateEdit);

    // 创建按钮框
    auto *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    lay->addWidget(btnBox);

    // 连接确定按钮信号
    connect(btnBox, &QDialogButtonBox::accepted, this, [=]{
        if (m_titleEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "标题不能为空！");
            return;
        }

        if (!m_dateEdit->date().isValid()) {
            QMessageBox::warning(this, "输入错误", "日期无效！");
            return;
        }

        accept();
    });

    // 连接取消按钮信号
    connect(btnBox, &QDialogButtonBox::rejected, this, &AddDayDialog::reject);
}

// 获取标题
QString AddDayDialog::title() const {
    return m_titleEdit->text();
}

// 获取日期
QDate AddDayDialog::date() const {
    return m_dateEdit->date();
}

// 设置标题
void AddDayDialog::setTitle(const QString &text) {
    m_titleEdit->setText(text);
}

// 设置日期
void AddDayDialog::setDate(const QDate &d) {
    if (d.isValid()) {
        m_dateEdit->setDate(d);
    } else {
        m_dateEdit->setDate(QDate::currentDate());
        QMessageBox::warning(this, "数据错误", "无效日期，已重置为当前日期！");
    }
}
