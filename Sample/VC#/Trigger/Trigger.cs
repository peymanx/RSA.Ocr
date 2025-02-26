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

namespace Trigger
{
    public partial class Trigger : Form
    {
        public uint m_handle = 0;
        public bool m_bAeOp = false;
        public int m_n_dev_count = 0;
        string m_strFriendlyName = "";

        public static IntPtr m_ptr_wnd = new IntPtr();
        public static IntPtr m_ptr = new IntPtr();
        public static bool m_b_start = false;

        public static double m_TriggerDelay = 0.0f;
        public static double m_TriggerFilter = 0.0f;
        public static double m_TriggerLoop = 0.0f;
        dvpDoubleDescr m_DelayDescr = new dvpDoubleDescr();
        dvpDoubleDescr m_FilterDescr = new dvpDoubleDescr();
        dvpDoubleDescr m_LoopDescr = new dvpDoubleDescr();

		public static bool m_bTriggerMode = false;

		// Display param
		public static Stopwatch m_Stopwatch = new Stopwatch();
		public static Double m_dfDisplayCount = 0;

		public static dvpCameraInfo[] m_info = new dvpCameraInfo[16];
		public static int m_CamCount = 0;

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
		public void UpdateControls()
		{
			dvpStatus status;

			if (IsValidHandle(m_handle))
			{
                // The device has been opened at this time.
                // Update and enable the basic controls.
				dvpStreamState state = new dvpStreamState();
				status = DVPCamera.dvpGetStreamState(m_handle, ref state);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				OpenDev.Text = "Close";
				StartPlay.Text = state == dvpStreamState.STATE_STARTED ? ("Stop") : ("Start");
				StartPlay.Enabled = true;
				PropertSet.Enabled = true;

                // Enable the related controls.

				LoopTrigger.Enabled = true;
				EnableIn.Enabled = true;
				EnableOut.Enabled = true;
				SoftTriggerFire.Enabled = true;
				
				
				// Update the window that is related to trigger function.
				bool bTrig = false;
				bool bLoop = false;

				// Update the enable status of the trigger mode. 
				status = DVPCamera.dvpGetTriggerState(m_handle, ref bTrig);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				TriggerMode.Checked = bTrig;
				if (status != dvpStatus.DVP_STATUS_OK)
				{
					TriggerMode.Enabled = false;
					TriggerLoop.Enabled = false;
					LoopTimer.Enabled = false;
					ApplyLoop.Enabled = false;
					TriggerDelay.Enabled = false;
					ApplyDelay.Enabled = false;
					SoftTriggerFire.Enabled = false;
				}
				else
				{
					TriggerMode.Enabled = state != dvpStreamState.STATE_STARTED;
					TriggerMode.Checked = bTrig;

					Delay.Enabled = bTrig;
					TriggerDelay.Enabled = bTrig;
					ApplyDelay.Enabled = bTrig;
				}
				
				LoopTrigger.Enabled = bTrig;

				// Get the enable state of loop trigger.
				bLoop = false;
				status = DVPCamera.dvpGetSoftTriggerLoopState(m_handle, ref bLoop);
				LoopTrigger.Checked = bLoop;
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					SoftTriggerFire.Enabled = bTrig && (!bLoop);
					if (bLoop && bTrig)
					{
						TriggerLoop.Enabled = true;
						ApplyLoop.Enabled = true;
						LoopTimer.Enabled = true;
					}
					else
					{
						TriggerLoop.Enabled = false;
						ApplyLoop.Enabled = false;
						LoopTimer.Enabled = false;
					}
				}
				else
				{
					LoopTrigger.Enabled = false;
					TriggerLoop.Enabled = false;
					LoopTimer.Enabled = false;
					ApplyLoop.Enabled = false;
					TriggerDelay.Enabled = false;
					ApplyDelay.Enabled = false;
					SoftTriggerFire.Enabled = false;
				}

                // Update the external trigger input status of the related IO.
				bool bExtTrigIn = false;
				dvpUserIoInfo UserIoInfo = new dvpUserIoInfo();
				dvpInputIoFunction InIoFunc = new dvpInputIoFunction();
				int i = 0;

                // Enumerate all UserIO,and check whether the IO is set as the trigger input function.
				status = DVPCamera.dvpGetUserIoInfo(m_handle, ref UserIoInfo);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				InputIOCombo.Items.Clear();
				int nInLength = 8 < UserIoInfo.inputValid.Length ? 8 : UserIoInfo.inputValid.Length;
				for (i = 0; i < nInLength; i++)
				{
					if (UserIoInfo.inputValid[i] != 0)
					{
						string str;
						dvpInputIo InputIo = (dvpInputIo)(i + (int)dvpInputIo.INPUT_IO_1);

                        // Add the IO name to the drop-down list.
						int nInItem = i + 1;
						str = "IN_" + nInItem.ToString();
						int item = InputIOCombo.Items.Add(str);

                        // Check whether there is the input IO is set as the trigger input function. 
						status = DVPCamera.dvpGetInputIoFunction(m_handle, InputIo, ref InIoFunc);
						Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
						if (!bExtTrigIn && InIoFunc == dvpInputIoFunction.INPUT_FUNCTION_TRIGGER)
						{
                            // Find the IO that is set as the trigger input signal.
							bExtTrigIn = true;

                            // Select the IO that is set as the trigger input function in drop-down list.
							InputIOCombo.SelectedIndex = i;
						}
					}
				}

				if (bExtTrigIn)
				{
                    // Indicate that the trigger input signal have been used on the window GUI.
					InputIOCombo.Enabled = true;
					EnableIn.Checked = true;
				}
				else
				{
					if (InputIOCombo.Items.Count > 0)
					{
                        // No IO is set as the trigger input function, the first one in the drop-down list is selected as the default. 
						InputIOCombo.SelectedIndex = 0;
					}

					InputIOCombo.Enabled = true;
					EnableIn.Checked = false ;
				}
				
				InputIOCombo.Enabled = bExtTrigIn;
				InputSignalTypeCombo.Enabled = bExtTrigIn;
				EditFilter.Enabled = bExtTrigIn;
				FilterApply.Enabled = bExtTrigIn;

                // Indicate that trigger input signal is used on the window GUI.
				EnableIn.Checked = bExtTrigIn;

                // Add the signal type of the trigger input to the list. 
				InputSignalTypeCombo.Items.Clear();
				InputSignalTypeCombo.Items.Add(("Off"));
				InputSignalTypeCombo.Items.Add(("Falling Edge"));
				InputSignalTypeCombo.Items.Add(("Low Level"));
				InputSignalTypeCombo.Items.Add(("Rising Edge"));
				InputSignalTypeCombo.Items.Add(("High Level"));

				dvpTriggerInputType TriggerType = new dvpTriggerInputType();
				status = DVPCamera.dvpGetTriggerInputType(m_handle, ref TriggerType);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				InputSignalTypeCombo.SelectedIndex = (int)TriggerType;

                // Update the window GUI of strobe function. 
				bool bExtStrobeOut = false;
				dvpOutputIoFunction OutIoFunc = new dvpOutputIoFunction();

                // Enumerate all UserIO, and check whether there is IO is set as the strobe output function.
				OutIOCombo.Items.Clear();
				nInLength = 8 < UserIoInfo.outputValid.Length ? 8 : UserIoInfo.outputValid.Length;
				for (i = 0; i < nInLength; i++)
				{
					if ( UserIoInfo.outputValid[i] != 0 )
					{
						string str;
						dvpOutputIo OutputIo = (dvpOutputIo)(i + dvpOutputIo.OUTPUT_IO_1);

						// Add the IO name to the drop-down list.
						int nOutItem = i + 1;
						str = "OUT_" + nOutItem.ToString();
						OutIOCombo.Items.Add(str);

						// Check whether there is the output IO is set as the strobe output function.
						status = DVPCamera.dvpGetOutputIoFunction(m_handle, OutputIo, ref OutIoFunc);
						// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
						if (!bExtStrobeOut && OutIoFunc == dvpOutputIoFunction.OUTPUT_FUNCTION_STROBE)
						{
							// Find the IO that is set as the strobe output function.
							bExtStrobeOut = true;

							// Select the IO that is set as the strobe output function in the drop-down list.
							OutIOCombo.SelectedIndex = i;

						}
					}
					
				}
				if (bExtStrobeOut)
				{
                    // Indicate that the strobe output signal has been used on the window GUI.
					EnableOut.Checked = true;
				}
				else
				{
                    // No IO is set as the trigger input function,the first one in the drop-down list is selected as the default.
					if (OutIOCombo.Items.Count > 0)
					{
						OutIOCombo.SelectedIndex = 0;
					}
					else
					{
						OutIOCombo.Items.Add("OUT_1");
						OutIOCombo.SelectedIndex = 0;
					}

                    // Indicate that no strobe output signal is used on the window GUI.
					EnableOut.Checked = false;
				}
				
				OutputSignalType.Enabled = bExtStrobeOut;
				OutIOCombo.Enabled = bExtStrobeOut;

                // Add the signal type of the strobe output to the list. 
				OutputSignalType.Items.Clear();
				OutputSignalType.Items.Add(("Off"));
				OutputSignalType.Items.Add(("Low Level"));
				OutputSignalType.Items.Add(("High Level"));

                // Get the signal type of the strobe. 
				dvpStrobeOutputType StrobeType = new dvpStrobeOutputType();
				status = DVPCamera.dvpGetStrobeOutputType(m_handle, ref StrobeType);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				OutputSignalType.SelectedIndex = (int)StrobeType;

                // The following descriptions of the information will be used to update the range of values in the edit box.
				status = DVPCamera.dvpGetTriggerDelayDescr(m_handle, ref m_DelayDescr);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				TriggerDelay.Maximum = (decimal)m_DelayDescr.fMax;
				TriggerDelay.Minimum = (decimal)m_DelayDescr.fMin;

				status = DVPCamera.dvpGetTriggerJitterFilterDescr(m_handle, ref m_FilterDescr);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				EditFilter.Maximum = (decimal)m_FilterDescr.fMax;
				EditFilter.Minimum = (decimal)m_FilterDescr.fMin;

				status = DVPCamera.dvpGetSoftTriggerLoopDescr(m_handle, ref m_LoopDescr);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				TriggerLoop.Maximum = (decimal)m_LoopDescr.fMax;
				TriggerLoop.Minimum = (decimal)m_LoopDescr.fMin;

                // Update values in the edit box. 
				status = DVPCamera.dvpGetSoftTriggerLoop(m_handle, ref m_TriggerLoop);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (m_TriggerLoop > m_LoopDescr.fMax)
					m_TriggerLoop = m_LoopDescr.fMax;
				if (m_TriggerLoop < m_LoopDescr.fMin)
					m_TriggerLoop = m_LoopDescr.fMin;
				TriggerLoop.Value = (decimal)m_TriggerLoop;
				status = DVPCamera.dvpGetTriggerDelay(m_handle, ref m_TriggerDelay);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				if (m_TriggerDelay > m_DelayDescr.fMax)
					m_TriggerDelay = m_DelayDescr.fMax;
				if (m_TriggerDelay < m_DelayDescr.fMin)
					m_TriggerDelay = m_DelayDescr.fMin;
				TriggerDelay.Value = (decimal)m_TriggerDelay;
				status = DVPCamera.dvpGetTriggerJitterFilter(m_handle, ref m_TriggerFilter);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				if (m_TriggerFilter < 1)
				{
					m_TriggerFilter = 1;
				}
				if (m_TriggerFilter > m_FilterDescr.fMax)
					m_TriggerFilter = m_FilterDescr.fMax;
				if (m_TriggerFilter < m_FilterDescr.fMin)
					m_TriggerFilter = m_FilterDescr.fMin;
				EditFilter.Value = (decimal)m_TriggerFilter;
			}
			else
			{
                // No device is opened at this time.
                // Update the basic controls.
				OpenDev.Text = "Open";
				StartPlay.Enabled = false;
				PropertSet.Enabled = false;

				if (DevNameCombo.Items.Count == 0)
				{
                    // No device exists.
					OpenDev.Enabled = false;
				}
				else
				{
					OpenDev.Enabled = true;
				}

                // Update the related controls.
				InputIOCombo.Enabled = false;
				InputSignalTypeCombo.Enabled = false;
				OutputSignalType.Enabled = false;
				OutIOCombo.Enabled = false;
				EditFilter.Enabled = false;
				TriggerDelay.Enabled = false;
				TriggerLoop.Enabled = false;
				LoopTimer.Enabled = false;
				TriggerMode.Enabled = false;
				LoopTrigger.Enabled = false;
				EnableIn.Enabled = false;
				EnableOut.Enabled = false;
				SoftTriggerFire.Enabled = false;
				ApplyLoop.Enabled = false;
				ApplyDelay.Enabled = false;
				FilterApply.Enabled = false;
				Delay.Enabled = false;
				TriggerDelay.Enabled = false;
				ApplyDelay.Enabled = false;
			}
		}


