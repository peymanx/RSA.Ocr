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
using System.Threading;
using System.Runtime.InteropServices;


namespace BasicFunction
{
    public partial class BasicFunction : Form
    {

        public uint m_handle = 0;
        public bool m_bAeOp = false;
        public int m_n_dev_count = 0;
        string m_strFriendlyName;

        public static IntPtr m_ptr_wnd = new IntPtr();
        public static IntPtr m_ptr = new IntPtr();
        public static bool m_b_start = false;

		// Display param
		public static Stopwatch m_Stopwatch = new Stopwatch();
		public static Double m_dfDisplayCount = 0;

		public static dvpCameraInfo []m_info = new dvpCameraInfo[16];
		public static int m_CamCount = 0;

		public static Boolean m_bSaveImageFlg;
		public static String m_ImagePath;

        public BasicFunction()
        {   
            Form.CheckForIllegalCrossThreadCalls = false;		
            InitializeComponent();

			// Initialize open mode
			// false: user dvpOpenByName open the camear
			// true : user dvpOpenByUserId open the camear
			UserDefinedName.Checked = false;
			
            m_ptr_wnd = pictureBoxMain.Handle;
			GainEdit.DecimalPlaces = 3;
			GainEdit.Increment = 0.125M;
            System.Timers.Timer t = new System.Timers.Timer(500);

            // Execute the event when the time has arrived. 
            t.Elapsed += new System.Timers.ElapsedEventHandler(theout);

            // Set the method of executing
            t.AutoReset = true;

            // Judge whether execute the System.Timers.Timer.Elapsed event.
            t.Enabled = true;
            InitDevList();

			m_bSaveImageFlg = false;
			m_ImagePath = "";
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

        private void BUTTON_SCAN_Click(object sender, EventArgs e)
        {
            InitDevList();
        }

        // Initialize the device list.
        public void InitDevList()
        {
            dvpStatus status;
	        uint i,n = 0;
	        dvpCameraInfo dev_info = new dvpCameraInfo();

	        // "n" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	        COMBO_DEVICES.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            m_n_dev_count = (int)n;
            if (status == dvpStatus.DVP_STATUS_OK)
            {
				m_CamCount = 0;
	            for (i=0; i<n; i++)
	            {
                    // Acquire each camera's information one by one.
                    status = DVPCamera.dvpEnum(i, ref dev_info);

					if (status == dvpStatus.DVP_STATUS_OK)
					{
						m_info[m_CamCount] = dev_info;
						
						int item = -1;
						if (!UserDefinedName.Checked)
						{
							// add FriendlyName
							item = COMBO_DEVICES.Items.Add(dev_info.FriendlyName);
						}
						else
						{
							// add User Define Name
							item = COMBO_DEVICES.Items.Add(dev_info.UserID);
						}
						if (item == 0)
						{
							COMBO_DEVICES.SelectedIndex = item;
						}
						m_CamCount++;
					}
					else
					{
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					}
	            }
            }

            if (n == 0)
            {
                BUTTON_OPEN.Enabled = false;
            }
            else
            {
                BUTTON_OPEN.Enabled = true;
            }

            if (!IsValidHandle(m_handle))
            {
                BUTTON_START.Enabled = false;
                BUTTON_PROPERTY.Enabled = false;
                BUTTON_SAVE.Enabled = false;
                CHECK_AEOPERATION.Enabled = false;
                COMBO_AE_MODE.Enabled = false;
				ExposureTimeEdit.Enabled = false;
				ExposureTimeApply.Enabled = false;
                COMBO_FLICK.Enabled = false;
				GainEdit.Enabled = false;
				GainApply.Enabled = false;
                COMBO_RESOLUTION.Enabled = false;
            }
			else
			{
				ExposureTimeEdit.Enabled = true;
				ExposureTimeApply.Enabled = true;
				GainEdit.Enabled = true;
				GainApply.Enabled = true;
			}	
        }

        private DVPCamera.dvpStreamCallback _proc;   

        // Callback function that is used for receiving data.
        public static int _dvpStreamCallback(/*dvpHandle*/uint handle, dvpStreamEvent _event, /*void **/IntPtr pContext, ref dvpFrame refFrame, /*void **/IntPtr pBuffer)
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

			if ( bDisplay )
			{
				m_dfDisplayCount++;

				// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
				// in order to avoid affecting the frame rate and the real-time of acquiring images.
				// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
				// however, the user can malloc memory and copy image data.
				dvpStatus status = DVPCamera.dvpDrawPicture(ref refFrame, pBuffer,
					m_ptr_wnd, (IntPtr)0, (IntPtr)0);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

			}

			if ( m_bSaveImageFlg )
			{
				dvpStatus status = DVPCamera.dvpSavePicture(ref refFrame, pBuffer, m_ImagePath, 100); 
          		Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				m_bSaveImageFlg = false;
				System.Diagnostics.Process.Start(m_ImagePath);
			}

            return 0;
        }

