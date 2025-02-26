<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ImageAcquisition
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
		Me.EditWait = New System.Windows.Forms.TextBox()
		Me.PropertSet = New System.Windows.Forms.Button()
		Me.DisplayImage = New System.Windows.Forms.GroupBox()
		Me.Display = New System.Windows.Forms.CheckBox()
		Me.OpenFolder = New System.Windows.Forms.Button()
		Me.label1 = New System.Windows.Forms.Label()
		Me.General = New System.Windows.Forms.GroupBox()
		Me.UserDefinedName = New System.Windows.Forms.CheckBox()
		Me.StartPlay = New System.Windows.Forms.Button()
		Me.ScanDev = New System.Windows.Forms.Button()
		Me.OpenDev = New System.Windows.Forms.Button()
		Me.DevicesList = New System.Windows.Forms.ComboBox()
		Me.Configuration = New System.Windows.Forms.GroupBox()
		Me.GrabTimeout = New System.Windows.Forms.TextBox()
		Me.Label2 = New System.Windows.Forms.Label()
		Me.SoftTrigger = New System.Windows.Forms.CheckBox()
		Me.SelectPath = New System.Windows.Forms.Button()
		Me.EditPath = New System.Windows.Forms.TextBox()
		Me.Counts = New System.Windows.Forms.Label()
		Me.SaveImage = New System.Windows.Forms.GroupBox()
		Me.SaveImageCheck = New System.Windows.Forms.CheckBox()
		Me.pictureBox = New System.Windows.Forms.PictureBox()
		Me.DisplayImage.SuspendLayout()
		Me.General.SuspendLayout()
		Me.Configuration.SuspendLayout()
		Me.SaveImage.SuspendLayout()
		CType(Me.pictureBox, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.SuspendLayout()
		'
		'EditWait
		'
		Me.EditWait.Location = New System.Drawing.Point(115, 20)
		Me.EditWait.MaxLength = 8
		Me.EditWait.Name = "EditWait"
		Me.EditWait.Size = New System.Drawing.Size(86, 21)
		Me.EditWait.TabIndex = 21
		'
		'PropertSet
		'
		Me.PropertSet.Location = New System.Drawing.Point(150, 77)
		Me.PropertSet.Name = "PropertSet"
		Me.PropertSet.Size = New System.Drawing.Size(66, 23)
		Me.PropertSet.TabIndex = 5
		Me.PropertSet.Text = "Property"
		Me.PropertSet.UseVisualStyleBackColor = True
		'
		'DisplayImage
		'
		Me.DisplayImage.Controls.Add(Me.Display)
		Me.DisplayImage.Location = New System.Drawing.Point(12, 239)
		Me.DisplayImage.Name = "DisplayImage"
		Me.DisplayImage.Size = New System.Drawing.Size(225, 47)
		Me.DisplayImage.TabIndex = 37
		Me.DisplayImage.TabStop = False
		Me.DisplayImage.Text = "Display Image"
		'
		'Display
		'
		Me.Display.AutoSize = True
		Me.Display.Location = New System.Drawing.Point(33, 23)
		Me.Display.Name = "Display"
		Me.Display.Size = New System.Drawing.Size(66, 16)
		Me.Display.TabIndex = 0
		Me.Display.Text = "Display"
		Me.Display.UseVisualStyleBackColor = True
		'
		'OpenFolder
		'
		Me.OpenFolder.Location = New System.Drawing.Point(128, 67)
		Me.OpenFolder.Name = "OpenFolder"
		Me.OpenFolder.Size = New System.Drawing.Size(88, 23)
		Me.OpenFolder.TabIndex = 25
		Me.OpenFolder.Text = "Open Folder"
		Me.OpenFolder.UseVisualStyleBackColor = True
		'
		'label1
		'
		Me.label1.AutoSize = True
		Me.label1.Location = New System.Drawing.Point(7, 23)
		Me.label1.Name = "label1"
		Me.label1.Size = New System.Drawing.Size(89, 12)
		Me.label1.TabIndex = 18
		Me.label1.Text = "Wait Timer(ms)"
		'
		'General
		'
		Me.General.Controls.Add(Me.UserDefinedName)
		Me.General.Controls.Add(Me.PropertSet)
		Me.General.Controls.Add(Me.StartPlay)
		Me.General.Controls.Add(Me.ScanDev)
		Me.General.Controls.Add(Me.OpenDev)
		Me.General.Controls.Add(Me.DevicesList)
		Me.General.Location = New System.Drawing.Point(12, 12)
		Me.General.Name = "General"
		Me.General.Size = New System.Drawing.Size(225, 110)
		Me.General.TabIndex = 35
		Me.General.TabStop = False
		Me.General.Text = "General"
		'
		'UserDefinedName
		'
		Me.UserDefinedName.AutoSize = True
		Me.UserDefinedName.Location = New System.Drawing.Point(8, 26)
		Me.UserDefinedName.Name = "UserDefinedName"
		Me.UserDefinedName.Size = New System.Drawing.Size(126, 16)
		Me.UserDefinedName.TabIndex = 39
		Me.UserDefinedName.Text = "User Defined Name"
		Me.UserDefinedName.UseVisualStyleBackColor = True
		'
		'StartPlay
		'
		Me.StartPlay.Location = New System.Drawing.Point(78, 77)
		Me.StartPlay.Name = "StartPlay"
		Me.StartPlay.Size = New System.Drawing.Size(66, 23)
		Me.StartPlay.TabIndex = 4
		Me.StartPlay.Text = "Start"
		Me.StartPlay.UseVisualStyleBackColor = True
		'
		'ScanDev
		'
		Me.ScanDev.Location = New System.Drawing.Point(150, 22)
		Me.ScanDev.Name = "ScanDev"
		Me.ScanDev.Size = New System.Drawing.Size(66, 23)
		Me.ScanDev.TabIndex = 3
		Me.ScanDev.Text = "Scan"
		Me.ScanDev.UseVisualStyleBackColor = True
		'
		'OpenDev
		'
		Me.OpenDev.Location = New System.Drawing.Point(6, 77)
		Me.OpenDev.Name = "OpenDev"
		Me.OpenDev.Size = New System.Drawing.Size(66, 23)
		Me.OpenDev.TabIndex = 2
		Me.OpenDev.Text = "Open"
		Me.OpenDev.UseVisualStyleBackColor = True
		'
		'DevicesList
		'
		Me.DevicesList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.DevicesList.FormattingEnabled = True
		Me.DevicesList.ImeMode = System.Windows.Forms.ImeMode.NoControl
		Me.DevicesList.Location = New System.Drawing.Point(7, 50)
		Me.DevicesList.Name = "DevicesList"
		Me.DevicesList.Size = New System.Drawing.Size(209, 20)
		Me.DevicesList.TabIndex = 0
		'
		'Configuration
		'
		Me.Configuration.Controls.Add(Me.GrabTimeout)
		Me.Configuration.Controls.Add(Me.Label2)
		Me.Configuration.Controls.Add(Me.EditWait)
		Me.Configuration.Controls.Add(Me.SoftTrigger)
		Me.Configuration.Controls.Add(Me.label1)
		Me.Configuration.Location = New System.Drawing.Point(12, 128)
		Me.Configuration.Name = "Configuration"
		Me.Configuration.Size = New System.Drawing.Size(225, 105)
		Me.Configuration.TabIndex = 36
		Me.Configuration.TabStop = False
		Me.Configuration.Text = "Configuration"
		'
		'GrabTimeout
		'
		Me.GrabTimeout.Location = New System.Drawing.Point(115, 47)
		Me.GrabTimeout.MaxLength = 8
		Me.GrabTimeout.Name = "GrabTimeout"
		Me.GrabTimeout.Size = New System.Drawing.Size(86, 21)
		Me.GrabTimeout.TabIndex = 40
		'
		'Label2
		'
		Me.Label2.Location = New System.Drawing.Point(7, 53)
		Me.Label2.Name = "Label2"
		Me.Label2.Size = New System.Drawing.Size(103, 15)
		Me.Label2.TabIndex = 39
		Me.Label2.Text = "Grab Timeout(ms)"
		'
		'SoftTrigger
		'
		Me.SoftTrigger.AutoSize = True
		Me.SoftTrigger.Location = New System.Drawing.Point(8, 79)
		Me.SoftTrigger.Name = "SoftTrigger"
		Me.SoftTrigger.Size = New System.Drawing.Size(96, 16)
		Me.SoftTrigger.TabIndex = 19
		Me.SoftTrigger.Text = "Soft Trigger"
		Me.SoftTrigger.UseVisualStyleBackColor = True
		'
		'SelectPath
		'
		Me.SelectPath.Location = New System.Drawing.Point(7, 67)
		Me.SelectPath.Name = "SelectPath"
		Me.SelectPath.Size = New System.Drawing.Size(88, 23)
		Me.SelectPath.TabIndex = 24
		Me.SelectPath.Text = "Select Path"
		Me.SelectPath.UseVisualStyleBackColor = True
		'
		'EditPath
		'
		Me.EditPath.Location = New System.Drawing.Point(8, 42)
		Me.EditPath.Name = "EditPath"
		Me.EditPath.ReadOnly = True
		Me.EditPath.Size = New System.Drawing.Size(209, 21)
		Me.EditPath.TabIndex = 23
		'
		'Counts
		'
		Me.Counts.AutoSize = True
		Me.Counts.Location = New System.Drawing.Point(147, 23)
		Me.Counts.Name = "Counts"
		Me.Counts.Size = New System.Drawing.Size(41, 12)
		Me.Counts.TabIndex = 1
		Me.Counts.Text = "Counts"
		'
		'SaveImage
		'
		Me.SaveImage.Controls.Add(Me.OpenFolder)
		Me.SaveImage.Controls.Add(Me.SelectPath)
		Me.SaveImage.Controls.Add(Me.EditPath)
		Me.SaveImage.Controls.Add(Me.Counts)
		Me.SaveImage.Controls.Add(Me.SaveImageCheck)
		Me.SaveImage.Location = New System.Drawing.Point(12, 292)
		Me.SaveImage.Name = "SaveImage"
		Me.SaveImage.Size = New System.Drawing.Size(225, 96)
		Me.SaveImage.TabIndex = 38
		Me.SaveImage.TabStop = False
		Me.SaveImage.Text = "Save Image"
		'
		'SaveImageCheck
		'
		Me.SaveImageCheck.AutoSize = True
		Me.SaveImageCheck.Location = New System.Drawing.Point(33, 21)
		Me.SaveImageCheck.Name = "SaveImageCheck"
		Me.SaveImageCheck.Size = New System.Drawing.Size(84, 16)
		Me.SaveImageCheck.TabIndex = 0
		Me.SaveImageCheck.Text = "Save Image"
		Me.SaveImageCheck.UseVisualStyleBackColor = True
		'
		'pictureBox
		'
		Me.pictureBox.BackColor = System.Drawing.SystemColors.ActiveBorder
		Me.pictureBox.Location = New System.Drawing.Point(250, 12)
		Me.pictureBox.Name = "pictureBox"
		Me.pictureBox.Size = New System.Drawing.Size(653, 571)
		Me.pictureBox.TabIndex = 34
		Me.pictureBox.TabStop = False
		'
		'ImageAcquisition
		'
		Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.ClientSize = New System.Drawing.Size(918, 600)
		Me.Controls.Add(Me.DisplayImage)
		Me.Controls.Add(Me.General)
		Me.Controls.Add(Me.Configuration)
		Me.Controls.Add(Me.SaveImage)
		Me.Controls.Add(Me.pictureBox)
		Me.Name = "ImageAcquisition"
		Me.Text = "ImageAcquisition"
		Me.DisplayImage.ResumeLayout(False)
		Me.DisplayImage.PerformLayout()
		Me.General.ResumeLayout(False)
		Me.General.PerformLayout()
		Me.Configuration.ResumeLayout(False)
		Me.Configuration.PerformLayout()
		Me.SaveImage.ResumeLayout(False)
		Me.SaveImage.PerformLayout()
		CType(Me.pictureBox, System.ComponentModel.ISupportInitialize).EndInit()
		Me.ResumeLayout(False)

	End Sub
    Private WithEvents EditWait As System.Windows.Forms.TextBox
    Private WithEvents PropertSet As System.Windows.Forms.Button
    Private WithEvents DisplayImage As System.Windows.Forms.GroupBox
    Private WithEvents Display As System.Windows.Forms.CheckBox
    Private WithEvents OpenFolder As System.Windows.Forms.Button
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents General As System.Windows.Forms.GroupBox
    Private WithEvents StartPlay As System.Windows.Forms.Button
    Private WithEvents ScanDev As System.Windows.Forms.Button
    Private WithEvents OpenDev As System.Windows.Forms.Button
    Private WithEvents DevicesList As System.Windows.Forms.ComboBox
    Private WithEvents Configuration As System.Windows.Forms.GroupBox
    Private WithEvents SoftTrigger As System.Windows.Forms.CheckBox
    Private WithEvents SelectPath As System.Windows.Forms.Button
    Private WithEvents EditPath As System.Windows.Forms.TextBox
    Private WithEvents Counts As System.Windows.Forms.Label
    Private WithEvents SaveImage As System.Windows.Forms.GroupBox
    Private WithEvents SaveImageCheck As System.Windows.Forms.CheckBox
	Private WithEvents pictureBox As System.Windows.Forms.PictureBox
	Friend WithEvents GrabTimeout As System.Windows.Forms.TextBox
	Friend WithEvents Label2 As System.Windows.Forms.Label
	Private WithEvents UserDefinedName As System.Windows.Forms.CheckBox

End Class
