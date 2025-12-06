using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace Common.Hash
{
    public static class Hash
    {
        public static UInt32 Calc32(ReadOnlySpan<byte> data)
        {
            return FNV32(data);
        }
        public static UInt32 Calc64(ReadOnlySpan<byte> data)
        {
            return FNV32(data);
        }

        public static UInt32 FNV32(ReadOnlySpan<byte> data)
        {
            UInt32 offset_basis = 2166136261u;
            UInt32 fnv_prime = 16777619u;
            UInt32 result = offset_basis;

            foreach (byte b in data)
            {
                result ^= b;
                result *= fnv_prime;
            }

            return result;
        }
        public static UInt64 FNV64(ReadOnlySpan<byte> data)
        {
            UInt64 offset_basis = 14695981039346656037u;
            UInt64 fnv_prime = 1099511628211u;
            UInt64 result = offset_basis;

            foreach (byte b in data)
            {
                result ^= b;
                result *= fnv_prime;
            }
            
            return result;
        }


    }
}
