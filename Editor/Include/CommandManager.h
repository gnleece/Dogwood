#pragma once

#include <deque>

using std::deque;

class ICommand
{
public:
    virtual void Execute() = 0;
    virtual void Undo() = 0;

    virtual bool Collapse(ICommand* /*command*/) { return false; }

    // TODO proper delete!
};

class CommandManager
{
public:
    static CommandManager& Singleton()
    {
        static CommandManager singleton;
        return singleton;
    }
    CommandManager() {}

    const unsigned int MAX_STACK_SIZE = 20;

    void ExecuteCommand(ICommand* command);
    bool Undo(int numCommands = 1);
    bool Redo(int numCommands = 1);

    bool CanUndo();
    bool CanRedo();

private:
    void PushCommand(ICommand* command);

    deque<ICommand*> m_commandStack;    // this is a deque instead of just a stack so that we
                                        // can remove items at the front if there are too many items
                                        // (we don't want the command stack to grow indefinitely)

    deque<ICommand*> m_undoStack;
};