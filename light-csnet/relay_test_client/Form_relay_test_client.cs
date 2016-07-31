using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace relay_test_client
{
    public partial class Form_relay_test_client : Form
    {
        private wise.net.Client client;
        private System.Windows.Forms.Timer packet_timer;

        private int user_index = 0;
        private JObject relay_info;
        private JObject login_info;
        private Dictionary<int, Button> buttons = new Dictionary<int, Button>();
        private Random random = new Random();
        private POSITION position;
        private const int button_width = 50;
        private const int button_height = 50;

        private POSITION destination;

        private long packet_send_time = 0;
        private long before_update_time = 0;
        private double update_time_gap = 0;

        public Form_relay_test_client()
        {
            InitializeComponent();

            // 타이머 생성 및 시작
            packet_timer = new System.Windows.Forms.Timer();
            packet_timer.Interval = 1;
            packet_timer.Tick += new EventHandler(packet_timer_Elapsed);
            packet_timer.Start();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (packet_timer != null)
            {
                packet_timer.Stop();
                packet_timer = null;
            }

            if (client != null)
            {
                client.Close();
                client = null;
            }

            base.OnFormClosing(e);
        }

        private void on_disconnect()
        {
            listBox_log.Items.Add("disconnected server");
            listBox_users.Items.Clear();
            buttons.Clear();
            groupBox_users_location.Controls.Clear();
            textBox_joined_channel.Text = "";
        }

        delegate void logging_Callback(string text);

        private void logging(string text)
        {
            this.listBox_log.Items.Add(text);
        }

        void log(String str)
        {
            if (this.listBox_log.InvokeRequired)
            {
                logging_Callback d = new logging_Callback(logging);
                this.Invoke(d, new object[] { str + " (Invoke)" });
            }
            else
            {
                // It's on the same thread, no need for Invoke
                this.listBox_log.Items.Add(str);
            }
        }
        
        void add_player(PACKET.PLAYER_DATA player)
        {
            Button btn = new Button();
            btn.Text = string.Format("{0}", player.NAME);
            btn.Size = new Size(button_width, button_height);
            btn.Location = new Point(0, 0);
            btn.Visible = false; // 안보이게 우선 만들어~

            //델리게이트를 통해 버튼 이벤트 생성
            //btn.Click += new EventHandler(btn_Click);

            groupBox_users_location.Controls.Add(btn);

            buttons.Add(player.ID, btn);
            listBox_users.Items.Add(player.NAME + "_" + player.ID);
        }

        void packet_timer_Elapsed(object sender, EventArgs e)
        {
            if (before_update_time != 0)
            {
                long nowTime = System.DateTime.Now.Ticks;
                update_time_gap = (nowTime - before_update_time) / 10000.0f;
            }

            before_update_time = System.DateTime.Now.Ticks;

            if (client == null)
            {
                return;
            }

            wise.net.Client.BufferPacketData data = client.ReadData();
            while(data != null)
            {
                switch(data.PacketID)
                {
                    case wise.net.Client.PACKET_ID_DISCONNECTED:
                        { 
                            on_disconnect();
                        }
                        break;
                    case (int)PACKET_S2C.JOIN:
                        {
                            var join = PACKET.RES_JOIN.GetRootAsRES_JOIN(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = data.Index.ToString();

                            PACKET.PLAYER_DATA player = join.DATA;
                            add_player(player);

                            log(player.NAME + " entered channel.");

                            for(int n = 0; n < join.PLAYERDATASLength; ++n)
                            {
                                PACKET.PLAYER_DATA the_other = join.GetPLAYERDATAS(n);
                                add_player(the_other);
                            }
                        }
                        break;
                    case (int)PACKET_S2C.JOINED:
                        {
                            var joined = PACKET.JOINED.GetRootAsJOINED(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = data.Index.ToString();

                            PACKET.PLAYER_DATA player = joined.DATA;
                            add_player(player);

                            log(player.NAME + " entered channel.");
                        }
                        break;
                    case (int)PACKET_S2C.LEAVE:
                        {
                            var leave = PACKET.RES_LEAVE.GetRootAsRES_LEAVE(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = "";

                            if(user_index == leave.USERINDEX)
                            {
                                listBox_users.Items.Clear();
                                buttons.Clear();
                                groupBox_users_location.Controls.Clear();

                                log("i leaved channel.");
                            }
                            else
                            {
                                Button btn = buttons[leave.USERINDEX];
                                log(btn.Text + " left channel.");
                                listBox_users.Items.Remove(btn.Text + "_" + leave.USERINDEX);


                                groupBox_users_location.Controls.Remove(btn);

                                buttons.Remove(leave.USERINDEX);
                            }
                        }
                        break;
                    case (int)PACKET_S2C.SPAWN:
                        {
                            var spawn = PACKET.RES_SPAWN.GetRootAsRES_SPAWN(new FlatBuffers.ByteBuffer(data.BufferData));
                            log("spawn. monster_id : " + spawn.MONSTERID);
                        }
                        break;
                    case (int)PACKET_S2C.READY:
                        {
                            var ready = PACKET.RES_READY.GetRootAsRES_READY(new FlatBuffers.ByteBuffer(data.BufferData));
                            log("ready. game_start : " + ready.GAMESTART);

                            if(ready.GAMESTART)
                            {
                                move();
                            }
                        }
                        break;
                    case (int)PACKET_S2C.SEND:
                        {
                            var send = PACKET.RES_SEND.GetRootAsRES_SEND(new FlatBuffers.ByteBuffer(data.BufferData));
                            try
                            {
                                for(int n = 0; n < send.MESSAGELength; ++n)
                                {
                                    var p = JsonConvert.DeserializeObject<relay_message>(send.GetMESSAGE(n));
                                    Button btn = buttons[p.user_index];
                                    btn.Location = new Point((int)p.position.X, (int)p.position.Y);
                                    btn.Visible = true;
                                }

                                move();
                            }
                            catch (Exception ex)
                            {
                                log("exception: " + ex);
                            }

                        }
                        break;
                }

                data = client.ReadData();
            }
        }

        private void move()
        {
            if(packet_send_time != 0)
            {
                long nowTime = System.DateTime.Now.Ticks;
                double timeGap = (nowTime - packet_send_time) / 10000.0f;

                log("elapsed time : " + timeGap + " update_time_gap : " + update_time_gap);
            }

            position.Force(2, destination);

            if ( Math.Abs(destination.X - position.X) + Math.Abs(destination.Y - position.Y) <= 2)
            {
                new_destination();
            }
            
            send_position();
            packet_send_time = System.DateTime.Now.Ticks;
        }

        private void button_leave_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                log("not connected server. cannot leave");
                return;
            }

            if (textBox_joined_channel.TextLength <= 0)
            {
                log("can't leave. not joined channel.");
                return;
            }

            FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
            PACKET.REQ_LEAVE.StartREQ_LEAVE(builder);
            PACKET.REQ_LEAVE.EndREQ_LEAVE(builder);

            if (false == client.Send((int)PACKET_C2S.LEAVE, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't leave. disconnected server.");
            }
        }

        private void send_position()
        {
            relay_message message = new relay_message();
            message.position = position;
            message.user_index = user_index;

            string jsonString = JsonConvert.SerializeObject(message);
            if( 0 < jsonString.Length)
                send(jsonString);
        }

        private void send(String json)
        {
            if (client == null)
            {
                log("not connected server. cannot broadcast");
                return;
            }

            if (textBox_joined_channel.TextLength <= 0)
            {
                log("can't send. not joined channel.");
                return;
            }

            FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
            var offset = PACKET.REQ_SEND.CreateREQ_SEND(builder, builder.CreateString(json));
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.SEND, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't send. disconnected server.");
            }
        }

        private void button_web_login_Click(object sender, EventArgs e)
        {
            string address = "http://" + textBox_ip.Text + ":" + int.Parse(textBox_port.Text) + "/";
            Http_call.login(address, textBox_user_id.Text, textBox_password.Text, login_callback);
        }

        void new_destination()
        {
            destination = new POSITION { X = random.Next(0, groupBox_users_location.Width - button_width), Y = random.Next(0, groupBox_users_location.Height - button_height) };
        }

        void login_callback(JObject json)
        {
            if (null != json)
            {
                position = new POSITION { X = random.Next(0, groupBox_users_location.Width - button_width), Y = random.Next(0, groupBox_users_location.Height - button_height) };
                new_destination();

                login_info = json;
                user_index = int.Parse(login_info["GUID"].ToString());

                log("connected server. server[" + textBox_ip.Text + ":" + textBox_port.Text + "]");
            }
            else
            {
                log("cannot connect server. server[" + textBox_ip.Text + ":" + textBox_port.Text + "]");
            }
        }

        private void button_relay_start_Click(object sender, EventArgs e)
        {
            Http_call.relay_start(user_index, relay_start_callback);
        }

        void relay_start_callback(JObject json)
        {
            if (null != json)
            {
                relay_info = json;
                string[] address = relay_info["server_address"].ToString().Split(':');

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

                JArray array = JArray.Parse(relay_info["members"].ToString());
                foreach( var member in array)
                {
                    if(member["id"].ToString() == user_index.ToString())
                    {
                        FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
                        var offset = PACKET.REQ_JOIN.CreateREQ_JOIN(builder, builder.CreateString(login_info["UserID"].ToString()), builder.CreateString(member["hash"].ToString()), user_index);
                        builder.Finish(offset.Value);

                        if (false == client.Send((int)PACKET_C2S.JOIN, builder.SizedByteArray(), uint.Parse(relay_info["roomnum"].ToString())))
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

        private void button_close_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                log("not connected server.");
                return;
            }

            client.Close();

            listBox_log.Items.Add("disconnect server.");
        }

        private void button_ready_Click(object sender, EventArgs e)
        {
            FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);

            if (false == client.Send((int)PACKET_C2S.READY, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't send. disconnected server.");
            }
        }

        private void button_spawn_Click(object sender, EventArgs e)
        {
            FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
            var offset = PACKET.REQ_SPAWN.CreateREQ_SPAWN(builder, 1); // 1은 MONSTER_ID
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.SPAWN, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't send. disconnected server.");
            }
        }
    }


    enum PACKET_C2S
    {
        JOIN = 10000,
        LEAVE,
        SPAWN,
        READY,
        SEND,
    }

    enum PACKET_S2C
    {
        JOIN = 20000,
        JOINED,
        LEAVE,
        SPAWN,
        READY,
        SEND,
    }

    class VECTOR
    {
        public float X = 0;
        public float Y = 0;
    }

    class POSITION
    {
        public float X = 0;
        public float Y = 0;

        public VECTOR Normal(POSITION pos)
        {
            POSITION target = new POSITION { X = pos.X - X, Y = pos.Y - Y };
            float Sum = (float)Math.Sqrt((target.X * target.X) + (target.Y * target.Y));
            return Sum != 0.0f ? new VECTOR { X = target.X / Sum, Y = target.Y / Sum } : new VECTOR { X = 0.0f, Y = 0.0f };
        }

        public POSITION Force(float vForce, POSITION rhs)
        {
            return Force(vForce, Normal(rhs));
        }

        public POSITION Force(float force, VECTOR vector)
        {
            X = X + vector.X * force;
            Y = Y + vector.Y * force;
            return this;
        }
    }

    class relay_message
    {
        public int user_index;
        public POSITION position;
    }

}
