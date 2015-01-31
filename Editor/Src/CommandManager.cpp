#include "CommandManager.h"

void CommandManager::ExecuteCommand(ICommand* command)
{
    if (command == NULL)
        return;
    
    // Execute the command!
    command->Execute();
    PushCommand(command);

    // Don't let stack grow indefinitely. Throw away oldest command if stack is too large
    if (m_commandStack.size() > MAX_STACK_SIZE)
    {
        delete m_commandStack.front();
        m_commandStack.pop_front();
    }

    // Since a new command was just executed, we clear the previous undo stack
    for (unsigned int i = 0; i < m_undoStack.size(); i++)
    {
        delete m_undoStack.front();
        m_undoStack.pop_front();
    }
}

bool CommandManager::Undo(int numCommands)
{
    for (int i = 0; i < numCommands; i++)
    {
        if (!CanUndo())
            return false;
        
        ICommand* command = m_commandStack.back();
        command->Undo();
        m_commandStack.pop_back();
        m_undoStack.push_back(command);
    }
    return true;
}

bool CommandManager::Redo(int numCommands)
{
    for (int i = 0; i < numCommands; i++)
    {
        if (!CanRedo())
            return false;

        ICommand* command = m_undoStack.back();
        command->Execute();
        m_undoStack.pop_back();
        m_commandStack.push_back(command);
    }
    return true;
}

bool CommandManager::CanUndo()
{
    return m_commandStack.size() > 0;
}

bool CommandManager::CanRedo()
{
    return m_undoStack.size() > 0;
}

void CommandManager::PushCommand(ICommand* command)
{
    // Check whether this command can be collapsed into the command that
    // is currently on the top of the stack
    if (CanUndo())
    {
        ICommand* top = m_commandStack.back();
        if (top->Collapse(command))
        {
            // We successfully collapsed the new command into the top command,
            // so we don't need to add it to the stack
            return;
        }
    }

    // We couldn't collapse the command, so push it onto the stack
    m_commandStack.push_back(command);
}