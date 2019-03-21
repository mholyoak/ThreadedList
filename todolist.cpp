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
    appendItemDescHandle(false, "");
}

void ToDoList::appendItemDescHandle(bool done, QString desc)
{
    emit preItemAppended();

    auto threadId = std::this_thread::get_id();
    std::cout << "ThreadID: " << threadId << " UI Thread add Item '" << desc.toStdString() << "'" << std::endl;

    ToDoItem item;
    item.done = done;
    item.description = desc;
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
    connect(this, &ToDoList::appendItemDescSignal, this, &ToDoList::appendItemDescHandle);

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
    for (int x = 0; !mTerminateThread; x++)
    {
        std::this_thread::sleep_for (std::chrono::seconds(5));
        auto threadId = std::this_thread::get_id();
        std::string desc = "New Thread Item " + std::to_string(x);
        std::cout << "ThreadID: " << threadId << " Secondary thread add Item '" << desc << "'" << std::endl;

        emit appendItemDescSignal(false, desc.c_str());
    }
}
