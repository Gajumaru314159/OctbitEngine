namespace OctbitEngine.Asset
{
    public interface ISerializable
    {
        void Serialize(BinaryWriter writer);
        void Deserialize(BinaryReader reader);
    }
}
