#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

// 数据库管理单例类
class DataBaseManager : public QObject {
    Q_OBJECT

public:
    // 获取单例实例
    static DataBaseManager *instance();

    // 获取数据库引用
    QSqlDatabase &db();

private:
    // 构造函数
    explicit DataBaseManager(QObject *parent = nullptr);

    // 析构函数
    ~DataBaseManager();

    // 禁用拷贝构造函数和赋值操作符
    Q_DISABLE_COPY(DataBaseManager)

    // 数据库连接对象
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
