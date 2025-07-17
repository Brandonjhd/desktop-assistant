#include "memomanager.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MemoManager::MemoManager(QObject *parent) : QObject(parent) {
    m_db = DataBaseManager::instance()->db();
    if (!m_db.isOpen()) {
        qWarning() << "数据库未打开，无法初始化 MemoManager！";
    }
}

// 添加备忘录
bool MemoManager::addMemo(const QString &title, const QString &content) {
    if (title.trimmed().isEmpty() || content.trimmed().isEmpty()) {
        return false;
    }

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO memo(title, content) VALUES (?, ?)");
    q.addBindValue(title.trimmed());
    q.addBindValue(content.trimmed());

    if (!q.exec()) {
        qWarning() << "addMemo error:" << q.lastError();
        return false;
    }

    return true;
}

// 获取备忘录标题列表
QStringList MemoManager::listMemos() {
    QStringList list;
    QSqlQuery q("SELECT title FROM memo ORDER BY id DESC", m_db);

    while (q.next()) {
        list.append(q.value(0).toString());
    }

    return list;
}

// 根据ID获取备忘录内容
QString MemoManager::memoContent(int id) {
    if (id <= 0) {
        return QString();
    }

    QSqlQuery q(m_db);
    q.prepare("SELECT content FROM memo WHERE id = ?");
    q.addBindValue(id);

    if (!q.exec() || !q.next()) {
        return QString();
    }

    return q.value(0).toString();
}

// 根据ID删除备忘录
bool MemoManager::delMemo(int id) {
    if (id <= 0) {
        return false;
    }

    QSqlQuery q(m_db);
    q.prepare("DELETE FROM memo WHERE id = ?");
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "delMemo error:" << q.lastError();
        return false;
    }

    return q.numRowsAffected() > 0;
}

// 根据ID更新备忘录
bool MemoManager::updateMemo(int id, const QString &newTitle, const QString &newContent) {
    if (id <= 0 || newTitle.trimmed().isEmpty() || newContent.trimmed().isEmpty()) {
        return false;
    }

    QSqlQuery q(m_db);
    q.prepare("UPDATE memo SET title = ?, content = ? WHERE id = ?");
    q.addBindValue(newTitle.trimmed());
    q.addBindValue(newContent.trimmed());
    q.addBindValue(id);

    if (!q.exec()) {
        qWarning() << "updateMemo error:" << q.lastError();
        return false;
    }

    return q.numRowsAffected() > 0;
}

// 根据标题获取ID
int MemoManager::getIdByTitle(const QString &title) {
    if (title.trimmed().isEmpty()) {
        return -1;
    }

    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM memo WHERE title = ? LIMIT 1");
    q.addBindValue(title.trimmed());

    if (!q.exec() || !q.next()) {
        return -1;
    }

    return q.value(0).toInt();
}
