using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DVPCameraType;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;


namespace MultipleCamera
{
    public partial class MultipleCamera : Form
    {
        public int m_n_dev_count_0;
        uint m_handle_0 = 0;

        public int m_n_dev_count_1;
        uint m_handle_1 = 0;

        public int m_n_dev_count_2;
        uint m_handle_2 = 0;

        public int m_n_dev_count_3;
        uint m_handle_3 = 0;

        public static IntPtr m_ptr_wnd0 = new IntPtr();
        public static IntPtr m_ptr0 = new IntPtr();
        public static IntPtr m_ptr_wnd1 = new IntPtr();
        public static IntPtr m_ptr1 = new IntPtr();
        public static IntPtr m_ptr_wnd2 = new IntPtr();
        public static IntPtr m_ptr2 = new IntPtr();
        public static IntPtr m_ptr_wnd3 = new IntPtr();
        public static IntPtr m_ptr3 = new IntPtr();

		dvpCameraInfo[] m_info0 = new dvpCameraInfo[16];
		dvpCameraInfo[] m_info1 = new dvpCameraInfo[16];
		dvpCameraInfo[] m_info2 = new dvpCameraInfo[16];
		dvpCameraInfo[] m_info3 = new dvpCameraInfo[16];

		// Display param
		public static Stopwatch m_Stopwatch0 = new Stopwatch();
		public static Stopwatch m_Stopwatch1 = new Stopwatch();
		public static Stopwatch m_Stopwatch2 = new Stopwatch();
		public static Stopwatch m_Stopwatch3 = new Stopwatch();
		public static Double[] m_dfDisplayCount = new Double[4];

		
        public MultipleCamera()
        {
			Form.CheckForIllegalCrossThreadCalls = false;	
            InitializeComponent();


            m_ptr_wnd0 = pictureBox.Handle;
            m_ptr_wnd1 = pictureBox1.Handle;
            m_ptr_wnd2 = pictureBox2.Handle;
            m_ptr_wnd3 = pictureBox3.Handle;

            InitDevList_0();
            InitDevList_1();
            InitDevList_2();
            InitDevList_3();

			System.Timers.Timer t = new System.Timers.Timer(500);

			// Execute the event when the time has arrived. 
			t.Elapsed += new System.Timers.ElapsedEventHandler(theout);

			// Set the method of executing
			t.AutoReset = true;

			// Judge whether execute the System.Timers.Timer.Elapsed event.
			t.Enabled = true;
						
        }

		public void theout(object source, System.Timers.ElapsedEventArgs e)
		{
			if (IsValidHandle(m_handle_0))
			{
				// Update the information of frame rate
				dvpFrameCount count = new dvpFrameCount();
				dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle_0, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				string str;
				if (m_dfDisplayCount[0] == 0 || m_Stopwatch0.ElapsedMilliseconds < 2)
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps]";
				}
				else
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps, Display "
						+ string.Format("{0:#0.00}", m_dfDisplayCount[0] * 1000.0f / m_Stopwatch0.ElapsedMilliseconds) + " fps]";
				}
				
