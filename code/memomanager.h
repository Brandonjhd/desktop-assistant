#ifndef MEMOMANAGER_H
#define MEMOMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

// 备忘录管理类
class MemoManager : public QObject {
    Q_OBJECT

public:
    // 构造函数
    explicit MemoManager(QObject *parent = nullptr);

    // 添加备忘录
    bool addMemo(const QString &title, const QString &content);

    // 获取备忘录标题列表
    QStringList listMemos();

    // 根据ID获取备忘录内容
    QString memoContent(int id);

    // 根据ID删除备忘录
    bool delMemo(int id);

    // 根据ID更新备忘录
    bool updateMemo(int id, const QString &newTitle, const QString &newContent);

    // 根据标题获取ID
    int getIdByTitle(const QString &title);

private:
    // 数据库连接对象
    QSqlDatabase m_db;
};

#endif // MEMOMANAGER_H
