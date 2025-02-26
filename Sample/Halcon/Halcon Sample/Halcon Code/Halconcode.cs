using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using HalconDotNet;
using DVPCameraType;

namespace Halcon_Code
{
    public partial class Halconcode : Form
    {
        UInt32 m_handle = 0;
        uint i, n = 0;
   
        dvpCameraInfo dev_info = new dvpCameraInfo();
        HObject ho_Image = null;

        HTuple hv_DataCodeHandleQR = new HTuple();
      
        //is falg
        bool IsOpenPciture = false;
        bool IsSteamStatus = false;

        //display halcon auto
        HTuple hv_Width = null, hv_Height = null;
            
        public Halconcode()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            InitDevList();
        }
        //handle
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
        private void CamerScan_Click(object sender, EventArgs e)
        {
            InitDevList();
        }

        //Update the control
        public void UpdateControls()
        {
            dvpStatus status = dvpStatus.DVP_STATUS_UNKNOW;
            dvpStreamState StreamState = dvpStreamState.STATE_STOPED;
            status = DVPCamera.dvpGetStreamState(m_handle, ref StreamState);

            if (IsValidHandle(m_handle))
            {    
                if (IsOpenPciture == true)
                {
                    if (StreamState == dvpStreamState.STATE_STARTED)
                    {
                        status = DVPCamera.dvpStop(m_handle);
                        //close camera
                        status = DVPCamera.dvpClose(m_handle);
                    }
                    OpenCamera.Enabled = false;
                    Grab.Enabled = false;
                }
                else
                {
                    Grab.Text = StreamState == dvpStreamState.STATE_STARTED ? "stop capture" : "start capture";
                }
                Grab.Enabled = true;
                OpenCamera.Text = "close camera";
            }
            else
            {
                Grab.Enabled = false;
                OpenCamera.Enabled = (n >= 1) ? true : false;
                //Decod.Enabled = false;
                OpenCamera.Text = "open camera";

            }
        }
        // Initialize the device list.
        public void InitDevList()
        {
            dvpStatus status;

            // "n" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
            CameraName.Items.Clear();

            // Get the number of cameras that has been connected to a computer.
            status = DVPCamera.dvpRefresh(ref n);
            if (status == dvpStatus.DVP_STATUS_OK)
            {
                for (i = 0; i < n; i++)
                {
                    int item = 0;
                    // Acquire each camera's information one by one.
                    status = DVPCamera.dvpEnum(i, ref dev_info);

                    if (status == dvpStatus.DVP_STATUS_OK)
                    {
                        // add FriendlyName
                        item = CameraName.Items.Add(dev_info.FriendlyName);
                    }
                    if (item == 0)
                    {
                        CameraName.SelectedIndex = item;
                    }
                }
            }
            UpdateControls();
        }

        private void OpenCamera_Click(object sender, EventArgs e)
        {
            dvpStatus status = dvpStatus.DVP_STATUS_UNKNOW;
            if (!IsValidHandle(m_handle))
            {
                // Open the specific device by the selected FriendlyName.
                status = DVPCamera.dvpOpenByName(CameraName.Text, dvpOpenMode.OPEN_NORMAL, ref m_handle);
                if (status != dvpStatus.DVP_STATUS_OK)
                {
                    m_handle = 0;
                    MessageBox.Show("Open the device failed!");
                }
            }
            else
            {
                // check camera
                dvpStreamState StreamState = new dvpStreamState();
                status = DVPCamera.dvpGetStreamState(m_handle, ref StreamState);
                if (StreamState == dvpStreamState.STATE_STARTED)
                {
                    // stop camera
                    status = DVPCamera.dvpStop(m_handle);
                }
                DVPCamera.dvpClose(m_handle);
                m_handle = 0;
            }
            UpdateControls();
        }

        Thread IamgeThread;

        public void showFrame()
        {       
            dvpStatus status = dvpStatus.DVP_STATUS_UNKNOW;
            dvpStreamState StreamState = dvpStreamState.STATE_STOPED;
            status = DVPCamera.dvpGetStreamState(m_handle, ref StreamState);
            if (StreamState == dvpStreamState.STATE_STOPED)
            {
                status = DVPCamera.dvpStart(m_handle);
                IsSteamStatus = true;
            }
            else
            {
                status = DVPCamera.dvpStop(m_handle);
                IsSteamStatus = false;
            }

            while (IsSteamStatus)
            {          
               dvpFrame refFrame = new dvpFrame();
               IntPtr pBuffer =new IntPtr();

              //HObject halcon_image =null;
               status = DVPCamera.dvpGetFrame(m_handle, ref refFrame,ref pBuffer, 5000);
                if (status != dvpStatus.DVP_STATUS_OK)
                {
                    continue;
                }          
            int width = refFrame.iWidth;
            int height = refFrame.iHeight;
            
            if (refFrame.bRotateState != 0)
            {
                height = refFrame.iWidth;
                width = refFrame.iHeight;
            }
            try
            {
                if (ho_Image != null)
                {
                    ho_Image.Dispose();
                    ho_Image = null;
                }

                if (refFrame.format == dvpImageFormat.FORMAT_BGR24)
                {
                    HOperatorSet.GenImageInterleaved(out ho_Image, pBuffer, "bgr", width, height, 0, "byte", 0, 0, 0, 0, -1, 0);
                }
                else if (refFrame.format == dvpImageFormat.FORMAT_MONO)
                {
                    HOperatorSet.GenImage1Extern(out ho_Image, "byte", width, height, pBuffer, 0);
                }
                if (ho_Image != null)
                {
                    HOperatorSet.GetImageSize(ho_Image, out hv_Width, out hv_Height);
                    HOperatorSet.SetPart(this.hWindowControl1.HalconWindow, 0, 0, hv_Height - 1, hv_Width - 1);
                    HOperatorSet.DispObj(ho_Image, hWindowControl1.HalconWindow);
                }
            }
            catch (System.Exception ex)
            {
                //MessageBox.Show(ex.Message.ToString());
            }
            Grab.Text = "停止采集";
         }
            UpdateControls();
       }
        private void Grab_Click_1(object sender, EventArgs e)
        {
            IamgeThread = new Thread(showFrame);
            IamgeThread.Start();
        }

        private void StopGrab_Click(object sender, EventArgs e)
        {
            try
            {
                IamgeThread.Abort();
            }
            catch (System.Exception ex)
            {
                //MessageBox.Show(ex.Message);
            }
            ho_Image.Dispose();
        }

        private void Halconcode_FormClosing(object sender, FormClosingEventArgs e)
        {
            IamgeThread.Abort();
        }    
    }
}
