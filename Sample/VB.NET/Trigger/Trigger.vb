Imports DVPCameraType
Imports DVPCameraType.DVPCamera
Imports System.Runtime.InteropServices
Imports Microsoft.VisualBasic

Public Class Trigger
	Dim m_handle As UInteger
	Dim m_bAeOp As Boolean
	Dim m_n_dev_count As Integer
	Dim m_strFriendlyName As String

	Dim m_ptr_wnd As IntPtr
	Dim m_b_start As Boolean

	Dim m_DelayDescr As New dvpDoubleDescr()
	Dim m_FilterDescr As New dvpDoubleDescr()
	Dim m_LoopDescr As New dvpDoubleDescr()

	Dim m_TriggerDelay As Double
	Dim m_TriggerFilter As Double
    Dim m_TriggerLoop As Double
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

	Private Sub InitDevList()
		Dim status As dvpStatus
		Dim i As UInteger
		Dim n As UInteger

		Dim dev_info As New dvpCameraInfo()
        DevNameCombo.Items.Clear()

        ' Get the number of cameras that has been connected to a computer.
		status = DVPCamera.dvpRefresh(n)
		Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		m_n_dev_count = n
		If m_n_dev_count = 0 Then
			Return
		End If
		If status = dvpStatus.DVP_STATUS_OK Then
			For i = 0 To n - 1

				' Acquire each camera's information one by one.
				status = DVPCamera.dvpEnum(i, dev_info)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				If (status = dvpStatus.DVP_STATUS_OK) Then

					' GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
					Dim item As Integer
					m_info(m_CamCount) = dev_info
					m_CamCount += 1

					If UserDefinedName.Checked Then
						item = DevNameCombo.Items.Add(dev_info.UserID)
					Else
						item = DevNameCombo.Items.Add(dev_info.FriendlyName)
					End If

					If (item = 0) Then
						DevNameCombo.SelectedIndex = item
					End If
				End If
			Next
		End If

		If (n = 0) Then
			OpenDev.Enabled = False
		Else
			OpenDev.Enabled = True
		End If
		InitCtrls()
	End Sub

    ' Set the status of controls
	Private Sub InitCtrls()
		Dim status As dvpStatus

        If (IsValidHandle(m_handle)) Then

            ' The device has been opened at this time.
            Dim state As New dvpStreamState()
			status = DVPCamera.dvpGetStreamState(m_handle, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
            OpenDev.Text = "Close"
            If state = dvpStreamState.STATE_STARTED Then
                StartPlay.Text = "Stop"
            Else
                StartPlay.Text = "Start"
            End If

            StartPlay.Enabled = True
            PropertSet.Enabled = True

            ' Enable the related controls.
			LoopTrigger.Enabled = True
            EnableIn.Enabled = True
            EnableOut.Enabled = True
            SoftTriggerFire.Enabled = True


            ' Update the window that is related to trigger function.
            Dim bTrig As Boolean
            Dim bLoop As Boolean
            bTrig = False
            bLoop = False

            ' Get the enable state of trigger mode.
			status = DVPCamera.dvpGetTriggerState(m_handle, bTrig)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			TriggerMode.Checked = bTrig
			If (status <> dvpStatus.DVP_STATUS_OK) Then

                ' Judge whether the trigger function can be supported by camera.
				TriggerDelay.Enabled = False
				Delay.Enabled = False
				ApplyDelay.Enabled = False
				SoftTriggerFire.Enabled = False
			Else
				TriggerDelay.Enabled = bTrig
				Delay.Enabled = bTrig
				ApplyDelay.Enabled = bTrig
				TriggerMode.Checked = bTrig
			End If

			TriggerMode.Enabled = state <> dvpStreamState.STATE_STARTED

            LoopTrigger.Enabled = bTrig

            ' Get the enable state of loop trigger.
			bLoop = False
			status = DVPCamera.dvpGetSoftTriggerLoopState(m_handle, bLoop)
			LoopTrigger.Checked = bLoop
			If status = dvpStatus.DVP_STATUS_OK Then
				SoftTriggerFire.Enabled = Not (bLoop) And bTrig
				If bLoop And bTrig Then
					LoopTimer.Enabled = True
					TriggerLoop.Enabled = True
					ApplyLoop.Enabled = True
				Else
					LoopTimer.Enabled = False
					TriggerLoop.Enabled = False
					ApplyLoop.Enabled = False
				End If
			Else
				LoopTrigger.Enabled = False
				LoopTimer.Enabled = True
				TriggerLoop.Enabled = True
				ApplyLoop.Enabled = True

			End If

			' Update the external trigger input status of the related IO.
			Dim bExtTrigIn As Boolean
			bExtTrigIn = False
			Dim UserIoInfo As New dvpUserIoInfo()
			Dim InIoFunc As New dvpInputIoFunction()

			' Enumerate all UserIO,and check whether the IO is set as the trigger input function.
			status = DVPCamera.dvpGetUserIoInfo(m_handle, UserIoInfo)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			InputIOCombo.Items.Clear()
			Dim nLength As Integer
			nLength = IIf(8 < UserIoInfo.inputValid.Length, 8, UserIoInfo.inputValid.Length)


			Dim c As Char

			For i = 0 To nLength - 1
				If UserIoInfo.inputValid(i) > c Then
					Dim str As String
					Dim InputIo As dvpInputIo
					InputIo = i + dvpInputIo.INPUT_IO_1

					' Add the IO name to the drop-down list.
					Dim nInItem As Integer
					nInItem = i + 1
					str = "IN_" + nInItem.ToString()
					Dim item As Integer
					item = InputIOCombo.Items.Add(str)

					' Check whether there is the input IO is set as the trigger input function. 
					status = DVPCamera.dvpGetInputIoFunction(m_handle, InputIo, InIoFunc)
					If (bExtTrigIn <> True And InIoFunc = dvpInputIoFunction.INPUT_FUNCTION_TRIGGER) Then

						' Find the IO that is set as the trigger input signal.
						bExtTrigIn = True

						' Select the IO that is set as the trigger input function in drop-down list.
						InputIOCombo.SelectedIndex = i
					End If
				End If
			Next

			If bExtTrigIn Then

            ' Indicate that the trigger input signal have been used on the window GUI.
				EnableIn.Checked = True
				InputIOCombo.Enabled = True
			Else

				' No IO is set as the trigger input function, the first one in the drop-down list is selected as the default. 
				If (InputIOCombo.Items.Count > 0) Then
					InputIOCombo.SelectedIndex = 0
				End If

				EnableIn.Checked = False
			End If

			InputIOCombo.Enabled = bExtTrigIn
			InputSignalTypeCombo.Enabled = bExtTrigIn
			EditFilter.Enabled = bExtTrigIn
			FilterApply.Enabled = bExtTrigIn

            ' Add the signal type of the trigger input to the list.
			InputSignalTypeCombo.Items.Clear()
			InputSignalTypeCombo.Items.Add(("Off"))
			InputSignalTypeCombo.Items.Add(("Falling Edge"))
			InputSignalTypeCombo.Items.Add(("Low Level"))
			InputSignalTypeCombo.Items.Add(("Rising Edge"))
			InputSignalTypeCombo.Items.Add(("High Level"))

			Dim TriggerType As dvpTriggerInputType
			status = DVPCamera.dvpGetTriggerInputType(m_handle, TriggerType)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			InputSignalTypeCombo.SelectedIndex = TriggerType

            ' Update the window GUI of strobe function.
			Dim bExtStrobeOut As Boolean
			bExtStrobeOut = False
			Dim OutIoFunc As dvpOutputIoFunction

            ' Enumerate all UserIO, and check whether there is IO is set as the strobe output function.
			OutIOCombo.Items.Clear()


			nLength = IIf(8 < UserIoInfo.outputValid.Length, 8, UserIoInfo.outputValid.Length)
			For i = 0 To nLength - 1

				If UserIoInfo.outputValid(i) > c Then
					Dim str1 As String
					Dim OutputIo As dvpOutputIo
					OutputIo = (i + dvpOutputIo.OUTPUT_IO_1)

					Dim nOutItem As Integer
					nOutItem = i + 1
					str1 = "OUT_" + nOutItem.ToString()
					OutIOCombo.Items.Add(str1)

					' Check whether there is the output IO is set as the strobe output function.
					status = DVPCamera.dvpGetOutputIoFunction(m_handle, OutputIo, OutIoFunc)
					' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

					If (bExtStrobeOut = False And OutIoFunc = dvpOutputIoFunction.OUTPUT_FUNCTION_STROBE) Then

						' Find the IO that is set as the strobe output function.
						bExtStrobeOut = True

						' Select the IO that is set as the strobe output function in the drop-down list.
						OutIOCombo.SelectedIndex = i
					End If
				End If
			Next

			If bExtStrobeOut Then
				EnableOut.Checked = True
			Else

				' No IO is set as the strobe output function, the first one in the drop-down list is selected as the default. 
				If (OutIOCombo.Items.Count > 0) Then
					OutIOCombo.SelectedIndex = 0
				Else
					OutIOCombo.Items.Add("OUT_1")
					OutIOCombo.SelectedIndex = 0
				End If

				EnableOut.Checked = False
			End If

			OutputSignalType.Enabled = bExtStrobeOut
			OutIOCombo.Enabled = bExtStrobeOut

			' Add the signal type of the strobe output to the list. 
			OutputSignalType.Items.Clear()
			OutputSignalType.Items.Add(("Off"))
			OutputSignalType.Items.Add(("Low Level"))
			OutputSignalType.Items.Add(("High Level"))

			' Get the signal type of the strobe.
			Dim StrobeType As dvpStrobeOutputType
			status = DVPCamera.dvpGetStrobeOutputType(m_handle, StrobeType)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			OutputSignalType.SelectedIndex = StrobeType

			' The following descriptions of the information will be used to update the range of values in the edit box.
			status = DVPCamera.dvpGetTriggerDelayDescr(m_handle, m_DelayDescr)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			TriggerDelay.Maximum = CType(m_DelayDescr.fMax, Decimal)
			TriggerDelay.Minimum = CType(m_DelayDescr.fMin, Decimal)

			status = DVPCamera.dvpGetTriggerJitterFilterDescr(m_handle, m_FilterDescr)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			EditFilter.Maximum = CType(m_FilterDescr.fMax, Decimal)
			EditFilter.Minimum = CType(m_FilterDescr.fMin, Decimal)

			status = DVPCamera.dvpGetSoftTriggerLoopDescr(m_handle, m_LoopDescr)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			TriggerLoop.Maximum = CType(m_LoopDescr.fMax, Decimal)
			TriggerLoop.Minimum = CType(m_LoopDescr.fMin, Decimal)

			' Update values in the edit box. 
			status = DVPCamera.dvpGetSoftTriggerLoop(m_handle, m_TriggerLoop)
			If m_TriggerLoop > m_LoopDescr.fMax Then
				m_TriggerLoop = m_LoopDescr.fMax
			ElseIf m_TriggerLoop < m_LoopDescr.fMin Then
				m_TriggerLoop = m_LoopDescr.fMin
			End If
			TriggerLoop.Value = CType(m_TriggerLoop, Decimal)
			
			status = DVPCamera.dvpGetTriggerDelay(m_handle, m_TriggerDelay)
			' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If m_TriggerDelay > m_DelayDescr.fMax Then
				m_TriggerDelay = m_DelayDescr.fMax
			ElseIf m_TriggerDelay < m_DelayDescr.fMin Then
				m_TriggerDelay = m_DelayDescr.fMin
			End If
			TriggerDelay.Value = CType(m_TriggerDelay, Decimal)

			status = DVPCamera.dvpGetTriggerJitterFilter(m_handle, m_TriggerFilter)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If m_TriggerFilter < 1 Then
				m_TriggerFilter = 1
			End If

			If m_TriggerFilter > m_FilterDescr.fMax Then
				m_TriggerFilter = m_FilterDescr.fMax
			ElseIf m_TriggerFilter < m_FilterDescr.fMin Then
				m_TriggerFilter = m_FilterDescr.fMin
			End If

			EditFilter.Value = CType(m_TriggerFilter, Decimal)
		Else
            ' No device is opened at this time.
            ' Update the basic controls.
			OpenDev.Text = "Open"
			StartPlay.Enabled = False
			PropertSet.Enabled = False
			If (DevNameCombo.Items.Count = 0) Then

				' No device exists.
				OpenDev.Enabled = False
			Else
				OpenDev.Enabled = True
			End If

			' Update the related controls.
			InputIOCombo.Enabled = False
			InputSignalTypeCombo.Enabled = False
			OutputSignalType.Enabled = False
			OutIOCombo.Enabled = False
			EditFilter.Enabled = False
			TriggerDelay.Enabled = False
			TriggerLoop.Enabled = False
			TriggerMode.Enabled = False
			LoopTrigger.Enabled = False
			EnableIn.Enabled = False
			EnableOut.Enabled = False
			SoftTriggerFire.Enabled = False
			ApplyLoop.Enabled = False
			ApplyDelay.Enabled = False
			FilterApply.Enabled = False
			Delay.Enabled = False
		End If
	End Sub

    ' Callback function.
	Public Function _dvpStreamCallback(ByVal handle As UInteger, ByVal _event As dvpStreamEvent, ByVal pContext As IntPtr, ByRef refFrame As dvpFrame, ByVal pBuffer As IntPtr) As Integer
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

		If bDisplay Then

			m_uDisplayCount += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.
			Dim status As dvpStatus = New dvpStatus()
			status = DVPCamera.dvpDrawPicture(refFrame, pBuffer, m_ptr_wnd, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
		Return 0
	End Function

	Private Sub OpenCamera()
		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf _dvpStreamCallback)
		Dim status As dvpStatus
		status = dvpStatus.DVP_STATUS_OK
		Dim strName As String
		strName = ""

		If IsValidHandle(m_handle) = False Then
			strName = DevNameCombo.Text
            If (strName <> "") Then

				If UserDefinedName.Checked Then
					' Open the specific device by the selected user define name.
					status = dvpOpenByUserId(DevNameCombo.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				Else
					' Open the specific device by the selected FriendlyName.
					status = dvpOpenByName(DevNameCombo.Text, dvpOpenMode.OPEN_NORMAL, m_handle)
				End If

                If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed")
                Else
                    m_strFriendlyName = strName

                    ' If it needs to display images ,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                    ' Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
					status = DVPCamera.dvpRegisterStreamCallback(m_handle, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
                End If
            End If
		Else

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

			status = DVPCamera.dvpClose(m_handle)
			m_handle = 0
			pictureBox.Refresh()

		End If
		InitCtrls()
	End Sub

    'Play the video stream.
	Private Sub StartCamera()
		If IsValidHandle(m_handle) Then
			Dim state As dvpStreamState
            Dim status As dvpStatus

            ' Implement a button to start and stop according to the current video's status.
			status = DVPCamera.dvpGetStreamState(m_handle, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle)
			Else
				status = DVPCamera.dvpStart(m_handle)
			End If
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
		InitCtrls()
	End Sub

	Private Sub SetParam()
		If IsValidHandle(m_handle) = True Then
			Dim status As dvpStatus
            status = DVPCamera.dvpShowPropertyModalDialog(m_handle, Me.Handle())

            ' At this time some configurations may change, synchronize it to the window GUI.
			InitCtrls()
		End If
	End Sub

	Private Sub Trigger_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
		m_handle = 0
		m_bAeOp = False
		m_n_dev_count = 0
		m_strFriendlyName = ""
		m_ptr_wnd = 0
		m_b_start = False
		m_TriggerDelay = 0.0
		m_TriggerFilter = 0.0
		m_TriggerLoop = 0.0
        m_ptr_wnd = pictureBox.Handle
        m_Timer.Interval = 500
        m_Timer.Enabled = True


		' Initialize open mode
		' false: user dvpOpenByName open the camear
		' true : user dvpOpenByUserId open the camear
		UserDefinedName.Checked = False

		InitDevList()

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
            End If

			If m_uDisplayCount = 0 Or m_Stopwatch.ElapsedMilliseconds < 2 Or TriggerMode.Checked Then
				str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount * 1000.0F / m_Stopwatch.ElapsedMilliseconds) + " fps]"
			End If

			Me.Text = str
        End If
    End Sub


	Private Sub Trigger_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then
			status = DVPCamera.dvpStop(m_handle)
			status = DVPCamera.dvpClose(m_handle)
			m_handle = 0
		End If
	End Sub

	Private Sub ScanDev_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev.Click
		InitDevList()
	End Sub

	Private Sub OpenDev_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev.Click
		OpenCamera()
	End Sub

	Private Sub StartPlay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay.Click
		StartCamera()
	End Sub

	Private Sub PropertSet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertSet.Click
		SetParam()
	End Sub

	Private Sub TriggerMode_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TriggerMode.CheckedChanged

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
			status = dvpSetTriggerState(m_handle, TriggerMode.Checked)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			' Start the video stream.
			If StreamState = dvpStreamState.STATE_STARTED Then
				status = dvpStart(m_handle)
				Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			End If

			InitCtrls()

		End If
	End Sub

	Private Sub SoftTriggerFire_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SoftTriggerFire.Click
		Dim status As dvpStatus
        If (IsValidHandle(m_handle)) Then

            ' Once execution of this function is equivalent to the generation of an external trigger.
            ' Note:If the exposure time is very long, clicking on the "Soft Trigger the Fire" too quick may cause the failure of the trigger,
            ' because the previous frame probably in a state of exposing continuously or output incompletely.
            status = DVPCamera.dvpTriggerFire(m_handle)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
        End If
	End Sub

	Private Sub LoopTrigger_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles LoopTrigger.CheckedChanged
		Dim status As dvpStatus
		status = DVPCamera.dvpSetSoftTriggerLoopState(m_handle, LoopTrigger.Checked)
		InitCtrls()
	End Sub

	Private Sub ApplyLoop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ApplyLoop.Click
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then
			m_TriggerLoop = CType(TriggerLoop.Value, Double)

			status = DVPCamera.dvpSetSoftTriggerLoop(m_handle, m_TriggerLoop)
            ' Debug.Assert(status=dvpStatus.DVP_STATUS_OK)

			status = DVPCamera.dvpGetSoftTriggerLoop(m_handle, m_TriggerLoop)
			If status = dvpStatus.DVP_STATUS_OK Then
				TriggerLoop.Value = CType(m_TriggerLoop, Decimal)
			End If
		End If
	End Sub

	Private Sub ApplyDelay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ApplyDelay.Click
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then
			m_TriggerDelay = CType(TriggerDelay.Value, Double)

			status = DVPCamera.dvpSetTriggerDelay(m_handle, m_TriggerDelay)
            ' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            status = DVPCamera.dvpGetTriggerDelay(m_handle, m_TriggerDelay)
            If status = dvpStatus.DVP_STATUS_OK Then
				TriggerDelay.Value = CType(m_TriggerDelay, Decimal)
            End If
        End If
	End Sub

	Private Sub EnableInCheck()
		If (IsValidHandle(m_handle)) Then
			Dim UserIoInfo As New dvpUserIoInfo()
			Dim i As Integer
			Dim status As dvpStatus
			status = DVPCamera.dvpGetUserIoInfo(m_handle, UserIoInfo)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set all INPUT IO as the Normal mode
            Dim nInLength As Integer
            nInLength = IIf(8 < UserIoInfo.inputValid.Length, 8, UserIoInfo.inputValid.Length)

			Dim c As Char

			For i = 0 To nInLength - 1
				If UserIoInfo.inputValid(i) > c Then
					status = DVPCamera.dvpSetInputIoFunction(m_handle, (dvpInputIo.INPUT_IO_1 + i), dvpInputIoFunction.INPUT_FUNCTION_NORMAL)
				End If
			Next

			' If ExtTriggerIn is enable,set the Input Io as the trigger input function. 
			If (EnableIn.Checked) Then
				Dim j As Integer
				j = InputIOCombo.SelectedIndex
				Dim InputIo As dvpInputIo

				If (j >= 0) Then
					InputIo = j + dvpInputIo.INPUT_IO_1
					status = DVPCamera.dvpSetInputIoFunction(m_handle, InputIo, dvpInputIoFunction.INPUT_FUNCTION_TRIGGER)
				End If

			End If
		End If
	End Sub
       
	Private Sub EnableIn_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnableIn.CheckedChanged
		EnableInCheck()
		InitCtrls()
	End Sub

	Private Sub InputIOCombo_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InputIOCombo.SelectedIndexChanged
        If (IsValidHandle(m_handle)) Then

            ' Changing the trigger input IO and enabling the trigger input function is the same process.
            EnableInCheck()
        End If
	End Sub

	Private Sub InputSignalTypeCombo_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InputSignalTypeCombo.SelectedIndexChanged
		Dim status As dvpStatus = New dvpStatus()
		If (IsValidHandle(m_handle)) Then
			Dim TriggerType As dvpTriggerInputType
			TriggerType = (dvpTriggerInputType.TRIGGER_IN_OFF + InputSignalTypeCombo.SelectedIndex)
			' status = DVPCamera.dvpSetTriggerInputType(m_handle, TriggerType)
		End If
	End Sub

	Private Sub EnableOutCheck()
		If (IsValidHandle(m_handle)) Then
			Dim UserIoInfo As New dvpUserIoInfo()
			Dim i As Integer
			Dim status As dvpStatus
			status = DVPCamera.dvpGetUserIoInfo(m_handle, UserIoInfo)
			' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set all OUTPUT IO as the Normal mode  
            Dim nOutLength As Integer
            nOutLength = IIf(8 < UserIoInfo.outputValid.Length, 8, UserIoInfo.outputValid.Length)

			Dim c As Char

			For i = 0 To nOutLength - 1
				If UserIoInfo.outputValid(i) > c Then
					status = DVPCamera.dvpSetOutputIoFunction(m_handle, (dvpOutputIo.OUTPUT_IO_1 + i), dvpOutputIoFunction.OUTPUT_FUNCTION_NORMAL)
				End If

                ' If ExtStrobeOut is enable,set the Output Io as the strobe output function. 
				If (EnableOut.Checked) Then
					Dim j As Integer
					j = OutIOCombo.SelectedIndex
					Dim OutputIo As New dvpOutputIo()
					If (j >= 0) Then
						OutputIo = (j + dvpOutputIo.OUTPUT_IO_1)
						status = DVPCamera.dvpSetOutputIoFunction(m_handle, OutputIo, dvpOutputIoFunction.OUTPUT_FUNCTION_STROBE)
						' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
					End If
				End If
			Next
		End If
	End Sub

	Private Sub OutIOCombo_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OutIOCombo.SelectedIndexChanged
        If IsValidHandle(m_handle) Then

            ' Changing the strobe output IO and enabling the strobe output function is the same process.
            EnableOutCheck()
        End If
	End Sub

	Private Sub OutputSignalType_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OutputSignalType.SelectedIndexChanged
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then
			Dim StrobeType As dvpStrobeOutputType
			StrobeType = dvpStrobeOutputType.STROBE_OUT_OFF + OutputSignalType.SelectedIndex
			status = DVPCamera.dvpSetStrobeOutputType(m_handle, StrobeType)
		End If
	End Sub

	Private Sub EnableOut_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles EnableOut.CheckedChanged
		EnableOutCheck()
		InitCtrls()
	End Sub

	Private Sub FilterApply_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FilterApply.Click
		Dim status As dvpStatus
		If (IsValidHandle(m_handle)) Then

			m_TriggerFilter = CType(EditFilter.Value, Double)

			status = DVPCamera.dvpSetTriggerJitterFilter(m_handle, m_TriggerFilter)
			' Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			status = DVPCamera.dvpGetTriggerJitterFilter(m_handle, m_TriggerFilter)
			If status = dvpStatus.DVP_STATUS_OK Then
				EditFilter.Value = CType(m_TriggerFilter, Decimal)
			End If
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

	Private Sub Trigger_Resize(sender As System.Object, e As System.EventArgs) Handles MyBase.Resize
		ResizeWindows()
	End Sub

	Private Sub UserDefinedName_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles UserDefinedName.CheckedChanged
		' save cur sel item
		Dim strName As String
		strName = DevNameCombo.Text
		' reset m_listDevices values
		DevNameCombo.Items.Clear()
		For i = 0 To m_CamCount - 1

			Dim item As Integer
			item = -1

			If UserDefinedName.Checked = False Then

				item = DevNameCombo.Items.Add(m_info(i).FriendlyName)
				If (strName = m_info(i).UserID) Then
					DevNameCombo.SelectedIndex = item
				End If

			Else
				' check User define name is null
				If m_info(i).UserID.Length = 0 Then
					Continue For
				End If

				item = DevNameCombo.Items.Add(m_info(i).UserID)
				If strName = m_info(i).FriendlyName Then
					DevNameCombo.SelectedIndex = item
				End If

			End If
		Next
	End Sub
End Class


