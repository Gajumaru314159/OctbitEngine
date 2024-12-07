namespace OctbitEngine.Runtime
{
    public class SetValueQuery : Query
    {
        public int RemoteId;
        public string PropertyName;
        public object Value;

        public void Serialize(BinaryWriter writer)
        {

        }
    }




}
