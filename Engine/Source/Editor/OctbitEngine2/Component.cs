using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine2
{
    public class Component
    {
        public Entity Entity { get; internal init; }


        public virtual void Reset() { }
        public virtual void Start() { }
        public virtual void FixedUpdate() { }
        public virtual void Update() { }
    }
}