        private void BUTTON_OPEN_Click(object sender, EventArgs e)
        {

			dvpStatus status;
            if (!IsValidHandle(m_handle))
            {
				if (UserDefinedName.Checked)
				{
					// Open the specific device by the selected user define name.
					status = DVPCamera.dvpOpenByUserId(COMBO_DEVICES.Text, dvpOpenMode.OPEN_NORMAL, ref m_handle);
				}
				else
				{
					// Open the specific device by the selected FriendlyName.
					status = DVPCamera.dvpOpenByName(COMBO_DEVICES.Text, dvpOpenMode.OPEN_NORMAL, ref m_handle);
				}
				
                if (status != dvpStatus.DVP_STATUS_OK)
                {
                    m_handle = 0;
					MessageBox.Show("Open the device failed!");
                }
                else
                {
                    m_strFriendlyName = COMBO_DEVICES.Text;     
                    _proc = _dvpStreamCallback;
                    using (Process curProcess = Process.GetCurrentProcess())
                    using (ProcessModule curModule = curProcess.MainModule)
                    {
						status = DVPCamera.dvpRegisterStreamCallback(m_handle, _proc, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    }
                   
                    InitControlResolution();
                    InitControlAeMode();
                    InitControlFlick();
                    UpdateControlExposure();
                    UpdateControlGain();
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

				}

                m_b_start = false;
				status = DVPCamera.dvpClose(m_handle);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle = 0;
				BUTTON_SAVE.Enabled = false;
				pictureBoxMain.Refresh();
            }

            UpdateControls();
        }

        public void InitControlResolution()
        {
			uint RoiSel = 0;
			dvpQuickRoi RoiDetail = new dvpQuickRoi();
	        dvpStatus status;
			dvpSelectionDescr RoiDescr = new dvpSelectionDescr();
            COMBO_RESOLUTION.Items.Clear();

            if ( !IsValidHandle(m_handle) )
            {
                COMBO_RESOLUTION.Enabled = false;
                return ;
            }

            // Get the index of the ROI.
			status = DVPCamera.dvpGetQuickRoiSelDescr(m_handle, ref RoiDescr);
			if (status == dvpStatus.DVP_STATUS_OK)
			{
				for (uint iNum = 0; iNum < RoiDescr.uCount; iNum++)
				{
					status = DVPCamera.dvpGetQuickRoiSelDetail(m_handle, iNum, ref RoiDetail);
					if (status == dvpStatus.DVP_STATUS_OK)
					{
						COMBO_RESOLUTION.Items.Add(RoiDetail.selection._string);
					}
				}
			}
			else
			{
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}

	        // Get the index of the ROI.
			status = DVPCamera.dvpGetQuickRoiSel(m_handle, ref RoiSel);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            if (status == dvpStatus.DVP_STATUS_OK)
	        {
				COMBO_RESOLUTION.SelectedIndex = (int)RoiSel;
	        }
            COMBO_RESOLUTION.Enabled = true;
        }

        public void InitControlAeMode()
        {
            COMBO_AE_MODE.Items.Clear();
            if (!IsValidHandle(m_handle))
            {
                COMBO_AE_MODE.Enabled = false;
                return;
            }

            COMBO_AE_MODE.Items.Add("AE_MODE_AE_AG");
            COMBO_AE_MODE.Items.Add("AE_MODE_AG_AE");
            COMBO_AE_MODE.Items.Add("AE_MODE_AE_ONLY");
            COMBO_AE_MODE.Items.Add("AE_MODE_AG_ONLY");
            COMBO_AE_MODE.SelectedIndex = 0;
		        
            COMBO_AE_MODE.Enabled = true;
        }

        public void InitControlFlick()
        {
            COMBO_FLICK.Items.Clear();
            if (m_handle == 0)
            {
                COMBO_FLICK.Enabled = false;
                return;
            }

            COMBO_FLICK.Items.Add("ANTIFLICK_DISABLE");
            COMBO_FLICK.Items.Add("ANTIFLICK_50HZ");
            COMBO_FLICK.Items.Add("ANTIFLICK_60HZ");
            COMBO_FLICK.SelectedIndex = 1;   
            COMBO_FLICK.Enabled = true;
        }

        public void UpdateControlExposure()
        {
	        double fExpoTime = 0.0f;
            dvpDoubleDescr ExpoTimeDescr = new dvpDoubleDescr(); ;
            dvpStatus status;

            // Get the descriptive information about the exposure time.
            status = DVPCamera.dvpGetExposureDescr(m_handle, ref ExpoTimeDescr);
			if (status == dvpStatus.DVP_STATUS_OK)
			{
				// Set the range of the exposure time.
				ExposureTimeEdit.Maximum = (decimal)ExpoTimeDescr.fMax;
				ExposureTimeEdit.Minimum = (decimal)ExpoTimeDescr.fMin;
				
			}
			else
			{
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			}

            // Get the initial value of the exposure time.
            status = DVPCamera.dvpGetExposure(m_handle, ref fExpoTime);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            if (status == dvpStatus.DVP_STATUS_OK)
	        {
                // Set the initial value of the exposure time. 
				ExposureTimeEdit.Value = (decimal)fExpoTime;
	        }
        }

        public void UpdateControlGain()
        {
	        dvpStatus  status;
	        float fAnalogGain = 0.0F;
	        dvpFloatDescr AnalogGainDescr = new dvpFloatDescr();

            // Get the descriptive information about the analog gain.
            status = DVPCamera.dvpGetAnalogGainDescr(m_handle, ref AnalogGainDescr);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            if (status == dvpStatus.DVP_STATUS_OK)
            {
                // Set the range of the analog gain.
				GainEdit.Maximum = (decimal)AnalogGainDescr.fMax;
				GainEdit.Minimum = (decimal)AnalogGainDescr.fMin;
            }

            // Get the value of the analog gain.
            status = DVPCamera.dvpGetAnalogGain(m_handle, ref fAnalogGain);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            if (status == dvpStatus.DVP_STATUS_OK)
            {
                // Set the initial value of the analog gain.
				GainEdit.Value = (decimal)(fAnalogGain);               
            }
        }

        private void BUTTON_START_Click(object sender, EventArgs e)
        {
            dvpStreamState state = dvpStreamState.STATE_STOPED;
            dvpStatus status = dvpStatus.DVP_STATUS_UNKNOW;

			// init display count is 0
			m_dfDisplayCount = 0;
			
            if ( IsValidHandle(m_handle) )
            {
                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    m_b_start = status == dvpStatus.DVP_STATUS_OK? false: true;
					BUTTON_SAVE.Enabled = false;
                }
                else
                {
					m_bSaveImageFlg = false;
                    status = DVPCamera.dvpStart(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    m_b_start = status == dvpStatus.DVP_STATUS_OK ? true : false;

					UpdateControlExposure();
					BUTTON_SAVE.Enabled = true;
                }
            }

            UpdateControls();
        }

        public void UpdateControls()
        {
            dvpStatus status = dvpStatus.DVP_STATUS_UNKNOW;
            if ( IsValidHandle(m_handle) )
	        {
                // The device has been opened at this time.
                // Update the basic controls.
                dvpStreamState state = new dvpStreamState();
                status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                BUTTON_START.Text = (state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start"));
                BUTTON_OPEN.Text = "Close";
                BUTTON_START.Enabled = true;
                BUTTON_PROPERTY.Enabled = true;
                

                // Update the related controls.
                COMBO_RESOLUTION.Enabled = true;
                COMBO_AE_MODE.Enabled = true;
                COMBO_FLICK.Enabled = true;
                CHECK_AEOPERATION.Enabled = true;

                // Update the AE operation control.
                dvpAeOperation AeOp = new dvpAeOperation();
                dvpAeMode AeMode = new dvpAeMode();

                status = DVPCamera.dvpGetAeOperation(m_handle, ref AeOp);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                status = DVPCamera.dvpGetAeMode(m_handle, ref AeMode);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (AeOp == dvpAeOperation.AE_OP_OFF)
                {
                    m_bAeOp = false;
					ExposureTimeEdit.Enabled = true;
					ExposureTimeApply.Enabled = true;
					GainEdit.Enabled = true;
					GainApply.Enabled = true;
                }
                else if (AeOp == dvpAeOperation.AE_OP_CONTINUOUS || AeOp == dvpAeOperation.AE_OP_ONCE)
                {		
                    m_bAeOp = true;
					ExposureTimeEdit.Enabled = AeMode == dvpAeMode.AE_MODE_AG_ONLY;
					ExposureTimeApply.Enabled = AeMode == dvpAeMode.AE_MODE_AG_ONLY;
					GainEdit.Enabled = AeMode == dvpAeMode.AE_MODE_AE_ONLY;
					GainApply.Enabled = AeMode == dvpAeMode.AE_MODE_AE_ONLY;
                }
                CHECK_AEOPERATION.Checked = m_bAeOp;

		        // Get the AE mode.
                COMBO_AE_MODE.SelectedIndex = (int)AeMode;

                // Update the Anti-Flick control.
                dvpAntiFlick AntiFlick = new dvpAntiFlick();
                status = DVPCamera.dvpGetAntiFlick(m_handle, ref AntiFlick);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                if (COMBO_FLICK.Items.Count > 0)
                    COMBO_FLICK.SelectedIndex = (int)AntiFlick;

                // Update the exposure time.
		        double fExpoTime = 0.0f;
                status = DVPCamera.dvpGetExposure(m_handle, ref fExpoTime);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (ExposureTimeEdit.Maximum < (decimal)fExpoTime)
					ExposureTimeEdit.Maximum = (decimal)fExpoTime;
				ExposureTimeEdit.Value = (decimal)fExpoTime;

                // Update the analog gain.
		        float fGain = 0.0f;
                status = DVPCamera.dvpGetAnalogGain(m_handle, ref fGain);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				GainEdit.Value = (decimal)fGain;

                // Update the ROI.
				uint RoiSel = 0;
				status = DVPCamera.dvpGetQuickRoiSel(m_handle, ref RoiSel);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (COMBO_RESOLUTION.Items.Count > RoiSel)
					COMBO_RESOLUTION.SelectedIndex = (int)RoiSel;
		        
                // Update the exposure time and gain.
                UpdateControlExposure();
                UpdateControlGain();
	        }
	        else
	        {
                // No device is opened at this time.
                // Update the basic controls.
                BUTTON_OPEN.Text = "Open";
                BUTTON_START.Enabled = false;
                BUTTON_PROPERTY.Enabled = false;
                BUTTON_SAVE.Enabled = false;

                BUTTON_OPEN.Enabled = m_n_dev_count == 0 ? false:true;
				ExposureTimeEdit.Enabled = false;
				ExposureTimeApply.Enabled = false;
				GainEdit.Enabled = false;
				GainApply.Enabled = false;
                COMBO_RESOLUTION.Enabled = false;
                COMBO_AE_MODE.Enabled = false;
                COMBO_FLICK.Enabled = false;
				CHECK_AEOPERATION.Enabled = false;
	        }
			InitAWB();
        }

        private void BUTTON_PROPERTY_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle, this.Handle);
				
                // At this time some configurations may change, synchronize it to the window GUI.
                UpdateControls();
            }
        }