        public Trigger()
        {
            Control.CheckForIllegalCrossThreadCalls = false;

            InitializeComponent();

			// Initialize open mode
			// false: user dvpOpenByName open the camear
			// true : user dvpOpenByUserId open the camear
			UserDefinedName.Checked = false;

            m_ptr_wnd = pictureBox.Handle;
            InitDevList();
            System.Timers.Timer t = new System.Timers.Timer(500);

            // Execute event when the time has arrived.
            t.Elapsed += new System.Timers.ElapsedEventHandler(theout);

            // Set the method of executing.
            t.AutoReset = true;

            // Enable the System.Timers.Timer.Elapsed event.
            t.Enabled = true;

        }

        public void theout(object source, System.Timers.ElapsedEventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                // Update the information of frame rate.
                dvpFrameCount count = new dvpFrameCount();
				dvpStatus status = DVPCamera.dvpGetFrameCount(m_handle, ref count);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				string str;
				if (m_dfDisplayCount == 0 || m_bTriggerMode)
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

        private void ScanDev_Click(object sender, EventArgs e)
        {
            InitDevList();
        }

        private DVPCamera.dvpStreamCallback _proc;

        // Callback function that used for receiving data.
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

			if (bDisplay || m_bTriggerMode)
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
            return 1;
        }

