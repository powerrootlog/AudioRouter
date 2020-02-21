namespace Denoise
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panSpeaker = new System.Windows.Forms.Panel();
            this.cmbSpeaker = new System.Windows.Forms.ComboBox();
            this.panMic = new System.Windows.Forms.Panel();
            this.cmbMic = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel1.SuspendLayout();
            this.panSpeaker.SuspendLayout();
            this.panMic.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.panSpeaker, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panMic, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(287, 397);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // panSpeaker
            // 
            this.panSpeaker.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panSpeaker.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panSpeaker.Controls.Add(this.cmbSpeaker);
            this.panSpeaker.Location = new System.Drawing.Point(3, 3);
            this.panSpeaker.Name = "panSpeaker";
            this.panSpeaker.Size = new System.Drawing.Size(281, 192);
            this.panSpeaker.TabIndex = 0;
            this.panSpeaker.Click += new System.EventHandler(this.panSpeaker_Click);
            // 
            // cmbSpeaker
            // 
            this.cmbSpeaker.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmbSpeaker.FormattingEnabled = true;
            this.cmbSpeaker.Location = new System.Drawing.Point(9, 107);
            this.cmbSpeaker.Name = "cmbSpeaker";
            this.cmbSpeaker.Size = new System.Drawing.Size(263, 24);
            this.cmbSpeaker.TabIndex = 0;
            // 
            // panMic
            // 
            this.panMic.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panMic.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.panMic.Controls.Add(this.cmbMic);
            this.panMic.Location = new System.Drawing.Point(3, 201);
            this.panMic.Name = "panMic";
            this.panMic.Size = new System.Drawing.Size(281, 193);
            this.panMic.TabIndex = 1;
            this.panMic.Click += new System.EventHandler(this.panMic_Click);
            // 
            // cmbMic
            // 
            this.cmbMic.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmbMic.FormattingEnabled = true;
            this.cmbMic.Location = new System.Drawing.Point(9, 121);
            this.cmbMic.Name = "cmbMic";
            this.cmbMic.Size = new System.Drawing.Size(263, 24);
            this.cmbMic.TabIndex = 0;
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 397);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "frmMain";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.panSpeaker.ResumeLayout(false);
            this.panMic.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panSpeaker;
        private System.Windows.Forms.Panel panMic;
        private System.Windows.Forms.ComboBox cmbSpeaker;
        private System.Windows.Forms.ComboBox cmbMic;
    }
}

