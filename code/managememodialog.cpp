#include "managememodialog.h"
#include "memomanager.h"
#include "addmemodialog.h"
#include "databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QAbstractItemView>

ManageMemoDialog::ManageMemoDialog(MemoManager *mgr, QWidget *parent, int showId)
    : QDialog(parent), m_mgr(mgr) {

    if (!m_mgr) {
        QMessageBox::critical(this, "初始化错误", "备忘录管理器为空！");
        return;
    }

    // 设置窗口属性
    setWindowTitle("备忘录管理");
    resize(450, 300);
    setModal(true);

    // 初始化表格
    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setHorizontalHeaderLabels({"标题"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #F0F4F8; }");

    // 初始化编辑区
    titleEdit = new QLineEdit(this);
    content = new QTextEdit(this);

    // 初始化按钮
    addBtn = new QPushButton("新增", this);
    delBtn = new QPushButton("删除", this);
    saveBtn = new QPushButton("保存", this);
    saveBtn->hide();

    // 连接信号槽
    connect(addBtn, &QPushButton::clicked, this, &ManageMemoDialog::addMemo);
    connect(delBtn, &QPushButton::clicked, this, &ManageMemoDialog::removeMemo);
    connect(table, &QTableWidget::currentCellChanged, this, [=](int row) { showContent(row); });
    connect(titleEdit, &QLineEdit::textChanged, this, &ManageMemoDialog::onTitleChanged);
    connect(content, &QTextEdit::textChanged, this, &ManageMemoDialog::onContentChanged);
    connect(saveBtn, &QPushButton::clicked, this, &ManageMemoDialog::saveChanges);

    // 布局设置
    QVBoxLayout *leftLot = new QVBoxLayout;
    leftLot->addWidget(table);

    QVBoxLayout *rightLot = new QVBoxLayout;
    rightLot->addWidget(titleEdit);
    rightLot->addWidget(content);

    QHBoxLayout *midLot = new QHBoxLayout;
    midLot->addLayout(leftLot, 1);
    midLot->addLayout(rightLot, 2);

    QHBoxLayout *btnLot = new QHBoxLayout;
    btnLot->addWidget(addBtn);
    btnLot->addWidget(delBtn);
    btnLot->addStretch();
    btnLot->addWidget(saveBtn);

    QVBoxLayout *root = new QVBoxLayout(this);
    root->addLayout(midLot);
    root->addLayout(btnLot);

    // 加载数据并定位
    reload();
    locate(showId);
}

// 重新加载表格数据
void ManageMemoDialog::reload() {
    table->setRowCount(0);

    QSqlQuery q(DataBaseManager::instance()->db());
    if (!q.exec("SELECT id, title FROM memo ORDER BY title ASC")) {
        QMessageBox::critical(this, "数据库错误", q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        if (q.value(1).toString().isEmpty()) continue;

        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(q.value(1).toString()));
        ++row;
    }

    if (table->rowCount() == 0) {
        QMessageBox::information(this, "提示", "暂无备忘录记录");
    }

    // 清空编辑区
    titleEdit->clear();
    content->clear();
    saveBtn->hide();
    currentId = -1;
}

// 按ID定位并选中行
void ManageMemoDialog::locate(int id) {
    if (id < 0) return;

    for (int i = 0; i < table->rowCount(); ++i) {
        auto *item = table->item(i, 0);
        if (!item) continue;

        QString title = item->text();
        int memoId = m_mgr->getIdByTitle(title);
        if (memoId == id) {
            table->setCurrentCell(i, 0);
            showContent(i);
            break;
        }
    }
}

// 检查是否修改
void ManageMemoDialog::checkModified() {
    bool modified = (titleEdit->text() != originalTitle) || (content->toPlainText() != originalContent);
    saveBtn->setVisible(modified);
}

// 添加备忘录
void ManageMemoDialog::addMemo() {
    AddMemoDialog dia(this);
    if (dia.exec() == QDialog::Accepted) {
        QString title = dia.title().trimmed();
        QString content = dia.content().trimmed();
        if (title.isEmpty() || content.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "标题和内容均不能为空！");
            return;
        }

        if (!m_mgr->addMemo(title, content)) {
            QMessageBox::warning(this, "失败", "新增失败！");
        }

        reload();
    }
}

// 删除备忘录
void ManageMemoDialog::removeMemo() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "操作错误", "请先选择一行！");
        return;
    }

    auto *item = table->item(row, 0);
    if (!item) return;

    QString title = item->text();
    int id = m_mgr->getIdByTitle(title);
    if (id <= 0) {
        QMessageBox::warning(this, "错误", "无效ID！");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除该备忘录吗？") == QMessageBox::Yes) {
        if (!m_mgr->delMemo(id)) {
            QMessageBox::warning(this, "失败", "删除失败！");
        } else {
            QMessageBox::information(this, "成功", "已删除！");
        }
        reload();
    }
}

// 显示选中行的内容
void ManageMemoDialog::showContent(int row) {
    if (row < 0) return;

    auto *item = table->item(row, 0);
    if (!item) return;

    QString title = item->text();
    int id = m_mgr->getIdByTitle(title);
    if (id <= 0) {
        content->clear();
        titleEdit->clear();
        return;
    }

    // 获取内容并设置
    QString memoContent = m_mgr->memoContent(id);
    titleEdit->setText(title);
    content->setPlainText(memoContent);

    // 保存原始数据用于修改检测
    originalTitle = title;
    originalContent = memoContent;
    currentId = id;
    saveBtn->hide();
}

// 标题变化处理
void ManageMemoDialog::onTitleChanged() {
    checkModified();
}

// 内容变化处理
void ManageMemoDialog::onContentChanged() {
    checkModified();
}

// 保存修改
void ManageMemoDialog::saveChanges() {
    if (currentId <= 0 || titleEdit->text().trimmed().isEmpty() || content->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "标题和内容均不能为空！");
        return;
    }

    QString newTitle = titleEdit->text().trimmed();
    QString newContent = content->toPlainText().trimmed();
    if (!m_mgr->updateMemo(currentId, newTitle, newContent)) {
        QMessageBox::warning(this, "失败", "更新失败！");
    } else {
        QMessageBox::information(this, "成功", "已保存！");
    }
    reload();
}
