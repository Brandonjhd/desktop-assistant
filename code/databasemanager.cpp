#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

// 获取单例实例
DataBaseManager *DataBaseManager::instance() {
    static DataBaseManager ins;
    return &ins;
}

// 获取数据库引用
QSqlDatabase &DataBaseManager::db() {
    return m_db;
}

// 构造函数
DataBaseManager::DataBaseManager(QObject *parent)
    : QObject(parent) {

    // 获取应用程序数据目录
    const QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDir.isEmpty()) {
        QMessageBox::critical(nullptr, "初始化错误", "无法获取可写入的 AppData 目录！");
        return;
    }

    // 创建目录
    QDir dir(appDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            QMessageBox::critical(nullptr, "初始化错误", QString("创建目录失败: %1").arg(appDir));
            return;
        }
    }

    // 配置数据库连接
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    const QString dbPath = dir.filePath("data.db");
    if (dbPath.isEmpty()) {
        QMessageBox::critical(nullptr, "初始化错误", "数据库路径无效！");
        return;
    }

    m_db.setDatabaseName(dbPath);

    // 打开数据库连接
    if (!m_db.open()) {
        QString errorMsg = QString("数据库打开失败: %1\n路径: %2").arg(m_db.lastError().text()).arg(dbPath);
        QMessageBox::critical(nullptr, "初始化错误", errorMsg);
        qCritical() << errorMsg;
        return;
    }

    // 执行数据库初始化
    QSqlQuery query(m_db);

    // 启用外键约束
    query.exec("PRAGMA foreign_keys = ON");

    // 创建重要日期表
    if (!query.exec(R"(CREATE TABLE IF NOT EXISTS important_day(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        title TEXT NOT NULL,
        date TEXT NOT NULL))")) {
        QMessageBox::warning(nullptr, "数据库警告", QString("important_day 表创建失败: %1").arg(query.lastError().text()));
    }

    // 创建备忘录表
    if (!query.exec(R"(CREATE TABLE IF NOT EXISTS memo(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        title TEXT NOT NULL,
        content TEXT NOT NULL,
        ctime TEXT DEFAULT (datetime('now','localtime'))))")) {
        QMessageBox::warning(nullptr, "数据库警告", QString("memo 表创建失败: %1").arg(query.lastError().text()));
    }
}

DataBaseManager::~DataBaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}
