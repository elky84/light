using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

enum PACKET_C2S
{
    ENTER = 10000,
    LEAVE = 10001,
    BROADCAST = 10002,
}

enum PACKET_S2C
{
    ENTER = 20000,
    LEAVE = 20001,
    BROADCAST = 20002,
}

public class Test_Socket : MonoBehaviour
{
    private wise.net.Client client;
    private int user_index = 0;

    private UILabel label_ip;
    private UILabel label_port;
    private UILabel label_input;

    private UILabel label_nickname;
    private UILabel label_joined_channel;
    private UILabel label_channel_no;

    public void Start()
    {
        label_ip = transform.FindChild("Object/Socket/IP/Label").GetComponent<UILabel>();
        label_port = transform.FindChild("Object/Socket/Port/Label").GetComponent<UILabel>();

        label_input = transform.FindChild("Object/Input/Label").GetComponent<UILabel>();

        label_nickname = transform.FindChild("Object/Channel/Nickname/Label").GetComponent<UILabel>();
        label_channel_no = transform.FindChild("Object/Channel/No/Label").GetComponent<UILabel>();

        label_joined_channel = transform.FindChild("Object/Channel/Joined/Label").GetComponent<UILabel>();

        EventDelegate.Add(transform.FindChild("Object/Socket/IP").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));
        EventDelegate.Add(transform.FindChild("Object/Socket/Port").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));

        EventDelegate.Add(transform.FindChild("Object/Input").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_send"));

        EventDelegate.Add(transform.FindChild("Object/Channel/Nickname").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_channel"));
        EventDelegate.Add(transform.FindChild("Object/Channel/No").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_channel"));

        EventDelegate.Add(transform.FindChild("Object/Socket/Send/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_send_Click"));
        EventDelegate.Add(transform.FindChild("Object/Socket/Close/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_close_Click"));
        EventDelegate.Add(transform.FindChild("Object/Socket/Connect/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_connect_Click"));

        EventDelegate.Add(transform.FindChild("Object/Channel/Enter/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_enter_Click"));
        EventDelegate.Add(transform.FindChild("Object/Channel/Leave/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_leave_Click"));
        EventDelegate.Add(transform.FindChild("Object/Channel/Broadcast/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_broadcast_Click"));
    }

    void on_input_submit_channel()
    {
        if (label_joined_channel.text != "")
        {
            button_leave_Click();
        }
        else
        {
            button_enter_Click();
        }
    }

    void on_input_submit_connect()
    {
        button_connect_Click();
    }

    void on_input_submit_send()
    {
        if(label_joined_channel.text != "")
        {
            button_broadcast_Click();
        }
        else
        {
            button_send_Click();
        }
    }

    void on_disconnect()
    {
        log("disconnected server.");
        clear_users();
    }

    void Update()
    {
        if (client == null)
        {
            return;
        }

        wise.net.Client.BufferPacketData data = client.ReadData();
        if (data != null)
        {
            switch (data.PacketID)
            {
                case wise.net.Client.PACKET_ID_DISCONNECTED:
                    {
                        on_disconnect();
                    }
                    break;
                case (int)PACKET_S2C.ENTER:
                    {
                        var enter = PACKET.ENTER.GetRootAsENTER(new FlatBuffers.ByteBuffer(data.BufferData));

                        label_joined_channel.text = data.Index.ToString();

                        if (user_index == enter.USERINDEX)
                        {
                            log(enter.USERNAME + " entered channel.");
                        }

                        add_user(enter.USERNAME);
                    }
                    break;
                case (int)PACKET_S2C.LEAVE:
                    {
                        var leave = PACKET.LEAVE.GetRootAsLEAVE(new FlatBuffers.ByteBuffer(data.BufferData));

                        label_joined_channel.text = "";

                        if (user_index == leave.USERINDEX)
                        {
                            clear_users();
                            log(leave.USERNAME + " leave channel.");
                        }
                        else
                        {
                            log(leave.USERNAME + " left channel.");
                            remove_user(leave.USERNAME);
                        }
                    }
                    break;
                case (int)PACKET_S2C.BROADCAST:
                    {
                        var broadcast = PACKET.BROADCAST.GetRootAsBROADCAST(new FlatBuffers.ByteBuffer(data.BufferData));
                        log("[" + broadcast.USERNAME + "] " + broadcast.MESSAGE);
                    }
                    break;
            }
        }
    }

    public void OnDestroy()
    {
        if (client != null)
        {
            client.Close();
            client = null;
        }
    }

    public void button_connect_Click()
    {
        if (client != null)
        {
            client.Close();
        }
        else
        {
            client = new wise.net.Client();
        }

        System.Random r = new System.Random();
        user_index = r.Next(1, 999);

        if (false == client.Connect(label_ip.text, int.Parse(label_port.text)))
        {
            log("cannot connect server. server[" + label_ip.text + ":" + label_port.text + "]");
            return;
        }

        log("connected server. server[" + label_ip.text + ":" + label_port.text + "]");
    }
    
    void log(string log)
    {
        Transform table = transform.FindChild("Object/Log/ScrollView/Table");
        GameObject new_object = Instantiate(Resources.Load<GameObject>("Prefabs/Log_Label"), Vector3.zero, Quaternion.identity) as GameObject;
        new_object.name = new_object.name.Replace("(Clone)", "");
        new_object.transform.parent = table;
        new_object.transform.localPosition = Vector3.zero;
        new_object.transform.localScale = Vector3.one;
        new_object.transform.rotation = Quaternion.identity;

        new_object.GetComponent<UILabel>().text = log;
        table.GetComponent<UITable>().Reposition();
    }

    void add_user(string name)
    {
        Transform table = transform.FindChild("Object/Users/ScrollView/Table");
        GameObject new_object = Instantiate(Resources.Load<GameObject>("Prefabs/User_Label"), Vector3.zero, Quaternion.identity) as GameObject;
        new_object.name = new_object.name.Replace("(Clone)", "");
        new_object.transform.parent = table;
        new_object.transform.localPosition = Vector3.zero;
        new_object.transform.localScale = Vector3.one;
        new_object.transform.rotation = Quaternion.identity;

        new_object.GetComponent<UILabel>().text = name;
        table.GetComponent<UITable>().Reposition();
    }

    void remove_user(string name)
    {
        Transform table = transform.FindChild("Object/Users/ScrollView/Table");
        for (int i = 0; i < table.childCount; ++i)
        {
            Transform child = table.GetChild(i);
            if (child.name == name)
            {
                child.parent = null;
                Destroy(child.gameObject);
            }
        }

        table.GetComponent<UITable>().Reposition();
    }

    void clear_users()
    {
        Transform table = transform.FindChild("Object/Users/ScrollView/Table");

        List<Transform> list_transform = new List<Transform>();
        for (int i = 0; i < table.childCount; ++i)
        {
            list_transform.Add(table.GetChild(i));
        }

        foreach (Transform child_trans in list_transform)
        {
            child_trans.parent = null;
            Destroy(child_trans.gameObject);
        }
        
        table.GetComponent<UITable>().Reposition();
    }


    public void button_send_Click()
    {
        if (client == null)
        {
            log("not connected server.");
            return;
        }

        byte[] StrByte = Encoding.UTF8.GetBytes(label_input.text);
        client.Send(555, StrByte);
    }

    public void button_close_Click()
    {
        if (client == null)
        {
            log("not connected server.");
            return;
        }

        client.Close();

        log("disconnect server.");
    }

    public void button_enter_Click()
    {
        if(client == null)
        {
            log("not connected server. cannot enter");
            return;
        }

        if (label_channel_no.text.Length <= 0)
        {
            log("can't enter. request channel no is empty.");
            return;
        }

        if (label_nickname.text.Length <= 0)
        {
            log("can't enter. request label_nickname is empty.");
            return;
        }

        FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
        var offset = PACKET.ENTER.CreateENTER(builder, builder.CreateString(label_nickname.text), builder.CreateString(""), user_index);
        builder.Finish(offset.Value);

        if (false == client.Send((int)PACKET_C2S.ENTER, builder.SizedByteArray(), uint.Parse(label_channel_no.text)))
        {
            log("can't enter. disconnected server.");
        }
    }

    public void button_leave_Click()
    {
        if (client == null)
        {
            log("not connected server. cannot leave.");
            return;
        }

        if (label_joined_channel.text.Length <= 0)
        {
            log("can't leave. not joined channel.");
            return;
        }

        FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
        var offset = PACKET.LEAVE.CreateLEAVE(builder, builder.CreateString(label_nickname.text), user_index);
        builder.Finish(offset.Value);

        if (false == client.Send((int)PACKET_C2S.LEAVE, builder.SizedByteArray(), uint.Parse(label_joined_channel.text)))
        {
            log("can't leave. disconnected server.");
        }
    }

    public void button_broadcast_Click()
    {
        if (client == null)
        {
            log("not connected server. cannot broadcast.");
            return;
        }

        if (label_joined_channel.text.Length <= 0)
        {
            log("can't send. not joined channel.");
            return;
        }

        FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
        var offset = PACKET.BROADCAST.CreateBROADCAST(builder, builder.CreateString(label_nickname.text), user_index, builder.CreateString(label_input.text));
        builder.Finish(offset.Value);

        if (false == client.Send((int)PACKET_C2S.BROADCAST, builder.SizedByteArray(), uint.Parse(label_joined_channel.text)))
        {
            log("can't send. disconnected server.");
        }
    }
}