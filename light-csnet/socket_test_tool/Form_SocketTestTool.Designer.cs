namespace socket_test_tool
{
    partial class Form_socket_test_tool
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.listBox_log = new System.Windows.Forms.ListBox();
            this.textBox_ip = new System.Windows.Forms.TextBox();
            this.button_connect = new System.Windows.Forms.Button();
            this.button_send = new System.Windows.Forms.Button();
            this.button_close = new System.Windows.Forms.Button();
            this.textBox_text = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_port = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.button_enter = new System.Windows.Forms.Button();
            this.button_leave = new System.Windows.Forms.Button();
            this.button_broadcast = new System.Windows.Forms.Button();
            this.listBox_users = new System.Windows.Forms.ListBox();
            this.textBox_channel_no = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_joined_channel = new System.Windows.Forms.TextBox();
            this.textBox_nickname = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // listBox_log
            // 
            this.listBox_log.FormattingEnabled = true;
            this.listBox_log.ItemHeight = 12;
            this.listBox_log.Location = new System.Drawing.Point(12, 38);
            this.listBox_log.Name = "listBox_log";
            this.listBox_log.Size = new System.Drawing.Size(627, 316);
            this.listBox_log.TabIndex = 0;
            // 
            // textBox_ip
            // 
            this.textBox_ip.Location = new System.Drawing.Point(33, 7);
            this.textBox_ip.Name = "textBox_ip";
            this.textBox_ip.Size = new System.Drawing.Size(170, 21);
            this.textBox_ip.TabIndex = 1;
            this.textBox_ip.Text = "127.0.0.1";
            // 
            // button_connect
            // 
            this.button_connect.Location = new System.Drawing.Point(314, 9);
            this.button_connect.Name = "button_connect";
            this.button_connect.Size = new System.Drawing.Size(75, 23);
            this.button_connect.TabIndex = 2;
            this.button_connect.Text = "connect";
            this.button_connect.UseVisualStyleBackColor = true;
            this.button_connect.Click += new System.EventHandler(this.button_connect_Click);
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(665, 9);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(75, 23);
            this.button_send.TabIndex = 3;
            this.button_send.Text = "send";
            this.button_send.UseVisualStyleBackColor = true;
            this.button_send.Click += new System.EventHandler(this.button_send_Click);
            // 
            // button_close
            // 
            this.button_close.Location = new System.Drawing.Point(746, 9);
            this.button_close.Name = "button_close";
            this.button_close.Size = new System.Drawing.Size(75, 23);
            this.button_close.TabIndex = 4;
            this.button_close.Text = "close";
            this.button_close.UseVisualStyleBackColor = true;
            this.button_close.Click += new System.EventHandler(this.button_close_Click);
            // 
            // textBox_text
            // 
            this.textBox_text.Location = new System.Drawing.Point(43, 360);
            this.textBox_text.Name = "textBox_text";
            this.textBox_text.Size = new System.Drawing.Size(697, 21);
            this.textBox_text.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(15, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "ip";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(211, 12);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "port";
            // 
            // textBox_port
            // 
            this.textBox_port.Location = new System.Drawing.Point(243, 7);
            this.textBox_port.MaxLength = 5;
            this.textBox_port.Name = "textBox_port";
            this.textBox_port.Size = new System.Drawing.Size(65, 21);
            this.textBox_port.TabIndex = 7;
            this.textBox_port.Text = "1997";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 365);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 12);
            this.label3.TabIndex = 9;
            this.label3.Text = "text";
            // 
            // button_enter
            // 
            this.button_enter.Location = new System.Drawing.Point(269, 385);
            this.button_enter.Name = "button_enter";
            this.button_enter.Size = new System.Drawing.Size(75, 23);
            this.button_enter.TabIndex = 10;
            this.button_enter.Text = "enter";
            this.button_enter.UseVisualStyleBackColor = true;
            this.button_enter.Click += new System.EventHandler(this.button_enter_Click);
            // 
            // button_leave
            // 
            this.button_leave.Location = new System.Drawing.Point(350, 385);
            this.button_leave.Name = "button_leave";
            this.button_leave.Size = new System.Drawing.Size(75, 23);
            this.button_leave.TabIndex = 11;
            this.button_leave.Text = "leave";
            this.button_leave.UseVisualStyleBackColor = true;
            this.button_leave.Click += new System.EventHandler(this.button_leave_Click);
            // 
            // button_broadcast
            // 
            this.button_broadcast.Location = new System.Drawing.Point(746, 360);
            this.button_broadcast.Name = "button_broadcast";
            this.button_broadcast.Size = new System.Drawing.Size(75, 23);
            this.button_broadcast.TabIndex = 12;
            this.button_broadcast.Text = "broadcast";
            this.button_broadcast.UseVisualStyleBackColor = true;
            this.button_broadcast.Click += new System.EventHandler(this.button_broadcast_Click);
            // 
            // listBox_users
            // 
            this.listBox_users.FormattingEnabled = true;
            this.listBox_users.ItemHeight = 12;
            this.listBox_users.Location = new System.Drawing.Point(645, 38);
            this.listBox_users.Name = "listBox_users";
            this.listBox_users.Size = new System.Drawing.Size(177, 316);
            this.listBox_users.TabIndex = 13;
            // 
            // textBox_channel_no
            // 
            this.textBox_channel_no.Location = new System.Drawing.Point(198, 387);
            this.textBox_channel_no.MaxLength = 5;
            this.textBox_channel_no.Name = "textBox_channel_no";
            this.textBox_channel_no.Size = new System.Drawing.Size(65, 21);
            this.textBox_channel_no.TabIndex = 14;
            this.textBox_channel_no.Text = "123";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(431, 390);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 12);
            this.label4.TabIndex = 15;
            this.label4.Text = "joined";
            // 
            // textBox_joined_channel
            // 
            this.textBox_joined_channel.Location = new System.Drawing.Point(476, 387);
            this.textBox_joined_channel.MaxLength = 5;
            this.textBox_joined_channel.Name = "textBox_joined_channel";
            this.textBox_joined_channel.ReadOnly = true;
            this.textBox_joined_channel.Size = new System.Drawing.Size(65, 21);
            this.textBox_joined_channel.TabIndex = 16;
            // 
            // textBox_nickname
            // 
            this.textBox_nickname.Location = new System.Drawing.Point(79, 387);
            this.textBox_nickname.MaxLength = 5;
            this.textBox_nickname.Name = "textBox_nickname";
            this.textBox_nickname.Size = new System.Drawing.Size(65, 21);
            this.textBox_nickname.TabIndex = 17;
            this.textBox_nickname.Text = "test";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(142, 390);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(50, 12);
            this.label5.TabIndex = 18;
            this.label5.Text = "channel";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 390);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(60, 12);
            this.label6.TabIndex = 19;
            this.label6.Text = "nickname";
            // 
            // Form_socket_test_tool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(834, 416);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_nickname);
            this.Controls.Add(this.textBox_joined_channel);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBox_channel_no);
            this.Controls.Add(this.listBox_users);
            this.Controls.Add(this.button_broadcast);
            this.Controls.Add(this.button_leave);
            this.Controls.Add(this.button_enter);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_port);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_text);
            this.Controls.Add(this.button_close);
            this.Controls.Add(this.button_send);
            this.Controls.Add(this.button_connect);
            this.Controls.Add(this.textBox_ip);
            this.Controls.Add(this.listBox_log);
            this.Name = "Form_socket_test_tool";
            this.Text = "socket_test_tool";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBox_log;
        private System.Windows.Forms.TextBox textBox_ip;
        private System.Windows.Forms.Button button_connect;
        private System.Windows.Forms.Button button_send;
        private System.Windows.Forms.Button button_close;
        private System.Windows.Forms.TextBox textBox_text;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_port;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button_enter;
        private System.Windows.Forms.Button button_leave;
        private System.Windows.Forms.Button button_broadcast;
        private System.Windows.Forms.ListBox listBox_users;
        private System.Windows.Forms.TextBox textBox_channel_no;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_joined_channel;
        private System.Windows.Forms.TextBox textBox_nickname;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
    }
}