        private void COMBO_RESOLUTION_SelectedIndexChanged(object sender, EventArgs e)
        {
            dvpStatus status;

            // Get the index of the ROI.
            int iSel = COMBO_RESOLUTION.SelectedIndex ;
            if (iSel < 0)
            {
                return;
            }

			if (!IsValidHandle(m_handle) )
				return ;
			dvpStreamState state = new dvpStreamState();
			
			status = DVPCamera.dvpGetStreamState(m_handle, ref state);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			if (state == dvpStreamState.STATE_STARTED)
			{
				// Close the video stream.
				status = DVPCamera.dvpStop(m_handle);

				if (status != dvpStatus.DVP_STATUS_OK)
				{
					MessageBox.Show(("Close the video stream fail!"));
					return;
				}
				m_b_start = false;
			}
            

            // Set the ROI.
			status = DVPCamera.dvpSetQuickRoiSel(m_handle, (uint)iSel);
            if (status != dvpStatus.DVP_STATUS_OK)
            {
                MessageBox.Show(("Set the ROI error!"));
				return;
            }

			if (state == dvpStreamState.STATE_STARTED)
			{
				// Open the video stream.
				status = DVPCamera.dvpStart(m_handle);
				if (status != dvpStatus.DVP_STATUS_OK)
				{
					MessageBox.Show(("Start the video stream fail!"));
					return;
				}

				m_b_start = true;
			}
			UpdateControlExposure();
			UpdateControlGain();
			ResizeWindows();
        }

