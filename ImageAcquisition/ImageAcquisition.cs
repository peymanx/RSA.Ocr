using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DVPCameraType;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace ImageAcquisition
{
    public partial class ImageAcquisition : Form
    {
        public uint m_handle = 0;
        public bool m_bAeOp = false;
        public int m_n_dev_count = 0;
        string m_strFriendlyName;
        
        public static IntPtr m_ptr_wnd = new IntPtr();
        public static IntPtr m_ptr = new IntPtr();
        public static bool m_b_start = false;
        public static int m_uImageCount = 0;
        static AutoResetEvent m_ThreadEvent = new AutoResetEvent(false);
        Thread m_pThread;
        public string m_strfilepath;
        public string m_strfiledir;

		// Display param
		public static Stopwatch m_Stopwatch = new Stopwatch();
		public static Double m_dfDisplayCount = 0;

		public static dvpCameraInfo[] m_info = new dvpCameraInfo[16];
		public static int m_CamCount = 0;

        public ImageAcquisition()
        {
            Control.CheckForIllegalCrossThreadCalls = false;

			InitializeComponent();

			// Initialize open mode
			// false: user dvpOpenByName open the camear
			// true : user dvpOpenByUserId open the camear
			UserDefinedName.Checked = false;

            InitDevList();

            Display.Checked = true;
            Form.CheckForIllegalCrossThreadCalls = false;
            System.Timers.Timer t = new System.Timers.Timer(500);
            
            // Execute the event when the time has arrived. 
            t.Elapsed += new System.Timers.ElapsedEventHandler(theout);
            
            // Set the method of executing
            t.AutoReset = true;

            // Judge whether execute the System.Timers.Timer.Elapsed event.
            t.Enabled = true;

            m_strfiledir = System.Windows.Forms.Application.StartupPath;           
            EditPath.Text = m_strfiledir;

			// Set the time out as 5000MS,wait time as 0.
			GrabTimeout.Text = "1";
			EditWait.Text = "1000";

			m_ptr_wnd = pictureBoxMain.Handle;
        }

        public void theout(object source, System.Timers.ElapsedEventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                // Update the information of frame rate
                dvpFrameCount count = new dvpFrameCount();
                dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				string str;
				if (m_dfDisplayCount == 0 || m_Stopwatch.ElapsedMilliseconds < 2)
				{
					str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps]";
				}
				else
				{
					str = m_strFriendlyName + " [" + count.uFrameCount.ToString() + " frames, "
						+ string.Format("{0:#0.00}", count.fFrameRate) + " fps, Display "
						+ string.Format("{0:#0.00}", m_dfDisplayCount * 1000.0f / m_Stopwatch.ElapsedMilliseconds) + " fps]";
				}
                this.Text = str;
                Counts.Text = m_uImageCount.ToString();
            }
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

        public void InitDevList()
        {
            dvpStatus status;
            uint i, n = 0;
            dvpCameraInfo dev_info = new dvpCameraInfo();

            // "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
            DevNameCombo.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            m_n_dev_count = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
				m_CamCount = 0;

                for (i = 0; i < n; i++)
                {
                    // Acquire each camera's information one by one.
                    status = DVPCamera.dvpEnum(i, ref dev_info);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
						m_info[m_CamCount] = dev_info;

						int item = -1;
						if (!UserDefinedName.Checked)
						{
							// add FriendlyName
							item = DevNameCombo.Items.Add(dev_info.FriendlyName);
						}
						else
						{
							// add User Define Name
							item = DevNameCombo.Items.Add(dev_info.UserID);
						}
						if (item == 0)
						{
							DevNameCombo.SelectedIndex = item;
						}
						m_CamCount++;

                        if (item == 0)
                        {
                            DevNameCombo.SelectedIndex = item;
                        }
                    }
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

            UpdateControls();          
        }

        private void UpdateControls()
        {
	        dvpStatus status;
	        if (IsValidHandle(m_handle))
	        {
                // The device has been opened at this time.
                // Update the basic controls.
                dvpStreamState state = dvpStreamState.STATE_STOPED;
                status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                OpenDev.Text = "Close";
                StartPlay.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : "Start";
                StartPlay.Enabled = true;
                PropertSet.Enabled = true;

				bool bSoftTrigger = false;
				status = DVPCamera.dvpGetTriggerState(m_handle, ref bSoftTrigger);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					if (state == dvpStreamState.STATE_STARTED )
					{
						SoftTrigger.Enabled = false;
					}
					else
					{
						
						SoftTrigger.Enabled = true;
					}
				}
				else
				{
					SoftTrigger.Enabled = false;
				}

                // Update the related controls.
                if (state == dvpStreamState.STATE_STARTED)
		        {
                    // If the thread of acquiring images is still running,it can simplify multithreaded synchronization by disabling some operations.
                    SaveImageCheck.Enabled = false;
                    Display.Enabled = false;
                    SelectPath.Enabled = false;
                    EditPath.Enabled = false;
                    EditWait.Enabled = false;
					
					GrabTimeout.Enabled = false;
		        }
		        else
		        {
                    if ( DevNameCombo.Items.Count > 0 )
                        OpenDev.Enabled = true;
                    else
                        OpenDev.Enabled = false;

                    SaveImageCheck.Enabled = true;
                    Display.Enabled = true;
                    SelectPath.Enabled = true;
                    EditPath.Enabled = true;
                    OpenFolder.Enabled = true;
                    EditWait.Enabled = true;
					GrabTimeout.Enabled = true;
		        }
	        }
	        else
	        {
                // No device is opened at this time.
                // Update the basic controls.
                OpenDev.Text = "Open";
                StartPlay.Enabled = false;
                PropertSet.Enabled = false;
                OpenDev.Enabled = m_n_dev_count > 0 ? true : false;

                // Update the related controls.
                SaveImageCheck.Enabled = false;
                Display.Enabled = false;
                SelectPath.Enabled = false;
                OpenFolder.Enabled = false;
                SaveImageCheck.Enabled = false;	   
    			SoftTrigger.Enabled = false;
				SoftTrigger.Checked = false;
	        }	       
        }

        private void ScanDev_Click(object sender, EventArgs e)
        {
            InitDevList();
        }

        private void OpenDev_Click(object sender, EventArgs e)
        {
            dvpStatus status;
 
            if (!IsValidHandle(m_handle))
            {
				if (DevNameCombo.Text != "")
                {
					if (UserDefinedName.Checked)
					{
						// Open the specific device by the selected user define name.
						status = DVPCamera.dvpOpenByUserId(DevNameCombo.Text, dvpOpenMode.OPEN_NORMAL, ref m_handle);
					}
					else
					{
						// Open the specific device by the selected FriendlyName.
						status = DVPCamera.dvpOpenByName(DevNameCombo.Text, dvpOpenMode.OPEN_NORMAL, ref m_handle);
					}

                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
                        MessageBox.Show("Open failed!");
                    }
                    else
                    {
						m_strFriendlyName = DevNameCombo.Text;
                    }
                }
            }
            else
            {
				// check camear
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// init display count is 0
					m_dfDisplayCount = 0;

					// stop camera
					status = DVPCamera.dvpStop(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

					if ((m_pThread.ThreadState & System.Threading.ThreadState.Stopped) == 0)
					{
						m_ThreadEvent.Set();
						m_pThread.Abort();
					}
				}

				// cloas camera
                status = DVPCamera.dvpClose(m_handle);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle = 0;
				pictureBoxMain.Refresh();
            }

            UpdateControls();
        }

        // private void StsartPlay_Click(object sender, EventArgs e)
        private void StartPlay_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount = 0;

            if (IsValidHandle(m_handle))
            {
                dvpStreamState state = dvpStreamState.STATE_STOPED;
                dvpStatus status;

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state != dvpStreamState.STATE_STOPED)
                {
                    status = DVPCamera.dvpStop(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					
					if ((m_pThread.ThreadState & System.Threading.ThreadState.Stopped) == 0)
                    {
                        m_ThreadEvent.Set();
						m_pThread.Abort();
                    }

                }
                else
                {
                    // Set as Trigger mode before starting video stream.
					bool bFlg = false;
					status = DVPCamera.dvpGetTriggerState(m_handle, ref bFlg);
					if (status == dvpStatus.DVP_STATUS_OK)
					{
						status = DVPCamera.dvpSetTriggerState(m_handle, SoftTrigger.Checked ? true : false);
						if (status != dvpStatus.DVP_STATUS_OK)
						{
							// Confirm whether the camera support the trigger function.
							MessageBox.Show("Set the trigger mode failed!");
						}
					}

					
                    status = DVPCamera.dvpStart(m_handle);
                    if (status != dvpStatus.DVP_STATUS_OK)
                    {
						MessageBox.Show("Start the video stream failed!");
                    }
                    else 
                    {
                        // Reset the event of exitting thread.
                        m_ThreadEvent.Reset();

                        // Create the thread of acquiring image.
                        ParameterizedThreadStart ParStart = new ParameterizedThreadStart(ImageThread);
                        m_pThread = new Thread(ParStart);
                        m_pThread.Start(this);
                        m_b_thread_start = true;

                    }                                      
                }
            }

            UpdateControls();
        }

        static void ImageThread(object obj)
        {
            ImageAcquisition ia = (ImageAcquisition)obj;
            ia.ImageThread();
            return ;
        }

        public static bool m_b_thread_start = false;
        public void ImageThread()
        {
	        dvpStatus status;           

            // Wait the event of exitting thread.
            int time_out = 40;
            if (EditWait.Text.Length > 0)
            {
				int.TryParse(EditWait.Text, out time_out);
				EditWait.Text = time_out.ToString();
            }
			GrabTimeout.Text = ((uint)(float.Parse(GrabTimeout.Text))).ToString();
            while (!m_ThreadEvent.WaitOne(time_out))
            {
		        IntPtr pBuffer = new IntPtr();
                dvpFrame frame = new dvpFrame();

                if ( SoftTrigger.Checked )
                {
                    bool bSoftTrigger = false;

                    status = DVPCamera.dvpGetTriggerState(m_handle, ref bSoftTrigger);
                    // Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    if (bSoftTrigger)
                    {
                        bool bSoftTriggerLoop = false;
                        status = DVPCamera.dvpGetSoftTriggerLoopState(m_handle, ref bSoftTriggerLoop);
                        // Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        if (!bSoftTriggerLoop)
                        {
                            status = DVPCamera.dvpTriggerFire(m_handle);
                            if (status != dvpStatus.DVP_STATUS_OK)
                            {
                                // Trigger failure maybe result from that the trigger signal interval is too dense.
                                // Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
								continue;
                            }
                        }
                    }
                }

                // Grab a frame image from the video stream and timeout should not less than the current exposure time.
				status = DVPCamera.dvpGetFrame(m_handle, ref frame, ref pBuffer, ((uint)(float.Parse(GrabTimeout.Text))));
		        if (status != dvpStatus.DVP_STATUS_OK)
		        {
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    continue;
		        }

                // If need to save images.
		        if ( SaveImageCheck.Checked )
                {
                    // Save the bmp image by using systematic time as filename.
                    DateTime now = DateTime.Now;
                    m_strfilepath = m_strfiledir + "\\" + now.ToString("yyyy-MM-dd") + " " + now.TimeOfDay.ToString().Replace(":", "-").Replace(".", "-") +".bmp";
                   
                    status = DVPCamera.dvpSavePicture(ref frame, pBuffer, m_strfilepath, 100);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    m_uImageCount++;
		        }

                // Display images in the preview window, if needed.
		        if ( Display.Checked )
		        {
					bool bDisplay = false;

					if (m_dfDisplayCount == 0)
					{
						m_Stopwatch.Restart();
						bDisplay = true;
					}
					else
					{
						if (m_Stopwatch.ElapsedMilliseconds - (long)(m_dfDisplayCount * 33.3f) >= 33)
						{
							bDisplay = true;
						}
					}

					if (bDisplay)
					{
						m_dfDisplayCount++;

						// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
						// in order to avoid affecting the frame rate and the real-time of acquiring images.
						// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
						// however, the user can malloc memory and copy image data.
						status = DVPCamera.dvpDrawPicture(ref frame, pBuffer,
							m_ptr_wnd, (IntPtr)0, (IntPtr)0);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					}
		        }		
	        }

            m_pThread.Abort();
            m_b_thread_start = false;
	        return ; 
        }
       
        private void PropertSet_Click(object sender, EventArgs e)
        {
			dvpStatus status;
            if (IsValidHandle(m_handle))
            {
				status = DVPCamera.dvpShowPropertyModalDialog(m_handle, this.Handle);
				
                // At this time some configurations may change, synchronize it to the window GUI.
                UpdateControls();
            }
        }

        private void SelectPath_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                m_strfiledir = dlg.SelectedPath;
                EditPath.Text = m_strfiledir;
            }
        }

        [DllImport("shell32.dll")]
        static extern IntPtr ShellExecute(
            IntPtr hwnd,
            string lpOperation,
            string lpFile,
            string lpParameters,
            string lpDirectory,
            uint nShowCmd);

        private void OpenFolder_Click(object sender, EventArgs e)
        {
			ShellExecute(m_ptr, ("open"), m_strfiledir, "", "", 1);
        }

        private void ImageAcquisition_FormClosing(object sender, FormClosingEventArgs e)
        {
			dvpStatus status;
			dvpStreamState state = new dvpStreamState();

            if (IsValidHandle(m_handle))
            {
				// Close the thread of grabbing images.
                if (m_b_thread_start)
                {
                    m_ThreadEvent.Set();
					m_pThread.Abort();
                }

				status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					// Stop the video stream.
					status = DVPCamera.dvpStop(m_handle);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				}

				// Close the camera.
				status = DVPCamera.dvpClose(m_handle);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				m_handle = 0;

			}
        }

		private void SoftTrigger_CheckedChanged(object sender, EventArgs e)
		{
			if (IsValidHandle(m_handle) )
			{
				dvpStatus status ;
				
				// get Stream State
				dvpStreamState StreamState = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle, ref StreamState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				
				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// Stop the video stream.
					status = DVPCamera.dvpStop(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}

				// Set Trigger State
				status = DVPCamera.dvpSetTriggerState(m_handle, SoftTrigger.Checked);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// Start the video stream.
					status = DVPCamera.dvpStart(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
			}
			
		}

		private void ResizeWindows()
		{
			if (IsValidHandle(m_handle))
			{
				dvpRegion roi;
				roi = new dvpRegion();
				dvpStatus status;
				status = DVPCamera.dvpGetRoi(m_handle, ref roi);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				pictureBoxMain.Width = this.Width - pictureBoxMain.Left;
				pictureBoxMain.Height = this.Height - pictureBoxMain.Top;

				if (pictureBoxMain.Width * roi.H > pictureBoxMain.Height * roi.W)
				{
					pictureBoxMain.Width = pictureBoxMain.Height * roi.W / roi.H;
				}
				else
				{
					pictureBoxMain.Height = pictureBoxMain.Width * roi.H / roi.W;
				}
			}
		}

		private void ImageAcquisition_Resize(object sender, EventArgs e)
		{
			ResizeWindows();
		}

		private void UserDefineName_CheckedChanged(object sender, EventArgs e)
		{
			// save cur sel item
			string strName;
			strName = DevNameCombo.Text;

			// reset m_listDevices values
			DevNameCombo.Items.Clear();

			for (int i = 0; i < m_CamCount; i++)
			{
				int item = -1;
				if ( !UserDefinedName.Checked )
				{
					item = DevNameCombo.Items.Add(m_info[i].FriendlyName);
					if (strName == m_info[i].UserID)
					{
						DevNameCombo.SelectedIndex = item;
					}
				}
				else
				{
					// check User define name is null
					if ( m_info[i].UserID.Length == 0 )
						continue;

					item = DevNameCombo.Items.Add(m_info[i].UserID);
					if (strName == m_info[i].FriendlyName )
					{
						DevNameCombo.SelectedIndex = item;
					}
				}
			}
		}

        private void ImageAcquisition_Load(object sender, EventArgs e)
        {

        }
    }

}
