// automatically generated by the FlatBuffers compiler, do not modify

namespace NetworkedPhysics
{

using System;
using FlatBuffers;

public sealed class Vec4 : Table {
  public static Vec4 GetRootAsVec4(ByteBuffer _bb) { return GetRootAsVec4(_bb, new Vec4()); }
  public static Vec4 GetRootAsVec4(ByteBuffer _bb, Vec4 obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public Vec4 __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public float X { get { int o = __offset(4); return o != 0 ? bb.GetFloat(o + bb_pos) : (float)0.0f; } }
  public float Y { get { int o = __offset(6); return o != 0 ? bb.GetFloat(o + bb_pos) : (float)0.0f; } }
  public float Z { get { int o = __offset(8); return o != 0 ? bb.GetFloat(o + bb_pos) : (float)0.0f; } }
  public float W { get { int o = __offset(10); return o != 0 ? bb.GetFloat(o + bb_pos) : (float)0.0f; } }

  public static Offset<Vec4> CreateVec4(FlatBufferBuilder builder,
      float x = 0.0f,
      float y = 0.0f,
      float z = 0.0f,
      float w = 0.0f) {
    builder.StartObject(4);
    Vec4.AddW(builder, w);
    Vec4.AddZ(builder, z);
    Vec4.AddY(builder, y);
    Vec4.AddX(builder, x);
    return Vec4.EndVec4(builder);
  }

  public static void StartVec4(FlatBufferBuilder builder) { builder.StartObject(4); }
  public static void AddX(FlatBufferBuilder builder, float x) { builder.AddFloat(0, x, 0.0f); }
  public static void AddY(FlatBufferBuilder builder, float y) { builder.AddFloat(1, y, 0.0f); }
  public static void AddZ(FlatBufferBuilder builder, float z) { builder.AddFloat(2, z, 0.0f); }
  public static void AddW(FlatBufferBuilder builder, float w) { builder.AddFloat(3, w, 0.0f); }
  public static Offset<Vec4> EndVec4(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<Vec4>(o);
  }
};


}