        private void BasicFunction_FormClosing(object sender, FormClosingEventArgs e)
        {
			dvpStatus status;
			dvpStreamState state = new dvpStreamState();

            if (IsValidHandle(m_handle))
            {
				status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (state == dvpStreamState.STATE_STARTED)
				{
					status = DVPCamera.dvpStop(m_handle);
					// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					
				}
				status = DVPCamera.dvpClose(m_handle);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle = 0;
            }
        }

        private void CHECK_AEOPERATION_Click(object sender, EventArgs e)
        {
            dvpStatus status;
            m_bAeOp = CHECK_AEOPERATION.Checked;

            if (m_bAeOp)
            {
                status = DVPCamera.dvpSetAeOperation(m_handle, dvpAeOperation.AE_OP_CONTINUOUS);
            }
            else
            {
                status = DVPCamera.dvpSetAeOperation(m_handle, dvpAeOperation.AE_OP_OFF);
            }
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            UpdateControls();
        }

        private void COMBO_FLICK_SelectedIndexChanged(object sender, EventArgs e)
        {
            dvpStatus status = dvpStatus.DVP_STATUS_OK;

            // Get the index of the AE.
            if ( COMBO_FLICK.SelectedIndex > 2 )
            {
                return;
            }

            if (COMBO_FLICK.SelectedIndex == 0)
            {
                status = DVPCamera.dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_DISABLE);
            }
            else if (COMBO_FLICK.SelectedIndex == 2)
            {
                status = DVPCamera.dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_60HZ);
            }
            else if (COMBO_FLICK.SelectedIndex == 1)
            {
                status = DVPCamera.dvpSetAntiFlick(m_handle, dvpAntiFlick.ANTIFLICK_50HZ);
            }
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
        }

        private void COMBO_AE_MODE_ImeModeChanged(object sender, EventArgs e)
        {
            dvpStatus status = dvpStatus.DVP_STATUS_OK;

            // Get the index of the AE mode.         
            if (COMBO_AE_MODE.SelectedIndex > 3)
            {
                return;
            }

            // Prioritize the auto exposure.
            if (COMBO_AE_MODE.SelectedIndex == 0)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_AG);
            }

            // Prioritize the auto gain.
            else if (COMBO_AE_MODE.SelectedIndex == 1)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_AE);
            }

            // Open the auto exposure only.
            else if (COMBO_AE_MODE.SelectedIndex == 2)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_ONLY);
            }

            // Open the auto gain only.
            else if (COMBO_AE_MODE.SelectedIndex == 3)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_ONLY);
            }
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            UpdateControls();
        }

        private void BUTTON_SAVE_Click(object sender, EventArgs e)
        {
            if (!m_b_start)
                return;

            SaveFileDialog sfd = new SaveFileDialog();
            
			sfd.Filter = "bmp file(*.bmp)|*.bmp|jpeg file(*.jpeg)|*.jpeg|png file(*.png)|*.png|tif file(*.tif)|*.tif|gif file(*.gif)|*.gif|raw Files(*.dat)|*.dat";           
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                m_ImagePath = sfd.FileName;
				m_bSaveImageFlg = true;

            }
        }

        private void COMBO_AE_MODE_SelectedIndexChanged(object sender, EventArgs e)
        {
            dvpStatus status = new dvpStatus();

            // Get the index of the AE mode.
            int iSel = COMBO_AE_MODE.SelectedIndex;
            if (iSel > 3)
            {
                return;
            }

            // Prioritize the auto exposure.
            else if (iSel == 2)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_ONLY);
            }

            // Prioritize the auto gain.
            else if (iSel == 3)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_ONLY);
            }
            // Open the auto exposure only.
            else if (iSel == 1)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AG_AE);
            }
            // Open the auto gain only.
            if (iSel == 0)
            {
                status = DVPCamera.dvpSetAeMode(m_handle, dvpAeMode.AE_MODE_AE_AG);
            }
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            UpdateControls();
        }

		// Initialize the auto white balance.
		private void InitAWB()
		{
			dvpStatus status;
			dvpAwbOperation AwbOperation = new dvpAwbOperation();

			if (IsValidHandle(m_handle))
			{

                // Gets the current white balance property
				status = DVPCamera.dvpGetAwbOperation(m_handle, ref AwbOperation);
				// ASSERT(status == DVP_STATUS_OK);
				if (status != dvpStatus.DVP_STATUS_OK)
				{
                    // Auto white balance is not supported or getting fails
					AWBEnable.Enabled = false;
					AWBOnce.Enabled = false;
					
					return;
				}
				// Set the controls
				AWBEnable.Enabled = true;
				
				AWBEnable.Checked = (AwbOperation == dvpAwbOperation.AWB_OP_CONTINUOUS);
				AWBOnce.Enabled = (!AWBEnable.Checked);
			}
			else
			{
				AWBEnable.Enabled = false;
				AWBOnce.Enabled = false;
			}
		}

		private void AWBEnable_CheckedChanged(object sender, EventArgs e)
		{
			dvpStatus status;
			dvpAwbOperation AwbOperation = new dvpAwbOperation();

			if (IsValidHandle(m_handle))
			{
                // Auto white balance,set to AWB_OP_CONTINUOUS stands for continuous operation. 
				if (AWBEnable.Checked)
				{
					status = DVPCamera.dvpGetAwbOperation(m_handle, ref AwbOperation);
					if (AwbOperation != dvpAwbOperation.AWB_OP_CONTINUOUS)
					{
						status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_CONTINUOUS);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					}
				}
				else
				{
                    // Close auto white balance.
					status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_OFF);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				InitAWB();
			}
		}

		private void AWBOnce_Click(object sender, EventArgs e)
		{
			dvpStatus status;

			if (IsValidHandle(m_handle))
			{
                // Auto white balance,set to AWB_OP_ONCE stands for operating once. 
				status = DVPCamera.dvpSetAwbOperation(m_handle, dvpAwbOperation.AWB_OP_ONCE);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				// The RGB gain is set to 1.00, avoiding a superimposed gain effect
				bool bGainState = false;
				status = DVPCamera.dvpGetRgbGainState(m_handle, ref bGainState);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					if (bGainState)
					{
						status = DVPCamera.dvpSetRgbGain(m_handle, 1.0f, 1.0f, 1.0f);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					}
					else
					{
						status = DVPCamera.dvpSetRgbGainState(m_handle, true);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
						status = DVPCamera.dvpSetRgbGain(m_handle, 1.0f, 1.0f, 1.0f);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
						status = DVPCamera.dvpSetRgbGainState(m_handle, bGainState);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
					}
				}


				InitAWB();
			}
		}

		private void ExposureTimeApply_Click(object sender, EventArgs e)
		{
			dvpStatus status;
			double f_time = 0.0f;
			if (IsValidHandle(m_handle))
			{
				if (ExposureTimeEdit.Maximum < ExposureTimeEdit.Value + 1)
				{
					ExposureTimeEdit.Value = ExposureTimeEdit.Maximum - 1;
				}
				if (ExposureTimeEdit.Minimum > ExposureTimeEdit.Value - 1)
				{
					ExposureTimeEdit.Value = ExposureTimeEdit.Minimum + 1;
				}

				status = DVPCamera.dvpSetExposure(m_handle, (float)ExposureTimeEdit.Value);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				//  Get the value of the exposure time again,there are differences between the set value and the obtained value for the reason of accuracy(step value),
                //  it is subject to the obtained value.
				status = DVPCamera.dvpGetExposure(m_handle, ref f_time);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set the text of the exposure time.
				ExposureTimeEdit.Value = (long)f_time;
			}
		}

		private void GainApply_Click(object sender, EventArgs e)
		{
			float fGain = 0.0f;
			dvpStatus status;

			if (IsValidHandle(m_handle))
			{
                // Get the value of the analog gain slider.
				fGain = (float)GainEdit.Value;

                // Firstly,set the value of the analog gain.
				status = DVPCamera.dvpSetAnalogGain(m_handle, fGain);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Secondly,Get the value of the analog gain,there are differences between the set value and the obtained value for the reason of accuracy(step value),
                // it is subject to the obtained value.
				status = DVPCamera.dvpGetAnalogGain(m_handle, ref fGain);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set the text of the analog gain.
				GainEdit.Value = (decimal)fGain;
			}
		}

		private void ResizeWindows()
		{
			if (IsValidHandle(m_handle) ) 
			{
				dvpRegion roi ;
				roi= new dvpRegion();
				dvpStatus status;
				status = DVPCamera.dvpGetRoi(m_handle, ref roi);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				pictureBoxMain.Width = this.Width - pictureBoxMain.Left;
				pictureBoxMain.Height = this.Height - pictureBoxMain.Top;

				if (pictureBoxMain.Width * roi.H > pictureBoxMain.Height * roi.W )
				{
					pictureBoxMain.Width = pictureBoxMain.Height * roi.W / roi.H;
				}
				else
				{
					pictureBoxMain.Height = pictureBoxMain.Width * roi.H / roi.W;
				}
			}
		}
		
		private void BasicFunction_Resize(object sender, EventArgs e)
		{
			ResizeWindows();
		}

		private void UserDefineName_CheckedChanged(object sender, EventArgs e)
		{
			
			// save cur sel item
			string strName;
			strName = COMBO_DEVICES.Text;

			// reset m_listDevices values
			COMBO_DEVICES.Items.Clear();

			for (int i = 0; i < m_CamCount; i++)
			{
				int item = -1;
				if ( !UserDefinedName.Checked )
				{
					item = COMBO_DEVICES.Items.Add(m_info[i].FriendlyName);
					if (strName == m_info[i].UserID)
					{
						COMBO_DEVICES.SelectedIndex = item;
					}
				}
				else
				{
					// check User define name is null
					if ( m_info[i].UserID.Length == 0 )
						continue;

					item = COMBO_DEVICES.Items.Add(m_info[i].UserID);
					if (strName == m_info[i].FriendlyName )
					{
						COMBO_DEVICES.SelectedIndex = item;
					}
				}
			}
		}

        private void pictureBox_Click(object sender, EventArgs e)
        {
			BUTTON_SAVE_Click(sender, e);
        }
    }
}

