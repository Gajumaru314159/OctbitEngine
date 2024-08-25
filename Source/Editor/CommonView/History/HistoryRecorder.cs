using Common.Log;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommonView.History
{
    public static class History
    {
        public static void Record(string operation,Action redo, Action undo, bool executeRedo = true)
        {
            if (executeRedo)
            {
                redo();
                Executed?.Invoke();
            }

            s_undoStack.Push(new(operation,undo,redo));
            s_redoStack.Clear();

            Log.Info($"Recorded: {operation}");
        }

        public static void Undo()
        {
            if (s_undoStack.Count > 0)
            {
                var record= s_undoStack.Pop();
                record.Undo();
                s_redoStack.Push(record);
                Executed?.Invoke();
                Log.Info($"Undo: {record.Operation}");
            }
        }

        public static void Redo()
        {
            if (s_redoStack.Count > 0)
            {
                var record = s_redoStack.Pop();
                record.Redo();
                s_undoStack.Push(record);
                Executed?.Invoke();
                Log.Info($"Redo: {record.Operation}");
            }
        }

        public static void ClearForTest()
        {
            s_undoStack.Clear();
            s_redoStack.Clear();
        }

        public static int UndoCount => s_undoStack.Count;
        public static int RedoCount => s_redoStack.Count;
        public static bool CanUndo => 0 < UndoCount;
        public static bool CanRedo => 0 < RedoCount;

        public static IEnumerable<HistoryRecord> UndoRecords => s_undoStack;
        public static IEnumerable<HistoryRecord> RedoRecords => s_redoStack;

        public delegate void ExecutedEvent();

        public static event ExecutedEvent? Executed;


        public record HistoryRecord(string Operation,Action Undo,Action Redo);

        private static Stack<HistoryRecord> s_undoStack = new();
        private static Stack<HistoryRecord> s_redoStack = new();
    }
}
