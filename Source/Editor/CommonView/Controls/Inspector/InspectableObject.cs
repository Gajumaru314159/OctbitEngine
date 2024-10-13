namespace CommonView.Controls
{
    public abstract class InspectableObject : Inspectable
    {
        public abstract string Name { get; }
        public abstract IList<Inspectable> Inspectables { get; }
    }
}
