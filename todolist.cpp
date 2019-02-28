#include "todolist.h"

#include <thread>
#include <iostream>

ToDoList::ToDoList(QObject *parent) : QObject(parent),
    mTerminateThread(false)
{
    mItems.append({ true, QStringLiteral("Wash the care")});
    mItems.append({ false, QStringLiteral("Fix the sink")});
}

QVector<ToDoItem> ToDoList::items() const
{
    return mItems;
}

bool ToDoList::setItemAt(int index, const ToDoItem &item)
{
    if (index < 0 || index >= mItems.size())
    {
        return false;
    }

    const ToDoItem &oldItem = mItems.at(index);
    if (item.done == oldItem.done && item.description == oldItem.description)
    {
        return false;
    }

    mItems[index] = item;
    return true;
}

void ToDoList::appendItem()
{
    emit preItemAppended();

    auto threadId = std::this_thread::get_id();
    std::cout << "appendItem ThreadID: " << threadId << std::endl;

    ToDoItem item;
    item.done = false;
    mItems.append(item);

    emit postItemAppended();
}

void ToDoList::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); )
    {
        if (mItems.at(i).done)
        {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        } else {
            i++;
        }
    }
}


void ToDoList::runThread()
{
    connect(this, &ToDoList::appendThreadItem, this, &ToDoList::appendItem);

    mThread = std::make_shared<std::thread>(&ToDoList::execute, this);
}

void ToDoList::stopThread()
{
    mTerminateThread = true;
    if (mThread != nullptr)
    {
        auto threadId = std::this_thread::get_id();
        std::cout << "Exiting Thread - ThreadID: " << threadId << std::endl;
        mThread->join();
    }
}


// Thread Method
void ToDoList::execute ()
{
    for (int x = 0; x < 20 && !mTerminateThread; x++)
    {
        std::this_thread::sleep_for (std::chrono::seconds(5));
        auto threadId = std::this_thread::get_id();
        std::cout << "Thread add Item ThreadID: " << threadId << std::endl;
        emit appendThreadItem();
    }
}
