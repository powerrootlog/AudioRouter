using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Denoise
{
    public partial class frmMain : Form
    {
        [DllImport("Adapter.dll")]
        public static extern bool attachSpeaker(bool enabled);
        [DllImport("Adapter.dll")]
        public static extern bool attachMic(bool enabled);
        [DllImport("Adapter.dll")]
        public static extern int getSpeakerCount();
        [DllImport("Adapter.dll")]
        public static extern int getMicCount();
        [DllImport("Adapter.dll")]
        [return: MarshalAs(UnmanagedType.LPWStr)]
        public static extern string getSpeakerList(int index);
        [DllImport("Adapter.dll")]
        [return: MarshalAs(UnmanagedType.LPWStr)]
        public static extern string getMicList(int index);



        Color activeColor = Color.GreenYellow;
        Color deactiveColor = Color.White;
        private bool bSpeakAttach = false;
        private bool bMicAttach = false;
        public frmMain()
        {
            InitializeComponent();

            int count = getSpeakerCount();
            for (int i = 0; i < count; i++)
                cmbSpeaker.Items.Add(getSpeakerList(i));
            if (count > 0)
                cmbSpeaker.SelectedIndex = 0;

            count = getMicCount();
            for (int i = 0; i < count; i++)
                cmbMic.Items.Add(getMicList(i));
            if (count > 0)
                cmbMic.SelectedIndex = 0;
        }

        private void panSpeaker_Click(object sender, EventArgs e)
        {
            bSpeakAttach = attachSpeaker(!bSpeakAttach);
            panSpeaker.BackColor = bSpeakAttach ? activeColor : deactiveColor;
        }

        private void panMic_Click(object sender, EventArgs e)
        {
            bMicAttach = attachMic(!bMicAttach);
            panMic.BackColor = bMicAttach ? activeColor : deactiveColor;
        }
    }
}
