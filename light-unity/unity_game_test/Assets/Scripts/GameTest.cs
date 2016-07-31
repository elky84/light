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

public class GameTest : MonoBehaviour
{
    private wise.net.Client client;
    private int user_index = 0;

    private UILabel label_ip;
    private UILabel label_port;

    private UILabel label_user_id;
    private UILabel label_password;

    private UILabel label_joined_channel;

    private Login.Response login_info;
    private RelayStart.Response relay_info;

    private float time = 0;
    private Int32 count = 0;

    public void Start()
    {
        label_ip = transform.FindChild("Object/Socket/IP/Label").GetComponent<UILabel>();
        label_port = transform.FindChild("Object/Socket/Port/Label").GetComponent<UILabel>();

        label_user_id = transform.FindChild("Object/Socket/UserID/Label").GetComponent<UILabel>();
        label_password = transform.FindChild("Object/Socket/Password/Label").GetComponent<UILabel>();

        label_joined_channel = transform.FindChild("Object/Channel/Joined/Label").GetComponent<UILabel>();

        EventDelegate.Add(transform.FindChild("Object/Socket/IP").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));
        EventDelegate.Add(transform.FindChild("Object/Socket/Port").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));
        EventDelegate.Add(transform.FindChild("Object/Socket/UserID").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));
        EventDelegate.Add(transform.FindChild("Object/Socket/Password").GetComponent<UIInput>().onSubmit, new EventDelegate(this, "on_input_submit_connect"));

        EventDelegate.Add(transform.FindChild("Object/Socket/WebLogin/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_web_login_Click"));
        EventDelegate.Add(transform.FindChild("Object/Socket/RelayStart/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_relay_start_Click"));

        EventDelegate.Add(transform.FindChild("Object/Socket/Close/Button").GetComponent<UIButton>().onClick, new EventDelegate(this, "button_close_Click"));
    }
    
    void on_input_submit_connect()
    {
        button_web_login_Click();
    }


    void button_web_login_Click()
    {
        ApiClient.instance.set_base_host("http://" + label_ip.text + ":" + label_port.text);
        Login pkt = new Login(label_user_id.text, label_password.text, "", "", "");
        ApiClient.instance.SendPacket(pkt, login_callback, true);
    }

    void login_callback(object req, ApiClient client)
    {
        if (null != req)
        {
            Login login = (Login)req;
            login_info = login.response;
            user_index = (int)login_info.GUID;

            log("connected server. server[" + label_ip.text + ":" + label_port.text + "]");
        }
        else
        {
            log("cannot connect server. server[" + label_ip.text + ":" + label_port.text + "]");
        }
    }

    private void button_relay_start_Click()
    {
        RelayStart pkt = new RelayStart(user_index);
        ApiClient.instance.SendPacket(pkt, relay_start_callback, true);
    }

    void relay_start_callback(object req, ApiClient api_client)
    {
        if (null != req)
        {
            RelayStart relay_start = (RelayStart)req;

            relay_info = relay_start.response;

            string[] address = relay_info.server_address.Split(':');

            if (client != null)
            {
                if (true == client.IsConnected())
                {
                    log("already connected server. server[" + address[0] + ":" + address[1] + "]");
                    return;
                }
            }
            else
            {
                client = new wise.net.Client();
            }

            if (false == client.Connect(address[0], int.Parse(address[1])))
            {
                log("cannot connect server. server[" + address[0] + ":" + address[1] + "]");
                return;
            }

            log("connect server. server[" + address[0] + ":" + address[1] + "]");

            foreach (var member in relay_info.members)
            {
                if (member.id == user_index.ToString())
                {
                    FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
                    var offset = PACKET.ENTER.CreateENTER(builder, builder.CreateString(login_info.UserID), builder.CreateString(member.hash), user_index);
                    builder.Finish(offset.Value);

                    if (false == client.Send((int)PACKET_C2S.ENTER, builder.SizedByteArray(), uint.Parse(relay_info.roomnum.ToString())))
                    {
                        log("can't enter.");
                    }
                    return;
                }
            }

            log("not find my information from relay/start.");
        }
        else
        {
            log("failed relay_start.");
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
                            send_broadcast();
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

                        send_broadcast();
                    }
                    break;
            }
        }
    }

    void send_broadcast()
    {
        if(time != 0.0f)
        {
            log("elapsed time : " + time + " count : " + count);
        }

        FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);

        var offset = PACKET.BROADCAST.CreateBROADCAST(builder, builder.CreateString(login_info.UserID), user_index, builder.CreateString(count.ToString()));
        builder.Finish(offset.Value);

        client.Send((int)PACKET_C2S.BROADCAST, builder.SizedByteArray(), uint.Parse(relay_info.roomnum.ToString()));

        count++;
        time = Time.deltaTime;
    }

    public void OnDestroy()
    {
        if (client != null)
        {
            client.Close();
            client = null;
        }
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
}