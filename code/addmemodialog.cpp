#include "addmemodialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>

AddMemoDialog::AddMemoDialog(QWidget *parent)
    : QDialog(parent) {

    // 设置窗口标题和属性
    setWindowTitle("添加备忘录");
    setModal(true);

    // 创建标题输入框
    m_titleEdit = new QLineEdit(this);

    // 创建内容编辑框
    m_contentEdit = new QTextEdit(this);

    // 创建表单布局
    QFormLayout *lay = new QFormLayout(this);
    lay->addRow("标题", m_titleEdit);
    lay->addRow("内容", m_contentEdit);

    // 创建按钮框
    auto *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    lay->addWidget(btnBox);

    // 连接确定按钮信号
    connect(btnBox, &QDialogButtonBox::accepted, this, [=] {
        if (m_titleEdit->text().trimmed().isEmpty() || m_contentEdit->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "标题和内容均不能为空！");
            return;
        }

        accept();
    });

    // 连接取消按钮信号
    connect(btnBox, &QDialogButtonBox::rejected, this, &AddMemoDialog::reject);
}

// 获取标题
QString AddMemoDialog::title() const {
    return m_titleEdit->text();
}

// 获取内容
QString AddMemoDialog::content() const {
    return m_contentEdit->toPlainText();
}

// 设置标题
void AddMemoDialog::setTitle(const QString &text) {
    m_titleEdit->setText(text);
}

// 设置内容
void AddMemoDialog::setContent(const QString &text) {
    m_contentEdit->setPlainText(text);
}
