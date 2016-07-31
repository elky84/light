namespace relay_test_client
{
    partial class Form_relay_test_client
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_port = new System.Windows.Forms.TextBox();
            this.listBox_users = new System.Windows.Forms.ListBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_joined_channel = new System.Windows.Forms.TextBox();
            this.button_web_login = new System.Windows.Forms.Button();
            this.button_relay_start = new System.Windows.Forms.Button();
            this.groupBox_users_location = new System.Windows.Forms.GroupBox();
            this.label_position = new System.Windows.Forms.Label();
            this.textBox_connection_state = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_password = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_user_id = new System.Windows.Forms.TextBox();
            this.button_close = new System.Windows.Forms.Button();
            this.button_leave = new System.Windows.Forms.Button();
            this.button_ready = new System.Windows.Forms.Button();
            this.button_spawn = new System.Windows.Forms.Button();
            this.groupBox_users_location.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBox_log
            // 
            this.listBox_log.FormattingEnabled = true;
            this.listBox_log.ItemHeight = 12;
            this.listBox_log.Location = new System.Drawing.Point(12, 50);
            this.listBox_log.Name = "listBox_log";
            this.listBox_log.Size = new System.Drawing.Size(643, 304);
            this.listBox_log.TabIndex = 0;
            // 
            // textBox_ip
            // 
            this.textBox_ip.Location = new System.Drawing.Point(36, 5);
            this.textBox_ip.Name = "textBox_ip";
            this.textBox_ip.Size = new System.Drawing.Size(170, 21);
            this.textBox_ip.TabIndex = 1;
            this.textBox_ip.Text = "127.0.0.1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(15, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "ip";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(214, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(26, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "port";
            // 
            // textBox_port
            // 
            this.textBox_port.Location = new System.Drawing.Point(246, 5);
            this.textBox_port.MaxLength = 5;
            this.textBox_port.Name = "textBox_port";
            this.textBox_port.Size = new System.Drawing.Size(65, 21);
            this.textBox_port.TabIndex = 7;
            this.textBox_port.Text = "3100";
            // 
            // listBox_users
            // 
            this.listBox_users.FormattingEnabled = true;
            this.listBox_users.ItemHeight = 12;
            this.listBox_users.Location = new System.Drawing.Point(661, 50);
            this.listBox_users.Name = "listBox_users";
            this.listBox_users.Size = new System.Drawing.Size(186, 304);
            this.listBox_users.TabIndex = 13;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(661, 29);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(90, 12);
            this.label4.TabIndex = 15;
            this.label4.Text = "joined_channel";
            // 
            // textBox_joined_channel
            // 
            this.textBox_joined_channel.Location = new System.Drawing.Point(757, 26);
            this.textBox_joined_channel.MaxLength = 5;
            this.textBox_joined_channel.Name = "textBox_joined_channel";
            this.textBox_joined_channel.ReadOnly = true;
            this.textBox_joined_channel.Size = new System.Drawing.Size(84, 21);
            this.textBox_joined_channel.TabIndex = 16;
            // 
            // button_web_login
            // 
            this.button_web_login.Location = new System.Drawing.Point(317, 3);
            this.button_web_login.Name = "button_web_login";
            this.button_web_login.Size = new System.Drawing.Size(75, 23);
            this.button_web_login.TabIndex = 20;
            this.button_web_login.Text = "web_login";
            this.button_web_login.UseVisualStyleBackColor = true;
            this.button_web_login.Click += new System.EventHandler(this.button_web_login_Click);
            // 
            // button_relay_start
            // 
            this.button_relay_start.Location = new System.Drawing.Point(398, 4);
            this.button_relay_start.Name = "button_relay_start";
            this.button_relay_start.Size = new System.Drawing.Size(75, 21);
            this.button_relay_start.TabIndex = 21;
            this.button_relay_start.Text = "web_relay_start";
            this.button_relay_start.UseVisualStyleBackColor = true;
            this.button_relay_start.Click += new System.EventHandler(this.button_relay_start_Click);
            // 
            // groupBox_users_location
            // 
            this.groupBox_users_location.Controls.Add(this.label_position);
            this.groupBox_users_location.Location = new System.Drawing.Point(3, 361);
            this.groupBox_users_location.Name = "groupBox_users_location";
            this.groupBox_users_location.Size = new System.Drawing.Size(844, 304);
            this.groupBox_users_location.TabIndex = 22;
            this.groupBox_users_location.TabStop = false;
            this.groupBox_users_location.Text = "users location";
            // 
            // label_position
            // 
            this.label_position.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label_position.AutoSize = true;
            this.label_position.Location = new System.Drawing.Point(784, 0);
            this.label_position.Name = "label_position";
            this.label_position.Size = new System.Drawing.Size(51, 12);
            this.label_position.TabIndex = 0;
            this.label_position.Text = "x: 0, y:0";
            this.label_position.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // textBox_connection_state
            // 
            this.textBox_connection_state.Location = new System.Drawing.Point(593, 5);
            this.textBox_connection_state.MaxLength = 5;
            this.textBox_connection_state.Name = "textBox_connection_state";
            this.textBox_connection_state.ReadOnly = true;
            this.textBox_connection_state.Size = new System.Drawing.Size(78, 21);
            this.textBox_connection_state.TabIndex = 24;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(487, 10);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(100, 12);
            this.label3.TabIndex = 23;
            this.label3.Text = "connection_state";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(244, 31);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(61, 12);
            this.label5.TabIndex = 28;
            this.label5.Text = "password";
            // 
            // textBox_password
            // 
            this.textBox_password.Location = new System.Drawing.Point(311, 28);
            this.textBox_password.MaxLength = 5;
            this.textBox_password.Name = "textBox_password";
            this.textBox_password.Size = new System.Drawing.Size(65, 21);
            this.textBox_password.TabIndex = 27;
            this.textBox_password.Text = "123456";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 31);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(46, 12);
            this.label6.TabIndex = 26;
            this.label6.Text = "user_id";
            // 
            // textBox_user_id
            // 
            this.textBox_user_id.Location = new System.Drawing.Point(66, 26);
            this.textBox_user_id.Name = "textBox_user_id";
            this.textBox_user_id.Size = new System.Drawing.Size(170, 21);
            this.textBox_user_id.TabIndex = 25;
            this.textBox_user_id.Text = "elky";
            // 
            // button_close
            // 
            this.button_close.Location = new System.Drawing.Point(677, 5);
            this.button_close.Name = "button_close";
            this.button_close.Size = new System.Drawing.Size(75, 21);
            this.button_close.TabIndex = 29;
            this.button_close.Text = "close";
            this.button_close.UseVisualStyleBackColor = true;
            this.button_close.Click += new System.EventHandler(this.button_close_Click);
            // 
            // button_leave
            // 
            this.button_leave.Location = new System.Drawing.Point(757, 3);
            this.button_leave.Name = "button_leave";
            this.button_leave.Size = new System.Drawing.Size(75, 21);
            this.button_leave.TabIndex = 30;
            this.button_leave.Text = "leave";
            this.button_leave.UseVisualStyleBackColor = true;
            this.button_leave.Click += new System.EventHandler(this.button_leave_Click);
            // 
            // button_ready
            // 
            this.button_ready.Location = new System.Drawing.Point(382, 27);
            this.button_ready.Name = "button_ready";
            this.button_ready.Size = new System.Drawing.Size(75, 21);
            this.button_ready.TabIndex = 31;
            this.button_ready.Text = "ready";
            this.button_ready.UseVisualStyleBackColor = true;
            this.button_ready.Click += new System.EventHandler(this.button_ready_Click);
            // 
            // button_spawn
            // 
            this.button_spawn.Location = new System.Drawing.Point(463, 27);
            this.button_spawn.Name = "button_spawn";
            this.button_spawn.Size = new System.Drawing.Size(75, 21);
            this.button_spawn.TabIndex = 32;
            this.button_spawn.Text = "spawn";
            this.button_spawn.UseVisualStyleBackColor = true;
            this.button_spawn.Click += new System.EventHandler(this.button_spawn_Click);
            // 
            // Form_relay_test_client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(853, 663);
            this.Controls.Add(this.button_spawn);
            this.Controls.Add(this.button_ready);
            this.Controls.Add(this.button_leave);
            this.Controls.Add(this.button_close);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_password);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.textBox_user_id);
            this.Controls.Add(this.textBox_connection_state);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.groupBox_users_location);
            this.Controls.Add(this.button_relay_start);
            this.Controls.Add(this.button_web_login);
            this.Controls.Add(this.textBox_joined_channel);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.listBox_users);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_port);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_ip);
            this.Controls.Add(this.listBox_log);
            this.Name = "Form_relay_test_client";
            this.Text = "relay_test_client";
            this.groupBox_users_location.ResumeLayout(false);
            this.groupBox_users_location.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBox_log;
        private System.Windows.Forms.TextBox textBox_ip;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_port;
        private System.Windows.Forms.ListBox listBox_users;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_joined_channel;
        private System.Windows.Forms.Button button_web_login;
        private System.Windows.Forms.Button button_relay_start;
        private System.Windows.Forms.GroupBox groupBox_users_location;
        private System.Windows.Forms.TextBox textBox_connection_state;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_password;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_user_id;
        private System.Windows.Forms.Button button_close;
        private System.Windows.Forms.Label label_position;
        private System.Windows.Forms.Button button_leave;
        private System.Windows.Forms.Button button_ready;
        private System.Windows.Forms.Button button_spawn;
    }
}

