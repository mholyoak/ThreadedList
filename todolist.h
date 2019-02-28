#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>
#include <memory>
#include <thread>

struct ToDoItem
{
    bool done;
    QString description;
};

class ToDoList : public QObject
{
    Q_OBJECT
public:
    explicit ToDoList(QObject *parent = nullptr);

    QVector<ToDoItem> items() const;

    bool setItemAt(int index, const ToDoItem &item);

    void runThread();
    void stopThread();

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();

private:
    bool    mTerminateThread;
    std::shared_ptr<std::thread> mThread;
    QVector<ToDoItem> mItems;

    // Thread Method
    void execute ();

signals:
    void appendThreadItem();

};

#endif // TODOLIST_H
