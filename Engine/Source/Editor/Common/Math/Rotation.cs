namespace Common.Math
{
    public struct Rotation
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public static Rotation Identity => new Rotation(0, 0, 0);

        public Rotation(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

    }
}
