#include "UndoStack.h"
#include<QDebug>

QUndoStack* UndoStack::m_undoStack = NULL;

UndoStack::UndoStack()
{

}

QUndoStack *UndoStack::InitStack()
{
    if(m_undoStack==NULL)
    {
        m_undoStack = new QUndoStack;
    }
    return m_undoStack;
}

