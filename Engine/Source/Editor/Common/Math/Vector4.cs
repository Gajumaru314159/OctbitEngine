namespace Common.Math
{
    public struct Vector4
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
        public float W { get; set; }

        public static Vector4 Zero => new Vector4(0, 0, 0, 0);
        public static Vector4 One => new Vector4(1, 1, 1, 1);

        public Vector4(float x, float y, float z, float w)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
            this.W = w;
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z,a.W + b.W);
        }

        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }

        public static Vector4 operator *(Vector4 a, float scalar)
        {
            return new Vector4(a.X * scalar, a.Y * scalar, a.Z * scalar,a.W * scalar);
        }

        public static Vector4 operator /(Vector4 a, float scalar)
        {
            return new Vector4(a.X / scalar, a.Y / scalar, a.Z / scalar,a.W / scalar);
        }

        public static bool operator ==(Vector4 a, Vector4 b)
        {
            return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
        }

        public static bool operator !=(Vector4 a, Vector4 b)
        {
            return !(a == b);
        }

        public override bool Equals(object? obj)
        {
            if (obj is Vector4)
            {
                return this == (Vector4)obj;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode() ^ Z.GetHashCode() ^ W.GetHashCode();
        }

        public float Length()
        {
            return (float)System.Math.Sqrt(X * X + Y * Y + Z * Z + W * W);
        }

        public float SqrLength()
        {
            return X * X + Y * Y + Z * Z + W * W;
        }

        public Vector4 Normalize()
        {
            float length = Length();
            if (length > 0)
            {
                return this / length;
            }
            return this;
        }
    }
}
