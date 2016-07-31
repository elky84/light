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

namespace socket_test_tool
{
    public partial class Form_socket_test_tool : Form
    {
        private wise.net.Client client;
        private System.Windows.Forms.Timer timer;
        private int user_index = 0;
        
        public Form_socket_test_tool()
        {
            InitializeComponent();

            textBox_channel_no.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDownHandler_Channel);
            textBox_nickname.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDownHandler_Channel);

            textBox_text.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDownHandler_Send);

            textBox_ip.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDownHandler_Connect);
            textBox_port.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDownHandler_Connect);
        }

        private void OnKeyDownHandler_Send(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (textBox_joined_channel.TextLength != 0)
                {
                    button_broadcast_Click(sender, e);
                }
                else
                {
                    button_send_Click(sender, e);
                }
            }
        }

        private void OnKeyDownHandler_Channel(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if(textBox_joined_channel.TextLength != 0)
                    button_leave_Click(sender, e);
                else
                    button_enter_Click(sender, e);

            }
        }

        private void OnKeyDownHandler_Connect(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                button_connect_Click(sender, e);
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if(timer != null)
            {
                timer.Stop();
                timer = null;
            }

            if(client != null)
            {
                client.Close();
                client = null;
            }

            base.OnFormClosing(e);
        }
        
        private void on_disconnect()
        {
            listBox_log.Items.Add("disconnected server[" + textBox_ip.Text + ":" + textBox_port.Text + "]");
            listBox_users.Items.Clear();
            textBox_nickname.ReadOnly = false;
            textBox_channel_no.ReadOnly = false;
            textBox_joined_channel.Text = "";

        }

        private void button_connect_Click(object sender, EventArgs e)
        {
            if ( client != null )
            {
                client.Close();
            }
            else
            {
                client = new wise.net.Client();
            }

            System.Random r = new System.Random();
            user_index = r.Next(1, 999);

            if ( false == client.Connect(textBox_ip.Text, int.Parse(textBox_port.Text)) )
            {
                log("cannot connect server. server[" + textBox_ip.Text + ":" + textBox_port.Text + "]");
                return;
            }

            // 타이머 생성 및 시작
            timer = new System.Windows.Forms.Timer();
            timer.Interval = 1;
            timer.Tick += new EventHandler(timer_Elapsed);
            timer.Start();

            log("connected server. server[" + textBox_ip.Text + ":" + textBox_port.Text + "]");
        }

        void log(String str)
        {
            listBox_log.Items.Add(str);
        }

        void timer_Elapsed(object sender, EventArgs e)
        {
            if (client == null)
            {
                return;
            }

            wise.net.Client.BufferPacketData data = client.ReadData();
            if(data != null)
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

                            textBox_nickname.ReadOnly = true;
                            textBox_channel_no.ReadOnly = true;

                            listBox_users.Items.Add(enter.USERNAME);
                        }
                        break;
                    case (int)PACKET_S2C.LEAVE:
                        {
                            var leave = PACKET.LEAVE.GetRootAsLEAVE(new FlatBuffers.ByteBuffer(data.BufferData));

                            textBox_joined_channel.Text = "";

                            if(user_index == leave.USERINDEX)
                            {
                                listBox_users.Items.Clear();
                                log(leave.USERNAME + " leave channel.");
                            }
                            else
                            {
                                log(leave.USERNAME + " left channel.");
                                listBox_users.Items.Remove(leave.USERNAME);
                            }

                            textBox_nickname.ReadOnly = false;
                            textBox_channel_no.ReadOnly = false;
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

        private void button_send_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                log("not connected server.");
                return;
            }
            
            byte[] StrByte = Encoding.UTF8.GetBytes(textBox_text.Text);

            if ( false == client.Send(555, StrByte) )
            {
                log("can't send. disconnected server.");
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

        private void button_enter_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                log("not connected server. cannot enter");
                return;
            }

            if (textBox_channel_no.TextLength <= 0)
            {
                log("can't enter. request channel no is empty.");
                return;
            }

            if (textBox_nickname.TextLength <= 0)
            {
                log("can't enter. request textBox_nickname is empty.");
                return;
            }

            FlatBuffers.FlatBufferBuilder builder = new FlatBuffers.FlatBufferBuilder(1);
            var offset = PACKET.ENTER.CreateENTER(builder, builder.CreateString(textBox_nickname.Text), builder.CreateString(""), user_index);
            builder.Finish(offset.Value);
            
            if (false == client.Send((int)PACKET_C2S.ENTER, builder.SizedByteArray(), uint.Parse(textBox_channel_no.Text)))
            {
                log("can't enter. disconnected server.");
            }
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
            var offset = PACKET.LEAVE.CreateLEAVE(builder, builder.CreateString(textBox_nickname.Text), user_index);
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.LEAVE, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't leave. disconnected server.");
            }
        }

        private void button_broadcast_Click(object sender, EventArgs e)
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
            var offset = PACKET.BROADCAST.CreateBROADCAST(builder, builder.CreateString(textBox_nickname.Text), user_index, builder.CreateString(textBox_text.Text));
            builder.Finish(offset.Value);

            if (false == client.Send((int)PACKET_C2S.BROADCAST, builder.SizedByteArray(), uint.Parse(textBox_joined_channel.Text)))
            {
                log("can't send. disconnected server.");
            }
        }
    }
}
