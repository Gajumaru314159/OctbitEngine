namespace Common.Thread
{
    public interface IProgress
    {
        int Minimum { get; set; }
        int Maximum { get; set; }
        int Value { get; set; }
        string Message { get; set; }

        void SetRange(int minimum, int maximum);
        void SetMessage(string message);
        void SetValue(int value);
        void Increment(int value = 1);
    }
}
