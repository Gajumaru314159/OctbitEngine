namespace Common.Math
{
    public struct IntVec2
    {
        public int X { get; set; }
        public int Y { get; set; }

        public static IntVec2 Zero => new IntVec2(0, 0);
        public static IntVec2 One => new IntVec2(1, 1);

        public IntVec2(int x, int y)
        {
            this.X = x;
            this.Y = y;
        }

        public static IntVec2 operator +(IntVec2 a, IntVec2 b)
        {
            return new IntVec2(a.X + b.X, a.Y + b.Y);
        }

        public static IntVec2 operator -(IntVec2 a, IntVec2 b)
        {
            return new IntVec2(a.X - b.X, a.Y - b.Y);
        }

        public static IntVec2 operator *(IntVec2 a, int scalar)
        {
            return new IntVec2(a.X * scalar, a.Y * scalar);
        }

        public static IntVec2 operator /(IntVec2 a, float scalar)
        {
            return new IntVec2((int)(a.X / scalar), (int)(a.Y / scalar));
        }

        public static bool operator ==(IntVec2 a, IntVec2 b)
        {
            return a.X == b.X && a.Y == b.Y;
        }

        public static bool operator !=(IntVec2 a, IntVec2 b)
        {
            return !(a == b);
        }

        public override bool Equals(object? obj)
        {
            if (obj is IntVec2)
            {
                return this == (IntVec2)obj;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode();
        }

        public float Length()
        {
            return (int)System.Math.Sqrt(X * X + Y * Y);
        }

        public int SqrLength()
        {
            return X * X + Y * Y;
        }

    }

    public struct IntVec3
    {
        public int X { get; set; }
        public int Y { get; set; }
        public int Z { get; set; }
    }

    public struct IntVec4
    {
        public int X { get; set; }
        public int Y { get; set; }
        public int Z { get; set; }
        public int W { get; set; }
    }
}
