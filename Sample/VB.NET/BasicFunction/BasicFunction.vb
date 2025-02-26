Imports DVPCameraType
Imports DVPCameraType.DVPCamera


Public Class basicfunction
    Private m_handle As UInteger
    Private m_AeEnable As Boolean
    Private m_strFriendlyName As String
    Public m_ptrWnd As IntPtr
    Private WithEvents m_Timer As New Timer()

	'Display param
	Public m_Stopwatch As New Stopwatch()
	Public m_uDisplayCount As UInteger

	Public m_info(16) As dvpCameraInfo
	Public m_CamCount As Integer

	Public m_bSaveImageFlg As Boolean
	Public m_ImagePath As String

	Private Function Add(ByVal AntiFlick As dvpAntiFlick) As Object
		Throw New NotImplementedException
	End Function


	' Callback function for acquiring the video stream.
	Private Function OnDrawPicture(ByVal handle As UInteger, ByVal _event As dvpStreamEvent, ByVal pContext As IntPtr, ByRef pFrame As dvpFrame, ByVal pBuffer As IntPtr) As Integer
		Dim bDisplay As Boolean
		bDisplay = False
		Dim status As dvpStatus = New dvpStatus()

		If m_uDisplayCount = 0 Then
			m_Stopwatch.Restart()
			bDisplay = True
		Else
			If (m_Stopwatch.ElapsedMilliseconds - (m_uDisplayCount * 33.3)) >= 33.0 Then
				bDisplay = True
			End If
		End If

		If bDisplay Then

			m_uDisplayCount += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.

			status = DVPCamera.dvpDrawPicture(pFrame, pBuffer, m_ptrWnd, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		End If

		If m_bSaveImageFlg Then
			m_bSaveImageFlg = False
			status = dvpSavePicture(pFrame, pBuffer, m_ImagePath, 100)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			System.Diagnostics.Process.Start(m_ImagePath)
		End If

		Return 0

	End Function


	Private Function IsValidHandle(ByVal handle As UInteger) As Boolean
		Dim bValidHandle As Boolean
		Dim status As dvpStatus

		' Check whether the camera handle is valid.
		status = DVPCamera.dvpIsValid(handle, bValidHandle)
		If status = dvpStatus.DVP_STATUS_OK Then
			Return bValidHandle
		End If

		Return False
	End Function

	' Load BasicFunction program.
	Private Sub BasicFunctionLoad(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		Control.CheckForIllegalCrossThreadCalls = False
		m_handle = 0
		m_strFriendlyName = ""
		m_ptrWnd = PictureBox.Handle
		GainEdit.DecimalPlaces = 3
		GainEdit.Increment = 0.125
		m_Timer.Interval = 500
		m_Timer.Enabled = True

		' Initialize open mode
		' false: user dvpOpenByName open the camear
		' true : user dvpOpenByUserId open the camear
		UserDefinedName.Checked = False

		m_bSaveImageFlg = False

		ScanCamera()

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
		End If
	End Sub


	' Initialize auto exposure.
	Private Sub InitAeMode()
		AE_ModeList.Items.Clear()

		If IsValidHandle(m_handle) Then
			AE_ModeList.Items.Insert(0, "AE_MODE_AE_AG")
			AE_ModeList.Items.Insert(1, "AE_MODE_AG_AE")
			AE_ModeList.Items.Insert(2, "AE_MODE_AE_ONLY")
			AE_ModeList.Items.Insert(3, "AE_MODE_AG_ONLY")

			' Set the current index of the AE mode.
			AE_ModeList.SelectedIndex = 0
		End If
	End Sub


	' Initialize exposure time.
	Private Sub InitExpoTime()
		Dim fExpoTime As Double
		Dim ExpoTimeDescr As New dvpDoubleDescr
		Dim status As dvpStatus

		status = dvpGetExposureDescr(m_handle, ExpoTimeDescr)
		If status <> dvpStatus.DVP_STATUS_OK Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			ExposureTimeEdit.Maximum = CType(ExpoTimeDescr.fMax, Decimal)
			ExposureTimeEdit.Minimum = CType(ExpoTimeDescr.fMin, Decimal)
		End If

		status = dvpGetExposure(m_handle, fExpoTime)

		If (status <> dvpStatus.DVP_STATUS_OK) Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			status = dvpStatus.DVP_STATUS_OK
			ExposureTimeEdit.Value = CType(fExpoTime, Decimal)
		End If
	End Sub


	' Initialize the anti flick mode.
	Private Sub InitAntiFlick()
		AntiFlickModeList.Items.Clear()
		Dim status As dvpStatus
		Dim AntiFlick As dvpAntiFlick = New dvpAntiFlick()
		If IsValidHandle(m_handle) Then

			AntiFlickModeList.Items.Insert(0, "ANTIFLICK_DISABLE")
			AntiFlickModeList.Items.Insert(1, "ANTIFLICK_50HZ")
			AntiFlickModeList.Items.Insert(2, "ANTIFLICK_60HZ")

			status = dvpGetAntiFlick(m_handle, AntiFlick)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			AntiFlickModeList.SelectedIndex = CType(AntiFlick, Integer)
		End If
	End Sub


	' Initialize the analog gain.
	Private Sub InitGain()
		Dim fAnalogGain As Single
		Dim AnalogGainDescr As New dvpFloatDescr
		Dim status As dvpStatus

		status = dvpGetAnalogGainDescr(m_handle, AnalogGainDescr)
		If status <> dvpStatus.DVP_STATUS_OK Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			GainEdit.Maximum = CType(AnalogGainDescr.fMax, Decimal)
			GainEdit.Minimum = CType(AnalogGainDescr.fMin, Decimal)
		End If

		status = dvpGetAnalogGain(m_handle, fAnalogGain)
		If status <> dvpStatus.DVP_STATUS_OK Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			GainEdit.Value = CType(fAnalogGain, Decimal)
		End If
	End Sub


	' Initialize the ROI.
	Private Sub InitROI()
		Dim RoiSel As UInteger
		Dim RoiDetail As New dvpQuickRoi
		Dim RoiDescr As New dvpSelectionDescr
		Dim status As dvpStatus
		Dim iNum As UInteger

		ROI_ModeList.Items.Clear()
		' Get the index number of the ROI.
		status = dvpGetQuickRoiSelDescr(m_handle, RoiDescr)
		If status <> dvpStatus.DVP_STATUS_OK Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			For iNum = 0 To RoiDescr.uCount - 1
				status = dvpGetQuickRoiSelDetail(m_handle, iNum, RoiDetail)

				If status <> dvpStatus.DVP_STATUS_OK Then
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				Else
					ROI_ModeList.Items.Insert(iNum, RoiDetail.selection._string)
				End If
			Next
		End If

		' Get the index number of the roi.
		status = dvpGetQuickRoiSel(m_handle, RoiSel)
		If status <> dvpStatus.DVP_STATUS_OK Then
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		Else
			ROI_ModeList.SelectedIndex = RoiSel
		End If
	End Sub


	Private Sub UpdateControls()
		Dim status As dvpStatus

		If IsValidHandle(m_handle) Then

			' The device has been opened at this time.
			Dim state As dvpStreamState

			status = dvpGetStreamState(m_handle, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Update the basic controls.
			If state = dvpStreamState.STATE_STARTED Then
				Start.Text() = "Stop"
			ElseIf state = dvpStreamState.STATE_STOPED Then
				Start.Text() = "Start"
			End If

			Open.Text() = "Close"
			Start.Enabled = True
			button_pro.Enabled = True

			' Update the related controls.
			AE_Enable.Enabled = True
			AE_ModeList.Enabled = True
			AntiFlickModeList.Enabled = True
			ROI_ModeList.Enabled = True

			' Update the AE operation and the AE mode control.
			Dim AeEnable As dvpAeOperation
			Dim AeMode As dvpAeMode

			status = dvpGetAeOperation(m_handle, AeEnable)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			status = dvpGetAeMode(m_handle, AeMode)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			AE_ModeList.SelectedIndex = AeMode

			If AeEnable = dvpAeOperation.AE_OP_OFF Then
				AE_Enable.Checked = False
				ExposureTimeEdit.Enabled = True
				ExposureTimeApply.Enabled = True

				GainEdit.Enabled = True
				GainApply.Enabled = True

			ElseIf AeEnable = dvpAeOperation.AE_OP_ONCE Or AeEnable = dvpAeOperation.AE_OP_CONTINUOUS Then
				AE_Enable.Checked = True
				If AeMode = dvpAeMode.AE_MODE_AG_ONLY Then
					ExposureTimeEdit.Enabled = True
					ExposureTimeApply.Enabled = True
				Else
					ExposureTimeEdit.Enabled = False
					ExposureTimeApply.Enabled = False
				End If

				If AeMode = dvpAeMode.AE_MODE_AE_ONLY Then
					GainEdit.Enabled = True
					GainApply.Enabled = True
				Else
					GainEdit.Enabled = False
					GainApply.Enabled = False
				End If
			End If

			' Update the exposure time.
			Dim fExpoTime As Double
			status = dvpGetExposure(m_handle, fExpoTime)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If fExpoTime > ExposureTimeEdit.Maximum Then
				ExposureTimeEdit.Maximum = fExpoTime
			End If
			ExposureTimeEdit.Value = CType(fExpoTime, Decimal)

			' Update the Anti-Flick control.
			Dim AntiFlick As dvpAntiFlick
			status = dvpGetAntiFlick(m_handle, AntiFlick)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If AntiFlickModeList.Items.Count > AntiFlick Then
				AntiFlickModeList.SelectedIndex = AntiFlick
			End If

			' Update the analog gain.
			Dim fGain As Single
			status = dvpGetAnalogGain(m_handle, fGain)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			GainEdit.Value = CType(fGain, Decimal)

			' Update the ROI.
			Dim unRoiSel As UInteger
			status = dvpGetQuickRoiSel(m_handle, unRoiSel)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			ROI_ModeList.SelectedIndex = unRoiSel

			' Update the exposure time and gain.
			InitExpoTime()
			InitGain()
		Else
			' No device is opened at this time.
			Open.Text() = "Open"
			Start.Enabled = False
			button_pro.Enabled = False
			Save.Enabled = False

			If DevicesList.Items.Count = 0 Then

				' No device exists.
				Open.Enabled = False
			Else
				Open.Enabled = True
			End If

			AE_Enable.Enabled = False
			AE_ModeList.Enabled = False
			ExposureTimeEdit.Enabled = False
			ExposureTimeApply.Enabled = False
			AntiFlickModeList.Enabled = False
			GainEdit.Enabled = False
			GainApply.Enabled = False
			ROI_ModeList.Enabled = False
			PictureBox.Image = Nothing
		End If

		InitAWB()
	End Sub


	' Enumerate the number of cameras that has been connected to a computer.
	Private Sub Scan_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Scan.Click
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
		status = dvpRefresh(iCount)
		Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

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
	End Sub


	Private Sub Open_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Open.Click
		OpenCamera()
	End Sub


	Private Sub OpenCamera()
		Dim status As dvpStatus
		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture)

		If IsValidHandle(m_handle) Then

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
			' close camera

			If dvpStatus.DVP_STATUS_OK = dvpClose(m_handle) Then
				m_handle = 0
				Save.Enabled = False
			Else
				MessageBox.Show("Close video stream fail!")
			End If

			PictureBox.Refresh()
		Else

			If DevicesList.Text <> "" Then

				If UserDefinedName.Checked Then
					' Open the specific device by the selected user define name.
					status = dvpOpenByUserId(DevicesList.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				Else
					' Open the specific device by the selected FriendlyName.
					status = dvpOpenByName(DevicesList.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				End If

				If dvpStatus.DVP_STATUS_OK = status Then
					m_strFriendlyName = DevicesList.Text

					' If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
					' Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
					status = dvpRegisterStreamCallback(m_handle, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

					' Initialize the auto exposure mode, the exposure time, the anti-flick, the analog gain, the ROI.
					InitAntiFlick()
					InitROI()
					InitAeMode()
					InitExpoTime()
					InitGain()
				Else
					MessageBox.Show("Open the device failed!")
				End If
			End If
		End If

		UpdateControls()
	End Sub


	Private Sub Start_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Start.Click
		StartCamera()
	End Sub


	Private Sub StartCamera()
		Dim state As dvpStreamState
		Dim status As dvpStatus

		m_uDisplayCount = 0

		If IsValidHandle(m_handle) Then

			' Implement a button to start and stop according to the current video's status.
			status = dvpGetStreamState(m_handle, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If dvpStatus.DVP_STATUS_OK = status Then
				If state = dvpStreamState.STATE_STARTED Then
					status = dvpStop(m_handle)
					If status <> dvpStatus.DVP_STATUS_OK Then
						MessageBox.Show("Stop video stream fail!")
					End If
					Save.Enabled = False
				Else
					m_bSaveImageFlg = False
					status = dvpStart(m_handle)
					Save.Enabled = True
					If status <> dvpStatus.DVP_STATUS_OK Then
						MessageBox.Show("Start video stream fail!")
					Else
						SetExpoTimeSlider()
					End If

				End If

			End If
		End If

		UpdateControls()
	End Sub


	Private Sub button_pro_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles button_pro.Click
		SetPropertyPage()
	End Sub


	Private Sub SetPropertyPage()
		Dim status As dvpStatus
		If IsValidHandle(m_handle) Then
			status = dvpShowPropertyModalDialog(m_handle, Me.Handle)

			' At this time some configurations may change, synchronize it to the window GUI.
			UpdateControls()
		End If
	End Sub

	Private Sub Save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Save.Click
		Save_Image()
	End Sub


	Private Sub Save_Image()
		Dim SaveDialog As New SaveFileDialog()
		
		SaveDialog.Filter = "bmp file(*.bmp)|*.bmp|jpeg file(*.jpeg)|*.jpeg|png file(*.png)|*.png|tif file(*.tif)|*.tif|gif file(*.gif)|*.gif|raw Files(*.dat)|*.dat"

		If (SaveDialog.ShowDialog() = DialogResult.OK) Then
			m_ImagePath = SaveDialog.FileName
			m_bSaveImageFlg = True

		End If
	End Sub


	Private Sub AE_Enable_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AE_Enable.CheckedChanged
		SetAeEnable()
	End Sub


	Private Sub SetAeEnable()
		Dim status As dvpStatus

		If AE_Enable.Checked = True Then
			status = dvpSetAeOperation(m_handle, dvpAeOperation.AE_OP_CONTINUOUS)
		Else
			status = dvpSetAeOperation(m_handle, dvpAeOperation.AE_OP_OFF)
		End If

		Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		UpdateControls()
	End Sub


	Private Sub AE_ModeList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AE_ModeList.SelectedIndexChanged
		SetAeMode()
	End Sub


	Private Sub SetAeMode()
		Dim status As dvpStatus
		Dim iNum As Integer

		If IsValidHandle(m_handle) Then

			iNum = AE_ModeList.SelectedIndex
			If iNum > 3 Then
				Return
			End If

			If iNum = 0 Then
				status = dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_AG)

			ElseIf iNum = 1 Then
				status = dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_AE)

			ElseIf iNum = 2 Then
				status = dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_ONLY)

			ElseIf iNum = 3 Then
				status = dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_ONLY)
			End If

			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			UpdateControls()
		End If
	End Sub


	Private Sub ExpoTimeSlider_Scroll(ByVal sender As System.Object, ByVal e As System.EventArgs)
		SetExpoTimeSlider()
	End Sub


	Private Sub SetExpoTimeSlider()
		Dim fExpoTime As Double
		Dim ExpoDescr As New dvpDoubleDescr
		Dim status As dvpStatus

		If IsValidHandle(m_handle) Then

			' Get the value of the exposure time slider.
			fExpoTime = CType(ExposureTimeEdit.Value, Double)

			' Set the exposure time. 
			' Get the range of the exposure time,in order to avoid that the set value is out of range.
			status = dvpGetExposureDescr(m_handle, ExpoDescr)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If fExpoTime > ExpoDescr.fMax - 1 Then
				fExpoTime = ExpoDescr.fMax - 1
			End If
			If fExpoTime < ExpoDescr.fMin + 1 Then
				fExpoTime = ExpoDescr.fMin + 1
			End If

			status = dvpSetExposure(m_handle, fExpoTime)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Get the value of the exposure time again,there are differences between the set value and the obtained value for the reason of accuracy(step value),
			' it is subject to the obtained value.
			status = dvpGetExposure(m_handle, fExpoTime)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Set the text of the exposure time.
			ExposureTimeEdit.Value = CType(fExpoTime, Decimal)
		End If

	End Sub


	Private Sub AntiFlickModeList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AntiFlickModeList.SelectedIndexChanged
		SetAntiFlickMode()
	End Sub


	Private Sub SetAntiFlickMode()
		Dim status As dvpStatus
		Dim iNum As Integer

		If IsValidHandle(m_handle) Then

			iNum = AntiFlickModeList.SelectedIndex

			If iNum > 2 Then
				Return
			End If

			If AntiFlickModeList.Items.Count <> 3 Then
				Return
			End If

			If iNum = 0 Then
				status = dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_DISABLE)
			ElseIf iNum = 1 Then
				status = dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_50HZ)
			ElseIf iNum = 2 Then
				status = dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_60HZ)
			End If

			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
	End Sub


	Private Sub SetGainSlider()
		Dim fGain As Single
		Dim status As dvpStatus

		If IsValidHandle(m_handle) Then

			' Get the value of the analog gain slider.
			fGain = GainEdit.Value

			' Firstly,set the value of the analog gain.
			status = dvpSetAnalogGain(m_handle, fGain)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Secondly,Get the value of the analog gain,there are differences between the set value and the obtained value for the reason of accuracy(step value),
			' it is subject to the obtained value.
			status = dvpGetAnalogGain(m_handle, fGain)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Set the text of the analog gain.
			GainEdit.Value = fGain

		End If
	End Sub


	Private Sub ROI_ModeList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ROI_ModeList.SelectedIndexChanged
		SetROIMode()
		ResizeWindows()
	End Sub


	Private Sub SetROIMode()
		Dim status As dvpStatus
		Dim iNum As Integer

		iNum = ROI_ModeList.SelectedIndex
		If iNum < 0 Then
			Return
		End If

		If IsValidHandle(m_handle) = False Then
			Return
		End If

		Dim state As dvpStreamState
		status = dvpGetStreamState(m_handle, state)
		Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		If state = dvpStreamState.STATE_STARTED Then
			status = dvpStop(m_handle)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Stop the video stream fail!")
				Return
			End If
		End If

		' Set the ROI mode
		status = dvpSetQuickRoiSel(m_handle, iNum)
		Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		If state = dvpStreamState.STATE_STARTED Then
			status = dvpStart(m_handle)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Start the video stream fail!")
				Return
			End If
		End If
		InitExpoTime()
		InitGain()
	End Sub


	Private Sub basicfunction_FormClosing(sender As System.Object, e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing

		' Close the camera before exitting.
		Dim status As dvpStatus
		If IsValidHandle(m_handle) Then
			status = dvpStop(m_handle)
			status = dvpClose(m_handle)
			m_handle = 0
		End If


	End Sub


	Private Sub ExposureTimeApply_Click(sender As System.Object, e As System.EventArgs) Handles ExposureTimeApply.Click
		SetExpoTimeSlider()
	End Sub


	Private Sub GainApply_Click(sender As System.Object, e As System.EventArgs) Handles GainApply.Click
		SetGainSlider()
	End Sub


	' Init the anto white balance.
	Private Sub InitAWB()

		Dim status As dvpStatus
		Dim AwbOperation As dvpAwbOperation = New dvpAwbOperation()

		If (IsValidHandle(m_handle)) Then

			' Get the current white balance property.
			status = DVPCamera.dvpGetAwbOperation(m_handle, AwbOperation)
			If (status <> dvpStatus.DVP_STATUS_OK) Then

				' Auto white balance is not supported or get fails.
				AWBEnable.Enabled = False
				AWBOnce.Enabled = False
				Return
			Else
				' Set the control
				AWBEnable.Enabled = True

				If AwbOperation <> dvpAwbOperation.AWB_OP_CONTINUOUS Then
					AWBEnable.Checked = False
					AWBOnce.Enabled = True
				Else
					AWBEnable.Checked = True
					AWBOnce.Enabled = False
				End If
			End If
		Else

			AWBEnable.Enabled = False
			AWBOnce.Enabled = False
		End If
	End Sub


	Private Sub AWBEnable_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AWBEnable.CheckedChanged
		Dim status As dvpStatus
		Dim AwbOperation As dvpAwbOperation = New dvpAwbOperation()

		If (IsValidHandle(m_handle)) Then

			' Auto white balance,set to AWB_OP_CONTINUOUS stands for continuous operation. 
			If (AWBEnable.Checked) Then
				status = DVPCamera.dvpGetAwbOperation(m_handle, AwbOperation)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				If (AwbOperation <> dvpAwbOperation.AWB_OP_CONTINUOUS) Then
					status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_CONTINUOUS)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
			Else
				' Close auto white balance.
				status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_OFF)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			End If
			InitAWB()
		End If
	End Sub


	Private Sub AWBOnce_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AWBOnce.Click
		Dim status As dvpStatus

		If (IsValidHandle(m_handle)) Then

			' Auto white balance,set to AWB_OP_ONCE stands for operating once. 
			status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_ONCE)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' The RGB gain is set to 1.00, avoiding a superimposed gain effect
			Dim bGainState As Boolean
			status = dvpGetRgbGainState(m_handle, bGainState)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If status = dvpStatus.DVP_STATUS_OK Then
				If (bGainState) Then
					status = dvpSetRgbGain(m_handle, 1.0F, 1.0F, 1.0F)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				Else
					status = dvpSetRgbGainState(m_handle, True)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
					status = dvpSetRgbGain(m_handle, 1.0F, 1.0F, 1.0F)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
					status = dvpSetRgbGainState(m_handle, bGainState)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

				End If
			End If

			InitAWB()
		End If
	End Sub

	Private Sub ResizeWindows()
		If (IsValidHandle(m_handle)) Then

			Dim roi As New dvpRegion
			Dim status As dvpStatus
			status = dvpGetRoi(m_handle, roi)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			PictureBox.Width = MyBase.Width - PictureBox.Left
			PictureBox.Height = MyBase.Height - PictureBox.Top

			If PictureBox.Width * roi.H > PictureBox.Height * roi.W Then
				PictureBox.Width = PictureBox.Height * roi.W / roi.H
			Else
				PictureBox.Height = PictureBox.Width * roi.H / roi.W
			End If
		End If
	End Sub

	Private Sub basicfunction_Resize(sender As System.Object, e As System.EventArgs) Handles MyBase.Resize
		ResizeWindows()
	End Sub

	Private Sub UserDefineName_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles UserDefinedName.CheckedChanged

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