				FriendlyName.Text = str;
			}

			if (IsValidHandle(m_handle_1))
			{
				// Update the information of frame rate
				dvpFrameCount count = new dvpFrameCount();
				dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle_1, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				string str;
				if (m_dfDisplayCount[1] == 0 || m_Stopwatch1.ElapsedMilliseconds < 2)
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps]";
				}
				else
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps, Display "
						+ string.Format("{0:#0.00}", m_dfDisplayCount[1] * 1000.0f / m_Stopwatch1.ElapsedMilliseconds) + " fps]";
				}
				FriendlyName1.Text = str;
			}

			if (IsValidHandle(m_handle_2))
			{
				// Update the information of frame rate
				dvpFrameCount count = new dvpFrameCount();
				dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle_2, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				string str;
				if (m_dfDisplayCount[2] == 0 || m_Stopwatch2.ElapsedMilliseconds < 2)
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps]";
				}
				else
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps, Display "
						+ string.Format("{0:#0.00}", m_dfDisplayCount[2] * 1000.0f / m_Stopwatch2.ElapsedMilliseconds) + " fps]";
				}
				FriendlyName2.Text = str;
			}

			if (IsValidHandle(m_handle_3))
			{
				// Update the information of frame rate
				dvpFrameCount count = new dvpFrameCount();
				dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle_3, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				string str;
				if (m_dfDisplayCount[3] == 0 || m_Stopwatch3.ElapsedMilliseconds < 2)
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps]";
				}
				else
				{
					str = "[" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps, Display "
						+ string.Format("{0:#0.00}", m_dfDisplayCount[3] * 1000.0f / m_Stopwatch3.ElapsedMilliseconds) + " fps]";
				}
				FriendlyName3.Text = str;
			}
		} 


        public void InitDevList_0()
        {
            dvpStatus status;
            uint i, n = 0;
            
            // "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
            DevNameCombo.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            m_n_dev_count_0 = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
				if (n > 16)
					n = 16;

                for (i = 0; i < n; i++)
                {
                    // Acquire each camera's information one by one.
					status = DVPCamera.dvpEnum(i, ref m_info0[i]);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
                        // GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
						int item = DevNameCombo.Items.Add(m_info0[i].FriendlyName);
                    }
                }
				if (n > 0)
				{
					DevNameCombo.SelectedIndex = 0;
					EditUserID.Text = m_info0[0].UserID;
				}
            }

            if (n == 0)
            {
                OpenDev.Enabled = false;
            }
            else
            {
                OpenDev.Enabled = true;
            }

            InitCtrls_0();
        }

        public void InitDevList_1()
        {
            dvpStatus status;
            uint i, n = 0;
            
            // "n" represents the number of cameras that is enumerated successfully,the drop-down list contains each camera's FriendlyName.
            DevNameCombo1.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
            m_n_dev_count_1 = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
				if (n > 16)
					n = 16;
                for (i = 0; i < n; i++)     
                {
                    // Acquire each camera's information one by one.
					status = DVPCamera.dvpEnum(i, ref m_info1[i]);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
                        // GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
						int item = DevNameCombo1.Items.Add(m_info1[i].FriendlyName);
                    }
                }
				if (n > 1)
				{
					DevNameCombo1.SelectedIndex = 1;
					EditUserID1.Text = m_info1[1].UserID;
				}
            }

            if (n == 0)
            {
                OpenDev1.Enabled = false;
            }
            else
            {
                OpenDev1.Enabled = true;
            }

            InitCtrls_1();
        }

        public void InitDevList_2()
        {
            dvpStatus status;
            uint i, n = 0;
            
            // "n" represents the number of cameras that is enumerated successfully,the drop-down list contains each camera's FriendlyName.
            DevNameCombo2.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
            m_n_dev_count_2 = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
                for (i = 0; i < n; i++)
                {
					if (n > 16)
						n = 16;

                    // Acquire each camera's information one by one.
					status = DVPCamera.dvpEnum(i, ref m_info2[i]);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
                        // GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
						int item = DevNameCombo2.Items.Add(m_info2[i].FriendlyName);
                    }
                }
				if (n > 2)
				{
					DevNameCombo2.SelectedIndex = 2;
					EditUserID2.Text = m_info2[2].UserID;
				}
            }
            if (n == 0)
            {
                OpenDev2.Enabled = false;
            }
            else
            {
                OpenDev2.Enabled = true;
            }

            InitCtrls_2();
        }

        public void InitDevList_3()
        {
            dvpStatus status;
            uint i, n = 0;
            
            // "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
            DevNameCombo3.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
            m_n_dev_count_3 = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
				if (n > 16)
					n = 16;

                for (i = 0; i < n; i++)
                {
                    // Acquire each camera's information one by one.
					status = DVPCamera.dvpEnum(i, ref m_info3[i]);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
                        // GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
						int item = DevNameCombo3.Items.Add(m_info3[i].FriendlyName);
                    }
                }

				if (n > 3)
				{
					DevNameCombo3.SelectedIndex = 3;
					EditUserID3.Text = m_info3[3].UserID;
				}
            }

            if (n == 0)
            {
                OpenDev3.Enabled = false;
            }
            else
            {
                OpenDev3.Enabled = true;
            }

            InitCtrls_3();
        }

        public bool IsValidHandle(uint handle)
        {
            bool bValidHandle = false;
            dvpStatus status = DVPCamera.dvpIsValid(handle, ref bValidHandle);
			if (status == dvpStatus.DVP_STATUS_OK)
			{
				return bValidHandle;
			}

			return false;
        }

        // Initialize the first camera.
        public void InitCtrls_0()
        {
	        dvpStatus status;
	        if (IsValidHandle(m_handle_0))
	        {
                // The device has been opened at this time.
		        dvpCameraInfo info = new dvpCameraInfo();
		        dvpStreamState state = new dvpStreamState();

                status = DVPCamera.dvpGetStreamState(m_handle_0, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set the basic controls.
                OpenDev.Text = "Close";
                StartPlay.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start");
                StartPlay.Enabled = true;

                // Set the enable status of the related controls.
                PropertSet.Enabled = true;
                EditUserID.Enabled = true;
                SetUserID.Enabled = true;

                // Get the camera's information.
				status = DVPCamera.dvpGetCameraInfo(m_handle_0, ref info);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                FriendlyName.Text = info.FriendlyName;
                EditUserID.Text = info.UserID;
	        }
	        else
	        {
                // No device is opened at this time.
                // Set the basic controls.
                OpenDev.Text = "Open";
		        FriendlyName.Text = "Friendly Name";
                StartPlay.Text = "Start";
                StartPlay.Enabled = false;

                // Set the enable status of the related controls.
                PropertSet.Enabled = false;
                SetUserID.Enabled = false;
	        }
        }

        public void InitCtrls_3()
        {
            dvpStatus status;
            if (IsValidHandle(m_handle_3))
            {
                // The device has been opened at this time.
                dvpCameraInfo info = new dvpCameraInfo();
                dvpStreamState state = new dvpStreamState();

                status = DVPCamera.dvpGetStreamState(m_handle_3, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                // Set the basic controls.
                OpenDev3.Text = "Close";
                StartPlay3.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start");
                StartPlay3.Enabled = true;

                // Set the enable status of the related controls.
                PropertSet3.Enabled = true;
                EditUserID3.Enabled = true;
                SetUserID3.Enabled = true;

                // Get the camera's information.
				status = DVPCamera.dvpGetCameraInfo(m_handle_3, ref info);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                FriendlyName3.Text = info.FriendlyName;
                EditUserID3.Text = info.UserID;
            }
            else
            {
                // No device is opened at this time.
                // Set the basic controls.
                OpenDev3.Text = "Open";
                FriendlyName3.Text = "Friendly Name";
                StartPlay3.Text = "Start";
                StartPlay3.Enabled = false;

                // Set the enable status of the related controls.
                PropertSet3.Enabled = false;
                SetUserID3.Enabled = false;
            }
        }

        public void InitCtrls_1()
        {
            dvpStatus status;
            if (IsValidHandle(m_handle_1))
            {
                // The device has been opened at this time.
                dvpCameraInfo info = new dvpCameraInfo();
                dvpStreamState state = new dvpStreamState();
                status = DVPCamera.dvpGetStreamState(m_handle_1, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set the basic controls.
                OpenDev1.Text = "Close";
                StartPlay1.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start");
                StartPlay1.Enabled = true;

                // Set the enable status of the related controls.
                PropertSet1.Enabled = true;
                EditUserID1.Enabled = true;
                SetUserID1.Enabled = true;

                // Get the camera's information.
				status = DVPCamera.dvpGetCameraInfo(m_handle_1, ref info);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                FriendlyName1.Text = info.FriendlyName;
                EditUserID1.Text = info.UserID;
            }
            else
            {
                // No device is opened at this time.
                // Set the basic controls.
                OpenDev1.Text = "Open";
                FriendlyName1.Text = "Friendly Name";
                StartPlay1.Text = "Start";
                StartPlay1.Enabled = false;

                // Set the enable status of the related controls.
                PropertSet1.Enabled = false;
                SetUserID1.Enabled = false;
            }
        }

        public void InitCtrls_2()
        {
            dvpStatus status;
            if (IsValidHandle(m_handle_2))
            {
                // The device has been opened at this time.
                dvpCameraInfo info = new dvpCameraInfo();
                dvpStreamState state = new dvpStreamState();
                status = DVPCamera.dvpGetStreamState(m_handle_2, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set the basic controls.
                OpenDev2.Text = "Close";
                StartPlay2.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start");
                StartPlay2.Enabled = true;

                // Set the enable status of the related controls.
                PropertSet2.Enabled = true;
                EditUserID2.Enabled = true;
                SetUserID2.Enabled = true;

                // Get the camera's information.
				status = DVPCamera.dvpGetCameraInfo(m_handle_2, ref info);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                FriendlyName2.Text = info.FriendlyName;
                EditUserID2.Text = info.UserID;
            }
            else
            {
                // No device is opened at this time.
                // Set the basic controls.
                OpenDev2.Text = "Open";
                FriendlyName2.Text = "Friendly Name";
                StartPlay2.Text = "Start";
                StartPlay2.Enabled = false;

                // Set the enable status of the related controls.
                PropertSet2.Enabled = false;
                SetUserID2.Enabled = false;
            }
        }

        private void ScanDev_Click(object sender, EventArgs e)
        {
            InitDevList_0();
        }

        private void ScanDev1_Click(object sender, EventArgs e)
        {
            InitDevList_1();
        }

        private void ScanDev2_Click(object sender, EventArgs e)
        {
            InitDevList_2();
        }

        private void ScanDev3_Click(object sender, EventArgs e)
        {
            InitDevList_3();
        }

        private DVPCamera.dvpStreamCallback _proc0;
        private DVPCamera.dvpStreamCallback _proc1;
        private DVPCamera.dvpStreamCallback _proc2;
        private DVPCamera.dvpStreamCallback _proc3;

        // The callback function that used for receiving data.
        public static int _dvpStreamCallback0(/*dvpHandle*/uint handle, dvpStreamEvent _event, /*void **/IntPtr pContext, ref dvpFrame refFrame, /*void **/IntPtr pBuffer)
        {
			bool bDisplay = false;

			if (m_dfDisplayCount[0] == 0)
			{
				m_Stopwatch0.Restart();
				bDisplay = true;
			}
			else
			{
				if (m_Stopwatch0.ElapsedMilliseconds - (long)(m_dfDisplayCount[0] * 33.3f) >= 33)
				{
					bDisplay = true;
				}
			}

			if (bDisplay)
			{
				m_dfDisplayCount[0]++;

				// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				// in order to avoid affecting the frame rate and the real-time of acquiring images.
				// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				// however, the user can malloc memory and copy image data.
				dvpStatus status = DVPCamera.dvpDrawPicture(ref refFrame, pBuffer,
					m_ptr_wnd0, (IntPtr)0, (IntPtr)0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}
            return 1;
        }
        public static int _dvpStreamCallback1(/*dvpHandle*/uint handle, dvpStreamEvent _event, /*void **/IntPtr pContext, ref dvpFrame refFrame, /*void **/IntPtr pBuffer)
        {
			bool bDisplay = false;

			if (m_dfDisplayCount[1] == 0)
			{
				m_Stopwatch1.Restart();
				bDisplay = true;
			}
			else
			{
				if (m_Stopwatch1.ElapsedMilliseconds - (long)(m_dfDisplayCount[1] * 33.3f) >= 33)
				{
					bDisplay = true;
				}
			}

			if (bDisplay)
			{
				m_dfDisplayCount[1]++;

				// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				// in order to avoid affecting the frame rate and the real-time of acquiring images.
				// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				// however, the user can malloc memory and copy image data.
				dvpStatus status = DVPCamera.dvpDrawPicture(ref refFrame, pBuffer,
					m_ptr_wnd1, (IntPtr)0, (IntPtr)0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}

			return 1;
        }

        public static int _dvpStreamCallback2(/*dvpHandle*/uint handle, dvpStreamEvent _event, /*void **/IntPtr pContext, ref dvpFrame refFrame, /*void **/IntPtr pBuffer)
        {
			bool bDisplay = false;

			if (m_dfDisplayCount[2] == 0)
			{
				m_Stopwatch2.Restart();
				bDisplay = true;
			}
			else
			{
				if (m_Stopwatch2.ElapsedMilliseconds - (long)(m_dfDisplayCount[2] * 33.3f) >= 33)
				{
					bDisplay = true;
				}
			}

			if (bDisplay)
			{
				m_dfDisplayCount[2]++;

				// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				// in order to avoid affecting the frame rate and the real-time of acquiring images.
				// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				// however, the user can malloc memory and copy image data.
				dvpStatus status = DVPCamera.dvpDrawPicture(ref refFrame, pBuffer,
					m_ptr_wnd2, (IntPtr)0, (IntPtr)0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}
			return 1;
        }
        public static int _dvpStreamCallback3(/*dvpHandle*/uint handle, dvpStreamEvent _event, /*void **/IntPtr pContext, ref dvpFrame refFrame, /*void **/IntPtr pBuffer)
        {
			bool bDisplay = false;

			if (m_dfDisplayCount[3] == 0)
			{
				m_Stopwatch3.Restart();
				bDisplay = true;
			}
			else
			{
				if (m_Stopwatch3.ElapsedMilliseconds - (long)(m_dfDisplayCount[3] * 33.3f) >= 33)
				{
					bDisplay = true;
				}
			}

			if (bDisplay)
			{
				m_dfDisplayCount[3]++;

				// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				// in order to avoid affecting the frame rate and the real-time of acquiring images.
				// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				// however, the user can malloc memory and copy image data.
				dvpStatus status = DVPCamera.dvpDrawPicture(ref refFrame, pBuffer,
					m_ptr_wnd3, (IntPtr)0, (IntPtr)0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}
			return 1;
        }

        private void OpenDev_Click(object sender, EventArgs e)
        {
            dvpStatus status;
            string strName;
            if (!IsValidHandle(m_handle_0))
            {
                strName = DevNameCombo.Text;
                if (strName != "")
                {
                    // Open the pointed device by the selected FriendlyName.
                    status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, ref m_handle_0);
                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
						DevNameCombo.Enabled = true;
						MessageBox.Show("Open the device failed!");
                    }
                    else
                    {
						DevNameCombo.Enabled = false;
						FriendlyName.Text = strName;

                        // If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                        // Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
                        _proc0 = _dvpStreamCallback0;
                        using (Process curProcess = Process.GetCurrentProcess())
                        using (ProcessModule curModule = curProcess.MainModule)
                        {
							status = DVPCamera.dvpRegisterStreamCallback(m_handle_0, _proc0, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr0);
							Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        }                       
                    }
                }
            }
            else
            {
				// check camear
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle_0, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// init display count is 0
					m_dfDisplayCount[0] = 0;

					// stop camera
					status = DVPCamera.dvpStop(m_handle_0);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				}

                status = DVPCamera.dvpClose(m_handle_0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_0 = 0;
				DevNameCombo.Enabled = true;
				pictureBox.Refresh();
            }

            InitCtrls_0();
        }

        private void OpenDev1_Click(object sender, EventArgs e)
        {
            dvpStatus status;
            string strName;

            if (!IsValidHandle(m_handle_1))
            {
                strName = DevNameCombo1.Text;
                if (strName != "")
                {
                    // Open the pointed device by the selected FriendlyName.
                    status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, ref m_handle_1);
                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
						DevNameCombo1.Enabled = true;
						MessageBox.Show("Open the device failed!");
                    }
                    else
                    {
						DevNameCombo1.Enabled = false;
						FriendlyName1.Text = strName;

                        // If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                        // Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
                        _proc1 = _dvpStreamCallback1;
                        using (Process curProcess = Process.GetCurrentProcess())
                        using (ProcessModule curModule = curProcess.MainModule)
                        {
							status = DVPCamera.dvpRegisterStreamCallback(m_handle_1, _proc1, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr1);
							Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        }
                    }
                }
            }

            else
            {
				// check camear
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle_1, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// init display count is 0
					m_dfDisplayCount[1] = 0;

					// stop camera
					status = DVPCamera.dvpStop(m_handle_1);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}

                status = DVPCamera.dvpClose(m_handle_1);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_1 = 0;
				DevNameCombo1.Enabled = true;
				pictureBox1.Refresh();
            }

            InitCtrls_1();
        }

        private void OpenDev2_Click(object sender, EventArgs e)
        {
            dvpStatus status;
            string strName;

            if (!IsValidHandle(m_handle_2))
            {
                strName = DevNameCombo2.Text;
                if (strName != "")
                {
                    // Open the pointed device by the selected FriendlyName.
                    status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, ref m_handle_2);
                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
						DevNameCombo2.Enabled = true;
						MessageBox.Show("Open the device failed!");
                    }
                    else
                    {
						DevNameCombo2.Enabled = false;
						FriendlyName2.Text = strName;

                        // If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                        // Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
                        _proc2 = _dvpStreamCallback2;
                        using (Process curProcess = Process.GetCurrentProcess())
                        using (ProcessModule curModule = curProcess.MainModule)
                        {
							status = DVPCamera.dvpRegisterStreamCallback(m_handle_2, _proc2, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr2);
							Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        }
                    }
                }
            }

            else
            {
				// check camear
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle_2, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// init display count is 0
					m_dfDisplayCount[2] = 0;

					// stop camera
					status = DVPCamera.dvpStop(m_handle_2);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}

                status = DVPCamera.dvpClose(m_handle_2);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_2 = 0;
				DevNameCombo2.Enabled = true;
				pictureBox2.Refresh();
            }

            InitCtrls_2();
        }

        private void OpenDev3_Click(object sender, EventArgs e)
        {
            dvpStatus status;
            string strName;

            if (!IsValidHandle(m_handle_3))
            {
                strName = DevNameCombo3.Text;
                if (strName != "")
                {
                    // Open the pointed device by the selected FriendlyName.
                    status = DVPCamera.dvpOpenByName(strName, dvpOpenMode.OPEN_NORMAL, ref m_handle_3);
                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
						DevNameCombo3.Enabled = true;
						MessageBox.Show("Open the device failed!");
                    }
                    else
                    {
						DevNameCombo3.Enabled = false;
						FriendlyName3.Text = strName;

                        // If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                        // Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
                        _proc3 = _dvpStreamCallback3;
                        using (Process curProcess = Process.GetCurrentProcess())
                        using (ProcessModule curModule = curProcess.MainModule)
                        {
							status = DVPCamera.dvpRegisterStreamCallback(m_handle_3, _proc3, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr3);
							Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        }
                    }
                }
            }

            else
            {
				// check camear
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle_3, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// init display count is 0
					m_dfDisplayCount[3] = 0;

					// stop camera
					status = DVPCamera.dvpStop(m_handle_3);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}

                status = DVPCamera.dvpClose(m_handle_3);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_3 = 0;
				DevNameCombo3.Enabled = true;
				pictureBox3.Refresh();
            }

            InitCtrls_3();
        }

        private void StartPlay_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount[0] = 0;

            if (IsValidHandle(m_handle_0))
            {
                dvpStreamState state = new dvpStreamState();
                dvpStatus status = new dvpStatus();

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle_0, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle_0);
                }
                else
                {
                    status = DVPCamera.dvpStart(m_handle_0);
                }

				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                InitCtrls_0();
            }
        }

        private void StartPlay1_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount[1] = 0;

            if (IsValidHandle(m_handle_1))
            {
                dvpStreamState state = new dvpStreamState();
                dvpStatus status = new dvpStatus();

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle_1, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle_1);
                }
                else
                {
                    status = DVPCamera.dvpStart(m_handle_1);
                }

				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                InitCtrls_1();
            }
        }

        private void StartPlay2_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount[2] = 0;

            if (IsValidHandle(m_handle_2))
            {
                dvpStreamState state = new dvpStreamState();
                dvpStatus status = new dvpStatus();

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle_2, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle_2);
                }
                else
                {
                    status = DVPCamera.dvpStart(m_handle_2);
                }

				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                InitCtrls_2();
            }
        }

        private void StartPlay3_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount[3] = 0;

            if (IsValidHandle(m_handle_3))
            {
                dvpStreamState state = new dvpStreamState();
                dvpStatus status = new dvpStatus();

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle_3, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle_3);
                }
                else
                {
                    status = DVPCamera.dvpStart(m_handle_3);
                }

				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                InitCtrls_3();
            }
        }

        private void PropertSet_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_0))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle_0, this.Handle);
            }
        }

        private void PropertSet1_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_1))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle_1, this.Handle);
            }
        }

        private void PropertSet2_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_2))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle_2, this.Handle);
            }
        }

        private void PropertSet3_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_3))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle_3, this.Handle);
            }
        }

        private void SetUserID_Click_1(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_0))
            {
                string str;
                str = EditUserID.Text;
                uint Length = (uint)str.Length;

                // Set the user ID.
                dvpStatus status = DVPCamera.dvpSetUserId(m_handle_0, str, ref Length);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					SaveBind.Enabled = true;
					m_info0[DevNameCombo.SelectedIndex].UserID = str;

					for (int item = 0; item < 16; item++)
					{
						if (m_info1[item].FriendlyName == m_info0[DevNameCombo.SelectedIndex].FriendlyName)
						{
							m_info1[item].UserID = str;
							EditUserID1.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info2[item].FriendlyName == m_info0[DevNameCombo.SelectedIndex].FriendlyName)
						{
							m_info2[item].UserID = str;
							EditUserID2.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info3[item].FriendlyName == m_info0[DevNameCombo.SelectedIndex].FriendlyName)
						{
							m_info3[item].UserID = str;
							EditUserID3.Text = str;
						}
					}
				}
            }
        }

        private void SetUserID1_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_1))
            {
                string str;
                str = EditUserID1.Text;
                uint Length = (uint)str.Length;

                // Set the user ID.
                dvpStatus status = DVPCamera.dvpSetUserId(m_handle_1, str, ref Length);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					SaveBind.Enabled = true;
					m_info1[DevNameCombo1.SelectedIndex].UserID = str;

					for (int item = 0; item < 16; item++)
					{
						if (m_info0[item].FriendlyName == m_info1[DevNameCombo1.SelectedIndex].FriendlyName)
						{
							m_info0[item].UserID = str;
							EditUserID.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info2[item].FriendlyName == m_info1[DevNameCombo1.SelectedIndex].FriendlyName)
						{
							m_info2[item].UserID = str;
							EditUserID2.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info3[item].FriendlyName == m_info1[DevNameCombo1.SelectedIndex].FriendlyName)
						{
							m_info3[item].UserID = str;
							EditUserID3.Text = str;
						}
					}
				}
            }
        }

        private void SetUserID2_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_2))
            {
                string str;
                str = EditUserID2.Text;
                uint Length = (uint)str.Length;

                // Set the user ID.
                dvpStatus status = DVPCamera.dvpSetUserId(m_handle_2, str, ref Length);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					SaveBind.Enabled = true;
					m_info2[DevNameCombo2.SelectedIndex].UserID = str;

					for (int item = 0; item < 16; item++)
					{
						if (m_info0[item].FriendlyName == m_info2[DevNameCombo2.SelectedIndex].FriendlyName)
						{
							m_info0[item].UserID = str;
							EditUserID.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info1[item].FriendlyName == m_info2[DevNameCombo2.SelectedIndex].FriendlyName)
						{
							m_info1[item].UserID = str;
							EditUserID1.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info3[item].FriendlyName == m_info2[DevNameCombo2.SelectedIndex].FriendlyName)
						{
							m_info3[item].UserID = str;
							EditUserID3.Text = str;
						}
					}
				}
            }
        }

        private void SetUserID3_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle_3))
            {
                string str;
                str = EditUserID3.Text;
                uint Length = (uint)str.Length;

                // Set the user ID.
                dvpStatus status = DVPCamera.dvpSetUserId(m_handle_3, str, ref Length);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					SaveBind.Enabled = true;
					m_info3[DevNameCombo3.SelectedIndex].UserID = str;

					for (int item = 0; item < 16; item++)
					{
						if (m_info0[item].FriendlyName == m_info3[DevNameCombo3.SelectedIndex].FriendlyName)
						{
							m_info0[item].UserID = str;
							EditUserID.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info1[item].FriendlyName == m_info3[DevNameCombo3.SelectedIndex].FriendlyName)
						{
							m_info1[item].UserID = str;
							EditUserID1.Text = str;
						}
					}
					for (int item = 0; item < 16; item++)
					{
						if (m_info2[item].FriendlyName == m_info3[DevNameCombo3.SelectedIndex].FriendlyName)
						{
							m_info2[item].UserID = str;
							EditUserID2.Text = str;
						}
					}
				}
                
            }
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern int WriteProfileString(string lpszSection, string lpszKeyName, string lpszString);
        [DllImport("kernel32")]
        private static extern bool GetProfileString(string lpApplicationName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize);

        private void SaveBind_Click(object sender, EventArgs e)
        {
            string strUserId0 = EditUserID.Text;
            string strUserId1 = EditUserID1.Text;
            string strUserId2 = EditUserID2.Text;
            string strUserId3 = EditUserID3.Text;
            bool b_error_flg = false;

            if ( strUserId0.Length > 0)
            {
                if (strUserId1.Length > 0)
                {
                    if (strUserId0 == strUserId1)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId2.Length > 0)
                {
                    if (strUserId0 == strUserId2)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId3.Length > 0)
                {
                    if (strUserId0 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (strUserId1.Length > 0)
            {               
                if (strUserId2.Length > 0)
                {
                    if (strUserId1 == strUserId2)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId3.Length > 0)
                {
                    if (strUserId1 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (strUserId2.Length > 0)
            {
                if (strUserId3.Length > 0)
                {
                    if (strUserId2 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (b_error_flg)
            {
                MessageBox.Show(("User id repetition"), ("Save Failed"));
                return;
            }

            // If there is not repetitive "user id",save the binding relationship into registry.
            WriteProfileString(("UserId"), ("0"), strUserId0);
            WriteProfileString(("UserId"), ("1"), strUserId1);
            WriteProfileString(("UserId"), ("2"), strUserId2);
            WriteProfileString(("UserId"), ("3"), strUserId3);
            SaveBind.Enabled = false;

        }

        private void LoadBind_Click(object sender, EventArgs e)
        {
            StringBuilder strUserId0 = new StringBuilder(256);
            StringBuilder strUserId1 = new StringBuilder(256);
            StringBuilder strUserId2 = new StringBuilder(256);
            StringBuilder strUserId3 = new StringBuilder(256);
			dvpStatus status;
            string str = "";
            GetProfileString(("UserId"), ("0"), str, strUserId0, 256);
            GetProfileString(("UserId"), ("1"), str, strUserId1, 256);
            GetProfileString(("UserId"), ("2"), str, strUserId2, 256);
            GetProfileString(("UserId"), ("3"), str, strUserId3, 256);

            bool b_error_flg = false;

            if (strUserId0.Length > 0)
            {
                if (strUserId1.Length > 0)
                {
                    if (strUserId0 == strUserId1)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId2.Length > 0)
                {
                    if (strUserId0 == strUserId2)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId3.Length > 0)
                {
                    if (strUserId0 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (strUserId1.Length > 0)
            {
                if (strUserId2.Length > 0)
                {
                    if (strUserId1 == strUserId2)
                    {
                        b_error_flg = true;
                    }
                }

                if (strUserId3.Length > 0)
                {
                    if (strUserId1 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (strUserId2.Length > 0)
            {
                if (strUserId3.Length > 0)
                {
                    if (strUserId2 == strUserId3)
                    {
                        b_error_flg = true;
                    }
                }
            }

            if (b_error_flg)
            {
                MessageBox.Show(("User id repetition"), ("Save Failed"));
                return;
            }

            // If camera have been opened, close camera one by one.
            if (IsValidHandle(m_handle_0))
            {
                status = DVPCamera.dvpClose(m_handle_0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_0 = 0;
            }
            if (IsValidHandle(m_handle_1))
            {
                status = DVPCamera.dvpClose(m_handle_1);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_1 = 0;
            }
            if (IsValidHandle(m_handle_2))
            {
                status = DVPCamera.dvpClose(m_handle_2);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_2 = 0;
            }
            if (IsValidHandle(m_handle_3))
            {
                status = DVPCamera.dvpClose(m_handle_3);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle_3 = 0;
            }
			string strNmae = "";

            // Open cameras by "user id".
			if (OpenByUserId(strUserId0, ref m_handle_0))
			{
				// Initialize cameras.
				DevNameCombo.Enabled = false;
				_proc0 = _dvpStreamCallback0;
				using (Process curProcess = Process.GetCurrentProcess())
				using (ProcessModule curModule = curProcess.MainModule)
				{
					status = DVPCamera.dvpRegisterStreamCallback(m_handle_0, _proc0, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr0);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				FriendlyName.Text = strNmae;
			}
			if (OpenByUserId(strUserId1, ref m_handle_1))
			{
				// Initialize cameras.				
				DevNameCombo1.Enabled = false;
				_proc1 = _dvpStreamCallback1;
				using (Process curProcess = Process.GetCurrentProcess())
				using (ProcessModule curModule = curProcess.MainModule)
				{
					status = DVPCamera.dvpRegisterStreamCallback(m_handle_1, _proc1, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr0);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				FriendlyName1.Text = strNmae;
			}
			if (OpenByUserId(strUserId2, ref m_handle_2))
			{
				// Initialize cameras.			
				DevNameCombo2.Enabled = false;
				_proc2 = _dvpStreamCallback2;
				using (Process curProcess = Process.GetCurrentProcess())
				using (ProcessModule curModule = curProcess.MainModule)
				{
					status = DVPCamera.dvpRegisterStreamCallback(m_handle_2, _proc2, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr0);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				FriendlyName2.Text = strNmae;
			}
			if (OpenByUserId(strUserId3, ref m_handle_3))
			{
				// Initialize cameras.				
				DevNameCombo3.Enabled = false;
				_proc3 = _dvpStreamCallback3;
				using (Process curProcess = Process.GetCurrentProcess())
				using (ProcessModule curModule = curProcess.MainModule)
				{
					status = DVPCamera.dvpRegisterStreamCallback(m_handle_3, _proc3, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr0);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				FriendlyName3.Text = strNmae;
			}

			InitCtrls_0();
			InitCtrls_1();
			InitCtrls_2();
			InitCtrls_3();
		}

		public bool OpenByUserId(StringBuilder UserId, ref uint pHandle)
        {
			string str_UserId = UserId.ToString();

            if (UserId.Length < 1)
	        {
		        return false;
	        }

            dvpStatus status = new dvpStatus(); ;

			// user dvpOpenByUserId open the camear
			status = DVPCamera.dvpOpenByUserId(str_UserId, dvpOpenMode.OPEN_NORMAL, ref pHandle);
			// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            if (status == dvpStatus.DVP_STATUS_OK)
			{
				return true;
			}
			else
			{
                // If open the device unsuccessfully, Get a corresponding referenced string according to the status code.
                // Note: This string may change according to the language and version.
				MessageBox.Show("Open the device failed!");
				return false;
			}
        }

        private void MultipleCamera_FormClosing(object sender, FormClosingEventArgs e)
        {
			dvpStatus status;
			dvpStreamState state = new dvpStreamState();

            if (IsValidHandle(m_handle_0))
            {
				status = DVPCamera.dvpGetStreamState(m_handle_0, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					status = DVPCamera.dvpStop(m_handle_0);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				status = DVPCamera.dvpClose(m_handle_0);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                m_handle_0 = 0;
            }
            if (IsValidHandle(m_handle_1))
            {
				status = DVPCamera.dvpGetStreamState(m_handle_1, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					status = DVPCamera.dvpStop(m_handle_1);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				status = DVPCamera.dvpClose(m_handle_1);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				m_handle_1 = 0;
            }
            if (IsValidHandle(m_handle_2))
            {
				status = DVPCamera.dvpGetStreamState(m_handle_2, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					status = DVPCamera.dvpStop(m_handle_2);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				status = DVPCamera.dvpClose(m_handle_2);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				m_handle_2 = 0;
            }
            if (IsValidHandle(m_handle_3))
            {
				status = DVPCamera.dvpGetStreamState(m_handle_3, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					status = DVPCamera.dvpStop(m_handle_3);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				status = DVPCamera.dvpClose(m_handle_3);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				m_handle_3 = 0;
            }
        }

		private void DevNameCombo_SelectedIndexChanged(object sender, EventArgs e)
		{
			EditUserID.Text = m_info0[DevNameCombo.SelectedIndex].UserID;
		}

		private void DevNameCombo1_SelectedIndexChanged(object sender, EventArgs e)
		{
			EditUserID1.Text = m_info1[DevNameCombo1.SelectedIndex].UserID;
		}

		private void DevNameCombo2_SelectedIndexChanged(object sender, EventArgs e)
		{
			EditUserID2.Text = m_info2[DevNameCombo2.SelectedIndex].UserID;
		}

		private void DevNameCombo3_SelectedIndexChanged(object sender, EventArgs e)
		{
			EditUserID3.Text = m_info3[DevNameCombo3.SelectedIndex].UserID;
		}

		private void pictureBox_Paint(object sender, PaintEventArgs e)
		{

		}

		private void pictureBox_BackColorChanged(object sender, EventArgs e)
		{

		}

		private void FriendlyName_TextChanged(object sender, EventArgs e)
		{
			
		}
    }
}
