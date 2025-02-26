Imports DVPCameraType
Imports DVPCameraType.DVPCamera

Public Class ImageAcquisition

    Private m_handle As UInteger
    Public m_strFriendlyName As String
    Dim m_pThread As Threading.Thread
    Dim m_ThreadFlag As Boolean
    Private Shared m_ThreadEvent As New System.Threading.AutoResetEvent(False)
    Public m_ptr_wnd As IntPtr
    Public m_strfiledir As String
    Public m_strfilepath As String
    Dim m_GetImage As ImageAcquisition
    Dim m_uImageCount As Integer
    Private WithEvents m_Timer As New Timer()

	'Display param
	Public m_Stopwatch As New Stopwatch()
	Public m_uDisplayCount As UInteger

	Public m_info(16) As dvpCameraInfo
	Public m_CamCount As Integer

    ' Check whether the camera is opened.
    Private Function IsValidHandle(ByVal handleid As UInteger) As Boolean
        Dim bValidHandle As Boolean
        Dim status As dvpStatus
        bValidHandle = False

		status = DVPCamera.dvpIsValid(handleid, bValidHandle)
		If status = dvpStatus.DVP_STATUS_OK Then
			Return bValidHandle
		End If

		Return False
	End Function


    Private Sub ImageAcquisition_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        m_handle = 0
        m_ptr_wnd = pictureBox.Handle
        m_strFriendlyName = ""
        m_strfiledir = ""
        m_strfilepath = ""
        m_GetImage = Me
        m_ThreadEvent.Set()
        m_ThreadFlag = False
        m_uImageCount = 0
        EditPath.Text = "."
        ScanCamera()


        Display.Checked = True
        Form.CheckForIllegalCrossThreadCalls = False
        m_Timer.Interval = 500

        ' Enable the execution of the system.Timers.Timer.Elapsed event.
        m_Timer.Enabled = True
		GrabTimeout.Text = "4000"
		EditWait.Text = "0"

		EditPath.Text = Application.StartupPath()
		m_strfiledir = EditPath.Text

		' Initialize open mode
		' false: user dvpOpenByName open the camear
		' true : user dvpOpenByUserId open the camear
		UserDefinedName.Checked = False

    End Sub


    Public Sub theout(ByVal sender As Object, ByVal e As EventArgs) Handles m_Timer.Tick
        If (IsValidHandle(m_handle)) Then

            ' Update the information of frame rate.
            Dim count As New dvpFrameCount
            Dim str As String
            Dim status As dvpStatus

			status = DVPCamera.dvpGetFrameCount(m_handle, count)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Get frame count fail!")
				Return
			End If

			If m_uDisplayCount = 0 Or m_Stopwatch.ElapsedMilliseconds < 2 Then
				str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount * 1000.0F / m_Stopwatch.ElapsedMilliseconds) + " fps]"
			End If

			Me.Text = str
            Counts.Text = m_uImageCount.ToString()
        End If
    End Sub


    Private Sub UpdateControls()
        Dim status As New dvpStatus()
        Dim state As New dvpStreamState()

        If IsValidHandle(m_handle) Then

			status = DVPCamera.dvpGetStreamState(m_handle, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
            
            OpenDev.Text = "Close"
            OpenDev.Enabled = True
            StartPlay.Text = IIf(state = dvpStreamState.STATE_STARTED, "Stop", "Start")

            StartPlay.Enabled = True
            PropertSet.Enabled = True

			Dim bSoftTrigger As Boolean
			status = DVPCamera.dvpGetTriggerState(m_handle, bSoftTrigger)
			If status = dvpStatus.DVP_STATUS_OK Then
				SoftTrigger.Enabled = state <> dvpStreamState.STATE_STARTED
				SoftTrigger.Checked = bSoftTrigger
			Else
				SoftTrigger.Enabled = False
				SoftTrigger.Checked = False
			End If

            ' Update the related controls.
            If (state = dvpStreamState.STATE_STARTED) Then

                ' If the thread of acquiring images is still running,it can simplify multithreaded synchronization by disabling some operations.
                SaveImageCheck.Enabled = False
                Display.Enabled = False
                SelectPath.Enabled = False
                EditPath.Enabled = False

				EditWait.Enabled = False
				If status = dvpStatus.DVP_STATUS_OK Then
					SoftTrigger.Enabled = False
				End If

				StartPlay.Text = "Stop"
				GrabTimeout.Enabled = False
            Else
                OpenDev.Enabled = IIf(DevicesList.Items.Count > 0, True, False)
                SaveImageCheck.Enabled = True
                Display.Enabled = True
                SelectPath.Enabled = True
                EditPath.Enabled = True
                OpenFolder.Enabled = True
				EditWait.Enabled = True
				If status = dvpStatus.DVP_STATUS_OK Then
					SoftTrigger.Enabled = True
				End If

				StartPlay.Text = "Start"
				GrabTimeout.Enabled = True
            End If
        Else

            ' No device is opened at this time.
            ' Update the basic controls.
            OpenDev.Text = "Open"
            StartPlay.Enabled = False
            PropertSet.Enabled = False
            OpenDev.Enabled = IIf(DevicesList.Items.Count > 0, True, False)

            ' Update the related controls.
            SaveImageCheck.Enabled = False
            Display.Enabled = False
            SelectPath.Enabled = False
            OpenFolder.Enabled = False
			SaveImageCheck.Enabled = False

			SoftTrigger.Enabled = False
			SoftTrigger.Checked = False

        End If
    End Sub

    Private Sub ImageThread()
        Dim status As dvpStatus
        Dim m_systm As New DateTime()

        ' Wait the time of the event timeout.
        Dim time_out As Integer
        time_out = 40

        If EditWait.Text.Length > 0 Then
			time_out = CUInt(EditWait.Text)
			EditWait.Text = time_out.ToString()
        End If
		GrabTimeout.Text = CUInt(GrabTimeout.Text).ToString()

        While m_ThreadEvent.WaitOne(time_out) = False
            Dim pBuffer As New IntPtr()
            pBuffer = 0
            Dim frame As New dvpFrame()

			If (SoftTrigger.Checked) Then
				Dim bSoftTrigger As Boolean
				status = DVPCamera.dvpGetTriggerState(m_handle, bSoftTrigger)
				' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				If bSoftTrigger Then
					Dim bSoftTriggerLoop As Boolean
					status = DVPCamera.dvpGetSoftTriggerLoopState(m_handle, bSoftTriggerLoop)
					' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
					If bSoftTriggerLoop = False Then
						status = DVPCamera.dvpTriggerFire(m_handle)
						' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
						If (status <> dvpStatus.DVP_STATUS_OK) Then
							' Trigger failure maybe result from that the trigger signal interval is too dense.
							Continue While
						End If
					End If
				End If
			End If

			' Grab a frame image from the video stream and timeout should not less than the current exposure time.
			status = DVPCamera.dvpGetFrame(m_handle, frame, pBuffer, CUInt(GrabTimeout.Text))

			If (status <> dvpStatus.DVP_STATUS_OK) Then
				' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				Continue While
			End If

			' If need to save images.
			If (SaveImageCheck.Checked) Then

				' Save the bmp image by using systematic time as filename.
				Dim now As New DateTime()
				now = DateTime.Now
				m_strfilepath = m_strfiledir + "\\" + now.ToString("yyyy-MM-dd") + " " + now.TimeOfDay.ToString().Replace(":", "-").Replace(".", "-") + ".bmp"

				status = DVPCamera.dvpSavePicture(frame, pBuffer, m_strfilepath, 100)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

				m_uImageCount = m_uImageCount + 1
			End If

			Dim bDisplay As Boolean
			bDisplay = False

			If m_uDisplayCount = 0 Then
				m_Stopwatch.Restart()
				bDisplay = True
			Else
				If (m_Stopwatch.ElapsedMilliseconds - (m_uDisplayCount * 33.3)) >= 33.0 Then
					bDisplay = True
				End If
			End If

			' Display images in the preview window, if needed.
			If Display.Checked And bDisplay Then

				m_uDisplayCount += 1

				' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				' in order to avoid affecting the frame rate and the real-time of acquiring images.
				' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				' however, the user can malloc memory and copy image data.
				status = DVPCamera.dvpDrawPicture(frame, pBuffer, m_ptr_wnd, 0, 0)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			End If

		End While

        m_pThread.Abort()
        m_ThreadFlag = False
    End Sub


    Private Sub ScanDev_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev.Click
        ScanCamera()
    End Sub


    Private Sub ScanCamera()
        Dim iNum As UInteger
        Dim iCount As UInteger
        Dim info As New dvpCameraInfo()
		Dim status As dvpStatus
        ' "iCount" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.

        ' Clear the list of the camera.
        DevicesList.Items.Clear()

        If dvpStatus.DVP_STATUS_OK = dvpRefresh(iCount) Then

            ' Enumeration of up to 16 cameras.
            If iCount > 16 Then
				iCount = 16
			ElseIf iCount = 0 Then
				Return
            End If

            For iNum = 0 To iCount - 1

				' Acquire each camera's information one by one.
				status = dvpEnum(iNum, info)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

				If dvpStatus.DVP_STATUS_OK = status Then

					m_info(m_CamCount) = info
					m_CamCount += 1

					If UserDefinedName.Checked Then
						DevicesList.Items.Add(info.UserID)
					Else
						DevicesList.Items.Add(info.FriendlyName)
					End If

				End If
            Next

            If iNum > 0 Then
                DevicesList.SelectedIndex = 0
            End If

			UpdateControls()
        End If
    End Sub


    Private Sub OpenDev_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev.Click
        OpenCamera()
    End Sub


    Private Sub OpenCamera()
        Dim status As New dvpStatus()
        Dim strName As String
        strName = ""

		If IsValidHandle(m_handle) Then

            ' Close the thread of grabbing image.
			If (m_ThreadFlag) Then
				m_ThreadEvent.Set()
				While (m_pThread.ThreadState = ThreadState.Running)
					System.Threading.Thread.Sleep(100)
				End While
				System.Threading.Thread.Sleep(100)
			End If

			Dim StreamState As dvpStreamState
			' check camera
			status = dvpGetStreamState(m_handle, StreamState)
			If dvpStatus.DVP_STATUS_OK = status Then
				If StreamState = dvpStreamState.STATE_STARTED Then
					' stop camera
					status = dvpStop(m_handle)
					If dvpStatus.DVP_STATUS_OK <> status Then
						MessageBox.Show("Stop video stream fail!")
						Return
					End If
				End If
			End If

			m_uDisplayCount = 0

			
			If dvpStatus.DVP_STATUS_OK = dvpClose(m_handle) Then
				m_handle = 0
			Else
				MessageBox.Show("Close video stream fail!")
			End If

			pictureBox.Refresh()
		Else
			If DevicesList.Text <> "" Then

				If UserDefinedName.Checked Then
					' Open the specific device by the selected user define name.
					status = dvpOpenByUserId(DevicesList.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				Else
					' Open the specific device by the selected FriendlyName.
					status = dvpOpenByName(DevicesList.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				End If
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed!")
				End If

				m_strFriendlyName = DevicesList.Text
			End If
		End If

		UpdateControls()
    End Sub


    Private Sub StartPlay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay.Click
        StartCamera()
    End Sub


    Private Sub StartCamera()
        Dim state As New dvpStreamState()
        Dim status As dvpStatus

        If IsValidHandle(m_handle) Then

            ' Implement a button to start and stop according to the current video's status.
            status = DVPCamera.dvpGetStreamState(m_handle, state)
                            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle)

                If status <> dvpStatus.DVP_STATUS_OK Then
                    MessageBox.Show("Stop video stream fail!")
                End If

				If m_pThread.ThreadState <> System.Threading.ThreadState.Aborted And m_pThread.ThreadState <> System.Threading.ThreadState.Stopped Then
					m_ThreadEvent.Set()
					m_pThread.Abort()
				End If
			Else

                ' Set the trigger mode before invoking the "dvpStart".
				Dim flg As Boolean
				flg = False

                ' Judge whether the trigger function can be supported.
				status = DVPCamera.dvpGetTriggerState(m_handle, flg)
				If (status = dvpStatus.DVP_STATUS_OK) Then
					flg = IIf(SoftTrigger.Checked, True, False)
					status = DVPCamera.dvpSetTriggerState(m_handle, flg)
					If (status <> dvpStatus.DVP_STATUS_OK) Then
                        MessageBox.Show("Set trigger mode fails!")
					End If
				End If

                ' Play the video stream.
				status = DVPCamera.dvpStart(m_handle)
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Start video stream failed!")
				Else
					' Reset the event of exitting thread.
					m_ThreadEvent.Reset()

					' Create the thread of acquiring image.
					m_pThread = New Threading.Thread(AddressOf ImageThread)
					m_pThread.Start()
					m_ThreadFlag = True

				End If
			End If
        End If
        UpdateControls()
    End Sub

    Private Sub PropertSet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertSet.Click
        SetParam()
    End Sub

    Private Sub SetParam()
        Dim status As dvpStatus

        If IsValidHandle(m_handle) Then
            status = DVPCamera.dvpShowPropertyModalDialog(m_handle, Me.Handle())

            ' At this time some configurations may change, synchronize it to the window GUI.
            UpdateControls()
        End If
    End Sub

    Private Sub SelectPath_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SelectPath.Click
        Dim Dlg As New FolderBrowserDialog()

        If (Dlg.ShowDialog() = DialogResult.OK) Then
            m_strfiledir = Dlg.SelectedPath
            EditPath.Text = m_strfiledir
        End If
    End Sub


    Private Sub OpenFolder_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenFolder.Click
        If m_strfiledir.Length > 0 Then
            System.Diagnostics.Process.Start(m_strfiledir)
        End If
    End Sub

	Private Sub ImageAcquisition_FormClosing(sender As System.Object, e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing

        ' Close the camera.
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then
			
            ' Close the thread of grabbing image.
			If (m_ThreadFlag) Then
				m_ThreadEvent.Set()
				m_pThread.Abort()
            End If

            ' Stop the video stream.
			status = DVPCamera.dvpStop(m_handle)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Close the camera.
			status = DVPCamera.dvpClose(m_handle)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			m_handle = 0
			pictureBox.Refresh()
		End If
	End Sub

	Private Sub SoftTrigger_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles SoftTrigger.CheckedChanged

		If IsValidHandle(m_handle) Then

			Dim status As dvpStatus

			' Get Stream State
			Dim StreamState As New dvpStreamState()
			status = dvpGetStreamState(m_handle, StreamState)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Stop the video stream.
			If StreamState = dvpStreamState.STATE_STARTED Then
				status = dvpStop(m_handle)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			End If

			' Enable/disable the trigger mode.
			status = dvpSetTriggerState(m_handle, SoftTrigger.Checked)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Start the video stream.
			If StreamState = dvpStreamState.STATE_STARTED Then
				status = dvpStart(m_handle)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			End If

			UpdateControls()

		End If

	End Sub

	Private Sub ResizeWindows()
		If (IsValidHandle(m_handle)) Then

			Dim roi As New dvpRegion
			Dim status As dvpStatus
			status = dvpGetRoi(m_handle, roi)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			pictureBox.Width = MyBase.Width - pictureBox.Left
			pictureBox.Height = MyBase.Height - pictureBox.Top

			If pictureBox.Width * roi.H > pictureBox.Height * roi.W Then
				pictureBox.Width = pictureBox.Height * roi.W / roi.H
			Else
				pictureBox.Height = pictureBox.Width * roi.H / roi.W
			End If
		End If
	End Sub

	Private Sub ImageAcquisition_Resize(sender As System.Object, e As System.EventArgs) Handles MyBase.Resize
		ResizeWindows()
	End Sub

	Private Sub UserDefinedName_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles UserDefinedName.CheckedChanged
		' save cur sel item
		Dim strName As String
		strName = DevicesList.Text
		' reset m_listDevices values
		DevicesList.Items.Clear()
		For i = 0 To m_CamCount - 1

			Dim item As Integer
			item = -1

			If UserDefinedName.Checked = False Then

				item = DevicesList.Items.Add(m_info(i).FriendlyName)
				If (strName = m_info(i).UserID) Then
					DevicesList.SelectedIndex = item
				End If

			Else
				' check User define name is null
				If m_info(i).UserID.Length = 0 Then
					Continue For
				End If

				item = DevicesList.Items.Add(m_info(i).UserID)
				If strName = m_info(i).FriendlyName Then
					DevicesList.SelectedIndex = item
				End If

			End If
		Next
	End Sub
End Class

