#ifndef MANAGEDAYDIALOG_H
#define MANAGEDAYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QModelIndex>

class ImportantDayManager;

// 重要日期管理对话框类
class ManageDayDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数
    explicit ManageDayDialog(ImportantDayManager *mgr, QWidget *parent = nullptr);

private slots:
    // 添加重要日期
    void addDay();

    // 删除重要日期
    void removeDay();

    // 双击表格行编辑重要日期
    void onDoubleClick(const QModelIndex &index);

private:
    // 重新加载表格数据
    void reload();

    // 表格组件
    QTableWidget *table;

    // 按钮组件
    QPushButton *addBtn;
    QPushButton *delBtn;

    // 重要日期管理器
    ImportantDayManager *m_mgr;
};

#endif // MANAGEDAYDIALOG_H
