using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Math
{
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vector3 operator *(Vector3 a, float scalar)
        {
            return new Vector3(a.x * scalar, a.y * scalar, a.z * scalar);
        }

        public static Vector3 operator /(Vector3 a, float scalar)
        {
            return new Vector3(a.x / scalar, a.y / scalar, a.z / scalar);
        }

        public static bool operator ==(Vector3 a, Vector3 b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }

        public static bool operator !=(Vector3 a, Vector3 b)
        {
            return !(a == b);
        }

        public override bool Equals(object? obj)
        {
            if (obj is Vector3)
            {
                return this == (Vector3)obj;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return x.GetHashCode() ^ y.GetHashCode() ^ z.GetHashCode();
        }

        public float Length()
        {
            return (float)System.Math.Sqrt(x * x + y * y + z * z);
        }

        public float SqrLength()
        {
            return x * x + y * y + z * z;
        }

        public Vector3 Normalize()
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