        private void OpenDev_Click(object sender, EventArgs e)
        {
            dvpStatus status = dvpStatus.DVP_STATUS_OK;


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
						MessageBox.Show("Open the device failed!");
                    }
                    else
                    {
						m_strFriendlyName = DevNameCombo.Text;

                        // If it needs to display images ,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
                        // Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
                        _proc = _dvpStreamCallback;
                        using (Process curProcess = Process.GetCurrentProcess())
                        using (ProcessModule curModule = curProcess.MainModule)
                        {
							status = DVPCamera.dvpRegisterStreamCallback(m_handle, _proc, dvpStreamEvent.STREAM_EVENT_PROCESSED, m_ptr);
							Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                        }
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

				}

				// cloas camera
				status = DVPCamera.dvpClose(m_handle);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                m_handle = 0;
				pictureBox.Refresh();
            }

            UpdateControls();
        }

        private void StartPlay_Click(object sender, EventArgs e)
        {
			// init display count is 0
			m_dfDisplayCount = 0;

            if (IsValidHandle(m_handle))
            {
                dvpStreamState state = new dvpStreamState();
                dvpStatus status;

                // Implement a button to start and stop according to the current video's status.
                status = DVPCamera.dvpGetStreamState(m_handle, ref state);

                if (state == dvpStreamState.STATE_STARTED)
                {
                    status = DVPCamera.dvpStop(m_handle);
                }
                else
                {
					if (!TriggerMode.Enabled)
					{
						m_bTriggerMode = false;
					}
					else
					{
						m_bTriggerMode = TriggerMode.Checked;
					}
					
                    status = DVPCamera.dvpStart(m_handle);
                }
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            }

            UpdateControls();
        }

        private void PropertSet_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                dvpStatus status = DVPCamera.dvpShowPropertyModalDialog(m_handle, this.Handle);

                // At this time some configurations may change, synchronize it to the window GUI.
                UpdateControls();
            }
        }

        private void TriggerMode_CheckedChanged(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
				dvpStatus status= new dvpStatus();

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

				// Enable/disable the trigger mode.
				status = DVPCamera.dvpSetTriggerState(m_handle, TriggerMode.Checked);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				if (StreamState == dvpStreamState.STATE_STARTED)
				{
					// Start the video stream.
					status = DVPCamera.dvpStart(m_handle);
					Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
				}
				UpdateControls();
            }
        }

        private void SoftTriggerFire_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                // Once execution of this function is equivalent to the generation of an external trigger.
                // Note:If the exposure time is very long, clicking on the "Soft Trigger the Fire" too quick may cause the failure of the trigger,
                // because the previous frame probably in a state of exposing continuously or output incompletely.
                dvpStatus status = DVPCamera.dvpTriggerFire(m_handle);
            }
        }

        private void LoopTrigger_CheckedChanged(object sender, EventArgs e)
        {           
            dvpStatus status = DVPCamera.dvpSetSoftTriggerLoopState(m_handle, LoopTrigger.Checked);
			Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
			UpdateControls();
        }

        private void ApplyLoop_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
				m_TriggerLoop = (double)TriggerLoop.Value;

                dvpStatus status = DVPCamera.dvpSetSoftTriggerLoop(m_handle, m_TriggerLoop);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				status = DVPCamera.dvpGetSoftTriggerLoop(m_handle, ref m_TriggerLoop);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					TriggerLoop.Value = (decimal)m_TriggerLoop;
				}
            }
        }

        private void ApplyDelay_Click(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
				m_TriggerDelay = (double)TriggerDelay.Value;

                dvpStatus status = DVPCamera.dvpSetTriggerDelay(m_handle, m_TriggerDelay);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				status = DVPCamera.dvpGetTriggerDelay(m_handle, ref m_TriggerDelay);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					TriggerDelay.Value = (decimal)m_TriggerDelay;
				}
            }
        }

        private void EnableInCheck()
        {
            if (IsValidHandle(m_handle))
            {
                dvpUserIoInfo UserIoInfo = new dvpUserIoInfo();
                int i = 0;
                dvpStatus status;
                status = DVPCamera.dvpGetUserIoInfo(m_handle, ref UserIoInfo);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set all INPUT IO as the Normal mode
                int nInLength = 8 < UserIoInfo.inputValid.Length ? 8 : UserIoInfo.inputValid.Length;
                for (i = 0; i < nInLength; i++)
                {
                    if (UserIoInfo.inputValid[i] > 0)
                    {
						status = DVPCamera.dvpSetInputIoFunction(m_handle, (dvpInputIo)(dvpInputIo.INPUT_IO_1 + i), dvpInputIoFunction.INPUT_FUNCTION_NORMAL);
						// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    }
                }

                // If ExtTriggerIn is enable,set the Input Io as the trigger input function. 
                if (EnableIn.Checked)
                {
                    int j = InputIOCombo.SelectedIndex;
                    dvpInputIo InputIo = new dvpInputIo();
                    if (j >= 0)
                    {
                        InputIo = (dvpInputIo)(j + dvpInputIo.INPUT_IO_1);
						status = DVPCamera.dvpSetInputIoFunction(m_handle, InputIo, dvpInputIoFunction.INPUT_FUNCTION_TRIGGER);
						// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    }
                }
            }
        }

        private void EnableIn_CheckedChanged(object sender, EventArgs e)
        {
			if (IsValidHandle(m_handle))
			{
				EnableInCheck();
			}

			UpdateControls();
        }

        private void InputIOCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                // Changing the trigger input IO and enabling the trigger input function is the same process.
                EnableInCheck();
            }
			
        }

        private void InputSignalTypeCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                dvpTriggerInputType TriggerType = (dvpTriggerInputType)(dvpTriggerInputType.TRIGGER_IN_OFF + InputSignalTypeCombo.SelectedIndex);
                dvpStatus status = DVPCamera.dvpSetTriggerInputType(m_handle, TriggerType);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            }
        }

        private void OutIOCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                // Changing the strobe output IO and enabling the strobe output function is the same process.
                EnableOutCheck();
            }
        }

        private void OutputSignalType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (IsValidHandle(m_handle))
            {
                dvpStrobeOutputType StrobeType = (dvpStrobeOutputType)(dvpStrobeOutputType.STROBE_OUT_OFF + OutputSignalType.SelectedIndex);
                dvpStatus status = DVPCamera.dvpSetStrobeOutputType(m_handle, StrobeType);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
            }
        }

        private void EnableOutCheck()
        {
            if (IsValidHandle(m_handle))
            {
                dvpUserIoInfo UserIoInfo = new dvpUserIoInfo();
                int i;
                dvpStatus status;
                status = DVPCamera.dvpGetUserIoInfo(m_handle, ref UserIoInfo);
				// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

                // Set all OUTPUT IO as the Normal mode.  
                int nOutLength = 8 < UserIoInfo.outputValid.Length ? 8 : UserIoInfo.outputValid.Length;
                for (i = 0; i < nOutLength; i++)
                {
                    if (UserIoInfo.outputValid[i] > 0)
                    {
                        status = DVPCamera.dvpSetOutputIoFunction(m_handle, (dvpOutputIo)(dvpOutputIo.OUTPUT_IO_1 + i), dvpOutputIoFunction.OUTPUT_FUNCTION_NORMAL);
						// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    }
                }

                // If ExtStrobeOut is enable,set the Output Io as the strobe output function. 
                if (EnableOut.Checked)
                {
                    int j = OutIOCombo.SelectedIndex;
                    dvpOutputIo OutputIo;
                    if (j >= 0)
                    {
                        OutputIo = (dvpOutputIo)(j + dvpOutputIo.OUTPUT_IO_1);
                        status = DVPCamera.dvpSetOutputIoFunction(m_handle, OutputIo, dvpOutputIoFunction.OUTPUT_FUNCTION_STROBE);
						// Debug.Assert(status == dvpStatus.DVP_STATUS_OK);
                    }
                }
            }
        }

        private void EnableOut_CheckedChanged(object sender, EventArgs e)
        {
			if (IsValidHandle(m_handle))
			{
				EnableOutCheck();
			}
			UpdateControls();
        }

        private void Trigger_FormClosing(object sender, FormClosingEventArgs e)
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

        private void FilterApply_Click(object sender, EventArgs e)
        {
			if (IsValidHandle(m_handle))
			{
				m_TriggerFilter = (double)EditFilter.Value;

				dvpStatus status = DVPCamera.dvpSetTriggerJitterFilter(m_handle, m_TriggerFilter);
				Debug.Assert(status == dvpStatus.DVP_STATUS_OK);

				status = DVPCamera.dvpGetTriggerJitterFilter(m_handle, ref m_TriggerFilter);
				if (status == dvpStatus.DVP_STATUS_OK)
				{
					EditFilter.Value = (decimal)m_TriggerFilter;
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

				pictureBox.Width = this.Width - pictureBox.Left;
				pictureBox.Height = this.Height - pictureBox.Top;

				if (pictureBox.Width * roi.H > pictureBox.Height * roi.W)
				{
					pictureBox.Width = pictureBox.Height * roi.W / roi.H;
				}
				else
				{
					pictureBox.Height = pictureBox.Width * roi.H / roi.W;
				}
			}
		}

		private void Trigger_Resize(object sender, EventArgs e)
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
				if (!UserDefinedName.Checked)
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
					if (m_info[i].UserID.Length == 0)
						continue;

					item = DevNameCombo.Items.Add(m_info[i].UserID);
					if (strName == m_info[i].FriendlyName)
					{
						DevNameCombo.SelectedIndex = item;
					}
				}
			}
		}
    }
}
