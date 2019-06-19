using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.Remoting.Channels;
using System.Timers;
using FlatBuffers;
using NetworkedPhysics;
using UnityEngine;
using UnityEngine.UI;

public class Test : MonoBehaviour {

    class GameEntity
    {
        public int Id { get; set; }
        public bool IsDirty { get; set; }
        public bool IsInteracting { get; set; }
        public Vector3 Position { get; set; }
        public Quaternion Rotation { get; set; }
        public Vector3 Scale { get; set; }
        public GameObject GameObject { get; set; }
        public EntityType Type { get; set; }
    }

    public GameObject Cube;
    public GameObject Sphere;
    public Text Text;

    private Dictionary<int, GameEntity> _entities;

    private UdpClient _udpClient;
    private bool disconnected = false;

    private int _totalBytesReceived;
    private int _totalPackets;

    private float elapsed = 0;

    void Start () {
        _udpClient = new UdpClient();
        _entities = new Dictionary<int, GameEntity>();

        IPEndPoint serverEndpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 34001);
        _udpClient.Connect(serverEndpoint);
        _udpClient.BeginReceive(ReceiveCallback, null);
        SendConnectMessage();

        Text.text = "Bytes per second " + _totalBytesReceived;
        Text.text += "\n Packets per second " + _totalPackets;
        Text.text += "\n Bytes per packet " + _totalBytesReceived / Math.Max(_totalPackets, 1);
    }
    
    void Update()
    {
        elapsed += Time.deltaTime;
        if (elapsed > 1f)
        {
            Text.text = "Bytes per second " + _totalBytesReceived;
            Text.text += "\n Packets per second " + _totalPackets;
            Text.text += "\n Bytes per packet " + _totalBytesReceived / Math.Max(_totalPackets, 1);
            _totalBytesReceived = 0;
            _totalPackets = 0;
            elapsed = 0;

            //if (_totalPackets > 0)
            //{
            //    Text.text = "Bytes per second " + _totalBytesReceived;
            //    //_totalBytesReceived = 0;
            //    //_totalPackets = 0;
            //    //elapsed = 0;

            //    Debug.Log("Bytes per second " + _totalBytesReceived);
            //    _totalBytesReceived = 0;
            //    _totalPackets = 0;
            //}
        }

        //Debug.Log("Num Entities " + _entities.Count);
        foreach (var entity in _entities.Values.ToArray())
        {
            if (!entity.IsDirty) continue;

            if (entity.GameObject == null)
            {
                if (entity.Type == EntityType.Cube)
                    entity.GameObject = Instantiate(Cube, transform);
                else if (entity.Type == EntityType.Sphere)
                    entity.GameObject = Instantiate(Sphere, transform);
                else
                    continue;

                entity.GameObject.name = entity.Id.ToString();
            }

            entity.GameObject.transform.position = entity.Position;
            entity.GameObject.transform.rotation = entity.Rotation;
            entity.GameObject.transform.localScale = entity.Scale;

            entity.IsDirty = false;

            if (entity.IsInteracting)
                entity.GameObject.GetComponent<Renderer>().material.color = Color.red;
        }

        if (Input.GetKey(KeyCode.W))
        {
            SendInputMessage(InputType.Forward);
        }
        if (Input.GetKey(KeyCode.S))
        {
            SendInputMessage(InputType.Backward);
        }
        if (Input.GetKey(KeyCode.A))
        {
            SendInputMessage(InputType.Leftward);
        }
        if (Input.GetKey(KeyCode.D))
        {
            SendInputMessage(InputType.Rightward);
        }
        if (Input.GetKeyDown(KeyCode.R))
        {
            SendResetMessage();
        }
    }

    private void SendConnectMessage()
    {
        var builder = new FlatBuffers.FlatBufferBuilder(1024);
        NetworkedPhysics.Message.StartMessage(builder);
        NetworkedPhysics.Message.AddDataType(builder, NetworkedPhysics.MessageType.ClientConnectMessage);
        var message = NetworkedPhysics.Message.EndMessage(builder);
        builder.Finish(message.Value);

        byte[] bytes = builder.SizedByteArray();
        _udpClient.Send(bytes, bytes.Length);
        Debug.Log("Sent Connect message " + bytes.Length);
    }

    private void SendDisconnectMessage()
    {
        var builder = new FlatBuffers.FlatBufferBuilder(1024);
        NetworkedPhysics.Message.StartMessage(builder);
        NetworkedPhysics.Message.AddDataType(builder, NetworkedPhysics.MessageType.ClientDisconnectMessage);
        var message = NetworkedPhysics.Message.EndMessage(builder);
        builder.Finish(message.Value);

        byte[] bytes = builder.SizedByteArray();
        _udpClient.Send(bytes, bytes.Length);
        Debug.Log("Sent Disconnect message " + bytes.Length);
    }

    private void SendInputMessage(InputType type)
    {
        var builder = new FlatBuffers.FlatBufferBuilder(1024);
        
        ClientInputMessage.StartClientInputMessage(builder);
        ClientInputMessage.AddType(builder, type);
        var input = ClientInputMessage.EndClientInputMessage(builder);

        Message.StartMessage(builder);
        Message.AddDataType(builder, MessageType.ClientInputMessage);
        Message.AddData(builder, input.Value);
        var message = Message.EndMessage(builder);
        builder.Finish(message.Value);

        byte[] bytes = builder.SizedByteArray();
        _udpClient.Send(bytes, bytes.Length);
        //Debug.Log("Sent Input message " + bytes.Length);
    }

    private void SendMessageAck(int messageId)
    {
        var builder = new FlatBuffers.FlatBufferBuilder(1024);

        MessageAck.StartMessageAck(builder);
        MessageAck.AddMessageId(builder, messageId);
        var ack = MessageAck.EndMessageAck(builder);

        Message.StartMessage(builder);
        Message.AddDataType(builder, MessageType.MessageAck);
        Message.AddData(builder, ack.Value);
        var message = NetworkedPhysics.Message.EndMessage(builder);
        builder.Finish(message.Value);

        byte[] bytes = builder.SizedByteArray();
        _udpClient.Send(bytes, bytes.Length);
        //Debug.Log("Sent MessageAck message " + messageId + " " + bytes.Length);
    }

    void SendResetMessage()
    {
        var builder = new FlatBufferBuilder(1024);
        Message.StartMessage(builder);
        Message.AddDataType(builder, MessageType.ResetSimulationMessage);
        var message = Message.EndMessage(builder);
        builder.Finish(message.Value);

        byte[] bytes = builder.SizedByteArray();
        _udpClient.Send(bytes, bytes.Length);
        Debug.Log("Sent Reset message " + bytes.Length);
    }

    private void ReceiveCallback(IAsyncResult ar)
    {
        IPEndPoint sender = new IPEndPoint(IPAddress.Loopback, 0);
        var bytes = _udpClient.EndReceive(ar, ref sender);

        //Debug.Log("Received bytes: " + bytes.Length);
        _totalBytesReceived += bytes.Length;
        _totalPackets++;

        ByteBuffer bb = new ByteBuffer(bytes);

        var message = NetworkedPhysics.Message.GetRootAsMessage(bb);

        if (message.DataType != MessageType.ServerSnapshotMessage)
            return;

        var snapshot = new ServerSnapshotMessage();
        message.GetData<ServerSnapshotMessage>(snapshot);

        for (int i = 0; i < snapshot.EntitiesLength; i++)
        {
            var entity = snapshot.GetEntities(i);
            GameEntity gameEntity;
            if (_entities.ContainsKey(entity.Id))
            {
                gameEntity = _entities[entity.Id];
            }
            else
            {
                gameEntity = new GameEntity();
                _entities.Add(entity.Id, gameEntity);
            }

            gameEntity.Id = entity.Id;
            gameEntity.Type = entity.Type;
            gameEntity.IsInteracting = entity.Interacting;
            if (entity.Transform.Position != default(Vec3))
            {
                gameEntity.IsDirty = true;
                gameEntity.Position = new Vector3(entity.Transform.Position.X, entity.Transform.Position.Y,
                    entity.Transform.Position.Z);
            }

            if (entity.Transform.Rotation != default(Vec4))
            {
                gameEntity.IsDirty = true;
                gameEntity.Rotation = new Quaternion(entity.Transform.Rotation.X, entity.Transform.Rotation.Y,
                    entity.Transform.Rotation.Z, entity.Transform.Rotation.W);
            }
            if (entity.Transform.Scale != default(Vec3))
            {
                gameEntity.Scale = new Vector3(entity.Transform.Scale.X, entity.Transform.Scale.Y, entity.Transform.Scale.Z);
                gameEntity.IsDirty = true;
            }

        }

        SendMessageAck(message.Id);

        if (!disconnected)
            _udpClient.BeginReceive(ReceiveCallback, null);
    }

    private void OnDestroy()
    {
        SendDisconnectMessage();
        disconnected = true;
        _udpClient.Client.Disconnect(true);
        _udpClient.Client.Shutdown(SocketShutdown.Both);
        _udpClient.Client.Close();
        //_udpClient.Client.Dispose();
        _udpClient.Close();
    }
}