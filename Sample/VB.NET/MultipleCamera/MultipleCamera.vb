Imports DVPCameraType
Imports DVPCameraType.DVPCamera
Imports System.Runtime.InteropServices

Public Class MultipleCamera

    Dim m_handle0 As UInteger
    Dim m_handle1 As UInteger
    Dim m_handle2 As UInteger
    Dim m_handle3 As UInteger

    Dim m_PtrWnd0 As IntPtr
    Dim m_PtrWnd1 As IntPtr
    Dim m_PtrWnd2 As IntPtr
	Dim m_PtrWnd3 As IntPtr

	Dim m_dev_info0(16) As dvpCameraInfo
	Dim m_dev_info1(16) As dvpCameraInfo
	Dim m_dev_info2(16) As dvpCameraInfo
	Dim m_dev_info3(16) As dvpCameraInfo

	Private WithEvents m_Timer0 As New Timer
	Private WithEvents m_Timer1 As New Timer
	Private WithEvents m_Timer2 As New Timer
	Private WithEvents m_Timer3 As New Timer

	'Display param
	Public m_Stopwatch(4) As Stopwatch
	Public m_uDisplayCount(4) As UInteger

	Public Sub theout0(ByVal sender As Object, ByVal e As EventArgs) Handles m_Timer0.Tick
		If (IsValidHandle(m_handle0)) Then

			' Update the information of frame rate.
			Dim count As New dvpFrameCount
			Dim str As String
			Dim status As dvpStatus

			status = DVPCamera.dvpGetFrameCount(m_handle0, count)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Get frame count fail!")
				Return
			End If

			Dim item As Integer
			item = 0
			If m_uDisplayCount(item) = 0 Or m_Stopwatch(item).ElapsedMilliseconds < 2 Then
				str = "[" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = "[" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount(item) * 1000.0F / m_Stopwatch(item).ElapsedMilliseconds) + " fps]"
			End If

			FriendlyName0.Text = str
		End If
	End Sub

	Public Sub theout1(ByVal sender As Object, ByVal e As EventArgs) Handles m_Timer1.Tick
		If (IsValidHandle(m_handle1)) Then

			' Update the information of frame rate.
			Dim count As New dvpFrameCount
			Dim str As String
			Dim status As dvpStatus

			status = DVPCamera.dvpGetFrameCount(m_handle1, count)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Get frame count fail!")
				Return
			End If

			Dim item As Integer
			item = 1
			If m_uDisplayCount(item) = 0 Or m_Stopwatch(item).ElapsedMilliseconds < 2 Then
				str = "[" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = "[" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount(item) * 1000.0F / m_Stopwatch(item).ElapsedMilliseconds) + " fps]"
			End If

			FriendlyName1.Text = str
		End If
	End Sub

	Public Sub theout2(ByVal sender As Object, ByVal e As EventArgs) Handles m_Timer2.Tick
		If (IsValidHandle(m_handle2)) Then

			' Update the information of frame rate.
			Dim count As New dvpFrameCount
			Dim str As String
			Dim status As dvpStatus

			status = DVPCamera.dvpGetFrameCount(m_handle2, count)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Get frame count fail!")
				Return
			End If

			Dim item As Integer
			item = 2
			If m_uDisplayCount(item) = 0 Or m_Stopwatch(item).ElapsedMilliseconds < 2 Then
				str = "[" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = "[" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount(item) * 1000.0F / m_Stopwatch(item).ElapsedMilliseconds) + " fps]"
			End If

			FriendlyName2.Text = str
		End If
	End Sub

	Public Sub theout3(ByVal sender As Object, ByVal e As EventArgs) Handles m_Timer3.Tick
		If (IsValidHandle(m_handle3)) Then

			' Update the information of frame rate.
			Dim count As New dvpFrameCount
			Dim str As String
			Dim status As dvpStatus

			status = DVPCamera.dvpGetFrameCount(m_handle3, count)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Get frame count fail!")
				Return
			End If

			Dim item As Integer
			item = 3
			If m_uDisplayCount(item) = 0 Or m_Stopwatch(item).ElapsedMilliseconds < 2 Then
				str = "[" + count.uFrameCount.ToString() + " frames, " + String.Format("{0:#0.00}", count.fFrameRate) + " fps]"
			Else
				str = "[" + count.uFrameCount.ToString() + " frames, " _
				  + String.Format("{0:#0.00}", count.fFrameRate) + " fps, Display " _
				  + String.Format("{0:#0.00}", m_uDisplayCount(item) * 1000.0F / m_Stopwatch(item).ElapsedMilliseconds) + " fps]"
			End If

			FriendlyName3.Text = str
		End If
	End Sub

    ' Check whether the camera is opened.
    Private Function IsValidHandle(ByVal handleid As UInteger) As Boolean
        Dim bValidHandle As Boolean
        Dim status As dvpStatus

        bValidHandle = False
        status = DVPCamera.dvpIsValid(handleid, bValidHandle)
        Return bValidHandle
    End Function


    Private Sub MultipleCamera_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

		Control.CheckForIllegalCrossThreadCalls = False

		m_PtrWnd0 = pictureBox0.Handle
        m_PtrWnd1 = pictureBox1.Handle
        m_PtrWnd2 = pictureBox2.Handle
        m_PtrWnd3 = pictureBox3.Handle
        ScanCamera0()
        ScanCamera1()
        ScanCamera2()
		ScanCamera3()

		For i = 0 To 3
			m_Stopwatch(i) = New Stopwatch()
			m_uDisplayCount(i) = New UInteger
		Next


		m_Timer0.Interval = 500
		m_Timer0.Enabled = True

		m_Timer1.Interval = 500
		m_Timer1.Enabled = True

		m_Timer2.Interval = 500
		m_Timer2.Enabled = True

		m_Timer3.Interval = 500
		m_Timer3.Enabled = True
    End Sub


    Private Function OnDrawPicture0(ByVal handle As UInteger, ByVal _event As dvpStreamEvent,
                               ByVal pContext As IntPtr, ByRef pFrame As dvpFrame,
                               ByVal pBuffer As IntPtr) As Integer
		Dim bDisplay As Boolean
		bDisplay = False

		Dim item As Integer
		item = 0

		If m_uDisplayCount(item) = 0 Then
			m_Stopwatch(item).Restart()
			bDisplay = True
		Else
			If (m_Stopwatch(item).ElapsedMilliseconds - (m_uDisplayCount(item) * 33.3)) >= 33.0 Then
				bDisplay = True
			End If
		End If

		If bDisplay Then

			m_uDisplayCount(item) += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.
			Dim status As dvpStatus = New dvpStatus()
			status = DVPCamera.dvpDrawPicture(pFrame, pBuffer, m_PtrWnd0, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		End If

		Return 0
    End Function


    Private Function OnDrawPicture1(ByVal handle As UInteger, ByVal _event As dvpStreamEvent,
                               ByVal pContext As IntPtr, ByRef pFrame As dvpFrame,
                               ByVal pBuffer As IntPtr) As Integer
		Dim bDisplay As Boolean
		bDisplay = False

		Dim item As Integer
		item = 1

		If m_uDisplayCount(item) = 0 Then
			m_Stopwatch(item).Restart()
			bDisplay = True
		Else
			If (m_Stopwatch(item).ElapsedMilliseconds - (m_uDisplayCount(item) * 33.3)) >= 33.0 Then
				bDisplay = True
			End If
		End If

		If bDisplay Then

			m_uDisplayCount(item) += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.
			Dim status As dvpStatus = New dvpStatus()
			status = DVPCamera.dvpDrawPicture(pFrame, pBuffer, m_PtrWnd1, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		End If

		Return 0
    End Function


    Private Function OnDrawPicture2(ByVal handle As UInteger, ByVal _event As dvpStreamEvent,
                               ByVal pContext As IntPtr, ByRef pFrame As dvpFrame,
                               ByVal pBuffer As IntPtr) As Integer
		Dim bDisplay As Boolean
		bDisplay = False

		Dim item As Integer
		item = 2

		If m_uDisplayCount(item) = 0 Then
			m_Stopwatch(item).Restart()
			bDisplay = True
		Else
			If (m_Stopwatch(item).ElapsedMilliseconds - (m_uDisplayCount(item) * 33.3)) >= 33.0 Then
				bDisplay = True
			End If
		End If

		If bDisplay Then

			m_uDisplayCount(item) += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.
			Dim status As dvpStatus = New dvpStatus()
			status = DVPCamera.dvpDrawPicture(pFrame, pBuffer, m_PtrWnd2, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		End If

		Return 0
    End Function


    Private Function OnDrawPicture3(ByVal handle As UInteger, ByVal _event As dvpStreamEvent,
                               ByVal pContext As IntPtr, ByRef pFrame As dvpFrame,
                               ByVal pBuffer As IntPtr) As Integer
		Dim bDisplay As Boolean
		bDisplay = False

		Dim item As Integer
		item = 3

		If m_uDisplayCount(item) = 0 Then
			m_Stopwatch(item).Restart()
			bDisplay = True
		Else
			If (m_Stopwatch(item).ElapsedMilliseconds - (m_uDisplayCount(item) * 33.3)) >= 33.0 Then
				bDisplay = True
			End If
		End If

		If bDisplay Then

			m_uDisplayCount(item) += 1

			' It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
			' in order to avoid affecting the frame rate and the real-time of acquiring images.
			' The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
			' however, the user can malloc memory and copy image data.
			Dim status As dvpStatus = New dvpStatus()
			status = DVPCamera.dvpDrawPicture(pFrame, pBuffer, m_PtrWnd3, 0, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

		End If

		Return 0
    End Function


    Private Sub UpdateTitle0()
        Dim info As New dvpCameraInfo()
        Dim status As dvpStatus

        ' Get the camera's information.
        status = DVPCamera.dvpGetCameraInfo(m_handle0, info)
        Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

        FriendlyName0.Text = info.FriendlyName
        EditUserID0.Text = info.UserID
    End Sub


    Private Sub UpdateTitle1()
        Dim info As New dvpCameraInfo()
        Dim status As dvpStatus

        ' Get the camera's information.
        status = DVPCamera.dvpGetCameraInfo(m_handle1, info)
        Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

        FriendlyName1.Text = info.FriendlyName
        EditUserID1.Text = info.UserID
    End Sub


    Private Sub UpdateTitle2()
        Dim info As New dvpCameraInfo()
        Dim status As dvpStatus

        ' Get the camera's information.
        status = DVPCamera.dvpGetCameraInfo(m_handle2, info)
        Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

        FriendlyName2.Text = info.FriendlyName
        EditUserID2.Text = info.UserID
    End Sub


    Private Sub UpdateTitle3()
        Dim info As New dvpCameraInfo()
        Dim status As dvpStatus

        ' Get the camera's information.
        status = DVPCamera.dvpGetCameraInfo(m_handle3, info)
        Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

        FriendlyName3.Text = info.FriendlyName
        EditUserID3.Text = info.UserID

    End Sub


    Private Sub UpdateControls0()
        If (IsValidHandle(m_handle0)) Then

            ' The device has been opened at this time.
            Dim info As New dvpCameraInfo()
            Dim state As New dvpStreamState()
            Dim status As dvpStatus

            status = DVPCamera.dvpGetStreamState(m_handle0, state)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set the basic controls.
            OpenDev0.Text = "Close"
            If dvpStreamState.STATE_STARTED = state Then
                StartPlay0.Text = "Stop"
            Else
                StartPlay0.Text = "Start"
            End If
            StartPlay0.Enabled = True

            ' Set the enable status of the related controls.
            PropertySet0.Enabled = True
            EditUserID0.Enabled = True
			SetUserID0.Enabled = True

            ' Get the information of camera.
			status = DVPCamera.dvpGetCameraInfo(m_handle0, info)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			EditUserID0.Text = info.UserID
        Else
            ' No device is opened at this time.
            ' Set the basic controls.
            OpenDev0.Text = "Open"
            FriendlyName0.Text = "Friendly Name"
            StartPlay0.Text = "Start"
            StartPlay0.Enabled = False

            ' Set the enable status of the related controls.
            PropertySet0.Enabled = False
            SetUserID0.Enabled = False
            pictureBox0.Image = Nothing
        End If
    End Sub


    Private Sub UpdateControls1()

        If (IsValidHandle(m_handle1)) Then

            ' The device has been opened at this time.
            Dim info As New dvpCameraInfo()
            Dim state As New dvpStreamState()
            Dim status As dvpStatus

            status = DVPCamera.dvpGetStreamState(m_handle1, state)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set the basic controls.
            OpenDev1.Text = "Close"
            If dvpStreamState.STATE_STARTED = state Then
                StartPlay1.Text = "Stop"
            Else
                StartPlay1.Text = "Start"
            End If
            StartPlay1.Enabled = True

            ' Set the enable status of the related controls.
            PropertySet1.Enabled = True
            EditUserID1.Enabled = True
            SetUserID1.Enabled = True

            ' Get the information of camera.
			status = DVPCamera.dvpGetCameraInfo(m_handle1, info)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			EditUserID1.Text = info.UserID
        Else
            ' No device is opened at this time.
            ' Set the basic controls.
            OpenDev1.Text = "Open"
            FriendlyName1.Text = "Friendly Name"
            StartPlay1.Text = "Start"
            StartPlay1.Enabled = False

            ' Set the enable status of the related controls.
            PropertySet1.Enabled = False
            SetUserID1.Enabled = False
            pictureBox1.Image = Nothing
        End If
    End Sub


    Private Sub UpdateControls2()
        Dim status As dvpStatus
        If (IsValidHandle(m_handle2)) Then

            ' The device has been opened at this time.
            Dim info As New dvpCameraInfo()
            Dim state As New dvpStreamState()

            status = DVPCamera.dvpGetStreamState(m_handle2, state)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set the basic controls.
            OpenDev2.Text = "Close"

            If dvpStreamState.STATE_STARTED = state Then
                StartPlay2.Text = "Stop"
            Else
                StartPlay2.Text = "Start"
            End If
            StartPlay2.Enabled = True

            ' Set the enable status of the related controls.
            PropertySet2.Enabled = True
            EditUserID2.Enabled = True
            SetUserID2.Enabled = True

            ' Get the information of camera.
			status = DVPCamera.dvpGetCameraInfo(m_handle2, info)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			EditUserID2.Text = info.UserID
        Else
            ' No device is opened at this time.
            ' Set the basic controls.
            OpenDev2.Text = "Open"
            FriendlyName2.Text = "Friendly Name"
            StartPlay2.Text = "Start"
            StartPlay2.Enabled = False

            ' Set the enable status of the related controls.
            PropertySet2.Enabled = False
            SetUserID2.Enabled = False
            pictureBox2.Image = Nothing
        End If
    End Sub


    Private Sub UpdateControls3()
        Dim status As dvpStatus
        If (IsValidHandle(m_handle3)) Then

            ' The device has been opened at this time.
            Dim info As New dvpCameraInfo()
            Dim state As New dvpStreamState()

            status = DVPCamera.dvpGetStreamState(m_handle3, state)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

            ' Set the basic controls.
            OpenDev3.Text = "Close"
            If dvpStreamState.STATE_STARTED = state Then
                StartPlay3.Text = "Stop"
            Else
                StartPlay3.Text = "Start"
            End If
            StartPlay3.Enabled = True

            ' Set the enable status of the related controls.
            PropertySet3.Enabled = True
            EditUserID3.Enabled = True
            SetUserID3.Enabled = True

            ' Get the information of camera.
			status = DVPCamera.dvpGetCameraInfo(m_handle3, info)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
			EditUserID3.Text = info.UserID
        Else
            ' No device is opened at this time.
            ' Set the basic controls.
            OpenDev3.Text = "Open"
            FriendlyName3.Text = "Friendly Name"
            StartPlay3.Text = "Start"
            StartPlay3.Enabled = False

            ' Set the enable status of the related controls.
            PropertySet3.Enabled = False
            SetUserID3.Enabled = False
            pictureBox3.Image = Nothing
        End If
    End Sub


    Private Sub ScanDev0_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev0.Click
        ScanCamera0()
    End Sub


    Private Sub ScanCamera0()
        Dim iNum As UInteger
        Dim iCount As UInteger

        ' "iCount" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
        ' Clear the list of the camera.
        DevNameCombo0.Items.Clear()
		Dim status As dvpStatus
        If dvpStatus.DVP_STATUS_OK = dvpRefresh(iCount) Then

            ' Enumeration of up to 16 cameras.
            If iCount > 16 Then
				iCount = 16
			ElseIf iCount = 0 Then
				Return
            End If

            For iNum = 0 To iCount - 1

				' Acquire each camera's information one by one.
				status = dvpEnum(iNum, m_dev_info0(iNum))
				If dvpStatus.DVP_STATUS_OK = status Then
					DevNameCombo0.Items.Add(m_dev_info0(iNum).FriendlyName)

				Else
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
            Next

            If iNum > 0 Then
				DevNameCombo0.SelectedIndex = 0
				EditUserID0.Text = m_dev_info0(0).UserID
            End If

            UpdateControls0()
        End If

    End Sub

    Private Sub ScanDev1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev1.Click
        ScanCamera1()
    End Sub

    Private Sub ScanCamera1()
        Dim iNum As UInteger
        Dim iCount As UInteger
		Dim status As dvpStatus

        ' "iCount" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.

        ' Clear the list of the camera.
        DevNameCombo1.Items.Clear()

        If dvpStatus.DVP_STATUS_OK = dvpRefresh(iCount) Then

            ' Enumeration of up to 16 cameras.
            If iCount > 16 Then
				iCount = 16
			ElseIf iCount = 0 Then
				Return
            End If

            For iNum = 0 To iCount - 1

				' Acquire each camera's information one by one.
				status = dvpEnum(iNum, m_dev_info1(iNum))
				If dvpStatus.DVP_STATUS_OK = status Then
					DevNameCombo1.Items.Add(m_dev_info1(iNum).FriendlyName)
				Else
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
            Next

            If iNum > 1 Then
				DevNameCombo1.SelectedIndex = 1
				EditUserID1.Text = m_dev_info1(1).UserID
            End If

            UpdateControls1()
        End If
    End Sub

    Private Sub ScanDev2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev2.Click
        ScanCamera2()
    End Sub

    Private Sub ScanCamera2()
        Dim iNum As UInteger
        Dim iCount As UInteger
        Dim info As New dvpCameraInfo()
		Dim status As dvpStatus
        ' "iCount" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.

        ' Clear the list of the camera.
        DevNameCombo2.Items.Clear()

        If dvpStatus.DVP_STATUS_OK = dvpRefresh(iCount) Then

            ' Enumeration of up to 16 cameras.
            If iCount > 16 Then
				iCount = 16
			ElseIf iCount = 0 Then
				Return
            End If

            For iNum = 0 To iCount - 1

				' Acquire each camera's information one by one.
				status = dvpEnum(iNum, m_dev_info2(iNum))
				If dvpStatus.DVP_STATUS_OK = status Then
					DevNameCombo2.Items.Add(m_dev_info2(iNum).FriendlyName)
				Else
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
            Next

            If iNum > 2 Then
				DevNameCombo2.SelectedIndex = 2
				EditUserID2.Text = m_dev_info2(2).UserID
            End If

            UpdateControls2()
        End If
    End Sub

    Private Sub ScanDev3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ScanDev3.Click
        ScanCamera3()
    End Sub

    Private Sub ScanCamera3()
        Dim iNum As UInteger
        Dim iCount As UInteger
		Dim status As dvpStatus
        ' "iCount" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.

        ' Clear the list of the camera.
        DevNameCombo3.Items.Clear()

        If dvpStatus.DVP_STATUS_OK = dvpRefresh(iCount) Then

            ' Enumeration of up to 16 cameras.
            If iCount > 16 Then
				iCount = 16
			ElseIf iCount = 0 Then
				Return
            End If

            For iNum = 0 To iCount - 1

				' Acquire each camera's information one by one.
				status = dvpEnum(iNum, m_dev_info3(iNum))
				If dvpStatus.DVP_STATUS_OK = status Then
					DevNameCombo3.Items.Add(m_dev_info3(iNum).FriendlyName)
				Else
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
            Next

            If iNum > 3 Then
				DevNameCombo3.SelectedIndex = 3
				EditUserID3.Text = m_dev_info3(3).UserID
            End If

            UpdateControls3()
        End If
    End Sub

    Private Sub OpenDev0_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev0.Click
        OpenCamera0()
    End Sub

	Private Sub OpenCamera0()

		m_uDisplayCount(0) = 0

		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture0)
		Dim status As dvpStatus
		Dim strName As String

		If IsValidHandle(m_handle0) = False Then
			strName = DevNameCombo0.Text
			If (strName <> "") Then

				' Open the pointed device by the selected FriendlyName.
				status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, m_handle0)
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed!")
				Else
					DevNameCombo0.Enabled = False
					FriendlyName0.Text = strName

					status = DVPCamera.dvpRegisterStreamCallback(m_handle0, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
			End If
		Else
			Dim StreamState As dvpStreamState
			' check camera
			status = dvpGetStreamState(m_handle0, StreamState)
			If dvpStatus.DVP_STATUS_OK = status Then
				If StreamState = dvpStreamState.STATE_STARTED Then
					' stop camera
					status = dvpStop(m_handle0)
					If dvpStatus.DVP_STATUS_OK <> status Then
						MessageBox.Show("Stop video stream fail!")
						Return
					End If
				End If
			End If

			status = DVPCamera.dvpClose(m_handle0)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Close camera fail!")
			End If
			pictureBox0.Refresh()
			DevNameCombo0.Enabled = True
			m_handle0 = 0
		End If
		UpdateControls0()
	End Sub

    Private Sub OpenDev1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev1.Click
        OpenCamera1()
    End Sub

	Private Sub OpenCamera1()

		m_uDisplayCount(1) = 0

		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture1)
		Dim status As dvpStatus
		Dim strName As String

		If IsValidHandle(m_handle1) = False Then
			strName = DevNameCombo1.Text
			If (strName <> "") Then

				' Open the pointed device by the selected FriendlyName.
				status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, m_handle1)
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed!")
				Else
					DevNameCombo1.Enabled = False
					FriendlyName1.Text = strName

					status = DVPCamera.dvpRegisterStreamCallback(m_handle1, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
			End If
		Else
			Dim StreamState As dvpStreamState
			' check camera
			status = dvpGetStreamState(m_handle1, StreamState)
			If dvpStatus.DVP_STATUS_OK = status Then
				If StreamState = dvpStreamState.STATE_STARTED Then
					' stop camera
					status = dvpStop(m_handle1)
					If dvpStatus.DVP_STATUS_OK <> status Then
						MessageBox.Show("Stop video stream fail!")
						Return
					End If
				End If
			End If

			status = DVPCamera.dvpClose(m_handle1)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Close camera fail!")
			End If
			DevNameCombo1.Enabled = True
			m_handle1 = 0
			pictureBox1.Refresh()
		End If
		UpdateControls1()
	End Sub

    Private Sub OpenDev2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev2.Click
        OpenCamera2()
    End Sub

	Private Sub OpenCamera2()

		m_uDisplayCount(2) = 0

		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture2)
		Dim status As dvpStatus
		Dim strName As String

		If IsValidHandle(m_handle2) = False Then
			strName = DevNameCombo2.Text
			If (strName <> "") Then

				' Open the pointed device by the selected FriendlyName.
				status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, m_handle2)
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed!")
				Else
					DevNameCombo2.Enabled = False
					FriendlyName2.Text = strName
					status = DVPCamera.dvpRegisterStreamCallback(m_handle2, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
				End If
			End If
		Else
			Dim StreamState As dvpStreamState
			' check camera
			status = dvpGetStreamState(m_handle2, StreamState)
			If dvpStatus.DVP_STATUS_OK = status Then
				If StreamState = dvpStreamState.STATE_STARTED Then
					' stop camera
					status = dvpStop(m_handle2)
					If dvpStatus.DVP_STATUS_OK <> status Then
						MessageBox.Show("Stop video stream fail!")
						Return
					End If
				End If
			End If

			status = DVPCamera.dvpClose(m_handle2)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Close camera fail!")
			End If
			DevNameCombo2.Enabled = True
			m_handle2 = 0
			pictureBox2.Refresh()
		End If
		UpdateControls2()
	End Sub

    Private Sub OpenDev3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenDev3.Click
        OpenCamera3()
    End Sub

	Private Sub OpenCamera3()

		m_uDisplayCount(3) = 0

		Static Dim pCallBack As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture3)
		Dim status As dvpStatus
		Dim strName As String

		If IsValidHandle(m_handle3) = False Then
			strName = DevNameCombo3.Text
			If (strName <> "") Then

				' Open the pointed device by the selected FriendlyName.
				status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, m_handle3)
				If (status <> dvpStatus.DVP_STATUS_OK) Then
					MessageBox.Show("Open the device failed!")
				Else
					DevNameCombo3.Enabled = False
					FriendlyName3.Text = strName
					status = DVPCamera.dvpRegisterStreamCallback(m_handle3, pCallBack, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
					Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

				End If
			End If
		Else

			Dim StreamState As dvpStreamState
			' check camera
			status = dvpGetStreamState(m_handle3, StreamState)
			If dvpStatus.DVP_STATUS_OK = status Then
				If StreamState = dvpStreamState.STATE_STARTED Then
					' stop camera
					status = dvpStop(m_handle3)
					If dvpStatus.DVP_STATUS_OK <> status Then
						MessageBox.Show("Stop video stream fail!")
						Return
					End If
				End If
			End If

			status = DVPCamera.dvpClose(m_handle3)
			If status <> dvpStatus.DVP_STATUS_OK Then
				MessageBox.Show("Close camera fail!")
			End If
			DevNameCombo3.Enabled = True
			m_handle3 = 0
			pictureBox3.Refresh()
		End If
		UpdateControls3()
	End Sub

    Private Sub StartPlay0_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay0.Click
        StartVideoStream0()
    End Sub

	Private Sub StartVideoStream0()

		m_uDisplayCount(0) = 0

		If IsValidHandle(m_handle0) Then
			Dim state As New dvpStreamState()
			Dim status As New dvpStatus()

			' Implement a button to start and stop according to the current video's status.
			status = DVPCamera.dvpGetStreamState(m_handle0, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle0)

				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Stop video stream fail!")
				End If
			Else
				UpdateTitle0()
				status = DVPCamera.dvpStart(m_handle0)
				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Start video stream fail!")
				End If
			End If

			UpdateControls0()
		End If
	End Sub

    Private Sub StartPlay1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay1.Click
        StartVideoStream1()
    End Sub

	Private Sub StartVideoStream1()

		m_uDisplayCount(1) = 0

		If IsValidHandle(m_handle1) Then
			Dim state As New dvpStreamState()
			Dim status As New dvpStatus()

			' Implement a button to start and stop according to the current video's status.
			status = DVPCamera.dvpGetStreamState(m_handle1, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle1)

				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Stop video stream fail!")
				End If
			Else
				UpdateTitle1()
				status = DVPCamera.dvpStart(m_handle1)
				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Start video stream fail!")
				End If
			End If

			UpdateControls1()
		End If
	End Sub

    Private Sub StartPlay2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay2.Click
        StartVideoStream2()
    End Sub

	Private Sub StartVideoStream2()

		m_uDisplayCount(2) = 0

		If IsValidHandle(m_handle2) Then
			Dim state As New dvpStreamState()
			Dim status As New dvpStatus()

			' Implement a button to start and stop according to the current video's status.
			status = DVPCamera.dvpGetStreamState(m_handle2, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle2)

				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Stop video stream fail!")
				End If
			Else
				UpdateTitle2()
				status = DVPCamera.dvpStart(m_handle2)
				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Start video stream fail!")
				End If
			End If
			UpdateControls2()
		End If
	End Sub

    Private Sub StartPlay3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartPlay3.Click
        StartVideoStream3()
    End Sub

	Private Sub StartVideoStream3()

		m_uDisplayCount(3) = 0

		If IsValidHandle(m_handle3) Then
			Dim state As New dvpStreamState()
			Dim status As New dvpStatus()

			' Implement a button to start and stop according to the current video's status.
			status = DVPCamera.dvpGetStreamState(m_handle3, state)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			If (state = dvpStreamState.STATE_STARTED) Then
				status = DVPCamera.dvpStop(m_handle3)

				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Stop video stream fail!")
				End If
			Else
				UpdateTitle3()
				status = DVPCamera.dvpStart(m_handle3)
				If status <> dvpStatus.DVP_STATUS_OK Then
					MessageBox.Show("Start video stream fail!")
				End If
			End If
			UpdateControls3()
		End If
	End Sub

    Private Sub PropertySet0_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertySet0.Click
        If (IsValidHandle(m_handle0)) Then
            DVPCamera.dvpShowPropertyModalDialog(m_handle0, Me.Handle)
        End If
    End Sub


    Private Sub PropertySet1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertySet1.Click
        If (IsValidHandle(m_handle1)) Then
            DVPCamera.dvpShowPropertyModalDialog(m_handle1, Me.Handle)
        End If
    End Sub


    Private Sub PropertySet2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertySet2.Click
        If (IsValidHandle(m_handle2)) Then
            DVPCamera.dvpShowPropertyModalDialog(m_handle2, Me.Handle)
        End If
    End Sub


    Private Sub PropertySet3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PropertySet3.Click
        If (IsValidHandle(m_handle3)) Then
            DVPCamera.dvpShowPropertyModalDialog(m_handle3, Me.Handle)
        End If
    End Sub


    Private Sub SetUserID0_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetUserID0.Click
        If (IsValidHandle(m_handle0)) Then
            Dim Length As UInteger
            Dim str As String
            Dim status As dvpStatus
            str = EditUserID0.Text
            Length = str.Length

            'Set the user ID.
            status = DVPCamera.dvpSetUserId(m_handle0, str, Length)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)

			m_dev_info0(DevNameCombo0.SelectedIndex).UserID = str

            ' Update the user ID of other list.
			Dim item As Integer
			For item = 0 To 15
				If m_dev_info1(item).FriendlyName = m_dev_info0(DevNameCombo0.SelectedIndex).FriendlyName Then
					m_dev_info1(item).UserID = str
					EditUserID1.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info2(item).FriendlyName = m_dev_info0(DevNameCombo0.SelectedIndex).FriendlyName Then
					m_dev_info2(item).UserID = str
					EditUserID2.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info3(item).FriendlyName = m_dev_info0(DevNameCombo0.SelectedIndex).FriendlyName Then
					m_dev_info3(item).UserID = str
					EditUserID3.Text = str
				End If
			Next

            SaveBind.Enabled = True
        End If
    End Sub

    Private Sub SetUserID1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetUserID1.Click
        If (IsValidHandle(m_handle1)) Then
            Dim Length As UInteger
            Dim str As String
            Dim status As dvpStatus
            str = EditUserID1.Text
            Length = str.Length

            'Set the user ID.
            status = DVPCamera.dvpSetUserId(m_handle1, str, Length)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
            m_dev_info0(DevNameCombo1.SelectedIndex).UserID = str

            ' Update the user ID of other list.
			Dim item As Integer
			For item = 0 To 15
				If m_dev_info0(item).FriendlyName = m_dev_info1(DevNameCombo1.SelectedIndex).FriendlyName Then
					m_dev_info0(item).UserID = str
					EditUserID0.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info2(item).FriendlyName = m_dev_info1(DevNameCombo1.SelectedIndex).FriendlyName Then
					m_dev_info2(item).UserID = str
					EditUserID2.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info3(item).FriendlyName = m_dev_info1(DevNameCombo1.SelectedIndex).FriendlyName Then
					m_dev_info3(item).UserID = str
					EditUserID3.Text = str
				End If
			Next
            SaveBind.Enabled = True
        End If
    End Sub

    Private Sub SetUserID2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetUserID2.Click
        If (IsValidHandle(m_handle2)) Then
            Dim Length As UInteger
            Dim str As String
            Dim status As dvpStatus
            str = EditUserID2.Text
            Length = str.Length

            'Set the user ID.
            status = DVPCamera.dvpSetUserId(m_handle2, str, Length)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
            m_dev_info2(DevNameCombo2.SelectedIndex).UserID = str

            ' Update the user ID of other list.
			Dim item As Integer
			For item = 0 To 15
				If m_dev_info0(item).FriendlyName = m_dev_info2(DevNameCombo2.SelectedIndex).FriendlyName Then
					m_dev_info0(item).UserID = str
					EditUserID0.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info1(item).FriendlyName = m_dev_info2(DevNameCombo2.SelectedIndex).FriendlyName Then
					m_dev_info1(item).UserID = str
					EditUserID1.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info3(item).FriendlyName = m_dev_info2(DevNameCombo2.SelectedIndex).FriendlyName Then
					m_dev_info3(item).UserID = str
					EditUserID3.Text = str
				End If
			Next
            SaveBind.Enabled = True
        End If
    End Sub

    Private Sub SetUserID3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SetUserID3.Click
        If (IsValidHandle(m_handle3)) Then
            Dim Length As UInteger
            Dim str As String
            Dim status As dvpStatus
            str = EditUserID3.Text
            Length = str.Length

            'Set the user ID.
            status = DVPCamera.dvpSetUserId(m_handle3, str, Length)
            Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
            m_dev_info3(DevNameCombo3.SelectedIndex).UserID = str

            ' Update the user ID of other list.
			Dim item As Integer
			For item = 0 To 15
				If m_dev_info0(item).FriendlyName = m_dev_info3(DevNameCombo3.SelectedIndex).FriendlyName Then
					m_dev_info0(item).UserID = str
					EditUserID0.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info1(item).FriendlyName = m_dev_info3(DevNameCombo3.SelectedIndex).FriendlyName Then
					m_dev_info1(item).UserID = str
					EditUserID1.Text = str
				End If
			Next
			For item = 0 To 15
				If m_dev_info2(item).FriendlyName = m_dev_info3(DevNameCombo3.SelectedIndex).FriendlyName Then
					m_dev_info2(item).UserID = str
					EditUserID2.Text = str
				End If
			Next
            SaveBind.Enabled = True
        End If
    End Sub

    Private Sub SaveBind_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SaveBind.Click
        SaveBindRelation()
    End Sub

    Private Sub SaveBindRelation()
        Dim strUserId0 As String
        Dim strUserId1 As String
        Dim strUserId2 As String
        Dim strUserId3 As String

        ' Get the content of the user id controls.
        strUserId0 = EditUserID0.Text
        strUserId1 = EditUserID1.Text
        strUserId2 = EditUserID2.Text
		strUserId3 = EditUserID3.Text

		' Avoid repetitive "user id", if not, binding relationship won't be able to set up.
		
		Dim b_error_flg As Boolean
		b_error_flg = False

		If (strUserId0.Length > 0) Then
			If (strUserId1.Length > 0) Then
				If (strUserId0 = strUserId1) Then
					b_error_flg = True
				End If
			End If

			If (strUserId2.Length > 0) Then
				If (strUserId0 = strUserId2) Then
					b_error_flg = True
				End If
			End If

			If (strUserId3.Length > 0) Then
				If (strUserId0 = strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

		If (strUserId1.Length > 0) Then
			If (strUserId2.Length > 0) Then
				If (strUserId1 = strUserId2) Then
					b_error_flg = True
				End If
			End If
			If (strUserId3.Length > 0) Then
				If (strUserId1 = strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

		If (strUserId2.Length > 0) Then
			If (strUserId3.Length > 0) Then
				If (strUserId2 = strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

		If (b_error_flg) Then
			' Judge whether there is repetitive "user id" except empty "user id". 
			MessageBox.Show(("User id repetition"), ("Save Failed"))
			Return
		End If

		' If there is not repetitive "user id",save the binding relationship into registry.
		DVPCamera.dvpWriteProfileString(("UserId"), ("0"), strUserId0)
		DVPCamera.dvpWriteProfileString(("UserId"), ("1"), strUserId1)
		DVPCamera.dvpWriteProfileString(("UserId"), ("2"), strUserId2)
		DVPCamera.dvpWriteProfileString(("UserId"), ("3"), strUserId3)

		SaveBind.Enabled = False
    End Sub



    Private Sub LoadBind_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles LoadBind.Click
        LoadBindRelation()
    End Sub

    Private Sub LoadBindRelation()
        Dim strUserId0 As String
        Dim strUserId1 As String
        Dim strUserId2 As String
		Dim strUserId3 As String

        Dim status As dvpStatus
		Dim str As String
		str = ""

		strUserId0 = DVPCamera.dvpGetProfileString(("UserId"), ("0"), 0)
		strUserId1 = DVPCamera.dvpGetProfileString(("UserId"), ("1"), 0)
		strUserId2 = DVPCamera.dvpGetProfileString(("UserId"), ("2"), 0)
		strUserId3 = DVPCamera.dvpGetProfileString(("UserId"), ("3"), 0)

		Dim b_error_flg As Boolean
		b_error_flg = False
		If (strUserId0.Length > 0) Then
			If (strUserId1.Length > 0) Then
				If (strUserId0 Is strUserId1) Then
					b_error_flg = True
				End If
			End If

			If (strUserId2.Length > 0) Then
				If (strUserId0 Is strUserId2) Then
					b_error_flg = True
				End If
			End If

			If (strUserId3.Length > 0) Then
				If (strUserId0 Is strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

		If (strUserId1.Length > 0) Then
			If (strUserId2.Length > 0) Then
				If (strUserId1 Is strUserId2) Then
					b_error_flg = True
				End If
			End If

			If (strUserId3.Length > 0) Then
				If (strUserId1 Is strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

		If (strUserId2.Length > 0) Then
			If (strUserId3.Length > 0) Then
				If (strUserId2 Is strUserId3) Then
					b_error_flg = True
				End If
			End If
		End If

        ' If cameras have been opened, close camera one by one. 
        If (IsValidHandle(m_handle0)) Then
            status = DVPCamera.dvpClose(m_handle0)
            If status <> dvpStatus.DVP_STATUS_OK Then
                MessageBox.Show("Close camera fail!")
            End If

            m_handle0 = 0
        End If

        If (IsValidHandle(m_handle1)) Then
            status = DVPCamera.dvpClose(m_handle1)
            If status <> dvpStatus.DVP_STATUS_OK Then
                MessageBox.Show("Close camera fail!")
            End If

            m_handle1 = 0
        End If

        If (IsValidHandle(m_handle2)) Then
            status = DVPCamera.dvpClose(m_handle2)
            If status <> dvpStatus.DVP_STATUS_OK Then
                MessageBox.Show("Close camera fail!")
            End If

            m_handle2 = 0
        End If

        If (IsValidHandle(m_handle3)) Then
            status = DVPCamera.dvpClose(m_handle3)
            If status <> dvpStatus.DVP_STATUS_OK Then
                MessageBox.Show("Close camera fail!")
            End If

            m_handle3 = 0
        End If

		Dim strNmae As String
		strNmae = ""

		' Open cameras by "user id".
		If OpenByUserId(strUserId0, m_handle0) Then
			DevNameCombo0.Enabled = False

			' Initialize the camera.
			FriendlyName0.Text = strNmae
			Static Dim pCallBack0 As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture0)
			status = DVPCamera.dvpRegisterStreamCallback(m_handle0, pCallBack0, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
		If OpenByUserId(strUserId1, m_handle1) Then
			DevNameCombo1.Enabled = False

			' Initialize the camera.
			FriendlyName1.Text = strNmae
			Static Dim pCallBack1 As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture1)
			status = DVPCamera.dvpRegisterStreamCallback(m_handle1, pCallBack1, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
		If OpenByUserId(strUserId2, m_handle2) Then
			DevNameCombo2.Enabled = False

			' Initialize the camera.
			FriendlyName2.Text = strNmae
			Static Dim pCallBack2 As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture2)
			status = DVPCamera.dvpRegisterStreamCallback(m_handle2, pCallBack2, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If
		If OpenByUserId(strUserId3, m_handle3) Then
			DevNameCombo3.Enabled = False

			' Initialize the camera.
			FriendlyName3.Text = strNmae
			Static Dim pCallBack3 As New DVPCamera.dvpStreamCallback(AddressOf OnDrawPicture3)
			status = DVPCamera.dvpRegisterStreamCallback(m_handle3, pCallBack3, dvpStreamEvent.STREAM_EVENT_PROCESSED, 0)
			Debug.Assert(status = dvpStatus.DVP_STATUS_OK)
		End If

        ' Update related controls.
        UpdateControls0()
        UpdateControls1()
        UpdateControls2()
        UpdateControls3()
    End Sub


	Private Function OpenByUserId(ByVal UserId As String, ByRef pHandle As UInteger) As Boolean

		If String.IsNullOrEmpty(UserId) Then
			Return False
		End If

		Dim status As dvpStatus
		Dim info As New dvpCameraInfo()
		
		' Open the camera with User Defind Name and Get the camera's handle. 
		status = DVPCamera.dvpOpenByUserId(UserId, dvpOpenMode.OPEN_NORMAL, pHandle)
		If status <> dvpStatus.DVP_STATUS_OK Then
			MessageBox.Show("Open the device failed!")
			Return True
		End If
		Return False
	End Function

	Private Sub MultipleCamera_FormClosing(sender As System.Object, e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing

        ' Close the camera.
		Dim status As dvpStatus
		If IsValidHandle(m_handle0) Then
			status = dvpStop(m_handle0)
			status = dvpClose(m_handle0)
			m_handle1 = 0
		End If
		If IsValidHandle(m_handle1) Then
			status = dvpStop(m_handle1)
			status = dvpClose(m_handle1)
			m_handle1 = 0
		End If
		If IsValidHandle(m_handle2) Then
			status = dvpStop(m_handle2)
			status = dvpClose(m_handle2)
			m_handle1 = 0
		End If
		If IsValidHandle(m_handle3) Then
			status = dvpStop(m_handle3)
			status = dvpClose(m_handle3)
			m_handle1 = 0
		End If
		
	End Sub

	Private Sub DevNameCombo0_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles DevNameCombo0.SelectedIndexChanged
		EditUserID0.Text = m_dev_info0(DevNameCombo0.SelectedIndex).UserID
	End Sub

	Private Sub DevNameCombo1_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles DevNameCombo1.SelectedIndexChanged
		EditUserID1.Text = m_dev_info1(DevNameCombo1.SelectedIndex).UserID
	End Sub

	Private Sub DevNameCombo2_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles DevNameCombo2.SelectedIndexChanged
		EditUserID2.Text = m_dev_info2(DevNameCombo2.SelectedIndex).UserID
	End Sub

	Private Sub DevNameCombo3_SelectedIndexChanged(sender As System.Object, e As System.EventArgs) Handles DevNameCombo3.SelectedIndexChanged
		EditUserID3.Text = m_dev_info3(DevNameCombo3.SelectedIndex).UserID
	End Sub
End Class
