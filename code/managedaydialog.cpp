#include "managedaydialog.h"
#include "importantdaymanager.h"
#include "adddaydialog.h"
#include "databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDateEdit>

ManageDayDialog::ManageDayDialog(ImportantDayManager *mgr, QWidget *parent)
    : QDialog(parent), m_mgr(mgr) {

    if (!m_mgr) {
        QMessageBox::critical(this, "初始化错误", "重要日管理器为空！");
        return;
    }

    // 设置窗口属性
    setWindowTitle("重要日管理");
    resize(420, 320);
    setModal(true);

    // 初始化表格
    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"标题", "日期"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置表头样式
    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #F0F4F8; }");

    // 连接双击信号
    connect(table, &QTableWidget::doubleClicked, this, &ManageDayDialog::onDoubleClick);

    // 初始化按钮
    addBtn = new QPushButton("新增", this);
    delBtn = new QPushButton("删除", this);

    // 连接按钮信号
    connect(addBtn, &QPushButton::clicked, this, &ManageDayDialog::addDay);
    connect(delBtn, &QPushButton::clicked, this, &ManageDayDialog::removeDay);

    // 创建按钮布局
    QHBoxLayout *btnLot = new QHBoxLayout;
    btnLot->addWidget(addBtn);
    btnLot->addWidget(delBtn);
    btnLot->addStretch();

    // 创建主布局
    QVBoxLayout *root = new QVBoxLayout(this);
    root->addWidget(table);
    root->addLayout(btnLot);

    // 加载数据
    reload();
}

// 重新加载表格数据
void ManageDayDialog::reload() {
    table->setRowCount(0);

    QSqlQuery q(DataBaseManager::instance()->db());
    if (!q.exec("SELECT id, title, date FROM important_day ORDER BY date ASC")) {
        QMessageBox::critical(this, "数据库错误", q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        if (q.value(1).toString().isEmpty() || q.value(2).toString().isEmpty()) {
            continue;
        }

        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(q.value(1).toString()));
        table->setItem(row, 1, new QTableWidgetItem(q.value(2).toString()));
        ++row;
    }

    if (table->rowCount() == 0) {
        QMessageBox::information(this, "提示", "暂无重要日记录");
    }
}

// 添加重要日期
void ManageDayDialog::addDay() {
    AddDayDialog dia(this);

    // 设置日期选择框样式
    QDateEdit *dateEdit = dia.findChild<QDateEdit*>();
    if (dateEdit) {
        dateEdit->setStyleSheet("background-color: white; border: 1px solid lightgray;");
    } else {
        QMessageBox::warning(this, "警告", "无法找到日期编辑控件！");
        return;
    }

    if (dia.exec() == QDialog::Accepted) {
        if (dia.title().trimmed().isEmpty() || !dia.date().isValid()) {
            QMessageBox::warning(this, "输入错误", "标题不能为空且日期必须有效！");
            return;
        }

        if (!m_mgr->addDay(dia.title(), dia.date())) {
            QMessageBox::warning(this, "失败", "新增失败！");
        }

        reload();
    }
}

// 删除重要日期
void ManageDayDialog::removeDay() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "操作错误", "请先选择一行！");
        return;
    }

    auto *titleItem = table->item(row, 0);
    auto *dateItem = table->item(row, 1);
    if (!titleItem || !dateItem) {
        return;
    }

    QString t = titleItem->text();
    QString dStr = dateItem->text();
    QDate d = QDate::fromString(dStr, Qt::ISODate);

    if (!d.isValid()) {
        QMessageBox::warning(this, "数据错误", "无效日期！");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除该记录吗？") == QMessageBox::Yes) {
        QSqlQuery q(DataBaseManager::instance()->db());
        q.prepare("SELECT id FROM important_day WHERE title = ? AND date = ?");
        q.addBindValue(t);
        q.addBindValue(d.toString(Qt::ISODate));

        if (q.exec() && q.next()) {
            int id = q.value(0).toInt();
            if (!m_mgr->removeDay(id)) {
                QMessageBox::warning(this, "失败", "删除失败！");
            }
            reload();
        } else {
            QMessageBox::warning(this, "查询失败", q.lastError().text());
        }
    }
}

// 双击表格行编辑重要日期
void ManageDayDialog::onDoubleClick(const QModelIndex &index) {
    int row = index.row();
    if (row < 0) {
        QMessageBox::warning(this, "操作错误", "请先选择一行！");
        return;
    }

    auto *titleItem = table->item(row, 0);
    auto *dateItem = table->item(row, 1);
    if (!titleItem || !dateItem) {
        return;
    }

    QString t = titleItem->text();
    QDate d = QDate::fromString(dateItem->text(), Qt::ISODate);

    if (!d.isValid()) {
        QMessageBox::warning(this, "数据错误", "无效日期！");
        return;
    }

    AddDayDialog dia(this);
    dia.setTitle(t);
    dia.setDate(d);

    // 设置日期选择框样式
    QDateEdit *dateEdit = dia.findChild<QDateEdit*>();
    if (dateEdit) {
        dateEdit->setStyleSheet("background-color: white; border: 1px solid lightgray;");
    } else {
        QMessageBox::warning(this, "警告", "无法找到日期编辑控件！");
        return;
    }

    if (dia.exec() == QDialog::Accepted) {
        if (dia.title().trimmed().isEmpty() || !dia.date().isValid()) {
            QMessageBox::warning(this, "输入错误", "标题不能为空且日期必须有效！");
            return;
        }

        QSqlQuery q(DataBaseManager::instance()->db());
        q.prepare("SELECT id FROM important_day WHERE title = ? AND date = ?");
        q.addBindValue(t);
        q.addBindValue(d.toString(Qt::ISODate));

        if (q.exec() && q.next()) {
            int id = q.value(0).toInt();
            if (!m_mgr->updateDay(id, dia.title(), dia.date())) {
                QMessageBox::warning(this, "失败", "更新失败！");
            }
            reload();
        } else {
            QMessageBox::warning(this, "查询失败", q.lastError().text());
        }
    }
}
