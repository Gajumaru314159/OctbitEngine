#nullable enable
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OctbitEngine.API;

namespace OctbitEngine.Core
{
    internal class TextureData : ITextureData
    {
        public int Width { get; }
        public int Height { get; }

        public string ImportedPath => throw new NotImplementedException();

        public void Import(string path)
        {
            throw new NotImplementedException();
        }
    }

}
