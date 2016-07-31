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

namespace game_test_client
{
    public partial class Form_game_test_client : Form
    {
        private wise.net.Client client;
        private System.Windows.Forms.Timer packet_timer;
        private System.Windows.Forms.Timer loop_timer;

        private int user_index = 0;
        private JObject relay_info;
        private JObject login_info;
        private Dictionary<int, Button> buttons = new Dictionary<int, Button>();
        private Random random = new Random();
        private POSITION position;
        private const int button_width = 35;
        private const int button_height = 20;

        private POSITION destination;

        public Form_game_test_client()
        {
            InitializeComponent();

            // 타이머 생성 및 시작
            packet_timer = new System.Windows.Forms.Timer();
            packet_timer.Interval = 1;
            packet_timer.Tick += new EventHandler(packet_timer_Elapsed);
            packet_timer.Start();

            // 타이머 생성 및 시작
            loop_timer = new System.Windows.Forms.Timer();
            loop_timer.Interval = 1;
            loop_timer.Tick += new EventHandler(loop_timer_Elapsed);
            loop_timer.Start();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if(loop_timer != null)
            {
                loop_timer.Stop();
                loop_timer = null;
            }

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

        void loop_timer_Elapsed(object sender, EventArgs e)
        {
            if (client == null)
            {
                return;
            }

            if (textBox_joined_channel.TextLength <= 0)
            {
                return;
            }

            move();
        }

        void packet_timer_Elapsed(object sender, EventArgs e)
        {
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
                    case (int)PACKET_S2C.ENTER:
                        {
                            var enter = PACKET.ENTER.GetRootAsENTER(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = data.Index.ToString();

                            if (user_index == enter.USERINDEX)
                            {
                                log(enter.USERNAME + " entered channel.");
                            }
                            else
                            {
                                send_position();
                            }

                            Button btn = new Button();
                            btn.Text = string.Format("{0}", enter.USERINDEX);
                            btn.Size = new Size(button_width, button_height);
                            btn.Location = new Point(0, 0);
                            btn.Visible = false; // 안보이게 우선 만들어~

                            //델리게이트를 통해 버튼 이벤트 생성
                            //btn.Click += new EventHandler(btn_Click);
                            
                            groupBox_users_location.Controls.Add(btn);

                            buttons.Add(enter.USERINDEX, btn);
                            listBox_users.Items.Add(enter.USERNAME + "_" + enter.USERINDEX);
                        }
                        break;
                    case (int)PACKET_S2C.LEAVE:
                        {
                            var leave = PACKET.LEAVE.GetRootAsLEAVE(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = "";

                            if(user_index == leave.USERINDEX)
                            {
                                listBox_users.Items.Clear();
                                buttons.Clear();
                                groupBox_users_location.Controls.Clear();

                                log(leave.USERNAME + " leave channel.");
                            }
                            else
                            {
                                log(leave.USERNAME + " left channel.");
                                listBox_users.Items.Remove(leave.USERNAME + "_" + leave.USERINDEX);

                                Button btn = buttons[leave.USERINDEX];

                                groupBox_users_location.Controls.Remove(btn);

                                buttons.Remove(leave.USERINDEX);
                            }
                        }
                        break;
                    case (int)PACKET_S2C.BROADCAST:
                        {
                            var broadcast = PACKET.BROADCAST.GetRootAsBROADCAST(new FlatBuffers.ByteBuffer(data.BufferData));
                            Button btn = buttons[broadcast.USERINDEX];

                            try
                            {
                                var p = JsonConvert.DeserializeObject<POSITION>(broadcast.MESSAGE);
                                btn.Location = new Point((int)p.X, (int)p.Y);
                                btn.Visible = true;
                            }
                            catch (Exception ex)
                            {
                                log("exception: " + ex + " message:" + broadcast.MESSAGE);
                            }
                        }
                        break;
                }

                data = client.ReadData();
            }
        }

        private void move()
        {
            position.Force(2, destination);

            if ( Math.Abs(destination.X - position.X) + Math.Abs(destination.Y - position.Y) <= 2)
            {
                new_destination();
            }
            
            send_position();
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
            var offset = PACKET.LEAVE.CreateLEAVE(builder, builder.CreateString(login_info["UserID"].ToString()), user_index);
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.LEAVE, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't leave. disconnected server.");
            }
        }

        private void send_position()
        {
            string jsonString = JsonConvert.SerializeObject(position);
            if( 0 < jsonString.Length)
                broadcast(jsonString);
        }

        private void broadcast(String json)
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
            var offset = PACKET.BROADCAST.CreateBROADCAST(builder, builder.CreateString(login_info["UserID"].ToString()), user_index, builder.CreateString(json));
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.BROADCAST, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
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
                        var offset = PACKET.ENTER.CreateENTER(builder, builder.CreateString(login_info["UserID"].ToString()), builder.CreateString(member["hash"].ToString()), user_index);
                        builder.Finish(offset.Value);

                        if (false == client.Send((int)PACKET_C2S.ENTER, builder.SizedByteArray(), uint.Parse(relay_info["roomnum"].ToString())))
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
    }

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

}
