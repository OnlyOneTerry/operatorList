#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include<QUndoStack>

class UndoStack
{
public:
   static QUndoStack*  InitStack();
private:
  UndoStack();
  static  QUndoStack* m_undoStack;
};

#endif // UNDOSTACK_H
