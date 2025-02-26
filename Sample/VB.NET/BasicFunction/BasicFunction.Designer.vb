<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class basicfunction
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
		Me.AE_ModeList = New System.Windows.Forms.ComboBox()
		Me.Gain = New System.Windows.Forms.GroupBox()
		Me.GainApply = New System.Windows.Forms.Button()
		Me.GainEdit = New System.Windows.Forms.NumericUpDown()
		Me.ROI = New System.Windows.Forms.GroupBox()
		Me.ROI_ModeList = New System.Windows.Forms.ComboBox()
		Me.ExposureTime = New System.Windows.Forms.GroupBox()
		Me.ExposureTimeApply = New System.Windows.Forms.Button()
		Me.ExposureTimeEdit = New System.Windows.Forms.NumericUpDown()
		Me.AntiFlickModeList = New System.Windows.Forms.ComboBox()
		Me.PictureBox = New System.Windows.Forms.PictureBox()
		Me.AE_Mode = New System.Windows.Forms.Label()
		Me.AE_Enable = New System.Windows.Forms.CheckBox()
		Me.button_pro = New System.Windows.Forms.Button()
		Me.Open = New System.Windows.Forms.Button()
		Me.DevicesList = New System.Windows.Forms.ComboBox()
		Me.Save = New System.Windows.Forms.Button()
		Me.AutoExposure = New System.Windows.Forms.GroupBox()
		Me.SaveImage = New System.Windows.Forms.GroupBox()
		Me.Start = New System.Windows.Forms.Button()
		Me.Scan = New System.Windows.Forms.Button()
		Me.General = New System.Windows.Forms.GroupBox()
		Me.UserDefinedName = New System.Windows.Forms.CheckBox()
		Me.groupBox2 = New System.Windows.Forms.GroupBox()
		Me.AWBOnce = New System.Windows.Forms.Button()
		Me.AWBEnable = New System.Windows.Forms.CheckBox()
		Me.GroupBox1 = New System.Windows.Forms.GroupBox()
		Me.Gain.SuspendLayout()
		CType(Me.GainEdit, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.ROI.SuspendLayout()
		Me.ExposureTime.SuspendLayout()
		CType(Me.ExposureTimeEdit, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.PictureBox, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.AutoExposure.SuspendLayout()
		Me.SaveImage.SuspendLayout()
		Me.General.SuspendLayout()
		Me.groupBox2.SuspendLayout()
		Me.GroupBox1.SuspendLayout()
		Me.SuspendLayout()
		'
		'AE_ModeList
		'
		Me.AE_ModeList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.AE_ModeList.FormattingEnabled = True
		Me.AE_ModeList.Location = New System.Drawing.Point(79, 39)
		Me.AE_ModeList.Name = "AE_ModeList"
		Me.AE_ModeList.Size = New System.Drawing.Size(170, 20)
		Me.AE_ModeList.TabIndex = 2
		'
		'Gain
		'
		Me.Gain.Controls.Add(Me.GainApply)
		Me.Gain.Controls.Add(Me.GainEdit)
		Me.Gain.Location = New System.Drawing.Point(4, 353)
		Me.Gain.Name = "Gain"
		Me.Gain.Size = New System.Drawing.Size(255, 49)
		Me.Gain.TabIndex = 19
		Me.Gain.TabStop = False
		Me.Gain.Text = "Gain(x)"
		'
		'GainApply
		'
		Me.GainApply.Location = New System.Drawing.Point(192, 17)
		Me.GainApply.Name = "GainApply"
		Me.GainApply.Size = New System.Drawing.Size(57, 23)
		Me.GainApply.TabIndex = 35
		Me.GainApply.Text = "Apply"
		Me.GainApply.UseVisualStyleBackColor = True
		'
		'GainEdit
		'
		Me.GainEdit.Location = New System.Drawing.Point(79, 18)
		Me.GainEdit.Name = "GainEdit"
		Me.GainEdit.Size = New System.Drawing.Size(96, 21)
		Me.GainEdit.TabIndex = 33
		'
		'ROI
		'
		Me.ROI.Controls.Add(Me.ROI_ModeList)
		Me.ROI.Location = New System.Drawing.Point(4, 465)
		Me.ROI.Name = "ROI"
		Me.ROI.Size = New System.Drawing.Size(255, 45)
		Me.ROI.TabIndex = 20
		Me.ROI.TabStop = False
		Me.ROI.Text = "Preset Roi"
		'
		'ROI_ModeList
		'
		Me.ROI_ModeList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.ROI_ModeList.FormattingEnabled = True
		Me.ROI_ModeList.Location = New System.Drawing.Point(79, 16)
		Me.ROI_ModeList.Name = "ROI_ModeList"
		Me.ROI_ModeList.Size = New System.Drawing.Size(170, 20)
		Me.ROI_ModeList.TabIndex = 0
		'
		'ExposureTime
		'
		Me.ExposureTime.Controls.Add(Me.ExposureTimeApply)
		Me.ExposureTime.Controls.Add(Me.ExposureTimeEdit)
		Me.ExposureTime.Location = New System.Drawing.Point(4, 246)
		Me.ExposureTime.Name = "ExposureTime"
		Me.ExposureTime.Size = New System.Drawing.Size(254, 50)
		Me.ExposureTime.TabIndex = 18
		Me.ExposureTime.TabStop = False
		Me.ExposureTime.Text = "Exposure Time(us)"
		'
		'ExposureTimeApply
		'
		Me.ExposureTimeApply.Location = New System.Drawing.Point(192, 19)
		Me.ExposureTimeApply.Name = "ExposureTimeApply"
		Me.ExposureTimeApply.Size = New System.Drawing.Size(57, 23)
		Me.ExposureTimeApply.TabIndex = 34
		Me.ExposureTimeApply.Text = "Apply"
		Me.ExposureTimeApply.UseVisualStyleBackColor = True
		'
		'ExposureTimeEdit
		'
		Me.ExposureTimeEdit.Increment = New Decimal(New Integer() {100, 0, 0, 0})
		Me.ExposureTimeEdit.Location = New System.Drawing.Point(79, 20)
		Me.ExposureTimeEdit.Name = "ExposureTimeEdit"
		Me.ExposureTimeEdit.Size = New System.Drawing.Size(96, 21)
		Me.ExposureTimeEdit.TabIndex = 33
		'
		'AntiFlickModeList
		'
		Me.AntiFlickModeList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.AntiFlickModeList.FormattingEnabled = True
		Me.AntiFlickModeList.Location = New System.Drawing.Point(79, 17)
		Me.AntiFlickModeList.Name = "AntiFlickModeList"
		Me.AntiFlickModeList.Size = New System.Drawing.Size(170, 20)
		Me.AntiFlickModeList.TabIndex = 0
		'
		'PictureBox
		'
		Me.PictureBox.BackColor = System.Drawing.SystemColors.ControlDark
		Me.PictureBox.Location = New System.Drawing.Point(265, 12)
		Me.PictureBox.Name = "PictureBox"
		Me.PictureBox.Size = New System.Drawing.Size(666, 560)
		Me.PictureBox.TabIndex = 21
		Me.PictureBox.TabStop = False
		'
		'AE_Mode
		'
		Me.AE_Mode.AutoSize = True
		Me.AE_Mode.Location = New System.Drawing.Point(10, 43)
		Me.AE_Mode.Name = "AE_Mode"
		Me.AE_Mode.Size = New System.Drawing.Size(47, 12)
		Me.AE_Mode.TabIndex = 1
		Me.AE_Mode.Text = "AE Mode"
		'
		'AE_Enable
		'
		Me.AE_Enable.AutoSize = True
		Me.AE_Enable.Location = New System.Drawing.Point(10, 20)
		Me.AE_Enable.Name = "AE_Enable"
		Me.AE_Enable.Size = New System.Drawing.Size(78, 16)
		Me.AE_Enable.TabIndex = 0
		Me.AE_Enable.Text = "AE Enable"
		Me.AE_Enable.UseVisualStyleBackColor = True
		'
		'button_pro
		'
		Me.button_pro.Location = New System.Drawing.Point(174, 74)
		Me.button_pro.Name = "button_pro"
		Me.button_pro.Size = New System.Drawing.Size(75, 23)
		Me.button_pro.TabIndex = 4
		Me.button_pro.Text = "Property"
		Me.button_pro.UseVisualStyleBackColor = True
		'
		'Open
		'
		Me.Open.Location = New System.Drawing.Point(10, 74)
		Me.Open.Name = "Open"
		Me.Open.Size = New System.Drawing.Size(75, 23)
		Me.Open.TabIndex = 1
		Me.Open.Text = "Open"
		Me.Open.UseVisualStyleBackColor = True
		'
		'DevicesList
		'
		Me.DevicesList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.DevicesList.FormattingEnabled = True
		Me.DevicesList.Location = New System.Drawing.Point(10, 48)
		Me.DevicesList.Name = "DevicesList"
		Me.DevicesList.Size = New System.Drawing.Size(239, 20)
		Me.DevicesList.TabIndex = 0
		'
		'Save
		'
		Me.Save.Location = New System.Drawing.Point(174, 14)
		Me.Save.Name = "Save"
		Me.Save.Size = New System.Drawing.Size(75, 23)
		Me.Save.TabIndex = 0
		Me.Save.Text = "Save"
		Me.Save.UseVisualStyleBackColor = True
		'
		'AutoExposure
		'
		Me.AutoExposure.Controls.Add(Me.AE_ModeList)
		Me.AutoExposure.Controls.Add(Me.AE_Mode)
		Me.AutoExposure.Controls.Add(Me.AE_Enable)
		Me.AutoExposure.Location = New System.Drawing.Point(4, 172)
		Me.AutoExposure.Name = "AutoExposure"
		Me.AutoExposure.Size = New System.Drawing.Size(254, 68)
		Me.AutoExposure.TabIndex = 17
		Me.AutoExposure.TabStop = False
		Me.AutoExposure.Text = "Auto Exposure"
		'
		'SaveImage
		'
		Me.SaveImage.Controls.Add(Me.Save)
		Me.SaveImage.Location = New System.Drawing.Point(4, 119)
		Me.SaveImage.Name = "SaveImage"
		Me.SaveImage.Size = New System.Drawing.Size(254, 47)
		Me.SaveImage.TabIndex = 16
		Me.SaveImage.TabStop = False
		Me.SaveImage.Text = "Save Image"
		'
		'Start
		'
		Me.Start.Location = New System.Drawing.Point(92, 74)
		Me.Start.Name = "Start"
		Me.Start.Size = New System.Drawing.Size(75, 23)
		Me.Start.TabIndex = 2
		Me.Start.Text = "Start"
		Me.Start.UseVisualStyleBackColor = True
		'
		'Scan
		'
		Me.Scan.Location = New System.Drawing.Point(174, 18)
		Me.Scan.Name = "Scan"
		Me.Scan.Size = New System.Drawing.Size(75, 23)
		Me.Scan.TabIndex = 3
		Me.Scan.Text = "Scan"
		Me.Scan.UseVisualStyleBackColor = True
		'
		'General
		'
		Me.General.Controls.Add(Me.UserDefinedName)
		Me.General.Controls.Add(Me.button_pro)
		Me.General.Controls.Add(Me.Scan)
		Me.General.Controls.Add(Me.Start)
		Me.General.Controls.Add(Me.Open)
		Me.General.Controls.Add(Me.DevicesList)
		Me.General.Location = New System.Drawing.Point(4, 6)
		Me.General.Name = "General"
		Me.General.Size = New System.Drawing.Size(254, 107)
		Me.General.TabIndex = 15
		Me.General.TabStop = False
		Me.General.Text = "General"
		'
		'UserDefinedName
		'
		Me.UserDefinedName.AutoSize = True
		Me.UserDefinedName.Location = New System.Drawing.Point(10, 23)
		Me.UserDefinedName.Name = "UserDefinedName"
		Me.UserDefinedName.Size = New System.Drawing.Size(126, 16)
		Me.UserDefinedName.TabIndex = 34
		Me.UserDefinedName.Text = "User Defined Name"
		Me.UserDefinedName.UseVisualStyleBackColor = True
		'
		'groupBox2
		'
		Me.groupBox2.Controls.Add(Me.AWBOnce)
		Me.groupBox2.Controls.Add(Me.AWBEnable)
		Me.groupBox2.Location = New System.Drawing.Point(4, 408)
		Me.groupBox2.Name = "groupBox2"
		Me.groupBox2.Size = New System.Drawing.Size(254, 51)
		Me.groupBox2.TabIndex = 31
		Me.groupBox2.TabStop = False
		Me.groupBox2.Text = "White Balance"
		'
		'AWBOnce
		'
		Me.AWBOnce.Location = New System.Drawing.Point(136, 16)
		Me.AWBOnce.Name = "AWBOnce"
		Me.AWBOnce.Size = New System.Drawing.Size(113, 23)
		Me.AWBOnce.TabIndex = 1
		Me.AWBOnce.Text = "White Balance"
		Me.AWBOnce.UseVisualStyleBackColor = True
		'
		'AWBEnable
		'
		Me.AWBEnable.AutoSize = True
		Me.AWBEnable.Location = New System.Drawing.Point(10, 20)
		Me.AWBEnable.Name = "AWBEnable"
		Me.AWBEnable.Size = New System.Drawing.Size(84, 16)
		Me.AWBEnable.TabIndex = 0
		Me.AWBEnable.Text = "AWB Enable"
		Me.AWBEnable.UseVisualStyleBackColor = True
		'
		'GroupBox1
		'
		Me.GroupBox1.Controls.Add(Me.AntiFlickModeList)
		Me.GroupBox1.Location = New System.Drawing.Point(4, 302)
		Me.GroupBox1.Name = "GroupBox1"
		Me.GroupBox1.Size = New System.Drawing.Size(254, 45)
		Me.GroupBox1.TabIndex = 32
		Me.GroupBox1.TabStop = False
		Me.GroupBox1.Text = "Anti Flick"
		'
		'basicfunction
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(937, 578)
		Me.Controls.Add(Me.GroupBox1)
		Me.Controls.Add(Me.groupBox2)
		Me.Controls.Add(Me.Gain)
		Me.Controls.Add(Me.ROI)
		Me.Controls.Add(Me.ExposureTime)
		Me.Controls.Add(Me.PictureBox)
		Me.Controls.Add(Me.AutoExposure)
		Me.Controls.Add(Me.SaveImage)
		Me.Controls.Add(Me.General)
		Me.Name = "basicfunction"
		Me.Text = "BasicFunction"
		Me.Gain.ResumeLayout(False)
		CType(Me.GainEdit, System.ComponentModel.ISupportInitialize).EndInit()
		Me.ROI.ResumeLayout(False)
		Me.ExposureTime.ResumeLayout(False)
		CType(Me.ExposureTimeEdit, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.PictureBox, System.ComponentModel.ISupportInitialize).EndInit()
		Me.AutoExposure.ResumeLayout(False)
		Me.AutoExposure.PerformLayout()
		Me.SaveImage.ResumeLayout(False)
		Me.General.ResumeLayout(False)
		Me.General.PerformLayout()
		Me.groupBox2.ResumeLayout(False)
		Me.groupBox2.PerformLayout()
		Me.GroupBox1.ResumeLayout(False)
		Me.ResumeLayout(False)

	End Sub
	Friend WithEvents AE_ModeList As System.Windows.Forms.ComboBox
	Friend WithEvents Gain As System.Windows.Forms.GroupBox
	Friend WithEvents ROI As System.Windows.Forms.GroupBox
	Friend WithEvents ROI_ModeList As System.Windows.Forms.ComboBox
	Friend WithEvents ExposureTime As System.Windows.Forms.GroupBox
	Friend WithEvents AntiFlickModeList As System.Windows.Forms.ComboBox
	Friend WithEvents PictureBox As System.Windows.Forms.PictureBox
	Friend WithEvents AE_Mode As System.Windows.Forms.Label
	Friend WithEvents AE_Enable As System.Windows.Forms.CheckBox
	Friend WithEvents button_pro As System.Windows.Forms.Button
	Friend WithEvents Open As System.Windows.Forms.Button
	Friend WithEvents DevicesList As System.Windows.Forms.ComboBox
	Friend WithEvents Save As System.Windows.Forms.Button
	Friend WithEvents AutoExposure As System.Windows.Forms.GroupBox
	Friend WithEvents SaveImage As System.Windows.Forms.GroupBox
	Friend WithEvents Start As System.Windows.Forms.Button
	Friend WithEvents Scan As System.Windows.Forms.Button
	Friend WithEvents General As System.Windows.Forms.GroupBox
	Private WithEvents GainApply As System.Windows.Forms.Button
	Private WithEvents GainEdit As System.Windows.Forms.NumericUpDown
	Private WithEvents ExposureTimeApply As System.Windows.Forms.Button
	Private WithEvents ExposureTimeEdit As System.Windows.Forms.NumericUpDown
	Private WithEvents groupBox2 As System.Windows.Forms.GroupBox
	Private WithEvents AWBOnce As System.Windows.Forms.Button
	Private WithEvents AWBEnable As System.Windows.Forms.CheckBox
	Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
	Private WithEvents UserDefinedName As System.Windows.Forms.CheckBox

End Class
