#pragma once

#include <deque>

using std::deque;

class ICommand
{
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;

    // TODO proper delete!
};

class CommandManager
{
public:
    const int MAX_STACK_SIZE = 20;

    void ExecuteCommand(ICommand* command);
    void Undo(int numCommands = 1);
    void Redo(int numCommands = 1);

    bool CanUndo();
    bool CanRedo();

private:
    deque<ICommand*> m_commandStack;    // this is a deque instead of just a stack so that we
                                        // can remove items at the front if there are too many items
                                        // (we don't want the command stack to grow indefinitely)

    deque<ICommand*> m_undoStack;
};