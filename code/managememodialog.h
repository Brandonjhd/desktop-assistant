#ifndef MANAGEMEMODIALOG_H
#define MANAGEMEMODIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class MemoManager;

// 备忘录管理对话框类
class ManageMemoDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数
    explicit ManageMemoDialog(MemoManager *mgr, QWidget *parent = nullptr, int showId = -1);

private slots:
    // 添加备忘录
    void addMemo();

    // 删除备忘录
    void removeMemo();

    // 显示选中行的内容
    void showContent(int row);

    // 监测标题修改
    void onTitleChanged();

    // 监测内容修改
    void onContentChanged();

    // 保存修改
    void saveChanges();

private:
    // 重新加载表格数据
    void reload();

    // 按ID定位
    void locate(int id);

    // 检查是否修改并显示保存按钮
    void checkModified();

    // 表格组件
    QTableWidget *table{nullptr};

    // 标题编辑框
    QLineEdit *titleEdit{nullptr};

    // 内容编辑框
    QTextEdit *content{nullptr};

    // 按钮组件
    QPushButton *addBtn{nullptr};
    QPushButton *delBtn{nullptr};
    QPushButton *saveBtn{nullptr};

    // 备忘录管理器
    MemoManager *m_mgr{nullptr};

    // 用于追踪原始数据以检测修改
    QString originalTitle;
    QString originalContent;
    int currentId = -1;
};

#endif // MANAGEMEMODIALOG_H
