namespace Halcon_Code
{
    partial class Halconcode
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Code

        private void InitializeComponent()
        {
            this.hWindowControl1 = new HalconDotNet.HWindowControl();
            this.OpenCamera = new System.Windows.Forms.Button();
            this.Grab = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.CameraName = new System.Windows.Forms.ComboBox();
            this.CamerScan = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // hWindowControl1
            // 
            this.hWindowControl1.BackColor = System.Drawing.Color.Black;
            this.hWindowControl1.BorderColor = System.Drawing.Color.Black;
            this.hWindowControl1.ImagePart = new System.Drawing.Rectangle(0, 0, 640, 480);
            this.hWindowControl1.Location = new System.Drawing.Point(30, 25);
            this.hWindowControl1.Name = "hWindowControl1";
            this.hWindowControl1.Size = new System.Drawing.Size(943, 679);
            this.hWindowControl1.TabIndex = 0;
            this.hWindowControl1.WindowSize = new System.Drawing.Size(943, 679);
            // 
            // OpenCamera
            // 
            this.OpenCamera.Location = new System.Drawing.Point(1041, 58);
            this.OpenCamera.Name = "OpenCamera";
            this.OpenCamera.Size = new System.Drawing.Size(154, 22);
            this.OpenCamera.TabIndex = 11;
            this.OpenCamera.Text = "Open Camera";
            this.OpenCamera.UseVisualStyleBackColor = true;
            this.OpenCamera.Click += new System.EventHandler(this.OpenCamera_Click);
            // 
            // Grab
            // 
            this.Grab.Location = new System.Drawing.Point(1203, 58);
            this.Grab.Name = "Grab";
            this.Grab.Size = new System.Drawing.Size(147, 22);
            this.Grab.TabIndex = 10;
            this.Grab.Text = "Start Grab";
            this.Grab.UseVisualStyleBackColor = true;
            this.Grab.Click += new System.EventHandler(this.Grab_Click_1);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(978, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 18;
            this.label1.Text = "Choose Camera：";
            // 
            // CameraName
            // 
            this.CameraName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CameraName.FormattingEnabled = true;
            this.CameraName.Location = new System.Drawing.Point(1041, 25);
            this.CameraName.Name = "CameraName";
            this.CameraName.Size = new System.Drawing.Size(244, 20);
            this.CameraName.TabIndex = 19;
            // 
            // CamerScan
            // 
            this.CamerScan.Location = new System.Drawing.Point(1291, 23);
            this.CamerScan.Name = "CamerScan";
            this.CamerScan.Size = new System.Drawing.Size(59, 23);
            this.CamerScan.TabIndex = 21;
            this.CamerScan.Text = "Refresh";
            this.CamerScan.UseVisualStyleBackColor = true;
            this.CamerScan.Click += new System.EventHandler(this.CamerScan_Click);
            // 
            // Halconcode
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1478, 840);
            this.Controls.Add(this.CamerScan);
            this.Controls.Add(this.CameraName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.OpenCamera);
            this.Controls.Add(this.Grab);
            this.Controls.Add(this.hWindowControl1);
            this.Name = "Halconcode";
            this.Text = "code";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Halconcode_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private HalconDotNet.HWindowControl hWindowControl1;
        private System.Windows.Forms.Button OpenCamera;
        private System.Windows.Forms.Button Grab;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox CameraName;
        private System.Windows.Forms.Button CamerScan;
    }
}

