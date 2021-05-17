
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using TwinCAT.Ads;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace adsClientConsole
{
    class Program
    {
        // Haptic
        static public byte HapticEnable = 0;
        static public byte POPCEnable = 0;
        static public Int32 popc_log_cnt = 0;
        static public Int32 Packet_Limit = 100;

        // ADS Connection Client
        static public TcAdsClient _tcClient;
        static public AdsStream adsWriteStream;
        static public AdsStream adsReadStream;
        static public AdsStream adsPOPCWriteStream;
        static public string AdsPort = "0x8003";
        static public string NetId = "127.0.0.1.1.1";

        static public bool ADS_Thread_start = false;
        static public bool UDP_Thread_start = false;

        static public bool motion_enable = false;
        
        static public bool stop_flag = false;
        static public bool m_bPC = false;
        static public bool m_bSG = false;
        static public bool m_logging_flag = false;
        static public int m_StiffnessMode = 0;
        static public int m_joint_index = 0;
        static public bool m_UpdateData = false;
        static public int listenPort = 5000;

        static public DateTime m_Dt;

        // UDP Connnection Server
        static public bool UDP_connect_start = false;

        static public UdpClient srv;

        static public UDPServer_SEND UDP_Server_Data;
        static public UDPServer_SEND UDP_Server_Data_cur;
        static public UDPClient_RECV UDP_Client_Data;

        static public string ipaddress = "192.168.10.50";
        static public IPEndPoint endpoint;

        // Thread define
        static public Thread ADSThread;
        static public Thread UDPThread_recv;
        static public Thread UDPThread_send;

        static public int BUTTON_WINDOW_SIZE = 30000;

        static public ushort m_button_left_prev = 0;
        static public int m_button_left_counter = 0;
        static public ushort m_button_left_filtered = 0;

        static public ushort m_button_right_prev = 0;
        static public int m_button_right_counter = 0;
        static public ushort m_button_right_filtered = 0;

        // Logging Count
        static public Int32 log_counter = 0;


        // Sending data Queue
        static public Queue<UDPServer_SEND> UDPsendDataQueue;

        // mutex
        static object lockObj;
        
        /////////////////////////////////////////////////////////
        //
        // * Data Structure
        // 
        /////////////////////////////////////////////////////////   
        public struct ParameterData
        {
            public float[] Left;
            public float[] right;
        }

        static public ParameterData Param;


        public struct SmartController_Setting
        {
            public float[] L_Buff1;
            public float[] L_Buff2;
            public float[] L_Buff3;
            public float[] L_Buff1J;
            public float[] L_Buff2J;
            public float[] L_Buff3J;
            public float[] R_Buff1;
            public float[] R_Buff2;
            public float[] R_Buff3;
            public float[] R_Buff1J;
            public float[] R_Buff2J;
            public float[] R_Buff3J;
        }

        static public SmartController_Setting Setting_param;

        //[StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct ADSClient_SystemData
        {
            //Position Mode Gain Value
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)]
            public float[] Pos_Pgain;

            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Pos_Igain;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Pos_Dgain;

            //Velocity Mode Gain Value
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Vel_Pgain;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Vel_Igain;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Vel_Dgain;

            //Torque Mode Gain Value
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Torq_Pgain;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Torq_Igain;
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Torq_Dgain;

            //Joint Efficient Const.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_EffiCont;
            //Joint Torque Const.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_TorqueConst;
            //Joint Spring Const.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_SpringConst;

            //Joint Target Position.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_TargetPoistion;
            //Joint Target Time.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_TargetTime;
            //Joint Accel TIme.
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] J_AccelTime;

            public float[] CMD_BufferL1;
            public float[] CMD_BufferL2;
            public float[] CMD_BufferL3;
            public float[] CMD_BufferL4;
            public float[] CMD_BufferL5;
            public float[] CMD_BufferL6;

            public float[] CMD_BufferR1;
            public float[] CMD_BufferR2;
            public float[] CMD_BufferR3;
            public float[] CMD_BufferR4;
            public float[] CMD_BufferR5;
            public float[] CMD_BufferR6;


            //Set Joint Mode
            //public bool SetJointModeSelect;
            //public bool SetCartesianModeSelect;




        };


        static public ADSClient_SystemData ADS_ClientData;

        /////////////////////////////////////////////////////////
        //
        // * ADS Server -> ADS Client Data
        // 
        /////////////////////////////////////////////////////////       

        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct AXIS_DATA
        {
            
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] actual_motor_position;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] actual_link_position;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] actual_motor_velocity;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] actual_link_velocity;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] actual_current;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] target_position;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] target_velocity;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] target_current;
            
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public Int32[] mode_of_operation;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public Int32[] status_word;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] cartesian_target_pose;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] cartesian_current_pose;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] cartesian_current_orientation;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] energy;

            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] E_slave_in;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] E_master_out_origin;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] E_master_out;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Fm;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] F_modified;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] Vm;

            //public ushort m_button_left;
            public ushort m_button_left_filtered;
            //public ushort m_button_right;
            public ushort m_button_right_filtered;

        };


        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct ADSServer_SystemData
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
            public Int32[] m_Cnt;
            public Int32 m_log_cnt;
            public Int32 m_cycle_sec;
            public Int32 m_cycle_min;
            public Int32 m_cycle_hour;
            public Int32 Gravity_Mode;
            public Int32 TorquePosition_Mode;
            public Int32 target_reached;
            public Int32 m_dSystemMode;// 0: Gravity mode, 1:Joint Mode, 2:Cartesian Mode
            
            // Position Mode
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] accel_time;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] target_reach_time;
            
            // Test Buffer Data
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] TestBuffer1;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] TestBuffer2;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public float[] TestBuffer3;
            
            // Module Data
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
            public AXIS_DATA[] ModuleData_Left;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
            public AXIS_DATA[] ModuleData_Right;
        };


        static public ADSServer_SystemData ADS_ServerData;

        // UDP Connect
        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct Controller_struct
        {
            public float x;
            public float y;
            public float z;
            public float m_energy_x;
            public float m_energy_y;
            public float m_energy_z;
            public float roll;
            public float pitch;
            public float yaw;
            public ushort button;
            public byte popc_state;
        };
        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct UDPServer_SEND
        {
            public byte Index1;
            public byte Index2;
            public Controller_struct left;
            public Controller_struct right;

        };


        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct Machine_struct
        {
            public float force_x;
            public float force_y;
            public float force_z;
            public float s_energy_x;
            public float s_energy_y;
            public float s_energy_z;

        };
        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct UDPClient_RECV
        {
            public byte Index1;
            public byte Index2;
            public Machine_struct left;
            public Machine_struct right;
        };


        [StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
        public struct ADSPOPC_SystemData
        {
            public Int32 log_cnt;

            public Machine_struct left;
            public Machine_struct right;
        };
        static public ADSPOPC_SystemData ADSpopc_ClientData;

        public static void logging_start()
        {

            FileInfo f_left_popc = new FileInfo("logging_left_popc.csv");
            FileInfo f_right_popc = new FileInfo("logging_right_popc.csv");

            StreamWriter write_left_popc = f_left_popc.CreateText();
            StreamWriter write_right_popc = f_right_popc.CreateText();

            write_left_popc.WriteLine(" ");
            write_right_popc.WriteLine(" ");


            write_left_popc.Close();
            write_right_popc.Close();

            m_logging_flag = true;
        }

        public static void logging_stop()
        {
            m_logging_flag = false;
        }

        public static byte[] StructToByte(object obj)
        {
            int size = Marshal.SizeOf(obj);
            byte[] arr = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(obj, ptr, true);
            Marshal.Copy(ptr, arr, 0, size);
            Marshal.FreeHGlobal(ptr);
            return arr;

        }
        public static T ByteToStruct<T>(byte[] buffer) where T : struct
        {

            int size = Marshal.SizeOf(typeof(T));
            if (size > buffer.Length)
            {
                throw new Exception();
            }

            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, 0, ptr, size);
            T obj = (T)Marshal.PtrToStructure(ptr, typeof(T));
            Marshal.FreeHGlobal(ptr);
            return obj;
        }

        public static void ADS_Connect()
        {
            AmsAddress serverAddress = null;
            try
            {
                // check if port is a hex value
                if (AdsPort.StartsWith(("0x")) || AdsPort.StartsWith(("0X")))
                {
                    string sHexValue = AdsPort.Substring(2);
                    serverAddress = new AmsAddress(NetId, Int32.Parse(sHexValue, System.Globalization.NumberStyles.HexNumber));
                }
            }
            catch
            {
                Console.WriteLine("Invalid AMS NetId or Ams port");
                return;
            }

            try
            {
                _tcClient.Connect(serverAddress.NetId, serverAddress.Port);
                Console.WriteLine("Ads Client Connect Complete");
            }
            catch
            {
                Console.WriteLine("Could not connect client");
            }
            
        }

        public static void Parameter_Setting_Right()
        {
            /*///////////////////////////////////////////////
            * 
            * Right Side Gravity n Friction Param
            * 
            *///////////////////////////////////////////////

            /////////////////////////////////////////////////////////
            //
            // * Joint 1 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 0;

            AdsBinaryWriter ADS_Writer = new AdsBinaryWriter(adsWriteStream);
            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            //Setting_param.R_Buff1[0] = Param.right[0];
            ADS_Writer.Write(Param.right[0]);
            _tcClient.ReadWrite(0x4, 0x13, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            //Setting_param.R_Buff1[1] = Param.right[1];
            ADS_Writer.Write(Param.right[1]);
            _tcClient.ReadWrite(0x4, 0x14, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[2]);
            _tcClient.ReadWrite(0x4, 0x15, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 2 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 1;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[3]);
            _tcClient.ReadWrite(0x4, 0x13, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[4]);
            _tcClient.ReadWrite(0x4, 0x14, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[5]);
            _tcClient.ReadWrite(0x4, 0x15, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 3 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 2;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[6]);
            _tcClient.ReadWrite(0x4, 0x13, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[7]);
            _tcClient.ReadWrite(0x4, 0x14, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            ADS_Writer.Write(Param.right[8]);
            _tcClient.ReadWrite(0x4, 0x15, adsReadStream, adsWriteStream);


            /*///////////////////////////////////////////////
            * 
            * Right side Friction Param2
            * 
            *///////////////////////////////////////////////

            /////////////////////////////////////////////////////////
            //
            // * Joint 1 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 0;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff1J[0] = 0.0f;
            ADS_Writer.Write(Param.right[9]);
            _tcClient.ReadWrite(0x4, 0x24, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff2J[0] = 0.0f;
            ADS_Writer.Write(Param.right[10]);
            _tcClient.ReadWrite(0x4, 0x25, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff3J[0] = 0.0f;
            ADS_Writer.Write(Param.right[11]);
            _tcClient.ReadWrite(0x4, 0x26, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 2 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 1;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff1J[1] = 0.0f;
            ADS_Writer.Write(Param.right[12]);
            _tcClient.ReadWrite(0x4, 0x24, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff2J[1] = 0.0f;
            ADS_Writer.Write(Param.right[13]);
            _tcClient.ReadWrite(0x4, 0x25, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff3J[1] = 0.0f;
            ADS_Writer.Write(Param.right[14]);
            _tcClient.ReadWrite(0x4, 0x26, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 3 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 2;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff1J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.R_Buff1J[2]);
            _tcClient.ReadWrite(0x4, 0x24, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff2J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.R_Buff2J[2]);
            _tcClient.ReadWrite(0x4, 0x25, adsReadStream, adsWriteStream);

            // 
            adsWriteStream.Position = 0;
            Setting_param.R_Buff3J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.R_Buff3J[2]);
            _tcClient.ReadWrite(0x4, 0x26, adsReadStream, adsWriteStream);
        
        }
        
        public static void Parameter_Setting_Left()
        {

            /*///////////////////////////////////////////////
            * 
            * Left Side Gravity n Friction Param
            * 
            *///////////////////////////////////////////////

            
            /////////////////////////////////////////////////////////
            //
            // * Joint 1 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 0;

            AdsBinaryWriter ADS_Writer = new AdsBinaryWriter(adsWriteStream);
            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer1
            adsWriteStream.Position = 0;
            // Buff11
            Setting_param.R_Buff1[0] = 0.0f;
            ADS_Writer.Write(Param.Left[0]);
            _tcClient.ReadWrite(0x4, 0x16, adsReadStream, adsWriteStream);

            // *Joint Buffer2
            adsWriteStream.Position = 0;
            // Buff12
            Setting_param.R_Buff1[1] = 0.0f;
            ADS_Writer.Write(Param.Left[1]);
            _tcClient.ReadWrite(0x4, 0x17, adsReadStream, adsWriteStream);

            // *Joint Buffer3
            adsWriteStream.Position = 0;
            // Buff13
            Setting_param.R_Buff1[2] = 0.0f;
            ADS_Writer.Write(Param.Left[2]);
            _tcClient.ReadWrite(0x4, 0x18, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 2 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 1;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer1
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2[0] = 0.0f;
            ADS_Writer.Write(Param.Left[3]);
            _tcClient.ReadWrite(0x4, 0x16, adsReadStream, adsWriteStream);

            // *Joint Buffer2
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2[1] = 0.0f;
            ADS_Writer.Write(Param.Left[4]);
            _tcClient.ReadWrite(0x4, 0x17, adsReadStream, adsWriteStream);

            // *Joint Buffer3
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2[2] = 0.0f;
            ADS_Writer.Write(Param.Left[5]);
            _tcClient.ReadWrite(0x4, 0x18, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 3 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 2;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer1
            adsWriteStream.Position = 0;
            Setting_param.L_Buff3[0] = 0.0f;
            ADS_Writer.Write(Param.Left[6]);
            _tcClient.ReadWrite(0x4, 0x16, adsReadStream, adsWriteStream);

            // *Joint Buffer2
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2[1] = 0.0f;
            ADS_Writer.Write(Param.Left[7]);
            _tcClient.ReadWrite(0x4, 0x17, adsReadStream, adsWriteStream);

            // *Joint Buffer3
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2[2] = 0.0f;
            ADS_Writer.Write(Param.Left[8]);
            _tcClient.ReadWrite(0x4, 0x18, adsReadStream, adsWriteStream);


            /*///////////////////////////////////////////////
            * 
            * Left side Friction Param2
            * 
            *///////////////////////////////////////////////
            

            /////////////////////////////////////////////////////////
            //
            // * Joint 1 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 0;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer4
            adsWriteStream.Position = 0;
            Setting_param.L_Buff1J[0] = 0.0f;
            ADS_Writer.Write(Param.Left[9]);
            _tcClient.ReadWrite(0x4, 0x27, adsReadStream, adsWriteStream);

            // *Joint Buffer5
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2J[0] = 0.0f;
            ADS_Writer.Write(Param.Left[10]);
            _tcClient.ReadWrite(0x4, 0x28, adsReadStream, adsWriteStream);

            // *Joint Buffer6
            adsWriteStream.Position = 0;
            Setting_param.L_Buff3J[0] = 0.0f;
            ADS_Writer.Write(Param.Left[11]);
            _tcClient.ReadWrite(0x4, 0x29, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 2 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 1;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer1
            adsWriteStream.Position = 0;
            Setting_param.L_Buff1J[1] = 0.0f;
            ADS_Writer.Write(Param.Left[12]);
            _tcClient.ReadWrite(0x4, 0x27, adsReadStream, adsWriteStream);

            // *Joint Buffer2
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2J[1] = 0.0f;
            ADS_Writer.Write(Param.Left[13]);
            _tcClient.ReadWrite(0x4, 0x28, adsReadStream, adsWriteStream);

            // *Joint Buffer3
            adsWriteStream.Position = 0;
            Setting_param.L_Buff3J[1] = 0.0f;
            ADS_Writer.Write(Param.Left[14]);
            _tcClient.ReadWrite(0x4, 0x29, adsReadStream, adsWriteStream);

            /////////////////////////////////////////////////////////
            //
            // * Joint 3 Axis
            // 
            ////////////////////////////////////////////////////////    
            m_joint_index = 2;

            adsWriteStream.Position = 0;
            ADS_Writer.Write(m_joint_index);//int.Parse(tbPy.Text));
            _tcClient.ReadWrite(0x4, 0x9, adsReadStream, adsWriteStream);

            // *Joint Buffer1
            adsWriteStream.Position = 0;
            Setting_param.L_Buff1J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.L_Buff1J[2]);
            _tcClient.ReadWrite(0x4, 0x27, adsReadStream, adsWriteStream);

            // *Joint Buffer2
            adsWriteStream.Position = 0;
            Setting_param.L_Buff2J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.L_Buff2J[2]);
            _tcClient.ReadWrite(0x4, 0x28, adsReadStream, adsWriteStream);

            // *Joint Buffer3
            adsWriteStream.Position = 0;
            Setting_param.L_Buff3J[2] = 0.0f;
            ADS_Writer.Write(Setting_param.L_Buff3J[2]);
            _tcClient.ReadWrite(0x4, 0x29, adsReadStream, adsWriteStream);


        }

        public static void Ready()
        {
            try
            {
                _tcClient.ReadWrite(0x4, 0x5, adsReadStream, adsWriteStream);
            }
            catch (Exception err)
            {
                Console.WriteLine(err.Message);
            }
        }

        public static void ServoOn()
        {
            try
            {
                AdsBinaryWriter binWriter = new AdsBinaryWriter(adsWriteStream);
                adsWriteStream.Position = 0;

                _tcClient.ReadWrite(0x4, 0x1, adsReadStream, adsWriteStream);
                ADS_Thread_start = true;
                m_UpdateData = true;
            }
            catch (Exception err)
            {
                Console.WriteLine(err.Message);
            }
        }

        public static void CartesianMode()
        {
            try
            {
                if (POPCEnable == 0) { POPCEnable = 1; }
                else{ POPCEnable = 0;}

                //Set Cartesian Mode
                _tcClient.ReadWrite(0x4, 0x20, adsReadStream, adsWriteStream);
            }
            catch (Exception err)
            {
                Console.WriteLine(err.Message);
            }
        }

        public static void HapticMode()
        {
            try
            {
                AdsBinaryWriter ADS_Writer = new AdsBinaryWriter(adsWriteStream);
                adsWriteStream.Position = 0;
                Int32 n = 1;
                ADS_Writer.Write(n);//int.Parse(tbPy.Text));
                _tcClient.ReadWrite(0x4, 0x31, adsReadStream, adsWriteStream);

                HapticEnable = 1;
            }
            catch(Exception err)
            {
                Console.WriteLine(err.Message);
            }
        }

        public static void UDP_Connect()
        {
            srv = new UdpClient(listenPort);
            endpoint = new IPEndPoint(IPAddress.Any, listenPort);
            UDP_Thread_start = true;

            UDPThread_recv = new Thread(new ThreadStart(UDP_Thread_recv));
            UDPThread_recv.Start();

            UDPThread_send = new Thread(new ThreadStart(UDP_Thread_send));
            UDPThread_send.Start();

            Console.WriteLine("Ok");
            
        }

        public static void Param_data_read()
        {
            BinaryReader left_br = new BinaryReader(File.Open(@"../../../leftControllerParam.txt", FileMode.Open));
            BinaryReader right_br = new BinaryReader(File.Open(@"../../../rightControllerParam.txt", FileMode.Open));
            int i = 0;
            while(true)
            {
                try
                {
                    Param.Left[i] = left_br.ReadSingle();
                    Param.right[i] = right_br.ReadSingle();
                    i++;
                }
                catch(EndOfStreamException e)
                {
                    left_br.Close();
                    right_br.Close();
                    
                    Console.WriteLine("Left");
                    for (int j = 0; j < 15; j++)
                    {
                        Console.WriteLine(Param.Left[j]);
                    }
                    Console.WriteLine();
                    Console.WriteLine("Right");
                    for (int j = 0; j < 15; j++)
                    {
                        Console.WriteLine(Param.right[j]);
                    }
                    Console.WriteLine();
                    
                    break;

                }
            }
        }

        static void Main(string[] args)
        {

            // Create New TcClient instance          
            _tcClient = new TcAdsClient();

            ADS_ServerData = new ADSServer_SystemData();
            ADS_ClientData = new ADSClient_SystemData();

            Setting_param = new SmartController_Setting();

            // ClientData Initalize
            ADS_ClientData.J_AccelTime = new float[3];
            ADS_ClientData.J_EffiCont = new float[3];
            ADS_ClientData.J_SpringConst = new float[3];
            ADS_ClientData.J_TargetPoistion = new float[3];
            ADS_ClientData.J_TargetTime = new float[3];
            ADS_ClientData.J_TorqueConst = new float[3];
            ADS_ClientData.Pos_Dgain = new float[3];
            ADS_ClientData.Pos_Igain = new float[3];
            ADS_ClientData.Pos_Pgain = new float[3];
            ADS_ClientData.Torq_Dgain = new float[3];
            ADS_ClientData.Torq_Igain = new float[3];
            ADS_ClientData.Torq_Pgain = new float[3];
            ADS_ClientData.Vel_Dgain = new float[3];
            ADS_ClientData.Vel_Igain = new float[3];
            ADS_ClientData.Vel_Pgain = new float[3];

            ADS_ClientData.CMD_BufferL1 = new float[3];
            ADS_ClientData.CMD_BufferL2 = new float[3];
            ADS_ClientData.CMD_BufferL3 = new float[3];
            ADS_ClientData.CMD_BufferL4 = new float[3];
            ADS_ClientData.CMD_BufferL5 = new float[3];
            ADS_ClientData.CMD_BufferL6 = new float[3];

            ADS_ClientData.CMD_BufferR1 = new float[3];
            ADS_ClientData.CMD_BufferR2 = new float[3];
            ADS_ClientData.CMD_BufferR3 = new float[3];
            ADS_ClientData.CMD_BufferR4 = new float[3];
            ADS_ClientData.CMD_BufferR5 = new float[3];
            ADS_ClientData.CMD_BufferR6 = new float[3];

            // Setting Parameter Initialize
            Setting_param.L_Buff1 = new float[3];
            Setting_param.L_Buff2 = new float[3];
            Setting_param.L_Buff3 = new float[3];
            Setting_param.L_Buff1J = new float[3];
            Setting_param.L_Buff2J = new float[3];
            Setting_param.L_Buff3J = new float[3];

            Setting_param.R_Buff1 = new float[3];
            Setting_param.R_Buff2 = new float[3];
            Setting_param.R_Buff3 = new float[3];
            Setting_param.R_Buff1J = new float[3];
            Setting_param.R_Buff2J = new float[3];
            Setting_param.R_Buff3J = new float[3];

            UDP_Server_Data = new UDPServer_SEND();
            UDP_Server_Data_cur = new UDPServer_SEND();
            UDP_Client_Data = new UDPClient_RECV();

            UDPsendDataQueue = new Queue<UDPServer_SEND>();
            lockObj = new object();

            Param = new ParameterData();
            Param.Left = new float[15];
            Param.right = new float[15];

            ADSpopc_ClientData = new ADSPOPC_SystemData();

            adsReadStream = new AdsStream(Marshal.SizeOf(typeof(ADSServer_SystemData)));
            adsPOPCWriteStream = new AdsStream(Marshal.SizeOf(typeof(ADSPOPC_SystemData)));
            adsWriteStream = new AdsStream(4);
            
            Console.WriteLine("Setting -- Start");
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine();
            Console.Write("ADS Communication ---------- ");
            ADS_Connect();
            ADS_Thread_start = true;
            Thread.Sleep(300);
            Console.WriteLine("Ok");
            
            Console.Write("Servo On ");
            ServoOn();
            Thread.Sleep(1000);
            Console.Write("---");
            ServoOn();
            Thread.Sleep(1000);
            Console.Write("---");
            ServoOn();
            Thread.Sleep(3000);
            Console.WriteLine("---- Ok");

            Console.Write("Parameter Tunning Read");
            Param_data_read();
            Console.WriteLine("---------- Ok");

            Console.Write("Left & Right Arm Parameter Setting ----------");
            Parameter_Setting_Right();
            Thread.Sleep(100);
            Parameter_Setting_Left();
            Thread.Sleep(100);
            Console.WriteLine("Ok");
            
            
            Console.Write("Ready Pose Setting");
            Ready();
            Console.WriteLine("---------- Ok"); 
            

            ADSThread = new Thread(new ThreadStart(ADS_Thread));
            ADSThread.Start();

            Console.Write("UDP Communication ---------- ");
            UDP_Connect();
            Console.WriteLine("Ok");

            Console.Write("Cartesian Mode ---------- ");
            CartesianMode();
            Console.WriteLine("Ok");

            Console.Write("logging Mode ---------- start");
            logging_start();

            //Console.Write("Haptic Mode ---------- ");
            //HapticMode();
            //Console.WriteLine("Ok");

            // Thread
            ADSThread.Join();
            UDPThread_recv.Join();
            UDPThread_send.Join();

            

        }

        static void ADS_Thread()
        {
            Console.WriteLine("ADS");
            while (true)
            {
                try
                {
                    
                    //Stopwatch sw = new Stopwatch();
                    //sw.Start();
                    
                    
                    if (ADS_Thread_start == true)
                    {

                            /////////////////////////////////////////////////////////
                            //
                            // * ADS Server -> ADS Client Data
                            // 
                            /////////////////////////////////////////////////////////
                            _tcClient.ReadWrite(0x4, 0x4, adsReadStream, adsWriteStream);
                            byte[] read_stream_buffer = adsReadStream.ToArray();
                            GCHandle r_handle = GCHandle.Alloc(read_stream_buffer, GCHandleType.Pinned);     // Alloc(들어갈 byte 배열, 핸들값)
                            ADS_ServerData = (ADSServer_SystemData)Marshal.PtrToStructure(r_handle.AddrOfPinnedObject(), typeof(ADSServer_SystemData));
                            r_handle.Free();


                            for (int i = 0; i <= ADS_ServerData.m_log_cnt; i++)
                            {

                                
                                if (UDPsendDataQueue.Count > 300)
                                {
                                    Console.WriteLine("Queue Max size over " + UDPsendDataQueue.Count);
                                    UDPsendDataQueue.Clear();
                                }

                                
                                UDP_Server_Data_cur.left.x = ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[0];
                                UDP_Server_Data_cur.left.y = ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[1];
                                UDP_Server_Data_cur.left.z = ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[2];
                                UDP_Server_Data_cur.left.roll = ADS_ServerData.ModuleData_Left[i].cartesian_current_orientation[0];
                                UDP_Server_Data_cur.left.pitch = ADS_ServerData.ModuleData_Left[i].cartesian_current_orientation[1];
                                UDP_Server_Data_cur.left.yaw = ADS_ServerData.ModuleData_Left[i].cartesian_current_orientation[2];
                                //UDP_Server_Data_cur.left.button = Convert.ToUInt16(ADS_ServerData.ModuleData_Left[i].m_button_left);
                                UDP_Server_Data_cur.left.button = ADS_ServerData.ModuleData_Left[i].m_button_left_filtered;
                                //UDP_Server_Data_cur.left.button = 0;
                                UDP_Server_Data_cur.left.popc_state = POPCEnable;
                                UDP_Server_Data_cur.left.m_energy_x = ADS_ServerData.ModuleData_Left[i].energy[0];
                                UDP_Server_Data_cur.left.m_energy_y = ADS_ServerData.ModuleData_Left[i].energy[1];
                                UDP_Server_Data_cur.left.m_energy_z = ADS_ServerData.ModuleData_Left[i].energy[2];

                                UDP_Server_Data_cur.right.x = ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[0];
                                UDP_Server_Data_cur.right.y = ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[1];
                                UDP_Server_Data_cur.right.z = ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[2];
                                UDP_Server_Data_cur.right.roll = ADS_ServerData.ModuleData_Right[i].cartesian_current_orientation[0];
                                UDP_Server_Data_cur.right.pitch = ADS_ServerData.ModuleData_Right[i].cartesian_current_orientation[1];
                                UDP_Server_Data_cur.right.yaw = ADS_ServerData.ModuleData_Right[i].cartesian_current_orientation[2];
                                //UDP_Server_Data_cur.right.button = Convert.ToUInt16(ADS_ServerData.ModuleData_Right[i].m_button_right);
                                UDP_Server_Data_cur.right.button = ADS_ServerData.ModuleData_Right[i].m_button_right_filtered;
                                //UDP_Server_Data_cur.right.button = 0;
                                UDP_Server_Data_cur.right.popc_state = POPCEnable;
                                UDP_Server_Data_cur.right.m_energy_x = ADS_ServerData.ModuleData_Right[i].energy[0];
                                UDP_Server_Data_cur.right.m_energy_y = ADS_ServerData.ModuleData_Right[i].energy[1];
                                UDP_Server_Data_cur.right.m_energy_z = ADS_ServerData.ModuleData_Right[i].energy[2];
                                
                                //Console.WriteLine("left :  " + UDP_Server_Data_cur.left.x + " " + UDP_Server_Data_cur.left.y + " " + UDP_Server_Data_cur.left.z + " ");
                                
                                lock (lockObj)
                                {
                                    //Console.WriteLine("right : " + UDP_Server_Data_cur.right.x + " " + UDP_Server_Data_cur.right.y + " " + UDP_Server_Data_cur.right.z + " ");
                                    UDPsendDataQueue.Enqueue(UDP_Server_Data_cur);
                                    Monitor.Pulse(lockObj);
                            
                                }
                         }


                        if (m_logging_flag)
                        {

                            //string left_str;
                            //string right_str;

                            string left_popc_str;
                            string right_popc_str;
                            /*
                            FileStream fs_left = new FileStream("logging_left.csv", FileMode.Append, FileAccess.Write);
                            FileStream fs_right = new FileStream("logging_right.csv", FileMode.Append, FileAccess.Write);
                            */
                            FileStream popc_left = new FileStream("logging_left_popc.csv", FileMode.Append, FileAccess.Write);
                            FileStream popc_right = new FileStream("logging_right_popc.csv", FileMode.Append, FileAccess.Write);
                            /*
                            StreamWriter write_left = new StreamWriter(fs_left, System.Text.Encoding.UTF8);
                            StreamWriter write_right = new StreamWriter(fs_right, System.Text.Encoding.UTF8);
                            */
                            StreamWriter write_left_popc = new StreamWriter(popc_left, System.Text.Encoding.UTF8);
                            StreamWriter write_right_popc = new StreamWriter(popc_right, System.Text.Encoding.UTF8);


                            for (int i = 0; i <= ADS_ServerData.m_log_cnt; i++)
                            {
                                /*
                                left_str = String.Format("{0:d},{1:0.000000},{2:0.000000},{3:0.000000},{4:0.000000},{5:0.000000},{6:0.000000},{7:0.000000},{8:0.000000},{9:0.000000},{10:0.000000},{11:0.000000},{12:0.000000},{13:0.000000},{14:0.000000},{15:0.000000},{16:0.000000},{17:0.000000},{18:0.000000},{19:0.000000},{20:0.000000},{21:0.000000},{22:0.000000},{23:0.000000},{24:0.000000},{25:0.000000},{26:0.000000},{27:0.000000},{28:0.000000},{29:0.000000},{30:0.000000},{31:0.000000},{32:0.000000},{33:0.000000}", ADS_ServerData.m_Cnt[i],
                                    ADS_ServerData.ModuleData_Left[i].actual_motor_position[0], ADS_ServerData.ModuleData_Left[i].actual_link_position[0], ADS_ServerData.ModuleData_Left[i].target_position[0], ADS_ServerData.ModuleData_Left[i].actual_motor_velocity[0], ADS_ServerData.ModuleData_Left[i].actual_link_velocity[0], ADS_ServerData.ModuleData_Left[i].target_current[0], ADS_ServerData.ModuleData_Left[i].actual_current[0], ADS_ServerData.accel_time[0], ADS_ServerData.target_reach_time[0], ADS_ServerData.ModuleData_Left[i].cartesian_target_pose[0], ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[0],
                                    ADS_ServerData.ModuleData_Left[i].actual_motor_position[1], ADS_ServerData.ModuleData_Left[i].actual_link_position[1], ADS_ServerData.ModuleData_Left[i].target_position[1], ADS_ServerData.ModuleData_Left[i].actual_motor_velocity[1], ADS_ServerData.ModuleData_Left[i].actual_link_velocity[1], ADS_ServerData.ModuleData_Left[i].target_current[1], ADS_ServerData.ModuleData_Left[i].actual_current[1], ADS_ServerData.accel_time[1], ADS_ServerData.target_reach_time[1], ADS_ServerData.ModuleData_Left[i].cartesian_target_pose[1], ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[1],
                                    ADS_ServerData.ModuleData_Left[i].actual_motor_position[2], ADS_ServerData.ModuleData_Left[i].actual_link_position[2], ADS_ServerData.ModuleData_Left[i].target_position[2], ADS_ServerData.ModuleData_Left[i].actual_motor_velocity[2], ADS_ServerData.ModuleData_Left[i].actual_link_velocity[2], ADS_ServerData.ModuleData_Left[i].target_current[2], ADS_ServerData.ModuleData_Left[i].actual_current[2], ADS_ServerData.accel_time[2], ADS_ServerData.target_reach_time[2], ADS_ServerData.ModuleData_Left[i].cartesian_target_pose[2], ADS_ServerData.ModuleData_Left[i].cartesian_current_pose[2]);

                                right_str = String.Format("{0:d},{1:0.000000},{2:0.000000},{3:0.000000},{4:0.000000},{5:0.000000},{6:0.000000},{7:0.000000},{8:0.000000},{9:0.000000},{10:0.000000},{11:0.000000},{12:0.000000},{13:0.000000},{14:0.000000},{15:0.000000},{16:0.000000},{17:0.000000},{18:0.000000},{19:0.000000},{20:0.000000},{21:0.000000},{22:0.000000},{23:0.000000},{24:0.000000},{25:0.000000},{26:0.000000},{27:0.000000},{28:0.000000},{29:0.000000},{30:0.000000},{31:0.000000},{32:0.000000},{33:0.000000}", ADS_ServerData.m_Cnt[i],
                                    ADS_ServerData.ModuleData_Right[i].actual_motor_position[0], ADS_ServerData.ModuleData_Right[i].actual_link_position[0], ADS_ServerData.ModuleData_Right[i].target_position[0], ADS_ServerData.ModuleData_Right[i].actual_motor_velocity[0], ADS_ServerData.ModuleData_Right[i].actual_link_velocity[0], ADS_ServerData.ModuleData_Right[i].target_current[0], ADS_ServerData.ModuleData_Right[i].actual_current[0], ADS_ServerData.accel_time[0], ADS_ServerData.target_reach_time[0], ADS_ServerData.ModuleData_Right[i].cartesian_target_pose[0], ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[0],
                                    ADS_ServerData.ModuleData_Right[i].actual_motor_position[1], ADS_ServerData.ModuleData_Right[i].actual_link_position[1], ADS_ServerData.ModuleData_Right[i].target_position[1], ADS_ServerData.ModuleData_Right[i].actual_motor_velocity[1], ADS_ServerData.ModuleData_Right[i].actual_link_velocity[1], ADS_ServerData.ModuleData_Right[i].target_current[1], ADS_ServerData.ModuleData_Right[i].actual_current[1], ADS_ServerData.accel_time[1], ADS_ServerData.target_reach_time[1], ADS_ServerData.ModuleData_Right[i].cartesian_target_pose[1], ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[1],
                                    ADS_ServerData.ModuleData_Right[i].actual_motor_position[2], ADS_ServerData.ModuleData_Right[i].actual_link_position[2], ADS_ServerData.ModuleData_Right[i].target_position[2], ADS_ServerData.ModuleData_Right[i].actual_motor_velocity[2], ADS_ServerData.ModuleData_Right[i].actual_link_velocity[2], ADS_ServerData.ModuleData_Right[i].target_current[2], ADS_ServerData.ModuleData_Right[i].actual_current[2], ADS_ServerData.accel_time[2], ADS_ServerData.target_reach_time[2], ADS_ServerData.ModuleData_Right[i].cartesian_target_pose[2], ADS_ServerData.ModuleData_Right[i].cartesian_current_pose[2]);
                                */
                                left_popc_str = String.Format("{0:0.000000},{1:0.000000},{2:0.000000},{3:0.000000},{4:0.000000},{5:0.000000},{6:0.000000},{7:0.000000},{8:0.000000},{9:0.000000},{10:0.000000},{11:0.000000},{12:0.000000},{13:0.000000},{14:0.000000},{15:0.000000},{16:0.000000},{17:0.000000}",
                                    ADS_ServerData.ModuleData_Left[i].E_slave_in[0], ADS_ServerData.ModuleData_Left[i].E_master_out_origin[0], ADS_ServerData.ModuleData_Left[i].E_master_out[0], ADS_ServerData.ModuleData_Left[i].Fm[0], ADS_ServerData.ModuleData_Left[i].F_modified[0], ADS_ServerData.ModuleData_Left[i].Vm[0],
                                    ADS_ServerData.ModuleData_Left[i].E_slave_in[1], ADS_ServerData.ModuleData_Left[i].E_master_out_origin[1], ADS_ServerData.ModuleData_Left[i].E_master_out[1], ADS_ServerData.ModuleData_Left[i].Fm[1], ADS_ServerData.ModuleData_Left[i].F_modified[1], ADS_ServerData.ModuleData_Left[i].Vm[1],
                                    ADS_ServerData.ModuleData_Left[i].E_slave_in[2], ADS_ServerData.ModuleData_Left[i].E_master_out_origin[2], ADS_ServerData.ModuleData_Left[i].E_master_out[2], ADS_ServerData.ModuleData_Left[i].Fm[2], ADS_ServerData.ModuleData_Left[i].F_modified[2], ADS_ServerData.ModuleData_Left[i].Vm[2]);
                                write_left_popc.WriteLine(left_popc_str);

                                right_popc_str = String.Format("{0:0.000000},{1:0.000000},{2:0.000000},{3:0.000000},{4:0.000000},{5:0.000000},{6:0.000000},{7:0.000000},{8:0.000000},{9:0.000000},{10:0.000000},{11:0.000000},{12:0.000000},{13:0.000000},{14:0.000000},{15:0.000000},{16:0.000000},{17:0.000000}",
                                    ADS_ServerData.ModuleData_Right[i].E_slave_in[0], ADS_ServerData.ModuleData_Right[i].E_master_out_origin[0], ADS_ServerData.ModuleData_Right[i].E_master_out[0], ADS_ServerData.ModuleData_Right[i].Fm[0], ADS_ServerData.ModuleData_Right[i].F_modified[0], ADS_ServerData.ModuleData_Right[i].Vm[0],
                                    ADS_ServerData.ModuleData_Right[i].E_slave_in[1], ADS_ServerData.ModuleData_Right[i].E_master_out_origin[1], ADS_ServerData.ModuleData_Right[i].E_master_out[1], ADS_ServerData.ModuleData_Right[i].Fm[1], ADS_ServerData.ModuleData_Right[i].F_modified[1], ADS_ServerData.ModuleData_Right[i].Vm[1],
                                    ADS_ServerData.ModuleData_Right[i].E_slave_in[2], ADS_ServerData.ModuleData_Right[i].E_master_out_origin[2], ADS_ServerData.ModuleData_Right[i].E_master_out[2], ADS_ServerData.ModuleData_Right[i].Fm[2], ADS_ServerData.ModuleData_Right[i].F_modified[2], ADS_ServerData.ModuleData_Right[i].Vm[2]);
                                write_right_popc.WriteLine(right_popc_str);

                                /*
                                left_str = String.Format("{0:d},{1:d}", ADS_ServerData.ModuleData_Left[i].m_button_left, ADS_ServerData.ModuleData_Left[i].m_button_left_filtered);
                                right_str = String.Format("{0:d},{1:d}", ADS_ServerData.ModuleData_Right[i].m_button_right, ADS_ServerData.ModuleData_Right[i].m_button_right_filtered);
                                */

                                /*
                                write_left.WriteLine(left_str);
                                write_right.WriteLine(right_str);
                                 */
                            }
                            /*
                            write_left.Close();
                            write_right.Close();
                            */
                            write_left_popc.Close();
                            write_right_popc.Close();

                            log_counter++;
                            if(log_counter == BUTTON_WINDOW_SIZE)
                            {
                                Console.WriteLine("Logging Size Full, Stop Logging System !!!");
                                logging_stop();
                            }


                        }

                    }
                    
                }
                
                catch (Exception err)
                {
                    Console.WriteLine(err.Message);
                    //ServoOn();
                }
                
                
            }
            //ServoOn();
        }
        static public bool UDP_recv_complete = false;
        static void UDP_Thread_recv()
        {
            while (true)
            {
                try
                {
                    if (UDP_Thread_start == true)
                    {
                        /////////////////////////////////////////////////////////
                        //
                        // * UDP Server -> UDP Client Data
                        // 
                        /////////////////////////////////////////////////////////

                        // 데이터 수신
                        //IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0);
                        byte[] Rdatagram = srv.Receive(ref endpoint);
                        GCHandle handle = GCHandle.Alloc(Rdatagram, GCHandleType.Pinned);
                        UDP_Client_Data = (UDPClient_RECV)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(UDPClient_RECV));
                        handle.Free();
                        if (Rdatagram != null)
                        {
                            UDP_connect_start = true;
                            if (UDP_recv_complete == false)
                            {
                                Console.WriteLine("Ip adress : " + Convert.ToString(endpoint.Address) + " Port : " + Convert.ToString(endpoint.Port) + " Connect Complete");
                                UDPsendDataQueue.Clear();
                                UDP_recv_complete = true;
                            }
                        }
                        ADSpopc_ClientData.left.force_x = UDP_Client_Data.left.force_x;
                        ADSpopc_ClientData.left.force_y = UDP_Client_Data.left.force_y;
                        ADSpopc_ClientData.left.force_z = UDP_Client_Data.left.force_z;
                        ADSpopc_ClientData.left.s_energy_x = UDP_Client_Data.left.s_energy_x;
                        ADSpopc_ClientData.left.s_energy_y = UDP_Client_Data.left.s_energy_y;
                        ADSpopc_ClientData.left.s_energy_z = UDP_Client_Data.left.s_energy_z;


                        ADSpopc_ClientData.right.force_x = UDP_Client_Data.right.force_x;
                        ADSpopc_ClientData.right.force_y = UDP_Client_Data.right.force_y;
                        ADSpopc_ClientData.right.force_z = UDP_Client_Data.right.force_z;
                        ADSpopc_ClientData.right.s_energy_x = UDP_Client_Data.right.s_energy_x;
                        ADSpopc_ClientData.right.s_energy_y = UDP_Client_Data.right.s_energy_y;
                        ADSpopc_ClientData.right.s_energy_z = UDP_Client_Data.right.s_energy_z;



                        popc_log_cnt++;
                        ADSpopc_ClientData.log_cnt = popc_log_cnt;

                        /////////////////////////////////////////////////////////
                        //
                        // * ADS Client -> ADS Server Data
                        // 
                        /////////////////////////////////////////////////////////
                        AdsBinaryWriter ADSpopc_Writer = new AdsBinaryWriter(adsPOPCWriteStream);
                        adsPOPCWriteStream.Position = 0;
                        ADSpopc_Writer.Write(StructToByte(ADSpopc_ClientData));
                        _tcClient.ReadWrite(0x4, 0x33, adsReadStream, adsPOPCWriteStream);
                        popc_log_cnt = 0;


                        /*
                        /////////////////////////////////////////////////////////
                        //
                        // * UDP Client -> UDP Server Data
                        // 
                        /////////////////////////////////////////////////////////

                        // 데이터 송신
                        byte[] Sdatagram = StructToByte(UDP_Server_Data);
                        // Client
                        //srv.Send(Sdatagram, Sdatagram.Length, ipaddress, sendPort);
                        // Server
                        srv.Send(Sdatagram, Sdatagram.Length, endpoint);
                        */
                    }
                    //Thread.Sleep(1);

                }
                catch (Exception err)
                {
                    Console.WriteLine(err.ToString());
                }

            }
        }

        static void UDP_Thread_send()
        {
           
            while (true)
            {
                try
                {
                    //Console.WriteLine("UDP_recv");
                    if (UDP_connect_start == true)
                    {
                        /////////////////////////////////////////////////////////
                        //
                        // * UDP Client -> UDP Server Data
                        // 
                        /////////////////////////////////////////////////////////

                        lock (lockObj)
                        {
                            while(UDPsendDataQueue.Count == 0)
                            {
                               Monitor.Wait(lockObj);
                            }
                            
                            UDP_Server_Data = (UDPServer_SEND)UDPsendDataQueue.Dequeue();
                        }

                        // 데이터 송신
                        //IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, listenPort);
                        byte[] Sdatagram = StructToByte(UDP_Server_Data);
                        // Client
                        //srv.Send(Sdatagram, Sdatagram.Length, ipaddress, sendPort);
                        // Server
                        srv.Send(Sdatagram, Sdatagram.Length, endpoint);
                        
                    }
                    //Thread.Sleep(1);

                }
                catch (Exception err)
                {
                    Console.WriteLine(err.ToString());
                }

            }
        }

    }
}
